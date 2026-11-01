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
class CCharStringData;class CCharString {CCharStringData*PStringData;};
class C2DVector {public:float X,Y;};
class C2DBoxF {public:float TLX,TLY,BRX,BRY;};
class CRGBColour {public:unsigned char B,G,R,A;};
#define malloc test_malloc
// 0053dd50 NUISystem::CMenuEntry::AddUIActionParam
// Leaf: ?AddUIActionParam@CMenuEntry@NUISystem@@UAEXPAVCActionParam@2@@Z
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
#define CComponent CComponentFields
// Begin shared header engine/_quarantine/CComponent.h
#ifndef FABLE_ENGINE_CCOMPONENT_H
#define FABLE_ENGINE_CCOMPONENT_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CComponent = 0x15c.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CComponent = 0x134.
// Retail evidence: 1 trusted landed function(s); reconciliation 3 conflict(s) in 1 file(s) vs 0 agreeing.
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

#pragma pack(push, 1)
struct CComponent {
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0xc];                         // +0x08
    unsigned char    _pad_0x14[0x1];                         // +0x14
    unsigned char    _pad_0x15[0x3];                         // +0x15
    unsigned char    _pad_0x18[0x4];                         // +0x18
    unsigned char    _pad_0x1c[0x4];                         // +0x1c
    CCharStringData* Name_PStringData;                       // +0x20 CCharString::PStringData
    unsigned char    States[0xc];                            // +0x24 map<unsigned_long,NUISystem::CUIState,std::less<unsigned_long>,std::allocator<std::pair<unsigned_long_const_,NUISystem::CUIState>_>_>
    float            Time;                                   // +0x30
    float            Position_X;                             // +0x34 C2DVector::X
    float            Position_Y;                             // +0x38 C2DVector::Y
    float            TargetPosition_X;                       // +0x3c C2DVector::X
    float            TargetPosition_Y;                       // +0x40 C2DVector::Y
    float            InitialPosition_X;                      // +0x44 C2DVector::X
    float            InitialPosition_Y;                      // +0x48 C2DVector::Y
    float            ParentPosition_X;                       // +0x4c C2DVector::X
    float            ParentPosition_Y;                       // +0x50 C2DVector::Y
    float            RenderPosition_X;                       // +0x54 C2DVector::X
    float            RenderPosition_Y;                       // +0x58 C2DVector::Y
    float            Zoom_X;                                 // +0x5c C2DVector::X
    float            Zoom_Y;                                 // +0x60 C2DVector::Y
    float            TargetZoom_X;                           // +0x64 C2DVector::X
    float            TargetZoom_Y;                           // +0x68 C2DVector::Y
    float            InitialZoom_X;                          // +0x6c C2DVector::X
    float            InitialZoom_Y;                          // +0x70 C2DVector::Y
    float            ParentZoom_X;                           // +0x74 C2DVector::X
    float            ParentZoom_Y;                           // +0x78 C2DVector::Y
    float            RenderZoom_X;                           // +0x7c C2DVector::X
    float            RenderZoom_Y;                           // +0x80 C2DVector::Y
    unsigned char    Colour[0x4];                            // +0x84 CRGBColour
    unsigned char    TargetColour[0x4];                      // +0x88 CRGBColour
    unsigned char    InitialColour[0x4];                     // +0x8c CRGBColour
    unsigned char    ParentColour[0x4];                      // +0x90 CRGBColour
    unsigned char    RenderColour[0x4];                      // +0x94 CRGBColour
    float            PositionTimeElapsed;                    // +0x98
    float            PositionTime;                           // +0x9c
    float            ZoomTimeElapsed;                        // +0xa0
    float            ZoomTime;                               // +0xa4
    float            ColourTimeElapsed;                      // +0xa8
    float            ColourTime;                             // +0xac
    unsigned char    Children[0xc];                          // +0xb0 vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent>_>_>
    unsigned char    ChildrenToDelete[0xc];                  // +0xbc vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent>_>_>
    CComponent*      PParent;                                // +0xc8
    float            Size_X;                                 // +0xcc C2DVector::X
    float            Size_Y;                                 // +0xd0 C2DVector::Y
    unsigned char    Deletion[0x8];                          // +0xd4 CDeletion
    CComponent*      PParentViewport;                        // +0xdc
    unsigned char    ActionsIAmParamOf[0x4];                 // +0xe0 list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction>_>_>
    unsigned char    ShapeChildren[0xc];                     // +0xe4 vector<unsigned_long,std::allocator<unsigned_long>_>
    float            RenderPositionSave_X;                   // +0xf0 C2DVector::X
    float            RenderPositionSave_Y;                   // +0xf4 C2DVector::Y
    float            RelativeRenderPosition_X;               // +0xf8 C2DVector::X
    float            RelativeRenderPosition_Y;               // +0xfc C2DVector::Y
    float            RelativeParentPosition_X;               // +0x100 C2DVector::X
    float            RelativeParentPosition_Y;               // +0x104 C2DVector::Y
    float            RelativeRenderZoom_X;                   // +0x108 C2DVector::X
    float            RelativeRenderZoom_Y;                   // +0x10c C2DVector::Y
    float            RelativeParentZoom_X;                   // +0x110 C2DVector::X
    float            RelativeParentZoom_Y;                   // +0x114 C2DVector::Y
    CComponent*      PPositionParent;                        // +0x118
    unsigned char    PositionChildren[0xc];                  // +0x11c set<NUISystem::CComponent_*,std::less<NUISystem::CComponent_*>,std::allocator<NUISystem::CComponent_*>_>
    void*            PActionsOnDestruction;                  // +0x128 list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction>_>_> *
    unsigned char    Flags;                                  // +0x12c
    unsigned char    Type;                                   // +0x12d
    unsigned char    FlagsSetTwo;                            // +0x12e
    char             Layer;                                  // +0x12f
    long             DefIndex;                               // +0x130
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CComponent) == 0x134);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Name_PStringData) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, States) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Time) == 0x30);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Position_X) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Position_Y) == 0x38);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, TargetPosition_X) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, TargetPosition_Y) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, InitialPosition_X) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, InitialPosition_Y) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, ParentPosition_X) == 0x4c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, ParentPosition_Y) == 0x50);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RenderPosition_X) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RenderPosition_Y) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Zoom_X) == 0x5c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Zoom_Y) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, TargetZoom_X) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, TargetZoom_Y) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, InitialZoom_X) == 0x6c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, InitialZoom_Y) == 0x70);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, ParentZoom_X) == 0x74);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, ParentZoom_Y) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RenderZoom_X) == 0x7c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RenderZoom_Y) == 0x80);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Colour) == 0x84);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, TargetColour) == 0x88);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, InitialColour) == 0x8c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, ParentColour) == 0x90);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RenderColour) == 0x94);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, PositionTimeElapsed) == 0x98);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, PositionTime) == 0x9c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, ZoomTimeElapsed) == 0xa0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, ZoomTime) == 0xa4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, ColourTimeElapsed) == 0xa8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, ColourTime) == 0xac);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Children) == 0xb0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, ChildrenToDelete) == 0xbc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, PParent) == 0xc8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Size_X) == 0xcc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Size_Y) == 0xd0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Deletion) == 0xd4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, PParentViewport) == 0xdc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, ActionsIAmParamOf) == 0xe0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, ShapeChildren) == 0xe4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RenderPositionSave_X) == 0xf0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RenderPositionSave_Y) == 0xf4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RelativeRenderPosition_X) == 0xf8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RelativeRenderPosition_Y) == 0xfc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RelativeParentPosition_X) == 0x100);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RelativeParentPosition_Y) == 0x104);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RelativeRenderZoom_X) == 0x108);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RelativeRenderZoom_Y) == 0x10c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RelativeParentZoom_X) == 0x110);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, RelativeParentZoom_Y) == 0x114);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, PPositionParent) == 0x118);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, PositionChildren) == 0x11c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, PActionsOnDestruction) == 0x128);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Flags) == 0x12c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Type) == 0x12d);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, FlagsSetTwo) == 0x12e);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, Layer) == 0x12f);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CComponent, DefIndex) == 0x130);
#endif
#endif

#endif // FABLE_ENGINE_CCOMPONENT_H

// End shared header engine/_quarantine/CComponent.h
#undef CComponent
class CCharString;class CWideString;
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E
inline void* operator new(size_t, void* where) { return where; }

namespace std {
template <class T> class allocator {
public:
    allocator() throw() {}
    allocator(const allocator&)throw(){}
    template<class U>allocator(const allocator<U>&)throw(){}
    ~allocator()throw(){}
    T* allocate(size_t n,const void* = 0){return n?(T*)malloc(n*sizeof(T)):0;}
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template<class T>struct _Nonconst_traits{typedef T&reference;typedef T*pointer;};
struct _List_iterator_base{_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){}_List_iterator_base(){}};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{typedef _List_iterator<T,_Nonconst_traits<T> >iterator;_List_iterator(_List_node<T>*n):_List_iterator_base(n){}_List_iterator(){}_List_iterator(const iterator&x):_List_iterator_base(x._M_node){}};
template <class V, class Tp, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) {}
};
template <class T,class A> class _List_base {
public:
    typedef _List_node<T> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    _List_base(const A&) : _M_node(allocator<_Node>(), (_Node*)0)
    {
        _Node* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};
template <class T,class A=allocator<T> > class list : public _List_base<T,A> {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T,_Nonconst_traits<T> > iterator;
    list(const A& a = A()) : _List_base<T,A>(a) {}
    iterator end() { return this->_M_node._M_data; }
    _Node* _M_create_node(const T& x)
    {
        _Node* p = this->_M_node.allocate(1);
        new (&p->_M_data) T(x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void push_back(const T& x) { insert(end(), x); }
};
}

class C2DBoxI;class C2DBoxF;class C2DVector;class CEnginePrimitiveHandle;class CIEngine;class CRGBColour;class CFontBank;
template<class T>class CDefPointer;
namespace std{template<class T>class allocator;template<class T>struct less;template<class A,class B>struct pair;template<class T,class A>class list;template<class T,class A>class vector;template<class K,class V,class C,class A>class map;}
namespace NUISystem{class CComponent;class CAction;class CDeletion;class CUIState;class CUIDef;enum ETextAlignement{};enum EType{};
class CUIInterface{
public:virtual ~CUIInterface();
public:virtual void Update(float);
public:virtual void Draw(CIEngine&,CEnginePrimitiveHandle*,long,long *,NUISystem::CComponent*);
};
class CComponent : public CUIInterface{
public:virtual NUISystem::CComponent* FindChild(CCharString);
public:virtual NUISystem::CComponent* FindChildForward(CCharString);
public:virtual void FindAllChildren(CCharString,std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> >&);
public:virtual void FindAllChildrenOfType(NUISystem::EType,std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> >&);
public:virtual void SetPAcceptButton(NUISystem::CComponent*);
public:virtual NUISystem::CComponent* FindChildNonRecursive(CCharString);
public:virtual NUISystem::CComponent* FindFirstChildOfType(NUISystem::EType);
public:virtual bool HasChild(NUISystem::CComponent*) const;
public:virtual void SetZoom(const C2DVector&);
public:virtual C2DVector GetZoom() const;
public:virtual void SetColour(const CRGBColour&);
public:virtual CRGBColour GetColour() const;
public:virtual void SetPosition(const C2DVector&);
public:virtual C2DVector GetPosition() const;
public:virtual C2DVector GetParentPosition() const;
public:virtual void SetParentPosition(C2DVector&);
public:virtual C2DVector GetParentZoom() const;
public:virtual void SetParentZoom(C2DVector&);
public:virtual CRGBColour GetParentColour() const;
public:virtual void SetParentColour(CRGBColour&);
public:virtual C2DVector GetSize() const;
public:virtual C2DBoxF GetBoundingBox() const;
public:virtual const C2DVector& GetRenderPosition() const;
public:virtual const C2DVector& GetRenderZoom() const;
public:virtual const CRGBColour& GetRenderColour() const;
public:virtual void SetRenderPosition(C2DVector&);
public:virtual void SetRenderZoom(C2DVector&);
public:virtual void SetRenderColour(CRGBColour&);
public:virtual void ChangePosition(const C2DVector&,float,bool);
public:virtual void ChangePositionDelta(const C2DVector&,float,bool);
public:virtual void ChangeTargetPositionDelta(const C2DVector&,float,bool);
public:virtual void UpdatePosition(float);
public:virtual void ChangeZoom(const C2DVector&,float,bool);
public:virtual void ChangeZoomDelta(const C2DVector&,float,bool);
public:virtual void UpdateZoom(float);
public:virtual void ChangeColour(const CRGBColour&,float,bool);
public:virtual void ChangeColourDelta(const CRGBColour&,float,bool);
public:virtual void UpdateColour(float);
public:virtual void ResetState();
public:virtual void ForceState(unsigned long,bool);
public:virtual void Initialise();
public:virtual bool HasState(unsigned long);
public:virtual bool HasStateInChildren(unsigned long);
public:virtual void ChangeState(unsigned long);
public:virtual void ChangeState(unsigned long,float);
public:virtual void ChangeState(NUISystem::CUIState&);
public:virtual bool ChangedState();
public:virtual NUISystem::CUIState* GetCurrentState();
public:virtual void SetParent(NUISystem::CComponent*);
public:virtual NUISystem::CComponent* GetParent() const;
public:virtual std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > >& GetChildren();
public:virtual float GetUpdateTime() const;
public:virtual C2DVector GetTargetPosition() const;
public:virtual CRGBColour GetTargetColour() const;
public:virtual C2DVector GetTargetZoom() const;
public:virtual void ProcessChangeState();
public:virtual void AddChild(CCountedPointer<NUISystem::CComponent>&);
public:virtual void AddChildDynamically(CCountedPointer<NUISystem::CComponent>&);
public:virtual void AddChildFront(CCountedPointer<NUISystem::CComponent>&);
public:virtual void SetDeletion(NUISystem::CDeletion);
public:virtual NUISystem::CDeletion& GetDeletion();
public:virtual void RemoveChild(unsigned long);
public:virtual NUISystem::EType GetType() const;
public:virtual void AddAction(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnSelected(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnBack(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddPreAction(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnInactive(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnRightClicked(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnDropped(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnHovered(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnUnhovered(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction>&);
public:virtual void Die();
public:virtual std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > >& GetStates();
public:virtual void SetName(const CCharString&);
public:virtual CCharString GetName();
public:virtual NUISystem::CComponent* FindComponent(NUISystem::CComponent*);
public:virtual unsigned long GetCurrentStateIndex();
public:virtual unsigned long GetTargetStateIndex() const;
public:virtual float CanScroll(bool,C2DBoxF&);
public:virtual void FakeChildDying(NUISystem::CComponent*);
public:virtual void SetParentViewport(NUISystem::CComponent*);
public:virtual void ClearHandle();
public:virtual void MakeUnavailable();
public:virtual void MakeUnavailableDynamically();
public:virtual void MakeAvailable();
public:virtual void MakeAvailableDynamically();
public:virtual void AddActionIAmParamOf(CCountedPointer<NUISystem::CAction>&);
public:virtual NUISystem::CComponent* FindFirstChildOfTypeNonRecursive(NUISystem::EType);
public:virtual bool IsAllowedToScroll() const;
public:virtual NUISystem::CComponent* GetParentViewport();
public:virtual bool IsBastardChild() const;
public:virtual bool IsIndependent() const;
public:virtual bool IsPositionIndependent() const;
public:virtual bool IsZoomIndependent() const;
public:virtual bool IsLayerIndependent() const;
public:virtual bool IsDrawFromViewport() const;
public:virtual bool IsPositionIsCenter() const;
public:virtual long GetDefIndex() const;
public:virtual CDefPointer<NUISystem::CUIDef const > LoadAndReturnDef() const;
public:virtual void OnObserverRemove();
public:virtual C2DVector CanMove(const C2DVector&);
public:virtual C2DVector GetDynamicSize() const;
public:virtual C2DVector GetTargetRenderPosition() const;
public:virtual bool IsPCStyle() const;
public:virtual void SetRelativeParentPosition(C2DVector&);
public:virtual void SetRelativeParentZoom(C2DVector&);
public:virtual bool IsUsingRelativeZoom() const;
public:virtual bool IsUsingRelativePosition() const;
public:virtual C2DVector GetRelativeRenderPosition();
public:virtual C2DVector GetRelativeRenderZoom();
public:virtual void SetRelativeRenderPosition(C2DVector);
public:virtual C2DVector SetRelativeRenderZoom(C2DVector);
public:virtual C2DVector GetTargetRelativeRenderPosition() const;
public:virtual C2DVector GetTargetRelativeRenderZoom() const;
public:virtual void AddPositionChild(NUISystem::CComponent*);
public:virtual void SetPositionParent(NUISystem::CComponent*);
public:virtual NUISystem::CComponent* GetPositionParent();
public:virtual void PositionChildDying(NUISystem::CComponent*);
public:virtual void PositionParentDying(NUISystem::CComponent*);
public:virtual long GetRenderLayer() const;
public:virtual void AddActionOnDesctruction(CCountedPointer<NUISystem::CAction>&);
public:virtual void ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >*);
public:virtual bool IsClickable() const;
public:virtual void RemoveFromHoveredList();
private:unsigned char unreadComponentMembers[sizeof(CComponentFields)-4];
};
class CChangingStateComponent : public CComponent{
public:virtual NUISystem::CUIState* GetState(unsigned long);
public:virtual const NUISystem::CUIState* GetState(unsigned long) const;
public:virtual void UpdateStateChange();
public:virtual bool ChangedStateLastUpdate();
public:virtual bool InternalChanged();
protected:virtual bool AllChildrenChangedState();
protected:virtual void OnActivate();
protected:virtual void OnDeactivate();
protected:unsigned char _pad_changing[40];
};
// Layout: CComponent shared134, CChangingStateComponent40-byte tail gives15c. Original hover/click members follow donor with list shrink48.
class CHoverable:public CChangingStateComponent{protected:virtual bool IsHovered(C2DBoxI&)const;virtual void OnHovered();virtual void OnUnhovered();virtual void UpdateHovering();long StateBeforeHovered;bool WasHovered;std::list<CCountedPointer<CAction>,std::allocator<CCountedPointer<CAction> > >*PActionsOnHovered;std::list<CCountedPointer<CAction>,std::allocator<CCountedPointer<CAction> > >*PActionsOnUnhovered;};
class CClickable:public CHoverable{public:virtual void OnLeftClicked();virtual void OnLeftUnclicked();virtual void OnRightClicked();virtual void OnRightUnclicked();protected:long StateBeforeLeftClicked;bool WasLeftClicked;std::list<CCountedPointer<CAction> >*PActionsOnLeftClicked;std::list<CCountedPointer<CAction> >*PActionsOnLeftHeld;std::list<CCountedPointer<CAction> >*PActionsOnLeftUnclicked;long StateBeforeRightClicked;bool WasRightClicked;std::list<CCountedPointer<CAction> >*PActionsOnRightClicked;float InputDelay;float LastInputTime;};
class CActionParam;
// Layout: original CMenuEntry extends CClickable194; LastInputTime194, IsAvailable198, list pointers19c/1a0.
class CMenuEntry:public CClickable{protected:float LastInputTime;bool IsAvailable;std::list<CActionParam*>*PUIActionParams;std::list<long>*PThingActionParams;public:virtual void AddThingActionParam(long);virtual void AddUIActionParam(CActionParam*);};
void CMenuEntry::AddUIActionParam(CActionParam*param){if(!PUIActionParams)PUIActionParams=new std::list<CActionParam*>();PUIActionParams->push_back(param);}
}

#undef malloc
#undef private
#undef protected
namespace NUISystem{class CDeletion{public:enum EDeletionMethod{};EDeletionMethod Method;std::list<CComponent*>AssociatedParents;};}
static int allocations;static void*blocks[64];static size_t sizes[64];
extern "C" void*__cdecl test_malloc(size_t n){CHECK(allocations<64);void*p=malloc(n);CHECK(p!=0);blocks[allocations]=p;sizes[allocations++]=n;memset(p,0x5a,n);return p;}
NUISystem::CUIInterface::~CUIInterface(){}
void NUISystem::CUIInterface::Update(float){abort();}
void NUISystem::CUIInterface::Draw(CIEngine&,CEnginePrimitiveHandle*,long,long *,NUISystem::CComponent*){abort();}
NUISystem::CComponent* NUISystem::CComponent::FindChild(CCharString){abort();}
NUISystem::CComponent* NUISystem::CComponent::FindChildForward(CCharString){abort();}
void NUISystem::CComponent::FindAllChildren(CCharString,std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> >&){abort();}
void NUISystem::CComponent::FindAllChildrenOfType(NUISystem::EType,std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> >&){abort();}
void NUISystem::CComponent::SetPAcceptButton(NUISystem::CComponent*){abort();}
NUISystem::CComponent* NUISystem::CComponent::FindChildNonRecursive(CCharString){abort();}
NUISystem::CComponent* NUISystem::CComponent::FindFirstChildOfType(NUISystem::EType){abort();}
bool NUISystem::CComponent::HasChild(NUISystem::CComponent*) const{abort();}
void NUISystem::CComponent::SetZoom(const C2DVector&){abort();}
C2DVector NUISystem::CComponent::GetZoom() const{abort();}
void NUISystem::CComponent::SetColour(const CRGBColour&){abort();}
CRGBColour NUISystem::CComponent::GetColour() const{abort();}
void NUISystem::CComponent::SetPosition(const C2DVector&){abort();}
C2DVector NUISystem::CComponent::GetPosition() const{abort();}
C2DVector NUISystem::CComponent::GetParentPosition() const{abort();}
void NUISystem::CComponent::SetParentPosition(C2DVector&){abort();}
C2DVector NUISystem::CComponent::GetParentZoom() const{abort();}
void NUISystem::CComponent::SetParentZoom(C2DVector&){abort();}
CRGBColour NUISystem::CComponent::GetParentColour() const{abort();}
void NUISystem::CComponent::SetParentColour(CRGBColour&){abort();}
C2DVector NUISystem::CComponent::GetSize() const{abort();}
C2DBoxF NUISystem::CComponent::GetBoundingBox() const{abort();}
const C2DVector& NUISystem::CComponent::GetRenderPosition() const{abort();}
const C2DVector& NUISystem::CComponent::GetRenderZoom() const{abort();}
const CRGBColour& NUISystem::CComponent::GetRenderColour() const{abort();}
void NUISystem::CComponent::SetRenderPosition(C2DVector&){abort();}
void NUISystem::CComponent::SetRenderZoom(C2DVector&){abort();}
void NUISystem::CComponent::SetRenderColour(CRGBColour&){abort();}
void NUISystem::CComponent::ChangePosition(const C2DVector&,float,bool){abort();}
void NUISystem::CComponent::ChangePositionDelta(const C2DVector&,float,bool){abort();}
void NUISystem::CComponent::ChangeTargetPositionDelta(const C2DVector&,float,bool){abort();}
void NUISystem::CComponent::UpdatePosition(float){abort();}
void NUISystem::CComponent::ChangeZoom(const C2DVector&,float,bool){abort();}
void NUISystem::CComponent::ChangeZoomDelta(const C2DVector&,float,bool){abort();}
void NUISystem::CComponent::UpdateZoom(float){abort();}
void NUISystem::CComponent::ChangeColour(const CRGBColour&,float,bool){abort();}
void NUISystem::CComponent::ChangeColourDelta(const CRGBColour&,float,bool){abort();}
void NUISystem::CComponent::UpdateColour(float){abort();}
void NUISystem::CComponent::ResetState(){abort();}
void NUISystem::CComponent::ForceState(unsigned long,bool){abort();}
void NUISystem::CComponent::Initialise(){abort();}
bool NUISystem::CComponent::HasState(unsigned long){abort();}
bool NUISystem::CComponent::HasStateInChildren(unsigned long){abort();}
void NUISystem::CComponent::ChangeState(NUISystem::CUIState&){abort();}
void NUISystem::CComponent::ChangeState(unsigned long,float){abort();}
void NUISystem::CComponent::ChangeState(unsigned long){abort();}
bool NUISystem::CComponent::ChangedState(){abort();}
NUISystem::CUIState* NUISystem::CComponent::GetCurrentState(){abort();}
void NUISystem::CComponent::SetParent(NUISystem::CComponent*){abort();}
NUISystem::CComponent* NUISystem::CComponent::GetParent() const{abort();}
std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > >& NUISystem::CComponent::GetChildren(){abort();}
float NUISystem::CComponent::GetUpdateTime() const{abort();}
C2DVector NUISystem::CComponent::GetTargetPosition() const{abort();}
CRGBColour NUISystem::CComponent::GetTargetColour() const{abort();}
C2DVector NUISystem::CComponent::GetTargetZoom() const{abort();}
void NUISystem::CComponent::ProcessChangeState(){abort();}
void NUISystem::CComponent::AddChild(CCountedPointer<NUISystem::CComponent>&){abort();}
void NUISystem::CComponent::AddChildDynamically(CCountedPointer<NUISystem::CComponent>&){abort();}
void NUISystem::CComponent::AddChildFront(CCountedPointer<NUISystem::CComponent>&){abort();}
void NUISystem::CComponent::SetDeletion(NUISystem::CDeletion){abort();}
NUISystem::CDeletion& NUISystem::CComponent::GetDeletion(){abort();}
void NUISystem::CComponent::RemoveChild(unsigned long){abort();}
NUISystem::EType NUISystem::CComponent::GetType() const{abort();}
void NUISystem::CComponent::AddAction(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddActionOnSelected(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddActionOnBack(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddPreAction(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddActionOnInactive(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddActionOnRightClicked(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddActionOnDropped(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddActionOnHovered(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddActionOnUnhovered(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::Die(){abort();}
std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > >& NUISystem::CComponent::GetStates(){abort();}
void NUISystem::CComponent::SetName(const CCharString&){abort();}
CCharString NUISystem::CComponent::GetName(){abort();}
NUISystem::CComponent* NUISystem::CComponent::FindComponent(NUISystem::CComponent*){abort();}
unsigned long NUISystem::CComponent::GetCurrentStateIndex(){abort();}
unsigned long NUISystem::CComponent::GetTargetStateIndex() const{abort();}
float NUISystem::CComponent::CanScroll(bool,C2DBoxF&){abort();}
void NUISystem::CComponent::FakeChildDying(NUISystem::CComponent*){abort();}
void NUISystem::CComponent::SetParentViewport(NUISystem::CComponent*){abort();}
void NUISystem::CComponent::ClearHandle(){abort();}
void NUISystem::CComponent::MakeUnavailable(){abort();}
void NUISystem::CComponent::MakeUnavailableDynamically(){abort();}
void NUISystem::CComponent::MakeAvailable(){abort();}
void NUISystem::CComponent::MakeAvailableDynamically(){abort();}
void NUISystem::CComponent::AddActionIAmParamOf(CCountedPointer<NUISystem::CAction>&){abort();}
NUISystem::CComponent* NUISystem::CComponent::FindFirstChildOfTypeNonRecursive(NUISystem::EType){abort();}
bool NUISystem::CComponent::IsAllowedToScroll() const{abort();}
NUISystem::CComponent* NUISystem::CComponent::GetParentViewport(){abort();}
bool NUISystem::CComponent::IsBastardChild() const{abort();}
bool NUISystem::CComponent::IsIndependent() const{abort();}
bool NUISystem::CComponent::IsPositionIndependent() const{abort();}
bool NUISystem::CComponent::IsZoomIndependent() const{abort();}
bool NUISystem::CComponent::IsLayerIndependent() const{abort();}
bool NUISystem::CComponent::IsDrawFromViewport() const{abort();}
bool NUISystem::CComponent::IsPositionIsCenter() const{abort();}
long NUISystem::CComponent::GetDefIndex() const{abort();}
CDefPointer<NUISystem::CUIDef const > NUISystem::CComponent::LoadAndReturnDef() const{abort();}
void NUISystem::CComponent::OnObserverRemove(){abort();}
C2DVector NUISystem::CComponent::CanMove(const C2DVector&){abort();}
C2DVector NUISystem::CComponent::GetDynamicSize() const{abort();}
C2DVector NUISystem::CComponent::GetTargetRenderPosition() const{abort();}
bool NUISystem::CComponent::IsPCStyle() const{abort();}
void NUISystem::CComponent::SetRelativeParentPosition(C2DVector&){abort();}
void NUISystem::CComponent::SetRelativeParentZoom(C2DVector&){abort();}
bool NUISystem::CComponent::IsUsingRelativeZoom() const{abort();}
bool NUISystem::CComponent::IsUsingRelativePosition() const{abort();}
C2DVector NUISystem::CComponent::GetRelativeRenderPosition(){abort();}
C2DVector NUISystem::CComponent::GetRelativeRenderZoom(){abort();}
void NUISystem::CComponent::SetRelativeRenderPosition(C2DVector){abort();}
C2DVector NUISystem::CComponent::SetRelativeRenderZoom(C2DVector){abort();}
C2DVector NUISystem::CComponent::GetTargetRelativeRenderPosition() const{abort();}
C2DVector NUISystem::CComponent::GetTargetRelativeRenderZoom() const{abort();}
void NUISystem::CComponent::AddPositionChild(NUISystem::CComponent*){abort();}
void NUISystem::CComponent::SetPositionParent(NUISystem::CComponent*){abort();}
NUISystem::CComponent* NUISystem::CComponent::GetPositionParent(){abort();}
void NUISystem::CComponent::PositionChildDying(NUISystem::CComponent*){abort();}
void NUISystem::CComponent::PositionParentDying(NUISystem::CComponent*){abort();}
long NUISystem::CComponent::GetRenderLayer() const{abort();}
void NUISystem::CComponent::AddActionOnDesctruction(CCountedPointer<NUISystem::CAction>&){abort();}
void NUISystem::CComponent::ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >*){abort();}
bool NUISystem::CComponent::IsClickable() const{abort();}
void NUISystem::CComponent::RemoveFromHoveredList(){abort();}
const NUISystem::CUIState* NUISystem::CChangingStateComponent::GetState(unsigned long) const{abort();}
NUISystem::CUIState* NUISystem::CChangingStateComponent::GetState(unsigned long){abort();}
void NUISystem::CChangingStateComponent::UpdateStateChange(){abort();}
bool NUISystem::CChangingStateComponent::ChangedStateLastUpdate(){abort();}
bool NUISystem::CChangingStateComponent::InternalChanged(){abort();}
bool NUISystem::CChangingStateComponent::AllChildrenChangedState(){abort();}
void NUISystem::CChangingStateComponent::OnActivate(){abort();}
void NUISystem::CChangingStateComponent::OnDeactivate(){abort();}
bool NUISystem::CHoverable::IsHovered(C2DBoxI&) const{abort();}
void NUISystem::CHoverable::OnHovered(){abort();}
void NUISystem::CHoverable::OnUnhovered(){abort();}
void NUISystem::CHoverable::UpdateHovering(){abort();}
void NUISystem::CClickable::OnLeftClicked(){abort();}
void NUISystem::CClickable::OnLeftUnclicked(){abort();}
void NUISystem::CClickable::OnRightClicked(){abort();}
void NUISystem::CClickable::OnRightUnclicked(){abort();}

void NUISystem::CMenuEntry::AddThingActionParam(long){abort();}
int main(){int tokens[8];NUISystem::CActionParam*values[9];values[0]=0;for(int i=0;i<8;++i){tokens[i]=100+i;values[i+1]=reinterpret_cast<NUISystem::CActionParam*>(&tokens[i]);}int cases=0;
 for(int initial=0;initial<5;++initial)for(int count=1;count<5;++count)for(int seed=0;seed<9;++seed)for(int mode=0;mode<2;++mode){
  NUISystem::CMenuEntry entry;entry.PUIActionParams=0;entry.PThingActionParams=0;allocations=0;NUISystem::CActionParam*expected[12];int expectedCount=0;
  if(initial){entry.PUIActionParams=new std::list<NUISystem::CActionParam*>;for(int i=0;i<initial;++i){NUISystem::CActionParam*x=values[(seed+i)%9];entry.PUIActionParams->push_back(x);expected[expectedCount++]=x;}}
  std::list<NUISystem::CActionParam*>*original=entry.PUIActionParams;int before=allocations;
  for(int i=0;i<count;++i){NUISystem::CActionParam*x=values[(seed+(mode?count-i:i))%9];entry.AddUIActionParam(x);expected[expectedCount++]=x;CHECK(entry.PUIActionParams!=0&&entry.PThingActionParams==0);if(original)CHECK(entry.PUIActionParams==original);original=entry.PUIActionParams;
   std::_List_node_base*sentinel=entry.PUIActionParams->_M_node._M_data;std::_List_node_base*n=sentinel->_M_next;std::_List_node_base*previous=sentinel;
   for(int j=0;j<expectedCount;++j){CHECK(n!=sentinel&&n->_M_prev==previous);CHECK(static_cast<std::_List_node<NUISystem::CActionParam*>*>(n)->_M_data==expected[j]);previous=n;n=n->_M_next;}CHECK(n==sentinel&&sentinel->_M_prev==previous);
   n=sentinel->_M_prev;for(int j=expectedCount-1;j>=0;--j){CHECK(static_cast<std::_List_node<NUISystem::CActionParam*>*>(n)->_M_data==expected[j]);n=n->_M_prev;}CHECK(n==sentinel);
  }
  CHECK(allocations-before==count+(initial?0:1));for(int j=0;j<allocations;++j){CHECK(sizes[j]==sizeof(std::_List_node<NUISystem::CActionParam*>));free(blocks[j]);}delete entry.PUIActionParams;++cases;
 }CHECK(cases==360);for(int i=0;i<8;++i)CHECK(tokens[i]==100+i);puts("0053dd50_TEST PASS");return 0;}