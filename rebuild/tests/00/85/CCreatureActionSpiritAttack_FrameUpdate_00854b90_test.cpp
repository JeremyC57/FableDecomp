// 00854b90 behavior fixture for the TRUE 566-byte extent body (ChatGPT pass 10 fixture; body identical to extent/00854b90.cpp; agent680).
#include "fable_compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __int64 long
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
#define private public
#define protected public
#define CHECK(x) do{if(!(x)){fprintf(stderr,"CHECK line %d\n",__LINE__);abort();}}while(0)
static unsigned int reserveExtra;static int overflowCalls,baseCalls,updates,applies,destructions,crcCalls,lookups;
// 00854b90 CCreatureAction_SpiritAttack::FrameUpdate
// Leaf: ?FrameUpdate@CCreatureAction_SpiritAttack@@EAEXXZ
// Flags:
// Begin shared header engine/CCountedPointer.h
#ifndef FABLE_ENGINE_CCOUNTEDPOINTER_H
#define FABLE_ENGINE_CCOUNTEDPOINTER_H
// Retail model (hand-written 2026-10-30, class-model migration wave 9): the engine's reference-counted owning pointer and
// its shared count block. Not polymorphic (no vtable, no RTTI); every member is inline in the release build.
//
// CCPPointerInfo (Ego_r.pdb, 12 bytes): RefCount +0x0 (ulong), DeleteFunc +0x4, Data +0x8 (void*). DeleteFunc is
//   __fastcall: every retail release loads `mov ecx,[eax+8]; call [eax+4]` (0x006E7AB0, 0x00CE1000, 0x00CE1040, 0x008AB2B0)
//   and the deleters it points at are `SIXPAX@Z` (static __fastcall void(void*)) CCountedPointer<T>::DeleteData in
//   FableWin.pdb (343 instantiations); Ego_r.pdb's `_func___cdecl_void_void_ptr` is the PDB's generic spelling.
//   Ctor: FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z = (unsigned long, void (__fastcall*)(void*), void*); retail
//   Reset 0x008AB2B0 expands it after `new(0xc)`: [eax]=1, [eax+4]=deleter, [eax+8]=Data (member order).
//
// CCountedPointer<T> (Ego_r.pdb, 8 bytes for every T): Data +0x0 (T*), Info +0x4 (CCPPointerInfo*). Member set = FableWin.pdb
//   (the debug build emits them; Ego_r keeps out of line only ShareData / ReleaseData / Reset / DeleteData, the rest inline):
//   ??0 (), (T*), (const&), member template ??$?0 (const CCountedPointer<U>&); ??1; ??4 (const&), member template ??$?4;
//   ??C operator-> QBE, ??D operator* QBE; DeleteData SIXPAX@Z; Get / get QBE; GetReferenceCount QBEKXZ; IsNull QBE_NXZ;
//   ReleaseData QAEXXZ; Reset / reset QAEXPAV; ShareData QAEXPAV..PAVCCPPointerInfo@@@Z; Validate QBEXXZ (debug check);
//   member template ??$SetStaticCast (63). Not modelled: the free __fastcall templates ??$?8 / ??$?9 (operator== / !=
//   of two CCountedPointers, YI_NABV..0@Z) -- no landed includer compares two pointers.
// Bodies (retail bytes; no ICF in retail: 732 separate /O2 dtor COMDATs, one per T):
//   ~CCountedPointer = 0x006E7AB0 (MovieBase) / 0x00CE1000 (CEntityScriptBase) and the other 730: `if (Info) { dec [Info];
//     reload Info; cmp [Info],0; jne; ecx = Info->Data; call Info->DeleteFunc; operator delete(Info) }`, then Data = 0
//     and Info = 0 in that order. It is its own body, not a ReleaseData call (below).
//   ReleaseData: the same release, then Info = 0 and Data = 0 in THAT order. Retail never emits it out of line (no
//     release COMDAT with the Info-first tail exists); expanded at 0x00A45330 / 0x00A5F250 / 0x006633A0 (`mov [x+4],edi;
//     mov [x],edi`). Reset and ShareData are written over it (their dead zero stores vanish):
//   Reset(T*) = 0x008AB2B0 (and 197 more): <ReleaseData> leaves only `mov [esi+4],0`, then Data = data; if (data)
//     Info = new CCPPointerInfo(1, DeleteData, Data) (the block reads Data back; a failed new leaves Info 0).
//   ShareData = 0x00CE1040 (CCountedPointer<CScriptThing>, Ego_r.pdb name): `if (Info != info) { <release>; Data = data;
//     Info = info; if (info) ++info->RefCount; }`.
//   operator= = 0x00704580 (CScriptThing): loads o.Data / o.Info first, then the ShareData body, returns *this.
//   The (T*) ctor is the allocation without the release (out of line at 0x008C58F0 / 0x008C5980, `ret 4`).
//   Copy ctor: Data, Info copied, then `if (Info) ++Info->RefCount` (expanded in every CScriptThing copy 0x004ABE90).
//   DeleteData = `delete static_cast<T*>(data)`: 0x00CDEE00 (polymorphic T: `test ecx,ecx; je; mov eax,[ecx]; push 1;
//     call [eax]`), 0x00674E80 (CExpressionSet: the inline implicit dtor, then operator delete). Only its address is
//     stored (the count block); the landed DeleteData rows explicitly instantiate it.
// Model test (wave 9, 955 landed rows, logs/mc_cpvar_*.txt): this split (dtor own body, ReleaseData zeroing Info first)
//   gives 15 DIFFER; a dtor that calls a zeroing ReleaseData gives the same leaf bytes at more sites but names ReleaseData
//   where retail calls the Data-first dtor COMDAT, and `ReleaseData(); Data = 0; Info = 0;` gives 20 DIFFER.
// Inlining: every member is plain `inline` (in-class), never __forceinline / noinline. Retail calls the COMDAT copies at
//   some sites and expands them at others; VC7.1 RTM 3077 reproduces that per-site choice from these bodies on the landed
//   rows (wave 9 HEADERS.md lists the shapes it does not reproduce). unproven: the source order of Reset's two stores and
//   the bodies of GetReferenceCount / reset / Get / get / IsNull (FableWin names only, no retail out-of-line copy found).
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CCPPointerInfo {
public:
    unsigned long RefCount;                                              // +0x0
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             // +0x4
    void* Data;                                                          // +0x8
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     // FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0xc);
#endif
#endif

template <class T> class CCountedPointer {
public:
    T* Data;                                                             // +0x0
    CCPPointerInfo* Info;                                                // +0x4

    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   // unproven body (FableWin name only)
    void Validate() const;                                               // FableWin.pdb only (debug build check)
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   // FableWin.pdb ??$SetStaticCast@..@?$CCountedPointer@..@@QAEXABV?$CCountedPointer@..@@@Z (63)
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              // landed 0x00A32620 (C3DMeshFile::GetRoot) expands it
    }
    void reset(T* data) { Reset(data); }                                 // unproven body (FableWin name only)
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    // per-T deleter, address stored in the count block
};

#endif // FABLE_ENGINE_CCOUNTEDPOINTER_H

// End shared header engine/CCountedPointer.h
#define FABLE_RETAIL_BASE_OBJECT
// Begin shared header engine/CBaseObject.h
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
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h
// Begin shared header engine/CBaseClass.h
#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H
// Retail layout (hand-written; Ego_r.pdb has no members): the root polymorphic base. RTTI .?AVCBaseClass@@ (no bases);
// vtable 0x01231710, slot 0 scalar deleting dtor 0x004371F0, which calls ~CBaseClass 0x0099A2E0 (one vtable store).
// The default ctor is out of line (0x0099A2D0: `mov eax,ecx; mov [eax],0x01231710; ret`): Ego_r.pdb places
// ??0CBaseClass@@QAE@XZ / ??1CBaseClass@@UAE@XZ side by side (0x00B01F20 / 0x00B01F30, one .cpp) and FableWin.pdb does the
// same (0x03044700 / 0x03044720), and 1,225 landed rows call 0x0099A2D0. The copy ctor is the implicit one: FableWin.pdb
// emits ??0CBaseClass@@QAE@ABV0@@Z beside the compiler-generated ??_E (0x018CD7E0), and retail expands it inline (a bare
// vtable store in a derived copy ctor, landed chunk 577 0x00DCEDF0 / chunk 578 0x00F05F30: write `: CBaseClass(other)`).
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CBaseClass {
public:
    CBaseClass();                                                        // 0x0099A2D0
    virtual ~CBaseClass();                                               // slot 0  0x0099A2E0
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);
#endif
#endif

#endif // FABLE_ENGINE_CBASECLASS_H

// End shared header engine/CBaseClass.h
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
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
#endif
#endif
#else // flattened Ego_r layout (the landed default)
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CBaseObject = 0x8.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

struct CBaseObjectPointer;

#pragma pack(push, 1)
class CBaseObject {
public:
    void*               __vftable;                           // +0x00 vptr, or first dword of a flattened base subobject
    CBaseObjectPointer* IntelligentPointer;                  // +0x04
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBaseObject, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CBaseObject, IntelligentPointer) == 0x4);
#endif
#endif

#endif // FABLE_RETAIL_BASE_OBJECT
#endif // FABLE_ENGINE_CBASEOBJECT_H

// End shared header engine/CBaseObject.h
#define CClass CClassFields
// Begin shared header engine/CClass.h
#ifndef FABLE_ENGINE_CCLASS_H
#define FABLE_ENGINE_CCLASS_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CClass = 0x11.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

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
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CClass) == 0x11);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CClass, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CClass, ClassListPos) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CClass, TypeListPos) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CClass, Type) == 0x10);
#endif
#endif

#endif // FABLE_ENGINE_CCLASS_H

// End shared header engine/CClass.h
#undef CClass
#define CThing CThingFields
// Begin shared header engine/_quarantine/CThing.h
#ifndef FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHING_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThing = 0xa8.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThing = 0x98.
// Retail evidence: 5 trusted landed function(s); reconciliation 3 conflict(s) in 3 file(s) vs 2 agreeing.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

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
    unsigned char         UsingComponents[9*sizeof(unsigned long)];             // +0x20 bitset<274>
    unsigned char         Components[3*sizeof(void*)+4];                  // +0x44 CVectorMap<ETCInterfaceType,CTCBase_*,CKeyPairCompareLess<ETCInterfaceType,CTCBase_*>_>
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
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CThing) == 0x98);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UniqueID) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UsingComponents) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, Components) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateComponentsList) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, SerialisationSectionName_PStringData) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PTCDriver) == 0x5c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PhysicsTC) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PTCGraphicAppearance) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, WorldFrameCreatedAt) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, ThingTypeFlags) == 0x6c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PBaseDef_Object) == 0x70);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptName_PStringData) == 0x74);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptData_PStringData) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListPrev) == 0x7c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListNext) == 0x80);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListPrev) == 0x84);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListNext) == 0x88);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, DefGlobalIndex) == 0x8c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, CurrentMapNumber) == 0x8e);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PlayerNumber) == 0x90);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, CreatedBySerialisation) == 0x91);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, PausedDuringCutscenes) == 0x92);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, AddingTCsPermitted) == 0x93);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThing, InitialisationStage) == 0x94);
#endif
#endif

#endif // FABLE_ENGINE_CTHING_H

// End shared header engine/_quarantine/CThing.h
#undef CThing
#define CThingPhysical CThingPhysicalFields
// Begin shared header engine/CThingPhysical.h
#ifndef FABLE_ENGINE_CTHINGPHYSICAL_H
#define FABLE_ENGINE_CTHINGPHYSICAL_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThingPhysical = 0xd8.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThingPhysical = 0xc0.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

#pragma pack(push, 1)
class CThingPhysical {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    unsigned char _pad_0x10[0x1];                            // +0x10
    unsigned char _pad_0x11[0x7];                            // +0x11
    unsigned char _pad_0x18[0x4];                            // +0x18
    unsigned char _pad_0x1c[0x4];                            // +0x1c
    unsigned char _pad_0x20[0x38];                           // +0x20
    unsigned char _pad_0x58[0x4];                            // +0x58
    unsigned char _pad_0x5c[0x4];                            // +0x5c
    unsigned char _pad_0x60[0x4];                            // +0x60
    long          f64;                                       // +0x64 retail-only long (previous header)
    unsigned char _pad_0x68[0x4];                            // +0x68
    unsigned char _pad_0x6c[0x4];                            // +0x6c
    unsigned char _pad_0x70[0x4];                            // +0x70
    unsigned char _pad_0x74[0x4];                            // +0x74
    unsigned char _pad_0x78[0x4];                            // +0x78
    unsigned char _pad_0x7c[0x4];                            // +0x7c
    unsigned char _pad_0x80[0x4];                            // +0x80
    unsigned char _pad_0x84[0x4];                            // +0x84
    unsigned char _pad_0x88[0x4];                            // +0x88
    unsigned char _pad_0x8c[0x4];                            // +0x8c
    unsigned char _pad_0x90[0x4];                            // +0x90
    unsigned char _pad_0x94[0x1];                            // +0x94
    unsigned char _pad_0x95[0x3];                            // +0x95
    unsigned char HealthOverTimeEffects[0x4];                // +0x98 list<CThingPhysical::CHealthOverTimeHelper,std::allocator<CThingPhysical::CHealthOverTimeHelper>_>
    float         Scale;                                     // +0x9c
    unsigned char PLastCreator[0x8];                         // +0xa0 CIntelligentPointer<CThingPhysical>
    unsigned char POriginalCreator[0x8];                     // +0xa8 CIntelligentPointer<CThingPhysical>
    float         MaxHealth;                                 // +0xb0
    float         Health;                                    // +0xb4
    long          CanComeBetweenCameraAndObservedThing;      // +0xb8
    unsigned char AbleToBeEngagedInCombat[0x4];              // +0xbc bitfield group: uchar:1
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CThingPhysical) == 0xc0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, f64) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, HealthOverTimeEffects) == 0x98);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, Scale) == 0x9c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, PLastCreator) == 0xa0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, POriginalCreator) == 0xa8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, MaxHealth) == 0xb0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, Health) == 0xb4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, CanComeBetweenCameraAndObservedThing) == 0xb8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, AbleToBeEngagedInCombat) == 0xbc);
#endif
#endif

#endif // FABLE_ENGINE_CTHINGPHYSICAL_H

// End shared header engine/CThingPhysical.h
#undef CThingPhysical
#define CThingGameObject CThingGameObjectFields
// Begin shared header engine/CThingGameObject.h
#ifndef FABLE_ENGINE_CTHINGGAMEOBJECT_H
#define FABLE_ENGINE_CTHINGGAMEOBJECT_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThingGameObject = 0xe8.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThingGameObject = 0xd0.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

#pragma pack(push, 1)
class CThingGameObject {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    unsigned char _pad_0x10[0x1];                            // +0x10
    unsigned char _pad_0x11[0x7];                            // +0x11
    unsigned char _pad_0x18[0x4];                            // +0x18
    unsigned char _pad_0x1c[0x4];                            // +0x1c
    unsigned char _pad_0x20[0x38];                           // +0x20
    unsigned char _pad_0x58[0x4];                            // +0x58
    unsigned char _pad_0x5c[0x4];                            // +0x5c
    unsigned char _pad_0x60[0x4];                            // +0x60
    unsigned char _pad_0x64[0x4];                            // +0x64
    unsigned char _pad_0x68[0x4];                            // +0x68
    unsigned char _pad_0x6c[0x4];                            // +0x6c
    unsigned char _pad_0x70[0x4];                            // +0x70
    unsigned char _pad_0x74[0x4];                            // +0x74
    unsigned char _pad_0x78[0x4];                            // +0x78
    unsigned char _pad_0x7c[0x4];                            // +0x7c
    unsigned char _pad_0x80[0x4];                            // +0x80
    unsigned char _pad_0x84[0x4];                            // +0x84
    unsigned char _pad_0x88[0x4];                            // +0x88
    unsigned char _pad_0x8c[0x4];                            // +0x8c
    unsigned char _pad_0x90[0x4];                            // +0x90
    unsigned char _pad_0x94[0x1];                            // +0x94
    unsigned char _pad_0x95[0x7];                            // +0x95
    unsigned char _pad_0x9c[0x4];                            // +0x9c
    unsigned char _pad_0xa0[0x4];                            // +0xa0
    unsigned char _pad_0xa4[0x4];                            // +0xa4
    unsigned char _pad_0xa8[0x4];                            // +0xa8
    unsigned char _pad_0xac[0x4];                            // +0xac
    unsigned char _pad_0xb0[0x1];                            // +0xb0
    unsigned char _pad_0xb1[0x3];                            // +0xb1
    unsigned char _pad_0xb4[0x4];                            // +0xb4
    unsigned char _pad_0xb8[0x4];                            // +0xb8
    unsigned char _pad_0xbc[0x1];                            // +0xbc
    unsigned char _pad_0xbd[0x3];                            // +0xbd
    unsigned char PThingStandingOn[0x8];                     // +0xc0 CIntelligentPointer<CThing>
    unsigned char GiveHeroStatChangesOnBeingHitOverrideSet[0x8]; // +0xc8 bitfield group: uchar:1
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CThingGameObject) == 0xd0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingGameObject, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingGameObject, PThingStandingOn) == 0xc0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingGameObject, GiveHeroStatChangesOnBeingHitOverrideSet) == 0xc8);
#endif
#endif

#endif // FABLE_ENGINE_CTHINGGAMEOBJECT_H

// End shared header engine/CThingGameObject.h
#undef CThingGameObject
class CCharString;class C3DVector;class C2DVector;class CThing;class CThingPhysical;class CThingCreatureBase;class CThingAICreature;class CThingPlayerCreature;class CThingBuilding;class CTCBase;class CMaterialDef;class CEnvironment;class CEngineGraphic;class CInventoryItem;class CThingDef;class CThingProperties;class CInterpolationInfo;class CEnginePrimitivePrepareInfo;class CPhysicsObject;class CTCParamsBase;class CThingLevelScript;class CThingGameState;class CPhysicsObjectDamage;class CCollisionInfo;class CThingDrawInfo;class CThingDebugDrawInfo;class CThingPhysicalProperties;class CSoundPair;class CAnimationEntry;
template<class T>class CDefPointer;template<class T>class CArray;namespace std{template<class T>class allocator;template<class T,class A>class vector;}
class C3DVector;
class CCharString;
class CCreatureAbilityDef;
class CEditEngineDrawInfo;
class CEngineGraphic;
class CEnginePrimitivePrepareInfo;
class CHitParameters;
class CInterpolationInfo;
class CMaterialDef;
class CMemoryDataInputStream;
class CPersistContext;
class CSoundPair;
class CThing;
class CThingBaseDef;
class CThingCreatureBase;
class CThingDrawInformation;
class CThingInfo;
class CThingPhysical;
class CThingPhysicalDebugInfo;
enum ECollideType{};
class C3DVector{public:float X,Y,Z;C3DVector(){}C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}};
// Layout: native CClass17-byte flattened header, rounded20 when used as a polymorphic base.
class CClass:public CBaseObject{public:private:unsigned char _pad_class[sizeof(CClassFields)-sizeof(CBaseObject)];};
class CThing:public CClass{public:class CSerialiseInfo;
public:
public:
public:
public:
public:
public:
public:
public:
protected:
protected:
protected:
protected:
protected:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:template<class T>bool GetTC(T**);CTCBase*AddTC(const CCharString&,bool,const CTCParamsBase*);const C3DVector&GetPos()const;bool IsDead()const{return (reinterpret_cast<const CThingFields*>(this)->CreatedBySerialisation[0]&1)!=0;}unsigned long GetTypeFlags()const{return reinterpret_cast<const CThingFields*>(this)->ThingTypeFlags;}static unsigned long __fastcall StatGetTypeFlags(){return 0;}
private:unsigned char _pad_members[sizeof(CThingFields)-sizeof(CClass)];
};

// Begin shared header engine/CBaseClass.h
#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H
// Retail layout (hand-written; Ego_r.pdb has no members): the root polymorphic base. RTTI .?AVCBaseClass@@ (no bases);
// vtable 0x01231710, slot 0 scalar deleting dtor 0x004371F0, which calls ~CBaseClass 0x0099A2E0 (one vtable store).
// The default ctor is out of line (0x0099A2D0: `mov eax,ecx; mov [eax],0x01231710; ret`): Ego_r.pdb places
// ??0CBaseClass@@QAE@XZ / ??1CBaseClass@@UAE@XZ side by side (0x00B01F20 / 0x00B01F30, one .cpp) and FableWin.pdb does the
// same (0x03044700 / 0x03044720), and 1,225 landed rows call 0x0099A2D0. The copy ctor is the implicit one: FableWin.pdb
// emits ??0CBaseClass@@QAE@ABV0@@Z beside the compiler-generated ??_E (0x018CD7E0), and retail expands it inline (a bare
// vtable store in a derived copy ctor, landed chunk 577 0x00DCEDF0 / chunk 578 0x00F05F30: write `: CBaseClass(other)`).
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CBaseClass {
public:
    CBaseClass();                                                        // 0x0099A2D0
    virtual ~CBaseClass();                                               // slot 0  0x0099A2E0
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);
#endif
#endif

#endif // FABLE_ENGINE_CBASECLASS_H

// End shared header engine/CBaseClass.h
#define CCreatureActionBase CCreatureActionBaseFields
// Begin shared header engine/CCreatureActionBase.h
#ifndef FABLE_ENGINE_CCREATUREACTIONBASE_H
#define FABLE_ENGINE_CCREATUREACTIONBASE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CCreatureActionBase = 0xb4.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CCreatureActionBase = 0xa8.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CAnimationEntry;
class CCPPointerInfo;

#pragma pack(push, 1)
class CCreatureActionBase {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    PCreature[0x8];                         // +0x08 CIntelligentPointer<CThingCreatureBase>
    long             TotalTime;                              // +0x10
    long             ActionTime;                             // +0x14
    long             AnimDelayTime;                          // +0x18
    long             FramesRunning;                          // +0x1c
    long             InterruptionGroupID;                    // +0x20
    long             InterruptionPriority;                   // +0x24
    long             ActionID;                               // +0x28
    unsigned char    PItemRemoved[0x8];                      // +0x2c CIntelligentPointer<CThing>
    long             Sounds_FirstSound;                      // +0x34 CSoundPair::FirstSound
    long             Sounds_LastSound;                       // +0x38 CSoundPair::LastSound
    unsigned long    PlayingSoundID;                         // +0x3c
    float            RunningTimeSeconds;                     // +0x40
    float            LastRunningTimeSeconds;                 // +0x44
    long             StartFrame;                             // +0x48
    long             PauseFrame;                             // +0x4c
    long             PauseTimer;                             // +0x50
    float            AnimMovementStep_X;                     // +0x54 C3DVector::X
    float            AnimMovementStep_Y;                     // +0x58 C3DVector::Y
    float            AnimMovementStep_Z;                     // +0x5c C3DVector::Z
    bool             AddedActionAnim;                        // +0x60
    bool             Finished;                               // +0x61
    bool             PerformedAction;                        // +0x62
    bool             Initialised;                            // +0x63
    long             NextMovementPosWorldFrame;              // +0x64
    float            NextMovementPos_X;                      // +0x68 C3DVector::X
    float            NextMovementPos_Y;                      // +0x6c C3DVector::Y
    float            NextMovementPos_Z;                      // +0x70 C3DVector::Z
    CAnimationEntry* PAnimEntry_Data;                        // +0x74 CCountedPointer<CAnimationEntry>::Data
    CCPPointerInfo*  PAnimEntry_Info;                        // +0x78 CCountedPointer<CAnimationEntry>::Info
    CAnimationEntry* PPreceedingPAnimEntry_Data;             // +0x7c CCountedPointer<CAnimationEntry>::Data
    CCPPointerInfo*  PPreceedingPAnimEntry_Info;             // +0x80 CCountedPointer<CAnimationEntry>::Info
    unsigned char    ActionEvents[0xc];                      // +0x84 vector<CCountedPointer<NCreatureAction::CActionEventBase>,std::allocator<CCountedPointer<NCreatureAction::CActionEventBase>_>_>
    unsigned char    CutSceneEvents[0xc];                    // +0x90 map<long,CCharString,std::less<long>,std::allocator<std::pair<long_const_,CCharString>_>_>
    unsigned char    DependentThings[0xc];                   // +0x9c vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CCreatureActionBase) == 0xa8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PCreature) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, TotalTime) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, ActionTime) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, AnimDelayTime) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, FramesRunning) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, InterruptionGroupID) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, InterruptionPriority) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, ActionID) == 0x28);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PItemRemoved) == 0x2c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, Sounds_FirstSound) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, Sounds_LastSound) == 0x38);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PlayingSoundID) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, RunningTimeSeconds) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, LastRunningTimeSeconds) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, StartFrame) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PauseFrame) == 0x4c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PauseTimer) == 0x50);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, AnimMovementStep_X) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, AnimMovementStep_Y) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, AnimMovementStep_Z) == 0x5c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, AddedActionAnim) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, Finished) == 0x61);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PerformedAction) == 0x62);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, Initialised) == 0x63);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, NextMovementPosWorldFrame) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, NextMovementPos_X) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, NextMovementPos_Y) == 0x6c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, NextMovementPos_Z) == 0x70);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PAnimEntry_Data) == 0x74);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PAnimEntry_Info) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PPreceedingPAnimEntry_Data) == 0x7c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PPreceedingPAnimEntry_Info) == 0x80);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, ActionEvents) == 0x84);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, CutSceneEvents) == 0x90);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, DependentThings) == 0x9c);
#endif
#endif

#endif // FABLE_ENGINE_CCREATUREACTIONBASE_H

// End shared header engine/CCreatureActionBase.h
#undef CCreatureActionBase
#define CCreatureAction_SpiritAttack CCreatureAction_SpiritAttackFields
// Begin shared header engine/CCreatureAction_SpiritAttack.h
#ifndef FABLE_ENGINE_CCREATUREACTION_SPIRITATTACK_H
#define FABLE_ENGINE_CCREATUREACTION_SPIRITATTACK_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CCreatureAction_SpiritAttack = 0xc4.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CCreatureAction_SpiritAttack = 0xb8.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

#pragma pack(push, 1)
class CCreatureAction_SpiritAttack {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    int           field10;                                   // +0x10 retail-only int (CCreatureActionSpiritAttack_InitialiseCCreatureActionSpiritAttackUAEXXZ_008545f0.cpp)
    unsigned char _pad_0x14[0x4];                            // +0x14
    unsigned char _pad_0x18[0x4];                            // +0x18
    unsigned char _pad_0x1c[0x4];                            // +0x1c
    unsigned char _pad_0x20[0x4];                            // +0x20
    unsigned char _pad_0x24[0x4];                            // +0x24
    unsigned char _pad_0x28[0x4];                            // +0x28
    unsigned char _pad_0x2c[0x4];                            // +0x2c
    unsigned char _pad_0x30[0x4];                            // +0x30
    unsigned char _pad_0x34[0x4];                            // +0x34
    unsigned char _pad_0x38[0x4];                            // +0x38
    unsigned char _pad_0x3c[0x4];                            // +0x3c
    unsigned char _pad_0x40[0x4];                            // +0x40
    unsigned char _pad_0x44[0x4];                            // +0x44
    unsigned char _pad_0x48[0x4];                            // +0x48
    unsigned char _pad_0x4c[0x4];                            // +0x4c
    unsigned char _pad_0x50[0x4];                            // +0x50
    unsigned char _pad_0x54[0x4];                            // +0x54
    unsigned char _pad_0x58[0x4];                            // +0x58
    unsigned char _pad_0x5c[0x4];                            // +0x5c
    unsigned char _pad_0x60[0x4];                            // +0x60
    unsigned char _pad_0x64[0x4];                            // +0x64
    unsigned char _pad_0x68[0x4];                            // +0x68
    unsigned char _pad_0x6c[0x4];                            // +0x6c
    unsigned char _pad_0x70[0x4];                            // +0x70
    unsigned char _pad_0x74[0x4];                            // +0x74
    unsigned char _pad_0x78[0x4];                            // +0x78
    unsigned char _pad_0x7c[0x4];                            // +0x7c
    unsigned char _pad_0x80[0x4];                            // +0x80
    unsigned char _pad_0x84[0x24];                           // +0x84
    unsigned char PTarget[0x8];                              // +0xa8 CIntelligentPointer<CThing>
    unsigned char PParticleEmitter[0x8];                     // +0xb0 CIntelligentPointer<CThing>
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_SpiritAttack) == 0xb8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureAction_SpiritAttack, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureAction_SpiritAttack, field10) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureAction_SpiritAttack, PTarget) == 0xa8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCreatureAction_SpiritAttack, PParticleEmitter) == 0xb0);
#endif
#endif

#endif // FABLE_ENGINE_CCREATUREACTION_SPIRITATTACK_H

// End shared header engine/CCreatureAction_SpiritAttack.h
#undef CCreatureAction_SpiritAttack
class CBaseIntelligentPointer{private:CBaseObjectPointer*PData;public:CBaseIntelligentPointer();virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;void SetPItem(CBaseObject*);};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T*Get()const{return static_cast<T*>(GetPItem());}T*operator->()const{return Get();}};
// Layout: original base prefix grounded in shared native CCreatureActionBase header; PCreature is protected at8.
class CTCWeaponTrail;
class CCreatureActionBase:public CBaseObject{protected:CIntelligentPointer<CThingCreatureBase>PCreature;unsigned char _pad_action[sizeof(CCreatureActionBaseFields)-16];public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
protected:virtual void FrameUpdate();};
class CCreatureAction_SpiritAttack:public CCreatureActionBase{private:CIntelligentPointer<CThing>PTarget,PParticleEmitter;virtual void FrameUpdate();};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_SpiritAttack)==sizeof(CCreatureAction_SpiritAttackFields));
#endif
class CThingPhysical:public CThing{
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
private:unsigned char _pad_members[sizeof(CThingPhysicalFields)>sizeof(CThing)?sizeof(CThingPhysicalFields)-sizeof(CThing):1];
};
class CThingGameObject:public CThingPhysical{protected:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
public:
private:unsigned char _pad_members[sizeof(CThingGameObjectFields)>sizeof(CThingPhysical)?sizeof(CThingGameObjectFields)-sizeof(CThingPhysical):1];
};

#define CThingCreatureBase CThingCreatureBaseFields
// Begin shared header engine/CThingCreatureBase.h
#ifndef FABLE_ENGINE_CTHINGCREATUREBASE_H
#define FABLE_ENGINE_CTHINGCREATUREBASE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThingCreatureBase = 0x188.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThingCreatureBase = 0x160.
// Retail evidence: 5 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CCPPointerInfo;
class CCreatureActionBase;
class CCreatureInteraction;
class CDefPointeeBase;
class CTCCreatureModeManager;
class CThingBodyReorienter;

#pragma pack(push, 1)
class CThingCreatureBase {
public:
    void*                   __vftable;                       // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char           _pad_0x04[0x4];                  // +0x04
    unsigned char           _pad_0x08[0x4];                  // +0x08
    unsigned char           _pad_0x0c[0x4];                  // +0x0c
    unsigned char           _pad_0x10[0x1];                  // +0x10
    unsigned char           _pad_0x11[0x7];                  // +0x11
    unsigned char           _pad_0x18[0x4];                  // +0x18
    unsigned char           _pad_0x1c[0x4];                  // +0x1c
    unsigned char           _pad_0x20[0x38];                 // +0x20
    unsigned char           _pad_0x58[0x4];                  // +0x58
    unsigned char           _pad_0x5c[0x4];                  // +0x5c
    long                    q;                               // +0x60 retail-only long (previous header)
    unsigned char           _pad_0x64[0x4];                  // +0x64
    unsigned char           _pad_0x68[0x4];                  // +0x68
    unsigned char           _pad_0x6c[0x4];                  // +0x6c
    unsigned char           _pad_0x70[0x4];                  // +0x70
    unsigned char           _pad_0x74[0x4];                  // +0x74
    unsigned char           _pad_0x78[0x4];                  // +0x78
    unsigned char           _pad_0x7c[0x4];                  // +0x7c
    unsigned char           _pad_0x80[0x4];                  // +0x80
    unsigned char           _pad_0x84[0x4];                  // +0x84
    unsigned char           _pad_0x88[0x4];                  // +0x88
    unsigned char           _pad_0x8c[0x4];                  // +0x8c
    unsigned char           _pad_0x90[0x4];                  // +0x90
    unsigned char           _pad_0x94[0x1];                  // +0x94
    unsigned char           _pad_0x95[0x7];                  // +0x95
    unsigned char           _pad_0x9c[0x4];                  // +0x9c
    unsigned char           _pad_0xa0[0x4];                  // +0xa0
    unsigned char           _pad_0xa4[0x4];                  // +0xa4
    unsigned char           _pad_0xa8[0x4];                  // +0xa8
    unsigned char           _pad_0xac[0x4];                  // +0xac
    unsigned char           _pad_0xb0[0x4];                  // +0xb0
    unsigned char           _pad_0xb4[0x4];                  // +0xb4
    unsigned char           _pad_0xb8[0x4];                  // +0xb8
    unsigned char           _pad_0xbc[0x1];                  // +0xbc
    unsigned char           _pad_0xbd[0x3];                  // +0xbd
    unsigned char           _pad_0xc0[0x4];                  // +0xc0
    unsigned char           _pad_0xc4[0x4];                  // +0xc4
    unsigned char           _pad_0xc8[0x8];                  // +0xc8
    unsigned long           LastMessageEventICreatedID;      // +0xd0
    unsigned char           CombatCollisionDebugGraphics[0xc];  // +0xd4 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    CDefPointeeBase*        PDef_Object;                     // +0xe0 CDefPointer<CThingCreatureDef_const_>::Object
    long                    ShotAccuracyPercentage;          // +0xe4
    float                   InitialPos_X;                    // +0xe8 C3DVector::X
    float                   InitialPos_Y;                    // +0xec C3DVector::Y
    float                   InitialPos_Z;                    // +0xf0 C3DVector::Z
    unsigned char           PLastAttackedByCreature[0x8];    // +0xf4 CIntelligentPointer<CThingCreatureBase>
    unsigned long           WFLastAttackedByCreature;        // +0xfc
    CCreatureActionBase*    PCurrentAction_Data;             // +0x100 CCountedPointer<CCreatureActionBase>::Data
    CCPPointerInfo*         PCurrentAction_Info;             // +0x104 CCountedPointer<CCreatureActionBase>::Info
    unsigned char           PQueuedActions[0x4];             // +0x108 list<CCountedPointer<CCreatureActionBase>,std::allocator<CCountedPointer<CCreatureActionBase>_>_>
    float                   MovementVector_X;                // +0x10c C3DVector::X
    float                   MovementVector_Y;                // +0x110 C3DVector::Y
    float                   MovementVector_Z;                // +0x114 C3DVector::Z
    float                   HeadPosOffset_X;                 // +0x118 C3DVector::X
    float                   HeadPosOffset_Y;                 // +0x11c C3DVector::Y
    float                   HeadPosOffset_Z;                 // +0x120 C3DVector::Z
    long                    IdleCounter;                     // +0x124
    float                   TurnSpeed;                       // +0x128
    CCreatureInteraction*   PCreatureInteraction_Data;       // +0x12c CCountedPointer<CCreatureInteraction>::Data
    CCPPointerInfo*         PCreatureInteraction_Info;       // +0x130 CCountedPointer<CCreatureInteraction>::Info
    CTCCreatureModeManager* PTCModeManager;                  // +0x134
    unsigned char           PreviousActionHandedness[0x4];   // +0x138 ECombatAnimationHandedness
    long                    PreviousActionHandednessWF;      // +0x13c
    CThingBodyReorienter*   BodyReorienter_ptr;              // +0x140 scoped_ptr<CThingBodyReorienter>::ptr
    unsigned char           CombatDebugGraphics[0xc];        // +0x144 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char           PItemToUnsheatheAfterCutscene[0x8]; // +0x150 CIntelligentPointer<CThing>
    void*                   DebugText;                       // +0x158 char *
    unsigned char           HeadPosCached[0x1];              // +0x15c bitfield group: bool:1
    unsigned char           HiddenOnMiniMap[0x1];            // +0x15d bitfield group: bool:1
    unsigned char           IsShowingDebugInfo[0x2];         // +0x15e bitfield group: bool:1
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CThingCreatureBase) == 0x160);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, q) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, LastMessageEventICreatedID) == 0xd0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, CombatCollisionDebugGraphics) == 0xd4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PDef_Object) == 0xe0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, ShotAccuracyPercentage) == 0xe4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, InitialPos_X) == 0xe8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, InitialPos_Y) == 0xec);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, InitialPos_Z) == 0xf0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PLastAttackedByCreature) == 0xf4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, WFLastAttackedByCreature) == 0xfc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PCurrentAction_Data) == 0x100);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PCurrentAction_Info) == 0x104);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PQueuedActions) == 0x108);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, MovementVector_X) == 0x10c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, MovementVector_Y) == 0x110);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, MovementVector_Z) == 0x114);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HeadPosOffset_X) == 0x118);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HeadPosOffset_Y) == 0x11c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HeadPosOffset_Z) == 0x120);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, IdleCounter) == 0x124);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, TurnSpeed) == 0x128);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PCreatureInteraction_Data) == 0x12c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PCreatureInteraction_Info) == 0x130);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PTCModeManager) == 0x134);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PreviousActionHandedness) == 0x138);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PreviousActionHandednessWF) == 0x13c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, BodyReorienter_ptr) == 0x140);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, CombatDebugGraphics) == 0x144);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PItemToUnsheatheAfterCutscene) == 0x150);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, DebugText) == 0x158);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HeadPosCached) == 0x15c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HiddenOnMiniMap) == 0x15d);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, IsShowingDebugInfo) == 0x15e);
#endif
#endif

#endif // FABLE_ENGINE_CTHINGCREATUREBASE_H

// End shared header engine/CThingCreatureBase.h
#undef CThingCreatureBase
// Layout: inherited shared native extents retain the original creature hierarchy; no creature-specific method is called.
class CThingCreatureBase:public CThingGameObject{private:unsigned char _pad_creature[sizeof(CThingCreatureBaseFields)>sizeof(CThingGameObject)?sizeof(CThingCreatureBaseFields)-sizeof(CThingGameObject):1];};

class CCRC{public:static unsigned long __fastcall Calc(unsigned long,const void*,unsigned long);};
inline void*operator new(size_t,void*p){return p;}
extern "C" void __cdecl free(void*);
namespace std{
struct __false_type{};
template<class A,class B>struct pair{A first;B second;pair(){}pair(const A&a,const B&b):first(a),second(b){}};
template<class T>class allocator{public:allocator()throw(){}allocator(const allocator&)throw(){}template<class U>allocator(const allocator<U>&)throw(){}~allocator()throw(){}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}~_Vector_base(){if(_M_start)free(_M_start);}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:FABLE_FORCEINLINE explicit vector(const A&a=A()):_Vector_base<T,A>(a){}T*begin(){return this->_M_start;}T*end(){return this->_M_finish;}protected:void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool);public:FABLE_FORCEINLINE void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){new(this->_M_finish)T(x);++this->_M_finish;}else{__false_type tag;_M_insert_overflow(this->_M_finish,x,tag,1,true);}}};
}

namespace std{
template<class T,class A>__declspec(noinline)void vector<T,A>::_M_insert_overflow(T*position,const T&x,const __false_type&,unsigned int fill,bool atEnd){
 unsigned int oldSize=this->_M_finish-this->_M_start;
 ++overflowCalls;CHECK(fill==1&&atEnd&&position==this->_M_finish);unsigned int length=oldSize+fill+reserveExtra;
 T*start=static_cast<T*>(malloc(length*sizeof(T)));T*finish=start;
 for(T*p=this->_M_start;p!=position;++p,++finish)new(finish)T(*p);
 for(unsigned int n=0;n<fill;++n,++finish)new(finish)T(x);
 if(!atEnd)for(T*p=position;p!=this->_M_finish;++p,++finish)new(finish)T(*p);
 for(T*p=this->_M_start;p!=this->_M_finish;++p)p->~T();
 free(this->_M_start);
 this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=start+length;
}
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{private:C Compare;bool Dirty;public:FABLE_FORCEINLINE CVectorMap():Dirty(false){}std::pair<K,V>*LowerBound(const K&);FABLE_FORCEINLINE void Insert(const std::pair<K,V>&value){this->push_back(value);Dirty=true;}};
enum ETCInterfaceType{};
template<class T>FABLE_FORCEINLINE bool CThing::GetTC(T**out){ETCInterfaceType type=T::StatGetInterfaceType();CThingFields*fields=reinterpret_cast<CThingFields*>(this);if(reinterpret_cast<unsigned long*>(fields->UsingComponents)[type>>5]&(1UL<<(type&31))){ETCInterfaceType key=type;CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>&>(fields->Components);std::pair<ETCInterfaceType,CTCBase*>*i=map.LowerBound(key);if(i==map.end()||type<i->first)i=map.end();*out=reinterpret_cast<T*>(i->second);return true;}return false;}
namespace NParticleEngine{
template<class K,class V>class CParticleUpdateParamCallback;
template<class K,class V>class CParticleUpdateParamDesc:public CBaseClass{private:CParticleUpdateParamCallback<K,V>*Callback;V Index;};
template<class K,class V>class CParticleUpdateParameters:public CBaseClass{public:template<class T>class CParticleUpdateCCompareVector:public CKeyPairCompareLess<T,C3DVector>{};template<class T>class CParticleUpdateCCompareParamDesc:public CKeyPairCompareLess<T,CParticleUpdateParamDesc<K,V> >{};typedef CVectorMap<K,C3DVector,CParticleUpdateCCompareVector<K> >TVectorParams;typedef CVectorMap<K,CParticleUpdateParamDesc<K,V>,CParticleUpdateCCompareParamDesc<K> >TUpdateParams;TVectorParams VectorParams;TUpdateParams UpdateParams;FABLE_FORCEINLINE CParticleUpdateParameters(){}virtual~CParticleUpdateParameters();void Update();};
}
class CTCDParticleEmitter{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0x4b;}void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>&);};
#define CTCCoopSpirit CTCCoopSpiritFields
// Begin shared header engine/_quarantine/CTCCoopSpirit.h
#ifndef FABLE_ENGINE_CTCCOOPSPIRIT_H
#define FABLE_ENGINE_CTCCOOPSPIRIT_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCCoopSpirit = 0x34.
// Retail evidence: 1 trusted landed function(s); reconciliation 1 conflict(s) in 1 file(s) vs 0 agreeing.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CDefPointeeBase;
class CPlayer;

#pragma pack(push, 1)
class CTCCoopSpirit {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    long             NoMovementTimer;                        // +0x0c
    long             Score;                                  // +0x10
    long             OffscreenCounter;                       // +0x14
    float            MovementVector_X;                       // +0x18 C3DVector::X
    float            MovementVector_Y;                       // +0x1c C3DVector::Y
    float            MovementVector_Z;                       // +0x20 C3DVector::Z
    CPlayer*         Master;                                 // +0x24
    CDefPointeeBase* PCoopSpiritDef_Object;                  // +0x28 CDefPointer<CCoopSpiritDef_const_>::Object
    unsigned char    PParticleEmitter[0x8];                  // +0x2c CIntelligentPointer<CThing>
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CTCCoopSpirit) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCoopSpirit, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCoopSpirit, NoMovementTimer) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCoopSpirit, Score) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCoopSpirit, OffscreenCounter) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCoopSpirit, MovementVector_X) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCoopSpirit, MovementVector_Y) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCoopSpirit, MovementVector_Z) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCoopSpirit, Master) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCoopSpirit, PCoopSpiritDef_Object) == 0x28);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCoopSpirit, PParticleEmitter) == 0x2c);
#endif
#endif

#endif // FABLE_ENGINE_CTCCOOPSPIRIT_H

// End shared header engine/_quarantine/CTCCoopSpirit.h
#undef CTCCoopSpirit
class CTCCoopSpirit:public CTCCoopSpiritFields{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0xc7;}C3DVector GetFocalPos()const;};
#define CTCGraphicAppearance CTCGraphicAppearanceFields
// Begin shared header engine/_quarantine/CTCGraphicAppearance.h
#ifndef FABLE_ENGINE_CTCGRAPHICAPPEARANCE_H
#define FABLE_ENGINE_CTCGRAPHICAPPEARANCE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCGraphicAppearance = 0x74.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CTCGraphicAppearance = 0x5c.
// Retail evidence: 4 trusted landed function(s); reconciliation 4 conflict(s) in 2 file(s) vs 1 agreeing.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CTCAnimationBase;

#pragma pack(push, 1)
struct CTCGraphicAppearance {
    void*             __vftable;                             // +0x00 vptr, or first dword of a flattened base subobject
    void*             f4;                                    // +0x04 retail-only Sub* (CTCGraphicAppearance_SetAlpha_004bfac0.cpp)
    unsigned char     _pad_0x08[0x4];                        // +0x08
    unsigned char     MeshEffects[0xc];                      // +0x0c map<void_*,NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry,std::less<void_*>,std::allocator<std::pair<void_*_const,NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry>_>_>
    unsigned char     TintColours[0xc];                      // +0x18 map<void_*,CRGBColour,std::less<void_*>,std::allocator<std::pair<void_*_const,CRGBColour>_>_>
    CTCAnimationBase* PTCAnimation;                          // +0x24
    float             CachedStaticFocalPosOffset_X;          // +0x28 C3DVector::X
    float             CachedStaticFocalPosOffset_Y;          // +0x2c C3DVector::Y
    float             CachedStaticFocalPosOffset_Z;          // +0x30 C3DVector::Z
    float             Scale;                                 // +0x34
    unsigned char     Primitives[0x4];                       // +0x38 list<NGraphicAppearance::CPrimitive,std::allocator<NGraphicAppearance::CPrimitive>_>
    unsigned char     Components[0x4];                       // +0x3c list<NGraphicAppearance::CAGraphicAppearanceComponent_*,std::allocator<NGraphicAppearance::CAGraphicAppearanceComponent_*>_>
    unsigned char     LightingChannel_ChannelIndex0;         // +0x40 CEnginePrimitiveThemeChannelDesc::ChannelIndex0
    unsigned char     LightingChannel_ChannelIndex1;         // +0x41 CEnginePrimitiveThemeChannelDesc::ChannelIndex1
    unsigned char     LightingChannel_ChannelBlendFactor;    // +0x42 CEnginePrimitiveThemeChannelDesc::ChannelBlendFactor
    unsigned char     LightingChannel_Pad;                   // +0x43 CEnginePrimitiveThemeChannelDesc::Pad
    unsigned long     MainGraphic_BankIndex;                 // +0x44 CEngineGraphic::BankIndex
    float             MainGraphic_AnimStep;                  // +0x48 CEngineGraphic::AnimStep
    float             MainGraphic_RenderSizeX;               // +0x4c CEngineGraphic::RenderSizeX
    bool              MainGraphic_AdditiveAlpha;             // +0x50 CEngineGraphic::AdditiveAlpha
    unsigned char     MainGraphic_Type;                      // +0x51 CEngineGraphic::Type
    unsigned short    ActivePersistenceFlags;                // +0x52
    long              FrameUpdatedWasRenderedFlag;           // +0x54
    unsigned char     UpdatePrimitiveReasons;                // +0x58
    unsigned char     PrimitivesDrawn[0x1];                  // +0x59 bitfield group: uchar:1
    unsigned char     Alpha;                                 // +0x5a
    signed char       ByPassColorFilter;                     // +0x5b
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CTCGraphicAppearance) == 0x5c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, f4) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, MeshEffects) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, TintColours) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, PTCAnimation) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, CachedStaticFocalPosOffset_X) == 0x28);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, CachedStaticFocalPosOffset_Y) == 0x2c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, CachedStaticFocalPosOffset_Z) == 0x30);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, Scale) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, Primitives) == 0x38);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, Components) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, LightingChannel_ChannelIndex0) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, LightingChannel_ChannelIndex1) == 0x41);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, LightingChannel_ChannelBlendFactor) == 0x42);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, LightingChannel_Pad) == 0x43);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, MainGraphic_BankIndex) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, MainGraphic_AnimStep) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, MainGraphic_RenderSizeX) == 0x4c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, MainGraphic_AdditiveAlpha) == 0x50);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, MainGraphic_Type) == 0x51);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, ActivePersistenceFlags) == 0x52);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, FrameUpdatedWasRenderedFlag) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, UpdatePrimitiveReasons) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, PrimitivesDrawn) == 0x59);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, Alpha) == 0x5a);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCGraphicAppearance, ByPassColorFilter) == 0x5b);
#endif
#endif

#endif // FABLE_ENGINE_CTCGRAPHICAPPEARANCE_H

// End shared header engine/_quarantine/CTCGraphicAppearance.h
#undef CTCGraphicAppearance
namespace NGraphicAppearance{class CTCGraphicAppearance:public CTCGraphicAppearanceFields{public:C3DVector GetFocalPos(float)const;};}
void CCreatureAction_SpiritAttack::FrameUpdate(){
 CCreatureActionBase::FrameUpdate();
 if(PTarget.Get()!=0&&!PTarget->IsDead()&&PParticleEmitter.Get()!=0&&!PParticleEmitter->IsDead()){
  const CThing&target=*PTarget.Get();CTCDParticleEmitter*emitter=0;PParticleEmitter->GetTC(&emitter);const CTCCoopSpirit*tc_spirit=0;PCreature->GetTC(&tc_spirit);
  C3DVector start_pos=tc_spirit->GetFocalPos();start_pos.Z+=0.8f;
  C3DVector end_pos=reinterpret_cast<const NGraphicAppearance::CTCGraphicAppearance*>(reinterpret_cast<const CThingFields*>(&target)->PTCGraphicAppearance)->GetFocalPos(1.0f);
  std::pair<unsigned long,C3DVector>u_start_pos(CCRC::Calc(0,"LIGHTNING_START",15),start_pos);
  std::pair<unsigned long,C3DVector>u_end_pos(CCRC::Calc(0,"LIGHTNING_END",13),end_pos);
  NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>update_params;
  update_params.VectorParams.Insert(u_start_pos);update_params.VectorParams.Insert(u_end_pos);
  update_params.Update();emitter->SetEmitterUpdateParameters(update_params);
 }
}

#undef private
#undef protected
CBaseObject::CBaseObject():IntelligentPointer(0){}
CBaseObject::~CBaseObject(){}
void CBaseObject::Validate()const{abort();}
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{return reinterpret_cast<CBaseObject*>(PData);}
void CBaseIntelligentPointer::SetPItem(CBaseObject*p){PData=reinterpret_cast<CBaseObjectPointer*>(p);}
static C3DVector startPosition,endPosition,seenStart,seenEnd;
static const CTCCoopSpirit*expectedSpirit;
static const NGraphicAppearance::CTCGraphicAppearance*expectedAppearance;
static const CTCDParticleEmitter*expectedDriver;
void CCreatureActionBase::FrameUpdate(){++baseCalls;}
unsigned long __fastcall CCRC::Calc(unsigned long seed,const void*data,unsigned long length){CHECK(seed==0);++crcCalls;if(length==15){CHECK(strcmp((const char*)data,"LIGHTNING_START")==0);return 0x13579bdfUL;}CHECK(length==13&&strcmp((const char*)data,"LIGHTNING_END")==0);return 0x2468ace0UL;}
C3DVector CTCCoopSpirit::GetFocalPos()const{CHECK(this==expectedSpirit);return startPosition;}
C3DVector NGraphicAppearance::CTCGraphicAppearance::GetFocalPos(float scale)const{CHECK(this==expectedAppearance&&scale==1.0f);return endPosition;}
template<class K,class V,class C>std::pair<K,V>*CVectorMap<K,V,C>::LowerBound(const K&key){++lookups;std::pair<K,V>*p=this->_M_start;while(p!=this->_M_finish&&p->first<key)++p;return p;}
namespace NParticleEngine{
template<class K,class V>CParticleUpdateParameters<K,V>::~CParticleUpdateParameters(){++destructions;}
template<class K,class V>void CParticleUpdateParameters<K,V>::Update(){CHECK(VectorParams.Dirty);CHECK(VectorParams.end()-VectorParams.begin()==2);CHECK(UpdateParams.begin()==UpdateParams.end());++updates;}
}
void CTCDParticleEmitter::SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>&p){CHECK(this==expectedDriver);CHECK(updates==1);CHECK(p.VectorParams._M_finish-p.VectorParams._M_start==2);const std::pair<unsigned long,C3DVector>*v=p.VectorParams._M_start;CHECK(v[0].first==0x13579bdfUL&&v[1].first==0x2468ace0UL);seenStart=v[0].second;seenEnd=v[1].second;++applies;}
int main(){int cases=0;
 for(int targetState=0;targetState<3;++targetState)for(int effectState=0;effectState<3;++effectState)for(int reserve=0;reserve<3;++reserve)for(int point=0;point<8;++point)for(int keyPosition=0;keyPosition<2;++keyPosition){
  CCreatureAction_SpiritAttack action;CThing target,effect;CThingCreatureBase creature;CTCDParticleEmitter driver;CTCCoopSpirit spirit;NGraphicAppearance::CTCGraphicAppearance appearance;
  expectedSpirit=&spirit;expectedAppearance=&appearance;expectedDriver=&driver;
  CThingFields*targetFields=reinterpret_cast<CThingFields*>(&target);CThingFields*effectFields=reinterpret_cast<CThingFields*>(&effect);CThingFields*creatureFields=reinterpret_cast<CThingFields*>(&creature);
  memset(targetFields->UsingComponents,0,sizeof targetFields->UsingComponents);memset(effectFields->UsingComponents,0,sizeof effectFields->UsingComponents);memset(creatureFields->UsingComponents,0,sizeof creatureFields->UsingComponents);
  targetFields->CreatedBySerialisation[0]=targetState==1?1:0;effectFields->CreatedBySerialisation[0]=effectState==1?1:0;
  targetFields->PTCGraphicAppearance=reinterpret_cast<CTCGraphicAppearance*>(&appearance);
  action.PTarget.SetPItem(targetState?&target:0);action.PParticleEmitter.SetPItem(effectState?&effect:0);action.PCreature.SetPItem(&creature);
  std::pair<ETCInterfaceType,CTCBase*>effectEntries[3],creatureEntries[3];
  for(int i=0;i<3;++i){effectEntries[i].first=(ETCInterfaceType)(i==keyPosition?0x4b:(i<keyPosition?2:0x100));effectEntries[i].second=i==keyPosition?reinterpret_cast<CTCBase*>(&driver):0;creatureEntries[i].first=(ETCInterfaceType)(i==keyPosition?0xc7:(i<keyPosition?2:0x100));creatureEntries[i].second=i==keyPosition?reinterpret_cast<CTCBase*>(&spirit):0;}
  std::pair<ETCInterfaceType,CTCBase*>**ep=reinterpret_cast<std::pair<ETCInterfaceType,CTCBase*>**>(effectFields->Components);ep[0]=effectEntries;ep[1]=effectEntries+3;
  std::pair<ETCInterfaceType,CTCBase*>**cp=reinterpret_cast<std::pair<ETCInterfaceType,CTCBase*>**>(creatureFields->Components);cp[0]=creatureEntries;cp[1]=creatureEntries+3;
  reinterpret_cast<unsigned long*>(effectFields->UsingComponents)[2]=1UL<<11;reinterpret_cast<unsigned long*>(creatureFields->UsingComponents)[6]=1UL<<7;
  startPosition.X=(point-4)*0.25f;startPosition.Y=point*2.0f;startPosition.Z=(point-4)*0.5f;endPosition.X=point*4.0f;endPosition.Y=-point*0.25f;endPosition.Z=16.0f-point;
  reserveExtra=reserve==2?3:reserve;overflowCalls=baseCalls=updates=applies=destructions=crcCalls=lookups=0;
  action.CCreatureAction_SpiritAttack::FrameUpdate();CHECK(baseCalls==1);
  if(targetState==2&&effectState==2){CHECK(updates==1&&applies==1&&destructions==1&&crcCalls==2&&lookups==2);CHECK(overflowCalls==(reserve==0?2:1));CHECK(seenStart.X==startPosition.X&&seenStart.Y==startPosition.Y);float expectedZ=startPosition.Z+0.8f;CHECK(seenStart.Z==expectedZ);CHECK(seenEnd.X==endPosition.X&&seenEnd.Y==endPosition.Y&&seenEnd.Z==endPosition.Z);}
  else CHECK(updates==0&&applies==0&&destructions==0&&crcCalls==0&&lookups==0&&overflowCalls==0);
  ++cases;
 }CHECK(cases==432);puts("00854b90_TEST PASS");return 0;}