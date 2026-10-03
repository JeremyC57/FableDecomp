// COM proxy machinery and the generated vtables.
#define DIRECTINPUT_VERSION 0x0800
#include "com.hpp"

#include <d3d9.h>
#include <dinput.h>
#include <ddraw.h>
#include <mmreg.h>
#include <dsound.h>

#include <mutex>
#include <unordered_map>

namespace host::com {

namespace {
struct Proxy {
    uint32_t guest;
    Class* cls;
    std::vector<void (*)(IUnknown*)> cleanup;
};
std::unordered_map<IUnknown*, Proxy> g_proxies;
std::recursive_mutex g_lock;

void unsupportedMethod(Ctx* c, uintptr_t name) {
    die("COM method %s is not supported yet (called from 0x%08X)", reinterpret_cast<const char*>(name), rd32(c->esp));
}
}  // namespace

Class makeClass(const char* name, std::initializer_list<MethodDef> methods) {
    Class cls;
    cls.name = name;
    cls.vtbl = gcalloc(static_cast<uint32_t>(methods.size()) + 1, 4);
    uint32_t i = 0;
    for (const MethodDef& m : methods) {
        const uint32_t t = m.fn ? addTrap(m.name, m.fn) : addTrap(m.name, unsupportedMethod, reinterpret_cast<uintptr_t>(m.name));
        wr32(cls.vtbl + 4 * i++, t);
    }
    return cls;
}

uint32_t wrapRaw(IUnknown* p, Class& cls) {
    if (!p) return 0;
    std::lock_guard<std::recursive_mutex> l(g_lock);
    auto it = g_proxies.find(p);
    if (it != g_proxies.end()) {
        if (it->second.cls != &cls) {
            // Same host object seen through a different interface (or a recycled address):
            // the derived interface's vtable is a superset, so prefer the longer one.
            it->second.cls = &cls;
            wr32(it->second.guest, cls.vtbl);
        }
        return it->second.guest;
    }
    const uint32_t g = gcalloc(1, 16);
    wr32(g, cls.vtbl);
    wr64(g + 8, reinterpret_cast<uint64_t>(p));
    g_proxies[p] = Proxy{g, &cls, {}};
    HLOG(2, "wrap %s %p -> 0x%08X", cls.name, static_cast<void*>(p), g);
    return g;
}

IUnknown* unwrapRaw(uint32_t g) {
    if (!g || g >= kGuestLimit) return nullptr;
    return reinterpret_cast<IUnknown*>(rd64(g + 8));
}

void onRelease(IUnknown* p, void (*fn)(IUnknown*)) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    auto it = g_proxies.find(p);
    if (it == g_proxies.end()) return;
    for (auto f : it->second.cleanup)
        if (f == fn) return;
    it->second.cleanup.push_back(fn);
}

void comAddRef(Ctx* c) {
    IUnknown* p = unwrapRaw(arg(c, 0));
    retStd(c, p ? p->AddRef() : 0, 1);
}

void comRelease(Ctx* c) {
    IUnknown* p = unwrapRaw(arg(c, 0));
    if (!p) { retStd(c, 0, 1); return; }
    std::lock_guard<std::recursive_mutex> l(g_lock);
    const ULONG n = p->Release();
    if (n == 0) {
        auto it = g_proxies.find(p);
        if (it != g_proxies.end()) {
            for (auto f : it->second.cleanup) f(p);
            wr64(it->second.guest + 8, 0);  // stale guest pointers now fault cleanly
            // The proxy memory itself is not reused, so dangling guest references stay detectable.
            g_proxies.erase(it);
        }
    }
    retStd(c, n, 1);
}

}  // namespace host::com

namespace host {
// D3DDISPLAYMODE out-parameters (GetDisplayMode, GetAdapterDisplayMode, EnumAdapterModes):
// a refresh rate of 0 ("default", reported by some drivers and by Wine on Xvfb) becomes
// 60 Hz. The game divides by it to get its frame period, and an infinite period stops it
// from ever rendering in-game.
template <> struct Arg<D3DDISPLAYMODE*, void> {
    D3DDISPLAYMODE* p = nullptr;
    void in(Ctx* c, uint32_t& off) { p = rd32(c->esp + 4 + off) ? gp<D3DDISPLAYMODE>(rd32(c->esp + 4 + off)) : nullptr; off += 4; }
    D3DDISPLAYMODE* host() { return p; }
    void post() { if (p && p->RefreshRate == 0) p->RefreshRate = 60; }
};
}  // namespace host

#include "com_vtables.inc"

namespace host::com {

namespace {
std::vector<std::pair<CLSID, HostClassFactory>>& hostClasses() {
    static std::vector<std::pair<CLSID, HostClassFactory>> v;
    return v;
}
}  // namespace
void registerHostClass(const CLSID& clsid, HostClassFactory f) { hostClasses().emplace_back(clsid, f); }
HostClassFactory hostClass(REFCLSID clsid) {
    for (const auto& [id, f] : hostClasses())
        if (IsEqualGUID(id, clsid)) return f;
    return nullptr;
}

Class* classForIid(REFIID iid) {
    for (const auto& e : kIids)
        if (IsEqualGUID(*e.iid, iid)) return e.cls();
    return nullptr;
}

void comQueryInterface(Ctx* c) {
    IUnknown* p = unwrapRaw(arg(c, 0));
    const GUID& iid = *gp<GUID>(arg(c, 1));
    const uint32_t out = arg(c, 2);
    Class* cls = classForIid(iid);
    if (!p || !cls) {
        if (out) wr32(out, 0);
        HLOG(1, "QueryInterface({%08lX-...}) -> E_NOINTERFACE", iid.Data1);
        retStd(c, static_cast<uint32_t>(E_NOINTERFACE), 3);
        return;
    }
    void* q = nullptr;
    const HRESULT hr = p->QueryInterface(iid, &q);
    if (out) wr32(out, SUCCEEDED(hr) ? wrapRaw(static_cast<IUnknown*>(q), *cls) : 0);
    retStd(c, static_cast<uint32_t>(hr), 3);
}

}  // namespace host::com
