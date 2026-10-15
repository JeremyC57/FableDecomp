// fable_recomp: statically recompile Fable.exe x86-32 code to portable C.
//
//   fable_recomp <Fable.exe> <functions.tsv> <out-dir> [--only addr,addr,...] [--per-file N]
//
// Discovers functions by recursive descent from every catalogued entry (and
// every direct call target), resolves MSVC jump tables, and emits one C
// function per guest function against rebuild/recomp/runtime/recomp.h.

#include "Image.hpp"

#include <Zydis/Zydis.h>

#include <algorithm>
#include <cstdio>
#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

namespace recomp {
namespace {

struct Insn {
    uint32_t addr = 0;
    ZydisDecodedInstruction in{};
    ZydisDecodedOperand op[ZYDIS_MAX_OPERAND_COUNT]{};
    uint32_t next() const { return addr + in.length; }
};

struct JumpTable {
    uint32_t table = 0;
    std::vector<uint32_t> targets;  // per index
};

struct Function {
    uint32_t entry = 0;
    std::set<uint32_t> insns;        // addresses of instructions in this function
    std::set<uint32_t> labels;       // block starts needing a label
    std::map<uint32_t, JumpTable> tables;  // keyed by the jmp instruction
    bool decodeError = false;
    bool eh = false;  // registers an MSVC C++ EH frame: gets a landing pad for catch continuations
};

std::string hex(uint32_t v) {
    char b[16];
    std::snprintf(b, sizeof b, "0x%08X", v);
    return b;
}
std::string hexu(uint64_t v) {
    char b[24];
    std::snprintf(b, sizeof b, "0x%llXu", static_cast<unsigned long long>(v));
    return b;
}
std::string fname(uint32_t a) {
    char b[16];
    std::snprintf(b, sizeof b, "F_%08X", a);
    return b;
}
std::string lname(uint32_t a) {
    char b[16];
    std::snprintf(b, sizeof b, "L_%08X", a);
    return b;
}

class Program {
public:
    explicit Program(const Image& img) : img_(img) {
        ZydisDecoderInit(&dec_, ZYDIS_MACHINE_MODE_LEGACY_32, ZYDIS_STACK_WIDTH_32);
        findEhStubs();
    }

    // MSVC C++ EH: each function with an EH frame registers a handler stub
    //     mov eax, offset FuncInfo ; jmp __CxxFrameHandler (via the `jmp [IAT]` thunk)
    // Map stub address -> FuncInfo.
    void findEhStubs() {
        uint32_t iat = 0;
        for (const auto& [a, name] : img_.imports())
            if (name.size() > 18 && name.compare(name.size() - 18, 18, "!__CxxFrameHandler") == 0) iat = a;
        if (!iat) return;
        std::set<uint32_t> thunks;
        for (const auto& sec : img_.sections()) {
            if (!(sec.flags & 0x20000000u)) continue;
            const uint32_t lo = img_.base() + sec.va, hi = lo + sec.vsize;
            for (uint32_t a = lo; a + 6 <= hi && img_.contains(a, 6); ++a)
                if (img_.r8(a) == 0xFF && img_.r8(a + 1) == 0x25 && img_.r32(a + 2) == iat) thunks.insert(a);
            for (uint32_t a = lo; a + 10 <= hi && img_.contains(a, 10); ++a)
                if (img_.r8(a) == 0xB8 && img_.r8(a + 5) == 0xE9 && thunks.count(a + 10 + img_.r32(a + 6))) ehStubs_[a] = img_.r32(a + 1);
        }
        std::cerr << "C++ EH handler stubs: " << ehStubs_.size() << "\n";
    }

    // Catch funclets return their continuation (`mov eax, offset cont ; ret`); those
    // addresses are in the parent function and only reached that way.
    void ehFrame(Function& f, uint32_t funcInfo, std::deque<uint32_t>& todo) {
        f.eh = true;
        if (!img_.contains(funcInfo, 20) || (img_.r32(funcInfo) & ~0xFu) != 0x19930520u) return;
        const uint32_t maxState = img_.r32(funcInfo + 4), unwind = img_.r32(funcInfo + 8);
        const uint32_t nTry = img_.r32(funcInfo + 12), tries = img_.r32(funcInfo + 16);
        for (uint32_t i = 0; i < maxState && img_.contains(unwind + 8 * i, 8); ++i)
            if (const uint32_t act = img_.r32(unwind + 8 * i + 4)) addEntry(act);
        for (uint32_t t = 0; t < nTry && img_.contains(tries + 20 * t, 20); ++t) {
            const uint32_t n = img_.r32(tries + 20 * t + 12), handlers = img_.r32(tries + 20 * t + 16);
            for (uint32_t h = 0; h < n && img_.contains(handlers + 16 * h, 16); ++h) {
                const uint32_t funclet = img_.r32(handlers + 16 * h + 12);
                addEntry(funclet);
                for (uint32_t a = funclet; a < funclet + 0x400 && img_.contains(a, 6); ++a)
                    if (img_.r8(a) == 0xB8 && img_.r8(a + 5) == 0xC3) {
                        const uint32_t cont = img_.r32(a + 1);
                        if (cont > f.entry && cont < f.entry + 0x20000 && img_.isCode(cont) && !isEntry(cont)) {
                            f.labels.insert(cont);
                            todo.push_back(cont);
                        }
                    }
            }
        }
    }

    const Insn* decode(uint32_t a) {
        auto it = cache_.find(a);
        if (it != cache_.end()) return it->second.in.length ? &it->second : nullptr;
        Insn ins;
        ins.addr = a;
        if (img_.isCode(a) && img_.contains(a, 1)) {
            const uint32_t avail = std::min<uint32_t>(15, img_.end() - a);
            if (ZYAN_FAILED(ZydisDecoderDecodeFull(&dec_, img_.at(a), avail, &ins.in, ins.op))) ins.in.length = 0;
        }
        auto& slot = cache_[a] = ins;
        return slot.in.length ? &slot : nullptr;
    }

    void addEntry(uint32_t a) {
        if (img_.isCode(a) && entries_.insert(a).second) work_.push_back(a);
    }
    bool isEntry(uint32_t a) const { return entries_.count(a) != 0; }

    void discoverAll() {
        while (!work_.empty()) {
            const uint32_t e = work_.front();
            work_.pop_front();
            if (!funcs_.count(e)) discover(e);
        }
    }

    // Code reached only through pointers (vtables, callback and CRT initializer
    // tables in .data/.rdata, `push offset fn` immediates) is not in the catalogue.
    // A referenced address becomes an entry if it starts an already-decoded
    // instruction, or lies in undiscovered code at a plausible function boundary.
    // Returns the number of entries added; call discoverAll() afterwards.
    size_t addReferencedEntries() {
        std::set<uint32_t> starts;
        std::map<uint32_t, uint32_t> covered;  // start -> end of decoded instruction
        std::set<uint32_t> cands;
        for (auto& [e, f] : funcs_)
            for (uint32_t a : f.insns) {
                const Insn* ins = decode(a);
                if (!ins) continue;
                starts.insert(a);
                covered[a] = ins->next();
                for (int i = 0; i < ins->in.operand_count_visible; ++i) {
                    const auto& o = ins->op[i];
                    if (o.type == ZYDIS_OPERAND_TYPE_IMMEDIATE && ins->in.mnemonic != ZYDIS_MNEMONIC_CALL &&
                        !(ins->in.meta.category == ZYDIS_CATEGORY_COND_BR || ins->in.meta.category == ZYDIS_CATEGORY_UNCOND_BR))
                        cands.insert(static_cast<uint32_t>(o.imm.value.u));
                }
            }
        for (uint32_t v : img_.relocTargets()) cands.insert(v);
        for (const auto& s : img_.sections()) {
            const uint32_t lo = img_.base() + s.va, hi = lo + s.vsize;
            if (img_.isCode(lo)) continue;
            for (uint32_t a = lo; a + 4 <= hi; a += 4)
                if (img_.contains(a, 4)) cands.insert(img_.r32(a));
        }
        auto isCovered = [&](uint32_t a) {
            auto it = covered.upper_bound(a);
            if (it == covered.begin()) return false;
            --it;
            return a >= it->first && a < it->second;
        };
        auto boundary = [&](uint32_t a) {
            if ((a & 15) == 0) return true;
            const uint8_t p = *img_.at(a - 1);
            if (p == 0xCC || p == 0x90 || p == 0xC3) return true;
            if (a >= 3 && *img_.at(a - 3) == 0xC2) return true;  // ret imm16
            if (*img_.at(a - 5) == 0xE9 || *img_.at(a - 2) == 0xEB) return true;
            if (*img_.at(a - 6) == 0xFF && *img_.at(a - 5) == 0x25) return true;  // jmp [import]
            // any unconditional jmp/ret (e.g. jmp [eax+24h]) that ends exactly here
            for (uint32_t k = 2; k <= 7; ++k) {
                const Insn* q = decode(a - k);
                if (q && q->next() == a &&
                    (q->in.mnemonic == ZYDIS_MNEMONIC_JMP || q->in.mnemonic == ZYDIS_MNEMONIC_RET))
                    return true;
            }
            return false;
        };
        size_t added = 0;
        for (uint32_t v : cands) {
            if (!img_.isCode(v) || !img_.contains(v, 1) || isEntry(v)) continue;
            bool ok = starts.count(v) != 0;
            if (!ok && !isCovered(v) && boundary(v) && decode(v)) ok = true;
            if (ok) { addEntry(v); ++added; }
        }
        return added;
    }

    std::map<uint32_t, Function>& functions() { return funcs_; }
    const Image& image() const { return img_; }

    static bool isGpr(ZydisRegister r) {
        return ZydisRegisterGetClass(r) == ZYDIS_REGCLASS_GPR32 || ZydisRegisterGetClass(r) == ZYDIS_REGCLASS_GPR16 ||
               ZydisRegisterGetClass(r) == ZYDIS_REGCLASS_GPR8;
    }

private:
    // Is `ins` an indirect jmp through a scaled table: jmp [idx*4 + table]?
    bool tableJump(const Insn& ins, uint32_t& table, ZydisRegister& idx) const {
        if (ins.in.mnemonic != ZYDIS_MNEMONIC_JMP) return false;
        const auto& o = ins.op[0];
        if (o.type != ZYDIS_OPERAND_TYPE_MEMORY || o.mem.base != ZYDIS_REGISTER_NONE || o.mem.scale != 4 ||
            o.mem.index == ZYDIS_REGISTER_NONE)
            return false;
        table = static_cast<uint32_t>(o.mem.disp.value);
        idx = o.mem.index;
        return img_.contains(table, 4);
    }

    // Look back (by address) up to `depth` instructions within the function for
    // `cmp reg, imm` bounding `reg`; also detects `movzx reg, byte [reg2 + tbl]`.
    std::vector<const Insn*> preceding(const Function& f, uint32_t addr, int depth) {
        std::vector<const Insn*> out;
        auto it = f.insns.lower_bound(addr);
        while (depth-- > 0 && it != f.insns.begin()) {
            --it;
            out.push_back(decode(*it));
        }
        return out;
    }

    static ZydisRegister full(ZydisRegister r) {
        return ZydisRegisterGetLargestEnclosing(ZYDIS_MACHINE_MODE_LEGACY_32, r);
    }

    bool resolveTable(Function& f, const Insn& jmp, uint32_t table, ZydisRegister idx) {
        uint32_t count = 0;
        uint32_t byteTable = 0;
        ZydisRegister bounded = full(idx);
        const auto before = preceding(f, jmp.addr, 12);
        // Constant loaded into a register earlier (for `cmp idx, reg` bounds).
        auto constantOf = [&](ZydisRegister r, size_t from) -> int64_t {
            for (size_t i = from; i < before.size(); ++i) {
                const Insn* q = before[i];
                if (!q) break;
                if (q->in.mnemonic == ZYDIS_MNEMONIC_MOV && q->op[0].type == ZYDIS_OPERAND_TYPE_REGISTER &&
                    full(q->op[0].reg.value) == full(r))
                    return q->op[1].type == ZYDIS_OPERAND_TYPE_IMMEDIATE ? static_cast<int64_t>(q->op[1].imm.value.u) : -1;
            }
            return -1;
        };
        for (size_t bi = 0; bi < before.size(); ++bi) {
            const Insn* p = before[bi];
            if (!p) break;
            if (p->in.mnemonic == ZYDIS_MNEMONIC_MOVZX && full(p->op[0].reg.value) == full(idx) &&
                p->op[1].type == ZYDIS_OPERAND_TYPE_MEMORY && p->op[1].size == 8 && p->op[1].mem.base != ZYDIS_REGISTER_NONE &&
                p->op[1].mem.index == ZYDIS_REGISTER_NONE && !byteTable) {
                byteTable = static_cast<uint32_t>(p->op[1].mem.disp.value);
                bounded = full(p->op[1].mem.base);
                continue;
            }
            if (p->in.mnemonic == ZYDIS_MNEMONIC_CMP && p->op[0].type == ZYDIS_OPERAND_TYPE_REGISTER &&
                full(p->op[0].reg.value) == bounded) {
                if (p->op[1].type == ZYDIS_OPERAND_TYPE_IMMEDIATE) {
                    count = static_cast<uint32_t>(p->op[1].imm.value.u) + 1;
                } else if (p->op[1].type == ZYDIS_OPERAND_TYPE_REGISTER) {
                    const int64_t k = constantOf(p->op[1].reg.value, bi + 1);
                    if (k >= 0) count = static_cast<uint32_t>(k) + 1;
                }
                break;
            }
        }
        if (count == 0 && !byteTable) {
            // Fallback: entries pointing into this function's neighbourhood.
            // Null entries are holes (indices the switch never takes) and do not end the table.
            for (uint32_t i = 0; i < 256 && img_.contains(table + i * 4, 4); ++i) {
                const uint32_t t = img_.r32(table + i * 4);
                if (t == 0 && i + 1 < 256) continue;
                if (!img_.isCode(t) || t < f.entry || t >= f.entry + 0x10000 || (t != f.entry && isEntry(t))) break;
                count = i + 1;
            }
            if (count) ++fallbackTables_;
        }
        if (count == 0 || count > 4096) return false;
        if (byteTable) {
            if (!img_.contains(byteTable, count)) return false;
            uint32_t maxIndex = 0;
            for (uint32_t i = 0; i < count; ++i) maxIndex = std::max<uint32_t>(maxIndex, img_.r8(byteTable + i));
            count = maxIndex + 1;
        }
        JumpTable jt;
        jt.table = table;
        for (uint32_t i = 0; i < count; ++i) {
            if (!img_.contains(table + i * 4, 4)) return false;
            const uint32_t t = img_.r32(table + i * 4);
            if (t == 0) continue;  // hole
            if (!img_.isCode(t)) return false;
            jt.targets.push_back(t);
        }
        f.tables[jmp.addr] = std::move(jt);
        return true;
    }

    void discover(uint32_t entry) {
        Function& f = funcs_[entry];
        f.entry = entry;
        f.labels.insert(entry);
        std::deque<uint32_t> todo{entry};
        std::unordered_set<uint32_t> seen;
        while (!todo.empty()) {
            uint32_t a = todo.front();
            todo.pop_front();
            while (true) {
                if (!seen.insert(a).second) break;
                if (a != entry && isEntry(a)) {  // fell through into another function
                    f.labels.insert(a);          // handled as a tail call at emit time
                    break;
                }
                const Insn* ins = decode(a);
                if (!ins) {
                    f.decodeError = true;
                    f.insns.insert(a);  // emitted as a trap
                    break;
                }
                f.insns.insert(a);
                const auto& in = ins->in;
                const auto m = in.mnemonic;
                if ((m == ZYDIS_MNEMONIC_PUSH || m == ZYDIS_MNEMONIC_MOV) && !ehStubs_.empty())
                    for (int i = 0; i < in.operand_count_visible; ++i)
                        if (ins->op[i].type == ZYDIS_OPERAND_TYPE_IMMEDIATE) {
                            auto st = ehStubs_.find(static_cast<uint32_t>(ins->op[i].imm.value.u));
                            if (st != ehStubs_.end()) ehFrame(f, st->second, todo);
                        }
                if (in.meta.category == ZYDIS_CATEGORY_COND_BR) {
                    uint64_t t = 0;
                    ZydisCalcAbsoluteAddress(&in, &ins->op[0], a, &t);
                    const auto target = static_cast<uint32_t>(t);
                    f.labels.insert(target);
                    f.labels.insert(ins->next());
                    if (!isEntry(target) || target == entry) todo.push_back(target);
                    a = ins->next();
                    continue;
                }
                if (m == ZYDIS_MNEMONIC_JMP) {
                    if (ins->op[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE) {
                        uint64_t t = 0;
                        ZydisCalcAbsoluteAddress(&in, &ins->op[0], a, &t);
                        const auto target = static_cast<uint32_t>(t);
                        f.labels.insert(target);
                        if (!isEntry(target) || target == entry) todo.push_back(target);
                    } else {
                        uint32_t table;
                        ZydisRegister idx;
                        if (tableJump(*ins, table, idx) && resolveTable(f, *ins, table, idx)) {
                            for (uint32_t t : f.tables[a].targets) {
                                f.labels.insert(t);
                                todo.push_back(t);
                            }
                        }
                    }
                    break;
                }
                if (m == ZYDIS_MNEMONIC_CALL) {
                    if (ins->op[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE) {
                        uint64_t t = 0;
                        ZydisCalcAbsoluteAddress(&in, &ins->op[0], a, &t);
                        addEntry(static_cast<uint32_t>(t));
                    }
                    a = ins->next();
                    continue;
                }
                if (m == ZYDIS_MNEMONIC_RET || m == ZYDIS_MNEMONIC_INT3 || m == ZYDIS_MNEMONIC_HLT ||
                    m == ZYDIS_MNEMONIC_UD2 || m == ZYDIS_MNEMONIC_IRETD)
                    break;
                a = ins->next();
            }
        }
        // Tail-jump / fall-through targets that are entries must be generated too.
        for (uint32_t l : f.labels)
            if (l != entry && isEntry(l) && !f.insns.count(l)) addEntry(l);
    }

    const Image& img_;
    ZydisDecoder dec_{};
    std::unordered_map<uint32_t, Insn> cache_;
    std::set<uint32_t> entries_;
    std::deque<uint32_t> work_;
    std::map<uint32_t, Function> funcs_;
    std::map<uint32_t, uint32_t> ehStubs_;  // handler stub -> FuncInfo

public:
    uint64_t fallbackTables_ = 0;
};

// ---------------------------------------------------------------------------
// C emission
// ---------------------------------------------------------------------------

struct Stats {
    std::map<std::string, uint64_t> unsupported;
    uint64_t insns = 0, unsupportedInsns = 0, unresolvedIndirectJumps = 0, decodeErrors = 0;
};

std::set<uint32_t>& traceAddrs() {
    static std::set<uint32_t> s;
    return s;
}
// --no-fold: every memory operand as the wrapped 32-bit sum (see Emitter::mem).
bool& foldDisplacements() {
    static bool on = true;
    return on;
}
// --no-x87-locals: every x87 operation on c->fpu (the memory model, see Emitter::x87).
bool& x87Locals() {
    static bool on = true;
    return on;
}
// Direct calls to these addresses (the CRT's _ftol2, when it is hooked to host_ftol2) are
// inlined as the conversion host_ftol2 performs; --no-ftol2-inline keeps the calls.
std::set<uint32_t>& ftol2Inline() {
    static std::set<uint32_t> s;
    return s;
}

class Emitter {
public:
    Emitter(Program& p, Stats& st) : p_(p), st_(st) {}

    std::string function(Function& f) {
        out_.str("");
        f_ = &f;
        xl_ = false;
        xin_.clear();
        xout_.clear();
        if (x87Locals()) xl_ = analyzeX87(f);
        xlFn_ = xl_;
        out_ << "void " << fname(f.entry) << "(Ctx* restrict c) {\n"  // restrict: no guest address aliases the Ctx
             << "    uint32_t eax = c->eax, ecx = c->ecx, edx = c->edx, ebx = c->ebx;\n"
             << "    uint32_t esp = c->esp, ebp = c->ebp, esi = c->esi, edi = c->edi;\n"
             << "    int fop = FOP(FK_EXPLICIT, 4); uint32_t fr = 0, fa = 0, fb = 0; /* flags undefined at entry */\n"
             << "    (void)fop; (void)fr; (void)fa; (void)fb;\n";
        if (xl_) {
            // x87 local model: base = TOP at entry (depth 0), registers relative to it.
            out_ << "    uint32_t xbase = c->fpu.top & 7u; uint16_t xcw = c->fpu.cw, xsw = c->fpu.sw;\n";
            std::string regs;
            for (int k = 0; k < 8; ++k)
                if (xused_ >> k & 1)
                    regs += std::string(regs.empty() ? "    double " : ", ") + "xs" + std::to_string(k) + " = c->fpu.st[(xbase + " +
                            std::to_string(k) + "u) & 7u]";
            if (!regs.empty()) out_ << regs << ";\n";
            out_ << "    (void)xbase; (void)xcw; (void)xsw;\n";
        }
        if (traceAddrs().count(f.entry)) out_ << "    SPILL; recomp_trace(c, " << hex(f.entry) << "u);\n";
        if (f.eh) {
            // Landing pad: the host's C++ exception dispatch longjmps here to continue
            // at a catch continuation (any label of this function).
            out_ << "    RecompLanding land_ __attribute__((cleanup(recomp_landing_pop)));\n"
                 << "    recomp_landing_push(&land_, esp);\n"
                 << "    if (__builtin_setjmp(land_.jb)) {\n"
                 << "        RELOAD; fop = FOP(FK_EXPLICIT, 4);\n";
            if (xl_) {
                // Retake the x87 state at the continuation's static depth.
                std::map<int, std::vector<uint32_t>> byDepth;
                for (uint32_t l : f.labels)
                    if (f.insns.count(l)) {
                        auto it = xin_.find(l);
                        const int d = it == xin_.end() ? 0 : it->second.d & 7;
                        if (d) byDepth[d].push_back(l);
                    }
                out_ << "        { uint32_t xd = 0u;";
                if (!byDepth.empty()) {
                    out_ << " switch (land_.target) {";
                    for (const auto& [d, ls] : byDepth) {
                        for (uint32_t l : ls) out_ << " case " << hex(l) << "u:";
                        out_ << " xd = " << d << "u; break;";
                    }
                    out_ << " }";
                }
                out_ << " " << xreload("xd") << " }\n";
            }
            out_ << "        switch (land_.target) {\n";
            for (uint32_t l : f.labels)
                if (f.insns.count(l)) out_ << "        case " << hex(l) << "u: goto " << lname(l) << ";\n";
            out_ << "        }\n"
                 << "        SPILL; recomp_fatal(c, land_.target, \"catch continuation outside the function\"); return;\n"
                 << "    }\n";
        }
        // Blocks are emitted in address order; a backward jump can pull in code that sits
        // below the entry (e.g. __security_check_cookie's failure path), so start at the entry.
        if (!f.insns.empty() && *f.insns.begin() != f.entry) out_ << "    goto " << lname(f.entry) << ";\n";
        uint32_t expected = 0;
        bool first = true;
        for (uint32_t a : f.insns) {
            if (first || a != expected || f.labels.count(a)) {
                if (!first && a != expected) {
                    // Previous run fell off without a terminator: continue at `expected`.
                    if (!terminated_) out_ << "    goto " << target(expected) << ";\n";
                }
                out_ << lname(a) << ":;\n";
            }
            first = false;
            terminated_ = false;
            if (xlFn_) {
                // An instruction the analysis never reached is dead in the C code as well
                // (e.g. bytes after an unsupported instruction): memory model.
                auto it = xin_.find(a);
                xl_ = it != xin_.end();
                xs_ = xl_ ? it->second : XState{};
                xd_ = xs_.d;
                auto ot = xout_.find(a);
                xoutD_ = ot == xout_.end() ? xs_.d : ot->second;
            }
            const Insn* ins = p_.decode(a);
            if (!ins) {
                ++st_.decodeErrors;
                out_ << "    SPILL; " << (xl_ ? xspill(xs_) : "") << "recomp_fatal(c, " << hex(a) << ", \"undecodable\"); return;\n";
                terminated_ = true;
                expected = a + 1;
                continue;
            }
            ++st_.insns;
            emit(*ins);
            expected = ins->next();
        }
        if (!terminated_) out_ << "    goto " << target(expected) << ";\n";
        xl_ = xlFn_;
        // Labels that are other functions' entries (tail calls / fall-through).
        for (uint32_t l : f.labels) {
            if (!f.insns.count(l)) {
                out_ << lname(l) << ":;\n";
                std::string xsp;
                if (xl_) {
                    auto it = xin_.find(l);
                    if (it != xin_.end()) xsp = xspill(it->second);
                }
                if (p_.isEntry(l) && p_.functions().count(l))
                    out_ << "    SPILL; " << xsp << fname(l) << "(c); return;\n";
                else
                    out_ << "    SPILL; " << xsp << "recomp_fatal(c, " << hex(l) << ", \"jump outside lifted code\"); return;\n";
            }
        }
        out_ << "}\n\n";
        return out_.str();
    }

    std::set<uint32_t> called;  // direct call / tail targets referenced
    uint32_t retBytes = 0;      // largest `ret N` in the last emitted function

private:
    // Label to jump to for an intra-function target, or a tail-call stub label.
    std::string target(uint32_t a) {
        // Every referenced address needs a label: an instruction emitted later in
        // this function, or (if outside it) a tail-call / trap stub.
        f_->labels.insert(a);
        if (!f_->insns.count(a) && p_.isEntry(a)) called.insert(a);
        if (dry_) drySuccs_.push_back(a);
        return lname(a);
    }

    void line(const std::string& s) {
        if (dry_) {
            dryText_ += s;
            dryText_ += '\n';
            return;
        }
        out_ << "    " << (xl_ ? xsync(s) : s) << "\n";
    }

    void unsupported(const Insn& ins) {
        const std::string m = ZydisMnemonicGetString(ins.in.mnemonic);
        if (!dry_) {
            st_.unsupported[m]++;
            ++st_.unsupportedInsns;
        }
        line("SPILL; recomp_fatal(c, " + hex(ins.addr) + ", \"unsupported " + m + "\"); return;");
        terminated_ = true;
    }

    // ---- x87 local model ----
    //
    // Memory model (--no-x87-locals): every x87 operation goes through c->fpu (ST(i),
    // fpush, fpop): TOP is loaded, masked and stored per operation, and since lifted code
    // passes `c` to its callees the C compiler must assume any guest store may alias
    // c->fpu, so registers and the control word are reloaded after every store (on the A53
    // half the instructions of the animation and particle functions).
    // Local model (every function with x87 code whose depths solve, see solveDepths): the
    // eight registers live in C locals xs0..xs7, named relative to a runtime base `xbase`.
    // At an instruction where `d` values have been pushed since the base was taken (a
    // compile-time number), ST(i) is xs[(i - d) & 7], the image of
    // c->fpu.st[(xbase + ((i - d) & 7)) & 7], and TOP is (xbase - d) & 7. CW and SW are
    // locals too (CW is written through on fldcw). At every SPILL the registers written
    // since the last sync, TOP (if anything was pushed or popped) and SW (if written) are
    // stored to c->fpu; after every RELOAD the base is retaken as xbase = c->fpu.top + d
    // and the registers are reloaded, which absorbs whatever the callee did to the stack
    // (a float result pushed, say). So c->fpu holds exactly what the memory model would
    // hold at every call, return, indirect branch and trap, and the C expressions computing
    // each value are the same: the two models give bit-identical results.
    struct XState {
        int d = 0;            // values pushed since the base was taken
        uint8_t dirty = 0;    // registers (base-relative) written since the last sync
        bool top = false;     // pushed or popped since the last sync
        bool sw = false;      // status word written since the last sync
    };
    struct DryInfo {
        int dOut = 0;
        uint8_t used = 0, written = 0;
        bool moved = false, swWritten = false, sync = false, reload = false, term = false;
        std::vector<uint32_t> succs;
    };

    static bool isX87(const Insn& ins) {
        return ins.in.meta.category == ZYDIS_CATEGORY_X87_ALU || ins.in.meta.category == ZYDIS_CATEGORY_FCMOV;
    }
    static bool isFtol2Call(const Insn& ins) {
        if (ins.in.mnemonic != ZYDIS_MNEMONIC_CALL || ins.op[0].type != ZYDIS_OPERAND_TYPE_IMMEDIATE) return false;
        uint64_t t = 0;
        ZydisCalcAbsoluteAddress(&ins.in, &ins.op[0], ins.addr, &t);
        return ftol2Inline().count(static_cast<uint32_t>(t)) != 0;
    }

    // Emit `ins` into a scratch buffer at depth `d` to learn its x87 effect and successors.
    DryInfo dryRun(const Insn& ins, int d) {
        dry_ = true;
        dryText_.clear();
        drySuccs_.clear();
        terminated_ = false;
        xd_ = d;
        xread_ = xwritten_ = 0;
        xmoved_ = xswWritten_ = xsync_ = false;
        emit(ins);
        DryInfo r;
        r.dOut = xd_;
        r.used = uint8_t(xread_ | xwritten_);
        r.written = xwritten_;
        r.moved = xmoved_;
        r.swWritten = xswWritten_;
        r.sync = xsync_;
        r.reload = dryText_.find("RELOAD;") != std::string::npos;
        r.term = terminated_;
        r.succs = drySuccs_;
        dry_ = false;
        terminated_ = false;
        return r;
    }

    // Dataflow over the emitted control flow (dry runs of every instruction give its x87
    // effect and successors). False: the function stays in the memory model.
    bool analyzeX87(Function& f) {
        bool any = false;
        for (uint32_t a : f.insns)
            if (const Insn* ins = p_.decode(a); ins && (isX87(*ins) || isFtol2Call(*ins))) { any = true; break; }
        if (!any) return false;
        const auto savedLabels = f.labels;  // the dry runs add the labels the real pass adds later
        const auto savedCalled = called;
        const uint32_t savedRet = retBytes;
        xl_ = true;
        const bool ok = solveDepths(f, savedLabels) && dirtyFlow(f, savedLabels);
        f.labels = savedLabels;
        called = savedCalled;
        retBytes = savedRet;
        xl_ = false;
        if (!ok) {
            ++x87Fallback;
            xin_.clear();
            xout_.clear();
            return false;
        }
        ++x87Local;
        return true;
    }

    // Pass 1: the depth at every instruction. Within a run between syncs the depth moves by
    // each instruction's static push/pop count; after a sync (call, indirect call, cpuid,
    // fsincos/fptan/fninit) the base is retaken, so the depth there is free: a new variable.
    // Paths meeting at a label equate their depths (a weighted union-find); a contradiction
    // (a cycle whose pushes do not balance) keeps the function in the memory model.
    // Fills xin_[a].d and xout_.
    bool solveDepths(Function& f, const std::set<uint32_t>& labels) {
        std::vector<int> parent, poff;  // value(v) = value(parent[v]) + poff[v]
        auto newVar = [&]() { parent.push_back(static_cast<int>(parent.size())); poff.push_back(0); return static_cast<int>(parent.size()) - 1; };
        auto find = [&](int v) {  // -> root, value(v) - value(root)
            int off = 0, r = v;
            while (parent[r] != r) { off += poff[r]; r = parent[r]; }
            int o2 = off;  // path compression
            while (parent[v] != v) { const int nx = parent[v], no = o2 - poff[v]; parent[v] = r; poff[v] = o2; v = nx; o2 = no; }
            return std::pair<int, int>(r, off);
        };
        struct Sym { int var, off; };
        std::map<uint32_t, Sym> sym;
        std::map<uint32_t, Sym> outSym;  // sync instructions: depth after
        std::deque<uint32_t> work;
        bool ok = true;
        auto visit = [&](uint32_t t, Sym s) {
            auto it = sym.find(t);
            if (it == sym.end()) {
                sym.emplace(t, s);
                work.push_back(t);
                return;
            }
            const auto [ra, xa] = find(it->second.var);
            const auto [rb, xb] = find(s.var);
            const int va = xa + it->second.off, vb = xb + s.off;  // relative to the roots
            if (ra == rb) {
                if (va != vb) {
                    if (ok && std::getenv("RECOMP_X87_DEBUG"))
                        std::cerr << "x87 depth conflict in " << fname(f.entry) << " at " << hex(t) << ": " << va << " vs " << vb << "\n";
                    ok = false;
                }
                return;
            }
            parent[rb] = ra;  // value(rb) = value(ra) + va - vb
            poff[rb] = va - vb;
        };
        std::map<uint32_t, DryInfo> dry;
        visit(f.entry, Sym{newVar(), 0});
        for (int pass = 0; ok && pass < 2; ++pass) {
            while (ok && !work.empty()) {
                const uint32_t a = work.front();
                work.pop_front();
                if (!f.insns.count(a)) continue;  // tail-call / trap stub: spills, never continues
                const Insn* ins = p_.decode(a);
                if (!ins) continue;
                const Sym s = sym[a];
                auto it = dry.find(a);
                if (it == dry.end()) it = dry.emplace(a, dryRun(*ins, 0)).first;  // pushes/pops do not depend on the depth
                const DryInfo& di = it->second;
                Sym o{s.var, s.off + di.dOut};
                if (di.sync || di.reload) {
                    o = Sym{newVar(), 0};
                    outSym[a] = o;
                }
                for (uint32_t t : di.succs) visit(t, o);
                if (!di.term) visit(ins->next(), o);
            }
            if (!f.eh) break;
            // EH: every label is a possible catch continuation, entered through the landing pad
            // with the base retaken; one reached only that way gets a depth of its own.
            for (uint32_t l : labels)
                if (f.insns.count(l) && !sym.count(l)) visit(l, Sym{newVar(), 0});
        }
        if (!ok) return false;
        // Concrete depths: the entry at 0, every other free root at 0.
        const auto [re, xe] = find(sym[f.entry].var);
        const int entryRootValue = -(xe + sym[f.entry].off);
        auto value = [&](const Sym& s) {
            const auto [r, x] = find(s.var);
            return (r == re ? entryRootValue : 0) + x + s.off;
        };
        xin_.clear();
        xout_.clear();
        for (const auto& [a, s] : sym) xin_[a].d = value(s);
        for (const auto& [a, s] : outSym) xout_[a] = value(s);
        return true;
    }

    // Pass 2: what is dirty (written since the last sync) at every instruction, at the depths
    // of pass 1, and which registers the function names. Fills xin_ / xused_.
    bool dirtyFlow(Function& f, const std::set<uint32_t>& labels) {
        std::map<uint32_t, XState> in;
        std::map<uint32_t, DryInfo> info;  // dry runs at the real depth (register names depend on it)
        std::deque<uint32_t> work;
        bool ok = true;
        uint8_t used = 0;
        auto merge = [&](uint32_t t, const XState& s) {
            auto it = in.find(t);
            if (it == in.end()) {
                in.emplace(t, s);
                work.push_back(t);
                return;
            }
            XState& o = it->second;
            if (o.d != s.d) { ok = false; return; }  // cannot happen after pass 1
            const XState n{o.d, uint8_t(o.dirty | s.dirty), o.top || s.top, o.sw || s.sw};
            if (n.dirty != o.dirty || n.top != o.top || n.sw != o.sw) {
                o = n;
                work.push_back(t);
            }
        };
        auto depth = [&](uint32_t a) { auto it = xin_.find(a); return it == xin_.end() ? 0 : it->second.d; };
        merge(f.entry, XState{depth(f.entry), 0, false, false});
        for (int pass = 0; ok && pass < 2; ++pass) {
            while (ok && !work.empty()) {
                const uint32_t a = work.front();
                work.pop_front();
                if (!f.insns.count(a)) continue;
                const Insn* ins = p_.decode(a);
                if (!ins) continue;
                const XState s = in[a];
                auto it = info.find(a);
                if (it == info.end()) it = info.emplace(a, dryRun(*ins, s.d)).first;
                const DryInfo& di = it->second;
                used |= di.used;
                XState o = s;
                if (di.sync || di.reload) {
                    o = XState{xout_.at(a), 0, false, false};
                } else {
                    o.d = di.dOut;
                    o.dirty |= di.written;
                    o.top = o.top || di.moved;
                    o.sw = o.sw || di.swWritten;
                }
                for (uint32_t t : di.succs) merge(t, o);
                if (!di.term) merge(ins->next(), o);
            }
            if (!f.eh) break;
            for (uint32_t l : labels)
                if (f.insns.count(l) && !in.count(l)) merge(l, XState{depth(l), 0, false, false});
        }
        if (!ok) return false;
        xin_ = std::move(in);
        xused_ = used;
        return true;
    }

    // Store what is dirty in state `s` to c->fpu (emitted after SPILL).
    std::string xspill(const XState& s) const {
        std::string r;
        for (int k = 0; k < 8; ++k)
            if (s.dirty >> k & 1) r += "c->fpu.st[(xbase + " + std::to_string(k) + "u) & 7u] = xs" + std::to_string(k) + "; ";
        if (s.top) r += "c->fpu.top = (xbase + " + std::to_string((-s.d) & 7) + "u) & 7u; ";
        if (s.sw) r += "c->fpu.sw = xsw; ";
        return r;
    }
    // Retake the base and reload the registers at depth `d` (an expression; emitted after RELOAD).
    std::string xreload(const std::string& d) const {
        std::string r = "xbase = (c->fpu.top + " + d + ") & 7u;";
        for (int k = 0; k < 8; ++k)
            if (xused_ >> k & 1) r += " xs" + std::to_string(k) + " = c->fpu.st[(xbase + " + std::to_string(k) + "u) & 7u];";
        return r + " xcw = c->fpu.cw; xsw = c->fpu.sw;";
    }
    std::string xreload(int d) const { return xreload(std::to_string(d & 7) + "u"); }
    // A line of the current instruction in the local model: x87 write-back after SPILL, retake after RELOAD.
    std::string xsync(const std::string& s) const {
        std::string r;
        r.reserve(s.size() + 96);
        for (size_t i = 0; i < s.size();) {
            if (s.compare(i, 6, "SPILL;") == 0) {
                const std::string w = xspill(xs_);
                r += w.empty() ? "SPILL;" : "SPILL; " + w.substr(0, w.size() - 1);
                i += 6;
            } else if (s.compare(i, 7, "RELOAD;") == 0) {
                r += "RELOAD; " + xreload(xoutD_);
                i += 7;
            } else {
                r += s[i++];
            }
        }
        return r;
    }

    // Names of ST(i) for reading / writing, push and pop, in the current model.
    std::string stR(int i) {
        if (!xl_) return "ST(" + std::to_string(i) + ")";
        const int k = (i - xd_) & 7;
        xread_ |= uint8_t(1u << k);
        return "xs" + std::to_string(k);
    }
    std::string stW(int i) {
        if (!xl_) return "ST(" + std::to_string(i) + ")";
        const int k = (i - xd_) & 7;
        xwritten_ |= uint8_t(1u << k);
        return "xs" + std::to_string(k);
    }
    std::string fpushS(const std::string& v) {
        if (!xl_) return "fpush(c, " + v + ");";
        ++xd_;
        xmoved_ = true;
        return stW(0) + " = " + v + ";";
    }
    std::string fpopS() {  // " fpop(c);" (leading space) in the memory model
        if (!xl_) return " fpop(c);";
        --xd_;
        xmoved_ = true;
        return "";
    }
    std::string prec(const std::string& v) const { return xl_ ? "fprec_cw(xcw, " + v + ")" : "fprec(c, " + v + ")"; }
    std::string cwArg() const { return xl_ ? "xcw" : "c"; }
    std::string cwFn(const std::string& f) const { return xl_ ? f + "_cw" : f; }
    std::string swRef(bool write) {
        if (!xl_) return "c->fpu.sw";
        if (write) xswWritten_ = true;
        return "xsw";
    }

public:
    uint64_t x87Local = 0, x87Fallback = 0;

private:

    // ---- registers & operands ----

    static std::string reg32(ZydisRegister r) {
        switch (ZydisRegisterGetLargestEnclosing(ZYDIS_MACHINE_MODE_LEGACY_32, r)) {
        case ZYDIS_REGISTER_EAX: return "eax";
        case ZYDIS_REGISTER_ECX: return "ecx";
        case ZYDIS_REGISTER_EDX: return "edx";
        case ZYDIS_REGISTER_EBX: return "ebx";
        case ZYDIS_REGISTER_ESP: return "esp";
        case ZYDIS_REGISTER_EBP: return "ebp";
        case ZYDIS_REGISTER_ESI: return "esi";
        case ZYDIS_REGISTER_EDI: return "edi";
        default: return "";
        }
    }
    static bool isHigh8(ZydisRegister r) {
        return r == ZYDIS_REGISTER_AH || r == ZYDIS_REGISTER_CH || r == ZYDIS_REGISTER_DH || r == ZYDIS_REGISTER_BH;
    }

    std::string readReg(ZydisRegister r) {
        const auto w = ZydisRegisterGetWidth(ZYDIS_MACHINE_MODE_LEGACY_32, r);
        const std::string n = reg32(r);
        if (n.empty()) return "0u /*" + std::string(ZydisRegisterGetString(r)) + "*/";
        if (w == 32) return n;
        if (w == 16) return "(uint32_t)(uint16_t)" + n;
        if (isHigh8(r)) return "(uint32_t)(uint8_t)(" + n + " >> 8)";
        return "(uint32_t)(uint8_t)" + n;
    }
    std::string writeReg(ZydisRegister r, const std::string& v) {
        const auto w = ZydisRegisterGetWidth(ZYDIS_MACHINE_MODE_LEGACY_32, r);
        const std::string n = reg32(r);
        if (w == 32) return n + " = (uint32_t)(" + v + ");";
        if (w == 16) return n + " = (" + n + " & 0xFFFF0000u) | (uint16_t)(" + v + ");";
        if (isHigh8(r)) return n + " = (" + n + " & 0xFFFF00FFu) | ((uint32_t)(uint8_t)(" + v + ") << 8);";
        return n + " = (" + n + " & 0xFFFFFF00u) | (uint8_t)(" + v + ");";
    }

    std::string addr(const ZydisDecodedOperand& o) {
        std::string s;
        auto add = [&](const std::string& t) { s += s.empty() ? t : " + " + t; };
        if (o.mem.segment == ZYDIS_REGISTER_FS) add("c->fs_base");
        if (o.mem.base != ZYDIS_REGISTER_NONE) add(reg32(o.mem.base));
        if (o.mem.index != ZYDIS_REGISTER_NONE)
            add(o.mem.scale > 1 ? reg32(o.mem.index) + " * " + std::to_string(o.mem.scale) + "u" : reg32(o.mem.index));
        if (o.mem.disp.value || s.empty()) add(hexu(static_cast<uint32_t>(o.mem.disp.value)));
        return "(uint32_t)(" + s + ")";
    }

    // A load or store of memory operand `o` through accessor `fn` (rd32, wr16, rdxmm...),
    // without the closing parenthesis; `pre` goes before the address argument. A base
    // and/or index register with a displacement below 64 KiB uses the host-pointer form
    // (`rd32p(MA(ecx, 60)`, see MA in recomp.h): the compiler folds the displacement into
    // the addressing mode. Absolute operands and large displacements keep the 32-bit sum.
    // `lea` is a value, not an access: it always uses addr().
    std::string mem(const std::string& fn, const ZydisDecodedOperand& o, const std::string& pre = "") {
        const int64_t d = o.mem.disp.value;
        const bool hasReg = o.mem.base != ZYDIS_REGISTER_NONE || o.mem.index != ZYDIS_REGISTER_NONE;
        if (!foldDisplacements() || !hasReg || d == 0 || d <= -0x10000 || d >= 0x10000 || o.mem.segment == ZYDIS_REGISTER_FS)
            return fn + "(" + pre + addr(o);
        std::string s;
        auto add = [&](const std::string& t) { s += s.empty() ? t : " + " + t; };
        if (o.mem.base != ZYDIS_REGISTER_NONE) add(reg32(o.mem.base));
        if (o.mem.index != ZYDIS_REGISTER_NONE)
            add(o.mem.scale > 1 ? reg32(o.mem.index) + " * " + std::to_string(o.mem.scale) + "u" : reg32(o.mem.index));
        return fn + "p(" + pre + "MA(" + s + ", " + std::to_string(d) + ")";
    }

    static uint32_t mask(int bits) { return bits >= 32 ? 0xFFFFFFFFu : ((1u << bits) - 1u); }

    // Value of operand as uint32 (zero-extended from `bits`).
    std::string rd(const ZydisDecodedOperand& o, int bits) {
        switch (o.type) {
        case ZYDIS_OPERAND_TYPE_REGISTER: return readReg(o.reg.value);
        case ZYDIS_OPERAND_TYPE_MEMORY:
            if (bits == 8) return "(uint32_t)" + mem("rd8", o) + ")";
            if (bits == 16) return "(uint32_t)" + mem("rd16", o) + ")";
            return mem("rd32", o) + ")";
        case ZYDIS_OPERAND_TYPE_IMMEDIATE:
            return hexu(static_cast<uint32_t>(o.imm.value.s) & mask(bits));
        default: return "0u";
        }
    }
    std::string wr(const ZydisDecodedOperand& o, int bits, const std::string& v) {
        if (o.type == ZYDIS_OPERAND_TYPE_REGISTER) return writeReg(o.reg.value, v);
        if (bits == 8) return mem("wr8", o) + ", (uint8_t)(" + v + "));";
        if (bits == 16) return mem("wr16", o) + ", (uint16_t)(" + v + "));";
        return mem("wr32", o) + ", (uint32_t)(" + v + "));";
    }

    static std::string bytes(int bits) { return std::to_string(bits / 8); }

    // ---- control flow helpers ----

    void callDirect(const Insn& ins, uint32_t t) {
        line("esp -= 4; wr32(esp, " + hex(ins.next()) + "u);");
        if (ftol2Inline().count(t)) {
            // host_ftol2 inline: pop ST(0), truncate to edx:eax, return (no spill, dispatch or reload).
            const std::string v = stR(0);
            line("{ int64_t r_ = ftol2_trunc(" + v + ");" + fpopS() + " eax = (uint32_t)r_; edx = (uint32_t)((uint64_t)r_ >> 32); esp += 4; }");
            called.insert(t);
            return;
        }
        if (!p_.functions().count(t)) {  // target is not code we lifted (garbage decode)
            line("SPILL; RECOMP_ICALL(c, " + hex(t) + "u); RELOAD;");  // resolved once through the inline cache, not a hash lookup per call
            return;
        }
        line("SPILL; " + fname(t) + "(c); RELOAD;");
        called.insert(t);
    }

    void emit(const Insn& ins) {
        const auto& in = ins.in;
        const auto* o = ins.op;
        const int w = in.operand_width ? in.operand_width : 32;
        const std::string n = bytes(w);
        const auto m = in.mnemonic;

        if (in.meta.category == ZYDIS_CATEGORY_X87_ALU || in.meta.category == ZYDIS_CATEGORY_FCMOV) {
            x87(ins);
            return;
        }
        if (simd(ins)) return;

        switch (m) {
        case ZYDIS_MNEMONIC_NOP: case ZYDIS_MNEMONIC_PAUSE: case ZYDIS_MNEMONIC_PREFETCHT0:
        case ZYDIS_MNEMONIC_PREFETCHT1: case ZYDIS_MNEMONIC_PREFETCHT2: case ZYDIS_MNEMONIC_PREFETCHNTA:
        case ZYDIS_MNEMONIC_FWAIT:
            return;
        case ZYDIS_MNEMONIC_MOV:
            if (o[0].type == ZYDIS_OPERAND_TYPE_REGISTER && ZydisRegisterGetClass(o[0].reg.value) == ZYDIS_REGCLASS_SEGMENT) return unsupported(ins);
            if (o[1].type == ZYDIS_OPERAND_TYPE_REGISTER && ZydisRegisterGetClass(o[1].reg.value) == ZYDIS_REGCLASS_SEGMENT)
                return line(wr(o[0], w, "0x23u"));  // reading a segment selector: harmless constant
            return line(wr(o[0], w, rd(o[1], w)));
        case ZYDIS_MNEMONIC_MOVZX:
            return line(wr(o[0], w, rd(o[1], o[1].size)));
        case ZYDIS_MNEMONIC_MOVSX: {
            const int sb = o[1].size;
            const std::string v = sb == 8 ? "(uint32_t)(int32_t)(int8_t)" + rd(o[1], 8) : "(uint32_t)(int32_t)(int16_t)" + rd(o[1], 16);
            return line(wr(o[0], w, v));
        }
        case ZYDIS_MNEMONIC_LEA:
            return line(wr(o[0], w, addr(o[1])));
        case ZYDIS_MNEMONIC_XCHG:
            return line("{ uint32_t t0 = " + rd(o[0], w) + ", t1 = " + rd(o[1], w) + "; " + wr(o[0], w, "t1") + " " + wr(o[1], w, "t0") + " }");
        case ZYDIS_MNEMONIC_PUSH: {
            const std::string v = o[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE ? hexu(static_cast<uint32_t>(o[0].imm.value.s))
                                  : (o[0].type == ZYDIS_OPERAND_TYPE_REGISTER && ZydisRegisterGetClass(o[0].reg.value) == ZYDIS_REGCLASS_SEGMENT)
                                      ? "0x23u"
                                      : rd(o[0], w);
            if (w == 16) return line("{ uint32_t v = " + v + "; esp -= 2; wr16(esp, (uint16_t)v); }");
            return line("{ uint32_t v = " + v + "; esp -= 4; wr32(esp, v); }");
        }
        case ZYDIS_MNEMONIC_POP:
            if (o[0].type == ZYDIS_OPERAND_TYPE_REGISTER && ZydisRegisterGetClass(o[0].reg.value) == ZYDIS_REGCLASS_SEGMENT)
                return line("esp += 4;");
            if (o[0].type == ZYDIS_OPERAND_TYPE_REGISTER && reg32(o[0].reg.value) == "esp") return line("esp = rd32(esp);");
            // x86 computes a memory destination after incrementing ESP.
            return line("{ uint32_t v = rd32(esp); esp += 4; " + wr(o[0], w, "v") + " }");
        case ZYDIS_MNEMONIC_PUSHAD:
            return line("{ uint32_t s = esp; esp -= 32; wr32(esp+28, eax); wr32(esp+24, ecx); wr32(esp+20, edx); wr32(esp+16, ebx); wr32(esp+12, s); wr32(esp+8, ebp); wr32(esp+4, esi); wr32(esp, edi); }");
        case ZYDIS_MNEMONIC_POPAD:
            return line("{ edi = rd32(esp); esi = rd32(esp+4); ebp = rd32(esp+8); ebx = rd32(esp+16); edx = rd32(esp+20); ecx = rd32(esp+24); eax = rd32(esp+28); esp += 32; }");
        case ZYDIS_MNEMONIC_PUSHFD:
            return line("{ uint32_t v = f_pack(fop, fr, fa, fb) | 0x202u | (c->df ? 0x400u : 0u) | c->extflags; esp -= 4; wr32(esp, v); }");
        case ZYDIS_MNEMONIC_POPFD:
            return line("{ uint32_t v = rd32(esp); esp += 4; c->df = (v >> 10) & 1u; c->extflags = v & 0x00240000u; fop = FOP(FK_EXPLICIT, 4); fr = v; }");
        case ZYDIS_MNEMONIC_LAHF:
            return line(writeReg(ZYDIS_REGISTER_AH, "f_pack(fop, fr, fa, fb) | 2u"));
        case ZYDIS_MNEMONIC_SAHF:
            return line("{ uint32_t v = (eax >> 8) & 0xD5u; fr = v | (f_of(fop, fr, fa, fb) ? EF_OF : 0u); fop = FOP(FK_EXPLICIT, 4); }");
        case ZYDIS_MNEMONIC_CLD: return line("c->df = 0;");
        case ZYDIS_MNEMONIC_STD: return line("c->df = 1;");
        case ZYDIS_MNEMONIC_CLC: return line("fr = f_pack(fop, fr, fa, fb) & ~EF_CF; fop = FOP(FK_EXPLICIT, 4);");
        case ZYDIS_MNEMONIC_STC: return line("fr = f_pack(fop, fr, fa, fb) | EF_CF; fop = FOP(FK_EXPLICIT, 4);");
        case ZYDIS_MNEMONIC_CMC: return line("fr = f_pack(fop, fr, fa, fb) ^ EF_CF; fop = FOP(FK_EXPLICIT, 4);");

        case ZYDIS_MNEMONIC_ADD: case ZYDIS_MNEMONIC_SUB: case ZYDIS_MNEMONIC_CMP: {
            const bool add = m == ZYDIS_MNEMONIC_ADD;
            std::string s = "{ uint32_t A = " + rd(o[0], w) + ", B = " + rd(o[1], w) + ", R = (A " + (add ? "+" : "-") + " B) & " +
                            hexu(mask(w)) + "; ";
            if (m != ZYDIS_MNEMONIC_CMP) s += wr(o[0], w, "R") + " ";
            s += "fop = FOP(" + std::string(add ? "FK_ADD" : "FK_SUB") + ", " + n + "); fr = R; fa = A; fb = B; }";
            return line(s);
        }
        case ZYDIS_MNEMONIC_ADC: case ZYDIS_MNEMONIC_SBB: {
            const bool adc = m == ZYDIS_MNEMONIC_ADC;
            return line("{ uint32_t A = " + rd(o[0], w) + ", B = " + rd(o[1], w) + ", C = (uint32_t)f_cf(fop, fr, fa, fb), R = (A " +
                        (adc ? "+ B + C" : "- B - C") + ") & " + hexu(mask(w)) + "; " + wr(o[0], w, "R") + " fr = " +
                        (adc ? "f_adc_flags" : "f_sbb_flags") + "(" + n + ", A, B, C, R); fop = FOP(FK_EXPLICIT, " + n + "); }");
        }
        case ZYDIS_MNEMONIC_AND: case ZYDIS_MNEMONIC_OR: case ZYDIS_MNEMONIC_XOR: case ZYDIS_MNEMONIC_TEST: {
            const char* opc = (m == ZYDIS_MNEMONIC_OR) ? "|" : (m == ZYDIS_MNEMONIC_XOR) ? "^" : "&";
            std::string s = "{ uint32_t R = (" + rd(o[0], w) + " " + opc + " " + rd(o[1], w) + ") & " + hexu(mask(w)) + "; ";
            if (m != ZYDIS_MNEMONIC_TEST) s += wr(o[0], w, "R") + " ";
            return line(s + "fop = FOP(FK_LOGIC, " + n + "); fr = R; }");
        }
        case ZYDIS_MNEMONIC_INC: case ZYDIS_MNEMONIC_DEC: {
            const bool inc = m == ZYDIS_MNEMONIC_INC;
            return line("{ uint32_t C = (uint32_t)f_cf(fop, fr, fa, fb), A = " + rd(o[0], w) + ", R = (A " + (inc ? "+" : "-") + " 1u) & " +
                        hexu(mask(w)) + "; " + wr(o[0], w, "R") + " fop = FOP(" + (inc ? "FK_INC" : "FK_DEC") + ", " + n +
                        "); fr = R; fa = A; fb = C; }");
        }
        case ZYDIS_MNEMONIC_NEG:
            return line("{ uint32_t A = " + rd(o[0], w) + ", R = (0u - A) & " + hexu(mask(w)) + "; " + wr(o[0], w, "R") +
                        " fop = FOP(FK_NEG, " + n + "); fr = R; fa = A; fb = 0; }");
        case ZYDIS_MNEMONIC_NOT:
            return line(wr(o[0], w, "~" + rd(o[0], w)));
        case ZYDIS_MNEMONIC_SHL: case ZYDIS_MNEMONIC_SHR: case ZYDIS_MNEMONIC_SAR:
        case ZYDIS_MNEMONIC_ROL: case ZYDIS_MNEMONIC_ROR:
            return shift(ins, w);
        case ZYDIS_MNEMONIC_RCL: case ZYDIS_MNEMONIC_RCR: {
            const bool left = m == ZYDIS_MNEMONIC_RCL;
            return line("{ uint32_t v = " + rd(o[0], w) + ", cf = (uint32_t)f_cf(fop, fr, fa, fb); unsigned k = (" + rd(o[1], 8) +
                        ") & 31u; for (unsigned i = 0; i < k; ++i) { uint32_t out = " +
                        (left ? "(v >> " + std::to_string(w - 1) + ") & 1u; v = ((v << 1) | cf) & " + hexu(mask(w)) + ";"
                              : "v & 1u; v = (v >> 1) | (cf << " + std::to_string(w - 1) + ");") +
                        " cf = out; } " + wr(o[0], w, "v") + " if (k) { fr = (f_pack(fop, fr, fa, fb) & ~EF_CF) | cf; fop = FOP(FK_EXPLICIT, 4); } }");
        }
        case ZYDIS_MNEMONIC_SHLD: case ZYDIS_MNEMONIC_SHRD: {
            const bool left = m == ZYDIS_MNEMONIC_SHLD;
            return line("{ unsigned k = (" + rd(o[2], 8) + ") & 31u; if (k) { uint32_t d = " + rd(o[0], 32) + ", s = " + rd(o[1], 32) +
                        ", r = " + (left ? "(d << k) | (s >> (32u - k))" : "(d >> k) | (s << (32u - k))") + "; " + wr(o[0], 32, "r") +
                        " fr = (r == 0 ? EF_ZF : 0u) | ((r & 0x80000000u) ? EF_SF : 0u) | (f_pf(FOP(FK_LOGIC,4), r) ? EF_PF : 0u) | (((d >> " +
                        (left ? "(32u - k)" : "(k - 1u)") + ") & 1u) ? EF_CF : 0u); fop = FOP(FK_EXPLICIT, 4); } }");
        }
        case ZYDIS_MNEMONIC_MUL:
        case ZYDIS_MNEMONIC_IMUL:
            return mul(ins, w);
        case ZYDIS_MNEMONIC_DIV: case ZYDIS_MNEMONIC_IDIV: {
            const bool sgn = m == ZYDIS_MNEMONIC_IDIV;
            const std::string d = rd(o[0], w);
            if (w == 32) {
                if (sgn)
                    return line("{ int64_t N = (int64_t)(((uint64_t)edx << 32) | eax); int32_t D = (int32_t)" + d +
                                "; if (D == 0) { SPILL; recomp_fatal(c, " + hex(ins.addr) + ", \"divide by zero\"); return; } eax = (uint32_t)(int32_t)(N / D); edx = (uint32_t)(int32_t)(N % D); }");
                return line("{ uint64_t N = ((uint64_t)edx << 32) | eax; uint32_t D = " + d + "; if (D == 0) { SPILL; recomp_fatal(c, " +
                            hex(ins.addr) + ", \"divide by zero\"); return; } eax = (uint32_t)(N / D); edx = (uint32_t)(N % D); }");
            }
            if (w == 16) {
                if (sgn)
                    return line("{ int32_t N = (int32_t)(((edx & 0xFFFFu) << 16) | (eax & 0xFFFFu)); int16_t D = (int16_t)" + d +
                                "; if (D == 0) { SPILL; recomp_fatal(c, " + hex(ins.addr) + ", \"divide by zero\"); return; } " +
                                writeReg(ZYDIS_REGISTER_AX, "N / D") + " " + writeReg(ZYDIS_REGISTER_DX, "N % D") + " }");
                return line("{ uint32_t N = ((edx & 0xFFFFu) << 16) | (eax & 0xFFFFu); uint32_t D = " + d + "; if (D == 0) { SPILL; recomp_fatal(c, " +
                            hex(ins.addr) + ", \"divide by zero\"); return; } " + writeReg(ZYDIS_REGISTER_AX, "N / D") + " " +
                            writeReg(ZYDIS_REGISTER_DX, "N % D") + " }");
            }
            if (sgn)
                return line("{ int16_t N = (int16_t)eax; int8_t D = (int8_t)" + d + "; if (D == 0) { SPILL; recomp_fatal(c, " + hex(ins.addr) +
                            ", \"divide by zero\"); return; } " + writeReg(ZYDIS_REGISTER_AL, "N / D") + " " + writeReg(ZYDIS_REGISTER_AH, "N % D") + " }");
            return line("{ uint32_t N = eax & 0xFFFFu, D = " + d + "; if (D == 0) { SPILL; recomp_fatal(c, " + hex(ins.addr) +
                        ", \"divide by zero\"); return; } " + writeReg(ZYDIS_REGISTER_AL, "N / D") + " " + writeReg(ZYDIS_REGISTER_AH, "N % D") + " }");
        }
        case ZYDIS_MNEMONIC_CDQ: return line("edx = (eax & 0x80000000u) ? 0xFFFFFFFFu : 0u;");
        case ZYDIS_MNEMONIC_CWDE: return line("eax = (uint32_t)(int32_t)(int16_t)eax;");
        case ZYDIS_MNEMONIC_CBW: return line(writeReg(ZYDIS_REGISTER_AX, "(uint16_t)(int16_t)(int8_t)eax"));
        case ZYDIS_MNEMONIC_CWD: return line(writeReg(ZYDIS_REGISTER_DX, "(eax & 0x8000u) ? 0xFFFFu : 0u"));
        case ZYDIS_MNEMONIC_BSWAP: return line(wr(o[0], 32, "__builtin_bswap32(" + rd(o[0], 32) + ")"));
        case ZYDIS_MNEMONIC_BT: case ZYDIS_MNEMONIC_BTS: case ZYDIS_MNEMONIC_BTR: case ZYDIS_MNEMONIC_BTC: {
            if (o[0].type != ZYDIS_OPERAND_TYPE_REGISTER) return unsupported(ins);
            std::string s = "{ unsigned k = (" + rd(o[1], w) + ") & " + std::to_string(w - 1) + "u; uint32_t v = " + rd(o[0], w) +
                            "; uint32_t cf = (v >> k) & 1u; ";
            if (m == ZYDIS_MNEMONIC_BTS) s += wr(o[0], w, "v | (1u << k)") + " ";
            if (m == ZYDIS_MNEMONIC_BTR) s += wr(o[0], w, "v & ~(1u << k)") + " ";
            if (m == ZYDIS_MNEMONIC_BTC) s += wr(o[0], w, "v ^ (1u << k)") + " ";
            return line(s + "fr = (f_pack(fop, fr, fa, fb) & ~EF_CF) | cf; fop = FOP(FK_EXPLICIT, 4); }");
        }
        case ZYDIS_MNEMONIC_BSF: case ZYDIS_MNEMONIC_BSR: {
            const bool f = m == ZYDIS_MNEMONIC_BSF;
            return line("{ uint32_t v = " + rd(o[1], w) + "; if (v) { " + wr(o[0], w, f ? "__builtin_ctz(v)" : "31u - __builtin_clz(v)") +
                        " } fop = FOP(FK_LOGIC, 4); fr = v; }");
        }
        case ZYDIS_MNEMONIC_XADD:
            return line("{ uint32_t A = " + rd(o[0], w) + ", B = " + rd(o[1], w) + ", R = (A + B) & " + hexu(mask(w)) + "; " + wr(o[1], w, "A") +
                        " " + wr(o[0], w, "R") + " fop = FOP(FK_ADD, " + n + "); fr = R; fa = A; fb = B; }");
        case ZYDIS_MNEMONIC_CMPXCHG: {
            const std::string acc = w == 32 ? "eax" : w == 16 ? "(eax & 0xFFFFu)" : "(eax & 0xFFu)";
            const ZydisRegister accReg = w == 32 ? ZYDIS_REGISTER_EAX : w == 16 ? ZYDIS_REGISTER_AX : ZYDIS_REGISTER_AL;
            return line("{ uint32_t A = " + acc + ", D = " + rd(o[0], w) + ", R = (A - D) & " + hexu(mask(w)) + "; fop = FOP(FK_SUB, " + n +
                        "); fr = R; fa = A; fb = D; if (R == 0) { " + wr(o[0], w, rd(o[1], w)) + " } else { " + writeReg(accReg, "D") + " } }");
        }
        case ZYDIS_MNEMONIC_LEAVE:
            return line("esp = ebp; ebp = rd32(esp); esp += 4;");
        case ZYDIS_MNEMONIC_ENTER:
            if (o[1].imm.value.u != 0) return unsupported(ins);
            return line("esp -= 4; wr32(esp, ebp); ebp = esp; esp -= " + hexu(o[0].imm.value.u & 0xFFFF) + ";");

        case ZYDIS_MNEMONIC_SETO: case ZYDIS_MNEMONIC_SETNO: case ZYDIS_MNEMONIC_SETB: case ZYDIS_MNEMONIC_SETNB:
        case ZYDIS_MNEMONIC_SETZ: case ZYDIS_MNEMONIC_SETNZ: case ZYDIS_MNEMONIC_SETBE: case ZYDIS_MNEMONIC_SETNBE:
        case ZYDIS_MNEMONIC_SETS: case ZYDIS_MNEMONIC_SETNS: case ZYDIS_MNEMONIC_SETP: case ZYDIS_MNEMONIC_SETNP:
        case ZYDIS_MNEMONIC_SETL: case ZYDIS_MNEMONIC_SETNL: case ZYDIS_MNEMONIC_SETLE: case ZYDIS_MNEMONIC_SETNLE:
            return line(wr(o[0], 8, "f_cond(" + std::to_string(cc(m)) + ", fop, fr, fa, fb)"));
        case ZYDIS_MNEMONIC_CMOVO: case ZYDIS_MNEMONIC_CMOVNO: case ZYDIS_MNEMONIC_CMOVB: case ZYDIS_MNEMONIC_CMOVNB:
        case ZYDIS_MNEMONIC_CMOVZ: case ZYDIS_MNEMONIC_CMOVNZ: case ZYDIS_MNEMONIC_CMOVBE: case ZYDIS_MNEMONIC_CMOVNBE:
        case ZYDIS_MNEMONIC_CMOVS: case ZYDIS_MNEMONIC_CMOVNS: case ZYDIS_MNEMONIC_CMOVP: case ZYDIS_MNEMONIC_CMOVNP:
        case ZYDIS_MNEMONIC_CMOVL: case ZYDIS_MNEMONIC_CMOVNL: case ZYDIS_MNEMONIC_CMOVLE: case ZYDIS_MNEMONIC_CMOVNLE:
            return line("if (f_cond(" + std::to_string(cc(m)) + ", fop, fr, fa, fb)) { " + wr(o[0], w, rd(o[1], w)) + " }");

        case ZYDIS_MNEMONIC_JO: case ZYDIS_MNEMONIC_JNO: case ZYDIS_MNEMONIC_JB: case ZYDIS_MNEMONIC_JNB:
        case ZYDIS_MNEMONIC_JZ: case ZYDIS_MNEMONIC_JNZ: case ZYDIS_MNEMONIC_JBE: case ZYDIS_MNEMONIC_JNBE:
        case ZYDIS_MNEMONIC_JS: case ZYDIS_MNEMONIC_JNS: case ZYDIS_MNEMONIC_JP: case ZYDIS_MNEMONIC_JNP:
        case ZYDIS_MNEMONIC_JL: case ZYDIS_MNEMONIC_JNL: case ZYDIS_MNEMONIC_JLE: case ZYDIS_MNEMONIC_JNLE: {
            uint64_t t = 0;
            ZydisCalcAbsoluteAddress(&in, &o[0], ins.addr, &t);
            return line("if (f_cond(" + std::to_string(cc(m)) + ", fop, fr, fa, fb)) goto " + target(static_cast<uint32_t>(t)) + ";");
        }
        case ZYDIS_MNEMONIC_JECXZ: case ZYDIS_MNEMONIC_JCXZ: {
            uint64_t t = 0;
            ZydisCalcAbsoluteAddress(&in, &o[0], ins.addr, &t);
            return line(std::string("if (") + (m == ZYDIS_MNEMONIC_JCXZ ? "(ecx & 0xFFFFu)" : "ecx") + " == 0) goto " +
                        target(static_cast<uint32_t>(t)) + ";");
        }
        case ZYDIS_MNEMONIC_LOOP: case ZYDIS_MNEMONIC_LOOPE: case ZYDIS_MNEMONIC_LOOPNE: {
            uint64_t t = 0;
            ZydisCalcAbsoluteAddress(&in, &o[0], ins.addr, &t);
            std::string cond = "--ecx != 0";
            if (m == ZYDIS_MNEMONIC_LOOPE) cond = "--ecx != 0 && f_zf(fop, fr)";
            if (m == ZYDIS_MNEMONIC_LOOPNE) cond = "--ecx != 0 && !f_zf(fop, fr)";
            return line("if (" + cond + ") goto " + target(static_cast<uint32_t>(t)) + ";");
        }
        case ZYDIS_MNEMONIC_JMP: {
            terminated_ = true;
            if (o[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE) {
                uint64_t t = 0;
                ZydisCalcAbsoluteAddress(&in, &o[0], ins.addr, &t);
                return line("goto " + target(static_cast<uint32_t>(t)) + ";");
            }
            auto jt = f_->tables.find(ins.addr);
            if (jt != f_->tables.end()) {
                std::string s = "switch (" + rd(o[0], 32) + ") {";
                std::set<uint32_t> done;
                for (uint32_t t : jt->second.targets)
                    if (done.insert(t).second) s += " case " + hexu(t) + ": goto " + target(t) + ";";
                return line(s + " default: { uint32_t t = " + rd(o[0], 32) + "; SPILL; RECOMP_ICALL(c, t); return; } }");
            }
            if (!dry_) ++st_.unresolvedIndirectJumps;
            return line("{ uint32_t t = " + rd(o[0], 32) + "; SPILL; RECOMP_ICALL(c, t); return; }");
        }
        case ZYDIS_MNEMONIC_CALL: {
            if (o[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE) {
                uint64_t t = 0;
                ZydisCalcAbsoluteAddress(&in, &o[0], ins.addr, &t);
                const auto tgt = static_cast<uint32_t>(t);
                if (tgt == ins.next())  // call $+5; pop reg  (get EIP)
                    return line("esp -= 4; wr32(esp, " + hex(tgt) + "u);");
                return callDirect(ins, tgt);
            }
            return line("{ uint32_t t = " + rd(o[0], 32) + "; esp -= 4; wr32(esp, " + hex(ins.next()) +
                        "u); SPILL; RECOMP_ICALL(c, t); RELOAD; }");
        }
        case ZYDIS_MNEMONIC_RET:
            terminated_ = true;
            retBytes = std::max<uint32_t>(retBytes, in.operand_count_visible ? static_cast<uint32_t>(o[0].imm.value.u) : 0u);
            return line("esp += " + std::to_string(4 + (in.operand_count_visible ? o[0].imm.value.u : 0)) + "; SPILL; return;");
        case ZYDIS_MNEMONIC_INT3: case ZYDIS_MNEMONIC_HLT: case ZYDIS_MNEMONIC_UD2: case ZYDIS_MNEMONIC_INT:
            terminated_ = true;
            return line("SPILL; recomp_fatal(c, " + hex(ins.addr) + ", \"trap instruction\"); return;");
        case ZYDIS_MNEMONIC_CPUID:
            return line("SPILL; recomp_cpuid(c); RELOAD;");
        case ZYDIS_MNEMONIC_RDTSC:
            return line("{ uint64_t t = recomp_rdtsc(); eax = (uint32_t)t; edx = (uint32_t)(t >> 32); }");
        case ZYDIS_MNEMONIC_MOVSB: case ZYDIS_MNEMONIC_MOVSW: case ZYDIS_MNEMONIC_MOVSD:
        case ZYDIS_MNEMONIC_STOSB: case ZYDIS_MNEMONIC_STOSW: case ZYDIS_MNEMONIC_STOSD:
        case ZYDIS_MNEMONIC_LODSB: case ZYDIS_MNEMONIC_LODSW: case ZYDIS_MNEMONIC_LODSD:
        case ZYDIS_MNEMONIC_CMPSB: case ZYDIS_MNEMONIC_CMPSW: case ZYDIS_MNEMONIC_CMPSD:
        case ZYDIS_MNEMONIC_SCASB: case ZYDIS_MNEMONIC_SCASW: case ZYDIS_MNEMONIC_SCASD:
            return strings(ins);
        default:
            return unsupported(ins);
        }
    }

    static int cc(ZydisMnemonic m) {
        switch (m) {
        case ZYDIS_MNEMONIC_JO: case ZYDIS_MNEMONIC_SETO: case ZYDIS_MNEMONIC_CMOVO: return 0x0;
        case ZYDIS_MNEMONIC_JNO: case ZYDIS_MNEMONIC_SETNO: case ZYDIS_MNEMONIC_CMOVNO: return 0x1;
        case ZYDIS_MNEMONIC_JB: case ZYDIS_MNEMONIC_SETB: case ZYDIS_MNEMONIC_CMOVB: return 0x2;
        case ZYDIS_MNEMONIC_JNB: case ZYDIS_MNEMONIC_SETNB: case ZYDIS_MNEMONIC_CMOVNB: return 0x3;
        case ZYDIS_MNEMONIC_JZ: case ZYDIS_MNEMONIC_SETZ: case ZYDIS_MNEMONIC_CMOVZ: return 0x4;
        case ZYDIS_MNEMONIC_JNZ: case ZYDIS_MNEMONIC_SETNZ: case ZYDIS_MNEMONIC_CMOVNZ: return 0x5;
        case ZYDIS_MNEMONIC_JBE: case ZYDIS_MNEMONIC_SETBE: case ZYDIS_MNEMONIC_CMOVBE: return 0x6;
        case ZYDIS_MNEMONIC_JNBE: case ZYDIS_MNEMONIC_SETNBE: case ZYDIS_MNEMONIC_CMOVNBE: return 0x7;
        case ZYDIS_MNEMONIC_JS: case ZYDIS_MNEMONIC_SETS: case ZYDIS_MNEMONIC_CMOVS: return 0x8;
        case ZYDIS_MNEMONIC_JNS: case ZYDIS_MNEMONIC_SETNS: case ZYDIS_MNEMONIC_CMOVNS: return 0x9;
        case ZYDIS_MNEMONIC_JP: case ZYDIS_MNEMONIC_SETP: case ZYDIS_MNEMONIC_CMOVP: return 0xA;
        case ZYDIS_MNEMONIC_JNP: case ZYDIS_MNEMONIC_SETNP: case ZYDIS_MNEMONIC_CMOVNP: return 0xB;
        case ZYDIS_MNEMONIC_JL: case ZYDIS_MNEMONIC_SETL: case ZYDIS_MNEMONIC_CMOVL: return 0xC;
        case ZYDIS_MNEMONIC_JNL: case ZYDIS_MNEMONIC_SETNL: case ZYDIS_MNEMONIC_CMOVNL: return 0xD;
        case ZYDIS_MNEMONIC_JLE: case ZYDIS_MNEMONIC_SETLE: case ZYDIS_MNEMONIC_CMOVLE: return 0xE;
        default: return 0xF;
        }
    }

    void shift(const Insn& ins, int w) {
        const auto* o = ins.op;
        const auto m = ins.in.mnemonic;
        const std::string n = bytes(w);
        const std::string cnt = "(" + rd(o[1], 8) + ") & 31u";
        const std::string mk = hexu(mask(w));
        std::string body;
        switch (m) {
        case ZYDIS_MNEMONIC_SHL:
            body = "R = (A << k) & " + mk + "; fop = FOP(FK_SHL, " + n + ");";
            break;
        case ZYDIS_MNEMONIC_SHR:
            body = "R = (A >> k) & " + mk + "; fop = FOP(FK_SHR, " + n + ");";
            break;
        case ZYDIS_MNEMONIC_SAR: {
            const std::string sx = w == 32 ? "(int32_t)A" : w == 16 ? "(int32_t)(int16_t)A" : "(int32_t)(int8_t)A";
            body = "R = (uint32_t)(" + sx + " >> (k > " + std::to_string(w - 1) + "u ? " + std::to_string(w - 1) + "u : k)) & " + mk +
                   "; fop = FOP(FK_SAR, " + n + "); A = (uint32_t)" + sx + ";";
            break;
        }
        case ZYDIS_MNEMONIC_ROL:
            body = std::string("R = ") + (w == 32 ? "rol32(A, k)" : w == 16 ? "rol16((uint16_t)A, k)" : "rol8((uint8_t)A, k)") +
                   "; fop = FOP(FK_ROL, " + n + ");";
            break;
        default:
            body = std::string("R = ") + (w == 32 ? "ror32(A, k)" : w == 16 ? "ror16((uint16_t)A, k)" : "ror8((uint8_t)A, k)") +
                   "; fop = FOP(FK_ROR, " + n + ");";
            break;
        }
        line("{ unsigned k = " + cnt + "; uint32_t A = " + rd(o[0], w) + ", R; if (k) { " + body + " " + wr(o[0], w, "R") +
             " fr = R; fa = A; fb = k; } }");
    }

    void mul(const Insn& ins, int w) {
        const auto* o = ins.op;
        const bool sgn = ins.in.mnemonic == ZYDIS_MNEMONIC_IMUL;
        const int vis = ins.in.operand_count_visible;
        const std::string n = bytes(w);
        if (vis >= 2) {  // imul r, r/m [, imm]
            const std::string a = vis == 3 ? rd(o[1], w) : rd(o[0], w);
            const std::string b = vis == 3 ? rd(o[2], w) : rd(o[1], w);
            const std::string sx = w == 16 ? "(int64_t)(int16_t)" : "(int64_t)(int32_t)";
            const std::string tr = w == 16 ? "(int64_t)(int16_t)P" : "(int64_t)(int32_t)P";
            return line("{ int64_t P = " + sx + "(" + a + ") * " + sx + "(" + b + "); " + wr(o[0], w, "(uint32_t)P") +
                        " fop = FOP(FK_MUL, " + n + "); fr = (" + tr + " != P); }");
        }
        const std::string s = rd(o[0], w);
        if (w == 32) {
            if (sgn)
                return line("{ int64_t P = (int64_t)(int32_t)eax * (int64_t)(int32_t)" + s +
                            "; eax = (uint32_t)P; edx = (uint32_t)((uint64_t)P >> 32); fop = FOP(FK_MUL, 4); fr = ((int64_t)(int32_t)eax != P); }");
            return line("{ uint64_t P = (uint64_t)eax * " + s + "; eax = (uint32_t)P; edx = (uint32_t)(P >> 32); fop = FOP(FK_MUL, 4); fr = (edx != 0); }");
        }
        if (w == 16) {
            if (sgn)
                return line("{ int32_t P = (int32_t)(int16_t)eax * (int32_t)(int16_t)" + s + "; " + writeReg(ZYDIS_REGISTER_AX, "P") + " " +
                            writeReg(ZYDIS_REGISTER_DX, "(uint32_t)P >> 16") + " fop = FOP(FK_MUL, 2); fr = ((int32_t)(int16_t)P != P); }");
            return line("{ uint32_t P = (eax & 0xFFFFu) * " + s + "; " + writeReg(ZYDIS_REGISTER_AX, "P") + " " +
                        writeReg(ZYDIS_REGISTER_DX, "P >> 16") + " fop = FOP(FK_MUL, 2); fr = (P > 0xFFFFu); }");
        }
        if (sgn)
            return line("{ int32_t P = (int32_t)(int8_t)eax * (int32_t)(int8_t)" + s + "; " + writeReg(ZYDIS_REGISTER_AX, "P") +
                        " fop = FOP(FK_MUL, 1); fr = ((int32_t)(int8_t)P != P); }");
        return line("{ uint32_t P = (eax & 0xFFu) * " + s + "; " + writeReg(ZYDIS_REGISTER_AX, "P") + " fop = FOP(FK_MUL, 1); fr = (P > 0xFFu); }");
    }

    void strings(const Insn& ins) {
        const auto m = ins.in.mnemonic;
        int w = 32;
        std::string kind;
        switch (m) {
        case ZYDIS_MNEMONIC_MOVSB: w = 8; kind = "movs"; break;
        case ZYDIS_MNEMONIC_MOVSW: w = 16; kind = "movs"; break;
        case ZYDIS_MNEMONIC_MOVSD: kind = "movs"; break;
        case ZYDIS_MNEMONIC_STOSB: w = 8; kind = "stos"; break;
        case ZYDIS_MNEMONIC_STOSW: w = 16; kind = "stos"; break;
        case ZYDIS_MNEMONIC_STOSD: kind = "stos"; break;
        case ZYDIS_MNEMONIC_LODSB: w = 8; kind = "lods"; break;
        case ZYDIS_MNEMONIC_LODSW: w = 16; kind = "lods"; break;
        case ZYDIS_MNEMONIC_LODSD: kind = "lods"; break;
        case ZYDIS_MNEMONIC_CMPSB: w = 8; kind = "cmps"; break;
        case ZYDIS_MNEMONIC_CMPSW: w = 16; kind = "cmps"; break;
        case ZYDIS_MNEMONIC_CMPSD: kind = "cmps"; break;
        case ZYDIS_MNEMONIC_SCASB: w = 8; kind = "scas"; break;
        case ZYDIS_MNEMONIC_SCASW: w = 16; kind = "scas"; break;
        default: kind = "scas"; break;
        }
        const std::string sz = std::to_string(w / 8);
        const std::string step = "(c->df ? -" + sz + " : " + sz + ")";
        const std::string rdf = w == 8 ? "(uint32_t)rd8" : w == 16 ? "(uint32_t)rd16" : "rd32";
        const std::string wrf = w == 8 ? "wr8" : w == 16 ? "wr16" : "wr32";
        const std::string cast = w == 8 ? "(uint8_t)" : w == 16 ? "(uint16_t)" : "(uint32_t)";
        const ZydisRegister acc = w == 8 ? ZYDIS_REGISTER_AL : w == 16 ? ZYDIS_REGISTER_AX : ZYDIS_REGISTER_EAX;
        std::string one;
        if (kind == "movs") one = wrf + "(edi, " + cast + rdf + "(esi)); esi += " + step + "; edi += " + step + ";";
        if (kind == "stos") one = wrf + "(edi, " + cast + readReg(acc) + "); edi += " + step + ";";
        if (kind == "lods") one = writeReg(acc, rdf + "(esi)") + " esi += " + step + ";";
        if (kind == "cmps")
            one = "{ uint32_t A = " + rdf + "(esi), B = " + rdf + "(edi); fop = FOP(FK_SUB, " + sz + "); fa = A; fb = B; fr = (A - B) & " +
                  hexu(mask(w)) + "; } esi += " + step + "; edi += " + step + ";";
        if (kind == "scas")
            one = "{ uint32_t A = " + readReg(acc) + ", B = " + rdf + "(edi); fop = FOP(FK_SUB, " + sz + "); fa = A; fb = B; fr = (A - B) & " +
                  hexu(mask(w)) + "; } edi += " + step + ";";
        const bool rep = ins.in.attributes & (ZYDIS_ATTRIB_HAS_REP | ZYDIS_ATTRIB_HAS_REPE | ZYDIS_ATTRIB_HAS_REPZ);
        const bool repne = ins.in.attributes & (ZYDIS_ATTRIB_HAS_REPNE | ZYDIS_ATTRIB_HAS_REPNZ);
        if (!rep && !repne) return line(one);
        if (kind == "cmps" || kind == "scas") {
            const std::string stop = repne ? "f_zf(fop, fr)" : "!f_zf(fop, fr)";
            return line("while (ecx != 0) { " + one + " --ecx; if (" + stop + ") break; }");
        }
        if (kind == "movs" && w == 32)
            return line("if (!c->df && ecx && (edi + ecx * 4u <= esi || esi + ecx * 4u <= edi)) { memcpy(GP(edi), GP(esi), (size_t)ecx * 4u); esi += ecx * 4u; edi += ecx * 4u; ecx = 0; } while (ecx != 0) { " + one + " --ecx; }");
        if (kind == "stos") {
            // Forward fill (MSVC's inlined memset / zeroing): memset, or a loop over a host
            // pointer that the compiler vectorises; the general loop below steps a 32-bit edi
            // one element at a time.
            const std::string fill = w == 8 ? "memset(GP(edi), (int)(uint8_t)eax, (size_t)ecx);"
                                             : "{ uint8_t* p_ = GP(edi); const uint32_t v_ = " + readReg(acc) + "; for (size_t i_ = 0; i_ < ecx; ++i_) " +
                                                   (w == 16 ? "wr16p(p_ + 2u * i_, (uint16_t)v_); }" : "wr32p(p_ + 4u * i_, v_); }");
            return line("if (!c->df && ecx) { " + fill + " edi += ecx * " + sz + "u; ecx = 0; } while (ecx != 0) { " + one + " --ecx; }");
        }
        return line("while (ecx != 0) { " + one + " --ecx; }");
    }

    // ---- x87 ----

    std::string fmemLoad(const ZydisDecodedOperand& o, bool integer) {
        if (integer) {
            if (o.size == 16) return "(double)(int16_t)" + mem("rd16", o) + ")";
            if (o.size == 64) return "(double)(int64_t)" + mem("rd64", o) + ")";
            return "(double)(int32_t)" + mem("rd32", o) + ")";
        }
        if (o.size == 32) return "(double)" + mem("rdf32", o) + ")";
        if (o.size == 64) return mem("rdf64", o) + ")";
        return "f80_load(" + addr(o) + ")";
    }
    std::string fmemStore(const ZydisDecodedOperand& o, const std::string& v) {
        if (o.size == 32) return mem("wrf32", o) + ", (float)(" + v + "));";
        if (o.size == 64) return mem("wrf64", o) + ", " + v + ");";
        return "f80_store(" + addr(o) + ", " + v + ");";
    }
    static int stIndex(ZydisRegister r) { return r - ZYDIS_REGISTER_ST0; }

    void x87(const Insn& ins) {
        const auto m = ins.in.mnemonic;
        const ZydisDecodedOperand* mem = nullptr;
        std::vector<int> sts;
        for (int i = 0; i < ins.in.operand_count; ++i) {
            const auto& o = ins.op[i];
            if (o.type == ZYDIS_OPERAND_TYPE_MEMORY) mem = &o;
            if (o.type == ZYDIS_OPERAND_TYPE_REGISTER && o.reg.value >= ZYDIS_REGISTER_ST0 && o.reg.value <= ZYDIS_REGISTER_ST7 &&
                o.visibility != ZYDIS_OPERAND_VISIBILITY_HIDDEN)
                sts.push_back(stIndex(o.reg.value));
        }
        const std::string loc = hex(ins.addr);
        auto arith = [&](const char* expr, bool reverse, bool pop, bool integer) {
            // dst = dst OP src  (reverse: dst = src OP dst)
            int di = 1, si = 0;  // implicit forms (e.g. faddp with no operands)
            std::string src;
            if (mem) {
                di = 0;
                src = fmemLoad(*mem, integer);
            } else if (sts.size() >= 2) {
                di = sts[0];
                si = sts[1];
            }
            if (!mem) src = stR(si);
            const std::string dst = stR(di), dstW = stW(di);
            const std::string a = reverse ? "s" : "d", b = reverse ? "d" : "s";
            std::string s = "{ double s = " + src + ", d = " + dst + "; " + dstW + " = " + prec(a + " " + expr + " " + b) + "; }";
            if (pop) s += fpopS();
            line(s);
        };
        // A conditional push (fsincos/fptan out of range) or a TOP reset (fninit) has no static
        // depth: the local model writes its state back, runs the memory-model form and retakes
        // the base at the nominal depth (xbase = TOP + d: right whichever way it went).
        auto viaMemory = [&](const std::string& code, int pushes) {
            if (!xl_) return line(code);
            xsync_ = true;
            const std::string w = dry_ ? std::string() : xspill(xs_);
            xd_ = dry_ ? xd_ + pushes : xoutD_;  // the depth chosen for the retaken base
            line("{ " + w + code + " " + xreload(xd_) + " }");
        };
        switch (m) {
        case ZYDIS_MNEMONIC_FLD:
            if (mem) return line(fpushS(fmemLoad(*mem, false)));
            {
                const std::string v = stR(sts.empty() ? 0 : sts[0]);
                return line("{ double v = " + v + "; " + fpushS("v") + " }");
            }
        case ZYDIS_MNEMONIC_FILD: return line(fpushS(fmemLoad(*mem, true)));
        case ZYDIS_MNEMONIC_FLD1: return line(fpushS("1.0"));
        case ZYDIS_MNEMONIC_FLDZ: return line(fpushS("0.0"));
        case ZYDIS_MNEMONIC_FLDPI: return line(fpushS("3.14159265358979323846"));
        case ZYDIS_MNEMONIC_FLDL2E: return line(fpushS("1.44269504088896340736"));
        case ZYDIS_MNEMONIC_FLDLN2: return line(fpushS("0.693147180559945309417"));
        case ZYDIS_MNEMONIC_FLDLG2: return line(fpushS("0.301029995663981195214"));
        case ZYDIS_MNEMONIC_FLDL2T: return line(fpushS("3.32192809488736234787"));
        case ZYDIS_MNEMONIC_FST: case ZYDIS_MNEMONIC_FSTP: {
            const bool pop = m == ZYDIS_MNEMONIC_FSTP;
            if (mem) {
                std::string s = fmemStore(*mem, stR(0));
                return line(s + (pop ? fpopS() : ""));
            }
            const std::string v = stR(0);
            std::string s = stW(sts.empty() ? 1 : sts[0]) + " = " + v + ";";
            return line(s + (pop ? fpopS() : ""));
        }
        case ZYDIS_MNEMONIC_FIST: case ZYDIS_MNEMONIC_FISTP: {
            const bool pop = m == ZYDIS_MNEMONIC_FISTP;
            const std::string v = stR(0);
            std::string s = mem->size == 16 ? this->mem("wr16", *mem) + ", (uint16_t)" + cwFn("fist16") + "(" + cwArg() + ", " + v + "));"
                            : mem->size == 64 ? this->mem("wr64", *mem) + ", (uint64_t)" + cwFn("fist64") + "(" + cwArg() + ", " + v + "));"
                                              : this->mem("wr32", *mem) + ", (uint32_t)" + cwFn("fist32") + "(" + cwArg() + ", " + v + "));";
            return line(s + (pop ? fpopS() : ""));
        }
        case ZYDIS_MNEMONIC_FISTTP: {
            std::string s = "{ double t = trunc(" + stR(0) + "); " +
                            std::string(mem->size == 16 ? this->mem("wr16", *mem) + ", (uint16_t)(int16_t)t);" : mem->size == 64 ? this->mem("wr64", *mem) + ", (uint64_t)(int64_t)t);" : this->mem("wr32", *mem) + ", (uint32_t)(int32_t)t);");
            return line(s + fpopS() + " }");
        }
        case ZYDIS_MNEMONIC_FADD: return arith("+", false, false, false);
        case ZYDIS_MNEMONIC_FADDP: return arith("+", false, true, false);
        case ZYDIS_MNEMONIC_FIADD: return arith("+", false, false, true);
        case ZYDIS_MNEMONIC_FSUB: return arith("-", false, false, false);
        case ZYDIS_MNEMONIC_FSUBP: return arith("-", false, true, false);
        case ZYDIS_MNEMONIC_FISUB: return arith("-", false, false, true);
        case ZYDIS_MNEMONIC_FSUBR: return arith("-", true, false, false);
        case ZYDIS_MNEMONIC_FSUBRP: return arith("-", true, true, false);
        case ZYDIS_MNEMONIC_FISUBR: return arith("-", true, false, true);
        case ZYDIS_MNEMONIC_FMUL: return arith("*", false, false, false);
        case ZYDIS_MNEMONIC_FMULP: return arith("*", false, true, false);
        case ZYDIS_MNEMONIC_FIMUL: return arith("*", false, false, true);
        case ZYDIS_MNEMONIC_FDIV: return arith("/", false, false, false);
        case ZYDIS_MNEMONIC_FDIVP: return arith("/", false, true, false);
        case ZYDIS_MNEMONIC_FIDIV: return arith("/", false, false, true);
        case ZYDIS_MNEMONIC_FDIVR: return arith("/", true, false, false);
        case ZYDIS_MNEMONIC_FDIVRP: return arith("/", true, true, false);
        case ZYDIS_MNEMONIC_FIDIVR: return arith("/", true, false, true);
        case ZYDIS_MNEMONIC_FCOM: case ZYDIS_MNEMONIC_FCOMP: case ZYDIS_MNEMONIC_FUCOM: case ZYDIS_MNEMONIC_FUCOMP:
        case ZYDIS_MNEMONIC_FICOM: case ZYDIS_MNEMONIC_FICOMP: {
            const bool integer = m == ZYDIS_MNEMONIC_FICOM || m == ZYDIS_MNEMONIC_FICOMP;
            const bool pop = m == ZYDIS_MNEMONIC_FCOMP || m == ZYDIS_MNEMONIC_FUCOMP || m == ZYDIS_MNEMONIC_FICOMP;
            const std::string a = stR(0);
            const std::string src = mem ? fmemLoad(*mem, integer) : stR(sts.empty() ? 1 : sts[0]);
            return line(fcmpS(a, src) + (pop ? fpopS() : ""));
        }
        case ZYDIS_MNEMONIC_FCOMPP: case ZYDIS_MNEMONIC_FUCOMPP: {
            const std::string a = stR(0), b = stR(1);
            std::string s = fcmpS(a, b) + fpopS();
            return line(s + fpopS());
        }
        case ZYDIS_MNEMONIC_FCOMI: case ZYDIS_MNEMONIC_FCOMIP: case ZYDIS_MNEMONIC_FUCOMI: case ZYDIS_MNEMONIC_FUCOMIP: {
            const bool pop = m == ZYDIS_MNEMONIC_FCOMIP || m == ZYDIS_MNEMONIC_FUCOMIP;
            const std::string a = stR(0), b = stR(sts.size() >= 2 ? sts[1] : 1);
            return line("fr = fcmpi(" + a + ", " + b + "); fop = FOP(FK_EXPLICIT, 4);" + (pop ? fpopS() : ""));
        }
        case ZYDIS_MNEMONIC_FTST: return line(fcmpS(stR(0), "0.0"));
        case ZYDIS_MNEMONIC_FXAM: {
            const std::string v = stR(0), swr = swRef(false), sww = swRef(true);
            return line("{ double v = " + v + "; uint16_t sw = (uint16_t)(" + swr + " & ~(X87_C0|X87_C1|X87_C2|X87_C3)); if (signbit(v)) sw |= X87_C1; if (isnan(v)) sw |= X87_C0; else if (isinf(v)) sw |= X87_C0|X87_C2; else if (v == 0.0) sw |= X87_C3; else sw |= X87_C2; " + sww + " = sw; }");
        }
        case ZYDIS_MNEMONIC_FXCH: {
            const int k = sts.empty() ? 1 : sts[0];
            const std::string r = stR(k), rw = stW(k), t0 = stR(0), t0w = stW(0);
            return line("{ double t = " + t0 + "; " + t0w + " = " + r + "; " + rw + " = t; }");
        }
        case ZYDIS_MNEMONIC_FCHS: { const std::string v = stR(0); return line(stW(0) + " = -" + v + ";"); }
        case ZYDIS_MNEMONIC_FABS: { const std::string v = stR(0); return line(stW(0) + " = fabs(" + v + ");"); }
        case ZYDIS_MNEMONIC_FSQRT: { const std::string v = stR(0); return line(stW(0) + " = " + prec("sqrt(" + v + ")") + ";"); }
        case ZYDIS_MNEMONIC_FSIN: case ZYDIS_MNEMONIC_FCOS: {
            const std::string v = stR(0), w = stW(0), fn = m == ZYDIS_MNEMONIC_FSIN ? "sin" : "cos";
            return line("if (" + trigS(v) + ") " + w + " = " + prec(fn + "(" + v + ")") + ";");
        }
        case ZYDIS_MNEMONIC_FSINCOS:
            return viaMemory("{ double v = ST(0); if (x87_trig_in_range(c, v)) { ST(0) = fprec(c, sin(v)); fpush(c, fprec(c, cos(v))); } }", 1);
        case ZYDIS_MNEMONIC_FPTAN:
            return viaMemory("if (x87_trig_in_range(c, ST(0))) { ST(0) = fprec(c, tan(ST(0))); fpush(c, 1.0); }", 1);
        case ZYDIS_MNEMONIC_FPATAN: case ZYDIS_MNEMONIC_FYL2X: case ZYDIS_MNEMONIC_FYL2XP1: {
            const std::string x = stR(0), y = stR(1);
            const std::string e = m == ZYDIS_MNEMONIC_FPATAN ? "atan2(y, x)" : m == ZYDIS_MNEMONIC_FYL2X ? "y * log2(x)" : "y * log2(x + 1.0)";
            std::string s = "{ double x = " + x + ", y = " + y + ";" + fpopS();
            return line(s + " " + stW(0) + " = " + prec(e) + "; }");
        }
        case ZYDIS_MNEMONIC_F2XM1: { const std::string v = stR(0); return line(stW(0) + " = " + prec("exp2(" + v + ") - 1.0") + ";"); }
        case ZYDIS_MNEMONIC_FSCALE: {
            const std::string v = stR(0), k = stR(1);
            return line(stW(0) + " = ldexp(" + v + ", (int)trunc(" + k + "));");
        }
        case ZYDIS_MNEMONIC_FRNDINT: { const std::string v = stR(0); return line(stW(0) + " = " + cwFn(xl_ ? "fround" : "fround_rc") + "(" + cwArg() + ", " + v + ");"); }
        case ZYDIS_MNEMONIC_FPREM: case ZYDIS_MNEMONIC_FPREM1: {
            const std::string v = stR(0), k = stR(1), w = stW(0);
            return line(w + " = " + std::string(m == ZYDIS_MNEMONIC_FPREM ? "fmod" : "remainder") + "(" + v + ", " + k + "); " + swRef(true) + " &= (uint16_t)~X87_C2;");
        }
        case ZYDIS_MNEMONIC_FNSTSW: {
            // The local model's TOP is xbase - d.
            const std::string v = xl_ ? "fnstsw_top(xsw, xbase + " + std::to_string((-xd_) & 7) + "u)" : "fnstsw(c)";
            if (mem) return line(this->mem("wr16", *mem) + ", " + v + ");");
            return line(writeReg(ZYDIS_REGISTER_AX, v));
        }
        case ZYDIS_MNEMONIC_FNSTCW: return line(this->mem("wr16", *mem) + ", " + (xl_ ? "xcw" : "c->fpu.cw") + ");");
        case ZYDIS_MNEMONIC_FLDCW:
            if (xl_) return line("xcw = " + this->mem("rd16", *mem) + "); c->fpu.cw = xcw;");
            return line("c->fpu.cw = " + this->mem("rd16", *mem) + ");");
        case ZYDIS_MNEMONIC_FNINIT: return viaMemory("c->fpu.top = 0; c->fpu.sw = 0; c->fpu.cw = 0x037F;", 0);
        case ZYDIS_MNEMONIC_FNCLEX: return line(swRef(true) + " &= 0x7F00u;");
        case ZYDIS_MNEMONIC_FFREE: case ZYDIS_MNEMONIC_FNOP: case ZYDIS_MNEMONIC_FWAIT: return;
        case ZYDIS_MNEMONIC_FINCSTP:
            if (xl_) { --xd_; xmoved_ = true; return; }
            return line("c->fpu.top = (c->fpu.top + 1u) & 7u;");
        case ZYDIS_MNEMONIC_FDECSTP:
            if (xl_) { ++xd_; xmoved_ = true; return; }
            return line("c->fpu.top = (c->fpu.top - 1u) & 7u;");
        case ZYDIS_MNEMONIC_FCMOVB: case ZYDIS_MNEMONIC_FCMOVE: case ZYDIS_MNEMONIC_FCMOVBE: case ZYDIS_MNEMONIC_FCMOVU:
        case ZYDIS_MNEMONIC_FCMOVNB: case ZYDIS_MNEMONIC_FCMOVNE: case ZYDIS_MNEMONIC_FCMOVNBE: case ZYDIS_MNEMONIC_FCMOVNU: {
            int ccode = 0x2;
            if (m == ZYDIS_MNEMONIC_FCMOVE) ccode = 0x4;
            if (m == ZYDIS_MNEMONIC_FCMOVBE) ccode = 0x6;
            if (m == ZYDIS_MNEMONIC_FCMOVU) ccode = 0xA;
            if (m == ZYDIS_MNEMONIC_FCMOVNB) ccode = 0x3;
            if (m == ZYDIS_MNEMONIC_FCMOVNE) ccode = 0x5;
            if (m == ZYDIS_MNEMONIC_FCMOVNBE) ccode = 0x7;
            if (m == ZYDIS_MNEMONIC_FCMOVNU) ccode = 0xB;
            const std::string v = stR(sts.size() >= 2 ? sts[1] : 1);
            return line("if (f_cond(" + std::to_string(ccode) + ", fop, fr, fa, fb)) " + stW(0) + " = " + v + ";");
        }
        default:
            return unsupported(ins);
        }
    }
    std::string fcmpS(const std::string& a, const std::string& b) {
        if (!xl_) return "fcmp(c, " + a + ", " + b + ");";
        xswWritten_ = true;
        return "xsw = fcmp_sw(xsw, " + a + ", " + b + ");";
    }
    std::string trigS(const std::string& v) {
        if (!xl_) return "x87_trig_in_range(c, " + v + ")";
        xswWritten_ = true;
        return "x87_trig_in_range_sw(&xsw, " + v + ")";
    }

    // ---- SIMD (MMX / SSE / SSE2) — implemented in Simd.inc ----
#include "Simd.inc"

    Program& p_;
    Stats& st_;
    Function* f_ = nullptr;
    std::ostringstream out_;
    bool terminated_ = false;
    // dry runs (x87 analysis)
    bool dry_ = false;
    std::string dryText_;
    std::vector<uint32_t> drySuccs_;
    // x87 local model
    bool xl_ = false;                    // current instruction uses it
    bool xlFn_ = false;                  // current function uses it
    std::map<uint32_t, XState> xin_;     // state on entry to each instruction / stub label
    std::map<uint32_t, int> xout_;       // depth after each sync instruction (the retaken base)
    int xoutD_ = 0;                      // xout_ of the instruction being emitted
    XState xs_;                          // state at the instruction being emitted
    int xd_ = 0;                         // depth while emitting it
    uint8_t xused_ = 0;                  // registers the function names
    uint8_t xread_ = 0, xwritten_ = 0;   // effects recorded by a dry run
    bool xmoved_ = false, xswWritten_ = false, xsync_ = false;
};

} // namespace
} // namespace recomp

int main(int argc, char** argv) {
    using namespace recomp;
    if (argc < 4) {
        std::cerr << "usage: fable_recomp <Fable.exe> <functions.tsv> <out-dir> [--only a,b,..] [--per-file N] [--prefix NAME] [--no-refs] [--no-fold] [--no-x87-locals] [--no-ftol2-inline] [--hook ADDR=host_fn]... [--trace ADDR]...\n";
        return 2;
    }
    const Image img(argv[1]);
    const std::filesystem::path outDir = argv[3];
    std::set<uint32_t> only;
    size_t perFile = 400;
    bool noRefs = false;
    bool noFtol2Inline = false;
    std::string prefix = "recomp";
    std::map<uint32_t, std::string> hooks;  // guest functions replaced by host code
    std::map<uint32_t, std::string> wraps;  // guest functions routed through host code, original kept
    for (int i = 4; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "--only" && i + 1 < argc) {
            std::stringstream ss(argv[++i]);
            std::string t;
            while (std::getline(ss, t, ',')) only.insert(static_cast<uint32_t>(std::stoul(t, nullptr, 16)));
        } else if (a == "--trace" && i + 1 < argc) {  // --trace 0x434F60: call recomp_trace at entry
            traceAddrs().insert(static_cast<uint32_t>(std::stoul(argv[++i], nullptr, 16)));
        } else if (a == "--hook" && i + 1 < argc) {  // --hook 0x9D8650=host_coswitch
            const std::string h = argv[++i];
            const auto eq = h.find('=');
            if (eq != std::string::npos) hooks[static_cast<uint32_t>(std::stoul(h.substr(0, eq), nullptr, 16))] = h.substr(eq + 1);
        } else if (a == "--wrap" && i + 1 < argc) {  // --wrap 0x55CB10=host_fe_event: original stays as F_0055CB10_orig
            const std::string h = argv[++i];
            const auto eq = h.find('=');
            if (eq != std::string::npos) wraps[static_cast<uint32_t>(std::stoul(h.substr(0, eq), nullptr, 16))] = h.substr(eq + 1);
        } else if (a == "--prefix" && i + 1 < argc) {
            prefix = argv[++i];
        } else if (a == "--no-refs") {
            noRefs = true;
        } else if (a == "--no-fold") {
            foldDisplacements() = false;
        } else if (a == "--no-x87-locals") {
            x87Locals() = false;
        } else if (a == "--no-ftol2-inline") {
            noFtol2Inline = true;
        } else if (a == "--per-file" && i + 1 < argc) {
            perFile = std::stoul(argv[++i]);
        }
    }

    if (!noFtol2Inline)
        for (const auto& [a, name] : hooks)
            if (name == "host_ftol2") ftol2Inline().insert(a);
    Program prog(img);
    if (std::string(argv[2]) != "-") {
        std::ifstream tsv(argv[2]);
        std::string row;
        std::getline(tsv, row);
        while (std::getline(tsv, row)) {
            const auto tab = row.find('\t');
            prog.addEntry(static_cast<uint32_t>(std::stoul(row.substr(0, tab), nullptr, 16)));
        }
    }
    prog.addEntry(img.entry());
    for (uint32_t e : img.exports()) prog.addEntry(e);
    prog.discoverAll();
    std::cerr << "exports: " << img.exports().size() << ", relocations into the image: " << img.relocTargets().size() << "\n";
    if (!noRefs) {
        for (int round = 0; round < 8; ++round) {
            const size_t n = prog.addReferencedEntries();
            std::cerr << "pointer-referenced entries added: " << n << "\n";
            if (!n) break;
            prog.discoverAll();
        }
    }
    auto& funcs = prog.functions();

    // Restrict to the closure of --only (direct calls and tail calls).
    std::set<uint32_t> selected;
    Stats stats;
    std::map<uint32_t, std::string> code;
    std::map<uint32_t, uint32_t> retOf;
    {
        Emitter em(prog, stats);
        std::deque<uint32_t> todo(only.begin(), only.end());
        if (only.empty())
            for (auto& [a, f] : funcs) todo.push_back(a);
        while (!todo.empty()) {
            const uint32_t a = todo.front();
            todo.pop_front();
            if (!funcs.count(a) || !selected.insert(a).second) continue;
            em.called.clear();
            em.retBytes = 0;
            code[a] = em.function(funcs[a]);
            retOf[a] = em.retBytes;
            for (uint32_t t : em.called)
                if (!selected.count(t)) todo.push_back(t);
        }
        std::cerr << "x87 in locals: " << em.x87Local << " functions, memory model (depth differs between paths): " << em.x87Fallback << "\n";
    }

    // Hooked functions: the body becomes a call to the host implementation, which follows
    // the lifted-function contract ([esp] = return address on entry; pops it on return).
    for (const auto& [a, name] : hooks)
        if (code.count(a)) {
            code[a] = "void " + fname(a) + "(Ctx* restrict c) { " + name + "(c); }\n\n";
            std::cerr << "hooked " << fname(a) << " -> " << name << "\n";
        }
    // Wrapped functions: the lifted body is renamed <name>_orig (callable from the host) and
    // the entry calls the host implementation.
    for (const auto& [a, name] : wraps)
        if (code.count(a)) {
            std::string body = code[a];
            const std::string sig = "void " + fname(a) + "(Ctx* restrict c) {";
            if (const auto at = body.find(sig); at != std::string::npos)
                body.replace(at, sig.size(), "void " + fname(a) + "_orig(Ctx* restrict c) {");
            code[a] = body + "void " + fname(a) + "(Ctx* restrict c) { " + name + "(c); }\n\n";
            std::cerr << "wrapped " << fname(a) << " -> " << name << "\n";
        }

    std::filesystem::create_directories(outDir);
    const std::string tableSym = prefix == "recomp" ? "recomp_table" : "recomp_table_" + prefix;
    {
        std::ofstream h(outDir / (prefix + "_funcs.h"));
        h << "/* Generated by fable_recomp. */\n#pragma once\n#include \"recomp.h\"\n"
          << "#define SPILL (c->eax = eax, c->ecx = ecx, c->edx = edx, c->ebx = ebx, c->esp = esp, c->ebp = ebp, c->esi = esi, c->edi = edi)\n"
          << "#define RELOAD (eax = c->eax, ecx = c->ecx, edx = c->edx, ebx = c->ebx, esp = c->esp, ebp = c->ebp, esi = c->esi, edi = c->edi)\n";
        for (uint32_t a : selected) h << "void " << fname(a) << "(Ctx* c);\n";
        for (const auto& hk : hooks) h << "void " << hk.second << "(Ctx* c);\n";
        for (const auto& w : wraps) h << "void " << w.second << "(Ctx* c);\nvoid " << fname(w.first) << "_orig(Ctx* c);\n";
        h << "typedef struct RecompEntry { uint32_t addr; GuestFn fn; } RecompEntry;\n"
          << "extern const RecompEntry " << tableSym << "[];\nextern const uint32_t " << tableSym << "_size;\n";
    }
    size_t fileIndex = 0, inFile = 0;
    std::ofstream cf;
    for (auto& [a, src] : code) {
        if (!cf.is_open() || inFile >= perFile) {
            if (cf.is_open()) cf.close();
            char name[32];
            std::snprintf(name, sizeof name, "_%04zu.c", fileIndex++);
            cf.open(outDir / (prefix + name));
            cf << "/* Generated by fable_recomp. */\n#include \"" << prefix << "_funcs.h\"\n\n";
            inFile = 0;
        }
        cf << src;
        ++inFile;
    }
    cf.close();
    {
        std::ofstream t(outDir / (prefix + "_table.c"));
        t << "#include \"" << prefix << "_funcs.h\"\nconst RecompEntry " << tableSym << "[] = {\n";
        for (uint32_t a : selected) t << "    {" << hex(a) << "u, " << fname(a) << "},\n";
        t << "};\nconst uint32_t " << tableSym << "_size = " << selected.size() << ";\n";
    }

    {
        std::ofstream l(outDir / (prefix + "_functions.txt"));
        for (auto& [a, r] : retOf) l << std::hex << a << ' ' << std::dec << r << '\n';
    }
    uint64_t decodeErr = 0;
    for (auto& [a, f] : funcs) decodeErr += f.decodeError;
    std::cout << "functions discovered: " << funcs.size() << ", emitted: " << selected.size() << " in " << fileIndex << " files\n"
              << "instructions lifted: " << stats.insns << ", unsupported: " << stats.unsupportedInsns
              << ", undecodable: " << stats.decodeErrors << ", functions with decode errors: " << decodeErr << "\n"
              << "unresolved indirect jumps: " << stats.unresolvedIndirectJumps
              << ", jump tables sized by fallback scan: " << prog.fallbackTables_ << "\n";
    std::vector<std::pair<uint64_t, std::string>> top;
    for (auto& [m, n] : stats.unsupported) top.emplace_back(n, m);
    std::sort(top.rbegin(), top.rend());
    std::cout << "unsupported mnemonics:";
    for (size_t i = 0; i < top.size() && i < 40; ++i) std::cout << ' ' << top[i].second << ':' << top[i].first;
    std::cout << '\n';
    return 0;
}
