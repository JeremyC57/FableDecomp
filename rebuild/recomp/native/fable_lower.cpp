// fable_lower: turn the FullDecomp branch's byte-matched C++ (compiled by clang to 32-bit MSVC-ABI
// LLVM IR, i686-pc-windows-msvc) into native code for a 64-bit host (aarch64 or x86-64 Linux) that
// runs inside FableRecomp next to the lifted code and shares its memory.
//
// Why this works: FableRecomp maps guest memory 1:1 into the host's low 4 GiB
// (RECOMP_IDENTITY_MEMORY), so a guest pointer zero-extended is a valid host pointer. The IR from the
// 32-bit MSVC front end already has the retail layouts, vtables and calling conventions; this tool
// makes it 64-bit-safe:
//   - every GEP becomes a byte offset computed with the 32-bit data layout;
//   - pointers stored to or loaded from memory are 32-bit words (ptrtoint / inttoptr);
//   - allocas move to the guest stack (Ctx::esp), so their addresses are guest addresses;
//   - globals and functions bind to their retail addresses (rebuild/oracles/reloc_bindings.tsv on
//     FullDecomp): globals become absolute addresses, calls to functions that are not native in this
//     module (and every indirect call: vtables hold guest addresses) go through nd_call, which runs
//     the lifted code with the x86 arguments on the guest stack;
//   - each native function gets a guest-ABI entry nd_entry_<ADDR>(Ctx*) for the lifted code to call
//     (wrapped with the lifter's --wrap ADDR=nd_wrap_<ADDR>, which falls back to F_<ADDR>_orig).
// A function that uses something this cannot lower (inline asm, inalloca, varargs, dynamic allocas,
// unbound externals, pointer-holding aggregates in registers) is left out: it stays lifted.
//
// usage: fable_lower <in.ll|bc> <out.bc> --bindings reloc_bindings.tsv --natives natives.tsv [--exclude addrs.txt]
//                    --triple aarch64-linux-gnu|x86_64-linux-gnu [--report report.tsv]
// natives.tsv: "<hex address>\t<IR function name>" per landed function.
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/AttributeMask.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include "llvm/TargetParser/Triple.h"

#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace llvm;

namespace {

struct Options {
    std::string in, out, bindings, natives, triple, report, exclude, privateCc;
};

// ---------------------------------------------------------------- symbol names / bindings
std::map<std::string, uint32_t> g_bind;      // COFF symbol -> retail address
std::map<uint32_t, std::string> g_natives;  // address -> IR name (landed functions)
const DataLayout* g_dl32 = nullptr;          // the i686 layout, for every size and offset
// Retail functions with a private (LTCG) register convention, e.g. an argument in eax: the IR's
// signature cannot say so, so they are never lowered and never called through a bridge.
std::set<uint32_t> g_privateCc;

uint32_t stackBytes(const DataLayout& dl, Type* t) { return (static_cast<uint32_t>(dl.getTypeAllocSize(t)) + 3u) & ~3u; }

// The COFF symbol of a global as VC7.1 / clang name it for i686 MSVC.
std::string coffName(const GlobalValue& gv) {
    StringRef n = gv.getName();
    if (n.starts_with("\x01")) return n.drop_front().str();
    if (n.starts_with("?")) return n.str();
    if (auto* f = dyn_cast<Function>(&gv)) {
        uint32_t bytes = 0;
        for (Type* t : f->getFunctionType()->params()) bytes += stackBytes(*g_dl32, t);
        switch (f->getCallingConv()) {
            case CallingConv::X86_StdCall: return "_" + n.str() + "@" + std::to_string(bytes);
            case CallingConv::X86_FastCall: return "@" + n.str() + "@" + std::to_string(bytes);
            default: return "_" + n.str();
        }
    }
    return "_" + n.str();
}

bool bound(const GlobalValue& gv, uint32_t& addr) {
    std::string c = coffName(gv);
    if (gv.hasDLLImportStorageClass()) c = "__imp_" + c;
    auto it = g_bind.find(c);
    if (it == g_bind.end()) return false;
    addr = it->second;
    return true;
}

// host libc functions native code may call directly on guest pointers
bool hostSafe(StringRef n) {
    static const std::set<std::string> ok = {"memcpy", "memmove", "memset", "memcmp", "strlen", "strcmp", "strncmp", "strcpy", "strncpy",
                                             "strcat", "strchr", "strrchr", "strstr", "sqrt", "sqrtf", "fabs", "fabsf", "floor", "ceil",
                                             "sin", "cos", "tan", "asin", "acos", "atan", "atan2", "pow", "fmod", "exp", "log", "log10",
                                             "sinf", "cosf", "wcslen", "wcscmp", "wcscpy"};
    return ok.count(n.str()) != 0;
}

// ---------------------------------------------------------------- support check
struct Verdict {
    bool ok = true;
    std::string why;
    void no(const std::string& w) {
        if (ok) ok = false, why = w;
    }
};

bool typeHoldsPtr(Type* t) {
    if (t->isPointerTy()) return true;
    if (auto* s = dyn_cast<StructType>(t)) {
        for (Type* e : s->elements())
            if (typeHoldsPtr(e)) return true;
        return false;
    }
    if (auto* a = dyn_cast<ArrayType>(t)) return typeHoldsPtr(a->getElementType());
    if (auto* v = dyn_cast<VectorType>(t)) return typeHoldsPtr(v->getElementType());
    return false;
}

bool argOk(Type* t) { return t->isIntegerTy() || t->isPointerTy() || t->isFloatTy() || t->isDoubleTy(); }

// What the lowering cannot handle in F's own body; callees are checked by the caller loop.
Verdict check(Function& F, const std::set<Function*>& keep) {
    Verdict v;
    if (F.isVarArg()) v.no("varargs");
    for (Argument& a : F.args()) {
        if (a.hasInAllocaAttr()) v.no("inalloca parameter");
        if (a.hasByValAttr()) v.no("byval parameter");
        if (!argOk(a.getType())) v.no("aggregate parameter");
    }
    Type* rt = F.getReturnType();
    if (!rt->isVoidTy() && !argOk(rt)) v.no("aggregate return");
    if (F.hasPersonalityFn()) v.no("EH personality");
    for (BasicBlock& bb : F)
        for (Instruction& i : bb) {
            if (auto* a = dyn_cast<AllocaInst>(&i)) {
                if (!a->isStaticAlloca()) v.no("dynamic alloca");
            } else if (auto* l = dyn_cast<LoadInst>(&i)) {
                if (typeHoldsPtr(l->getType()) && !l->getType()->isPointerTy()) v.no("aggregate load holding pointers");
            } else if (auto* s = dyn_cast<StoreInst>(&i)) {
                Type* t = s->getValueOperand()->getType();
                if (typeHoldsPtr(t) && !t->isPointerTy()) v.no("aggregate store holding pointers");
            } else if (isa<AtomicCmpXchgInst>(i) || isa<AtomicRMWInst>(i)) {
                v.no("atomics");
            } else if (isa<InvokeInst>(i) || isa<IndirectBrInst>(i) || isa<CallBrInst>(i) || isa<LandingPadInst>(i)) {
                v.no("invoke/indirectbr");
            } else if (auto* cb = dyn_cast<CallBase>(&i)) {
                if (cb->isInlineAsm()) {
                    v.no("inline asm");
                    continue;
                }
                for (unsigned k = 0; k < cb->arg_size(); ++k)
                    if (cb->isInAllocaArgument(k) || cb->paramHasAttr(k, Attribute::InAlloca)) v.no("inalloca call");
                Function* callee = dyn_cast<Function>(cb->getCalledOperand()->stripPointerCasts());
                if (callee && callee->isIntrinsic()) continue;
                if (callee && keep.count(callee) && cb->getFunctionType() == callee->getFunctionType()) continue;
                if (callee && hostSafe(callee->getName())) continue;
                // a bridge: x86 argument words and an x86 result
                for (unsigned k = 0; k < cb->arg_size(); ++k)
                    if (!argOk(cb->getArgOperand(k)->getType()) && !cb->paramHasAttr(k, Attribute::ByVal)) v.no("aggregate argument to a bridged call");
                Type* crt = cb->getType();
                if (!crt->isVoidTy() && !argOk(crt)) v.no("aggregate result of a bridged call");
                if (cb->getFunctionType()->isVarArg() && !callee) v.no("indirect varargs call");
                if (callee) {
                    uint32_t a;
                    if (!bound(*callee, a) && !hostSafe(callee->getName())) v.no("unbound callee " + callee->getName().str());
                    else if (g_privateCc.count(a)) v.no("calls a private-convention function " + callee->getName().str());
                }
            }
            for (Use& u : i.operands()) {
                Value* op = u.get();
                std::vector<Constant*> work;
                if (auto* c = dyn_cast<Constant>(op)) work.push_back(c);
                while (!work.empty()) {
                    Constant* c = work.back();
                    work.pop_back();
                    if (isa<GlobalVariable>(c) || isa<GlobalAlias>(c)) {
                        uint32_t a;
                        if (!bound(*cast<GlobalValue>(c), a)) v.no("unbound global " + c->getName().str());
                    } else if (auto* fn = dyn_cast<Function>(c)) {
                        auto* cb = dyn_cast<CallBase>(&i);
                        if (cb && cb->isCallee(&u)) continue;
                        uint32_t a;
                        if (!fn->isIntrinsic() && !bound(*fn, a)) v.no("address of unbound function " + fn->getName().str());
                    } else if (isa<ConstantExpr>(c) || isa<ConstantAggregate>(c)) {
                        for (Use& cu : c->operands())
                            if (auto* cc = dyn_cast<Constant>(cu.get())) work.push_back(cc);
                    }
                }
            }
        }
    return v;
}

// ---------------------------------------------------------------- lowering helpers
Module* M;
LLVMContext* C;
Type *I8, *I32, *I64, *F64, *PtrTy;
FunctionCallee ndFrameEnter, ndFrameLeave, ndCall, ndEntryBegin, ndEntryEnd, ndGetReg, ndEnabled, ndClock, ndTimeAdd, ndVerify;

Value* toHost(IRBuilder<>& b, Value* w32) { return b.CreateIntToPtr(b.CreateZExt(w32, I64), PtrTy); }
Value* toWord(IRBuilder<>& b, Value* p) { return b.CreateTrunc(b.CreatePtrToInt(p, I64), I32); }
Constant* absPtr(uint32_t a) { return ConstantExpr::getIntToPtr(ConstantInt::get(I64, a), PtrTy); }

// Constant expressions inside instruction operands become instructions, so the passes below see
// every GEP / cast on a global as an instruction.
void expandConstExprs(Function& F) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (BasicBlock& bb : F)
            for (Instruction& i : bb)
                for (Use& u : i.operands()) {
                    auto* ce = dyn_cast<ConstantExpr>(u.get());
                    if (!ce) continue;
                    Instruction* at = &i;
                    if (auto* phi = dyn_cast<PHINode>(&i)) at = phi->getIncomingBlock(u)->getTerminator();
                    Instruction* ni = ce->getAsInstruction();
                    ni->insertBefore(at);
                    u.set(ni);
                    changed = true;
                }
    }
}

// Globals and taken function addresses -> absolute retail addresses.
void bindGlobals(Function& F) {
    for (BasicBlock& bb : F)
        for (Instruction& i : bb)
            for (Use& u : i.operands()) {
                if (isa<GlobalVariable>(u.get()) || isa<GlobalAlias>(u.get())) {
                    uint32_t a = 0;
                    bound(*cast<GlobalValue>(u.get()), a);
                    u.set(absPtr(a));
                } else if (auto* fn = dyn_cast<Function>(u.get())) {
                    auto* cb = dyn_cast<CallBase>(&i);
                    if ((cb && cb->isCallee(&u)) || fn->isIntrinsic()) continue;
                    uint32_t a = 0;
                    bound(*fn, a);
                    u.set(absPtr(a));
                }
            }
}

// GEPs -> byte offsets from the 32-bit layout.
void lowerGeps(Function& F) {
    std::vector<GetElementPtrInst*> geps;
    for (BasicBlock& bb : F)
        for (Instruction& i : bb)
            if (auto* g = dyn_cast<GetElementPtrInst>(&i)) geps.push_back(g);
    for (GetElementPtrInst* g : geps) {
        IRBuilder<> b(g);
        Value* off = ConstantInt::get(I64, 0);
        Type* cur = g->getSourceElementType();
        bool first = true;
        for (auto it = g->idx_begin(); it != g->idx_end(); ++it) {
            Value* idx = *it;
            if (first) {
                Value* sidx = b.CreateSExtOrTrunc(idx, I64);
                off = b.CreateAdd(off, b.CreateMul(sidx, ConstantInt::get(I64, g_dl32->getTypeAllocSize(cur))));
                first = false;
                continue;
            }
            if (auto* st = dyn_cast<StructType>(cur)) {
                unsigned field = static_cast<unsigned>(cast<ConstantInt>(idx)->getZExtValue());
                off = b.CreateAdd(off, ConstantInt::get(I64, g_dl32->getStructLayout(st)->getElementOffset(field)));
                cur = st->getElementType(field);
            } else {
                Type* el = cur->isArrayTy() ? cur->getArrayElementType() : cast<VectorType>(cur)->getElementType();
                Value* sidx = b.CreateSExtOrTrunc(idx, I64);
                off = b.CreateAdd(off, b.CreateMul(sidx, ConstantInt::get(I64, g_dl32->getTypeAllocSize(el))));
                cur = el;
            }
        }
        // guest addresses wrap at 4 GiB: (base32 + off) mod 2^32, zero-extended
        Value* base = toWord(b, g->getPointerOperand());
        Value* sum = b.CreateAdd(base, b.CreateTrunc(off, I32));
        Value* np = toHost(b, sum);
        g->replaceAllUsesWith(np);
        g->eraseFromParent();
    }
}

// Pointer loads / stores as 32-bit words; pointer casts to and from integers at 32 bits.
void lowerPtrMemory(Function& F) {
    std::vector<Instruction*> todo;
    for (BasicBlock& bb : F)
        for (Instruction& i : bb)
            if ((isa<LoadInst>(i) && i.getType()->isPointerTy()) ||
                (isa<StoreInst>(i) && cast<StoreInst>(i).getValueOperand()->getType()->isPointerTy()))
                todo.push_back(&i);
    for (Instruction* i : todo) {
        IRBuilder<> b(i);
        if (auto* l = dyn_cast<LoadInst>(i)) {
            LoadInst* w = b.CreateAlignedLoad(I32, l->getPointerOperand(), l->getAlign(), l->isVolatile());
            l->replaceAllUsesWith(toHost(b, w));
            l->eraseFromParent();
        } else {
            auto* s = cast<StoreInst>(i);
            b.CreateAlignedStore(toWord(b, s->getValueOperand()), s->getPointerOperand(), s->getAlign(), s->isVolatile());
            s->eraseFromParent();
        }
    }
}

// Static allocas -> one frame on the guest stack.
void lowerAllocas(Function& F) {
    std::vector<AllocaInst*> as;
    for (Instruction& i : F.getEntryBlock())
        if (auto* a = dyn_cast<AllocaInst>(&i)) as.push_back(a);
    if (as.empty()) return;
    uint64_t size = 0;
    std::vector<uint64_t> offs;
    for (AllocaInst* a : as) {
        uint64_t al = std::max<uint64_t>(4, a->getAlign().value());
        size = (size + al - 1) & ~(al - 1);
        offs.push_back(size);
        size += *a->getAllocationSize(*g_dl32);
    }
    size = (size + 15) & ~uint64_t{15};
    IRBuilder<> b(&*F.getEntryBlock().getFirstInsertionPt());
    Value* saved = b.CreateCall(ndFrameEnter, {ConstantInt::get(I32, size)});  // returns the old esp; the frame is [old - size, old)
    Value* frame = b.CreateSub(saved, ConstantInt::get(I32, size));
    for (size_t k = 0; k < as.size(); ++k) {
        AllocaInst* a = as[k];
        // lifetime markers only take allocas
        for (User* u : make_early_inc_range(a->users()))
            if (auto* ii = dyn_cast<IntrinsicInst>(u))
                if (ii->isLifetimeStartOrEnd()) ii->eraseFromParent();
        IRBuilder<> ab(a);
        a->replaceAllUsesWith(toHost(ab, ab.CreateAdd(frame, ConstantInt::get(I32, offs[k]))));
        a->eraseFromParent();
    }
    for (BasicBlock& bb : F)
        if (auto* r = dyn_cast<ReturnInst>(bb.getTerminator())) IRBuilder<>(r).CreateCall(ndFrameLeave, {saved});
}

// ---- x86 argument words for a bridged call / an entry
struct Word {
    enum Kind { Stack, Ecx, Edx } kind;
};

// How each IR parameter travels in the x86 call: register (ecx / edx) or stack words.
std::vector<int> regOf(CallingConv::ID cc, FunctionType* ft, const AttributeList& al) {
    std::vector<int> r(ft->getNumParams(), 0);  // 0 stack, 1 ecx, 2 edx
    if (cc == CallingConv::X86_ThisCall && ft->getNumParams() > 0) r[0] = 1;
    if (cc == CallingConv::X86_FastCall) {
        int next = 1;
        for (unsigned k = 0; k < ft->getNumParams() && next <= 2; ++k)
            if (al.hasParamAttr(k, Attribute::InReg)) r[k] = next++;
    }
    return r;
}

// result kinds for nd_call / nd_entry_end: 0 none, 1 eax, 2 edx:eax, 3 st(0)
int retKind(Type* t) {
    if (t->isVoidTy()) return 0;
    if (t->isFloatTy() || t->isDoubleTy()) return 3;
    if (t->isIntegerTy(64)) return 2;
    return 1;
}

uint32_t calleeCleans(CallingConv::ID cc, FunctionType* ft, const AttributeList& al) {
    if (cc != CallingConv::X86_StdCall && cc != CallingConv::X86_ThisCall && cc != CallingConv::X86_FastCall) return 0;
    std::vector<int> r = regOf(cc, ft, al);
    uint32_t n = 0;
    for (unsigned k = 0; k < ft->getNumParams(); ++k) {
        if (r[k]) continue;
        Type* bt = al.getParamByValType(k);
        n += stackBytes(*g_dl32, bt ? bt : ft->getParamType(k));
    }
    return n;
}

// Replace a call that leaves this module (or an indirect one) with nd_call.
void bridgeCall(CallBase* cb, Value* target32) {
    IRBuilder<> b(cb);
    FunctionType* ft = cb->getFunctionType();
    const AttributeList& al = cb->getAttributes();
    std::vector<int> regs = regOf(cb->getCallingConv(), ft, al);
    std::vector<Value*> words;
    Value* ecx = ConstantInt::get(I32, 0);
    Value* edx = ConstantInt::get(I32, 0);
    uint32_t mask = 0;
    auto asWord = [&](Value* v) -> Value* {
        Type* t = v->getType();
        if (t->isPointerTy()) return toWord(b, v);
        if (t->isFloatTy()) return b.CreateBitCast(v, I32);
        if (t->isIntegerTy() && t->getIntegerBitWidth() < 32) return b.CreateZExt(v, I32);
        return v;
    };
    for (unsigned k = 0; k < cb->arg_size(); ++k) {
        Value* v = cb->getArgOperand(k);
        Type* t = v->getType();
        if (regs[k] == 1) { ecx = asWord(v), mask |= 1; continue; }
        if (regs[k] == 2) { edx = asWord(v), mask |= 2; continue; }
        if (Type* bt = cb->getParamByValType(k)) {  // the struct's bytes, word by word
            uint32_t n = stackBytes(*g_dl32, bt) / 4;
            for (uint32_t w = 0; w < n; ++w) words.push_back(b.CreateAlignedLoad(I32, b.CreateConstGEP1_32(I8, v, w * 4), Align(1)));
            continue;
        }
        if (t->isIntegerTy(64) || t->isDoubleTy()) {
            Value* q = t->isDoubleTy() ? b.CreateBitCast(v, I64) : v;
            words.push_back(b.CreateTrunc(q, I32));
            words.push_back(b.CreateTrunc(b.CreateLShr(q, 32), I32));
            continue;
        }
        if (t->isIntegerTy() && t->getIntegerBitWidth() < 32 && cb->paramHasAttr(k, Attribute::SExt)) {
            words.push_back(b.CreateSExt(v, I32));
            continue;
        }
        words.push_back(asWord(v));
    }
    // host-side argument array (nd_call copies it to the guest stack) and result record
    IRBuilder<> eb(&*cb->getFunction()->getEntryBlock().getFirstInsertionPt());
    AllocaInst* arr = eb.CreateAlloca(I32, ConstantInt::get(I32, std::max<size_t>(1, words.size())));
    StructType* resT = StructType::get(*C, {I32, I32, F64});
    AllocaInst* res = eb.CreateAlloca(resT);
    for (size_t w = 0; w < words.size(); ++w) b.CreateStore(words[w], b.CreateConstGEP1_32(I32, arr, static_cast<unsigned>(w)));
    Type* rt = cb->getType();
    b.CreateCall(ndCall, {target32, arr, ConstantInt::get(I32, words.size()), ConstantInt::get(I32, mask | (retKind(rt) << 8)), ecx, edx, res});
    Value* out = nullptr;
    if (!rt->isVoidTy()) {
        Value* eax = b.CreateLoad(I32, b.CreateStructGEP(resT, res, 0));
        if (rt->isPointerTy()) out = toHost(b, eax);
        else if (rt->isIntegerTy(64)) out = b.CreateOr(b.CreateZExt(eax, I64), b.CreateShl(b.CreateZExt(b.CreateLoad(I32, b.CreateStructGEP(resT, res, 1)), I64), 32));
        else if (rt->isIntegerTy()) out = b.CreateTrunc(eax, rt);
        else {
            Value* st = b.CreateLoad(F64, b.CreateStructGEP(resT, res, 2));
            out = rt->isFloatTy() ? b.CreateFPTrunc(st, rt) : st;
        }
        cb->replaceAllUsesWith(out);
    }
    cb->eraseFromParent();
}

// The guest-ABI entry of native function F at address A: nd_entry_<A>(Ctx*), and the wrap the
// lifter's --wrap calls: nd_wrap_<A>(Ctx*) = enabled ? entry : F_<A>_orig.
void makeEntry(Function* F, uint32_t A, CallingConv::ID cc, const AttributeList& al) {
    char hex[16];
    std::snprintf(hex, sizeof hex, "%08X", A);
    FunctionType* et = FunctionType::get(Type::getVoidTy(*C), {PtrTy}, false);
    Function* E = Function::Create(et, GlobalValue::InternalLinkage, std::string("nd_entry_") + hex, M);
    IRBuilder<> b(BasicBlock::Create(*C, "", E));
    Value* ctx = E->getArg(0);
    Value* esp = b.CreateCall(ndEntryBegin, {ctx});
    FunctionType* ft = F->getFunctionType();
    std::vector<int> regs = regOf(cc, ft, al);
    std::vector<Value*> args;
    uint32_t off = 4;
    auto stackWord = [&](uint32_t o) { return b.CreateAlignedLoad(I32, toHost(b, b.CreateAdd(esp, ConstantInt::get(I32, o))), Align(4)); };
    for (unsigned k = 0; k < ft->getNumParams(); ++k) {
        Type* t = ft->getParamType(k);
        Value* w;
        if (regs[k]) {
            w = b.CreateCall(ndGetReg, {ctx, ConstantInt::get(I32, regs[k])});
        } else if (Type* bt = al.getParamByValType(k)) {  // the copy on the guest stack
            args.push_back(toHost(b, b.CreateAdd(esp, ConstantInt::get(I32, off))));
            off += stackBytes(*g_dl32, bt);
            continue;
        } else if (t->isIntegerTy(64) || t->isDoubleTy()) {
            Value* lo = b.CreateZExt(stackWord(off), I64), *hi = b.CreateZExt(stackWord(off + 4), I64);
            Value* q = b.CreateOr(lo, b.CreateShl(hi, 32));
            args.push_back(t->isDoubleTy() ? b.CreateBitCast(q, t) : q);
            off += 8;
            continue;
        } else {
            w = stackWord(off);
            off += 4;
        }
        if (t->isPointerTy()) args.push_back(toHost(b, w));
        else if (t->isFloatTy()) args.push_back(b.CreateBitCast(w, t));
        else if (t->isIntegerTy() && t->getIntegerBitWidth() < 32) args.push_back(b.CreateTrunc(w, t));
        else args.push_back(w);
    }
    CallInst* r = b.CreateCall(F, args);
    r->setAttributes(F->getAttributes());
    Type* rt = ft->getReturnType();
    Value* eax = ConstantInt::get(I32, 0), *edx = ConstantInt::get(I32, 0), *st = ConstantFP::get(F64, 0.0);
    if (rt->isPointerTy()) eax = toWord(b, r);
    else if (rt->isIntegerTy(64)) eax = b.CreateTrunc(r, I32), edx = b.CreateTrunc(b.CreateLShr(r, 32), I32);
    else if (rt->isIntegerTy()) eax = b.CreateZExtOrTrunc(r, I32);
    else if (rt->isFloatTy()) st = b.CreateFPExt(r, F64);
    else if (rt->isDoubleTy()) st = r;
    int kind = retKind(rt);
    for (unsigned k = 0; k < ft->getNumParams() && rt->isVoidTy(); ++k)
        if (al.hasParamAttr(k, Attribute::StructRet)) eax = toWord(b, args[k]), kind = 1;  // MSVC returns the sret pointer in eax
    b.CreateCall(ndEntryEnd, {ctx, ConstantInt::get(I32, kind), eax, edx, st, ConstantInt::get(I32, calleeCleans(cc, ft, al))});
    b.CreateRetVoid();

    Function* orig = Function::Create(et, GlobalValue::ExternalLinkage, std::string("F_") + hex + "_orig", M);
    Function* W = Function::Create(et, GlobalValue::ExternalLinkage, std::string("nd_wrap_") + hex, M);
    IRBuilder<> wb(BasicBlock::Create(*C, "", W));
    BasicBlock* yes = BasicBlock::Create(*C, "native", W);
    BasicBlock* no = BasicBlock::Create(*C, "lifted", W);
    // nd_enabled: bit 0 the native body, bit 1 time the call (FABLE_ND_TIME), native or lifted,
    // bit 2 verify the call (FABLE_ND_VERIFY: nd_verify runs both bodies on the same input)
    Value* flags = wb.CreateCall(ndEnabled, {ConstantInt::get(I32, A), W->getArg(0)});
    {
        BasicBlock* verify = BasicBlock::Create(*C, "verify", W);
        BasicBlock* cont = BasicBlock::Create(*C, "cont", W);
        wb.CreateCondBr(wb.CreateICmpNE(wb.CreateAnd(flags, ConstantInt::get(I32, 4)), ConstantInt::get(I32, 0)), verify, cont);
        IRBuilder<> vb(verify);
        vb.CreateCall(ndVerify, {ConstantInt::get(I32, A), W->getArg(0), E, orig});
        vb.CreateRetVoid();
        wb.SetInsertPoint(cont);
    }
    Value* timed = wb.CreateICmpNE(wb.CreateAnd(flags, ConstantInt::get(I32, 2)), ConstantInt::get(I32, 0));
    BasicBlock* start = BasicBlock::Create(*C, "start", W);
    BasicBlock* pick = BasicBlock::Create(*C, "pick", W);
    BasicBlock* done = BasicBlock::Create(*C, "done", W);
    BasicBlock* stop = BasicBlock::Create(*C, "stop", W);
    BasicBlock* out = BasicBlock::Create(*C, "out", W);
    BasicBlock* entryBB = wb.GetInsertBlock();
    wb.CreateCondBr(timed, start, pick);
    IRBuilder<> sb(start);
    Value* t0 = sb.CreateCall(ndClock, {});
    sb.CreateBr(pick);
    IRBuilder<> pb(pick);
    PHINode* tp = pb.CreatePHI(I64, 2);
    tp->addIncoming(ConstantInt::get(I64, 0), entryBB);
    tp->addIncoming(t0, start);
    pb.CreateCondBr(pb.CreateICmpNE(pb.CreateAnd(flags, ConstantInt::get(I32, 1)), ConstantInt::get(I32, 0)), yes, no);
    IRBuilder<>(yes).CreateCall(E, {W->getArg(0)});
    IRBuilder<>(yes).CreateBr(done);
    IRBuilder<>(no).CreateCall(orig, {W->getArg(0)});
    IRBuilder<>(no).CreateBr(done);
    IRBuilder<>(done).CreateCondBr(timed, stop, out);
    IRBuilder<> tb(stop);
    tb.CreateCall(ndTimeAdd, {ConstantInt::get(I32, A), tp, flags});
    tb.CreateBr(out);
    IRBuilder<>(out).CreateRetVoid();
}

}  // namespace

int main(int argc, char** argv) {
    Options o;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        auto next = [&] { return i + 1 < argc ? std::string(argv[++i]) : std::string(); };
        if (a == "--bindings") o.bindings = next();
        else if (a == "--natives") o.natives = next();
        else if (a == "--triple") o.triple = next();
        else if (a == "--report") o.report = next();
        else if (a == "--exclude") o.exclude = next();
        else if (a == "--private-cc") o.privateCc = next();
        else if (o.in.empty()) o.in = a;
        else o.out = a;
    }
    if (o.in.empty() || o.out.empty() || o.bindings.empty() || o.natives.empty() || o.triple.empty()) {
        errs() << "usage: fable_lower <in> <out.bc> --bindings reloc_bindings.tsv --natives natives.tsv --triple T [--report r.tsv]\n";
        return 2;
    }
    {
        std::ifstream f(o.bindings);
        std::string line;
        std::getline(f, line);
        while (std::getline(f, line)) {
            std::istringstream s(line);
            std::string sym, tgt;
            if (std::getline(s, sym, '\t') && std::getline(s, tgt, '\t')) g_bind[sym] = static_cast<uint32_t>(std::stoul(tgt, nullptr, 16));
        }
    }
    {
        std::ifstream f(o.natives);
        std::string line;
        while (std::getline(f, line)) {
            std::istringstream s(line);
            std::string addr, name;
            if (std::getline(s, addr, '\t') && std::getline(s, name)) g_natives[static_cast<uint32_t>(std::stoul(addr, nullptr, 16))] = name;
        }
    }
    LLVMContext ctx;
    SMDiagnostic err;
    std::unique_ptr<Module> mod = parseIRFile(o.in, err, ctx);
    if (!mod) {
        err.print(argv[0], errs());
        return 1;
    }
    M = mod.get();
    C = &ctx;
    DataLayout dl32 = M->getDataLayout();
    g_dl32 = &dl32;
    I8 = Type::getInt8Ty(ctx), I32 = Type::getInt32Ty(ctx), I64 = Type::getInt64Ty(ctx), F64 = Type::getDoubleTy(ctx);
    PtrTy = PointerType::get(ctx, 0);

    // The landed functions; then which of them (and the module-local helpers they call) can be lowered.
    std::map<Function*, uint32_t> addrOf;
    if (!o.privateCc.empty()) {
        std::ifstream f(o.privateCc);
        std::string line;
        while (std::getline(f, line))
            if (!line.empty()) g_privateCc.insert(static_cast<uint32_t>(std::stoul(line, nullptr, 16)));
    }
    std::set<uint32_t> taken;  // an excluded address (hooked or wrapped by host code already) is never native
    if (!o.exclude.empty()) {
        std::ifstream f(o.exclude);
        std::string line;
        while (std::getline(f, line))
            if (!line.empty()) taken.insert(static_cast<uint32_t>(std::stoul(line, nullptr, 16)));
    }
    for (auto& [a, n] : g_natives) {
        Function* f = M->getFunction(n);
        if (!f || f->isDeclaration()) continue;
        // a definition whose own symbol is bound to another address is not this landed function
        // (build_native.sh already picks it by the file's Leaf: line)
        uint32_t b = 0;
        if (bound(*f, b) && b != a) continue;
        if (taken.insert(a).second) addrOf[f] = a;
    }
    for (Function& f : *M) {  // every other definition with a retail address is a landed function too
        uint32_t a;
        if (!f.isDeclaration() && !addrOf.count(&f) && bound(f, a) && taken.insert(a).second) addrOf[&f] = a;
    }
    std::set<Function*> keep;  // natives + local helpers kept native
    for (auto& [f, a] : addrOf) keep.insert(f);
    for (Function& f : *M)  // helpers: definitions that are not landed functions (inline / template copies)
        if (!f.isDeclaration() && !addrOf.count(&f) && (f.hasLocalLinkage() || f.hasLinkOnceLinkage() || f.hasWeakLinkage())) keep.insert(&f);
    std::map<Function*, std::string> rejected;
    for (auto& [f, a] : addrOf)
        if (g_privateCc.count(a)) rejected[f] = "private calling convention", keep.erase(f);
    for (bool changed = true; changed;) {
        changed = false;
        for (Function* f : std::vector<Function*>(keep.begin(), keep.end())) {
            Verdict v = check(*f, keep);
            // a helper with a retail address can be bridged when it cannot be lowered
            if (!v.ok) {
                rejected[f] = v.why;
                keep.erase(f);
                changed = true;
            }
        }
        // a kept function calling a dropped helper with no address cannot bridge to it
        for (Function* f : std::vector<Function*>(keep.begin(), keep.end())) {
            Verdict v = check(*f, keep);
            if (!v.ok && !rejected.count(f)) {
                rejected[f] = v.why;
                keep.erase(f);
                changed = true;
            }
        }
    }

    ndFrameEnter = M->getOrInsertFunction("nd_frame_enter", FunctionType::get(I32, {I32}, false));
    ndFrameLeave = M->getOrInsertFunction("nd_frame_leave", FunctionType::get(Type::getVoidTy(ctx), {I32}, false));
    ndCall = M->getOrInsertFunction("nd_call", FunctionType::get(Type::getVoidTy(ctx), {I32, PtrTy, I32, I32, I32, I32, PtrTy}, false));
    ndEntryBegin = M->getOrInsertFunction("nd_entry_begin", FunctionType::get(I32, {PtrTy}, false));
    ndEntryEnd = M->getOrInsertFunction("nd_entry_end", FunctionType::get(Type::getVoidTy(ctx), {PtrTy, I32, I32, I32, F64, I32}, false));
    ndGetReg = M->getOrInsertFunction("nd_get_reg", FunctionType::get(I32, {PtrTy, I32}, false));
    ndEnabled = M->getOrInsertFunction("nd_enabled", FunctionType::get(I32, {I32, PtrTy}, false));
    ndClock = M->getOrInsertFunction("nd_clock", FunctionType::get(I64, {}, false));
    ndVerify = M->getOrInsertFunction("nd_verify", FunctionType::get(Type::getVoidTy(ctx), {I32, PtrTy, PtrTy, PtrTy}, false));
    ndTimeAdd = M->getOrInsertFunction("nd_time_add", FunctionType::get(Type::getVoidTy(ctx), {I32, I64, I32}, false));

    std::vector<std::string> reportRows;
    for (auto& [f, a] : addrOf) {
        char hex[16];
        std::snprintf(hex, sizeof hex, "%08X", a);
        reportRows.push_back(std::string(hex) + "\t" + f->getName().str() + "\t" + (keep.count(f) ? "native" : "lifted") + "\t" +
                             (rejected.count(f) ? rejected[f] : ""));
    }

    // Remember each landed function's x86 convention before the bodies are rewritten.
    struct EntryInfo { Function* f; uint32_t a; CallingConv::ID cc; AttributeList al; };
    std::vector<EntryInfo> entries;
    for (auto& [f, a] : addrOf)
        if (keep.count(f)) entries.push_back({f, a, f->getCallingConv(), f->getAttributes()});

    // Bodies of everything else go: the module keeps only what is lowered.
    for (Function& f : *M)
        if (!f.isDeclaration() && !keep.count(&f)) {
            f.deleteBody();
            f.setLinkage(GlobalValue::ExternalLinkage);
        }

    for (Function* f : keep) {
        expandConstExprs(*f);
        bindGlobals(*f);
        lowerAllocas(*f);  // before the GEPs: the frame offsets come from alloca sizes
        lowerGeps(*f);
        lowerPtrMemory(*f);
        // calls
        std::vector<CallBase*> calls;
        for (BasicBlock& bb : *f)
            for (Instruction& i : bb)
                if (auto* cb = dyn_cast<CallBase>(&i)) calls.push_back(cb);
        for (CallBase* cb : calls) {
            Function* callee = dyn_cast<Function>(cb->getCalledOperand()->stripPointerCasts());
            if (callee && (callee->isIntrinsic() || callee->getName().starts_with("nd_"))) continue;
            if (callee && keep.count(callee) && cb->getFunctionType() == callee->getFunctionType()) continue;
            if (callee && hostSafe(callee->getName())) continue;  // host libc on guest pointers (identity-mapped)
            Value* target;
            IRBuilder<> b(cb);
            if (callee) {
                uint32_t a = 0;
                bound(*callee, a);
                // dllimport: the target is the IAT slot's contents
                target = callee->hasDLLImportStorageClass() ? static_cast<Value*>(b.CreateLoad(I32, absPtr(a))) : static_cast<Value*>(ConstantInt::get(I32, a));
            } else {
                target = toWord(b, cb->getCalledOperand());
            }
            bridgeCall(cb, target);
        }
    }
    for (EntryInfo& e : entries) makeEntry(e.f, e.a, e.cc, e.al);

    // The code's address range for the code-share measurement (win/code_share.cpp): every lowered
    // function in one named section, registered by a constructor with the number of lowered functions.
    // ELF: section "fabledecomp", bracketed by the linker's __start_ / __stop_ symbols. COFF: ".text$fdm",
    // between markers in ".text$fda" / ".text$fdz" (the linker sorts grouped sections by suffix).
    {
        const bool coff = Triple(o.triple).isOSBinFormatCOFF();
        for (Function& f : *M)
            if (!f.isDeclaration()) f.setSection(coff ? ".text$fdm" : "fabledecomp");
        FunctionType* vt = FunctionType::get(Type::getVoidTy(ctx), false);
        Value *lo, *hi;
        if (coff) {
            auto marker = [&](const char* name, const char* sec) {
                Function* f = Function::Create(vt, GlobalValue::InternalLinkage, name, M);
                f->addFnAttr(Attribute::NoInline);
                f->addFnAttr(Attribute::OptimizeNone);
                f->setSection(sec);
                IRBuilder<>(BasicBlock::Create(ctx, "", f)).CreateRetVoid();
                appendToUsed(*M, {f});
                return f;
            };
            lo = marker("nd_text_begin", ".text$fda"), hi = marker("nd_text_end", ".text$fdz");
        } else {
            auto bound = [&](const char* name) {
                auto* g = new GlobalVariable(*M, I8, true, GlobalValue::ExternalLinkage, nullptr, name);
                g->setVisibility(GlobalValue::HiddenVisibility);
                return g;
            };
            lo = bound("__start_fabledecomp"), hi = bound("__stop_fabledecomp");
        }
        Function* ctor = Function::Create(vt, GlobalValue::InternalLinkage, "nd_register", M);
        FunctionCallee reg = M->getOrInsertFunction("nd_register_range", FunctionType::get(Type::getVoidTy(ctx), {PtrTy, PtrTy, I32}, false));
        IRBuilder<> cb(BasicBlock::Create(ctx, "", ctor));
        cb.CreateCall(reg, {lo, hi, ConstantInt::get(I32, entries.size())});
        cb.CreateRetVoid();
        appendToGlobalCtors(*M, ctor, 65535);
    }

    // No alignment or dereferenceability assumptions: the front end derives them from the C++ types,
    // but retail objects can sit at any address (packed members, odd allocations) and VC7.1's code never
    // relied on them. Over 32-bit words LLVM would turn an `align 4` into cleared low address bits.
    for (Function& f : *M) {
        AttributeMask m;
        m.addAttribute(Attribute::Alignment).addAttribute(Attribute::Dereferenceable).addAttribute(Attribute::DereferenceableOrNull);
        m.addAttribute(Attribute::NonNull);  // retail code calls methods through null pointers that check `this`
        f.removeRetAttrs(m);
        for (unsigned k = 0; k < f.arg_size(); ++k) f.removeParamAttrs(k, m);
        for (BasicBlock& bb : f)
            for (Instruction& i : make_early_inc_range(bb)) {
                if (auto* l = dyn_cast<LoadInst>(&i)) l->setAlignment(Align(1));
                else if (auto* st = dyn_cast<StoreInst>(&i)) st->setAlignment(Align(1));
                else if (auto* cb = dyn_cast<CallBase>(&i)) {
                    if (auto* ii = dyn_cast<IntrinsicInst>(cb); ii && ii->getIntrinsicID() == Intrinsic::assume) {
                        ii->eraseFromParent();
                        continue;
                    }
                    cb->removeRetAttrs(m);
                    for (unsigned k = 0; k < cb->arg_size(); ++k) cb->removeParamAttrs(k, m);
                }
            }
    }

    // x86 conventions and attributes off; everything but the wraps internal; the 64-bit layout on.
    for (Function& f : *M) {
        f.setCallingConv(CallingConv::C);
        f.removeFnAttr("target-cpu");
        f.removeFnAttr("target-features");
        f.removeFnAttr("tune-cpu");
        for (unsigned k = 0; k < f.arg_size(); ++k) f.removeParamAttr(k, Attribute::InReg);
        for (BasicBlock& bb : f)
            for (Instruction& i : bb)
                if (auto* cb = dyn_cast<CallBase>(&i)) {
                    cb->setCallingConv(CallingConv::C);
                    for (unsigned k = 0; k < cb->arg_size(); ++k) cb->removeParamAttr(k, Attribute::InReg);
                }
        if (!f.isDeclaration() && !f.getName().starts_with("nd_wrap_")) {
            f.setLinkage(GlobalValue::InternalLinkage);
            f.setComdat(nullptr);
        }
        f.setDLLStorageClass(GlobalValue::DefaultStorageClass);
    }
    // dead globals (vtables, RTTI, literals now bound to retail addresses) reference each other: to a fixpoint
    for (bool erased = true; erased;) {
        erased = false;
        for (GlobalAlias& g : make_early_inc_range(M->aliases())) {
            g.removeDeadConstantUsers();
            if (g.use_empty()) g.eraseFromParent(), erased = true;
        }
        for (GlobalVariable& g : make_early_inc_range(M->globals())) {
            if (g.getName().starts_with("llvm.")) continue;  // llvm.global_ctors / llvm.used have no users by design
            g.removeDeadConstantUsers();
            if (g.use_empty()) g.eraseFromParent(), erased = true;
        }
        for (Function& f : make_early_inc_range(*M)) {
            f.removeDeadConstantUsers();
            if (f.isDeclaration() && f.use_empty() && !f.getName().starts_with("nd_")) f.eraseFromParent(), erased = true;
        }
    }
    for (GlobalVariable& g : M->globals()) g.setComdat(nullptr);
    for (Function& f : *M) f.setComdat(nullptr);
    M->getComdatSymbolTable().clear();
    M->setPICLevel(PICLevel::BigPIC);

    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    std::string e;
    const Target* t = TargetRegistry::lookupTarget(o.triple, e);
    if (!t) {
        errs() << e << "\n";
        return 1;
    }
    std::unique_ptr<TargetMachine> tm(t->createTargetMachine(o.triple, "generic", "", TargetOptions(), Reloc::PIC_));
    M->setTargetTriple(o.triple);
    M->setDataLayout(tm->createDataLayout());

    if (verifyModule(*M, &errs())) {
        errs() << "fable_lower: the lowered module does not verify\n";
        return 1;
    }
    std::error_code ec;
    raw_fd_ostream os(o.out, ec);
    WriteBitcodeToFile(*M, os);
    os.close();

    if (!o.report.empty()) {
        std::ofstream r(o.report);
        r << "address\tfunction\tstatus\treason\n";
        for (const std::string& row : reportRows) r << row << "\n";
    }
    errs() << "fable_lower: " << entries.size() << " of " << addrOf.size() << " landed functions native\n";
    return 0;
}
