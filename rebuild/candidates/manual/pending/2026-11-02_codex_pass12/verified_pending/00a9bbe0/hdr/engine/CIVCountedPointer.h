#ifdef FABLE_RETAIL_GFMAIN
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
#if defined(FABLE_RETAIL_MESH_FILE_LOADER) || defined(FABLE_RETAIL_GFMAIN)
        if (Object) { Object->Release(); Object = 0; }
#else
        CIVCountedPointeeBase* object = Object;
        if (object && object->Release() == 0) object->Delete();
        Object = 0;
#endif
    }
private:
    CIVCountedPointeeBase* Object;
};
#endif

#else
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
#ifdef FABLE_RETAIL_MESH_FILE_LOADER
        if (Object) { Object->Release(); Object = 0; }
#else
        CIVCountedPointeeBase* object = Object;
        if (object && object->Release() == 0) object->Delete();
        Object = 0;
#endif
    }
private:
    CIVCountedPointeeBase* Object;
};
#endif

#endif
