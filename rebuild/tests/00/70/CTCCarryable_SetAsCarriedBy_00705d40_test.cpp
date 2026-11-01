#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __int64 long
#define TEST_BITSET_SIZE (9*sizeof(unsigned long))
#define TEST_COMPONENT_SIZE (3*sizeof(void*)+8)
#define THISCALL_ARGS(T) const T*self
#else
#define TEST_BITSET_SIZE 36
#define TEST_COMPONENT_SIZE 16
#define THISCALL_ARGS(T) const T*self,void*
#endif
#define private public
#define protected public
// 00705d40 CTCCarryable::SetAsCarriedBy
// Leaf: ?SetAsCarriedBy@CTCCarryable@@AAEXPAVCTCCarrying@@@Z
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
// Begin shared header engine/CBaseClassNonCopyable.h
#ifndef FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
// Retail layout (hand-written; no members). RTTI CBaseClassNonCopyable : CBaseClass; vtable 0x0129A7C4, slot 0 scalar
// deleting dtor 0x0099A470. Ego_r.pdb / FableWin.pdb: ??0CBaseClassNonCopyable@@IAE@XZ (protected),
// ??1CBaseClassNonCopyable@@MAE@XZ (protected virtual). The copy operations are private and undefined (non-copyable).
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

class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    // slot 0
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseClassNonCopyable) == 0x4);
#endif
#endif

#endif // FABLE_ENGINE_CBASECLASSNONCOPYABLE_H

// End shared header engine/CBaseClassNonCopyable.h
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
// Begin shared header engine/CSubDefClassBase.h
#ifndef FABLE_ENGINE_CSUBDEFCLASSBASE_H
#define FABLE_ENGINE_CSUBDEFCLASSBASE_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration). RTTI .?AVCSubDefClassBase@@ : CDefClassBase
// (ghidra_out/rtti_bases.tsv); vtable 0x01231D54 = CDefClassBase's 25 slots with only slot 0 replaced (scalar deleting dtor
// 0x0044F620; slots 1..24 identical to 0x01230BCC, 18..20 _purecall). No members of its own: 0x25 bytes like its packed base
// (Ego_r.pdb 0x25; see engine/CDefClassBase.h for the pack(1) evidence). Ctor 0x0044C0C0
// (??0CSubDefClassBase@@QAE@XZ, bound in rebuild/oracles/reloc_bindings.tsv). FableWin.pdb ??1CSubDefClassBase@@UAE@XZ is a
// user-declared virtual dtor; the landed derived scalar-deleting rows expand it inline, so it is defined inline here.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CSubDefClassBase.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
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
    CDefClassBase();CDefClassBase(const CDefClassBase&);                                 // 0x0046644C
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

class CSubDefClassBase : public CDefClassBase {
public:
    CSubDefClassBase();                                                  // 0x0044C0C0
    virtual ~CSubDefClassBase() { }                                      // slot 0  0x0044F620 (scalar deleting; inline dtor)
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CSubDefClassBase) == 0x25);
#endif
#endif

#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CSUBDEFCLASSBASE_H

// End shared header engine/CSubDefClassBase.h
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
    CDefClassBase();CDefClassBase(const CDefClassBase&);                                 // 0x0046644C
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
    unsigned char         UsingComponents[TEST_BITSET_SIZE];             // +0x20 bitset<274>
    unsigned char         Components[TEST_COMPONENT_SIZE];                  // +0x44 CVectorMap<ETCInterfaceType,CTCBase_*,CKeyPairCompareLess<ETCInterfaceType,CTCBase_*>_>
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
#define CTCBase CTCBaseFields
// Begin shared header engine/CTCBase.h
#ifndef FABLE_ENGINE_CTCBASE_H
#define FABLE_ENGINE_CTCBASE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCBase = 0xc.
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
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CTCBase) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBase, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBase, ParentThing) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBase, ToBeRemoved) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCBase, IsInGlobalUpdateTCs) == 0x9);
#endif
#endif

#endif // FABLE_ENGINE_CTCBASE_H

// End shared header engine/CTCBase.h
#undef CTCBase
#define CTCPhysicsBase CTCPhysicsBaseFields
// Begin shared header engine/CTCPhysicsBase.h
#ifndef FABLE_ENGINE_CTCPHYSICSBASE_H
#define FABLE_ENGINE_CTCPHYSICSBASE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCPhysicsBase = 0x40.
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
class CTCPhysicsBase {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    long             sub;                                    // +0x04 retail-only long (previous header)
    unsigned char    _pad_0x08[0x4];                         // +0x08
    float            Position_X;                             // +0x0c C3DVector::X
    float            Position_Y;                             // +0x10 C3DVector::Y
    float            Position_Z;                             // +0x14 C3DVector::Z
    float            OldPosition_X;                          // +0x18 C3DVector::X
    float            OldPosition_Y;                          // +0x1c C3DVector::Y
    float            OldPosition_Z;                          // +0x20 C3DVector::Z
    long             OldPositionLastFrameSet;                // +0x24
    float            Velocity_X;                             // +0x28 C3DVector::X
    float            Velocity_Y;                             // +0x2c C3DVector::Y
    float            Velocity_Z;                             // +0x30 C3DVector::Z
    float            Radius;                                 // +0x34
    CDefPointeeBase* PhysicsDef_Object;                      // +0x38 CDefPointer<CPhysicsDef_const_>::Object
    unsigned char    OldMoving[0x4];                         // +0x3c bitfield group: bool:1
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CTCPhysicsBase) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, sub) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Position_X) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Position_Y) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Position_Z) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPosition_X) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPosition_Y) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPosition_Z) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldPositionLastFrameSet) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Velocity_X) == 0x28);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Velocity_Y) == 0x2c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Velocity_Z) == 0x30);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, Radius) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, PhysicsDef_Object) == 0x38);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase, OldMoving) == 0x3c);
#endif
#endif

#endif // FABLE_ENGINE_CTCPHYSICSBASE_H

// End shared header engine/CTCPhysicsBase.h
#undef CTCPhysicsBase
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

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
#define CCarryableDef CCarryableDefFields
// Begin shared header engine/CCarryableDef.h
#ifndef FABLE_ENGINE_CCARRYABLEDEF_H
#define FABLE_ENGINE_CCARRYABLEDEF_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CCarryableDef = 0x50.
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

#pragma pack(push, 1)
class CCarryableDef {
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
    long          ActiveCarrySlot;                           // +0x28
    long          SecondaryActiveCarrySlot;                  // +0x2c
    long          PassiveCarrySlot;                          // +0x30
    long          OnKillFX;                                  // +0x34
    float         OffsetCoordRelativeToAttachToDummy_X;      // +0x38 C3DVector::X
    float         OffsetCoordRelativeToAttachToDummy_Y;      // +0x3c C3DVector::Y
    float         OffsetCoordRelativeToAttachToDummy_Z;      // +0x40 C3DVector::Z
    float         OffsetAmountForThingsOnTopOfMe;            // +0x44
    float         PassiveCarrySlotScale;                     // +0x48
    bool          IncludeInThingScansWhenCarried;            // +0x4c
    unsigned char _pad_0x4d[0x3];                            // +0x4d
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CCarryableDef) == 0x50);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCarryableDef, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCarryableDef, ActiveCarrySlot) == 0x28);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCarryableDef, SecondaryActiveCarrySlot) == 0x2c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCarryableDef, PassiveCarrySlot) == 0x30);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCarryableDef, OnKillFX) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCarryableDef, OffsetCoordRelativeToAttachToDummy_X) == 0x38);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCarryableDef, OffsetCoordRelativeToAttachToDummy_Y) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCarryableDef, OffsetCoordRelativeToAttachToDummy_Z) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCarryableDef, OffsetAmountForThingsOnTopOfMe) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCarryableDef, PassiveCarrySlotScale) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCarryableDef, IncludeInThingScansWhenCarried) == 0x4c);
#endif
#endif

#endif // FABLE_ENGINE_CCARRYABLEDEF_H

// End shared header engine/CCarryableDef.h
#undef CCarryableDef
#define CWorldMap CWorldMapFields
// Begin shared header engine/CWorldMap.h
#ifndef FABLE_ENGINE_CWORLDMAP_H
#define FABLE_ENGINE_CWORLDMAP_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CWorldMap = 0xfc.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CWorldMap = 0xd8.
// Retail evidence: 11 trusted landed function(s); reconciliation 1 conflict(s) in 1 file(s) vs 10 agreeing.
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
class CLevelLoader;
class CMainGameComponent;
class CRegionConnectionGraph;
struct CThingMapManager;
class CWADBank;
class CWorld;

#pragma pack(push, 1)
class CWorldMap {
public:
    void*                   __vftable;                       // +0x00 vptr, or first dword of a flattened base subobject
    long                    f4;                              // +0x04 retail-only long (previous header)
    CWorld*                 World;                           // +0x08
    CMainGameComponent*     Component;                       // +0x0c
    CThingMapManager*       PThingMapManager_ptr;            // +0x10 scoped_ptr<NThingMap::CThingMapManager>::ptr
    unsigned char           LoadedScriptedMapBrushes[0xc];   // +0x14 map<CCharString,long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,long>_>_>
    unsigned char           Maps[0xc];                       // +0x20 CArray<CMapInfo>
    unsigned char           Regions[0xc];                    // +0x2c vector<CRegion,std::allocator<CRegion>_>
    unsigned char           RegionTime[0xc];                 // +0x38 vector<long,std::allocator<long>_>
    CRegionConnectionGraph* PRegionGraph_Data;               // +0x44 CCountedPointer<CRegionConnectionGraph>::Data
    CCPPointerInfo*         PRegionGraph_Info;               // +0x48 CCountedPointer<CRegionConnectionGraph>::Info
    bool                    SoundThemesDisabled;             // +0x4c
    unsigned char           _pad_0x4d[0x3];                  // +0x4d
    long                    Granularity;                     // +0x50
    long                    GranularityShift;                // +0x54
    long                    MapPlacementPitch;               // +0x58
    long                    Area_TLX;                        // +0x5c C2DBoxI::TLX
    long                    Area_TLY;                        // +0x60 C2DBoxI::TLY
    long                    Area_BRX;                        // +0x64 C2DBoxI::BRX
    long                    Area_BRY;                        // +0x68 C2DBoxI::BRY
    unsigned char           MapPlacement[0xc];               // +0x6c vector<unsigned_short,std::allocator<unsigned_short>_>
    unsigned char           PlayerPositionInfo[0xc];         // +0x78 vector<CWorldMap::CPlayerPositionInfo,std::allocator<CWorldMap::CPlayerPositionInfo>_>
    unsigned char           HeightChangedAreas[0xc];         // +0x84 vector<C2DBoxI,std::allocator<C2DBoxI>_>
    unsigned char           LoadedMaps[0xc];                 // +0x90 CArray<long>
    long                    LoadedRegion;                    // +0x9c
    CWADBank*               LevelWADBank_Data;               // +0xa0 CCountedPointer<CWADBank>::Data
    CCPPointerInfo*         LevelWADBank_Info;               // +0xa4 CCountedPointer<CWADBank>::Info
    bool                    UseLevelWADBank;                 // +0xa8
    unsigned char           _pad_0xa9[0x3];                  // +0xa9
    unsigned long           CurrentMapUIDCount;              // +0xac
    unsigned char           DestroyedThemePositions[0xc];    // +0xb0 vector<C2DCoordI,std::allocator<C2DCoordI>_>
    CLevelLoader*           PLevelLoader_Data;               // +0xbc CCountedPointer<CLevelLoader>::Data
    CCPPointerInfo*         PLevelLoader_Info;               // +0xc0 CCountedPointer<CLevelLoader>::Info
    unsigned char           PCameraRegionParticle[0x8];      // +0xc4 CIntelligentPointer<CThing>
    unsigned char           PHeroBreathParticle[0x8];        // +0xcc CIntelligentPointer<CThing>
    bool                    RegionUnloading;                 // +0xd4
    bool                    PassabilityBeingShown;           // +0xd5
    unsigned char           _pad_0xd6[0x2];                  // +0xd6
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CWorldMap) == 0xd8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, f4) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, World) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Component) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PThingMapManager_ptr) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LoadedScriptedMapBrushes) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Maps) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Regions) == 0x2c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, RegionTime) == 0x38);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PRegionGraph_Data) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PRegionGraph_Info) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, SoundThemesDisabled) == 0x4c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Granularity) == 0x50);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, GranularityShift) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, MapPlacementPitch) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Area_TLX) == 0x5c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Area_TLY) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Area_BRX) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Area_BRY) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, MapPlacement) == 0x6c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PlayerPositionInfo) == 0x78);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, HeightChangedAreas) == 0x84);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LoadedMaps) == 0x90);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LoadedRegion) == 0x9c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LevelWADBank_Data) == 0xa0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LevelWADBank_Info) == 0xa4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, UseLevelWADBank) == 0xa8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, CurrentMapUIDCount) == 0xac);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, DestroyedThemePositions) == 0xb0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PLevelLoader_Data) == 0xbc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PLevelLoader_Info) == 0xc0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PCameraRegionParticle) == 0xc4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PHeroBreathParticle) == 0xcc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, RegionUnloading) == 0xd4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PassabilityBeingShown) == 0xd5);
#endif
#endif

#endif // FABLE_ENGINE_CWORLDMAP_H

// End shared header engine/CWorldMap.h
#undef CWorldMap
#define CDefinitionManager CDefinitionManagerFields
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
class C3DVector{public:float X,Y,Z;};class CRightHandedSet{protected:C3DVector Up,Forward;};
class C2DCoordI{public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CWorldMap;
enum ETCInterfaceType{};class C2DVector;class CTCParamsBase;class CEnginePrimitivePrepareInfo;class CPersistContext;namespace NTCMessages{class CMessageBase;}
// Layout: shared CTCBase prefix; native table omits donor editor-only property transfer pair.
class CTCBase:public CBaseClassNonCopyable{public:CTCBase();protected:CWorldMap&GetWorldMap();CThing&ParentThing;public:CThing&GetParentThing(){return ParentThing;}const CThing&GetParentThing()const{return ParentThing;}private:bool ToBeRemoved,IsInGlobalUpdateTCs;
protected:virtual void OnSerialise(CPersistContext&,bool);
public:virtual void OnRemove();
public:virtual void ConstructFromParams(const CTCParamsBase&);
public:virtual void OnCreate();
public:virtual void InitialActivate();
public:virtual void OnInitialActivate();
public:virtual void RenderUpdate(float);
public:virtual bool HasFrameUpdate()const;
public:virtual void FrameUpdate();
public:virtual void BulletTimeFrameUpdate();
public:virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
public:virtual void Copy(const CTCBase&);
public:virtual void OnDie();
public:virtual void OnKill();
public:virtual void OnLevelUnload();
public:virtual void OnSetInLimbo(bool);
public:virtual bool Validate(CCharString&)const;
public:virtual bool IsFreeForDeletion()const;
public:virtual bool IsSerialiseCreatable()const;
public:virtual bool IsSerialisableForHeroState()const;
public:virtual ETCInterfaceType GetInterfaceType()const;
public:virtual CCharString GetClassTypeName()const;
public:virtual bool IsComponentSerialisable()const;
public:virtual bool IsDriver()const;
public:virtual bool IsCopyable()const;
public:virtual void GlobalUpdate(bool);
public:virtual void HandleMessage(const NTCMessages::CMessageBase&);
};
// Layout: shared physics base64, actual virtual prefix and unread member storage after12.
class CTCPhysicsBase:public CTCBase{protected:C3DVector Position;private:unsigned char unreadPhysics[0x40-0x18];public:const C3DVector&GetPosition()const{return Position;}
protected:virtual C2DVector GetOffsetVector()const;
protected:virtual C3DVector GetOffsetVector3D()const;
public:virtual void SetPosition(const C3DVector&);
public:virtual void ResetPosition(const C3DVector&);
public:virtual void ResetRHSet(const CRightHandedSet&);
public:virtual void SetPositionToNavigateTo(const C3DVector&);
public:virtual void SetForcedToMovePos(const C3DVector&,bool);
public:virtual void ResetMovementState();
public:virtual void EnablePhysics(bool);
public:virtual bool IsPhysicsEnabled()const;
public:virtual void EnableGravity(bool);
public:virtual bool IsGravityEnabled()const;
public:virtual void EnableNavigator(bool);
public:virtual bool IsNavigationEnabled()const;
public:virtual bool HasNavigationLayers()const;
public:virtual void SetInteractionFlags(long);
public:virtual void ReturnToDefaultInteractionFlags();
public:virtual void OnDroppedFromUpdateList();
public:virtual void OnAddedToUpdateList();
public:virtual bool IsAlwaysFacingMovementDirection()const;
public:virtual bool IsAffectedByGravity()const;
public:virtual bool IsAffectedByAir()const;
public:virtual bool IsAffectedByWind()const;
public:virtual bool IsAngledWithGround()const;
public:virtual bool IsImmoveable()const;
public:virtual bool IsOnAnySurface()const;
public:virtual bool IsOnAnyStationarySurface()const;
public:virtual bool IsOnMeshSurface()const;
public:virtual bool IsOnGroundSurface()const;
public:virtual bool CanCollideWithThings()const;
public:virtual bool CanCollideWithThingsOneWay()const;
public:virtual bool CanCollideWithLandscape()const;
public:virtual bool CanInteractWithThings()const;
public:virtual bool IsExcludingCreatures()const;
public:virtual bool IsPhysicsMeshImpassable()const;
public:virtual float GetFacingAngleXY()const;
public:virtual float GetFacingAngleYZ()const;
public:virtual float GetFacingAngleXZ()const;
public:virtual void SetFacingAngleXY(float);
public:virtual void SetFacingAngleYZ(float);
public:virtual void SetFacingAngleXZ(float);
public:virtual C3DVector GetFacingVector()const;
public:virtual C3DVector GetFacingVector(float)const;
public:virtual const CRightHandedSet& GetRHSet()const;
public:virtual CRightHandedSet GetRHSet(float)const;
public:virtual const CRightHandedSet&GetLastRHSet()const;
public:virtual void SetFacingAnglesFromRHSet(const CRightHandedSet&);
public:virtual void SetRHSet(const CRightHandedSet&);
public:virtual C3DVector GetImpulse()const;
public:virtual void SetRHSetAsFlushOnNormal(const C3DVector&);
public:virtual void SetRHSetAsFlushOnGround();
public:virtual float GetRenderZOffset(float)const;
public:virtual void ResetInterpolation();
};

namespace NGraphicAppearance{
// Layout: shared graphics header Alpha5a and bitgroup59; original ForceResendEveryRenderFrame is bit1 in donor71/native59.
class CTCGraphicAppearance:public CTCGraphicAppearanceFields{public:void RemoveAllMeshEffectsInheritedFromCarrier();void InheritMeshEffectsFromCarrier(const CTCGraphicAppearance*);void SetAlpha(unsigned char);unsigned char GetAlpha()const{return Alpha;}void SetForceResendEveryRenderFrame(bool b){if(b)PrimitivesDrawn[0]|=2;else PrimitivesDrawn[0]&=~2;}};
}
namespace std{template<class T>class allocator{};template<class A,class B>struct pair{A first;B second;};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T*end(){return this->_M_finish;}};}
template<class T>class CArray:public std::vector<T>{};template<class K,class V>class CKeyPairCompareLess{};template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{C Compare;bool Dirty;public:std::pair<K,V>*LowerBound(const K&);};
class CThingBaseDef:public CParentDefClassBase{};
// Layout: shared native CThing PhysicsTC60, graphics64, base-definition70 and component map44/bitset20.
class CThing:public CThingFields{public:const C3DVector&GetPos()const;void SetCurrentMapNumber(long);void SetThingScanInclusion(bool);
 const CThingBaseDef&PeekBaseDef()const{return *reinterpret_cast<const CThingBaseDef*>(PBaseDef_Object);}
 CTCPhysicsBase*GetPhysicsTC(){return PhysicsTC;}
 NGraphicAppearance::CTCGraphicAppearance*GetTCGraphicAppearance(){return reinterpret_cast<NGraphicAppearance::CTCGraphicAppearance*>(PTCGraphicAppearance);}
 __forceinline bool HasTC(ETCInterfaceType kind)const{if(reinterpret_cast<const unsigned long*>(UsingComponents)[kind>>5]&(1UL<<(kind&31)))return true;return false;}
 template<class T>__forceinline bool GetTC(T**out){ETCInterfaceType kind=T::StatGetInterfaceType();if(HasTC(kind)){*out=static_cast<T*>(&GetTC(kind));return true;}return false;}
 __forceinline CTCBase&GetTC(ETCInterfaceType kind){ETCInterfaceType key=kind;CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(key);return *((it==map.end()||kind<it->first)?map.end():it)->second;}
};
class CTCWeaponTrail:public CTCBase{public:void ClearAllWeaponTrails();};
class CTCVolumeContainmentTracker:public CTCBase{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return static_cast<ETCInterfaceType>(0x75);}void OnThingCarried(CThing&);};
class CDrawMapInfo;class C2DBoxI;class CAFile;class CASuspendableProcess;class CMap;
class CIDrawWorldMap{public:
 virtual long DrawGetNoMaps()const=0;
 virtual CDrawMapInfo DrawGetMapInfo(long)const=0;
 virtual C2DBoxI DrawGetDimensions()const=0;
 virtual long DrawGetGranularity()const=0;
 virtual bool DrawIsPosInMap(const C3DVector&)const=0;
 virtual float DrawGetSizeZAt(const C3DVector&,float)const=0;
 virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long,const C3DVector&)const=0;
 virtual long DrawGetMyPlayerMapNumber()const=0;
 virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const=0;
 virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const=0;
 virtual long DrawGetFrame()const=0;
 virtual~CIDrawWorldMap();
};
class CWorldMap:public CBaseClassNonCopyable,public CIDrawWorldMap{public:
 virtual~CWorldMap();
 virtual void DeleteAllLevels();virtual void DeleteAllRegions();
 virtual void LoadFromFile(CAFile&,long);virtual void SaveToFile(CAFile&);virtual void RemoveLevel(long);
 virtual void LoadTopologyFromFile(long,CCountedPointer<CAFile>,CASuspendableProcess&);virtual void PostLoadTopology(long);
 virtual void ForceLoad(long);virtual void ForceUnload(long);virtual bool IsMapLoaded(long)const;virtual bool IsMapUsed(long)const;
 virtual long GetGranularity()const;virtual const C2DBoxI&GetDimensions()const;
 virtual CMap&GetMapAt(const C2DCoordI&);virtual const CMap&PeekMapAt(const C2DCoordI&)const;
 virtual long GetMapNumberAt(const C2DCoordI&)const;virtual long GetMapNumberAt(const C3DVector&)const;
};
// Layout: CCarryableDef original subdef base25, named fields28..4c agree with shared header.
class CCarryableDef:public CSubDefClassBase{public:long ActiveCarrySlot,SecondaryActiveCarrySlot,PassiveCarrySlot,OnKillFX;C3DVector OffsetCoordRelativeToAttachToDummy;float OffsetAmountForThingsOnTopOfMe,PassiveCarrySlotScale;bool IncludeInThingScansWhenCarried;static CCharString __fastcall StatGetTypeName(){return CCharString("CCarryableDef");}};
class CDefinitionManager:public CDefinitionManagerFields{public:CDefPointer<CDefClassBase>GetPDefFromGlobalIndex(long);};
template<>bool CDefClassBase::GetSubDef<CCarryableDef>(CDefPointer<const CCarryableDef>*)const;
class CWorld;
// Layout: native CarrySlotsc..1b; donor vector extra word shifts BypassFilters20 to1c. Only this prefix is accessed.
class CTCCarrying:public CTCBase{private:unsigned char unreadCarrySlots[0x10];bool BypassFilters;public:bool IsAllCarriedThingsBypassFilter()const{return BypassFilters;}void ResetAllCarriedThingsAsBypassFilter();};
// Layout: original CTCCarryable matches CTCBase12, ForceFragilityc, PCarryingTC10, vector14, float20, size24.
class CTCCarryable:public CTCBase{public:bool ForceFragility;void UpdateCarriedPosition(bool);private:CTCCarrying*PCarryingTC;C3DVector OffsetCoordRelativeToAttachToDummy;float OffsetAmount;void SetAsCarriedBy(CTCCarrying*);};
void CTCCarryable::SetAsCarriedBy(CTCCarrying* carrier)
{
    if (ParentThing.HasTC(static_cast<ETCInterfaceType>(0x4c)))
        static_cast<CTCWeaponTrail&>(ParentThing.GetTC(static_cast<ETCInterfaceType>(0x4c))).ClearAllWeaponTrails();
    if (carrier == 0) {
        UpdateCarriedPosition(true);
        ParentThing.GetPhysicsTC()->ResetInterpolation();
        PCarryingTC = 0;
        ParentThing.GetPhysicsTC()->EnablePhysics(true);
        ParentThing.SetThingScanInclusion(true);
        CWorldMap& map = GetWorldMap();
        const C3DVector& pos = ParentThing.GetPos();
        C2DCoordI coord((long)pos.X, (long)pos.Y);
        CThing&parent = ParentThing;
        parent.SetCurrentMapNumber(map.GetMapNumberAt(coord));
        if (ParentThing.GetTCGraphicAppearance())
            ParentThing.GetTCGraphicAppearance()->RemoveAllMeshEffectsInheritedFromCarrier();
        return;
    }
    bool persist = false;
    CDefPointer<const CCarryableDef> def;
    if (ParentThing.PeekBaseDef().GetSubDef(&def))
        persist = def->IncludeInThingScansWhenCarried;
    ParentThing.GetPhysicsTC()->EnablePhysics(false);
    ParentThing.SetThingScanInclusion(persist);
    ParentThing.GetTCGraphicAppearance()->SetForceResendEveryRenderFrame(true);
    PCarryingTC = carrier;
    if (carrier->IsAllCarriedThingsBypassFilter())
        carrier->ResetAllCarriedThingsAsBypassFilter();
    NGraphicAppearance::CTCGraphicAppearance* carrierGraphic = carrier->GetParentThing().GetTCGraphicAppearance();
    NGraphicAppearance::CTCGraphicAppearance* graphic = ParentThing.GetTCGraphicAppearance();
    if (carrierGraphic && graphic)
        graphic->InheritMeshEffectsFromCarrier(carrierGraphic);
    {CTCVolumeContainmentTracker*tracker;
    if (carrier->GetParentThing().GetTC(&tracker))
        tracker->OnThingCarried(ParentThing);}
    if (carrier->GetParentThing().GetTCGraphicAppearance())
        ParentThing.GetTCGraphicAppearance()->SetAlpha(carrier->GetParentThing().GetTCGraphicAppearance()->GetAlpha());
    CTCPhysicsBase* physics = ParentThing.GetPhysicsTC();
    physics->ResetPosition(carrier->GetParentThing().GetPhysicsTC()->GetPosition());
    physics->ResetRHSet(carrier->GetParentThing().GetPhysicsTC()->GetRHSet());
    physics->ResetInterpolation();
}

#undef private
#undef protected
#define CHECK(x) do{if(!(x)){printf("failure line %d\n",__LINE__);exit(1);}}while(0)
class C2DVector{public:float X,Y;};class CDefString{long TablePos;};
static CThing*owner,*carrierThing,*constructParent;static CTCCarryable*carryable;static CTCCarrying*carrying;
static CTCPhysicsBase*ownPhysics,*otherPhysics;static NGraphicAppearance::CTCGraphicAppearance*ownGraphic,*otherGraphic;
static CTCWeaponTrail*trail;static CTCVolumeContainmentTracker*tracker;
static CThingBaseDef*baseDef;static CCarryableDef*subdef;static CWorldMap*world;
static bool attaching,hasDef;static unsigned refs;static int clearTrails,updatePos,resetInterpolation,enablePhysics,persistCalls,mapCalls,removeEffects,inheritEffects,resetFilter,carriedCalls,alphaCalls,posCalls,rhsCalls,deleteCalls,subCalls,lookupCalls;
static C3DVector position;static CRightHandedSet orientation;static bool enabled,persisted;static unsigned char alphaValue;static long mapSeen;static C2DCoordI*coordSeen;
static bool sameVec(const C3DVector&a,const C3DVector&b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CTCBase::CTCBase():ParentThing(*constructParent),ToBeRemoved(false),IsInGlobalUpdateTCs(false){}
CResource::CResource(){IVRefCount=0;ResourceList=0;PrevResource=NextResource=0;ResourceSize=LastUsedFrame=0;}CResource::~CResource(){}
bool CResource::UnloadResource(){return false;}unsigned long CResource::GetUnloadableMemory()const{return 0;}bool CResource::IsResourceLoaded()const{return true;}void CResource::OnResourceLoaded(){}bool CResource::CanAbortResourceLoad(long)const{return false;}void CResource::OnResourceLoadAborted(){}unsigned long CResource::GetDebugMemoryInUse()const{return 0;}void CResource::ResetDebugMemoryInUse(){}
CDefClassBase::CDefClassBase(){PDefManager=0;GlobalIndex=0;Setup=Template=DefaultValsApplied=0;}
CParentDefClassBase::CParentDefClassBase(){}CParentDefClassBase::~CParentDefClassBase(){}CSubDefClassBase::CSubDefClassBase(){}
void CIVCountedPointeeBase::Delete()const{CHECK(this==subdef&&IVRefCount==0);++deleteCalls;}
class TestOwnerDef:public CThingBaseDef{public:void Transfer(CPersistContext&){}void Copy(const CDefClassBase*){}unsigned long GetSizeofClass(){return sizeof(*this);}};
class TestSubDef:public CCarryableDef{public:void Transfer(CPersistContext&){}void Copy(const CDefClassBase*){}unsigned long GetSizeofClass(){return sizeof(*this);}};
template<>bool CDefClassBase::GetSubDef<CCarryableDef>(CDefPointer<const CCarryableDef>*out)const{CHECK(this==baseDef&&out->IsNull());++subCalls;if(hasDef){out->Object=subdef;subdef->AddRef();}return hasDef;}
void CTCCarryable::UpdateCarriedPosition(bool b){CHECK(this==carryable&&b&&!attaching);++updatePos;}
CWorldMap&CTCBase::GetWorldMap(){CHECK(this==carryable&&!attaching);return *world;}
const C3DVector&CThing::GetPos()const{CHECK(this==owner&&!attaching);return position;}
static long __fastcall TestMapNumber(THISCALL_ARGS(CWorldMap),const C2DCoordI&coord){CHECK(self==world&&!attaching);CHECK(coord.X==(long)position.X&&coord.Y==(long)position.Y);++mapCalls;return -13;}
void CThing::SetCurrentMapNumber(long n){CHECK(this==owner&&!attaching);mapSeen=n;}
void CThing::SetThingScanInclusion(bool b){CHECK(this==owner);++persistCalls;persisted=b;}
void CTCWeaponTrail::ClearAllWeaponTrails(){CHECK(this==trail);++clearTrails;}
void CTCCarrying::ResetAllCarriedThingsAsBypassFilter(){CHECK(this==carrying&&BypassFilters);++resetFilter;}
void CTCVolumeContainmentTracker::OnThingCarried(CThing&thing){CHECK(this==tracker&&&thing==owner);++carriedCalls;}
void NGraphicAppearance::CTCGraphicAppearance::RemoveAllMeshEffectsInheritedFromCarrier(){CHECK(this==ownGraphic&&!attaching);++removeEffects;}
void NGraphicAppearance::CTCGraphicAppearance::InheritMeshEffectsFromCarrier(const CTCGraphicAppearance*g){CHECK(this==ownGraphic&&g==otherGraphic&&attaching);++inheritEffects;}
void NGraphicAppearance::CTCGraphicAppearance::SetAlpha(unsigned char v){CHECK(this==ownGraphic&&attaching);alphaValue=v;Alpha=v;++alphaCalls;}
void CTCPhysicsBase::EnablePhysics(bool b){CHECK(this==ownPhysics);enabled=b;++enablePhysics;}
void CTCPhysicsBase::ResetInterpolation(){CHECK(this==ownPhysics);++resetInterpolation;}
void CTCPhysicsBase::ResetPosition(const C3DVector&v){CHECK(this==ownPhysics&&attaching&&sameVec(v,otherPhysics->Position));++posCalls;}
void CTCPhysicsBase::ResetRHSet(const CRightHandedSet&r){CHECK(this==ownPhysics&&attaching&&sameVec(r.Up,orientation.Up)&&sameVec(r.Forward,orientation.Forward));++rhsCalls;}
const CRightHandedSet&CTCPhysicsBase::GetLastRHSet()const{abort();}
const CRightHandedSet&CTCPhysicsBase::GetRHSet()const{CHECK(this==otherPhysics&&attaching);return orientation;}
template<class K,class V,class C>std::pair<K,V>*CVectorMap<K,V,C>::LowerBound(const K&key){CHECK(key==0x4c||key==0x75);++lookupCalls;std::pair<K,V>*i=this->_M_start;while(i!=this->_M_finish&&i->first<key)++i;return i;}
void CTCBase::OnSerialise(CPersistContext&,bool){abort();}
void CTCBase::OnRemove(){abort();}
void CTCBase::ConstructFromParams(const CTCParamsBase&){abort();}
void CTCBase::OnCreate(){abort();}
void CTCBase::InitialActivate(){abort();}
void CTCBase::OnInitialActivate(){abort();}
void CTCBase::RenderUpdate(float){abort();}
bool CTCBase::HasFrameUpdate()const{abort();}
void CTCBase::FrameUpdate(){abort();}
void CTCBase::BulletTimeFrameUpdate(){abort();}
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&){abort();}
void CTCBase::Copy(const CTCBase&){abort();}
void CTCBase::OnDie(){abort();}
void CTCBase::OnKill(){abort();}
void CTCBase::OnLevelUnload(){abort();}
void CTCBase::OnSetInLimbo(bool){abort();}
bool CTCBase::Validate(CCharString&)const{abort();}
bool CTCBase::IsFreeForDeletion()const{abort();}
bool CTCBase::IsSerialiseCreatable()const{abort();}
bool CTCBase::IsSerialisableForHeroState()const{abort();}
ETCInterfaceType CTCBase::GetInterfaceType()const{abort();}
CCharString CTCBase::GetClassTypeName()const{abort();}
bool CTCBase::IsComponentSerialisable()const{abort();}
bool CTCBase::IsDriver()const{abort();}
bool CTCBase::IsCopyable()const{abort();}
void CTCBase::GlobalUpdate(bool){abort();}
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&){abort();}
C2DVector CTCPhysicsBase::GetOffsetVector()const{abort();}
C3DVector CTCPhysicsBase::GetOffsetVector3D()const{abort();}
void CTCPhysicsBase::SetPosition(const C3DVector&){abort();}
void CTCPhysicsBase::SetPositionToNavigateTo(const C3DVector&){abort();}
void CTCPhysicsBase::SetForcedToMovePos(const C3DVector&,bool){abort();}
void CTCPhysicsBase::ResetMovementState(){abort();}
bool CTCPhysicsBase::IsPhysicsEnabled()const{abort();}
void CTCPhysicsBase::EnableGravity(bool){abort();}
bool CTCPhysicsBase::IsGravityEnabled()const{abort();}
void CTCPhysicsBase::EnableNavigator(bool){abort();}
bool CTCPhysicsBase::IsNavigationEnabled()const{abort();}
bool CTCPhysicsBase::HasNavigationLayers()const{abort();}
void CTCPhysicsBase::SetInteractionFlags(long){abort();}
void CTCPhysicsBase::ReturnToDefaultInteractionFlags(){abort();}
void CTCPhysicsBase::OnDroppedFromUpdateList(){abort();}
void CTCPhysicsBase::OnAddedToUpdateList(){abort();}
bool CTCPhysicsBase::IsAlwaysFacingMovementDirection()const{abort();}
bool CTCPhysicsBase::IsAffectedByGravity()const{abort();}
bool CTCPhysicsBase::IsAffectedByAir()const{abort();}
bool CTCPhysicsBase::IsAffectedByWind()const{abort();}
bool CTCPhysicsBase::IsAngledWithGround()const{abort();}
bool CTCPhysicsBase::IsImmoveable()const{abort();}
bool CTCPhysicsBase::IsOnAnySurface()const{abort();}
bool CTCPhysicsBase::IsOnAnyStationarySurface()const{abort();}
bool CTCPhysicsBase::IsOnMeshSurface()const{abort();}
bool CTCPhysicsBase::IsOnGroundSurface()const{abort();}
bool CTCPhysicsBase::CanCollideWithThings()const{abort();}
bool CTCPhysicsBase::CanCollideWithThingsOneWay()const{abort();}
bool CTCPhysicsBase::CanCollideWithLandscape()const{abort();}
bool CTCPhysicsBase::CanInteractWithThings()const{abort();}
bool CTCPhysicsBase::IsExcludingCreatures()const{abort();}
bool CTCPhysicsBase::IsPhysicsMeshImpassable()const{abort();}
float CTCPhysicsBase::GetFacingAngleXY()const{abort();}
float CTCPhysicsBase::GetFacingAngleYZ()const{abort();}
float CTCPhysicsBase::GetFacingAngleXZ()const{abort();}
void CTCPhysicsBase::SetFacingAngleXY(float){abort();}
void CTCPhysicsBase::SetFacingAngleYZ(float){abort();}
void CTCPhysicsBase::SetFacingAngleXZ(float){abort();}
C3DVector CTCPhysicsBase::GetFacingVector()const{abort();}
C3DVector CTCPhysicsBase::GetFacingVector(float)const{abort();}
CRightHandedSet CTCPhysicsBase::GetRHSet(float)const{abort();}
void CTCPhysicsBase::SetFacingAnglesFromRHSet(const CRightHandedSet&){abort();}
void CTCPhysicsBase::SetRHSet(const CRightHandedSet&){abort();}
C3DVector CTCPhysicsBase::GetImpulse()const{abort();}
void CTCPhysicsBase::SetRHSetAsFlushOnNormal(const C3DVector&){abort();}
void CTCPhysicsBase::SetRHSetAsFlushOnGround(){abort();}
float CTCPhysicsBase::GetRenderZOffset(float)const{abort();}
bool CDefClassBase::UnloadResource(){abort();}
CDefString CDefClassBase::GetInstantiationName()const{abort();}
void CDefClassBase::SetInstantiationName(CDefString){abort();}
void CDefClassBase::AddSubDef(const CCharString&, long, long){abort();}
void CDefClassBase::RemoveSubDef(const CCharString&){abort();}
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&){abort();}
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&)const{abort();}
bool CDefClassBase::IsParentDef()const{abort();}
void CDefClassBase::PostTransfer(){abort();}
void CDefClassBase::InternalSaveBinary(CDataOutputStream&){abort();}
void CDefClassBase::InternalLoadBinary(CDataInputStream&){abort();}
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&)const{abort();}
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap(){abort();}
CDefString CParentDefClassBase::GetInstantiationName()const{abort();}
void CParentDefClassBase::SetInstantiationName(CDefString){abort();}
void CParentDefClassBase::AddSubDef(const CCharString&, long, long){abort();}
void CParentDefClassBase::RemoveSubDef(const CCharString&){abort();}
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&){abort();}
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&)const{abort();}
bool CParentDefClassBase::IsParentDef()const{abort();}
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&){abort();}
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&){abort();}
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap(){abort();}

int main(){
 CThing thing,carrier;memset(&thing,0,sizeof thing);memset(&carrier,0,sizeof carrier);owner=&thing;carrierThing=&carrier;constructParent=owner;
 CTCCarryable attached;carryable=&attached;CTCPhysicsBase physics;ownPhysics=&physics;CTCWeaponTrail weaponTrail;trail=&weaponTrail;
 constructParent=carrierThing;CTCCarrying carryingTC;carrying=&carryingTC;CTCPhysicsBase carrierPhysics;otherPhysics=&carrierPhysics;CTCVolumeContainmentTracker containment;tracker=&containment;
 NGraphicAppearance::CTCGraphicAppearance graphicA,graphicB;ownGraphic=&graphicA;otherGraphic=&graphicB;
 TestOwnerDef base;TestSubDef def;baseDef=&base;subdef=&def;thing.PBaseDef_Object=&base;thing.PhysicsTC=ownPhysics;carrier.PhysicsTC=otherPhysics;
 typedef void(*VF)();VF worldTable[20];memset(worldTable,0,sizeof worldTable);
#ifdef _MSC_VER
 worldTable[17]=reinterpret_cast<VF>(&TestMapNumber);
#else
 worldTable[17]=reinterpret_cast<VF>(&TestMapNumber);
#endif
 VF*worldStorage[4]={worldTable,0,0,0};world=reinterpret_cast<CWorldMap*>(worldStorage);
 std::pair<ETCInterfaceType,CTCBase*>ownerEntries[3],carrierEntries[3];ownerEntries[0].first=(ETCInterfaceType)1;ownerEntries[0].second=0;ownerEntries[1].first=(ETCInterfaceType)0x4c;ownerEntries[1].second=trail;ownerEntries[2].first=(ETCInterfaceType)250;ownerEntries[2].second=0;carrierEntries[0].first=(ETCInterfaceType)2;carrierEntries[0].second=0;carrierEntries[1].first=(ETCInterfaceType)0x75;carrierEntries[1].second=tracker;carrierEntries[2].first=(ETCInterfaceType)260;carrierEntries[2].second=0;
 std::pair<ETCInterfaceType,CTCBase*>**mapA=reinterpret_cast<std::pair<ETCInterfaceType,CTCBase*>**>(thing.Components),**mapB=reinterpret_cast<std::pair<ETCInterfaceType,CTCBase*>**>(carrier.Components);mapA[0]=ownerEntries;mapA[1]=ownerEntries+3;mapB[0]=carrierEntries;mapB[1]=carrierEntries+3;
 const float coords[4]={-2.5f,0.0f,1.5f,8.0f};unsigned cases=0;
 for(unsigned attach=0;attach<2;++attach)for(unsigned hasTrail=0;hasTrail<2;++hasTrail)for(unsigned present=0;present<2;++present)for(unsigned scans=0;scans<2;++scans)for(unsigned filters=0;filters<2;++filters)for(unsigned hasTracker=0;hasTracker<2;++hasTracker)for(unsigned graphic=0;graphic<2;++graphic)for(unsigned holding=0;holding<2;++holding)for(unsigned variation=0;variation<4;++variation){
  attaching=attach!=0;hasDef=present!=0;refs=holding*3;def.IVRefCount=refs;def.IncludeInThingScansWhenCarried=scans!=0;
  thing.PTCGraphicAppearance=attaching||graphic?reinterpret_cast<CTCGraphicAppearance*>(ownGraphic):0;carrier.PTCGraphicAppearance=graphic?reinterpret_cast<CTCGraphicAppearance*>(otherGraphic):0;
  carryingTC.BypassFilters=filters!=0;attached.PCarryingTC=carrying;graphicA.PrimitivesDrawn[0]=0xa4;graphicA.Alpha=99;graphicB.Alpha=(unsigned char)(variation*85);
  memset(thing.UsingComponents,0,sizeof thing.UsingComponents);memset(carrier.UsingComponents,0,sizeof carrier.UsingComponents);reinterpret_cast<unsigned long*>(thing.UsingComponents)[2]=hasTrail?1UL<<12:0;reinterpret_cast<unsigned long*>(carrier.UsingComponents)[3]=hasTracker?1UL<<21:0;
  position.X=coords[variation];position.Y=coords[3-variation];position.Z=16;carrierPhysics.Position=position;orientation.Up=position;orientation.Forward.X=1;orientation.Forward.Y=0;orientation.Forward.Z=0;
  clearTrails=updatePos=resetInterpolation=enablePhysics=persistCalls=mapCalls=removeEffects=inheritEffects=resetFilter=carriedCalls=alphaCalls=posCalls=rhsCalls=deleteCalls=subCalls=lookupCalls=0;enabled=attaching;persisted=false;mapSeen=100;
  attached.SetAsCarriedBy(attaching?carrying:0);
  CHECK(clearTrails==(int)hasTrail&&enablePhysics==1&&enabled==!attaching&&persistCalls==1&&resetInterpolation==1);
  CHECK(def.IVRefCount==refs&&deleteCalls==(attaching&&hasDef&&refs==0));CHECK(lookupCalls==(int)(hasTrail+(attach?hasTracker:0)));
  if(attaching){CHECK(attached.PCarryingTC==carrying&&subCalls==1&&persisted==(hasDef&&scans)&&resetFilter==(int)filters&&carriedCalls==(int)hasTracker);CHECK(inheritEffects==(int)graphic&&alphaCalls==(int)graphic&&(!graphic||alphaValue==graphicB.Alpha));CHECK(graphicA.PrimitivesDrawn[0]==0xa6&&posCalls==1&&rhsCalls==1&&updatePos==0&&mapCalls==0&&removeEffects==0);}
  else{CHECK(attached.PCarryingTC==0&&persisted&&subCalls==0&&updatePos==1&&mapCalls==1&&mapSeen==-13&&removeEffects==(int)graphic);CHECK(inheritEffects==0&&resetFilter==0&&carriedCalls==0&&alphaCalls==0&&posCalls==0&&rhsCalls==0&&graphicA.PrimitivesDrawn[0]==0xa4);}
  ++cases;
 }CHECK(cases==1024);puts("00705d40_TEST PASS");return 0;
}