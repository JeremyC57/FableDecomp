#ifndef FABLE_ENGINE_CIVCOUNTEDPOINTER_H
#define FABLE_ENGINE_CIVCOUNTEDPOINTER_H
// Retail declaration (Codex pass 11): Ego_r TPI0x33e8, class template V,
// one private CIVCountedPointeeBase* Object at +0. Native release is inlined.
#include "engine/CIVCountedPointeeBase.h"
template<class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) { }
    ~CIVCountedPointer() { Release(); }
    void Reset(T* object);
    T* operator->() const { return static_cast<T*>(Object); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object ? Object->GetRefCount() : 0; }
    void Release() {
        CIVCountedPointeeBase* object = Object;
        if (object && object->Release() == 0) object->Delete();
        Object = 0;
    }
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER
    // chunk 724c: FableWin ??4?$CIVCountedPointer@...@@QAEAAV0@ABV0@@Z, inlined by CPSCDecalRenderer::PreparePrimitives 0x00B1F200
    // (`cmp Object,other.Object ; je` -> inline Release -> copy -> `inc [Object+4]`).
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other) {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object) Object->AddRef();
        }
        return *this;
    }
#endif
private:
    CIVCountedPointeeBase* Object;
};
#endif
