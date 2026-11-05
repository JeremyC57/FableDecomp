// vector<CScreenFilterSThingByPass>::push_back(const CScreenFilterSThingByPass& x) -- STLport push_back: copy-construct at the finish pointer while capacity remains, else the out-of-line overflow insert
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?push_back@
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD3E7B, 69 bytes, `__fastcall` (vector in ecx, the element on the
//   stack: `ret 4`). Callers: the cutscene screen-filter macro code @ 0x00CD1008 / 0x00CD10FA (each pushes a stack
//   CScreenFilterSThingByPass built by its constructor 0x00CD2757).
// Label: the catalog name "Vector_EmplaceBackWithCapacityCheck" is descriptive; the body is STLport's push_back for
//   the 16-byte CScreenFilterSThingByPass (PDB: struct, CScriptThing Thing +0, bool BypassSet +0xC; the vector is the
//   one CGameScriptInterface::ScreenFilterFadeTo takes).
// Callees: CScreenFilterSThingByPass's implicit copy constructor @ 0x00CD2AE2 (out of line: CScriptThing copy
//   0x004ABE90 + the flag byte; catalogued as CTavernTableDef::operator=), _M_insert_overflow(pos, x, __false_type,
//   fill_len, atend) @ 0x00CD3DCD.
// Shape: frame-pointer size TU; `_Construct(finish, x)` is the placement new with VC's null test on the pointer;
//   finish is re-read and advanced by 16; the overflow call gets (finish, x, a __false_type tag temporary homed in the
//   dead argument slot [ebp+0Bh], 1, true).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
struct CScreenFilterSThingByPass {                      // 16 bytes
    CScriptThing Thing;                                 // +0x00
    bool         BypassSet;                             // +0x0c
};
inline void* operator new(unsigned int, void* where) { return where; }
namespace std {
struct __false_type { __false_type() { } };             // empty tag with a user constructor (no byte store)
template <class T> class allocator { };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00CD3DCD
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
template class vector<CScreenFilterSThingByPass>;
}