// MSVC C++ exceptions for the recompiled game.
//
// _CxxThrowException walks the guest SEH chain (fs:[0]) the way the VC7.1 runtime
// does: for each frame whose handler is a `mov eax, FuncInfo; jmp __CxxFrameHandler`
// stub it looks for a try block covering the frame's current state with a catch that
// matches the thrown type. It then
//   1. initialises the catch object in the catching frame,
//   2. unwinds the frames in between (their unwind-map actions = destructors) and the
//      catching frame down to the try block,
//   3. runs the catch funclet with ebp = the frame's ebp (it returns the continuation),
//   4. destroys the exception object, and
//   5. resumes the catching function at the continuation with esp restored from the
//      frame ([ebp-10h]), through the landing pad the lifter put in every function
//      that registers an EH frame (recomp_resume_at longjmps to it).
// SEH __try frames (_except_handler3) in between are passed over.
#include "host.hpp"

#include <cstring>

using namespace host;

namespace {

struct Current {
    uint32_t obj = 0, info = 0;
};
thread_local Current t_current;  // for `throw;`

const char* typeName(uint32_t td) { return td ? gp<char>(td + 8) : ""; }

// FuncInfo of a C++ EH registration node's handler, or 0.
uint32_t funcInfoOf(uint32_t rn) {
    const uint32_t h = rd32(rn + 4);
    if (h < 0x401000 || h >= 0x1200000 || rd8(h) != 0xB8 || rd8(h + 5) != 0xE9) return 0;
    const uint32_t fi = rd32(h + 1);
    return (rd32(fi) & ~0xFu) == 0x19930520u ? fi : 0;
}

// Calls a funclet (unwind action or catch block) with ebp = the frame's ebp, on the
// current guest stack. Returns eax.
uint32_t callFunclet(uint32_t fn, uint32_t frameEbp) {
    Ctx* c = cur();
    const uint32_t savedEbp = c->ebp;
    c->ebp = frameEbp;
    const uint32_t r = guestCall(fn, {});  // guestCall keeps the caller's registers but uses ours for the call
    c->ebp = savedEbp;
    return r;
}

void unwindTo(uint32_t rn, uint32_t fi, int32_t target) {
    const uint32_t unwind = rd32(fi + 8), maxState = rd32(fi + 4);
    int32_t state = static_cast<int32_t>(rd32(rn + 8));
    while (state != target && state >= 0 && static_cast<uint32_t>(state) < maxState) {
        const int32_t next = static_cast<int32_t>(rd32(unwind + 8 * state));
        const uint32_t action = rd32(unwind + 8 * state + 4);
        wr32(rn + 8, static_cast<uint32_t>(next));
        if (action) callFunclet(action, rn + 12);
        state = next;
    }
    wr32(rn + 8, static_cast<uint32_t>(target));
}

// Does catch handler `ht` accept the exception described by ThrowInfo `info`?
// Returns the matching CatchableType (or 1 for catch(...)), 0 if none.
uint32_t matchHandler(uint32_t ht, uint32_t info) {
    const uint32_t td = rd32(ht + 4);
    if (!td || !*typeName(td)) return 1;  // catch (...)
    const uint32_t cta = rd32(info + 12);
    for (uint32_t i = 0, n = rd32(cta); i < n; ++i) {
        const uint32_t ct = rd32(cta + 4 + 4 * i), ctd = rd32(ct + 4);
        if (ctd == td || std::strcmp(typeName(ctd), typeName(td)) == 0) return ct;
    }
    return 0;
}

void initCatchObject(uint32_t ht, uint32_t ct, uint32_t obj, uint32_t frameEbp) {
    const uint32_t disp = rd32(ht + 8);
    if (ct <= 1 || !disp || !rd32(ht + 4)) return;
    const uint32_t dst = frameEbp + disp, src = obj + rd32(ct + 8);  // + mdisp
    const uint32_t props = rd32(ct), size = rd32(ct + 20), copy = rd32(ct + 24);
    if (rd32(ht) & 8) wr32(dst, src);                    // catch (T&)
    else if (props & 1) std::memcpy(gp(dst), gp(obj), size);  // simple type (incl. pointers)
    else if (copy) guestCallThis(copy, dst, {src});      // copy constructor
    else std::memcpy(gp(dst), gp(src), size);
}

[[noreturn]] void unhandled(uint32_t info, uint32_t from) {
    const char* type = "?";
    if (info) {
        const uint32_t cta = rd32(info + 12);
        if (cta && rd32(cta)) type = typeName(rd32(rd32(cta + 4) + 4));
    }
    die("unhandled C++ exception (type %s) thrown from 0x%08X", type, from);
}

}  // namespace

// _CxxThrowException(pExceptionObject, pThrowInfo)
IMPORT("msvcr71.dll", _CxxThrowException) {
    uint32_t obj = arg(c, 0), info = arg(c, 1);
    const uint32_t from = rd32(c->esp);
    if (!info) {  // throw;  (rethrow the exception being handled)
        obj = t_current.obj, info = t_current.info;
        if (!info) die("rethrow with no current exception (from 0x%08X)", from);
    }
    const uint32_t teb = c->fs_base;
    HLOG(1, "C++ exception %s thrown from 0x%08X", typeName(rd32(rd32(rd32(info + 12) + 4) + 4)), from);

    for (uint32_t rn = rd32(teb); rn && rn != 0xFFFFFFFFu; rn = rd32(rn)) {
        const uint32_t fi = funcInfoOf(rn);
        if (!fi) continue;
        const int32_t state = static_cast<int32_t>(rd32(rn + 8));
        const uint32_t nTry = rd32(fi + 12), tries = rd32(fi + 16);
        for (uint32_t t = 0; t < nTry; ++t) {
            const uint32_t tb = tries + 20 * t;
            const int32_t low = static_cast<int32_t>(rd32(tb)), high = static_cast<int32_t>(rd32(tb + 4));
            if (state < low || state > high) continue;
            const uint32_t n = rd32(tb + 12), handlers = rd32(tb + 16);
            for (uint32_t h = 0; h < n; ++h) {
                const uint32_t ht = handlers + 16 * h;
                const uint32_t ct = matchHandler(ht, info);
                if (!ct) continue;
                const uint32_t frameEbp = rn + 12;
                initCatchObject(ht, ct, obj, frameEbp);
                // unwind the frames in between, then this one down to the try block
                for (uint32_t r = rd32(teb); r != rn; r = rd32(r))
                    if (const uint32_t f = funcInfoOf(r)) unwindTo(r, f, -1);
                wr32(teb, rn);
                unwindTo(rn, fi, low);
                wr32(rn + 8, static_cast<uint32_t>(high + 1));
                // the catch block
                const Current outer = t_current;
                t_current = {obj, info};
                const uint32_t cont = callFunclet(rd32(ht + 12), frameEbp);
                t_current = outer;
                if (const uint32_t dtor = rd32(info + 4)) guestCallThis(dtor, obj, {});
                HLOG(1, "  caught in frame 0x%08X, continuing at 0x%08X", rn, cont);
                c->ebp = frameEbp;
                c->esp = rd32(rn - 4);  // [ebp-10h]: esp saved by the function's prologue
                recomp_resume_at(c, rn, cont);
                die("no landing pad for the C++ catch continuation 0x%08X (frame 0x%08X)", cont, rn);
            }
        }
    }
    unhandled(info, from);
}

IMPORT("msvcr71.dll", __CxxFrameHandler) {
    // Only reached if guest code calls the handler directly (the throw path above walks
    // frames itself): report that nothing was handled.
    retCdecl(c, 1);  // ExceptionContinueSearch
}
