// Minimal COM object base for host-implemented interfaces.
#pragma once
#include <atomic>
#include <cstring>
#include <initializer_list>

#include "w32.hpp"
#include <unknwn.h>

namespace w32 {

template <class I> struct ComObject : I {
    std::atomic<ULONG> refs{1};
    virtual ~ComObject() = default;
    // Interfaces this object answers QueryInterface for (besides IUnknown).
    virtual bool supports(REFIID iid) const = 0;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** out) override {
        if (!out) return E_POINTER;
        if (iid == IID_IUnknown || supports(iid)) {
            *out = static_cast<I*>(this);
            this->AddRef();
            return S_OK;
        }
        *out = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return ++refs; }
    ULONG STDMETHODCALLTYPE Release() override {
        const ULONG r = --refs;
        if (!r) delete this;
        return r;
    }
};

inline bool iidIn(REFIID iid, std::initializer_list<const IID*> l) {
    for (const IID* x : l)
        if (iid == *x) return true;
    return false;
}

}  // namespace w32
