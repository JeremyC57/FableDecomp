// NScript::CQ_OakValeBanditRaidScript::CQ_OakValeBanditRaidScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- script-module object: CScriptBase base, the two arguments stored at +0x44 / +0x40, this class's vtable, a cleared three-word array at +0x4c and an embedded CScriptThing at +0x64
// Leaf: ??0CQ_OakValeBanditRaidScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DBF190, length 71 bytes.
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
// Naming wave 35: identity out-of-line copy of the ctor the Alloc 00dc24e0 inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores script vtable 012d9e18, ret 8 (two args); PDB ??0CQ_OakValeBanditRaidScript@NScript@@QAE@PAVCScriptDataBase@@...; outer CQ_OakValeBanditRaidScript proven by its Main 00dbf2d0 entity registrations; was CScript_012d9e18::CScript_012d9e18 (wave3/labels_alloc.tsv); PDB ??0CQ_OakValeBanditRaidScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
//   Was `??0CScript_012d9e18@@QAE@PAX0@Z`; the class is renamed into NScript and takes the PDB parameter types, handed to the row's
//   argument-carrying base model unchanged (data = the first argument, stored at +0x44; the interface at +0x40); same bytes.
class CScriptDataBase;
class CGameScriptInterfaceBase;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CCPPointerInfo;

#include "engine/CCountedPointer.h"

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() { }

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

#include "engine/CArray.h"


struct __declspec(novtable) CScriptPairBase : CScriptBase {
    unsigned char  _pad04[0x3c];
    void*          m_second;                    // +0x40
    void*          m_first;                     // +0x44
    CScriptPairBase(void* first, void* second) : m_second(second), m_first(first) { }
};

namespace NScript {
class CQ_OakValeBanditRaidScript : public CScriptPairBase {
public:     // vtable 0x012D9E18
    unsigned char  _pad48[0x4];
    CArray<void*>  m_list;                      // +0x4c
    unsigned char  _pad58[0xc];
    CScriptThing   m_thing;                     // +0x64
    CQ_OakValeBanditRaidScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_OakValeBanditRaidScript();
};
}
using NScript::CQ_OakValeBanditRaidScript;

NScript::CQ_OakValeBanditRaidScript::CQ_OakValeBanditRaidScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptPairBase((void*)data, (void*)&game)
{
}