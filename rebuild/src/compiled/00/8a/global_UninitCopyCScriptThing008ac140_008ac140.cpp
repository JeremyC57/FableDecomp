// _global::UninitCopy_CScriptThing_008ac140(const CScriptThing* first, const CScriptThing* last, CScriptThing* result, const __false_type&) -- STLport __uninitialized_copy over 12-byte CScriptThing handles: placement-copy each handle (vtable, then the counted PImp with its add-ref), return the advanced result
// Leaf: UninitCopy_CScriptThing_008ac140
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AC140, length 58 bytes (+ int3 padding), `ret 8`.
// Callers: the vector<CScriptThing> helpers of the same TU: the fill/regrowth helper @ 0x008AC180 (call 0x008AC1EC), the
//   reserve @ 0x008ACD30 (call 0x008ACDA8) and the range insert @ 0x008AD5F0 (calls 0x008AD65B / 0x008AD6B4).
// Callees: none (the copy constructor is inline). Vtable 0x01238C8C = CScriptThing (rtti-vtables.tsv).
// Layout (Ego_r.pdb CScriptThing, 0xC bytes; engine/CScriptThing.h flattens the vptr into a `__vftable` member, which a
//   polymorphic model cannot declare, so the class is modelled locally): vptr +0x0, CCountedPointer<CScriptThing> PImp
//   +0x4 = { Data +0x4, Info +0x8 }; the CCPPointerInfo count block keeps RefCount as word 0.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack): `_Construct(&*cur, *first)` = placement new (null test)
//   of the implicit copy constructor -- the vtable store, then PImp's copy (Data, Info, `if (Info) ++Info->RefCount`).
inline void* operator new(unsigned int, void* where) { return where; }

class CCPPointerInfo { public:                        // Ego_r.pdb, 12 bytes
    unsigned long RefCount;                    // +0x0
    void (__cdecl* DeleteFunc)(void*);         // +0x4
    void* Data;                                // +0x8
};

template <class T> class CCountedPointer { public:
    T* Data;                                   // +0x0
    CCPPointerInfo* Info;                      // +0x4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

CScriptThing* __fastcall UninitCopy_CScriptThing_008ac140(const CScriptThing* first, const CScriptThing* last, CScriptThing* result, const __false_type&)
{
    CScriptThing* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}