// Guest-visible proxies for host COM objects (Direct3D 9, DirectInput 8).
//
// A proxy is 16 bytes of guest memory: { guest vtable, 0, host pointer (64-bit) }.
// Guest code calls methods through the vtable as usual; every slot is a trap
// that unwraps `this`, converts the arguments and calls the real 64-bit object.
#pragma once
#include "host.hpp"

#include <unknwn.h>

#include <initializer_list>
#include <type_traits>
#include <vector>

namespace host::com {

struct Class {
    const char* name = nullptr;
    uint32_t vtbl = 0;  // guest address of the vtable
};
struct MethodDef {
    const char* name;
    Handler fn;  // nullptr: not convertible automatically -> traps with a message
};
Class makeClass(const char* name, std::initializer_list<MethodDef> methods);
template <class I> Class& classOf();
struct IidEntry {
    const IID* iid;
    Class* (*cls)();
};
Class* classForIid(REFIID iid);

uint32_t wrapRaw(IUnknown* p, Class& cls);
IUnknown* unwrapRaw(uint32_t g);
template <class I> uint32_t wrap(I* p) { return p ? wrapRaw(reinterpret_cast<IUnknown*>(p), classOf<I>()) : 0; }
template <class I> I* unwrap(uint32_t g) { return reinterpret_cast<I*>(unwrapRaw(g)); }
// Per-object cleanup hook (staging buffers etc.), run when the host object is released.
void onRelease(IUnknown* p, void (*fn)(IUnknown*));

void comQueryInterface(Ctx* c);
void comAddRef(Ctx* c);
void comRelease(Ctx* c);

template <auto M, class R, class I, class... A> void comThunkImpl(Ctx* c, R (I::*)(A...)) {
    I* self = unwrap<I>(arg(c, 0));
    if (!self) die("COM call on a null or unknown object 0x%08X", arg(c, 0));
    const uint32_t used = detail::invoke<R, A...>(c, 4, [self](auto&&... a) -> R { return (self->*M)(std::forward<decltype(a)>(a)...); });
    c->esp += 4 + 4 + used;
}
template <auto M> void comThunk(Ctx* c) { comThunkImpl<M>(c, M); }

}  // namespace host::com

namespace host {
// Interface pointer arguments: unwrap the guest proxy.
template <class I> struct Arg<I*, std::enable_if_t<std::is_base_of_v<IUnknown, I>>> {
    I* v = nullptr;
    void in(Ctx* c, uint32_t& off) { v = com::unwrap<I>(rd32(c->esp + 4 + off)); off += 4; }
    I* host() { return v; }
    void post() {}
};
// Interface out-parameters: wrap whatever the host returns.
template <class I> struct Arg<I**, std::enable_if_t<std::is_base_of_v<IUnknown, I>>> {
    uint32_t g = 0;
    I* v = nullptr;
    void in(Ctx* c, uint32_t& off) { g = rd32(c->esp + 4 + off); off += 4; }
    I** host() { return g ? &v : nullptr; }
    void post() { if (g) wr32(g, com::wrap<I>(v)); }
};
}  // namespace host
