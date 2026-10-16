// CTCSkeletalMorph::CTCSkeletalMorph(CThing* thing) -- CTCBase(thing), unchanged, empty Morphs array, then PSkeletalMorph takes a new C3DSkeletalMorph
// Leaf: ??0CTCSkeletalMorph
// Retail: Fable.exe 0x007865E0, 96 bytes (`ret 4`). Vtable 0x0126B6F4 = CTCSkeletalMorph (rtti-vtables).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, operator new @ 0x00BFEA1A (0x18 bytes), C3DSkeletalMorph::
// C3DSkeletalMorph() @ 0x00A44E80, CCountedPointer<C3DSkeletalMorph>::Reset(C3DSkeletalMorph*) @ 0x00786BF0 (manifest
// "CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset", byte twin).
// Layout = Ego_r.pdb CTCSkeletalMorph (0x28): Changed +0x0C, Morphs (CArray<CSkeletalMorph>, retail 12 bytes) +0x10,
// PSkeletalMorph (CCountedPointer<C3DSkeletalMorph>: Data, Info) +0x1C.
// Shape: one shared zero register (ebx) for every init-list zero; the CArray as STLport _Vector_base with the
// _STLP_alloc_proxy end member; the new-expression's null test feeds Reset.
#include <stddef.h>
void* __cdecl operator new(size_t size);            // 0x00BFEA1A
struct CThing;
struct C3DSkeletalMorph {
    unsigned char _data[0x18];
    C3DSkeletalMorph();                             // 0x00A44E80
};
template <class T> struct _STLP_alloc_proxy {
    T* _M_data;
    _STLP_alloc_proxy(T* p) : _M_data(p) { }
};
template <class T> struct CArray {                  // retail 12 bytes: STLport _Vector_base layout
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T* Data;                                        // +0x00
    CCPPointerInfo* Info;                           // +0x04
    CCountedPointer() : Data(0), Info(0) { }
    void Reset(T* item);                            // 0x00786BF0
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
    CTCBase(CThing* thing);                         // 0x00686800
};
struct CTCSkeletalMorph : CTCBase {                 // vtable 0x0126B6F4
    struct CSkeletalMorph;
    bool                              Changed;          // +0x0c
    CArray<CSkeletalMorph>            Morphs;           // +0x10
    CCountedPointer<C3DSkeletalMorph> PSkeletalMorph;   // +0x1c
    virtual void v00();
    CTCSkeletalMorph(CThing* thing);
};

CTCSkeletalMorph::CTCSkeletalMorph(CThing* thing)
    : CTCBase(thing)
    , Changed(false)
{
    PSkeletalMorph.Reset(new C3DSkeletalMorph);
}