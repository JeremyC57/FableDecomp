// CScript_012d9e18::CScript_012d9e18(void* first, void* second) -- script-module object: CScriptBase base, the two arguments stored at +0x44 / +0x40, this class's vtable, a cleared three-word array at +0x4c and an embedded CScriptThing at +0x64
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DBF190, length 71 bytes.
// Leaf: ??0CScript_012d9e18
// Callees: CScriptBase::CScriptBase() @ 0x00CB8110, CBaseClass::CBaseClass() @ 0x0099A2D0 (the CScriptThing's base).
// The manifest label "CShaderResource::CShaderResource" is propagated (vtable 0x012D9E18 has no RTTI: script module,
// named by its vtable as the landed AllocCVtable factories are). Same two-argument shape as the factory family
// (0x00D67740: first at +0x44, second at +0x40), plus the inline array and CScriptThing (vtable 0x01238C8C) members.
// Shape: both argument stores BEFORE the derived vtable store = an inlined `__declspec(novtable)` intermediate base that
// owns the pair (a plain init list on the most-derived class puts the vtable store between them -- the F15 family
// shape); the +0x4c array is an STLport vector default-constructed through _Vector_base(allocator_type()) with its
// _STLP_alloc_proxy (a plain three-pointer array hoists `mov ecx,edi` above the zero stores); the CScriptThing's
// counted pointer is a member object with an inline constructor (its zero stores follow the CScriptThing vtable).
// engine/CScriptThing.h (vptr, PImp_Data, PImp_Info) is flat; the inlined constructor needs the polymorphic class with
// the CCountedPointer<CScriptThing> PImp member it names, so CScriptThing is modelled locally on those members.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CCPPointerInfo;

template <class T> class CCountedPointer { public:     // Data, Info (8 bytes); the inline default constructor nulls both
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};

class CScriptThing : public CBaseClass { public:              // vtable 0x01238C8C (engine/CScriptThing.h: PImp at +4)
    CCountedPointer<CScriptThing> PImp;         // +0x04
    CScriptThing() { }
    virtual ~CScriptThing();
};

class CScriptBase { public:
    virtual ~CScriptBase();
    CScriptBase();                              // 0x00CB8110
};

template <class T> struct CAllocator {         // STLport allocator<T>: empty
    CAllocator() { }
};

template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};

template <class T> struct CVectorBase {        // STLport _Vector_base<T, allocator<T> >
    T*             m_pStart;
    T*             m_pFinish;
    CAllocProxy<T> m_endOfStorage;
    CVectorBase(const CAllocator<T>& alloc) : m_pStart(0), m_pFinish(0), m_endOfStorage(alloc, 0) { }
};

template <class T> struct CSTLportVector : CVectorBase<T> {     // STLport vector<T>
    explicit CSTLportVector(const CAllocator<T>& alloc = CAllocator<T>()) : CVectorBase<T>(alloc) { }
};

template <class T> class CArray : public CSTLportVector<T> { public: };


struct __declspec(novtable) CScriptPairBase : CScriptBase {
    unsigned char  _pad04[0x3c];
    void*          m_second;                    // +0x40
    void*          m_first;                     // +0x44
    CScriptPairBase(void* first, void* second) : m_second(second), m_first(first) { }
};

struct CScript_012d9e18 : CScriptPairBase {     // vtable 0x012D9E18
    unsigned char  _pad48[0x4];
    CArray<void*>  m_list;                      // +0x4c
    unsigned char  _pad58[0xc];
    CScriptThing   m_thing;                     // +0x64
    CScript_012d9e18(void* first, void* second);
    virtual ~CScript_012d9e18();
};

CScript_012d9e18::CScript_012d9e18(void* first, void* second)
    : CScriptPairBase(first, second)
{
}