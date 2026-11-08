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
#define CHECK(x)do{if(!(x)){fprintf(stderr,"line %d\n",__LINE__);abort();}}while(0)
// 0065a075 CTCVillage::GetWarningStringForCurrentCrimes
// Leaf: ?GetWarningStringForCurrentCrimes@CTCVillage@@QBE?AVCCharString@@JAA_NAAJ@Z
// Flags: /O1 /Oy-
#include <string.h>
#pragma intrinsic(strcmp)
#define FABLE_RETAIL_DEF_CLASS_BASE
// Begin shared header engine/CDefPointer.h
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h
// Begin shared header engine/CDefPointeeBase.h
#ifndef FABLE_ENGINE_CDEFPOINTEEBASE_H
#define FABLE_ENGINE_CDEFPOINTEEBASE_H
// Retail layout (hand-written; no Ego_r layout row: the class adds no members). RTTI: CDefPointeeBase : CResource.
// Vtable 0x01230BA0: slot 0 scalar deleting dtor 0x0042D9A0, slots 1-9 inherited from CResource unchanged.
// Only the FABLE_RETAIL_DEF_CLASS_BASE opt-in uses it (included by engine/CDefClassBase.h). The dtor is inline: 0x0042D9A0
// stores this vtable and calls ~CResource 0x009FC550; only FableWin.pdb (debug) has ??1CDefPointeeBase@@UAE@XZ.
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h
// Begin shared header engine/CResource.h
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h
// Begin shared header engine/CIVCountedPointeeBase.h
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
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#endif
#endif
#else // flattened Ego_r layout (the landed default)
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

#pragma pack(push, 1)
class CIVCountedPointeeBase {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long IVRefCount;                                // +0x04
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, IVRefCount) == 0x4);
#endif
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE

#endif // FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H

// End shared header engine/CIVCountedPointeeBase.h
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
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
#endif
#endif
#else // flattened Ego_r layout (the landed default)
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

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
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceList) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, PrevResource) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, NextResource) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceSize) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, LastUsedFrame) == 0x18);
#endif
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE

#endif // FABLE_ENGINE_CRESOURCE_H

// End shared header engine/CResource.h

class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }                                       // slot 0  0x0042D9A0 (scalar deleting; inline dtor)
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDefPointeeBase) == 0x1c);
#endif
#endif

#endif // FABLE_ENGINE_CDEFPOINTEEBASE_H

// End shared header engine/CDefPointeeBase.h
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
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDefPointer<CDefPointeeBase>) == 0x4);
#endif
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CDEFPOINTER_H

// End shared header engine/CDefPointer.h
// Begin shared header engine/CParentDefClassBase.h
#ifndef FABLE_ENGINE_CPARENTDEFCLASSBASE_H
#define FABLE_ENGINE_CPARENTDEFCLASSBASE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CParentDefClassBase = 0x3d.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CParentDefClassBase = 0x39.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
// Retail opt-in (2026-10-30, class-model migration wave 2): define FABLE_RETAIL_DEF_CLASS_BASE for the polymorphic retail
// class. RTTI .?AVCParentDefClassBase@@ : CDefClassBase (ghidra_out/rtti_bases.tsv); vtable 0x01230CBC, 25 slots: slot 0
// scalar deleting dtor 0x004303F0, overrides at slots 10-16, 21, 22 and 24 (addresses per line; spellings are the Ego_r.pdb
// and FableWin.pdb names, identical in both), slots 1-9, 17, 23 inherited, 18-20 _purecall 0x00BFEA08.
// Layout (retail bytes): GetInstantiationName 0x004303B0 reads [ecx+0x25], SetInstantiationName 0x004303C0 writes it,
// GetPSubDefInfoMap 0x004303E0 is `lea eax,[ecx+0x29]`, the ctor 0x00430370 stores CDefString -1 at +0x25 and zeroes the
// CVectorMap at +0x29/+0x2D/+0x31 and its Dirty byte at +0x36: 0x39 bytes, packed like its base (Ego_r.pdb 0x3D carries a
// Dinkumware 0x14 vector). Out-of-line members: ctor 0x00430370 and ~CParentDefClassBase 0x00430300 (one TU: the deleting
// dtor 0x004303F0 auto-inlines the dtor body; derived deleting dtors call 0x00430300), copy ctor 0x0046EE98 and operator=
// 0x00431ED0 (both PDBs; bound in rebuild/oracles/reloc_bindings.tsv).
// Copy members (class-model migration wave 3 experiment, that wave's LESSONS.md item 1): they
// stay declared out of line. Undeclared (implicit) members over typed CDefString / CVectorMap models reproduce the 39 /Os
// callers of 0x00431ED0 tested and the standalone 0x00431ED0 body, but at /O2 RTM 3077 then inlines operator= into the
// callers retail makes call it (0x00434370 CEngineDef, 0x00625350) and still calls it from 0x00432E60 CFrontEndDef, which
// retail expands; no declaration (implicit, explicit inline, out of line in TU order, inline_depth, QFE 4035) gives both.
// The 5 /O2 expanders (0x00432C70, 0x00432E60, 0x00434450, 0x004345A0, 0x0067C980) keep row-local __forceinline models.
// Members: CDefString / CVectorMap have no shared header. An includer that has the complete types (CDefString and
// CVectorMap<K,V,C> defined before the include) defines FABLE_PDCB_TYPED_MEMBERS for the typed members; otherwise the two
// members are opaque with the same offsets.
#ifdef FABLE_RETAIL_DEF_CLASS_BASE
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h
// Begin shared header engine/CDefClassBase.h
#ifndef FABLE_ENGINE_CDEFCLASSBASE_H
#define FABLE_ENGINE_CDEFCLASSBASE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py then hand-corrected to retail (RETAIL_MARK below).
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CDefClassBase = 0x25.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
// Retail opt-in: define FABLE_RETAIL_DEF_CLASS_BASE for the polymorphic retail class. RTTI: CDefClassBase :
// CDefPointeeBase : CResource : CIVCountedPointeeBase. Vtable 0x01230BCC, 25 slots; slots 18-20 are _purecall 0x00BFEA08
// and are named from the derived overrides (CFireHeartPatternDef vtable 0x012CCC50: Transfer 0x00D35070, Copy 0x00D34FB0,
// GetSizeofClass 0x00D34EF0). Signatures are the Ego_r.pdb / FableWin.pdb spellings. Size 0x25, packed (corrected
// 2026-10-29 from 0x28): the class is declared under #pragma pack(1) (Ego_r.pdb 0x25), so a derived def's first member
// sits at +0x25 when it is a byte (landed CDegradableDef::operator= 0x00454434: Degradable bool +0x25, GraphicType +0x28)
// and at +0x28 when it is 4-aligned (CFireHeartPatternDef: _Construct 0x00D34EC0 copies its first member at +0x28).
// Slots 14/15 (0x0042DA60 / 0x0042DA50) are the two GetPSubDefInfo overloads; their bodies are identical
// (`xor eax,eax; ret 4`), but slot 14 is the const one: the landed const GetSubDef<T> rows (e.g. 0x008C79B0) call [vptr+0x38]
// from a const member, and the CParentDefClassBase overrides (0x009AD4A0 / 0x009AD510) are bound UBE / UAE. Declared
// non-const first, VC7.1 emits the const overload first; a probe build reproduces all 25 retail slot names (HEADERS.md).
// Non-virtual members (2026-10-29 migration wave 1): operator= 0x00431F10 (catalog + Ego_r ??4CDefClassBase@@QAEAAV0@ABV0@@Z),
// the GetSubDef member template (Ego_r spelling), CSubDefInfo {DefIndex, OriginalParentDefIndex} (Ego_r layout, 8 bytes) and
// the FableWin.pdb accessors AreDefaultValsApplied / SetDefaultValsApplied (Ego_r inlines them away: inline bodies here).
// ~CDefClassBase is inline: only FableWin.pdb (debug) has ??1CDefClassBase@@UAE@XZ, and the retail scalar deleting dtor
// 0x0042DAB0 stores the CDefPointeeBase vtable 0x01230BA0 and calls ~CResource 0x009FC550 (landed vector<CArenaRoundDef>
// ::_M_fill_insert 0x00F34D70 destroys its by-value copy with that inline sequence). FableWin.pdb also has a private
// default ctor ??0CDefClassBase@@AAE@XZ (derived defs reach it through friendship); it is not declared here yet.
#ifdef FABLE_RETAIL_DEF_CLASS_BASE
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h
// Begin shared header engine/CDefPointeeBase.h
#ifndef FABLE_ENGINE_CDEFPOINTEEBASE_H
#define FABLE_ENGINE_CDEFPOINTEEBASE_H
// Retail layout (hand-written; no Ego_r layout row: the class adds no members). RTTI: CDefPointeeBase : CResource.
// Vtable 0x01230BA0: slot 0 scalar deleting dtor 0x0042D9A0, slots 1-9 inherited from CResource unchanged.
// Only the FABLE_RETAIL_DEF_CLASS_BASE opt-in uses it (included by engine/CDefClassBase.h). The dtor is inline: 0x0042D9A0
// stores this vtable and calls ~CResource 0x009FC550; only FableWin.pdb (debug) has ??1CDefPointeeBase@@UAE@XZ.
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h
// Begin shared header engine/CResource.h
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h
// Begin shared header engine/CIVCountedPointeeBase.h
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
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#endif
#endif
#else // flattened Ego_r layout (the landed default)
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

#pragma pack(push, 1)
class CIVCountedPointeeBase {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long IVRefCount;                                // +0x04
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, IVRefCount) == 0x4);
#endif
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE

#endif // FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H

// End shared header engine/CIVCountedPointeeBase.h
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
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
#endif
#endif
#else // flattened Ego_r layout (the landed default)
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

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
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceList) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, PrevResource) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, NextResource) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceSize) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CResource, LastUsedFrame) == 0x18);
#endif
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE

#endif // FABLE_ENGINE_CRESOURCE_H

// End shared header engine/CResource.h

class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }                                       // slot 0  0x0042D9A0 (scalar deleting; inline dtor)
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDefPointeeBase) == 0x1c);
#endif
#endif

#endif // FABLE_ENGINE_CDEFPOINTEEBASE_H

// End shared header engine/CDefPointeeBase.h
class CCharString;
class CDefString;
class CDefinitionManager;
class CPersistContext;
class CDataInputStream;
class CDataOutputStream;
class CValidateDefinitionInitBase;
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <class T> class CDefPointer;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
public:
    class CSubDefInfo {                                                  // Ego_r.pdb CSubDefInfo, 8 bytes
    public:
        long DefIndex;                                                   // +0x00
        long OriginalParentDefIndex;                                     // +0x04
    };
    CDefClassBase(const CDefClassBase&);                                 // 0x0046644C
    CDefClassBase& operator=(const CDefClassBase&);                      // 0x00431F10 (Ego_r ??4CDefClassBase@@QAEAAV0@ABV0@@Z; out of line: an implicit one inlines the 0x00431F60 base copy, retail calls it)
    template <class T> bool GetSubDef(CDefPointer<const T>*) const;      // Ego_r ??$GetSubDef@V..@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBV..@@@@@Z
    bool AreDefaultValsApplied() const { return DefaultValsApplied; }    // FableWin.pdb ?AreDefaultValsApplied@CDefClassBase@@QBE_NXZ
    void SetDefaultValsApplied(bool value) { DefaultValsApplied = value; }   // FableWin.pdb ?SetDefaultValsApplied@CDefClassBase@@QAEX_N@Z
    virtual ~CDefClassBase() { }                                         // slot 0  0x0042DAB0 (scalar deleting; the dtor is inline)
    virtual bool UnloadResource();                                       // slot 2  0x009ACE90
    virtual CDefString GetInstantiationName() const;                     // slot 10 0x0042DA10
    virtual void SetInstantiationName(CDefString);                       // slot 11 0x0042DA20
    virtual void AddSubDef(const CCharString&, long, long);              // slot 12 0x0042DA30
    virtual void RemoveSubDef(const CCharString&);                       // slot 13 0x0042DA40
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             // slot 15 0x0042DA50 (declared first: VC7.1 reverses overloads)
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; // slot 14 0x0042DA60
    virtual bool IsParentDef() const;                                    // slot 16 0x0042DA70
    virtual void PostTransfer();                                         // slot 17 0x0042DA80
    virtual void Transfer(CPersistContext&) = 0;                         // slot 18
    virtual void Copy(const CDefClassBase*) = 0;                         // slot 19
    virtual unsigned long GetSizeofClass() = 0;                          // slot 20
    virtual void InternalSaveBinary(CDataOutputStream&);                 // slot 21 0x009ACAB0
    virtual void InternalLoadBinary(CDataInputStream&);                  // slot 22 0x009ACB20
    virtual void Validate(const CValidateDefinitionInitBase&, const CCharString&) const;  // slot 23 0x0042DA90
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             // slot 24 0x0042DAA0
    CDefinitionManager* PDefManager;                                     // +0x1c
    unsigned long       GlobalIndex;                                     // +0x20
    unsigned char       Setup : 1;                                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDefClassBase) == 0x25);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, PDefManager) == 0x1c);
#endif
#endif
#else // flattened Ego_r layout (the landed default)
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CDefinitionManager;

#pragma pack(push, 1)
class CDefClassBase {
public:
    void*               __vftable;                           // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char       _base_0x04[0x18];                    // +0x04 flattened base-class subobject(s)
    CDefinitionManager* PDefManager;                         // +0x1c
    unsigned long       GlobalIndex;                         // +0x20
    unsigned char       DefaultValsApplied[0x1];             // +0x24 bitfield group: uchar:1
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDefClassBase) == 0x25);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, PDefManager) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, GlobalIndex) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, DefaultValsApplied) == 0x24);
#endif
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE

#endif // FABLE_ENGINE_CDEFCLASSBASE_H

// End shared header engine/CDefClassBase.h
class CDataInputStream;
class CDataOutputStream;
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {
public:
    CParentDefClassBase();                                               // 0x00430370 (??0CParentDefClassBase@@QAE@XZ)
    CParentDefClassBase(const CParentDefClassBase&);                     // 0x0046EE98 (??0CParentDefClassBase@@QAE@ABV0@@Z)
    CParentDefClassBase& operator=(const CParentDefClassBase&);          // 0x00431ED0 (??4CParentDefClassBase@@QAEAAV0@ABV0@@Z)
    virtual ~CParentDefClassBase();                                      // slot 0  0x004303F0 (scalar deleting); ~ 0x00430300
    virtual CDefString GetInstantiationName() const;                     // slot 10 0x004303B0
    virtual void SetInstantiationName(CDefString);                       // slot 11 0x004303C0
    virtual void AddSubDef(const CCharString&, long, long);              // slot 12 0x009B00D0
    virtual void RemoveSubDef(const CCharString&);                       // slot 13 0x009B0170
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             // slot 15 0x009AD510 (declared first: VC7.1 reverses overloads)
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; // slot 14 0x009AD4A0
    virtual bool IsParentDef() const;                                    // slot 16 0x004303D0 (`mov al,1; ret`)
    virtual void InternalSaveBinary(CDataOutputStream&);                 // slot 21 0x009B0200
    virtual void InternalLoadBinary(CDataInputStream&);                  // slot 22 0x009AED70
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             // slot 24 0x004303E0
#ifdef FABLE_PDCB_TYPED_MEMBERS
    CDefString InstantiationName;                                        // +0x25
    CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> > SubDefInfoMap;   // +0x29
#else
    long          InstantiationName_TablePos;                            // +0x25 CDefString InstantiationName (TablePos)
    unsigned char SubDefInfoMap[0x10];                                   // +0x29 CVectorMap<unsigned long,CSubDefInfo,CKeyPairCompareLess<..> >
#endif
};
#pragma pack(pop)
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CParentDefClassBase) == 0x39);
#endif
#endif
#else // flattened layout (the landed default)
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

#pragma pack(push, 1)
class CParentDefClassBase {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _base_0x04[0x21];                          // +0x04 flattened base-class subobject(s)
    long          InstantiationName_TablePos;                // +0x25 CDefString::TablePos
    unsigned char SubDefInfoMap[0x10];                       // +0x29 CVectorMap<unsigned_long,CDefClassBase::CSubDefInfo,CKeyPairCompareLess<unsigned_long,CDefClassBase::CSubDefInfo>_>
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CParentDefClassBase) == 0x39);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CParentDefClassBase, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CParentDefClassBase, InstantiationName_TablePos) == 0x25);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CParentDefClassBase, SubDefInfoMap) == 0x29);
#endif
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE

#endif // FABLE_ENGINE_CPARENTDEFCLASSBASE_H

// End shared header engine/CParentDefClassBase.h
#define CTCVillage CTCVillageFields
// Begin shared header engine/CTCVillage.h
#ifndef FABLE_ENGINE_CTCVILLAGE_H
#define FABLE_ENGINE_CTCVILLAGE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCVillage = 0x174.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CTCVillage = 0xe8.
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CDefPointeeBase;

#pragma pack(push, 1)
class CTCVillage {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    long             TorchTasksUpdateFrame;                  // +0x0c
    long             FrameStartedSecuritySweep;              // +0x10
    float            PositionPlayerWasInWhenSecuritySweepCalculated_X; // +0x14 C3DVector::X
    float            PositionPlayerWasInWhenSecuritySweepCalculated_Y; // +0x18 C3DVector::Y
    float            PositionPlayerWasInWhenSecuritySweepCalculated_Z; // +0x1c C3DVector::Z
    long             LastAweCrowdEventFrame;                 // +0x20
    long             LastLynchCrowdEventFrame;               // +0x24
    unsigned char    VillageBuildings[0x4];                  // +0x28 list<CIntelligentPointer<CThingBuilding>,std::allocator<CIntelligentPointer<CThingBuilding>_>_>
    unsigned char    VillagePeople[0x4];                     // +0x2c list<CIntelligentPointer<CThingAICreature>,std::allocator<CIntelligentPointer<CThingAICreature>_>_>
    unsigned char    VillageTorches[0x4];                    // +0x30 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillageHorns[0x4];                      // +0x34 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillageGates[0x4];                      // +0x38 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillageCrops[0x4];                      // +0x3c list<CIntelligentPointer<CThingObject>,std::allocator<CIntelligentPointer<CThingObject>_>_>
    unsigned char    VillageCages[0x4];                      // +0x40 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillageCampfires[0x4];                  // +0x44 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillagePatrols[0x4];                    // +0x48 list<CIntelligentPointer<CThingTrackNode>,std::allocator<CIntelligentPointer<CThingTrackNode>_>_>
    unsigned char    TorchTasks[0x4];                        // +0x4c list<CCountedPointer<CVillageTaskTorch>,std::allocator<CCountedPointer<CVillageTaskTorch>_>_>
    unsigned char    HornTasks[0x4];                         // +0x50 list<CCountedPointer<CVillageTaskHorn>,std::allocator<CCountedPointer<CVillageTaskHorn>_>_>
    unsigned char    PatrolTasks[0x4];                       // +0x54 list<CCountedPointer<CVillageTaskPatrol>,std::allocator<CCountedPointer<CVillageTaskPatrol>_>_>
    unsigned char    CropTasks[0x4];                         // +0x58 list<CCountedPointer<CVillageTaskPickCrops>,std::allocator<CCountedPointer<CVillageTaskPickCrops>_>_>
    unsigned char    SecuritySweepTasks[0x4];                // +0x5c list<CCountedPointer<CVillageTaskSecuritySweep>,std::allocator<CCountedPointer<CVillageTaskSecuritySweep>_>_>
    unsigned char    CrateTasks[0x4];                        // +0x60 list<CCountedPointer<CVillageTaskCarryCrate>,std::allocator<CCountedPointer<CVillageTaskCarryCrate>_>_>
    unsigned char    PlayersCrimes[0xc];                     // +0x64 set<long,std::less<long>,std::allocator<long>_>
    unsigned char    VillageMarkers[0x4];                    // +0x70 list<CIntelligentPointer<CThingMarker>,std::allocator<CIntelligentPointer<CThingMarker>_>_>
    unsigned char    VillageDummies[0x4];                    // +0x74 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    bool             Initialised;                            // +0x78
    bool             InitiallyPopulated;                     // +0x79
    bool             HasBeenInitiallyPopulated;              // +0x7a
    bool             HornTasksAdded;                         // +0x7b
    bool             IsEnemyBecauseOfCrime;                  // +0x7c
    unsigned char    _pad_0x7d[0x3];                         // +0x7d
    long             FramePlayerLastSeenByGuard;             // +0x80
    bool             InCurfew;                               // +0x84
    unsigned char    _pad_0x85[0x3];                         // +0x85
    long             NumGuards;                              // +0x88
    long             HouseCurrentlyChecking;                 // +0x8c
    bool             TimeIsPaused;                           // +0x90
    unsigned char    _pad_0x91[0x3];                         // +0x91
    CDefPointeeBase* PDef_Object;                            // +0x94 CDefPointer<CVillageDef_const_>::Object
    unsigned char    StanceToHero[0x4];                      // +0x98 EVillageStanceToHero
    long             TimeStopIndex;                          // +0x9c
    long             AmountOfFineAlreadyPaid;                // +0xa0
    long             NextGuardRespawn;                       // +0xa4
    long             FrameBribeEnds;                         // +0xa8
    long             FrameForNextGuardGreeting;              // +0xac
    bool             WorkTimeAccordingToCrier;               // +0xb0
    bool             CurfewTimeAccordingToCrier;             // +0xb1
    bool             Limbo;                                  // +0xb2
    bool             LaggyLimbo;                             // +0xb3
    bool             SecuritySweepUrgent;                    // +0xb4
    unsigned char    _pad_0xb5[0x3];                         // +0xb5
    float            InitialSecuritySweepRadius;             // +0xb8
    bool             TeacherTeaching;                        // +0xbc
    unsigned char    GuardFollowingPlayer[0xb];              // +0xbd CIntelligentPointer<CThingAICreature>
    long             NextFrameToFollowPlayer;                // +0xc8
    bool             LightActiveStateToChange;               // +0xcc
    unsigned char    _pad_0xcd[0x3];                         // +0xcd
    long             MaxFrameToAssignTask;                   // +0xd0
    long             HeroEnemyOfGuardsUntil;                 // +0xd4
    long             LastFrameApologised;                    // +0xd8
    unsigned char    GuardWarningHero[0x8];                  // +0xdc CIntelligentPointer<CThingAICreature>
    bool             CurrentIsHeroCriminal;                  // +0xe4
    unsigned char    _pad_0xe5[0x3];                         // +0xe5
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CTCVillage) == 0xe8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TorchTasksUpdateFrame) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, FrameStartedSecuritySweep) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PositionPlayerWasInWhenSecuritySweepCalculated_X) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PositionPlayerWasInWhenSecuritySweepCalculated_Y) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PositionPlayerWasInWhenSecuritySweepCalculated_Z) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LastAweCrowdEventFrame) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LastLynchCrowdEventFrame) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageBuildings) == 0x28);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillagePeople) == 0x2c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageTorches) == 0x30);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageHorns) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageGates) == 0x38);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageCrops) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageCages) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageCampfires) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillagePatrols) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TorchTasks) == 0x4c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HornTasks) == 0x50);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PatrolTasks) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, CropTasks) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, SecuritySweepTasks) == 0x5c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, CrateTasks) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PlayersCrimes) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageMarkers) == 0x70);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageDummies) == 0x74);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, Initialised) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, InitiallyPopulated) == 0x79);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HasBeenInitiallyPopulated) == 0x7a);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HornTasksAdded) == 0x7b);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, IsEnemyBecauseOfCrime) == 0x7c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, FramePlayerLastSeenByGuard) == 0x80);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, InCurfew) == 0x84);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, NumGuards) == 0x88);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HouseCurrentlyChecking) == 0x8c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TimeIsPaused) == 0x90);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PDef_Object) == 0x94);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, StanceToHero) == 0x98);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TimeStopIndex) == 0x9c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, AmountOfFineAlreadyPaid) == 0xa0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, NextGuardRespawn) == 0xa4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, FrameBribeEnds) == 0xa8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, FrameForNextGuardGreeting) == 0xac);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, WorkTimeAccordingToCrier) == 0xb0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, CurfewTimeAccordingToCrier) == 0xb1);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, Limbo) == 0xb2);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LaggyLimbo) == 0xb3);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, SecuritySweepUrgent) == 0xb4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, InitialSecuritySweepRadius) == 0xb8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TeacherTeaching) == 0xbc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, GuardFollowingPlayer) == 0xbd);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, NextFrameToFollowPlayer) == 0xc8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LightActiveStateToChange) == 0xcc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, MaxFrameToAssignTask) == 0xd0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HeroEnemyOfGuardsUntil) == 0xd4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LastFrameApologised) == 0xd8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, GuardWarningHero) == 0xdc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCVillage, CurrentIsHeroCriminal) == 0xe4);
#endif
#endif

#endif // FABLE_ENGINE_CTCVILLAGE_H

// End shared header engine/CTCVillage.h
#undef CTCVillage
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

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
#define CPlayer CPlayerFields
// Begin shared header engine/CPlayer.h
#ifndef FABLE_ENGINE_CPLAYER_H
#define FABLE_ENGINE_CPLAYER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CPlayer = 0x234.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CPlayer = 0x22c.
// Retail evidence: 2 trusted landed function(s); reconciliation clean.
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CCPPointerInfo;
class CDefPointeeBase;
class CGameDefinitionManager;
class CGamePlayerInterface;
class CHeroLogBook;
class CInputProcessBButtonExitMode;
class CInputProcessBetting;
class CInputProcessBlock;
class CInputProcessBoastUI;
class CInputProcessCameraLookAround;
class CInputProcessClickPastText;
class CInputProcessCombat;
class CInputProcessConsole;
class CInputProcessControlCreature;
class CInputProcessControlCreatureActivateZTarget;
class CInputProcessControlCreatureActivateZTargetOnPress;
class CInputProcessControlCreatureRightStick;
class CInputProcessControlFreeCamera;
class CInputProcessControlSpirit;
class CInputProcessCreatureMovement;
class CInputProcessCreatureMovementWatchForControlAngleChange;
class CInputProcessCreditsUI;
class CInputProcessCutScene;
class CInputProcessCycleSpecialCameraModes;
class CInputProcessDead;
class CInputProcessDebugControls;
class CInputProcessDigging;
class CInputProcessFireheartMinigame;
class CInputProcessFirstPerson;
class CInputProcessFirstPersonLookAround;
class CInputProcessFirstPersonTargeting;
class CInputProcessFishing;
class CInputProcessFreezeControls;
class CInputProcessHeroAbilitiesScreen;
class CInputProcessHeroInformationScreens;
class CInputProcessInGameMenu;
class CInputProcessInventory;
class CInputProcessInventoryClothing;
class CInputProcessInventoryExperienceScreen;
class CInputProcessInventoryMagicScreen;
class CInputProcessInventoryMapScreen;
class CInputProcessInventoryQuestsScreen;
class CInputProcessInventoryStatsScreen;
class CInputProcessInventoryTradeScreen;
class CInputProcessInventoryWeapons;
class CInputProcessJumpingAndRolling;
class CInputProcessLightning;
class CInputProcessMain;
class CInputProcessOracleMinigame;
class CInputProcessParalysed;
class CInputProcessPhotojournalCapture;
class CInputProcessProjectileTargetingAnalogueZoom;
class CInputProcessQuestCompletionUI;
class CInputProcessQuickAccessItems;
class CInputProcessQuickAccessMenu;
class CInputProcessRebootGame;
class CInputProcessSetRangedWeaponMode;
class CInputProcessSetRangedWeaponThirdPersonMode;
class CInputProcessSpecialAbilities;
class CInputProcessStrafe;
class CInputProcessTargetLockCycleTargets;
class CInputProcessTargetLockRightStickTargetSelect;
class CInputProcessTavernGame;
class CInputProcessToggleViewHeroMode;
class CInputProcessUseEnvironment;
class CInputProcessUseRangedWeapon;
class CInputProcessUseRangedWeaponZLock;
class CInputProcessWatchForRangedWeaponThirdPersonModeTermination;
class CInputProcessWatchForWillChargeUpThirdPersonModeTermination;
class CInputProcessYesNoQuestion;
class CInputProcessZTarget;
class CMainGameComponent;
class CPlayerManager;
class CThingManager;
class CWorld;
class CWorldMap;

#pragma pack(push, 1)
class CPlayer {
public:
    void*                                                        __vftable; // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char                                                _pad_0x04[0x4]; // +0x04
    bool                                                         DrawingFreeCamDebug; // +0x08
    bool                                                         PlayerIsCharacter; // +0x09
    unsigned char                                                _pad_0x0a[0x2]; // +0x0a
    CGamePlayerInterface*                                        PlayerInterface; // +0x0c
    CPlayerManager*                                              PlayerManager; // +0x10
    CMainGameComponent*                                          Component; // +0x14
    CGameDefinitionManager*                                      DefinitionManager; // +0x18
    CWorld*                                                      World; // +0x1c
    CThingManager*                                               ThingManager; // +0x20
    CWorldMap*                                                   PWorldMap; // +0x24
    long                                                         Number; // +0x28
    unsigned char                                                ControlledCreature[0x8]; // +0x2c CIntelligentPointer<CThingPlayerCreature>
    unsigned char                                                PlayerCharacter[0x8]; // +0x34 CIntelligentPointer<CThingPlayerCreature>
    long                                                         PlayerRespawnDelay; // +0x3c
    unsigned char                                                CurrentFreeCamera[0x38]; // +0x40 CCamera
    unsigned char                                                FreeCamera[0x38]; // +0x78 CCamera
    unsigned char                                                OldFreeCamera[0x38]; // +0xb0 CCamera
    unsigned char                                                RespawnInfo[0x10]; // +0xe8 CThingRespawnInfo
    CHeroLogBook*                                                PHeroLogBook_Data; // +0xf8 CCountedPointer<CHeroLogBook>::Data
    CCPPointerInfo*                                              PHeroLogBook_Info; // +0xfc CCountedPointer<CHeroLogBook>::Info
    CInputProcessMain*                                           InputProcessMain_ptr; // +0x100 scoped_ptr<CInputProcessMain>::ptr
    CInputProcessControlCreature*                                InputProcessControlCreature_ptr; // +0x104 scoped_ptr<CInputProcessControlCreature>::ptr
    CInputProcessZTarget*                                        InputProcessZTarget_ptr; // +0x108 scoped_ptr<CInputProcessZTarget>::ptr
    CInputProcessDead*                                           InputProcessDead_ptr; // +0x10c scoped_ptr<CInputProcessDead>::ptr
    CInputProcessInventory*                                      InputProcessInventory_ptr; // +0x110 scoped_ptr<CInputProcessInventory>::ptr
    CInputProcessInventoryClothing*                              InputProcessInventoryClothing_ptr; // +0x114 scoped_ptr<CInputProcessInventoryClothing>::ptr
    CInputProcessInventoryWeapons*                               InputProcessInventoryWeapons_ptr; // +0x118 scoped_ptr<CInputProcessInventoryWeapons>::ptr
    CInputProcessHeroAbilitiesScreen*                            InputProcessHeroAbilitiesScreen_ptr; // +0x11c scoped_ptr<CInputProcessHeroAbilitiesScreen>::ptr
    CInputProcessInventoryMapScreen*                             InputProcessInventoryMapScreen_ptr; // +0x120 scoped_ptr<CInputProcessInventoryMapScreen>::ptr
    CInputProcessInventoryStatsScreen*                           InputProcessInventoryStatsScreen_ptr; // +0x124 scoped_ptr<CInputProcessInventoryStatsScreen>::ptr
    CInputProcessInventoryMagicScreen*                           InputProcessInventoryMagicScreen_ptr; // +0x128 scoped_ptr<CInputProcessInventoryMagicScreen>::ptr
    CInputProcessInventoryExperienceScreen*                      InputProcessInventoryExperienceScreen_ptr; // +0x12c scoped_ptr<CInputProcessInventoryExperienceScreen>::ptr
    CInputProcessInventoryTradeScreen*                           InputProcessInventoryTradeScreen_ptr; // +0x130 scoped_ptr<CInputProcessInventoryTradeScreen>::ptr
    CInputProcessInventoryQuestsScreen*                          InputProcessInventoryQuestsScreen_ptr; // +0x134 scoped_ptr<CInputProcessInventoryQuestsScreen>::ptr
    CInputProcessHeroInformationScreens*                         InputProcessHeroInformationScreens_ptr; // +0x138 scoped_ptr<CInputProcessHeroInformationScreens>::ptr
    CInputProcessFirstPerson*                                    InputProcessFirstPerson_ptr; // +0x13c scoped_ptr<CInputProcessFirstPerson>::ptr
    CInputProcessClickPastText*                                  InputProcessClickPastText_ptr; // +0x140 scoped_ptr<CInputProcessClickPastText>::ptr
    CInputProcessYesNoQuestion*                                  InputProcessYesNoQuestion_ptr; // +0x144 scoped_ptr<CInputProcessYesNoQuestion>::ptr
    CInputProcessFreezeControls*                                 InputProcessFreezeControls_ptr; // +0x148 scoped_ptr<CInputProcessFreezeControls>::ptr
    CInputProcessControlFreeCamera*                              InputProcessControlFreeCamera_ptr; // +0x14c scoped_ptr<CInputProcessControlFreeCamera>::ptr
    CInputProcessCreatureMovement*                               InputProcessCreatureMovement_ptr; // +0x150 scoped_ptr<CInputProcessCreatureMovement>::ptr
    CInputProcessUseEnvironment*                                 InputProcessUseEnvironment_ptr; // +0x154 scoped_ptr<CInputProcessUseEnvironment>::ptr
    CInputProcessBlock*                                          InputProcessBlock_ptr; // +0x158 scoped_ptr<CInputProcessBlock>::ptr
    CInputProcessDebugControls*                                  InputProcessDebugControls_ptr; // +0x15c scoped_ptr<CInputProcessDebugControls>::ptr
    CInputProcessQuickAccessItems*                               InputProcessQuickAccessItems_ptr; // +0x160 scoped_ptr<CInputProcessQuickAccessItems>::ptr
    CInputProcessCombat*                                         InputProcessCombat_ptr; // +0x164 scoped_ptr<CInputProcessCombat>::ptr
    CInputProcessSpecialAbilities*                               InputProcessSpecialAbilities_ptr; // +0x168 scoped_ptr<CInputProcessSpecialAbilities>::ptr
    CInputProcessControlCreatureRightStick*                      InputProcessControlCreatureRightStick_ptr; // +0x16c scoped_ptr<CInputProcessControlCreatureRightStick>::ptr
    CInputProcessCameraLookAround*                               InputProcessRightStickLookAround_ptr; // +0x170 scoped_ptr<CInputProcessCameraLookAround>::ptr
    CInputProcessCycleSpecialCameraModes*                        InputProcessCycleSpecialCameraModes_ptr; // +0x174 scoped_ptr<CInputProcessCycleSpecialCameraModes>::ptr
    CInputProcessRebootGame*                                     InputProcessRebootGame_ptr; // +0x178 scoped_ptr<CInputProcessRebootGame>::ptr
    CInputProcessJumpingAndRolling*                              InputProcessJumpingAndRolling_ptr; // +0x17c scoped_ptr<CInputProcessJumpingAndRolling>::ptr
    CInputProcessFirstPersonTargeting*                           InputProcessFirstPersonTargeting_ptr; // +0x180 scoped_ptr<CInputProcessFirstPersonTargeting>::ptr
    CInputProcessUseRangedWeapon*                                InputProcessUseRangedWeapon_ptr; // +0x184 scoped_ptr<CInputProcessUseRangedWeapon>::ptr
    CInputProcessControlCreatureActivateZTarget*                 InputProcessControlCreatureActivateZTarget_ptr; // +0x188 scoped_ptr<CInputProcessControlCreatureActivateZTarget>::ptr
    CInputProcessControlCreatureActivateZTargetOnPress*          InputProcessControlCreatureActivateZTargetOnPress_ptr; // +0x18c scoped_ptr<CInputProcessControlCreatureActivateZTargetOnPress>::ptr
    CInputProcessFirstPersonLookAround*                          InputProcessLeftStickLookAround_ptr; // +0x190 scoped_ptr<CInputProcessFirstPersonLookAround>::ptr
    CInputProcessInGameMenu*                                     InputProcessInGameMenu_ptr; // +0x194 scoped_ptr<CInputProcessInGameMenu>::ptr
    CInputProcessControlSpirit*                                  InputProcessControlSpirit_ptr; // +0x198 scoped_ptr<CInputProcessControlSpirit>::ptr
    CInputProcessTavernGame*                                     InputProcessTavernGame_ptr; // +0x19c scoped_ptr<CInputProcessTavernGame>::ptr
    CInputProcessSetRangedWeaponMode*                            InputProcessSetRangedWeaponMode_ptr; // +0x1a0 scoped_ptr<CInputProcessSetRangedWeaponMode>::ptr
    CInputProcessCutScene*                                       InputProcessCutScene_ptr; // +0x1a4 scoped_ptr<CInputProcessCutScene>::ptr
    CInputProcessFishing*                                        InputProcessFishing_ptr; // +0x1a8 scoped_ptr<CInputProcessFishing>::ptr
    CInputProcessDigging*                                        InputProcessDigging_ptr; // +0x1ac scoped_ptr<CInputProcessDigging>::ptr
    CInputProcessParalysed*                                      InputProcessParalysed_ptr; // +0x1b0 scoped_ptr<CInputProcessParalysed>::ptr
    CInputProcessBoastUI*                                        InputProcessBoastUI_ptr; // +0x1b4 scoped_ptr<CInputProcessBoastUI>::ptr
    CInputProcessSetRangedWeaponThirdPersonMode*                 InputProcessSetRangedWeaponThirdPerson_ptr; // +0x1b8 scoped_ptr<CInputProcessSetRangedWeaponThirdPersonMode>::ptr
    CInputProcessUseRangedWeaponZLock*                           InputProcessUseRangedWeaponZLock_ptr; // +0x1bc scoped_ptr<CInputProcessUseRangedWeaponZLock>::ptr
    CInputProcessWatchForRangedWeaponThirdPersonModeTermination* InputProcessWatchForRangedWeaponThirdPersonModeTermination_ptr; // +0x1c0 scoped_ptr<CInputProcessWatchForRangedWeaponThirdPersonModeTermination>::ptr
    CInputProcessWatchForWillChargeUpThirdPersonModeTermination* InputProcessWatchForWillChargeUpThirdPersonModeTermination_ptr; // +0x1c4 scoped_ptr<CInputProcessWatchForWillChargeUpThirdPersonModeTermination>::ptr
    CInputProcessTargetLockCycleTargets*                         InputProcessTargetLockCycleTargets_ptr; // +0x1c8 scoped_ptr<CInputProcessTargetLockCycleTargets>::ptr
    CInputProcessQuickAccessMenu*                                InputProcessQuickAccessMenu_ptr; // +0x1cc scoped_ptr<CInputProcessQuickAccessMenu>::ptr
    CInputProcessQuestCompletionUI*                              InputProcessQuestCompletionUI_ptr; // +0x1d0 scoped_ptr<CInputProcessQuestCompletionUI>::ptr
    CInputProcessTargetLockRightStickTargetSelect*               InputProcessTargetLockRightStickTargetSelect_ptr; // +0x1d4 scoped_ptr<CInputProcessTargetLockRightStickTargetSelect>::ptr
    CInputProcessBButtonExitMode*                                InputProcessBButtonExitMode_ptr; // +0x1d8 scoped_ptr<CInputProcessBButtonExitMode>::ptr
    CInputProcessCreditsUI*                                      InputProcessCreditsUI_ptr; // +0x1dc scoped_ptr<CInputProcessCreditsUI>::ptr
    CInputProcessBetting*                                        InputProcessBetting_ptr; // +0x1e0 scoped_ptr<CInputProcessBetting>::ptr
    CInputProcessLightning*                                      InputProcessLightning_ptr; // +0x1e4 scoped_ptr<CInputProcessLightning>::ptr
    CInputProcessCreatureMovementWatchForControlAngleChange*     InputProcessWatchForControlAngleChange_ptr; // +0x1e8 scoped_ptr<CInputProcessCreatureMovementWatchForControlAngleChange>::ptr
    CInputProcessOracleMinigame*                                 InputProcessOracleMinigame_ptr; // +0x1ec scoped_ptr<CInputProcessOracleMinigame>::ptr
    CInputProcessFireheartMinigame*                              InputProcessFireheartMinigame_ptr; // +0x1f0 scoped_ptr<CInputProcessFireheartMinigame>::ptr
    CInputProcessStrafe*                                         InputProcessStrafe_ptr; // +0x1f4 scoped_ptr<CInputProcessStrafe>::ptr
    CInputProcessConsole*                                        InputProcessConsole_ptr; // +0x1f8 scoped_ptr<CInputProcessConsole>::ptr
    CInputProcessProjectileTargetingAnalogueZoom*                InputProcessProjectileTargetingAnalogueZoom_ptr; // +0x1fc scoped_ptr<CInputProcessProjectileTargetingAnalogueZoom>::ptr
    CInputProcessPhotojournalCapture*                            InputProcessPhotojournalCapture_ptr; // +0x200 scoped_ptr<CInputProcessPhotojournalCapture>::ptr
    CInputProcessToggleViewHeroMode*                             InputProcessToggleViewHeroMode_ptr; // +0x204 scoped_ptr<CInputProcessToggleViewHeroMode>::ptr
    bool                                                         Local; // +0x208
    bool                                                         ShowWorldThing; // +0x209
    bool                                                         ZTargeting; // +0x20a
    bool                                                         ProjectileTargetLocked; // +0x20b
    bool                                                         RightTriggerTargetLocked; // +0x20c
    unsigned char                                                _pad_0x20d[0x1]; // +0x20d
    unsigned char                                                _pad_0x20e[0x2]; // +0x20e
    unsigned char                                                PlayerModes[0x4]; // +0x210 list<EPlayerMode,std::allocator<EPlayerMode>_>
    bool                                                         SuppressFullModeRemoval; // +0x214
    bool                                                         DisallowModeChanges; // +0x215
    bool                                                         InitialPlayerModeSet; // +0x216
    bool                                                         AggressiveMode; // +0x217
    bool                                                         SpellMode; // +0x218
    bool                                                         ExpressionShiftMode; // +0x219
    bool                                                         PCProjectileWeaponThirdPersonAimingMode; // +0x21a
    bool                                                         KillEverythingMode; // +0x21b
    CDefPointeeBase*                                             PlayerDef_Object; // +0x21c CDefPointer<CPlayerDef_const_>::Object
    bool                                                         UsingFreeCamera; // +0x220
    bool                                                         FreeCameraTrackingPlayer; // +0x221
    bool                                                         FreeCameraTrackingPlayerYZ; // +0x222
    bool                                                         ControllingFreeCamera; // +0x223
    bool                                                         KeepAbilitiesDuringCutscenes; // +0x224
    unsigned char                                                _pad_0x225[0x3]; // +0x225
    long                                                         JoystickDeviceNumber; // +0x228
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CPlayer) == 0x22c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, DrawingFreeCamDebug) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, PlayerIsCharacter) == 0x9);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, PlayerInterface) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, PlayerManager) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, Component) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, DefinitionManager) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, World) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, ThingManager) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, PWorldMap) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, Number) == 0x28);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, ControlledCreature) == 0x2c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, PlayerCharacter) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, PlayerRespawnDelay) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, CurrentFreeCamera) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, FreeCamera) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, OldFreeCamera) == 0xb0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, RespawnInfo) == 0xe8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, PHeroLogBook_Data) == 0xf8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, PHeroLogBook_Info) == 0xfc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessMain_ptr) == 0x100);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessControlCreature_ptr) == 0x104);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessZTarget_ptr) == 0x108);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessDead_ptr) == 0x10c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessInventory_ptr) == 0x110);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessInventoryClothing_ptr) == 0x114);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessInventoryWeapons_ptr) == 0x118);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessHeroAbilitiesScreen_ptr) == 0x11c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessInventoryMapScreen_ptr) == 0x120);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessInventoryStatsScreen_ptr) == 0x124);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessInventoryMagicScreen_ptr) == 0x128);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessInventoryExperienceScreen_ptr) == 0x12c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessInventoryTradeScreen_ptr) == 0x130);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessInventoryQuestsScreen_ptr) == 0x134);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessHeroInformationScreens_ptr) == 0x138);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessFirstPerson_ptr) == 0x13c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessClickPastText_ptr) == 0x140);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessYesNoQuestion_ptr) == 0x144);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessFreezeControls_ptr) == 0x148);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessControlFreeCamera_ptr) == 0x14c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessCreatureMovement_ptr) == 0x150);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessUseEnvironment_ptr) == 0x154);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessBlock_ptr) == 0x158);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessDebugControls_ptr) == 0x15c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessQuickAccessItems_ptr) == 0x160);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessCombat_ptr) == 0x164);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessSpecialAbilities_ptr) == 0x168);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessControlCreatureRightStick_ptr) == 0x16c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessRightStickLookAround_ptr) == 0x170);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessCycleSpecialCameraModes_ptr) == 0x174);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessRebootGame_ptr) == 0x178);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessJumpingAndRolling_ptr) == 0x17c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessFirstPersonTargeting_ptr) == 0x180);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessUseRangedWeapon_ptr) == 0x184);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessControlCreatureActivateZTarget_ptr) == 0x188);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessControlCreatureActivateZTargetOnPress_ptr) == 0x18c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessLeftStickLookAround_ptr) == 0x190);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessInGameMenu_ptr) == 0x194);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessControlSpirit_ptr) == 0x198);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessTavernGame_ptr) == 0x19c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessSetRangedWeaponMode_ptr) == 0x1a0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessCutScene_ptr) == 0x1a4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessFishing_ptr) == 0x1a8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessDigging_ptr) == 0x1ac);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessParalysed_ptr) == 0x1b0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessBoastUI_ptr) == 0x1b4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessSetRangedWeaponThirdPerson_ptr) == 0x1b8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessUseRangedWeaponZLock_ptr) == 0x1bc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessWatchForRangedWeaponThirdPersonModeTermination_ptr) == 0x1c0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessWatchForWillChargeUpThirdPersonModeTermination_ptr) == 0x1c4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessTargetLockCycleTargets_ptr) == 0x1c8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessQuickAccessMenu_ptr) == 0x1cc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessQuestCompletionUI_ptr) == 0x1d0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessTargetLockRightStickTargetSelect_ptr) == 0x1d4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessBButtonExitMode_ptr) == 0x1d8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessCreditsUI_ptr) == 0x1dc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessBetting_ptr) == 0x1e0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessLightning_ptr) == 0x1e4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessWatchForControlAngleChange_ptr) == 0x1e8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessOracleMinigame_ptr) == 0x1ec);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessFireheartMinigame_ptr) == 0x1f0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessStrafe_ptr) == 0x1f4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessConsole_ptr) == 0x1f8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessProjectileTargetingAnalogueZoom_ptr) == 0x1fc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessPhotojournalCapture_ptr) == 0x200);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InputProcessToggleViewHeroMode_ptr) == 0x204);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, Local) == 0x208);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, ShowWorldThing) == 0x209);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, ZTargeting) == 0x20a);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, ProjectileTargetLocked) == 0x20b);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, RightTriggerTargetLocked) == 0x20c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, PlayerModes) == 0x210);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, SuppressFullModeRemoval) == 0x214);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, DisallowModeChanges) == 0x215);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, InitialPlayerModeSet) == 0x216);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, AggressiveMode) == 0x217);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, SpellMode) == 0x218);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, ExpressionShiftMode) == 0x219);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, PCProjectileWeaponThirdPersonAimingMode) == 0x21a);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, KillEverythingMode) == 0x21b);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, PlayerDef_Object) == 0x21c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, UsingFreeCamera) == 0x220);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, FreeCameraTrackingPlayer) == 0x221);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, FreeCameraTrackingPlayerYZ) == 0x222);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, ControllingFreeCamera) == 0x223);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, KeepAbilitiesDuringCutscenes) == 0x224);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CPlayer, JoystickDeviceNumber) == 0x228);
#endif
#endif

#endif // FABLE_ENGINE_CPLAYER_H

// End shared header engine/CPlayer.h
#undef CPlayer
#define CWorld CWorldFields
// Begin shared header engine/CWorld.h
#ifndef FABLE_ENGINE_CWORLD_H
#define FABLE_ENGINE_CWORLD_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CWorld = 0x1b8.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CWorld = 0x198.
// Retail evidence: 3 trusted landed function(s); reconciliation clean.
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class C3DAnimationManager;
class CAIGameCameraBase;
class CAtmosProcess;
class CBulletTimeManager;
class CCPPointerInfo;
class CCharStringData;
class CCombatManager;
class CEnvironment;
class CFactionManager;
class CGameDefinitionManager;
class CGameScriptInterface;
class CGameTimeManager;
class CMainGameComponent;
class CMeshDataBank;
class CMessageEventManager;
class CMusicManager;
class CNavigatorManager;
class COpinionReactionManager;
class CPlayerManager;
class CScriptConversationManager;
class CScriptInfoManager;
class CThingManager;
class CThingSearchTools;
class CWideStringData;
class CWorldMap;

#pragma pack(push, 1)
class CWorld {
public:
    void*                       __vftable;                   // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char               _base_0x04[0x4];             // +0x04 flattened base-class subobject(s)
    CMainGameComponent*         Component;                   // +0x08
    CPlayerManager*             PlayerManager;               // +0x0c
    CGameDefinitionManager*     DefinitionManager;           // +0x10
    CWorldMap*                  PWorldMap_ptr;               // +0x14 scoped_ptr<CWorldMap>::ptr
    CEnvironment*               PEnvironment_ptr;            // +0x18 scoped_ptr<CEnvironment>::ptr
    CGameTimeManager*           PGameTimeManager_ptr;        // +0x1c scoped_ptr<CGameTimeManager>::ptr
    CThingSearchTools*          PThingSearchTools_ptr;       // +0x20 scoped_ptr<CThingSearchTools>::ptr
    CAtmosProcess*              PAtmosProcessor_Data;        // +0x24 CCountedPointer<CAtmosProcess>::Data
    CCPPointerInfo*             PAtmosProcessor_Info;        // +0x28 CCountedPointer<CAtmosProcess>::Info
    CAIGameCameraBase*          PGameCamera_ptr;             // +0x2c scoped_ptr<CAIGameCameraBase>::ptr
    CAIGameCameraBase*          PGameCameraManager_ptr;      // +0x30 scoped_ptr<CAIGameCameraBase>::ptr
    CAIGameCameraBase*          PCurrentGameCamera;          // +0x34
    CGameScriptInterface*       PGameScriptInterface_ptr;    // +0x38 scoped_ptr<CGameScriptInterface>::ptr
    CMeshDataBank*              PMainMeshBank_Data;          // +0x3c CCountedPointer<CMeshDataBank>::Data
    CCPPointerInfo*             PMainMeshBank_Info;          // +0x40 CCountedPointer<CMeshDataBank>::Info
    C3DAnimationManager*        PAnimationManager;           // +0x44
    CNavigatorManager*          PNavigationManager_ptr;      // +0x48 scoped_ptr<CNavigatorManager>::ptr
    CCombatManager*             PThingCombatManager_ptr;     // +0x4c scoped_ptr<CCombatManager>::ptr
    CThingManager*              PThingManager_ptr;           // +0x50 scoped_ptr<CThingManager>::ptr
    CFactionManager*            PFactionManager_ptr;         // +0x54 scoped_ptr<CFactionManager>::ptr
    CScriptInfoManager*         PScriptInfoManager_Data;     // +0x58 CCountedPointer<CScriptInfoManager>::Data
    CCPPointerInfo*             PScriptInfoManager_Info;     // +0x5c CCountedPointer<CScriptInfoManager>::Info
    CMessageEventManager*       PMessageEventManager_Data;   // +0x60 CCountedPointer<CMessageEventManager>::Data
    CCPPointerInfo*             PMessageEventManager_Info;   // +0x64 CCountedPointer<CMessageEventManager>::Info
    CBulletTimeManager*         PBulletTimeManager_ptr;      // +0x68 scoped_ptr<CBulletTimeManager>::ptr
    CMusicManager*              PMusicManager_Data;          // +0x6c CCountedPointer<CMusicManager>::Data
    CCPPointerInfo*             PMusicManager_Info;          // +0x70 CCountedPointer<CMusicManager>::Info
    COpinionReactionManager*    POpinionReactionManager_Data; // +0x74 CCountedPointer<COpinionReactionManager>::Data
    CCPPointerInfo*             POpinionReactionManager_Info; // +0x78 CCountedPointer<COpinionReactionManager>::Info
    CScriptConversationManager* PScriptConversationManager_ptr; // +0x7c scoped_ptr<CScriptConversationManager>::ptr
    bool                        JustLoaded;                  // +0x80
    unsigned char               _pad_0x81[0x3];              // +0x81
    CCharStringData*            CurrentWorldName_PStringData; // +0x84 CCharString::PStringData
    long                        ConsolePauseAtFrameNumber;   // +0x88
    long                        FrameStarted3DRendering;     // +0x8c
    double                      LastUpdateTimeLength;        // +0x90
    double                      LastUpdateTime;              // +0x98
    long                        CountdownTimer;              // +0xa0
    bool                        Paused;                      // +0xa4
    bool                        SlowMotion;                  // +0xa5
    bool                        ShowDebugText;               // +0xa6
    bool                        ShowFPSText;                 // +0xa7
    bool                        ShowProfileText;             // +0xa8
    unsigned char               _pad_0xa9[0x3];              // +0xa9
    unsigned char               InitialActiveQuests[0xc];    // +0xac CArray<CCharString>
    unsigned char               RegisteredQuests[0xc];       // +0xb8 vector<CCharString,std::allocator<CCharString>_>
    unsigned char               ActiveTestQuests[0xc];       // +0xc4 vector<CTestQuest,std::allocator<CTestQuest>_>
    unsigned long               CreatureGenerationDisabledGroups; // +0xd0
    bool                        CreatureGenerationEnabled;   // +0xd4
    bool                        TeleportingEnabled;          // +0xd5
    bool                        ExperienceSpendingEnabled;   // +0xd6
    bool                        SavingEnabled;               // +0xd7
    bool                        DontPopulateNextLoadedRegion; // +0xd8
    bool                        HeroSleepingEnabled;         // +0xd9
    bool                        MapTableShowQuestCardsOnUsed; // +0xda
    bool                        ScreenToFadeInOnNextRegionChange; // +0xdb
    bool                        DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn; // +0xdc
    bool                        MiniMapEnabled;              // +0xdd
    bool                        MiniMapActiveBeforeDisabled; // +0xde
    bool                        RegionLoadDisplayRegionName; // +0xdf
    bool                        GuildMasterMessagesEnabled;  // +0xe0
    bool                        SummonerDeathExplosionAffectsHero; // +0xe1
    bool                        WaitingForInventoryTutorialToFinish; // +0xe2
    unsigned char               _pad_0xe3[0x1];              // +0xe3
    unsigned char               HeroInformationScreenModeAfterTutorial[0x4]; // +0xe4 EPlayerMode
    long                        FrameCachedLODCentre;        // +0xe8
    float                       CachedLODCentre_X;           // +0xec C3DVector::X
    float                       CachedLODCentre_Y;           // +0xf0 C3DVector::Y
    float                       CachedLODCentre_Z;           // +0xf4 C3DVector::Z
    unsigned char               SaveGameLoadStatus[0x4];     // +0xf8 ESaveGameLoadStatus
    CWideStringData*            SaveGamePathName_PStringData; // +0xfc CWideString::PStringData
    bool                        AutoSaveLocked;              // +0x100
    bool                        SerialisingAboutToLoadHeroState; // +0x101
    bool                        SerialisingHeroState;        // +0x102
    bool                        SerialisingNonPersistentQuestItems; // +0x103
    unsigned char               RegionLoadStatus[0x4];       // +0x104 ERegionLoadStatus
    float                       RegionLoadStartPos_X;        // +0x108 C3DVector::X
    float                       RegionLoadStartPos_Y;        // +0x10c C3DVector::Y
    float                       RegionLoadStartPos_Z;        // +0x110 C3DVector::Z
    float                       RegionLoadStartAngleXY;      // +0x114
    unsigned char               RegionLoadFollowers[0xc];    // +0x118 vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    long                        PreviousRegion;              // +0x124
    long                        NumberOfTimesFreezeControlsModeAddedDuringRegionLoad; // +0x128
    bool                        RegionLoadViaTeleporter;     // +0x12c
    bool                        RegionLoadViaDoor;           // +0x12d
    bool                        PutIntoPauseModeOnRegionChange; // +0x12e
    bool                        DisplayMiniMapOnRegionLoad;  // +0x12f
    bool                        RegionLoadWaitingForConfirmation; // +0x130
    bool                        RegionLoadScreenFullyFaded;  // +0x131
    bool                        RegionLoadScreenWasFadedOut; // +0x132
    bool                        WaitingForResetToFrontEndConfirmation; // +0x133
    unsigned char               MostRecentSaveType[0x4];     // +0x134 ESaveType
    unsigned char               MostRecentSaveTypeBeforeManualSave[0x4]; // +0x138 ESaveType
    CWideStringData*            MostRecentManualSaveName_PStringData; // +0x13c CWideString::PStringData
    bool                        AutoSaveCheckPointExists;    // +0x140
    unsigned char               SaveGameMarkerPos[0xf];      // +0x141 C3DVector
    float                       SaveGameMarkerAngleXY;       // +0x150
    float                       GuildSealRecallPos_X;        // +0x154 C3DVector::X
    float                       GuildSealRecallPos_Y;        // +0x158 C3DVector::Y
    float                       GuildSealRecallPos_Z;        // +0x15c C3DVector::Z
    float                       GuildSealRecallAngleXY;      // +0x160
    bool                        WeatherMaskingPrimitivesSent; // +0x164
    unsigned char               _pad_0x165[0x3];             // +0x165
    unsigned char               WeatherMaskingPrimitiveHandles[0xc];  // +0x168 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char               AtmosBanksWaitingToCopy[0x4]; // +0x174 list<CAtmosCopyInfo,std::allocator<CAtmosCopyInfo>_>
    unsigned char               PlayerSpawnStatus[0x4];      // +0x178 EPlayerSpawnStatus
    long                        VillagerReactionDebug;       // +0x17c
    double                      StartTime;                   // +0x180
    double                      TimePlayed;                  // +0x188
    bool                        HasInitialisedStartTime;     // +0x190
    unsigned char               _pad_0x191[0x7];             // +0x191
    static long __fastcall GetFrame();                       // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CWorld) == 0x198);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, Component) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PlayerManager) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, DefinitionManager) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PWorldMap_ptr) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PEnvironment_ptr) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PGameTimeManager_ptr) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PThingSearchTools_ptr) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PAtmosProcessor_Data) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PAtmosProcessor_Info) == 0x28);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PGameCamera_ptr) == 0x2c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PGameCameraManager_ptr) == 0x30);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PCurrentGameCamera) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PGameScriptInterface_ptr) == 0x38);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PMainMeshBank_Data) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PMainMeshBank_Info) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PAnimationManager) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PNavigationManager_ptr) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PThingCombatManager_ptr) == 0x4c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PThingManager_ptr) == 0x50);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PFactionManager_ptr) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PScriptInfoManager_Data) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PScriptInfoManager_Info) == 0x5c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PMessageEventManager_Data) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PMessageEventManager_Info) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PBulletTimeManager_ptr) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PMusicManager_Data) == 0x6c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PMusicManager_Info) == 0x70);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, POpinionReactionManager_Data) == 0x74);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, POpinionReactionManager_Info) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PScriptConversationManager_ptr) == 0x7c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, JustLoaded) == 0x80);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, CurrentWorldName_PStringData) == 0x84);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, ConsolePauseAtFrameNumber) == 0x88);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, FrameStarted3DRendering) == 0x8c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, LastUpdateTimeLength) == 0x90);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, LastUpdateTime) == 0x98);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, CountdownTimer) == 0xa0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, Paused) == 0xa4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, SlowMotion) == 0xa5);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, ShowDebugText) == 0xa6);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, ShowFPSText) == 0xa7);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, ShowProfileText) == 0xa8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, InitialActiveQuests) == 0xac);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, RegisteredQuests) == 0xb8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, ActiveTestQuests) == 0xc4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, CreatureGenerationDisabledGroups) == 0xd0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, CreatureGenerationEnabled) == 0xd4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, TeleportingEnabled) == 0xd5);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, ExperienceSpendingEnabled) == 0xd6);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, SavingEnabled) == 0xd7);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, DontPopulateNextLoadedRegion) == 0xd8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, HeroSleepingEnabled) == 0xd9);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, MapTableShowQuestCardsOnUsed) == 0xda);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, ScreenToFadeInOnNextRegionChange) == 0xdb);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, DoneExtraFrameUpdateBeforeRegionLoadScreenFadeIn) == 0xdc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, MiniMapEnabled) == 0xdd);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, MiniMapActiveBeforeDisabled) == 0xde);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadDisplayRegionName) == 0xdf);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildMasterMessagesEnabled) == 0xe0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, SummonerDeathExplosionAffectsHero) == 0xe1);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, WaitingForInventoryTutorialToFinish) == 0xe2);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, HeroInformationScreenModeAfterTutorial) == 0xe4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, FrameCachedLODCentre) == 0xe8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, CachedLODCentre_X) == 0xec);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, CachedLODCentre_Y) == 0xf0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, CachedLODCentre_Z) == 0xf4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, SaveGameLoadStatus) == 0xf8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, SaveGamePathName_PStringData) == 0xfc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, AutoSaveLocked) == 0x100);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, SerialisingAboutToLoadHeroState) == 0x101);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, SerialisingHeroState) == 0x102);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, SerialisingNonPersistentQuestItems) == 0x103);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStatus) == 0x104);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStartPos_X) == 0x108);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStartPos_Y) == 0x10c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStartPos_Z) == 0x110);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadStartAngleXY) == 0x114);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadFollowers) == 0x118);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PreviousRegion) == 0x124);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, NumberOfTimesFreezeControlsModeAddedDuringRegionLoad) == 0x128);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadViaTeleporter) == 0x12c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadViaDoor) == 0x12d);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PutIntoPauseModeOnRegionChange) == 0x12e);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, DisplayMiniMapOnRegionLoad) == 0x12f);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadWaitingForConfirmation) == 0x130);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadScreenFullyFaded) == 0x131);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, RegionLoadScreenWasFadedOut) == 0x132);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, WaitingForResetToFrontEndConfirmation) == 0x133);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, MostRecentSaveType) == 0x134);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, MostRecentSaveTypeBeforeManualSave) == 0x138);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, MostRecentManualSaveName_PStringData) == 0x13c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, AutoSaveCheckPointExists) == 0x140);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, SaveGameMarkerPos) == 0x141);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, SaveGameMarkerAngleXY) == 0x150);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildSealRecallPos_X) == 0x154);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildSealRecallPos_Y) == 0x158);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildSealRecallPos_Z) == 0x15c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, GuildSealRecallAngleXY) == 0x160);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, WeatherMaskingPrimitivesSent) == 0x164);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, WeatherMaskingPrimitiveHandles) == 0x168);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, AtmosBanksWaitingToCopy) == 0x174);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, PlayerSpawnStatus) == 0x178);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, VillagerReactionDebug) == 0x17c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, StartTime) == 0x180);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, TimePlayed) == 0x188);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorld, HasInitialisedStartTime) == 0x190);
#endif
#endif

#endif // FABLE_ENGINE_CWORLD_H

// End shared header engine/CWorld.h
#undef CWorld
#define COpinionDeedEffectsDef COpinionDeedEffectsDefFields
// Begin shared header engine/_quarantine/COpinionDeedEffectsDef.h
#ifndef FABLE_ENGINE_COPINIONDEEDEFFECTSDEF_H
#define FABLE_ENGINE_COPINIONDEEDEFFECTSDEF_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof COpinionDeedEffectsDef = 0x94.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof COpinionDeedEffectsDef = 0x80.
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

#pragma pack(push, 1)
class COpinionDeedEffectsDef {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    unsigned char _pad_0x10[0x4];                            // +0x10
    unsigned char _pad_0x14[0x4];                            // +0x14
    unsigned char _pad_0x18[0x4];                            // +0x18
    unsigned char _pad_0x1c[0x4];                            // +0x1c
    unsigned char _pad_0x20[0x4];                            // +0x20
    unsigned char _pad_0x24[0x4];                            // +0x24
    unsigned char _pad_0x28[0x1];                            // +0x28
    unsigned char _pad_0x29[0x13];                           // +0x29
    long          KnowledgeLifeInSeconds;                    // +0x3c
    long          MinimumSecondsBetweenPostings;             // +0x40
    long          MinimumSecondsBetweenEffectsForIndividual; // +0x44
    float         PreReactionDelayIgnoreOnDelay;             // +0x48
    unsigned char Effects[0x4];                              // +0x4c list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>
    long          ReactionPriority;                          // +0x50
    unsigned char Reaction[0xc];                             // +0x54 vector<COpinionDeedReactionDef,std::allocator<COpinionDeedReactionDef>_>
    unsigned char ReactionDialogueLayer[0x4];                // +0x60 EDialogueLayer
    unsigned char WarningText[0xc];                          // +0x64 vector<CCharString,std::allocator<CCharString>_>
    unsigned char CrimeSeverity[0x4];                        // +0x70 ECrimeSeverity
    long          CrimeFineAmount;                           // +0x74
    unsigned char Tutorial[0x4];                             // +0x78 ETutorialCategory
    unsigned char OnReactSetGreeted[0x1];                    // +0x7c bitfield group: bool:1
    unsigned char BlockWhileFollowingPlayer[0x3];            // +0x7d bitfield group: bool:1
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(COpinionDeedEffectsDef) == 0x80);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, KnowledgeLifeInSeconds) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, MinimumSecondsBetweenPostings) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, MinimumSecondsBetweenEffectsForIndividual) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, PreReactionDelayIgnoreOnDelay) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, Effects) == 0x4c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, ReactionPriority) == 0x50);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, Reaction) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, ReactionDialogueLayer) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, WarningText) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, CrimeSeverity) == 0x70);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, CrimeFineAmount) == 0x74);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, Tutorial) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, OnReactSetGreeted) == 0x7c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(COpinionDeedEffectsDef, BlockWhileFollowingPlayer) == 0x7d);
#endif
#endif

#endif // FABLE_ENGINE_COPINIONDEEDEFFECTSDEF_H

// End shared header engine/_quarantine/COpinionDeedEffectsDef.h
#undef COpinionDeedEffectsDef

template<class T>class CBasicString{protected:T*PData;unsigned long StringLength;public:__declspec(noinline)bool operator==(const T*s)const{return Compare(PData,s)==0;}protected:static long __fastcall Compare(const T*a,const T*b){while(*a||*b){if(*a<*b)return -1;if(*a>*b)return 1;++a;++b;}return 0;}};
// Layout: only original string-data Data prefix is read; complete allocation metadata is not materialized.
class CCharStringData{public:CBasicString<char>Data;};
class CCharString{protected:CCharStringData*PStringData;public:CCharString();CCharString(const CCharString&);~CCharString();const CCharString&operator=(const CCharString&);const CCharString&operator=(const char*);FABLE_FORCEINLINE bool operator==(const char*s)const{if(!PStringData){if(strcmp(s,"")==0)return true;return false;}return PStringData->Data==s;}};
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;};
template<class A,class R>struct unary_function{};template<class A,class B,class R>struct binary_function{};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity:unary_function<T,T>{const T&operator()(const T&x)const{return x;}};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class T>struct _Const_traits{typedef const T&reference;typedef const T*pointer;};
template<class T>struct _Nonconst_traits{typedef T&reference;typedef T*pointer;};
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T>class _Rb_global{public:static _Rb_tree_node_base*__cdecl _M_increment(_Rb_tree_node_base*);};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{_Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Rb_tree_node<T>*p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<T,_Nonconst_traits<T> >&x){_M_node=x._M_node;}typename Traits::reference operator*()const{return static_cast<_Rb_tree_node<T>*>(_M_node)->_M_value_field;}_Rb_tree_iterator&operator++(){_M_node=_Rb_global<bool>::_M_increment(_M_node);return *this;}};
template<class V,class A>struct _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > >_M_header;};
template<class K,class V,class Select,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{protected:unsigned int _M_node_count;Compare _M_key_compare;public:typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}const_iterator end()const{return const_iterator(this->_M_header._M_data);}};
template<class K,class C=less<K>,class A=allocator<K> >class set{public:_Rb_tree<K,K,_Identity<K>,C,A>_M_t;public:typedef typename _Rb_tree<K,K,_Identity<K>,C,A>::const_iterator const_iterator;const_iterator begin()const{return _M_t.begin();}const_iterator end()const{return _M_t.end();}};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:unsigned int size()const{return this->_M_finish-this->_M_start;}const T&operator[](unsigned int n)const{return this->_M_start[n];}T*begin(){return this->_M_start;}T*end(){return this->_M_finish;}};
template<unsigned int N>struct _Base_bitset{unsigned long _M_w[N];};
template<unsigned int N>class bitset:public _Base_bitset<(N+31)/32>{public:bool test(unsigned int)const;};
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>&a,const std::pair<K,V>&b)const{return a.first<b.first;}};
class CTCBase;enum ETCInterfaceType{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{C Compare;bool Dirty;public:std::pair<K,V>*LowerBound(const K&);};
class CTCHeroOpinionDeedLog;
// Layout: imported bitset20 and Components44 retain the original nonconst GetTC template.
class CThing:public CThingFields{public:template<class T>FABLE_FORCEINLINE bool GetTC(T**out){ETCInterfaceType type=T::StatGetInterfaceType();if(reinterpret_cast<const std::bitset<274>&>(UsingComponents).test(static_cast<unsigned int>(type))){CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);ETCInterfaceType key=type;std::pair<ETCInterfaceType,CTCBase*>*i=map.LowerBound(key);std::pair<ETCInterfaceType,CTCBase*>*result=map.end();if(i!=result&&!(type<i->first))result=i;*out=reinterpret_cast<T*>(result->second);return true;}return false;}};
class CThingPlayerCreature:public CThing{};
class CPlayer:public CPlayerFields{public:CThingPlayerCreature*GetPControlledCreature()const;};
class CWorld:public CWorldFields{public:const CPlayer*PeekMyPlayer()const;};
namespace boost{template<class T>class scoped_ptr{public:T*px;public:T&operator*()const{return *px;}};}
// Layout: original PWorld24 and static singleton identity independently proven by raw TPI and native callers.
class CMainGameComponent{protected:static CMainGameComponent*PMainGameComponent;unsigned char _pad_0[0x24];boost::scoped_ptr<CWorld>PWorld;public:static CMainGameComponent*__fastcall Get(){return PMainGameComponent;}CWorld&GetWorld(){return *PWorld;}};
enum EOpinionDeedType{};
class COpinionDeed{public:EOpinionDeedType DeedType;};
// Layout: original parent definition39 followed by fields; WarningText64 and original bool bitfield7c.
class COpinionDeedEffectsDef:public CParentDefClassBase{private:unsigned char _pad_0x39[0x64-0x39];public:std::vector<CCharString>WarningText;unsigned char _pad_0x70[12];bool CanBeHeard:1,ReactionRestrictToTarget:1,AllowSimultaneousReactionDialogue:1,CanApologise:1,MustNotRunWhenWarned:1,OnlyGetFinedForOneInstance:1,IsFlirting:1,OnReactSetGreeted:1;};
class CTCHeroOpinionDeedLog{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return static_cast<ETCInterfaceType>(0x51);}const COpinionDeed*GetDeedFromHandle(long)const;CDefPointer<const COpinionDeedEffectsDef>PeekPDeedEffectsDef(EOpinionDeedType)const;};
class CTCVillage:public CTCVillageFields{private:using CTCVillageFields::PlayersCrimes;public:CCharString GetWarningStringForCurrentCrimes(long,bool&,long&)const;};
CCharString CTCVillage::GetWarningStringForCurrentCrimes(long index,bool&severe,long&deedHandle)const{
 CCharString warning;severe=false;deedHandle=-1;
 CThingPlayerCreature*hero=CMainGameComponent::Get()->GetWorld().PeekMyPlayer()->GetPControlledCreature();
 if(hero){CTCHeroOpinionDeedLog*log;hero->GetTC(&log);
 const std::set<long>&crimes=reinterpret_cast<const std::set<long>&>(PlayersCrimes);
 std::set<long>::const_iterator last=crimes.end();
 for(std::set<long>::const_iterator it=crimes.begin();it!=last;++it){const COpinionDeed*deed=log->GetDeedFromHandle(*it);
 if(deed){CDefPointer<const COpinionDeedEffectsDef>effects=log->PeekPDeedEffectsDef(deed->DeedType);
 if(index>=0&&static_cast<unsigned int>(index)<effects->WarningText.size()){deedHandle=*it;warning=effects->WarningText[index];if(effects->MustNotRunWhenWarned)severe=true;}else warning="";
 if(warning==""){severe=false;deedHandle=-1;break;}
 }
 }
 }
 return warning;
}

#undef private
#undef protected
CResource::CResource(){IVRefCount=0;ResourceList=0;PrevResource=NextResource=0;ResourceSize=LastUsedFrame=0;}CResource::~CResource(){}
bool CResource::UnloadResource(){return false;}unsigned long CResource::GetUnloadableMemory()const{return 0;}bool CResource::IsResourceLoaded()const{return true;}void CResource::OnResourceLoaded(){}bool CResource::CanAbortResourceLoad(long)const{return false;}void CResource::OnResourceLoadAborted(){}unsigned long CResource::GetDebugMemoryInUse()const{return 0;}void CResource::ResetDebugMemoryInUse(){}
static int string_allocs;
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const CCharString&x):PStringData(0){if(x.PStringData)*this=x.PStringData->Data.PData;}
CCharString::~CCharString(){if(PStringData){delete[]PStringData->Data.PData;delete PStringData;--string_allocs;}}
const CCharString&CCharString::operator=(const char*p){unsigned long n=static_cast<unsigned long>(strlen(p));char*q=new char[n+1];memcpy(q,p,n+1);if(!PStringData){PStringData=new CCharStringData;++string_allocs;}else delete[]PStringData->Data.PData;PStringData->Data.PData=q;PStringData->Data.StringLength=n;return *this;}
const CCharString&CCharString::operator=(const CCharString&x){if(this!=&x)*this=x.PStringData?x.PStringData->Data.PData:"";return *this;}
class TestDef:public CDefPointeeBase{public:unsigned char bytes[sizeof(COpinionDeedEffectsDef)+64];};
static TestDef defs[4];static COpinionDeed deeds[4];static CTCHeroOpinionDeedLog log_tc;static CWorld world;static CPlayer player;static CThingPlayerCreature hero;static CMainGameComponent game;
CMainGameComponent*CMainGameComponent::PMainGameComponent=&game;
static bool hero_exists;static int stage,mask,held,lookups,defcalls,deletes,visited[4],deleted[4],boundcalls,bitcalls;
void CIVCountedPointeeBase::Delete()const{int i;for(i=0;i<4&&this!=&defs[i];++i){}CHECK(i<4&&IVRefCount==0);++deleted[i];++deletes;}
const CPlayer*CWorld::PeekMyPlayer()const{CHECK(this==&world&&stage==0);stage=1;return &player;}
CThingPlayerCreature*CPlayer::GetPControlledCreature()const{CHECK(this==&player&&stage==1);stage=2;return hero_exists?&hero:0;}
template<unsigned int N>bool std::bitset<N>::test(unsigned int index)const{CHECK(N==274&&index==0x51&&stage==2);++bitcalls;return true;}
static std::pair<ETCInterfaceType,CTCBase*>interfaces[2];
template<class K,class V,class C>std::pair<K,V>*CVectorMap<K,V,C>::LowerBound(const K&key){CHECK(key==0x51&&stage==2);++boundcalls;return reinterpret_cast<std::pair<K,V>*>(&interfaces[0]);}
const COpinionDeed*CTCHeroOpinionDeedLog::GetDeedFromHandle(long n)const{CHECK(this==&log_tc&&stage==2&&n>=0&&n<4);++lookups;++visited[n];return mask&(1<<n)?&deeds[n]:0;}
CDefPointer<const COpinionDeedEffectsDef>CTCHeroOpinionDeedLog::PeekPDeedEffectsDef(EOpinionDeedType t)const{CHECK(this==&log_tc&&stage==2&&t>=0&&t<4);++defcalls;CDefPointer<const COpinionDeedEffectsDef>r;r.Object=&defs[t];defs[t].AddRef();return r;}
template<class T>std::_Rb_tree_node_base*__cdecl std::_Rb_global<T>::_M_increment(std::_Rb_tree_node_base*x){if(x->_M_right){x=x->_M_right;while(x->_M_left)x=x->_M_left;}else{std::_Rb_tree_node_base*y=x->_M_parent;while(x==y->_M_right){x=y;y=y->_M_parent;}if(x->_M_right!=y)x=y;}return x;}
int main(){game.PWorld.px=&world;interfaces[0].first=static_cast<ETCInterfaceType>(0x51);interfaces[0].second=reinterpret_cast<CTCBase*>(&log_tc);interfaces[1].first=static_cast<ETCInterfaceType>(0x52);interfaces[1].second=0;
 void**map=reinterpret_cast<void**>(hero.Components);map[0]=interfaces;map[1]=interfaces+1;map[2]=interfaces+2;
 CTCVillage village;int cases=0;
 for(int hp=0;hp<2;++hp)for(int count=0;count<=4;++count)for(mask=0;mask<16;++mask)for(int index=-1;index<=2;++index)for(int severe_mode=0;severe_mode<2;++severe_mode)for(held=0;held<2;++held)for(int blank=0;blank<2;++blank){
 CCharString texts[4][2];const char*text_values[4][2];for(int i=0;i<4;++i){deeds[i].DeedType=static_cast<EOpinionDeedType>(i);COpinionDeedEffectsDef*def=reinterpret_cast<COpinionDeedEffectsDef*>(&defs[i]);defs[i].IVRefCount=held;def->WarningText._M_start=texts[i];def->WarningText._M_finish=texts[i]+i%3;def->WarningText._M_end_of_storage._M_data=texts[i]+2;def->MustNotRunWhenWarned=((i+severe_mode)&1)!=0;for(int j=0;j<2;++j){text_values[i][j]=blank&&((i+j)%3==1)?"":"warning";texts[i][j]=text_values[i][j];}}
 std::_Rb_tree_node<long>nodes[4],header;header._M_parent=count?&nodes[0]:0;header._M_left=count?&nodes[0]:&header;header._M_right=count?&nodes[count-1]:&header;for(int i=0;i<count;++i){nodes[i]._M_value_field=i;nodes[i]._M_parent=i?&nodes[i-1]:&header;nodes[i]._M_left=0;nodes[i]._M_right=i+1<count?&nodes[i+1]:0;}
 reinterpret_cast<std::set<long>&>(village.PlayersCrimes)._M_t._M_header._M_data=&header;
 bool expected_severe=false;long expected_handle=-1;const char*expected="";int expected_visits[4]={0,0,0,0},expected_defs[4]={0,0,0,0};if(hp)for(int i=0;i<count;++i){expected_visits[i]=1;if(mask&(1<<i)){expected_defs[i]=1;if(index>=0&&index<i%3){expected_handle=i;expected=text_values[i][index];if((i+severe_mode)&1)expected_severe=true;}else expected="";if(!*expected){expected_severe=false;expected_handle=-1;break;}}}
 hero_exists=hp!=0;stage=lookups=defcalls=deletes=boundcalls=bitcalls=0;memset(visited,0,sizeof(visited));memset(deleted,0,sizeof(deleted));bool severe=true;long handle=1234;
 {CCharString result=village.GetWarningStringForCurrentCrimes(index,severe,handle);const char*actual=result.PStringData?result.PStringData->Data.PData:"";CHECK(strcmp(actual,expected)==0&&severe==expected_severe&&handle==expected_handle);}
 CHECK(stage==2&&boundcalls==hp&&bitcalls==hp);for(int i=0;i<4;++i){CHECK(visited[i]==expected_visits[i]&&deleted[i]==(held?0:expected_defs[i])&&defs[i].IVRefCount==static_cast<unsigned long>(held));}++cases;
 }CHECK(cases==5120&&string_allocs==0);puts("0065a075_TEST PASS");return 0;}