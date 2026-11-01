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
#define malloc TestMalloc
#define free TestFree
#define CHECK(x)do{if(!(x)){fprintf(stderr,"line %d\n",__LINE__);abort();}}while(0)
// 00701750 COpinionReactionManager::GetThingOpinionOfPlayer
// Leaf: ?GetThingOpinionOfPlayer@COpinionReactionManager@@QBEMABVCThing@@W4EOpinion@@@Z
// Flags:
#include <stddef.h>
extern "C" void*__cdecl malloc(size_t);extern "C" void __cdecl free(void*);
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
namespace std{
template<class T>class allocator{public:T*allocate(size_t n){return static_cast<T*>(malloc(n*sizeof(T)));}void deallocate(T*p,size_t){free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class Out,class N,class T>inline Out fill_n(Out first,N n,const T&v){for(;n>0;--n,++first)*first=v;return first;}
struct __true_type{};
template<class I,class N,class T>inline I __uninitialized_fill_n(I first,N n,const T&v,const __true_type&){return fill_n(first,n,v);}
template<class I,class N,class T>inline I uninitialized_fill_n(I first,N n,const T&v){return __uninitialized_fill_n(first,n,v,__true_type());}
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:explicit vector(size_t n):_Vector_base<T,A>(n,A()){this->_M_finish=uninitialized_fill_n(this->_M_start,n,T());}vector&operator=(const vector&x){if(this!=&x){size_t n=x._M_finish-x._M_start;if(n>size_t(this->_M_end_of_storage._M_data-this->_M_start)){T*tmp=this->_M_end_of_storage.allocate(n);for(size_t i=0;i<n;++i)tmp[i]=x._M_start[i];if(this->_M_start)this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);this->_M_start=tmp;this->_M_end_of_storage._M_data=tmp+n;}else{for(size_t i=0;i<n;++i)this->_M_start[i]=x._M_start[i];}this->_M_finish=this->_M_start+n;}return *this;}const T&operator[](size_t n)const{return this->_M_start[n];}const T*begin()const{return this->_M_start;}const T*end()const{return this->_M_finish;}};
template<class A,class B>struct pair{A first;B second;};
}
template<class T>class CArray:public std::vector<T>{public:explicit CArray(unsigned int n):std::vector<T>(n){}const T&operator[](unsigned int n)const{return std::vector<T>::operator[](n);}T&operator[](unsigned int n){return const_cast<T&>(static_cast<const std::vector<T>&>(*this)[n]);} };
class COpinionSet{protected:CArray<float>OpinionValues;public:float&operator[](long n){return const_cast<float&>(OpinionValues[n]);}COpinionSet():OpinionValues(5){}float operator[](long n)const{return OpinionValues[n];}};
enum EOpinion{};enum ETCInterfaceType{};class CTCBase;
template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{C Compare;bool Dirty;public:__declspec(noinline)const std::pair<K,V>*LowerBound(const K&key)const{const std::pair<K,V>*first=this->begin();long count=this->end()-first;while(count>0){long half=count/2;const std::pair<K,V>*middle=first+half;if(middle->first<key){first=middle+1;count-=half+1;}else count=half;}return first;}};
// Layout: shared flattened CThing fields; the original const PeekTC template reads native bitset20 and map44.
class CThing:public CThingFields{public:template<class T>__forceinline bool PeekTC(T**out)const{ETCInterfaceType type=T::StatGetInterfaceType();if(reinterpret_cast<const unsigned long*>(UsingComponents)[type>>5]&(1UL<<(type&31))){ETCInterfaceType key=type;const CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<const CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);const std::pair<ETCInterfaceType,CTCBase*>*i=map.LowerBound(key);if(i==map.end()||type<i->first)i=map.end();*out=reinterpret_cast<T*>(i->second);return true;}return false;}};
class CThingPlayerCreature;
class CPlayer:public CPlayerFields{public:const CThingPlayerCreature*PeekPControlledCreature()const;};
class CWorld:public CWorldFields{public:const CPlayer*PeekMyPlayer()const;};
#define CTCCreatureOpinionOfHero CTCCreatureOpinionOfHeroFields
// Begin shared header engine/CTCCreatureOpinionOfHero.h
#ifndef FABLE_ENGINE_CTCCREATUREOPINIONOFHERO_H
#define FABLE_ENGINE_CTCCREATUREOPINIONOFHERO_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCCreatureOpinionOfHero = 0x65c.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CTCCreatureOpinionOfHero = 0x350.
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
class CTCCreatureOpinionOfHero {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    long             LastOpinionReactionFrame;               // +0x0c
    float            NumberOfTimesHit;                       // +0x10
    float            ToleranceToBeingHitOverride;            // +0x14
    long             FrameToDecayNumberOfTimesHit;           // +0x18
    CDefPointeeBase* PDef_Object;                            // +0x1c CDefPointer<COpinionOfHeroDef_const_>::Object
    CDefPointeeBase* PPersonalityDef_Object;                 // +0x20 CDefPointer<COpinionPersonalityDef_const_>::Object
    unsigned char    AttitudeMask[0xc];                      // +0x24 vector<bool,std::allocator<bool>_>
    unsigned char    ReactionMask[0xc];                      // +0x30 vector<bool,std::allocator<bool>_>
    unsigned char    DeedMask[0xc];                          // +0x3c vector<bool,std::allocator<bool>_>
    unsigned char    RelativeOpinion[0xc];                   // +0x48 COpinionSet
    unsigned char    PendingDeeds[0x4];                      // +0x54 list<long,std::allocator<long>_>
    long             LastFrameAppliedDeedEffect[92];         // +0x58
    unsigned char    KnownDeeds[0x170];                      // +0x1c8 list<long,std::allocator<long>_>[92]
    unsigned char    TransientOffsets[0x4];                  // +0x338 list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>
    unsigned char    ForcedAttitude[0x4];                    // +0x33c EOpinionAttitudeType
    bool             InteractedFlag;                         // +0x340
    bool             GreetedFlag;                            // +0x341
    bool             HeroOpinionEnemy;                       // +0x342
    bool             InFindAGuardAndYellForHelp;             // +0x343
    unsigned char    PUnknownThreatOpinionSource[0x8];       // +0x344 CIntelligentPointer<CThing>
    long             FrameLastUpdatedUnknownThreat;          // +0x34c
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CTCCreatureOpinionOfHero) == 0x350);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, LastOpinionReactionFrame) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, NumberOfTimesHit) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, ToleranceToBeingHitOverride) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, FrameToDecayNumberOfTimesHit) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, PDef_Object) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, PPersonalityDef_Object) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, AttitudeMask) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, ReactionMask) == 0x30);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, DeedMask) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, RelativeOpinion) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, PendingDeeds) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, LastFrameAppliedDeedEffect) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, KnownDeeds) == 0x1c8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, TransientOffsets) == 0x338);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, ForcedAttitude) == 0x33c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, InteractedFlag) == 0x340);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, GreetedFlag) == 0x341);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, HeroOpinionEnemy) == 0x342);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, InFindAGuardAndYellForHelp) == 0x343);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, PUnknownThreatOpinionSource) == 0x344);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCCreatureOpinionOfHero, FrameLastUpdatedUnknownThreat) == 0x34c);
#endif
#endif

#endif // FABLE_ENGINE_CTCCREATUREOPINIONOFHERO_H

// End shared header engine/CTCCreatureOpinionOfHero.h
#undef CTCCreatureOpinionOfHero
class CTCCreatureOpinionOfHero:public CTCCreatureOpinionOfHeroFields{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0x4f;}};
class CTCOpinionSource{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0x50;}const COpinionSet&PeekOpinionProduced()const;};class COpinionReactionManagerDef;
// Layout: original nonpolymorphic manager PDef0 World4; no dedicated header supplied.
class COpinionReactionManager{private:CDefPointer<const COpinionReactionManagerDef>PDef;const CWorld&World;public:COpinionReactionManager(const CWorld&w):World(w){}float GetThingOpinionOfPlayer(const CThing&,EOpinion)const;private:void GetOpinions(const CTCCreatureOpinionOfHero*,COpinionSet&,const CTCOpinionSource*)const;};
float COpinionReactionManager::GetThingOpinionOfPlayer(const CThing&thing,EOpinion opinion)const{
 const CTCCreatureOpinionOfHero*opinionOfHero=0;thing.PeekTC(&opinionOfHero);
 COpinionSet opinions;
 const CThingPlayerCreature*hero=World.PeekMyPlayer()->PeekPControlledCreature();const CTCOpinionSource*source=0;
 if(hero)reinterpret_cast<const CThing*>(hero)->PeekTC(&source);
 GetOpinions(opinionOfHero,opinions,source);return opinions[opinion];
}

#undef private
#undef protected
#undef malloc
#undef free
class CThingPlayerCreature:public CThing{};
static CWorld world;static CPlayer player;static CThingPlayerCreature hero;static CThing thing;static CTCCreatureOpinionOfHero opinion_tc;static CTCOpinionSource source_tc;
static const CTCCreatureOpinionOfHero*want_opinion;static const CTCOpinionSource*want_source;static bool hero_exists;static int mode,seed,stage,allocations,frees;static void*live[4];static int live_count;
extern "C" void*__cdecl TestMalloc(size_t n){CHECK(live_count<4&&n!=0);void*p=malloc(n);CHECK(p);memset(p,0xa5,n);live[live_count++]=p;++allocations;return p;}
extern "C" void __cdecl TestFree(void*p){CHECK(p);int i;for(i=0;i<live_count&&live[i]!=p;++i){}CHECK(i<live_count);live[i]=live[--live_count];++frees;free(p);}
const CPlayer*CWorld::PeekMyPlayer()const{CHECK(this==&world&&stage==0);stage=1;return &player;}
const CThingPlayerCreature*CPlayer::PeekPControlledCreature()const{CHECK(this==&player&&stage==1);stage=2;return hero_exists?&hero:0;}
void COpinionReactionManager::GetOpinions(const CTCCreatureOpinionOfHero*a,COpinionSet&out,const CTCOpinionSource*b)const{
 CHECK(stage==2&&a==want_opinion&&b==want_source);stage=3;CHECK(out.OpinionValues._M_finish-out.OpinionValues._M_start==5);CHECK(out.OpinionValues._M_end_of_storage._M_data==out.OpinionValues._M_finish);
 for(long i=0;i<5;++i)CHECK(out[i]==0.0f);
 if(mode){float*next=static_cast<float*>(TestMalloc(7*sizeof(float)));TestFree(out.OpinionValues._M_start);out.OpinionValues._M_start=next;out.OpinionValues._M_finish=next+5;out.OpinionValues._M_end_of_storage._M_data=next+7;}
 for(long i=0;i<5;++i)out[i]=float(seed*16+i+(a?2:0)+(b?4:0));
}
static void configure(CThing&t,std::pair<ETCInterfaceType,CTCBase*>*pairs,int n,int key,bool bit){memset(t.UsingComponents,0,sizeof(t.UsingComponents));reinterpret_cast<unsigned long*>(t.UsingComponents)[key>>5]=bit?(1UL<<(key&31)):0;void**slots=reinterpret_cast<void**>(t.Components);slots[0]=pairs;slots[1]=pairs+n;slots[2]=pairs+n;}
int main(){COpinionReactionManager manager(world);int cases=0;
 for(int hp=0;hp<2;++hp)for(int ob=0;ob<2;++ob)for(int oe=0;oe<2;++oe)for(int sb=0;sb<2;++sb)for(int se=0;se<2;++se)for(mode=0;mode<2;++mode)for(seed=-1;seed<=1;++seed)for(long index=0;index<5;++index){
 std::pair<ETCInterfaceType,CTCBase*>opairs[3],spairs[3];opairs[0].first=(ETCInterfaceType)1;opairs[0].second=0;opairs[1].first=(ETCInterfaceType)(oe?0x4f:0x51);opairs[1].second=oe?reinterpret_cast<CTCBase*>(&opinion_tc):0;opairs[2].first=(ETCInterfaceType)0x60;opairs[2].second=0;
 spairs[0].first=(ETCInterfaceType)1;spairs[0].second=0;spairs[1].first=(ETCInterfaceType)(se?0x50:0x51);spairs[1].second=se?reinterpret_cast<CTCBase*>(&source_tc):0;spairs[2].first=(ETCInterfaceType)0x60;spairs[2].second=0;
 configure(thing,opairs,2,0x4f,ob!=0);configure(hero,spairs,2,0x50,sb!=0);hero_exists=hp!=0;want_opinion=ob&&oe?&opinion_tc:0;want_source=hp&&sb&&se?&source_tc:0;stage=allocations=frees=live_count=0;
 float value=manager.GetThingOpinionOfPlayer(thing,(EOpinion)index);CHECK(value==float(seed*16+index+(want_opinion?2:0)+(want_source?4:0)));CHECK(stage==3&&allocations==1+mode&&frees==1+mode&&live_count==0);++cases;
 }CHECK(cases==960);puts("00701750_TEST PASS");return 0;}