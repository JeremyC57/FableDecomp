// Self-contained target and read-only GetDef context, with observable engine fixtures.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __int64 int __attribute__((mode(DI)))
#endif
#define private public
#define protected public
// 0078c000 CTCBerserk::GetNumFramesBeforeCrazyIdle
// Leaf: ?GetNumFramesBeforeCrazyIdle@CTCBerserk@@SIJXZ
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
// Begin shared header engine/_quarantine/CSpecialAbilitiesBerserkDef.h
#ifndef FABLE_ENGINE_CSPECIALABILITIESBERSERKDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESBERSERKDEF_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CSpecialAbilitiesBerserkDef = 0x1d4.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CSpecialAbilitiesBerserkDef = 0x188.
// Retail evidence: 1 trusted landed function(s); reconciliation 1 conflict(s) in 1 file(s) vs 1 agreeing.
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
class CSpecialAbilitiesBerserkDef {
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
    float         SpeedMultiplier;                           // +0x3c
    long          FramesToFadeOut;                           // +0x40
    float         LowerBerserkPercent;                       // +0x44
    float         TimeForThemeTransition;                    // +0x48
    float         BlurCenterFromFloor;                       // +0x4c
    float         BerserkThreshold;                          // +0x50
    float         BerserkLength;                             // +0x54
    float         BerserkIntensity;                          // +0x58
    float         BerserkFade;                               // +0x5c
    float         BerserkRange;                              // +0x60
    float         BerserkTwist;                              // +0x64
    float         TimeForRadialBlurToFadeOut;                // +0x68
    float         CameraStartDist;                           // +0x6c
    float         CameraEndDist;                             // +0x70
    float         CameraStartDeltaHeight;                    // +0x74
    float         CameraEndDeltaHeight;                      // +0x78
    unsigned char BerserkDuration[0xc];                      // +0x7c vector<float,std::allocator<float>_>
    unsigned char BerserkDamageToEnemyMultiplier[0xc];       // +0x88 vector<float,std::allocator<float>_>
    float         BerserkDamageToEnemyMultiplierAdditionPerHit; // +0x94
    unsigned char BerserkDamageToMeMultiplier[0xc];          // +0x98 vector<float,std::allocator<float>_>
    unsigned char MaxIncreaseInStrength[0xc];                // +0xa4 vector<float,std::allocator<float>_>
    unsigned char BerserkScale[0xc];                         // +0xb0 vector<float,std::allocator<float>_>
    float         BerserkBlastRadius;                        // +0xbc
    unsigned char CamDist[0xc];                              // +0xc0 vector<float,std::allocator<float>_>
    unsigned char CamHeight[0xc];                            // +0xcc vector<float,std::allocator<float>_>
    unsigned char CamFOV[0xc];                               // +0xd8 vector<float,std::allocator<float>_>
    unsigned char CamSwitchTime[0xc];                        // +0xe4 vector<float,std::allocator<float>_>
    unsigned char RumbleLevel[0xc];                          // +0xf0 vector<float,std::allocator<float>_>
    unsigned char RumbleSmoothness[0xc];                     // +0xfc vector<float,std::allocator<float>_>
    unsigned char RumbleAttack[0xc];                         // +0x108 vector<float,std::allocator<float>_>
    unsigned char RumbleDecay[0xc];                          // +0x114 vector<float,std::allocator<float>_>
    float         RumbleReleaseLevelDelta;                   // +0x120
    float         RumbleReleaseSmoothnessDelta;              // +0x124
    float         RumbleReleaseAttackDelta;                  // +0x128
    float         RumbleReleaseDecayDelta;                   // +0x12c
    float         CrazyActionRadius;                         // +0x130
    float         CrazyActionDamage;                         // +0x134
    unsigned char DecapitateMaxProb[0xc];                    // +0x138 vector<float,std::allocator<float>_>
    float         TargetNearestEnemyDistance;                // +0x144
    long          WeaponTrailAttack;                         // +0x148
    long          WeaponTrailKnockDown;                      // +0x14c
    unsigned char LevelUpTimings[0xc];                       // +0x150 vector<float,std::allocator<float>_>
    unsigned char TintColorR[0xc];                           // +0x15c vector<long,std::allocator<long>_>
    unsigned char TintColorG[0xc];                           // +0x168 vector<long,std::allocator<long>_>
    unsigned char TintColorB[0xc];                           // +0x174 vector<long,std::allocator<long>_>
    bool          CanBlockWhileBerserked;                    // +0x180
    unsigned char _pad_0x181[0x3];                           // +0x181
    float         CrazyIdleIntervalSecs;                     // +0x184
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CSpecialAbilitiesBerserkDef) == 0x188);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, SpeedMultiplier) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, FramesToFadeOut) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, LowerBerserkPercent) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, TimeForThemeTransition) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, BlurCenterFromFloor) == 0x4c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, BerserkThreshold) == 0x50);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, BerserkLength) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, BerserkIntensity) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, BerserkFade) == 0x5c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, BerserkRange) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, BerserkTwist) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, TimeForRadialBlurToFadeOut) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, CameraStartDist) == 0x6c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, CameraEndDist) == 0x70);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, CameraStartDeltaHeight) == 0x74);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, CameraEndDeltaHeight) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, BerserkDuration) == 0x7c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, BerserkDamageToEnemyMultiplier) == 0x88);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, BerserkDamageToEnemyMultiplierAdditionPerHit) == 0x94);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, BerserkDamageToMeMultiplier) == 0x98);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, MaxIncreaseInStrength) == 0xa4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, BerserkScale) == 0xb0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, BerserkBlastRadius) == 0xbc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, CamDist) == 0xc0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, CamHeight) == 0xcc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, CamFOV) == 0xd8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, CamSwitchTime) == 0xe4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, RumbleLevel) == 0xf0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, RumbleSmoothness) == 0xfc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, RumbleAttack) == 0x108);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, RumbleDecay) == 0x114);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, RumbleReleaseLevelDelta) == 0x120);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, RumbleReleaseSmoothnessDelta) == 0x124);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, RumbleReleaseAttackDelta) == 0x128);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, RumbleReleaseDecayDelta) == 0x12c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, CrazyActionRadius) == 0x130);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, CrazyActionDamage) == 0x134);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, DecapitateMaxProb) == 0x138);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, TargetNearestEnemyDistance) == 0x144);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, WeaponTrailAttack) == 0x148);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, WeaponTrailKnockDown) == 0x14c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, LevelUpTimings) == 0x150);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, TintColorR) == 0x15c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, TintColorG) == 0x168);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, TintColorB) == 0x174);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, CanBlockWhileBerserked) == 0x180);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesBerserkDef, CrazyIdleIntervalSecs) == 0x184);
#endif
#endif

#endif // FABLE_ENGINE_CSPECIALABILITIESBERSERKDEF_H

// End shared header engine/_quarantine/CSpecialAbilitiesBerserkDef.h
#define CDefinitionManager CDefinitionManagerLayout
// Begin shared header engine/CDefinitionManager.h
#ifndef FABLE_ENGINE_CDEFINITIONMANAGER_H
#define FABLE_ENGINE_CDEFINITIONMANAGER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CDefinitionManager = 0xf0.
// Retail layout: 12-byte STLport vector/map, 0x10 CVectorMap, 0xc CArray (landed dtor 0x009B02E0) -> sizeof 0xd0.
// Retail evidence: 4 trusted landed function(s); reconciliation clean.
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
class CCharStringData;
class CSymbolMap;
class CWideStringData;
struct _func___cdecl_void_CCharString_ptr;

#pragma pack(push, 1)
class CDefinitionManager {
public:
    void*                               __vftable;           // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char                       _pad_0x04[0x4];      // +0x04
    unsigned char                       _pad_0x08[0x4];      // +0x08
    unsigned char                       _pad_0x0c[0x4];      // +0x0c
    unsigned char                       _pad_0x10[0x4];      // +0x10
    unsigned char                       _pad_0x14[0x4];      // +0x14
    void*                               m_classArray;        // +0x18 retail-only const CDefClassBase** (CDefinitionManager_GetPDefFromClassIndex_00c96430.cpp)
    unsigned char                       _pad_0x1c[0x4];      // +0x1c
    unsigned char                       _pad_0x20[0x4];      // +0x20
    unsigned char                       _pad_0x24[0x4];      // +0x24
    unsigned char                       _pad_0x28[0x4];      // +0x28
    unsigned char                       _pad_0x2c[0x4];      // +0x2c
    unsigned char                       _pad_0x30[0x4];      // +0x30
    unsigned char                       _pad_0x34[0x4];      // +0x34
    unsigned char                       _pad_0x38[0x4];      // +0x38
    void*                               PDefTypeDebugInfoMap; // +0x3c map<CCharString,CDefinitionManager::CDefTypeDebugInfo,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CDefinitionManager::CDefTypeDebugInfo>_>_> *
    unsigned char                       SymbolPathList[0xc]; // +0x40 vector<CWideString,std::allocator<CWideString>_> (retail 12-byte STLport vector)
    unsigned char                       CompilePathList[0xc]; // +0x4c vector<CWideString,std::allocator<CWideString>_>
    CSymbolMap*                         PSymbolMap_Data;     // +0x58 CCountedPointer<CSymbolMap>::Data
    CCPPointerInfo*                     PSymbolMap_Info;     // +0x5c CCountedPointer<CSymbolMap>::Info
    CSymbolMap*                         PDefinitionSymbolMap_Data; // +0x60 CCountedPointer<CSymbolMap>::Data
    CCPPointerInfo*                     PDefinitionSymbolMap_Info; // +0x64 CCountedPointer<CSymbolMap>::Info
    unsigned char                       NameCRCToIndexMap[0x10]; // +0x68 CVectorMap<unsigned_long,long,CKeyPairCompareLess<unsigned_long,long>_>
    unsigned char                       DefClassInfo[0x10];  // +0x78 CVectorMap<unsigned_long,CDefinitionManager::CDefClassInfo,CKeyPairCompareLess<unsigned_long,CDefinitionManager::CDefClassInfo>_>
    unsigned char                       CompressedDefData[0xc]; // +0x88 CArray<unsigned_char>
    unsigned char                       CompressedOffsets[0x10]; // +0x94 CVectorMap<unsigned_long,unsigned_long,CKeyPairCompareLess<unsigned_long,unsigned_long>_>
    unsigned char                       InstantiatedDefs[0xc]; // +0xa4 CArray<CDefinitionManager::CInstantiatedDefInfo_*>
    _func___cdecl_void_CCharString_ptr* ErrorCallback;       // +0xb0
    CCharStringData*                    CurrentFilename_PStringData; // +0xb4 CCharString::PStringData
    CWideStringData*                    CompiledFileName_PStringData; // +0xb8 CWideString::PStringData
    unsigned long                       DependencyCheckCRC;  // +0xbc
    bool                                UseSafeBinaryFormat; // +0xc0
    unsigned char                       _pad_0xc1[0x1];      // +0xc1
    unsigned char                       _pad_0xc2[0x2];      // +0xc2
    unsigned char                       RecompiledDefs[0xc]; // +0xc4 map<unsigned_long,CDefPointer<CDefClassBase>,std::less<unsigned_long>,std::allocator<std::pair<unsigned_long_const_,CDefPointer<CDefClassBase>_>_>_> (retail 12-byte STLport map)
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDefinitionManager) == 0xd0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, m_classArray) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PDefTypeDebugInfoMap) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, SymbolPathList) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CompilePathList) == 0x4c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PSymbolMap_Data) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PSymbolMap_Info) == 0x5c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PDefinitionSymbolMap_Data) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PDefinitionSymbolMap_Info) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, NameCRCToIndexMap) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, DefClassInfo) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CompressedDefData) == 0x88);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CompressedOffsets) == 0x94);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, InstantiatedDefs) == 0xa4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, ErrorCallback) == 0xb0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CurrentFilename_PStringData) == 0xb4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CompiledFileName_PStringData) == 0xb8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, DependencyCheckCRC) == 0xbc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, UseSafeBinaryFormat) == 0xc0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, RecompiledDefs) == 0xc4);
#endif
#endif

#endif // FABLE_ENGINE_CDEFINITIONMANAGER_H

// End shared header engine/CDefinitionManager.h
#undef CDefinitionManager
#define CTCBerserk CTCBerserkLayout
// Begin shared header engine/CTCBerserk.h
#ifndef FABLE_ENGINE_CTCBERSERK_H
#define FABLE_ENGINE_CTCBERSERK_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCBerserk = 0x78.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CTCBerserk = 0x74.
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CDefPointeeBase;

#pragma pack(push, 1)
class CTCBerserk {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    long             Timer;                                  // +0x0c
    long             DarkenEnvTimer;                         // +0x10
    float            StrengthPercentage;                     // +0x14
    float            MaxIncreaseInStrength;                  // +0x18
    float            ReleasedAnimationTimer;                 // +0x1c
    long             AnimTimer;                              // +0x20
    float            BerserkDamageToMe;                      // +0x24
    float            BerserkDamageToEnemy;                   // +0x28
    float            BerserkScale;                           // +0x2c
    float            RumbleLevel;                            // +0x30
    float            RumbleSmoothness;                       // +0x34
    float            RumbleAttack;                           // +0x38
    float            RumbleDecay;                            // +0x3c
    unsigned long    BerserkingLoopingSound;                 // +0x40
    long             SpellLevel;                             // +0x44
    bool             JustReleased;                           // +0x48
    bool             AbilityInitialised;                     // +0x49
    unsigned char    _pad_0x4a[0x2];                         // +0x4a
    unsigned char    DependentThings[0xc];                   // +0x4c vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    StartUpEffect[0x8];                     // +0x58 CIntelligentPointer<CThing>
    unsigned char    BerserkBodyEffect[0x8];                 // +0x60 CIntelligentPointer<CThing>
    CDefPointeeBase* PDef_Object;                            // +0x68 CDefPointer<CSpecialAbilitiesBerserkDef_const_>::Object
    void*            RadialBlurFadeKey;                      // +0x6c
    bool             Paused;                                 // +0x70
    bool             Pausable;                               // +0x71
    bool             IsInCutscene;                           // +0x72
    unsigned char    _pad_0x73[0x1];                         // +0x73
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CTCBerserk) == 0x74);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, Timer) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, DarkenEnvTimer) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, StrengthPercentage) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, MaxIncreaseInStrength) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, ReleasedAnimationTimer) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, AnimTimer) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, BerserkDamageToMe) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, BerserkDamageToEnemy) == 0x28);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, BerserkScale) == 0x2c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, RumbleLevel) == 0x30);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, RumbleSmoothness) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, RumbleAttack) == 0x38);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, RumbleDecay) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, BerserkingLoopingSound) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, SpellLevel) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, JustReleased) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, AbilityInitialised) == 0x49);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, DependentThings) == 0x4c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, StartUpEffect) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, BerserkBodyEffect) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, PDef_Object) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, RadialBlurFadeKey) == 0x6c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, Paused) == 0x70);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, Pausable) == 0x71);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBerserk, IsInCutscene) == 0x72);
#endif
#endif

#endif // FABLE_ENGINE_CTCBERSERK_H

// End shared header engine/CTCBerserk.h
#undef CTCBerserk
class CCharStringData;
class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CMainGameComponent{protected:static long ConstantFPS;public:static long __fastcall GetConstantFPS(){return ConstantFPS;}};
// Layout: original shared manager projection, with original template methods restored.
class CDefinitionManager:public CDefinitionManagerLayout{public:
 long GetGlobalIndexFromClassIndex(const CCharString&,long)const;
 CDefPointer<const CDefClassBase>GetPDefFromGlobalIndex(long)const;
 template<class T>bool GetDef(long,CDefPointer<const T>*)const;
};
class CGameDefinitionManager:public CDefinitionManager{public:static CGameDefinitionManager&__fastcall Get();};
class CTCBerserk:public CTCBerserkLayout{public:static long __fastcall GetNumFramesBeforeCrazyIdle();};
template<class T>__declspec(noinline)bool CDefinitionManager::GetDef(long index,CDefPointer<const T>*out)const{
 if(index<=0)return false;
 CDefPointer<const CDefClassBase>value=GetPDefFromGlobalIndex(index);
 if(value.IsNull())return false;
 out->SetDynamicCast(value);return true;
}
long __fastcall CTCBerserk::GetNumFramesBeforeCrazyIdle(){
 long index=CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_BERSERK_DEF",1);
 {
  CDefPointer<const CSpecialAbilitiesBerserkDef>def;
  CGameDefinitionManager::Get().GetDef(index,&def);
  // Layout: shared def is flattened; the original pointee base begins at zero.
  return (long)(CMainGameComponent::GetConstantFPS()*reinterpret_cast<const CSpecialAbilitiesBerserkDef*>(def.InternalGetObject())->CrazyIdleIntervalSecs);
 }
}

#undef private
#undef protected
static unsigned deletes,stringsMade,stringsFreed,resolves,lookups;
void CIVCountedPointeeBase::Delete()const{if(IVRefCount!=0)abort();++deletes;}
CResource::CResource(){IVRefCount=0;ResourceList=0;PrevResource=NextResource=0;ResourceSize=LastUsedFrame=0;}
CResource::~CResource(){}
bool CResource::UnloadResource(){return false;}unsigned long CResource::GetUnloadableMemory()const{return 0;}bool CResource::IsResourceLoaded()const{return true;}void CResource::OnResourceLoaded(){}bool CResource::CanAbortResourceLoad(long)const{return false;}void CResource::OnResourceLoadAborted(){}unsigned long CResource::GetDebugMemoryInUse()const{return 0;}void CResource::ResetDebugMemoryInUse(){}
// Representation fixture: real refcount/vtable base plus room for the shared flattened def's field offset.
class TestDef:public CDefPointeeBase{public:unsigned char bytes[sizeof(CSpecialAbilitiesBerserkDef)];};
static TestDef data;
static CGameDefinitionManager manager;
long CMainGameComponent::ConstantFPS=0;
CGameDefinitionManager&__fastcall CGameDefinitionManager::Get(){return manager;}
CCharString::CCharString(const char*p,long length){if(length!=-1)abort();PStringData=reinterpret_cast<CCharStringData*>(const_cast<char*>(p));++stringsMade;}
CCharString::~CCharString(){++stringsFreed;}
long CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&name,long index)const{if(this!=&manager||index!=1||strcmp(reinterpret_cast<const char*>(name.PStringData),"SPECIAL_ABILITIES_BERSERK_DEF")||stringsMade!=stringsFreed+1)abort();++resolves;return 73;}
CDefPointer<const CDefClassBase>CDefinitionManager::GetPDefFromGlobalIndex(long index)const{if(this!=&manager||index!=73||stringsMade!=stringsFreed)abort();++lookups;CDefPointer<const CDefClassBase>r;r.Object=&data;data.AddRef();return r;}
int main(){
 const long rates[]={0,1,2,24,30,60,-4};const float times[]={0.0f,0.25f,0.5f,1.0f,2.0f,3.5f,-1.25f};
 unsigned cases=0;
 for(unsigned held=0;held<6;++held)for(unsigned i=0;i<sizeof rates/sizeof rates[0];++i)for(unsigned j=0;j<sizeof times/sizeof times[0];++j){
  data.IVRefCount=held;CMainGameComponent::ConstantFPS=rates[i];float value=times[j];
  memcpy(reinterpret_cast<unsigned char*>(&data)+offsetof(CSpecialAbilitiesBerserkDef,CrazyIdleIntervalSecs),&value,sizeof value);
  unsigned before=deletes;
  long answer=CTCBerserk::GetNumFramesBeforeCrazyIdle();
  if(answer!=(long)(rates[i]*value)||data.IVRefCount!=held||deletes!=before+(held==0?1:0))return 1;
  ++cases;if(stringsMade!=cases||stringsFreed!=cases||resolves!=cases||lookups!=cases)return 2;
 }
 puts("0078c000_TEST PASS");return 0;
}