#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __int64 long
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#define private public
#define protected public
#define CHECK(x) do{if(!(x)){printf("failure %d\n",__LINE__);exit(1);}}while(0)
// 00699b00 CGameCameraManager::PreloadScriptedCamera
// Leaf: ?PreloadScriptedCamera@CGameCameraManager@@AAEXV?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@@Z
// Flags:
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

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
#define CGameCameraManager CGameCameraManagerFields
// Begin shared header engine/CGameCameraManager.h
#ifndef FABLE_ENGINE_CGAMECAMERAMANAGER_H
#define FABLE_ENGINE_CGAMECAMERAMANAGER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CGameCameraManager = 0x184.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CGameCameraManager = 0x160.
// Retail evidence: 4 trusted landed function(s); reconciliation 3 conflict(s) in 2 file(s) vs 2 agreeing; PINNED by 2 retyped file(s) despite conflicts.
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

struct CBase;
class CCPPointerInfo;
class CDefPointeeBase;
struct CScriptModuleBase;
class CWorld;

#pragma pack(push, 1)
class CGameCameraManager {
public:
    void*              __vftable;                            // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char      Camera[sizeof(void*)==4?0x38:0x40];                         // +0x04 CCamera
    unsigned char      OldCamera[sizeof(void*)==4?0x38:0x40];                      // +0x3c CCamera
    CWorld*            World;                                // +0x74
    unsigned char      CamTools[0x20];                       // +0x78 CTools
    CBase*             CurrentMode_Data;                     // +0x98 CCountedPointer<NCameraModes::CBase>::Data
    CCPPointerInfo*    CurrentMode_Info;                     // +0x9c CCountedPointer<NCameraModes::CBase>::Info
    CBase*             PendingMode_Data;                     // +0xa0 CCountedPointer<NCameraModes::CBase>::Data
    CCPPointerInfo*    PendingMode_Info;                     // +0xa4 CCountedPointer<NCameraModes::CBase>::Info
    bool               NewAutoMode;                          // +0xa8
    bool               ObservedThingInMelee;                 // +0xa9
    bool               ObservedThingInBuilding;              // +0xaa
    unsigned char      _pad_0xab[0x1];                       // +0xab
    unsigned char      AvailableCamPoints[0x4];              // +0xac list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    bool               SingleCut;                            // +0xb0
    bool               ChangeInControlDirection;             // +0xb1
    unsigned char      _pad_0xb2[0x2];                       // +0xb2
    long               InsideHeroCount;                      // +0xb4
    float              ControlAngleXY;                       // +0xb8
    bool               AngleChange;                          // +0xbc
    bool               ControlNoControlMovement;             // +0xbd
    unsigned char      _pad_0xbe[0x2];                       // +0xbe
    long               ControlNoControlMovementStartFrame;   // +0xc0
    float              RightStickLookAroundAngles_X;         // +0xc4 C2DVector::X
    float              RightStickLookAroundAngles_Y;         // +0xc8 C2DVector::Y
    float              RightStickTargetLookAroundAngles_X;   // +0xcc C2DVector::X
    float              RightStickTargetLookAroundAngles_Y;   // +0xd0 C2DVector::Y
    CScriptModuleBase* PCurrentScriptModule_Data;            // +0xd4 CCountedPointer<NCameraModes::CScriptModuleBase>::Data
    CCPPointerInfo*    PCurrentScriptModule_Info;            // +0xd8 CCountedPointer<NCameraModes::CScriptModuleBase>::Info
    CScriptModuleBase* PPendingScriptModule_Data;            // +0xdc CCountedPointer<NCameraModes::CScriptModuleBase>::Data
    CCPPointerInfo*    PPendingScriptModule_Info;            // +0xe0 CCountedPointer<NCameraModes::CScriptModuleBase>::Info
    long               ScriptedCameraRemoveTimer;            // +0xe4
    bool               CameraPreload;                        // +0xe8
    bool               CameraPreloadUseFastMode;             // +0xe9
    bool               ScriptHasPreloadedCamera;             // +0xea
    unsigned char      _pad_0xeb[0x1];                       // +0xeb
    unsigned char      EarthQuakes[0xc];                     // +0xec vector<CGameCameraManager::CQuake,std::allocator<CGameCameraManager::CQuake>_>
    float              MinWorldHeight;                       // +0xf8
    float              MaxWorldHeight;                       // +0xfc
    unsigned char      PObservedThing[0x8];                  // +0x100 CIntelligentPointer<CThingGameObject_const_>
    unsigned char      CameraList[0xc];                      // +0x108 vector<CDefPointer<CCameraModeDef_const_>,std::allocator<CDefPointer<CCameraModeDef_const_>_>_>
    unsigned char      CameraListIndex[0x4];                 // +0x114 _Vector_iterator<std::_Vector_val<CDefPointer<CCameraModeDef_const_>,std::allocator<CDefPointer<CCameraModeDef_const_>_>_>_>
    unsigned char      InitialModeCameraListIndex[0x4];      // +0x118 _Vector_iterator<std::_Vector_val<CDefPointer<CCameraModeDef_const_>,std::allocator<CDefPointer<CCameraModeDef_const_>_>_>_>
    unsigned char      SpecialCameraList[0xc];               // +0x11c vector<CDefPointer<CCameraModeDef_const_>,std::allocator<CDefPointer<CCameraModeDef_const_>_>_>
    unsigned char      SpecialCameraListIndex[0x4];          // +0x128 _Vector_iterator<std::_Vector_val<CDefPointer<CCameraModeDef_const_>,std::allocator<CDefPointer<CCameraModeDef_const_>_>_>_>
    bool               UsingCameraManually;                  // +0x12c
    bool               BlockModeChange;                      // +0x12d
    unsigned char      _pad_0x12e[0x2];                      // +0x12e
    CDefPointeeBase*   PCameraManagerSet_Object;             // +0x130 CDefPointer<CCameraManagerSetDef_const_>::Object
    CDefPointeeBase*   PDef_Object;                          // +0x134 CDefPointer<CCameraManagerDef_const_>::Object
    unsigned char      RegisteredModes[0xc];                 // +0x138 map<NCameraModes::EMode,CCountedPointer<NCameraModes::CBaseInit>,std::less<NCameraModes::EMode>,std::allocator<std::pair<NCameraModes::EMode_const_,CCountedPointer<NCameraModes::CBaseInit>_>_>_>
    bool               ScriptedCameraModeFromGame;           // +0x144
    bool               ScriptedCameraPreloadFlag;            // +0x145
    unsigned char      _pad_0x146[0x2];                      // +0x146
    float              CamRSLookAroundDeltaTheta;            // +0x148
    float              CamZoomMinHeightFactor;               // +0x14c
    bool               AutoFOVZoomInCombat;                  // +0x150
    bool               CamAutoGoBehind;                      // +0x151
    bool               LimitMinMaxWorldHeight;               // +0x152
    unsigned char      _pad_0x153[0x1];                      // +0x153
    long               DrawDebugGraphics;                    // +0x154
    unsigned long      DebugPersistFrame;                    // +0x158
    unsigned char      DebugPoints[0x4];                     // +0x15c list<std::pair<C3DVector,CRGBColour>,std::allocator<std::pair<C3DVector,CRGBColour>_>_>
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CGameCameraManager) == 0x160);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, Camera) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, OldCamera) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, World) == 0x74);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, CamTools) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, CurrentMode_Data) == 0x98);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, CurrentMode_Info) == 0x9c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, PendingMode_Data) == 0xa0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, PendingMode_Info) == 0xa4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, NewAutoMode) == 0xa8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, ObservedThingInMelee) == 0xa9);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, ObservedThingInBuilding) == 0xaa);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, AvailableCamPoints) == 0xac);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, SingleCut) == 0xb0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, ChangeInControlDirection) == 0xb1);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, InsideHeroCount) == 0xb4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, ControlAngleXY) == 0xb8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, AngleChange) == 0xbc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, ControlNoControlMovement) == 0xbd);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, ControlNoControlMovementStartFrame) == 0xc0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, RightStickLookAroundAngles_X) == 0xc4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, RightStickLookAroundAngles_Y) == 0xc8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, RightStickTargetLookAroundAngles_X) == 0xcc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, RightStickTargetLookAroundAngles_Y) == 0xd0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, PCurrentScriptModule_Data) == 0xd4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, PCurrentScriptModule_Info) == 0xd8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, PPendingScriptModule_Data) == 0xdc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, PPendingScriptModule_Info) == 0xe0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, ScriptedCameraRemoveTimer) == 0xe4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, CameraPreload) == 0xe8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, CameraPreloadUseFastMode) == 0xe9);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, ScriptHasPreloadedCamera) == 0xea);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, EarthQuakes) == 0xec);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, MinWorldHeight) == 0xf8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, MaxWorldHeight) == 0xfc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, PObservedThing) == 0x100);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, CameraList) == 0x108);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, CameraListIndex) == 0x114);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, InitialModeCameraListIndex) == 0x118);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, SpecialCameraList) == 0x11c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, SpecialCameraListIndex) == 0x128);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, UsingCameraManually) == 0x12c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, BlockModeChange) == 0x12d);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, PCameraManagerSet_Object) == 0x130);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, PDef_Object) == 0x134);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, RegisteredModes) == 0x138);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, ScriptedCameraModeFromGame) == 0x144);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, ScriptedCameraPreloadFlag) == 0x145);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, CamRSLookAroundDeltaTheta) == 0x148);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, CamZoomMinHeightFactor) == 0x14c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, AutoFOVZoomInCombat) == 0x150);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, CamAutoGoBehind) == 0x151);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, LimitMinMaxWorldHeight) == 0x152);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, DrawDebugGraphics) == 0x154);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, DebugPersistFrame) == 0x158);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager, DebugPoints) == 0x15c);
#endif
#endif

#endif // FABLE_ENGINE_CGAMECAMERAMANAGER_H

// End shared header engine/CGameCameraManager.h
#undef CGameCameraManager
#define CDisplayEngine CDisplayEngineFields
// Begin shared header engine/CDisplayEngine.h
#ifndef FABLE_ENGINE_CDISPLAYENGINE_H
#define FABLE_ENGINE_CDISPLAYENGINE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CDisplayEngine = 0x100.
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

class CCPPointerInfo;
class CDisplayViewManager;
class CGameDefinitionManager;
class CGraphicDataBank;
class CIEngine;
class CMainGameComponent;
class CMeshDataBank;
class CPlayerManager;
class CWorld;

#pragma pack(push, 1)
class CDisplayEngine {
public:
    void*                   __vftable;                       // +0x00 vptr, or first dword of a flattened base subobject
    unsigned long           EnginePreloadFlags;              // +0x04
    CMainGameComponent*     Component;                       // +0x08
    CPlayerManager*         PlayerManager;                   // +0x0c
    CGameDefinitionManager* DefinitionManager;               // +0x10
    CGraphicDataBank*       EngineGraphicBank;               // +0x14
    CMeshDataBank*          MeshBank;                        // +0x18
    float                   MainWindow_TLX;                  // +0x1c C2DBoxF::TLX
    float                   MainWindow_TLY;                  // +0x20 C2DBoxF::TLY
    float                   MainWindow_BRX;                  // +0x24 C2DBoxF::BRX
    float                   MainWindow_BRY;                  // +0x28 C2DBoxF::BRY
    CIEngine*               Engine3D;                        // +0x2c
    unsigned char           Camera[sizeof(void*)==4?0x38:0x40];                    // +0x30 CCamera
    long                    Frame;                           // +0x68
    bool                    DrawGame;                        // +0x6c
    unsigned char           _pad_0x6d[0x3];                  // +0x6d
    unsigned char           LetterBox[0x20];                 // +0x70 CLetterBoxModeInfo
    bool                    CameraInputsOn;                  // +0x90
    unsigned char           _pad_0x91[0x7];                  // +0x91
    double                  LastRefreshTime;                 // +0x98
    double                  LastRenderTimeLength;            // +0xa0
    double                  LastWorldUpdateRenderTime;       // +0xa8
    bool                    PreparePrimitives;               // +0xb0
    unsigned char           _pad_0xb1[0x3];                  // +0xb1
    float                   GammaRamp;                       // +0xb4
    float                   OldGammaRamp;                    // +0xb8
    unsigned char           ScreenFadeOutInfo[0x1c];         // +0xbc CFadeInFadeOutBase
    bool                    ScreenFadeOutLocked;             // +0xd8
    bool                    DrawMemoryUse;                   // +0xd9
    unsigned char           _pad_0xda[0x2];                  // +0xda
    long                    DrawDebugPage;                   // +0xdc
    CDisplayViewManager*    PViewManager_Data;               // +0xe0 CCountedPointer<CDisplayViewManager>::Data
    CCPPointerInfo*         PViewManager_Info;               // +0xe4 CCountedPointer<CDisplayViewManager>::Info
    long                    InitialFadeReadynessCount;       // +0xe8
    bool                    InitialFadeActive;               // +0xec
    unsigned char           _pad_0xed[0x3];                  // +0xed
    float                   ScreenFadeDuration;              // +0xf0
    float                   TimeSinceFadeStarted;            // +0xf4
    CWorld*                 World;                           // +0xf8
    unsigned char           _pad_0xfc[0x4];                  // +0xfc
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDisplayEngine) == 0x100);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, EnginePreloadFlags) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, Component) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, PlayerManager) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, DefinitionManager) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, EngineGraphicBank) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, MeshBank) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, MainWindow_TLX) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, MainWindow_TLY) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, MainWindow_BRX) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, MainWindow_BRY) == 0x28);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, Engine3D) == 0x2c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, Camera) == 0x30);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, Frame) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, DrawGame) == 0x6c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, LetterBox) == 0x70);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, CameraInputsOn) == 0x90);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, LastRefreshTime) == 0x98);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, LastRenderTimeLength) == 0xa0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, LastWorldUpdateRenderTime) == 0xa8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, PreparePrimitives) == 0xb0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, GammaRamp) == 0xb4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, OldGammaRamp) == 0xb8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, ScreenFadeOutInfo) == 0xbc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, ScreenFadeOutLocked) == 0xd8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, DrawMemoryUse) == 0xd9);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, DrawDebugPage) == 0xdc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, PViewManager_Data) == 0xe0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, PViewManager_Info) == 0xe4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, InitialFadeReadynessCount) == 0xe8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, InitialFadeActive) == 0xec);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, ScreenFadeDuration) == 0xf0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, TimeSinceFadeStarted) == 0xf4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, World) == 0xf8);
#endif
#endif

#endif // FABLE_ENGINE_CDISPLAYENGINE_H

// End shared header engine/CDisplayEngine.h
#undef CDisplayEngine
#define CIEngine CIEngineFields
// Begin shared header engine/CIEngine.h
#ifndef FABLE_ENGINE_CIENGINE_H
#define FABLE_ENGINE_CIENGINE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CIEngine = 0xc.
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

#pragma pack(push, 1)
class CIEngine {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    bool          Active;                                    // +0x08
    unsigned char _pad_0x09[0x3];                            // +0x09
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CIEngine) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CIEngine, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CIEngine, Active) == 0x8);
#endif
#endif

#endif // FABLE_ENGINE_CIENGINE_H

// End shared header engine/CIEngine.h
#undef CIEngine
class C2DBoxF;
class C2DBoxI;
class C2DVector;
class C3DAnimationInfo;
class C3DBoundingBox;
class C3DBoundingSphere;
class C3DVector;
class CCamera;
class CCharString;
class CEngineInit;
class CEngineInterpolationInfo;
class CEnginePrimitive;
class CEnginePrimitiveAnimation;
class CEnginePrimitiveAttachmentInfo;
class CEnginePrimitiveHandle;
class CEnginePrimitivePosition;
class CEngineVideoOptions;
class CIDrawWorld;
class CRGBColour;
class CStaticMapGenerationVisibiilityInfo;
class CTexture;
class CWideString;
namespace std{template<class A,class B>struct pair;template<class T>struct less;template<class T>class allocator;template<class K,class V,class C,class A>class map;}
template<class T>class CArray;
enum EEnginePrimitiveStatIndex{};
class CInitBaseClass:public CBaseClass{bool Valid;public:virtual~CInitBaseClass();virtual void Validate()const;};
// Layout: original CIEngine virtual prefix through slot57 restored; shared header preserves storage evidence.
class CIEngine:public CInitBaseClass{public:virtual~CIEngine();
 virtual bool Initialise(const CEngineInit&);
 virtual void Release();
 virtual void SetCamera(const CCamera*);
 virtual void SetWindow(const C2DBoxF*);
 virtual void SetEnvironmentalEffectsAlwaysUpdate(bool);
 virtual void SetWorld(const CIDrawWorld*);
 virtual void Render(const CEngineInterpolationInfo&,unsigned long);
 virtual void WorldFrameUpdate(bool);
 virtual void PausedWorldFrameUpdate();
 virtual bool ConvertScreenPosToWorldPos(const C2DVector&,C3DVector&)const;
 virtual bool ConvertScreenPosToWorldPosAtViewDistance(float,const C2DVector&,C3DVector&)const;
 virtual bool ConvertWorldPosToScreenPos(const C3DVector&,C2DVector&)const;
 virtual bool SetVideoOptions(const CEngineVideoOptions&);
 virtual bool PreChangeResolution();
 virtual bool PostChangeResolution(unsigned long,unsigned long,unsigned long);
 virtual void Refresh();
 virtual bool CheckPrimitiveVisibility(unsigned __int64);
 virtual float GetPrimitiveDrawDistanceFactor()const;
 virtual void AddPrimitive(const CEnginePrimitive&);
 virtual bool ClipSphere(const C3DBoundingSphere&)const;
 virtual bool ClipBox(const C3DBoundingBox&)const;
 virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long);
 virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle&,const CEnginePrimitiveAnimation&);
 virtual void SetPrimitivePosition(CEnginePrimitiveHandle&,const CEnginePrimitivePosition&);
 virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long);
 virtual void SetChildPrimitive(CEnginePrimitiveHandle&,unsigned long,CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long);
 virtual void SetChildPrimitive(CEnginePrimitiveHandle&,unsigned long,const CEnginePrimitive&,unsigned long);
 virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle&,unsigned long,CEnginePrimitiveHandle&);
 virtual void ClearChildPrimitive(CEnginePrimitiveHandle&,unsigned long);
 virtual void ClearAllChildPrimitives(CEnginePrimitiveHandle&);
 virtual void SetAttachedPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,const CEnginePrimitiveAttachmentInfo&,const CEnginePrimitiveHandle&,long,unsigned long);
 virtual void CollectPrimitiveStatsForArea(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > >&,const C2DBoxF&,EEnginePrimitiveStatIndex,unsigned long)const;
 virtual void ProcessPrimitiveStats(const std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long&,CArray<CCharString>&)const;
 virtual const C3DAnimationInfo* GetEditorAnimation(const CWideString&)const;
 virtual unsigned long AddCustomMorphTexture(CTexture&,const CCharString&)const;
 virtual long GetStaticMapUpdatePassCount();
 virtual void StartStaticMapUpdatePass(const CWideString&,long,const CStaticMapGenerationVisibiilityInfo&,const CArray<CCharString>&);
 virtual bool IsStaticMapNeedingUpdate(long,const CCharString&,unsigned long)const;
 virtual void UpdateStaticMap(long,const CCharString&,unsigned long,long);
 virtual void CloseStaticMapFile();
 virtual void AddMap(long,long);
 virtual void RemoveMap(long);
 virtual void SetMapAsLoaded(long);
 virtual void SetMapAsUnloaded(long);
 virtual void SetMapVisibilityAllMaps(bool);
 virtual void SetMapVisibility(long,bool);
 virtual void AreaChanged(const C2DBoxI&);
 virtual void OnActivate();
 virtual void OnDeactivate();
 virtual bool IsActive()const;
 virtual void SetStaticMapFileForUse(const CWideString&);
 virtual void SetLandscapeDebugTintColour(long,CRGBColour);
 virtual void ReloadLocalDetailDefs();
 virtual void StartNewScene(unsigned long,float);
 virtual void CutscenePreload(bool,bool);
 virtual void CutsceneAddCameraAngle(const CCamera&);
private:bool Active;};
class CDisplayEngine:public CDisplayEngineFields{public:CIEngine&Get3DEngine(){return *Engine3D;}};
namespace boost{template<class T>class scoped_ptr{T*ptr;public:void fixture_set(T*p){ptr=p;}T&operator*()const{return *ptr;}};}
// Layout: original MainGameComponent PDisplayEngine at28; raw original type is scoped_ptr.
class CMainGameComponent{public:static CMainGameComponent*PMainGameComponent;unsigned char _pad_0[0x28];boost::scoped_ptr<CDisplayEngine>PDisplayEngine;public:static CMainGameComponent*__fastcall Get(){return PMainGameComponent;}CDisplayEngine&GetDisplayEngine(){return *PDisplayEngine;}};
class C3DVector{public:float X,Y,Z;};
class CRightHandedSet{protected:C3DVector Up,Forward;};
// Layout: original camera56-byte object; flags at24/25, FOVFlags28, floats2c..34.
class CCamera{public:C3DVector WorldPosition;CRightHandedSet RHSet;bool HeightLocked,ViewVecZLocked;unsigned long FOVFlags;float HorizontalFOV,VerticalFOV,Zoom;public:CCamera():HeightLocked(false),ViewVecZLocked(false){}CCamera(const CCamera&);};
class CGameCameraManager;class CCameraModeDef;
namespace NCameraModes{
class CScriptModuleBase{public:virtual ~CScriptModuleBase();};
// Layout: native CBase prefix is a4: donor vector shrinks4 and two physics-state list pairs shrink16 each.
class CBase{unsigned char _pad_0x04[0xa0];public:virtual~CBase();CBase(CGameCameraManager&,const CDefPointer<const CCameraModeDef>&);void TransferPersistantBaseData(const CBase&);void Init();};
class CScriptedControl:public CBase{CCountedPointer<CScriptModuleBase>PScriptModule;float ObservedThingMeshHeightOnInit;CCamera InitCamera;public:CScriptedControl(CGameCameraManager&,const CDefPointer<const CCameraModeDef>&,CCountedPointer<CScriptModuleBase>&);};
}
namespace {extern CDefPointer<const CCameraModeDef>DEFAULT_CAMERA_MODE_DEF;}
// Layout: the actual receiver storage is the supplied header; original method is private.
class CGameCameraManager:public CGameCameraManagerFields{public:void PreloadScriptedCamera(CCountedPointer<NCameraModes::CScriptModuleBase>);};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CCamera)==0x38);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CBase)==0xa4);
#endif
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CScriptedControl)==0xe8);
#endif

#pragma auto_inline(off)
NCameraModes::CScriptedControl::CScriptedControl(CGameCameraManager&manager,const CDefPointer<const CCameraModeDef>&def,CCountedPointer<CScriptModuleBase>&module):CBase(manager,def),PScriptModule(module),ObservedThingMeshHeightOnInit(1.0f){}
#pragma auto_inline(on)

void CGameCameraManager::PreloadScriptedCamera(CCountedPointer<NCameraModes::CScriptModuleBase> module){
 CCamera&camera=*reinterpret_cast<CCamera*>(Camera);
 CCamera savedCamera(camera);
 CCamera&oldCamera=*reinterpret_cast<CCamera*>(OldCamera);
 CCamera savedOldCamera(oldCamera);
 CCountedPointer<NCameraModes::CBase>mode;
 mode.Reset(new NCameraModes::CScriptedControl(*this,DEFAULT_CAMERA_MODE_DEF,module));
 if(CurrentMode_Data)mode->TransferPersistantBaseData(*reinterpret_cast<NCameraModes::CBase*>(CurrentMode_Data));
 mode->Init();CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().CutsceneAddCameraAngle(camera);
 camera=savedCamera;oldCamera=savedOldCamera;
}

#undef private
#undef protected
class CRGBColour{public:unsigned char R,G,B,A;};
namespace {CDefPointer<const CCameraModeDef>DEFAULT_CAMERA_MODE_DEF;}
CMainGameComponent*CMainGameComponent::PMainGameComponent=0;
static CGameCameraManager*active;static NCameraModes::CBase*expectedPrevious;
static int copies,constructs,transfers,inits,angles,modeDeletes,moduleDeletes,stage;
static CCamera originalCamera,originalOld;
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CInitBaseClass::~CInitBaseClass(){}void CInitBaseClass::Validate()const{abort();}
CIEngine::~CIEngine(){}
CCamera::CCamera(const CCamera&o){memcpy(this,&o,sizeof o);++copies;}
NCameraModes::CScriptModuleBase::~CScriptModuleBase(){++moduleDeletes;}
NCameraModes::CBase::CBase(CGameCameraManager&manager,const CDefPointer<const CCameraModeDef>&def){CHECK(&manager==active);CHECK(&def==&DEFAULT_CAMERA_MODE_DEF);++constructs;}
NCameraModes::CBase::~CBase(){++modeDeletes;}
void NCameraModes::CBase::TransferPersistantBaseData(const CBase&other){CHECK(&other==expectedPrevious&&stage==0);++transfers;}
void NCameraModes::CBase::Init(){CHECK(stage==0&&transfers==(expectedPrevious!=0));++inits;stage=1;
 CCamera&c=*reinterpret_cast<CCamera*>(active->Camera);CCamera&o=*reinterpret_cast<CCamera*>(active->OldCamera);c.WorldPosition.X=64.0f;o.WorldPosition.X=128.0f;
}
void CIEngine::CutsceneAddCameraAngle(const CCamera&c){CHECK(stage==1&&&c==reinterpret_cast<const CCamera*>(active->Camera));CHECK(c.WorldPosition.X==64.0f);++angles;stage=2;}
static void prepare(CCamera&c,unsigned seed){memset(&c,0,sizeof c);c.WorldPosition.X=float(seed);c.WorldPosition.Y=float(seed+1);c.WorldPosition.Z=float(seed+2);c.FOVFlags=seed+7;c.HeightLocked=(seed&1)!=0;c.ViewVecZLocked=(seed&2)!=0;c.HorizontalFOV=4.0f;c.VerticalFOV=2.0f;c.Zoom=1.0f;}
bool CIEngine::Initialise(const CEngineInit&){abort();}
void CIEngine::Release(){abort();}
void CIEngine::SetCamera(const CCamera*){abort();}
void CIEngine::SetWindow(const C2DBoxF*){abort();}
void CIEngine::SetEnvironmentalEffectsAlwaysUpdate(bool){abort();}
void CIEngine::SetWorld(const CIDrawWorld*){abort();}
void CIEngine::Render(const CEngineInterpolationInfo&,unsigned long){abort();}
void CIEngine::WorldFrameUpdate(bool){abort();}
void CIEngine::PausedWorldFrameUpdate(){abort();}
bool CIEngine::ConvertScreenPosToWorldPos(const C2DVector&,C3DVector&)const{abort();}
bool CIEngine::ConvertScreenPosToWorldPosAtViewDistance(float,const C2DVector&,C3DVector&)const{abort();}
bool CIEngine::ConvertWorldPosToScreenPos(const C3DVector&,C2DVector&)const{abort();}
bool CIEngine::SetVideoOptions(const CEngineVideoOptions&){abort();}
bool CIEngine::PreChangeResolution(){abort();}
bool CIEngine::PostChangeResolution(unsigned long,unsigned long,unsigned long){abort();}
void CIEngine::Refresh(){abort();}
bool CIEngine::CheckPrimitiveVisibility(unsigned __int64){abort();}
float CIEngine::GetPrimitiveDrawDistanceFactor()const{abort();}
void CIEngine::AddPrimitive(const CEnginePrimitive&){abort();}
bool CIEngine::ClipSphere(const C3DBoundingSphere&)const{abort();}
bool CIEngine::ClipBox(const C3DBoundingBox&)const{abort();}
void CIEngine::SetPersistentPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long){abort();}
void CIEngine::SetPrimitiveAnimation(CEnginePrimitiveHandle&,const CEnginePrimitiveAnimation&){abort();}
void CIEngine::SetPrimitivePosition(CEnginePrimitiveHandle&,const CEnginePrimitivePosition&){abort();}
unsigned long CIEngine::AddChildPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long){abort();}
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&,unsigned long,CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long){abort();}
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&,unsigned long,const CEnginePrimitive&,unsigned long){abort();}
void CIEngine::GetChildPrimitiveHandle(CEnginePrimitiveHandle&,unsigned long,CEnginePrimitiveHandle&){abort();}
void CIEngine::ClearChildPrimitive(CEnginePrimitiveHandle&,unsigned long){abort();}
void CIEngine::ClearAllChildPrimitives(CEnginePrimitiveHandle&){abort();}
void CIEngine::SetAttachedPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,const CEnginePrimitiveAttachmentInfo&,const CEnginePrimitiveHandle&,long,unsigned long){abort();}
void CIEngine::CollectPrimitiveStatsForArea(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > >&,const C2DBoxF&,EEnginePrimitiveStatIndex,unsigned long)const{abort();}
void CIEngine::ProcessPrimitiveStats(const std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long&,CArray<CCharString>&)const{abort();}
const C3DAnimationInfo* CIEngine::GetEditorAnimation(const CWideString&)const{abort();}
unsigned long CIEngine::AddCustomMorphTexture(CTexture&,const CCharString&)const{abort();}
long CIEngine::GetStaticMapUpdatePassCount(){abort();}
void CIEngine::StartStaticMapUpdatePass(const CWideString&,long,const CStaticMapGenerationVisibiilityInfo&,const CArray<CCharString>&){abort();}
bool CIEngine::IsStaticMapNeedingUpdate(long,const CCharString&,unsigned long)const{abort();}
void CIEngine::UpdateStaticMap(long,const CCharString&,unsigned long,long){abort();}
void CIEngine::CloseStaticMapFile(){abort();}
void CIEngine::AddMap(long,long){abort();}
void CIEngine::RemoveMap(long){abort();}
void CIEngine::SetMapAsLoaded(long){abort();}
void CIEngine::SetMapAsUnloaded(long){abort();}
void CIEngine::SetMapVisibilityAllMaps(bool){abort();}
void CIEngine::SetMapVisibility(long,bool){abort();}
void CIEngine::AreaChanged(const C2DBoxI&){abort();}
void CIEngine::OnActivate(){abort();}
void CIEngine::OnDeactivate(){abort();}
bool CIEngine::IsActive()const{abort();}
void CIEngine::SetStaticMapFileForUse(const CWideString&){abort();}
void CIEngine::SetLandscapeDebugTintColour(long,CRGBColour){abort();}
void CIEngine::ReloadLocalDetailDefs(){abort();}
void CIEngine::StartNewScene(unsigned long,float){abort();}
void CIEngine::CutscenePreload(bool,bool){abort();}
int main(){
 CIEngine engine;CDisplayEngine display;memset(static_cast<CDisplayEngineFields*>(&display),0,sizeof(CDisplayEngineFields));display.Engine3D=&engine;
 CMainGameComponent mainComponent;mainComponent.PDisplayEngine.fixture_set(&display);CMainGameComponent::PMainGameComponent=&mainComponent;
 CGameCameraManager manager;memset(&manager,0,sizeof manager);active=&manager;
 unsigned cases=0;
 for(unsigned seed=0;seed<16;++seed)for(int previous=0;previous<2;++previous)for(int present=0;present<2;++present)for(int extra=0;extra<3;++extra){
  NCameraModes::CBase current(manager,DEFAULT_CAMERA_MODE_DEF);expectedPrevious=previous?&current:0;manager.CurrentMode_Data=reinterpret_cast<CBase*>(expectedPrevious);
  CCountedPointer<NCameraModes::CScriptModuleBase>module(present?new NCameraModes::CScriptModuleBase:0),holders[2];
  for(int k=0;k<extra;++k)holders[k]=module;
  unsigned long refs=module.GetReferenceCount();
  prepare(*reinterpret_cast<CCamera*>(manager.Camera),seed);prepare(*reinterpret_cast<CCamera*>(manager.OldCamera),seed+16);
  memcpy(&originalCamera,manager.Camera,sizeof originalCamera);memcpy(&originalOld,manager.OldCamera,sizeof originalOld);
  copies=constructs=transfers=inits=angles=modeDeletes=moduleDeletes=stage=0;
  manager.PreloadScriptedCamera(module);
  CHECK(copies==2&&constructs==1&&transfers==previous&&inits==1&&angles==1&&stage==2);CHECK(modeDeletes==1&&moduleDeletes==0&&module.GetReferenceCount()==refs);
  CHECK(memcmp(manager.Camera,&originalCamera,sizeof originalCamera)==0&&memcmp(manager.OldCamera,&originalOld,sizeof originalOld)==0);
  for(int j=0;j<extra;++j)holders[j].ReleaseData();module.ReleaseData();CHECK(moduleDeletes==present);++cases;
 }
 CHECK(cases==192);puts("00699b00_TEST PASS");return 0;
}