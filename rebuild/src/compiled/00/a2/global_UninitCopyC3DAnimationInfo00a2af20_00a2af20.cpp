// _global::UninitCopy_C3DAnimationInfo_00a2af20(const C3DAnimationInfo* first, const C3DAnimationInfo* last, C3DAnimationInfo* result, const __false_type&) -- placement copy-construct [first, last) of C3DAnimationInfo bank entries (base then derived vtable, counted data pointer), return the advanced result
// Leaf: UninitCopy_C3DAnimationInfo_00a2af20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2AF20, length 114 bytes (`ret 8`).
// Identity: the two vtable stores are 0x0129CD28 = CMeshDataBankEntry and 0x0129CD64 = C3DAnimationInfo (rtti-vtables.tsv), and
//   the 0x28 stride is sizeof(C3DAnimationInfo) in Ego_r.pdb, so this is `__uninitialized_copy<C3DAnimationInfo*>`; the
//   catalog `_Move<CTCCameraPointScriptedSpline::CKeyCamera*>` is propagated. Callers: C3DMeshInfo array alloc 0x00A2B030,
//   vector reserve 0x00A2C360 and fill-insert 0x00A2CAC0.
// Layout: a local polymorphic model (the generated engine headers flatten the vtable into `void* __vftable`, which cannot
//   emit the two vtable stores); member names and offsets are Ego_r.pdb's. The pointee uses the shared
//   engine/CIVCountedPointeeBase.h (only IVRefCount is touched).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack, `ret 8`); the placement new keeps the `test eax,eax` null test on the
//   destination and the advanced result is returned.
//   The implicit copy constructor is inlined: base vtable store, Bank/Index, derived vtable store, the
//   CIVCountedPointer copy (`test ebx,ebx ; mov [edi-14h],ebx ; je ; inc [ebx+4]`), the floats; C3DVector's user copy
//   constructor puts the induction pair at +0x20 (= &MovementVector + 8).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct CMeshDataBank;

#include "engine/CIVCountedPointeeBase.h"      // Ego_r.pdb: vtable word +0, IVRefCount +4 (only the count is touched here)

template <class T>
struct CIVCountedPointer {                      // 4 bytes; the copy adds a reference
    CIVCountedPointeeBase* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) ++Object->IVRefCount; }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

struct CMeshDataBankEntry {                     // Ego_r.pdb, 0x0c bytes; vtable 0x0129CD28
    virtual ~CMeshDataBankEntry();
    CMeshDataBank* Bank;                        // +0x04
    unsigned long  Index;                       // +0x08
};

struct C3DAnimationInfo : CMeshDataBankEntry {  // Ego_r.pdb, 0x28 bytes; vtable 0x0129CD64
    struct CAnimationData;
    CIVCountedPointer<CAnimationData> Data;     // +0x0c
    float     Duration;                         // +0x10
    float     NonLoopingDuration;               // +0x14
    C3DVector MovementVector;                   // +0x18
    float     Rotation;                         // +0x24
};

C3DAnimationInfo* __fastcall UninitCopy_C3DAnimationInfo_00a2af20(const C3DAnimationInfo* first, const C3DAnimationInfo* last, C3DAnimationInfo* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) C3DAnimationInfo(*first);
    return result;
}