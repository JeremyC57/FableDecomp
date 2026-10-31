// Target destructor pasted unchanged. Valid placement-constructed pointers and
// component; test-only access/constructor and tracked allocator permit inspection.
#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __int64 long
#define __cdecl
#define __forceinline inline
#define __fastcall
#endif
static void fail(int line){std::fprintf(stderr,"failure %d\n",line);std::exit(1);}
#define CHECK(x) do{if(!(x))fail(__LINE__);}while(0)
// 007cbf60 CTCMultiArrow::~CTCMultiArrow
// Flags:
// Leaf: ??1CTCMultiArrow@@UAE@XZ
void tracked_free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) tracked_free(__p); }
};
template <class _Tp, class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp> inline void __fastcall _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    _Tp* begin() {return this->_M_start;}
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

#ifndef FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
// Retail layout (hand-written; no members). RTTI CBaseClassNonCopyable : CBaseClass; vtable 0x0129A7C4, slot 0 scalar
// deleting dtor 0x0099A470. Ego_r.pdb / FableWin.pdb: ??0CBaseClassNonCopyable@@IAE@XZ (protected),
// ??1CBaseClassNonCopyable@@MAE@XZ (protected virtual). The copy operations are private and undefined (non-copyable).


// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#define FABLE_STATIC_ASSERT(expression) \

#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H
// Retail layout (hand-written; Ego_r.pdb has no members): the root polymorphic base. RTTI .?AVCBaseClass@@ (no bases);
// vtable 0x01231710, slot 0 scalar deleting dtor 0x004371F0, which calls ~CBaseClass 0x0099A2E0 (one vtable store).
// The default ctor is out of line (0x0099A2D0: `mov eax,ecx; mov [eax],0x01231710; ret`): Ego_r.pdb places
// ??0CBaseClass@@QAE@XZ / ??1CBaseClass@@UAE@XZ side by side (0x00B01F20 / 0x00B01F30, one .cpp) and FableWin.pdb does the
// same (0x03044700 / 0x03044720), and 1,225 landed rows call 0x0099A2D0. The copy ctor is the implicit one: FableWin.pdb
// emits ??0CBaseClass@@QAE@ABV0@@Z beside the compiler-generated ??_E (0x018CD7E0), and retail expands it inline (a bare
// vtable store in a derived copy ctor, landed chunk 577 0x00DCEDF0 / chunk 578 0x00F05F30: write `: CBaseClass(other)`).


class CBaseClass {
public:
    CBaseClass();                                                        // 0x0099A2D0
    virtual ~CBaseClass();                                               // slot 0  0x0099A2E0
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);
#endif

#endif // FABLE_ENGINE_CBASECLASS_H


class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    // slot 0
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseClassNonCopyable) == 0x4);
#endif

#endif // FABLE_ENGINE_CBASECLASSNONCOPYABLE_H

#define FABLE_RETAIL_BASE_OBJECT
#ifndef FABLE_ENGINE_CBASEOBJECT_H
#define FABLE_ENGINE_CBASEOBJECT_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Retail opt-in (2026-10-30, class-model migration wave 5): define FABLE_RETAIL_BASE_OBJECT for the polymorphic retail class.
// RTTI .?AVCBaseObject@@ : CBaseClass (ghidra_out/rtti_bases.tsv, 1 base). Vtable 0x0129A7D8, 2 slots (the next dword
// is string data): slot 0 scalar deleting dtor 0x0099A4D0, slot 1 Validate 0x0099A400 (`ret`). Members and spellings are
// the Ego_r.pdb and FableWin.pdb names, identical in both (Ego_r 0x00B01FD0-0x00B020F0, FableWin 0x03044AF0-0x03044E40):
// ??0CBaseObject@@IAE@XZ, ??0CBaseObject@@IAE@ABV0@@Z, ??4CBaseObject@@IAEAAV0@ABV0@@Z (protected), ??1CBaseObject@@MAE@XZ
// (protected virtual), ?Validate@CBaseObject@@UBEXXZ, ?ClearIntelligentPointers@CBaseObject@@QAEXXZ,
// ?InternalValidate@CBaseObject@@IBEXXZ. Retail bodies (CBaseClass.cpp's TU): ctor 0x0099A380 and copy ctor 0x0099A390
// both `mov [eax],0x0129A7D8; mov [eax+4],0` (the copy does not copy the pointer); operator= 0x0099A3B0 `mov eax,ecx;
// ret 4`; ClearIntelligentPointers 0x0099A3C0 (zeroes [p+0], deletes p when [p+4]==0, zeroes +4); ~CBaseObject 0x0099A430
// (the same body inline, then the CBaseClass vtable store: same-TU auto-inlining, wave 2 LESSONS item 2). All of them are out
// of line: no landed row outside that TU stores 0x0129A7D8, and the four spellings above are bound in reloc_bindings.tsv.
// Layout (retail bytes): IntelligentPointer +0x04 (read/zeroed at [ecx+4] by 0x0099A3C0); sizeof 8 (Ego_r.pdb agrees).
// unproven: InternalValidate has no retail body (debug only: Validate is `ret`); the class key of CBaseObjectPointer
// (no PDB mangled name carries it; 1,839 landed rows say `class`, 72 `struct`).
// The CBaseClass.cpp TU rows (0x0099A2D0-0x0099A4D0) model this class row-locally by design (auto-inlined bodies).
#ifdef FABLE_RETAIL_BASE_OBJECT


class CBaseObjectPointer;
class CBaseObject : public CBaseClass {
protected:
    CBaseObject();                                                       // 0x0099A380
    CBaseObject(const CBaseObject&);                                     // 0x0099A390
    CBaseObject& operator=(const CBaseObject&);                          // 0x0099A3B0
public:
    void ClearIntelligentPointers();                                     // 0x0099A3C0
    virtual void Validate() const;                                       // slot 1  0x0099A400
protected:
    virtual ~CBaseObject();                                              // slot 0  0x0099A4D0 (scalar deleting); ~ 0x0099A430
    void InternalValidate() const;                                       // unproven: no retail body
public:
    CBaseObjectPointer* IntelligentPointer;                              // +0x04
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
#endif
#else // flattened Ego_r layout (the landed default)
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CBaseObject = 0x8.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


struct CBaseObjectPointer;

#pragma pack(push, 1)
class CBaseObject {
public:
    void*               __vftable;                           // +0x00 vptr, or first dword of a flattened base subobject
    CBaseObjectPointer* IntelligentPointer;                  // +0x04
};
#pragma pack(pop)

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBaseObject, __vftable) == 0x0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBaseObject, IntelligentPointer) == 0x4);
#endif

#endif // FABLE_RETAIL_BASE_OBJECT
#endif // FABLE_ENGINE_CBASEOBJECT_H

#define CThing CThingFields
#define CTCGraphicAppearance CTCGraphicAppearanceFields
#ifndef FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHING_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThing = 0xa8.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThing = 0x98.
// Retail evidence: 5 trusted landed function(s); reconciliation 3 conflict(s) in 3 file(s) vs 2 agreeing.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


class CCharStringData;
class CDefPointeeBase;
class CTCDriverBase;
struct CTCGraphicAppearance;
class CTCPhysicsBase;

#pragma pack(push, 1)
class CThing {
public:
    void*                 __vftable;                         // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char         _pad_0x04[0x4];                    // +0x04
    unsigned char         _pad_0x08[0x4];                    // +0x08
    unsigned char         _pad_0x0c[0x4];                    // +0x0c
    unsigned char         _pad_0x10[0x1];                    // +0x10
    unsigned char         _pad_0x11[0x7];                    // +0x11
    unsigned __int64      UniqueID;                          // +0x18
    unsigned char         UsingComponents[0x24];             // +0x20 bitset<274>
    unsigned char         Components[0x10];                  // +0x44 CVectorMap<ETCInterfaceType,CTCBase_*,CKeyPairCompareLess<ETCInterfaceType,CTCBase_*>_>
    unsigned char         UpdateComponentsList[0x4];         // +0x54 list<CTCBase_*,std::allocator<CTCBase_*>_>
    CCharStringData*      SerialisationSectionName_PStringData; // +0x58 CCharString::PStringData
    CTCDriverBase*        PTCDriver;                         // +0x5c
    CTCPhysicsBase*       PhysicsTC;                         // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance;              // +0x64
    long                  WorldFrameCreatedAt;               // +0x68
    unsigned long         ThingTypeFlags;                    // +0x6c
    CDefPointeeBase*      PBaseDef_Object;                   // +0x70 CDefPointer<CThingBaseDef_const_>::Object
    CCharStringData*      ScriptName_PStringData;            // +0x74 CCharString::PStringData
    CCharStringData*      ScriptData_PStringData;            // +0x78 CCharString::PStringData
    CThing*               RenderListPrev;                    // +0x7c
    CThing*               RenderListNext;                    // +0x80
    CThing*               UpdateListPrev;                    // +0x84
    CThing*               UpdateListNext;                    // +0x88
    unsigned short        DefGlobalIndex;                    // +0x8c
    short                 CurrentMapNumber;                  // +0x8e
    signed char           PlayerNumber;                      // +0x90
    unsigned char         CreatedBySerialisation[0x1];       // +0x91 bitfield group: uchar:1
    unsigned char         PausedDuringCutscenes[0x1];        // +0x92 bitfield group: uchar:1
    unsigned char         AddingTCsPermitted[0x1];           // +0x93 bitfield group: uchar:1
    unsigned char         InitialisationStage[0x4];          // +0x94 bitfield group: uchar:3
};
#pragma pack(pop)

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CThing) == 0x98);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, __vftable) == 0x0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UniqueID) == 0x18);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UsingComponents) == 0x20);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, Components) == 0x44);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateComponentsList) == 0x54);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, SerialisationSectionName_PStringData) == 0x58);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PTCDriver) == 0x5c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PhysicsTC) == 0x60);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PTCGraphicAppearance) == 0x64);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, WorldFrameCreatedAt) == 0x68);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, ThingTypeFlags) == 0x6c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PBaseDef_Object) == 0x70);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptName_PStringData) == 0x74);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptData_PStringData) == 0x78);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListPrev) == 0x7c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListNext) == 0x80);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListPrev) == 0x84);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListNext) == 0x88);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, DefGlobalIndex) == 0x8c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, CurrentMapNumber) == 0x8e);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PlayerNumber) == 0x90);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, CreatedBySerialisation) == 0x91);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PausedDuringCutscenes) == 0x92);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, AddingTCsPermitted) == 0x93);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, InitialisationStage) == 0x94);
#endif

#endif // FABLE_ENGINE_CTHING_H

#undef CThing
#undef CTCGraphicAppearance
#define CClass CClassFields
#ifndef FABLE_ENGINE_CCLASS_H
#define FABLE_ENGINE_CCLASS_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CClass = 0x11.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


#pragma pack(push, 1)
class CClass {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _base_0x04[0x4];                           // +0x04 flattened base-class subobject(s)
    unsigned char ClassListPos[0x4];                         // +0x08 _List_iterator<std::_List_val<CClass_*,std::allocator<CClass_*>_>_>
    unsigned char TypeListPos[0x4];                          // +0x0c _List_iterator<std::_List_val<CClass_*,std::allocator<CClass_*>_>_>
    unsigned char Type;                                      // +0x10
};
#pragma pack(pop)

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CClass) == 0x11);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CClass, __vftable) == 0x0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CClass, ClassListPos) == 0x8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CClass, TypeListPos) == 0xc);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CClass, Type) == 0x10);
#endif

#endif // FABLE_ENGINE_CCLASS_H

#undef CClass
#define CTCBase CTCBaseFields
#ifndef FABLE_ENGINE_CTCBASE_H
#define FABLE_ENGINE_CTCBASE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCBase = 0xc.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>


class CThing;

#pragma pack(push, 1)
class CTCBase {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    CThing*       ParentThing;                               // +0x04
    bool          ToBeRemoved;                               // +0x08
    bool          IsInGlobalUpdateTCs;                       // +0x09
    unsigned char _pad_0x0a[0x2];                            // +0x0a
};
#pragma pack(pop)

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CTCBase) == 0xc);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBase, __vftable) == 0x0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBase, ParentThing) == 0x4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBase, ToBeRemoved) == 0x8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBase, IsInGlobalUpdateTCs) == 0x9);
#endif

#endif // FABLE_ENGINE_CTCBASE_H

#undef CTCBase
#define FABLE_RETAIL_DEF_CLASS_BASE
#ifndef FABLE_ENGINE_CDEFPOINTER_H
#define FABLE_ENGINE_CDEFPOINTER_H
// Retail model (hand-written 2026-10-30, class-model migration wave 2): the counted def handle CDefPointer<T>.
// Layout: Ego_r.pdb CDefPointer<T> is 4 bytes, `CDefPointeeBase* Object` at +0 (ghidra_out/struct_layouts_egor.tsv; every
// instantiation). Ego_r inlines every member; the member set is FableWin.pdb's (debug, nothing inlined):
//   ??0..QAE@W4ENullDef@@@Z (332 instantiations), copy ??0..QAE@ABV0@@Z (52), the template converting ctor ??$?0 (e.g.
//   CDefPointer<const CDefClassBase>::CDefPointer<CDefClassBase>(const CDefPointer<CDefClassBase>&)), ??1 (249), ??4 (31),
//   the template ??$?4 operator=<U>, ?Release@..QAEXXZ (249), ?InternalGetObject@..QBEPAVCDefPointeeBase@@XZ (245),
//   ??$SetDynamicCast@$$CBVCDefClassBase@@@..QAEXABV?$CDefPointer@$$CBVCDefClassBase@@@@@Z (246), ??C operator-> (229), ??D
//   operator* (42), ?Get@..QBEPBV..XZ (73), ?IsNull@..QBE_NXZ (42), ?Reset@..QAEXPBV..@Z (18), ?GetRefCount@..QBEKXZ (1).
// Inline bodies (retail bytes): retail expands all of them. One template gives both release shapes the landed GetSubDef<T>
// rows show (0x008C79B0 and family): the manager's CDefPointer<CDefClassBase> temporary releases as `mov edi,[eax+4]; ...
// dec edi; ... mov [eax],edi; jne`, the CDefPointer<const T> assignment and destructor as `dec dword ptr [ecx+4]; jne`;
// Release() must hold the pointee in a local (the temporary's Delete call reuses ecx; reloading Object differs).
// The assignment shape (`cmp; test; dec [ecx+4]; jne; call [eax+4]; and [esi],0; inc [eax+4]`) is the out-of-line
// operator= rows (e.g. 0x00592844). SetDynamicCast<U>: FableWin.pdb pairs it with GFDebugPDynamicCast<CDefPointeeBase*,
// const T*> (a debug-only type check); release code is the plain assignment.
// Forced inline: retail never calls a CDefPointer member out of line (no relocation binding names one; the four
// out-of-line operator= bodies, e.g. 0x00592844, are unreferenced COMDATs), size-optimised TUs included (landed GetSubDef<T>
// rows 0x007BE88C / 0x007BE92E: plain `inline` is called out of line there, 112 vs 162 bytes); __forceinline reproduces that.
// ENullDef constructor (added 2026-10-31, owner request): FableWin.pdb's ??0?$CDefPointer@..@@QAE@W4ENullDef@@@Z (332
// instantiations) is the null constructor; no ??0..@@QAE@XZ default constructor exists in either PDB although classes
// default-construct CDefPointer members, so the enum parameter carries a default argument (inferred from that absence).
// The enumerator's name is in neither PDB name list: NULL_DEF is a placeholder spelling (naming queue), the enum's type
// name ENullDef is the PDB's. Retail bytes: `and/mov dword ptr [..], 0` — the body is `Object = 0`. Unproven: Reset's body.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CDefPointer.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else

#ifndef FABLE_ENGINE_CDEFPOINTEEBASE_H
#define FABLE_ENGINE_CDEFPOINTEEBASE_H
// Retail layout (hand-written; no Ego_r layout row: the class adds no members). RTTI: CDefPointeeBase : CResource.
// Vtable 0x01230BA0: slot 0 scalar deleting dtor 0x0042D9A0, slots 1-9 inherited from CResource unchanged.
// Only the FABLE_RETAIL_DEF_CLASS_BASE opt-in uses it (included by engine/CDefClassBase.h). The dtor is inline: 0x0042D9A0
// stores this vtable and calls ~CResource 0x009FC550; only FableWin.pdb (debug) has ??1CDefPointeeBase@@UAE@XZ.
#include <stddef.h>

#ifndef FABLE_ENGINE_CRESOURCE_H
#define FABLE_ENGINE_CRESOURCE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CResource = 0x1c.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
// Retail opt-in: define FABLE_RETAIL_DEF_CLASS_BASE for the polymorphic retail class (RTTI CResource :
// CIVCountedPointeeBase; vtable 0x012354A4, slots 2-9 below with their retail addresses; Ego_r.pdb signatures).
#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H
#define FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CIVCountedPointeeBase = 0x8.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
// Retail opt-in: define FABLE_RETAIL_DEF_CLASS_BASE before the include for the polymorphic retail class (vtable
// 0x012354D0: slot 0 scalar deleting dtor 0x004664E0, slot 1 Delete 0x0042D930). The dtor is inline (0x004664E0 is only the
// vtable store + conditional delete; ~CResource 0x009FC550 ends with the same store; only FableWin.pdb has the ??1 symbol).
// The const reference-count accessors AddRef / Release (returns the new count, long) / GetRefCount are FableWin.pdb-only
// (Ego_r and retail inline them everywhere; no retail body). Retail shapes: `inc [eax+4]`, `dec [ecx+4]; jne` and, for the
// CDefPointer<CDefClassBase> temporary, `mov edi,[eax+4]; dec edi; mov [eax],edi` (engine/CDefPointer.h; landed GetSubDef<T>
// rows, wave 2). Whether Lionhead wrote IVRefCount `mutable` or const_cast in the accessors is not visible in the bytes (the
// two compile identically); `mutable` follows the landed local models.
// The flattened layout below stays the
// default so the landed includers are unchanged.
#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }                                 // slot 0  0x004664E0 (scalar deleting; inline dtor)
    virtual void Delete() const;                                         // slot 1  0x0042D930
    void AddRef() const { ++IVRefCount; }                                // FableWin.pdb ?AddRef@CIVCountedPointeeBase@@QBEXXZ (inline)
    long Release() const { return --IVRefCount; }                        // FableWin.pdb ?Release@CIVCountedPointeeBase@@QBEJXZ (inline)
    long GetRefCount() const { return IVRefCount; }                      // FableWin.pdb ?GetRefCount@CIVCountedPointeeBase@@QBEJXZ (inline)
    mutable unsigned long IVRefCount;                                    // +0x04 (mutable: the const accessors count; see above)
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#endif
#else // flattened Ego_r layout (the landed default)
#include <stddef.h>


#pragma pack(push, 1)
class CIVCountedPointeeBase {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long IVRefCount;                                // +0x04
};
#pragma pack(pop)

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, __vftable) == 0x0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, IVRefCount) == 0x4);
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE

#endif // FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H

class CResourceList;
class CResource : public CIVCountedPointeeBase {
public:
    CResource();                                                         // 0x009FBEC0 (??0CResource@@QAE@XZ, bound)
    virtual ~CResource();                                                // slot 0  0x004664FF (scalar deleting); ~CResource 0x009FC550
    virtual bool UnloadResource();                                       // slot 2  0x0042D940
    virtual unsigned long GetUnloadableMemory() const;                   // slot 3  0x009FBEF0
    virtual bool IsResourceLoaded() const;                               // slot 4  0x0042D950
    virtual void OnResourceLoaded();                                     // slot 5  0x0042D960
    virtual bool CanAbortResourceLoad(long) const;                       // slot 6  0x0042D970
    virtual void OnResourceLoadAborted();                                // slot 7  0x0042D980
    virtual unsigned long GetDebugMemoryInUse() const;                   // slot 8  0x009FBF00
    virtual void ResetDebugMemoryInUse();                                // slot 9  0x009FBF10
    CResourceList* ResourceList;                                         // +0x08
    CResource*     PrevResource;                                         // +0x0c
    CResource*     NextResource;                                         // +0x10
    unsigned long  ResourceSize;                                         // +0x14
    unsigned long  LastUsedFrame;                                        // +0x18
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
#endif
#else // flattened Ego_r layout (the landed default)
#include <stddef.h>


class CResourceList;

#pragma pack(push, 1)
class CResource {
public:
    void*          __vftable;                                // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char  _pad_0x04[0x4];                           // +0x04
    CResourceList* ResourceList;                             // +0x08
    CResource*     PrevResource;                             // +0x0c
    CResource*     NextResource;                             // +0x10
    unsigned long  ResourceSize;                             // +0x14
    unsigned long  LastUsedFrame;                            // +0x18
};
#pragma pack(pop)

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, __vftable) == 0x0);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceList) == 0x8);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, PrevResource) == 0xc);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, NextResource) == 0x10);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceSize) == 0x14);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, LastUsedFrame) == 0x18);
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE

#endif // FABLE_ENGINE_CRESOURCE_H


class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }                                       // slot 0  0x0042D9A0 (scalar deleting; inline dtor)
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDefPointeeBase) == 0x1c);
#endif

#endif // FABLE_ENGINE_CDEFPOINTEEBASE_H

#if defined(_MSC_VER)
#define FABLE_DEFPTR_INLINE __forceinline
#else
#define FABLE_DEFPTR_INLINE inline
#endif

enum ENullDef { NULL_DEF };                                          // W4ENullDef@@ (global); enumerator name unproven

template <class T> class CDefPointer {
public:
    FABLE_DEFPTR_INLINE CDefPointer(ENullDef = NULL_DEF) : Object(0) {}             // ??0..QAE@W4ENullDef@@@Z
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDefPointer<CDefPointeeBase>) == 0x4);
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CDEFPOINTER_H

#pragma pack(push,1)
class CClass:public CBaseObject{unsigned char unreadClass[sizeof(CClassFields)-sizeof(CBaseObject)];};
// Layout: native CThing Dead bit at91; true object/class prefix retained.
class CThing:public CClass{public:
 unsigned char unreadThing[0x91-sizeof(CClass)];bool Dead:1;unsigned char unreadFlags:7;
 unsigned char unreadTail[sizeof(CThingFields)-0x92];
public:bool IsDead()const{return Dead;}void Kill(bool);
};
#pragma pack(pop)
#pragma pack(push, 1)
class CBaseIntelligentPointer { public:                     // vtable 0x0129C95C, 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
public:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return GetPointer(); }
    T* operator->() const { return GetPointer(); }
private:
    T* GetPointer() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

template <class T> inline bool __fastcall IS_THING_ALIVE(const T& p)
{
    if (p.Get() != 0) {
        if (!p->IsDead())
            return true;
    }
    return false;
}

class CTCBase : public CBaseClassNonCopyable { public:CTCBase(CThing&t):ParentThing(t),ToBeRemoved(false),IsInGlobalUpdateTCs(false){}
protected:
    CThing& ParentThing;
private:                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
public:
    virtual ~CTCBase();                                     // 0x00686830
};

class CSpecialAbilitiesMultiArrowDef;
class CTCMultiArrow : public CTCBase { public:CTCMultiArrow(CThing&t):CTCBase(t){}             // vtable 0x0126ED7C
    CDefPointer<const CSpecialAbilitiesMultiArrowDef>   PDef;                       // +0x0C
    long                                                NumberOfShots;              // +0x10
    long                                                NumberInVolley;             // +0x14
    long                                                NumOfVolleys;               // +0x18
    long                                                SpellLevel;                 // +0x1C
    bool                                                RemovedEffect;              // +0x20
    long                                                CreateAmmoEffectCount;      // +0x24
    long                                                CreateAmmoEffectDelay;      // +0x28
    long                                                CreateAmmoEffectTimer;      // +0x2C
    std::vector<CIntelligentPointer<CThing> >           AmmoEffects;                // +0x30
    CIntelligentPointer<CThing>                         BowEffect;                  // +0x3C
    bool                                                CutSceneCancelled;          // +0x44
public:
    virtual ~CTCMultiArrow();
};

CTCMultiArrow::~CTCMultiArrow()
{
    if (IS_THING_ALIVE(BowEffect))
        BowEffect->Kill(true);
    for (long i = 0; i < (long)AmmoEffects.size(); ++i) {
        if (IS_THING_ALIVE(AmmoEffects[i]))
            (*(AmmoEffects.begin()+i))->Kill(true);
    }
}

class CBaseObjectPointer{public:CBaseObject*Object;};
static const CBaseIntelligentPointer*pointerOrder[5];static CThing*killOrder[5];
static int pointerDestroyed,killed,expectedKills,expectedPointers,defDeleted,baseDestroyed;
static void*allocation;static bool tracking;
static CTCMultiArrow*expectedComponent;
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::~CBaseObject(){}void CBaseObject::Validate()const{}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{CHECK(tracking);return PData?PData->Object:0;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){if(tracking){CHECK(killed==expectedKills);CHECK(pointerDestroyed<expectedPointers&&this==pointerOrder[pointerDestroyed++]);}}
void CThing::Kill(bool b){CHECK(tracking&&b&&!Dead&&killed<expectedKills&&this==killOrder[killed++]);Dead=true;}
void tracked_free(void*p){CHECK(tracking&&p==allocation&&pointerDestroyed==expectedPointers);allocation=0;std::free(p);}
CTCBase::~CTCBase(){if(tracking){CHECK(this==expectedComponent&&pointerDestroyed==expectedPointers&&allocation==0);++baseDestroyed;}}
void CIVCountedPointeeBase::Delete()const{CHECK(false);}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=0;}
CResource::~CResource(){}bool CResource::UnloadResource(){return false;}unsigned long CResource::GetUnloadableMemory()const{return 0;}bool CResource::IsResourceLoaded()const{return false;}
void CResource::OnResourceLoaded(){}bool CResource::CanAbortResourceLoad(long)const{return false;}void CResource::OnResourceLoadAborted(){}unsigned long CResource::GetDebugMemoryInUse()const{return 0;}void CResource::ResetDebugMemoryInUse(){}
class TestDef:public CDefPointeeBase{public:virtual void Delete()const{CHECK(IVRefCount==0&&tracking&&pointerDestroyed==expectedPointers&&allocation==0);++defDeleted;}};
int main(){
 int cases=0;tracking=false;
 for(int count=0;count<4;++count)for(int bow=0;bow<3;++bow)for(int pattern=0;pattern<9;++pattern)for(int refs=0;refs<3;++refs){
  CThing parent,objects[4];CBaseObjectPointer links[4];TestDef def;
  void*storage=std::malloc(sizeof(CTCMultiArrow));CHECK(storage);CTCMultiArrow*component=new(storage)CTCMultiArrow(parent);expectedComponent=component;
  component->PDef.Object=refs?&def:0;def.IVRefCount=refs;component->BowEffect.PData=bow?&links[0]:0;links[0].Object=&objects[0];objects[0].Dead=bow==1;
  const int capacity=count?count:(pattern&1);allocation=capacity?std::malloc(sizeof(CIntelligentPointer<CThing>)*capacity):0;CHECK(!capacity||allocation);
  CIntelligentPointer<CThing>*items=static_cast<CIntelligentPointer<CThing>*>(allocation);
  component->AmmoEffects._M_start=items;component->AmmoEffects._M_finish=items?items+count:0;component->AmmoEffects._M_end_of_storage._M_data=items?items+capacity:0;
  pointerOrder[0]=&component->BowEffect;expectedPointers=count+1;expectedKills=0;if(bow==2)killOrder[expectedKills++]=&objects[0];
  for(int i=0;i<count;++i){new(items+i)CIntelligentPointer<CThing>;int state=(pattern+i)%3;links[i+1].Object=&objects[i+1];objects[i+1].Dead=state==1;items[i].PData=state?&links[i+1]:0;pointerOrder[i+1]=items+i;if(state==2)killOrder[expectedKills++]=&objects[i+1];}
  killed=pointerDestroyed=defDeleted=baseDestroyed=0;tracking=true;
  component->~CTCMultiArrow();tracking=false;
  CHECK(killed==expectedKills&&pointerDestroyed==expectedPointers&&baseDestroyed==1&&allocation==0);
  CHECK(defDeleted==(refs==1?1:0)&&def.IVRefCount==static_cast<unsigned long>(refs?refs-1:0));
  std::free(storage);++cases;
 }
 CHECK(cases==324);
#ifdef _MSC_VER
 CHECK(sizeof(CTCBase)==12&&sizeof(CTCMultiArrow)==0x48&&sizeof(CIntelligentPointer<CThing>)==8&&sizeof(std::vector<CIntelligentPointer<CThing> >)==12);
#else
 CHECK(sizeof(long)==8);
#endif
 std::puts("007cbf60_TEST PASS");return 0;
}