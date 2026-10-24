// _global::UninitFillN_CScriptThing_008ac180(CScriptThing* first, unsigned int n, const CScriptThing& x, const __false_type&) -- STLport __uninitialized_fill_n<CScriptThing*>: copy-construct n script-thing handles (share the implementation, add a reference), return first + n
// Leaf: UninitFillN_CScriptThing_008ac180
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AC180, length 54 bytes, `ret 8`.
// Callers: one site, 0x008AD69A in the vector<CScriptThing> insert-range helper 0x008AD5F0.
// Identity: the catalog label `_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>*, ...>` is
//   propagated. The vtable stored at +0 is 0x01238C8C = CScriptThing (rtti-vtables.tsv); the element copy is exactly the
//   landed out-of-line CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 inlined. engine/CScriptThing.h flattens the
//   vptr into a plain `__vftable` word and has no constructors (its implicit copy would copy the vptr word, retail stores
//   the constant), so the class is modelled locally with the header's member names, as the landed 0x004ABE90.
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)`, `__fastcall` (first in ecx -> eax as `cur`,
//   n in edx -> esi counted down, &x on the stack -> edx loaded once). CBase's vtable store is dead; CScriptThing's comes
//   first, then the CCountedPointer<CScriptThing> PImp copy (Data, Info, `++Info->m_references` at +0 when non-null);
//   placement-new null test; /O2 loop-head pad.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct CCPPointerInfo {
    long m_references;                          // +0 reference count
};

struct CBase {
    virtual ~CBase();                           // 0x0099A2E0
};

struct CScriptThing;

struct CCountedPointer_CScriptThing {           // CCountedPointer<CScriptThing>
    CScriptThing*   Data;                       // +0
    CCPPointerInfo* Info;                       // +4
    CCountedPointer_CScriptThing(const CCountedPointer_CScriptThing& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
};

struct CScriptThing : CBase {                   // vtable 0x01238C8C, 12 bytes
    CCountedPointer_CScriptThing PImp;          // +0x04 (PImp_Data +4, PImp_Info +8)
    CScriptThing(const CScriptThing& other) : PImp(other.PImp) { }
    virtual ~CScriptThing();
};

CScriptThing* __fastcall UninitFillN_CScriptThing_008ac180(CScriptThing* first, unsigned int n, const CScriptThing& x, const __false_type&)
{
    CScriptThing* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CScriptThing(x);
    return cur;
}