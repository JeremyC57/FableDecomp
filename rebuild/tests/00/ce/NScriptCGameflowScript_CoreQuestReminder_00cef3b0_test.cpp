#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
#define require(ok) do{if(!(ok)){fprintf(stderr,"failed %d\n",__LINE__);abort();}}while(0)
// 00cef3b0 NScript::CGameflowScript::CoreQuestReminder
// Leaf: ?CoreQuestReminder@CGameflowScript@NScript@@QAEXXZ
// Flags:
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
// Begin shared header engine/CLevelScriptBase.h
#ifndef FABLE_ENGINE_CLEVELSCRIPTBASE_H
#define FABLE_ENGINE_CLEVELSCRIPTBASE_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration): template <class T> class CLevelScriptBase :
// public CScriptBase (Ego_r.pdb CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>: 0x5C = CScriptBase 0x54 + Game +0x54 +
// MasterScriptData +0x58; retail CScriptBase is 0x40, so Game +0x40, MasterScriptData +0x44, 0x48 bytes: the landed quest
// rows read Game at +0x40). No new virtuals (Ego_r/FableWin: ctor (CScriptDataBase*, CGameScriptInterfaceBase&), virtual
// dtor, GetMasterData).
// The ctor is inline (wave 7): every level-script ctor and Alloc expands it -- `call 0x00CB8110` (CScriptBase()) followed
// at once by the two member stores, Game +0x40 then MasterScriptData +0x44, with the dead CLevelScriptBase<T> vtable store
// dropped (e.g. 0x00D61C30, the Alloc family 0x00EED220); Ego_r.pdb (release) has no ??0?$CLevelScriptBase COMDAT, while
// retail keeps five unreferenced out-of-line copies 0x00CDD160..0x00CDD220 (one per T, vtables 0x012C2EEC..0x012C2F3C),
// probably an explicit instantiation of the inline member (unproven: no caller, no TU evidence). The parameter is CScriptDataBase*,
// the member T* (T derives from CScriptDataBase at offset 0, so the conversion emits no code).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CLevelScriptBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
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
// Begin shared header engine/CScriptBase.h
#ifndef FABLE_ENGINE_CSCRIPTBASE_H
#define FABLE_ENGINE_CSCRIPTBASE_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration). No RTTI on this object: the ctor 0x00CB8110 calls
// CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0 and stores vtable 0x012C1648 (5 slots): 0 scalar deleting dtor
// 0x00CBD4F0 (~CScriptBase 0x00CBD510), 1 RegisterMain 0x00CBD4B0, 2 Main 0x00CBD4C0, 3 Init 0x00CBD4D0 (all `ret`),
// 4 OnPersist 0x00CBD4E0 (`ret 4`: the one-argument slot). Slot 2 = Main is proven by the RegisterMain rows (the "Main"
// thread's vcall thunk is `jmp [eax+8]`) and slot 3 = Init by the quest vtables (chunk 579 0x00F26B90); slot 1 RegisterMain
// is the remaining PDB virtual. Access: Ego_r.pdb / FableWin.pdb ?RegisterMain/?Main/?Init@CScriptBase@@MAEXXZ (protected),
// ?OnPersist@CScriptBase@@UAEXAAVCPersistContext@@@Z (public).
// Size 0x40 (Ego_r 0x54: Dinkumware containers): the ctor initialises Threads +0x04 (STLport list, one node pointer),
// EntityScripts +0x08 and EntityScriptBindings +0x18 (CVectorMap: STLport vector + Compare + Dirty bytes, 0x10 each) and
// RuntimeScriptedEntityPersist +0x30 (a CVectorMap, 0x10); IsActiveThreadTerminating 0x00CB7940 reads PActiveThread +0x2C.
// Containers are opaque here (real types in the comments). Non-virtual members: Ego_r.pdb spellings; retail addresses where
// the landed rows / bindings prove them.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CScriptBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
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
// Begin shared header engine/fable_nscript_forward.h
#ifndef FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H
#define FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H
// Forward declarations shared by the FABLE_RETAIL_NSCRIPT headers (CScriptThing, CGameScriptInterfaceBase, CScriptBase,
// CEntityScriptBase, CTimer, the NScript predicates, CEntityScriptBinding<T>, CLevelScriptBase<T>). Class keys are the
// ones the FableWin.pdb / Ego_r.pdb mangled names carry (V = class, U = struct: CScreenFilterSThingByPass, std::pair,
// std::less). Enums: VC7.1 accepts an MS forward enum declaration (the row defines the enumerators it uses, before or
// after the include); C++98 has none, so under g++ (syntax checks only) each enum gets an empty placeholder definition.
// std templates: STLport 4.6 parameter lists without defaults (a row's own definition may add the defaults).

class C2DCoordI;
class C2DVector;
class C3DVector;
class CCharString;
class CCounter;
class CRGBColour;
class CRGBFloatColour;
class CRightHandedSet;
class CScriptGameResourceObjectMovieBase;
class CScriptGameResourceObjectScriptedThingBase;
class CScriptThing;
class CThing;
class CTimer;
class CWideString;
class CPersistContext;
class CScriptBase;
class CSpawnedFuncBase;
class CEntityScriptBindingBase;
class CScriptDataBase;
class CActiveEntityScriptBase;
struct CScreenFilterSThingByPass;
class CHeroLogBook;
template <class T> class CArray;
template <class T> class CCountedPointer;
namespace NScript { class CPredicateBase; }
namespace std {
template <class T> class allocator;
template <class T> struct less;
template <class T1, class T2> struct pair;
template <class T, class A> class vector;
template <class T, class A> class list;
template <class K, class C, class A> class set;
template <class K, class V, class C, class A> class map;
}
#if defined(_MSC_VER)
typedef unsigned __int64 FableNScriptUInt64;                           // the PDB's unsigned __int64 (thing unique IDs)
enum EAIStateGroupType;
enum ECameraOp;
enum EClothingCoversArea;
enum ECreatureGroup;
enum ECrime;
enum ECutsceneBehaviour;
enum EDoorTriggerType;
enum EGameAction;
enum EHeroAbility;
enum EHeroTitle;
enum EHeroTrainableStatType;
enum EMiniGameType;
enum EMorality;
enum EMusicSetType;
enum EOpinion;
enum EOpinionAttitudeType;
enum EOpinionDeedType;
enum EOpinionPostDeedType;
enum EOpinionReactionType;
enum EScriptAIPriority;
enum EScriptVillageAttitude;
enum EScriptingStateGroups;
enum ESex;
enum ETargetingType;
enum ETextGroupSelectionMethod;
enum ETutorialCategory;
namespace NEntityEvents { enum EEventType; }
#else
typedef unsigned long FableNScriptUInt64;
enum EAIStateGroupType { };
enum ECameraOp { };
enum EClothingCoversArea { };
enum ECreatureGroup { };
enum ECrime { };
enum ECutsceneBehaviour { };
enum EDoorTriggerType { };
enum EGameAction { };
enum EHeroAbility { };
enum EHeroTitle { };
enum EHeroTrainableStatType { };
enum EMiniGameType { };
enum EMorality { };
enum EMusicSetType { };
enum EOpinion { };
enum EOpinionAttitudeType { };
enum EOpinionDeedType { };
enum EOpinionPostDeedType { };
enum EOpinionReactionType { };
enum EScriptAIPriority { };
enum EScriptVillageAttitude { };
enum EScriptingStateGroups { };
enum ESex { };
enum ETargetingType { };
enum ETextGroupSelectionMethod { };
enum ETutorialCategory { };
namespace NEntityEvents { enum EEventType { }; }
#endif

#endif // FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H

// End shared header engine/fable_nscript_forward.h

class CScriptBase : public CBaseClassNonCopyable {
public:
    CScriptBase();                                                       // 0x00CB8110
    virtual ~CScriptBase();                                              // slot 0  0x00CBD4F0 (scalar deleting); 0x00CBD510
protected:
    virtual void RegisterMain();                                         // slot 1  0x00CBD4B0
    virtual void Main();                                                 // slot 2  0x00CBD4C0
    virtual void Init();                                                 // slot 3  0x00CBD4D0
public:
    virtual void OnPersist(CPersistContext&);                            // slot 4  0x00CBD4E0
    void Activate();
    void SetPredicate(const NScript::CPredicateBase&);                   // 0x00CB7920
    void RemovePredicate();
    bool IsActiveThreadTerminating();                                    // 0x00CB7940
    CSpawnedFuncBase* GetPActiveThread();
    unsigned long GetThreadID();
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);     // 0x00CB7E50
    void KillSpawnedFunction(const CCharString&, unsigned long);
    void KillAllThreads();
    void RunSubThreads();
    void AddEntityScriptBinding(CEntityScriptBindingBase*);              // 0x00CB8230
    bool IsEntityScriptBound(const CCharString&) const;
    void PostAddScriptedEntities();                                      // 0x00CB8930
    void OnRegionLoaded(const CCharString&);
    void OnRegionUnloaded(const CCharString&);
    void OnScriptedEntityActivated(FableNScriptUInt64, CScriptThing&);
    void OnScriptedEntityDeactivated(FableNScriptUInt64, CScriptThing&, bool);
    void OnScriptedEntityInterrupted(FableNScriptUInt64);
    void LoadGameState(const CCharString&);
    void SaveGameState(CCharString&);
protected:
    bool UpdateSpawnedFunction(CSpawnedFuncBase*);
    void RunSpawnedFunctions();
    void RunEntityThreads();
    void DoScriptDataPersist(CCharString&, bool);
    void DoEntityScriptPersist(CActiveEntityScriptBase&, CCharString&, bool);
    void LoadEntityScript(FableNScriptUInt64, CActiveEntityScriptBase&);
    void SaveEntityScript(FableNScriptUInt64, CActiveEntityScriptBase&);
public:
    unsigned char     Threads[0x4];                                      // +0x04 std::list<CCountedPointer<CSpawnedFuncBase> >
    unsigned char     EntityScripts[0x10];                               // +0x08 CVectorMap<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>, ..>
    unsigned char     EntityScriptBindings[0x10];                        // +0x18 CVectorMap<CCharString, CCountedPointer<CEntityScriptBindingBase>, ..>
    unsigned long     LastWorldUpdateFrame;                              // +0x28
    CSpawnedFuncBase* PActiveThread;                                     // +0x2c
    unsigned char     RuntimeScriptedEntityPersist[0x10];                // +0x30 NScript::CRuntimeScriptedEntityPersist
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CScriptBase) == 0x40);
#endif
#endif

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CSCRIPTBASE_H

// End shared header engine/CScriptBase.h
class CGameScriptInterfaceBase;

template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    virtual ~CLevelScriptBase();
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CLEVELSCRIPTBASE_H

// End shared header engine/CLevelScriptBase.h
// Begin shared header engine/CGameScriptInterfaceBase.h
#ifndef FABLE_ENGINE_CGAMESCRIPTINTERFACEBASE_H
#define FABLE_ENGINE_CGAMESCRIPTINTERFACEBASE_H
// Retail polymorphic interface (hand-generated 2026-10-29 by the wave 1 class-model migration, scripts/gen_cgsi.py).
// RTTI .?AVCGameScriptInterfaceBase@@ : CBaseClassNonCopyable : CBaseClass (ghidra_out/rtti_bases.tsv); own vtable 0x0126009C
// = slot 0 scalar deleting dtor 0x006E7180 + 916 _purecall (0x00BFEA08) slots. Every slot is named from the overriding
// CGameScriptInterface (RTTI, vtable 0x01260F0C; ghidra_out/cgsi_slots_aligned.tsv): the mangled names are FableWin.pdb's
// (916 of them also in Ego_r.pdb; slot 2 TraceMessage is FableWin-only) and each declaration below is the VC7.1 undname
// spelling with the class prefix dropped. Overloads are declared in reverse vtable order (VC7.1 groups them, last declared
// first); the probe build of this header reproduces all 917 mangled names in retail slot order (HEADERS.md).
// Slot 378 EndLetterBox is protected (FableWin ?EndLetterBox@CGameScriptInterface@@MAEXXZ). One stand-in: slot 308 AddLogBookEntry takes CHeroLogBook::CHeroLogBookEntry::ECategory (a nested enum that cannot be
// forward-declared); it is declared `long` here (same slot, same 4-byte by-value ABI; its mangled name differs).
// Enums are MS forward declarations (VC7.1 extension); under g++ (C++98 has no opaque enums) they get empty placeholder
// definitions so the header still parses (the landed rows are only ever compiled by VC7.1).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CGameScriptInterfaceBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
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
// Begin shared header engine/fable_nscript_forward.h
#ifndef FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H
#define FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H
// Forward declarations shared by the FABLE_RETAIL_NSCRIPT headers (CScriptThing, CGameScriptInterfaceBase, CScriptBase,
// CEntityScriptBase, CTimer, the NScript predicates, CEntityScriptBinding<T>, CLevelScriptBase<T>). Class keys are the
// ones the FableWin.pdb / Ego_r.pdb mangled names carry (V = class, U = struct: CScreenFilterSThingByPass, std::pair,
// std::less). Enums: VC7.1 accepts an MS forward enum declaration (the row defines the enumerators it uses, before or
// after the include); C++98 has none, so under g++ (syntax checks only) each enum gets an empty placeholder definition.
// std templates: STLport 4.6 parameter lists without defaults (a row's own definition may add the defaults).

class C2DCoordI;
class C2DVector;
class C3DVector;
class CCharString;
class CCounter;
class CRGBColour;
class CRGBFloatColour;
class CRightHandedSet;
class CScriptGameResourceObjectMovieBase;
class CScriptGameResourceObjectScriptedThingBase;
class CScriptThing;
class CThing;
class CTimer;
class CWideString;
class CPersistContext;
class CScriptBase;
class CSpawnedFuncBase;
class CEntityScriptBindingBase;
class CScriptDataBase;
class CActiveEntityScriptBase;
struct CScreenFilterSThingByPass;
class CHeroLogBook;
template <class T> class CArray;
template <class T> class CCountedPointer;
namespace NScript { class CPredicateBase; }
namespace std {
template <class T> class allocator;
template <class T> struct less;
template <class T1, class T2> struct pair;
template <class T, class A> class vector;
template <class T, class A> class list;
template <class K, class C, class A> class set;
template <class K, class V, class C, class A> class map;
}
#if defined(_MSC_VER)
typedef unsigned __int64 FableNScriptUInt64;                           // the PDB's unsigned __int64 (thing unique IDs)
enum EAIStateGroupType;
enum ECameraOp;
enum EClothingCoversArea;
enum ECreatureGroup;
enum ECrime;
enum ECutsceneBehaviour;
enum EDoorTriggerType;
enum EGameAction;
enum EHeroAbility;
enum EHeroTitle;
enum EHeroTrainableStatType;
enum EMiniGameType;
enum EMorality;
enum EMusicSetType;
enum EOpinion;
enum EOpinionAttitudeType;
enum EOpinionDeedType;
enum EOpinionPostDeedType;
enum EOpinionReactionType;
enum EScriptAIPriority;
enum EScriptVillageAttitude;
enum EScriptingStateGroups;
enum ESex;
enum ETargetingType;
enum ETextGroupSelectionMethod;
enum ETutorialCategory;
namespace NEntityEvents { enum EEventType; }
#else
typedef unsigned long FableNScriptUInt64;
enum EAIStateGroupType { };
enum ECameraOp { };
enum EClothingCoversArea { };
enum ECreatureGroup { };
enum ECrime { };
enum ECutsceneBehaviour { };
enum EDoorTriggerType { };
enum EGameAction { };
enum EHeroAbility { };
enum EHeroTitle { };
enum EHeroTrainableStatType { };
enum EMiniGameType { };
enum EMorality { };
enum EMusicSetType { };
enum EOpinion { };
enum EOpinionAttitudeType { };
enum EOpinionDeedType { };
enum EOpinionPostDeedType { };
enum EOpinionReactionType { };
enum EScriptAIPriority { };
enum EScriptVillageAttitude { };
enum EScriptingStateGroups { };
enum ESex { };
enum ETargetingType { };
enum ETextGroupSelectionMethod { };
enum ETutorialCategory { };
namespace NEntityEvents { enum EEventType { }; }
#endif

#endif // FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H

// End shared header engine/fable_nscript_forward.h

class CGameScriptInterfaceBase : public CBaseClassNonCopyable {
public:
    virtual ~CGameScriptInterfaceBase();                                 // slot 0  0x006E7180 (scalar deleting)
    virtual void Error(const CCharString&, const CCharString&, const CCharString&, unsigned long) const = 0; //   1 0x0088DE00
    virtual void TraceMessage(const CCharString&) const = 0;                                             //   2 0x006E7D20
    virtual void Validate() const = 0;                                                                   //   3 0x006E72A0
    virtual void SetDebugCameraType(long) const = 0;                                                     //   4 0x0088E0F0
    virtual void DeactivateBoastUi() const = 0;                                                          //   5 0x008A0C60
    virtual bool IsXbox() const = 0;                                                                     //   6 0x0088E100
    virtual void NewScriptFrame() const = 0;                                                             //   7 0x006E7410
    virtual bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority) = 0; //   8 0x0089B5B0
    virtual bool IsEntityUnderScriptedControl(const CScriptThing&) const = 0;                            //   9 0x008960C0
    virtual bool IsActiveThreadTerminating() const = 0;                                                  //  10 0x006E71B0
    virtual bool IsLevelLoaded(const CCharString&) const = 0;                                            //  11 0x0088E2A0
    virtual bool IsRegionLoaded(const CCharString&) const = 0;                                           //  12 0x0088E300
    virtual bool IsRegionLoadedAndPreloaded(const CCharString&) const = 0;                               //  13 0x00890550
    virtual bool IsRegionDefLoaded(const CCharString&) const = 0;                                        //  14 0x00891DB0
    virtual const CCharString& GetRegionName() const = 0;                                                //  15 0x0088E340
    virtual bool MsgIsLevelLoaded(const CCharString&) const = 0;                                         //  16 0x008931B0
    virtual bool MsgIsLevelUnloaded(const CCharString&) const = 0;                                       //  17 0x00893240
    virtual bool MsgOnLevelLoaded(std::list<CCharString,std::allocator<CCharString> >&) const = 0;       //  18 0x0089AC10
    virtual bool MsgOnLevelUnloaded(std::list<CCharString,std::allocator<CCharString> >&) const = 0;     //  19 0x0089AC80
    virtual bool MsgIsRegionLoaded(const CCharString&) const = 0;                                        //  20 0x008932D0
    virtual bool MsgIsRegionUnloaded(const CCharString&) const = 0;                                      //  21 0x00893360
    virtual bool MsgOnRegionLoaded(CCharString&) const = 0;                                              //  22 0x008933F0
    virtual bool MsgOnRegionUnloaded(CCharString&) const = 0;                                            //  23 0x00893470
    virtual bool MsgOnRegionPreunload(CCharString&) const = 0;                                           //  24 0x008934F0
    virtual bool MsgOnQuestCompleted(const CCharString&) const = 0;                                      //  25 0x00893570
    virtual bool MsgOnAnyQuestCompleted(CCharString&) const = 0;                                         //  26 0x00893610
    virtual bool MsgOnQuestFailed(const CCharString&) const = 0;                                         //  27 0x00893690
    virtual bool MsgOnQuestCompletedBeforeScreenShown(const CCharString&) const = 0;                     //  28 0x00893730
    virtual bool MsgOnQuestFailedBeforeScreenShown(const CCharString&) const = 0;                        //  29 0x008937D0
    virtual bool MsgOnQuestAccepted(CCharString&) const = 0;                                             //  30 0x00893870
    virtual bool MsgOnFeatAccepted(long&) const = 0;                                                     //  31 0x008938F0
    virtual bool MsgIsBoastMade(long) const = 0;                                                         //  32 0x00893960
    virtual bool MsgOnBoastMade(long&, CCharString&) const = 0;                                          //  33 0x008939D0
    virtual bool MsgOnBoastsMade(std::vector<std::pair<long,CCharString>,std::allocator<std::pair<long,CCharString> > >&) const = 0; //  34 0x008A9AE0
    virtual void RemoveBoastMessage() const = 0;                                                         //  35 0x00893A50
    virtual bool IsQuestStartScreenActive(CCharString&) const = 0;                                       //  36 0x0089AD90
    virtual bool MsgOnLeavingQuestStartScreen(CCharString&) const = 0;                                   //  37 0x0089AE30
    virtual bool MsgOnLeavingExperienceSpendingScreen() const = 0;                                       //  38 0x00893AB0
    virtual long MsgIsQuestionAnsweredYesOrNo() const = 0;                                               //  39 0x00894300
    virtual bool MsgIsGameInfoClickedPast() const = 0;                                                   //  40 0x00894370
    virtual bool MsgIsTutorialClickedPast() const = 0;                                                   //  41 0x008943C0
    virtual bool MsgIsActionModeButtonPressed() const = 0;                                               //  42 0x00894440
    virtual bool MsgOnExpressionPerformed(CCharString&) const = 0;                                       //  43 0x00893B00
    virtual bool MsgIsCutSceneSkipped() const = 0;                                                       //  44 0x00893BA0
    virtual void RemoveAllCutSceneSkippedMessages() const = 0;                                           //  45 0x0089AF30
    virtual bool MsgOnHeroHairTypeChanged(EClothingCoversArea, CCharString&) const = 0;                  //  46 0x00893C00
    virtual bool MsgOnHeroUsedTeleporter(CCharString&) const = 0;                                        //  47 0x00893D80
    virtual bool MsgOnHeroUsedGuildSeal() const = 0;                                                     //  48 0x00893E20
    virtual bool MsgOnGameSavedManually() const = 0;                                                     //  49 0x00893E70
    virtual bool MsgOnHeroSlept(bool&) const = 0;                                                        //  50 0x00893EC0
    virtual bool MsgOnHeroFiredRangedWeapon() const = 0;                                                 //  51 0x00893F30
    virtual bool MsgOnHeroCastSpell(EHeroAbility&) const = 0;                                            //  52 0x00893F80
    virtual bool MsgOnHeroPickedPocket(CScriptThing&) const = 0;                                         //  53 0x00893FF0
    virtual bool MsgOnHeroPickedLock(CScriptThing&) const = 0;                                           //  54 0x00894080
    virtual bool MsgOnFishingGameFinished(CScriptThing&) const = 0;                                      //  55 0x00894110
    virtual bool MsgOnTavernGameFinished(const CScriptThing&) const = 0;                                 //  56 0x008941A0
    virtual bool MsgOnHeroRewardedWithItemsFrom(CCharString&) const = 0;                                 //  57 0x00894250
    virtual bool MsgOnChestOpeningCancelled() const = 0;                                                 //  58 0x008942C0
    virtual void DontPopulateNextLoadedRegion() const = 0;                                               //  59 0x0088E380
    virtual void GetWanderingPopulationScriptDefNameInCurrentRegion(CCharString&) const = 0;             //  60 0x0088E390
    virtual void GetWanderingPopulationScriptDefNameInRegion(const CCharString&, CCharString&) const = 0; //  61 0x00891E30
    virtual bool IsHeroAllowedHenchmenInCurrentRegion() const = 0;                                       //  62 0x00891EA0
    virtual bool IsHeroAllowedHenchmenInRegion(const CCharString&) const = 0;                            //  63 0x00891EF0
    virtual void PostAddScriptedEntities() const = 0;                                                    //  64 0x006E7460
    virtual bool IsPlayerHoldingUnsheatheRangedWeaponButton() const = 0;                                 //  65 0x008905B0
    virtual bool IsPlayerHoldingLockTargetButton() const = 0;                                            //  66 0x008905E0
    virtual bool IsPlayerHoldingFireRangedWeaponButton() const = 0;                                      //  67 0x00890610
    virtual bool IsPlayerHoldingFirstPersonTargetingButton() const = 0;                                  //  68 0x00890640
    virtual bool IsHeroInProjectileWeaponMode() const = 0;                                               //  69 0x00890670
    virtual CScriptThing& GetHero() = 0;                                                                 //  70 0x00891CA0
    virtual CScriptThing GetHeroTargetedThing() const = 0;                                               //  71 0x0089A490
    virtual CScriptThing GetThingWithScriptName(const CCharString&, const CCharString&) const = 0;       //  73 0x008930F0
    virtual CScriptThing GetThingWithScriptName(const CCharString&) const = 0;                           //  72 0x008A7D60
    virtual CScriptThing GetRandomThingWithScriptName(const CCharString&) const = 0;                     //  74 0x008A7E00
    virtual long GetAllThingsWithScriptName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; //  75 0x008A8570
    virtual long GetAllCreaturesInAreaWithScriptName(const CCharString&, const C3DVector&, float, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; //  76 0x008A86C0
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing&, const CCharString&) const = 0;    //  77 0x0089A510
    virtual CScriptThing GetFurthestWithScriptName(const CScriptThing&, const CCharString&) const = 0;   //  78 0x0089A6E0
    virtual long GetAllThingsWithDefNameByDistanceFrom(const CScriptThing&, const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; //  79 0x008A9C40
    virtual long GetAllThingsWithDefName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; //  80 0x008A8890
    virtual CScriptThing GetNearestWithDefName(const CScriptThing&, const CCharString&) const = 0;       //  81 0x0089A880
    virtual CScriptThing GetFurthestWithDefName(const CScriptThing&, const CCharString&) const = 0;      //  82 0x0089AA70
    virtual CScriptThing GetThingWithUID(unsigned __int64) const = 0;                                    //  83 0x00891D50
    virtual long GetAllCreaturesExcludingHero(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; //  84 0x008A89D0
    virtual long GetAllThingsInLevel(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; //  85 0x008A8AF0
    virtual bool IsThingWithThisUIDAlive(unsigned __int64) const = 0;                                    //  86 0x0088E260
    virtual long RegisterTimer() = 0;                                                                    //  87 0x006E79E0
    virtual void DeregisterTimer(long) = 0;                                                              //  88 0x006E7690
    virtual void SetTimer(long, long) = 0;                                                               //  89 0x006E76E0
    virtual long GetTimer(long) const = 0;                                                               //  90 0x006E7710
    virtual CScriptThing CreateCreature(const CCharString&, const C3DVector&, const CCharString&, bool) const = 0; //  91 0x008A9100
    virtual CScriptThing CreateCreatureNearby(const CCharString&, const C3DVector&, float, const CCharString&, bool) const = 0; //  92 0x0089F300
    virtual CScriptThing CreateCreatureOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const = 0; //  93 0x00894500
    virtual CScriptThing TurnCreatureInto(const CScriptThing&, const CCharString&) const = 0;            //  94 0x0089F4A0
    virtual void SetCreatureCreationDelayFrames(long) = 0;                                               //  95 0x006E7D30
    virtual void ResetCreatureCreationDelayFrames() = 0;                                                 //  96 0x006E7D40
    virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, const CCharString&) const = 0; //  98 0x00894820
    virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, float, const CCharString&) const = 0; //  97 0x00894950
    virtual CScriptThing CreateObjectOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const = 0; //  99 0x008946E0
    virtual CScriptThing CreateEffect(const CCharString&, const CScriptThing&, const CCharString&, const CCharString&, bool, bool) const = 0; // 101 0x0089F910
    virtual CScriptThing CreateEffect(const CCharString&, const C3DVector&, const CCharString&, float, bool, bool) const = 0; // 100 0x0089F9E0
    virtual CScriptThing CreateLight(const C3DVector&, const CRGBColour&, const CCharString&, float, float, bool) const = 0; // 102 0x0089FAA0
    virtual CScriptThing CreateExperienceOrb(const C3DVector&, long) const = 0;                          // 103 0x0089FBB0
    virtual CScriptThing CreateExplosion(const CCharString&, const C3DVector&, const C3DVector*, CCharString) const = 0; // 104 0x0089FC80
    virtual CScriptThing CreatePhysicalBarrier(float, const C3DVector&, const C3DVector*, CCharString) const = 0; // 105 0x0089FD20
    virtual CScriptThing CreateRumble(const C3DVector&, float, float, CCharString) const = 0;            // 106 0x0089FDA0
    virtual void ClearAllRumbles() const = 0;                                                            // 107 0x00892DF0
    virtual void RemoveThing(const CScriptThing&, bool, bool) const = 0;                                 // 108 0x008910D0
    virtual void ShowOnScreenMessage(const CCharString&, float) const = 0;                               // 111 0x00892780
    virtual void ShowOnScreenMessage(const CCharString&, const CCharString&, const CCharString&, float) const = 0; // 110 0x008927B0
    virtual void ShowOnScreenMessage(const C2DVector&, const CCharString&, const CRGBColour&, const CCharString&) const = 0; // 109 0x00892810
    virtual void AddScreenMessage(const CCharString&, ETextGroupSelectionMethod) const = 0;              // 112 0x00892850
    virtual void AddScreenTitleMessage(const CCharString&, float, bool) const = 0;                       // 113 0x0089E4C0
    virtual void GiveHeroYesNoQuestion(const CCharString&, const CCharString&, const CCharString&, const CCharString&, bool) const = 0; // 114 0x0089E540
    virtual void DisplayGameInfo(const CCharString&) const = 0;                                          // 115 0x008929D0
    virtual void DisplayGameInfoText(const CWideString&) const = 0;                                      // 116 0x00892A80
    virtual bool IsSafeToDisplayGameInfo() const = 0;                                                    // 117 0x00892B30
    virtual bool DisplayTutorial(ETutorialCategory) const = 0;                                           // 118 0x0089E710
    virtual bool IsTutorialSystemEnabled() const = 0;                                                    // 119 0x0088F3E0
    virtual void GiveHeroWeapon(const CCharString&, bool) const = 0;                                     // 120 0x0088E130
    virtual void GiveHeroObject(const CCharString&, long, bool) const = 0;                               // 121 0x008902E0
    virtual void SetWeaponAsHerosActiveWeapon(const CCharString&) const = 0;                             // 122 0x00898B30
    virtual void GiveHeroItem(const CScriptThing&) const = 0;                                            // 123 0x00898BB0
    virtual bool GiveHeroItemsFromContainer(const CScriptThing&, bool) const = 0;                        // 124 0x00898C40
    virtual void TakeObjectFromHero(const CCharString&) const = 0;                                       // 125 0x00898DB0
    virtual void GiveHeroGold(long) const = 0;                                                           // 126 0x00898F40
    virtual long GetHeroGold() const = 0;                                                                // 127 0x00898FA0
    virtual void GiveHeroExperience(long) const = 0;                                                     // 128 0x00899000
    virtual void SetHeroAbleToGainExperience(bool) const = 0;                                            // 129 0x00899060
    virtual void SheatheHeroWeapons() const = 0;                                                         // 130 0x008916A0
    virtual void SetHeroWillAsUsable(bool) const = 0;                                                    // 131 0x00898340
    virtual void SetHeroWeaponsAsUsable(bool) const = 0;                                                 // 132 0x00898410
    virtual void SetWeaponOutCrimeEnabled(bool) const = 0;                                               // 133 0x008984E0
    virtual void SetGuardsIgnoreCrimes(bool) const = 0;                                                  // 134 0x00898540
    virtual void RemoveAllHeroWeapons() const = 0;                                                       // 135 0x00898600
    virtual bool IsReportedOrUnreportedCrimeKnown(const CScriptThing&) const = 0;                        // 136 0x008985A0
    virtual void ConfiscateAllHeroItems() const = 0;                                                     // 137 0x008986D0
    virtual void ConfiscateAllHeroWeapons() const = 0;                                                   // 138 0x008987E0
    virtual void ConfiscateItemsOfTypeFromHero(const CCharString&) const = 0;                            // 139 0x00898840
    virtual void ReturnAllConfiscatedItemsToHero() const = 0;                                            // 140 0x00898940
    virtual bool GiveHeroTutorial(ETutorialCategory) const = 0;                                          // 141 0x00898A50
    virtual void MakeHeroCarryItemInHand(const CCharString&) const = 0;                                  // 143 0x00897E10
    virtual void MakeHeroCarryItemInHand(const CScriptThing&, bool, bool) const = 0;                     // 142 0x00897FA0
    virtual void AddTattooToHero(const CCharString&) const = 0;                                          // 144 0x008981C0
    virtual void GiveHeroAbility(EHeroAbility, bool) const = 0;                                          // 145 0x0088FCC0
    virtual bool IsPlayerZTargetingThing(const CScriptThing&) const = 0;                                 // 146 0x00899B10
    virtual bool IsPlayerCreatureBlocking() const = 0;                                                   // 147 0x00899BA0
    virtual bool IsPlayerCreatureReadyToFireProjectileWeapon(float&) const = 0;                          // 148 0x00899C10
    virtual long GetPlayerCreatureCombatMultiplier() const = 0;                                          // 149 0x00899C90
    virtual long GetPlayerCreatureCombatMultiplierRunningNumHits() const = 0;                            // 150 0x00899CF0
    virtual void ResetPlayerCreatureCombatMultiplier() const = 0;                                        // 151 0x00899D50
    virtual bool IsPlayerCreatureFlourishEnabled() const = 0;                                            // 152 0x00899DB0
    virtual void SetPlayerCreatureOnlyTarget(const CScriptThing&) const = 0;                             // 153 0x00899E10
    virtual void ResetPlayerCreatureOnlyTarget() const = 0;                                              // 154 0x00899E90
    virtual void RespawnHero(bool) const = 0;                                                            // 155 0x0088E150
    virtual void GiveHeroMorality(float) const = 0;                                                      // 156 0x00899EF0
    virtual float GetHeroMorality() const = 0;                                                           // 157 0x00899F90
    virtual EMorality GetHeroMoralityCategory() const = 0;                                               // 158 0x0089A000
    virtual void ChangeHeroMoralityDueToTheft() const = 0;                                               // 159 0x0089A070
    virtual void ChangeHeroMoralityDueToPicklock() const = 0;                                            // 160 0x0089A0E0
    virtual void GiveHeroRenownPoints(long) const = 0;                                                   // 161 0x008990C0
    virtual long GetHeroRenownLevel() const = 0;                                                         // 162 0x00899140
    virtual bool IsHeroRenownLevelFull() const = 0;                                                      // 163 0x008991A0
    virtual void IncreaseHeroRenownLevel() const = 0;                                                    // 164 0x00899220
    virtual long GetHeroStrengthLevel() const = 0;                                                       // 165 0x00899390
    virtual long GetHeroSkillLevel() const = 0;                                                          // 166 0x00899400
    virtual long GetHeroWillLevel() const = 0;                                                           // 167 0x00899470
    virtual long GetHeroStatLevel(EHeroTrainableStatType) const = 0;                                     // 168 0x00899290
    virtual long GetHeroStatMax(EHeroTrainableStatType) const = 0;                                       // 169 0x00899310
    virtual void SetHeroAge(float) const = 0;                                                            // 170 0x008994E0
    virtual float GetHeroAge() const = 0;                                                                // 171 0x00899560
    virtual void SetHeroAsTeenager(bool) const = 0;                                                      // 172 0x008995D0
    virtual void SetHeroAsApprentice(bool) const = 0;                                                    // 173 0x00899680
    virtual float GetDistanceHeroCanBeHeardFrom() const = 0;                                             // 174 0x00899700
    virtual long GetHeroRoughExperienceLevel() const = 0;                                                // 175 0x00899770
    virtual long GetHeroExperienceAvailableToSpend() const = 0;                                          // 176 0x008997E0
    virtual float GetHeroFatness() const = 0;                                                            // 177 0x00899850
    virtual float GetHeroScariness() const = 0;                                                          // 178 0x008998C0
    virtual float GetHeroAttractiveness() const = 0;                                                     // 179 0x00899930
    virtual float GetHeroWillEnergyLevel() const = 0;                                                    // 180 0x008999A0
    virtual void SetHeroWillEnergyLevel(float) const = 0;                                                // 181 0x00899A10
    virtual void SetHeroWillEnergyAsAbleToRefill(bool) const = 0;                                        // 182 0x00899A90
    virtual long GetNumberOfItemsOfTypeInInventory(const CCharString&) const = 0;                        // 183 0x00897190
    virtual bool IsObjectInThingsPossession(const CCharString&, const CScriptThing&) const = 0;          // 184 0x00897220
    virtual bool IsHeroHandLampLit() const = 0;                                                          // 185 0x00898E60
    virtual void SetHeroHandLampAsLit(bool) const = 0;                                                   // 186 0x00898EC0
    virtual bool IsWearingClothingItem(const CScriptThing&, const CCharString&) const = 0;               // 187 0x0089B2B0
    virtual bool IsHeroNaked() const = 0;                                                                // 188 0x0089B330
    virtual void RemoveHeroClothing() const = 0;                                                         // 189 0x0089B390
    virtual void SetHeroAsWearing(const CCharString&) const = 0;                                         // 190 0x0089B3F0
    virtual void ChangeHeroHairstyle(const CCharString&) const = 0;                                      // 191 0x0089B450
    virtual void RemoveHeroHairstyle() const = 0;                                                        // 192 0x0089B4D0
    virtual bool IsWearingHairstyle(CScriptThing&, const CCharString&) const = 0;                        // 193 0x0089B530
    virtual bool IsPlayerCarryingItemOfType(const CCharString&) const = 0;                               // 194 0x008973A0
    virtual bool IsPlayerWieldingWeapon() const = 0;                                                     // 195 0x00897480
    virtual bool IsEntityWieldingWeapon(const CScriptThing&) const = 0;                                  // 196 0x008974F0
    virtual bool IsEntityWieldingMeleeWeapon(const CScriptThing&) const = 0;                             // 197 0x00890220
    virtual bool IsEntityWieldingRangedWeapon(const CScriptThing&) const = 0;                            // 198 0x00890250
    virtual bool GetPreviouslyWieldedMeleeWeaponName(CCharString&) const = 0;                            // 199 0x00897560
    virtual bool GetPreviouslyWieldedRangedWeaponName(CCharString&) const = 0;                           // 200 0x00897630
    virtual bool IsEntityAbleToAttack(const CScriptThing&) const = 0;                                    // 201 0x00890280
    virtual CScriptThing EntityGetThingInPrimarySlot(const CScriptThing&) const = 0;                     // 202 0x008976F0
    virtual void ApplyHeroPenaltyForDeath() const = 0;                                                   // 203 0x008977F0
    virtual void GiveHeroTitle(const CCharString&) const = 0;                                            // 204 0x0088E110
    virtual EHeroTitle GetHeroTitle() const = 0;                                                         // 205 0x00897880
    virtual void EntitySetAsMarryable(const CScriptThing&, bool) const = 0;                              // 206 0x00894DD0
    virtual void EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing&, bool) const = 0;          // 207 0x008978E0
    virtual void EntityForceMarriageToHero(const CScriptThing&, bool) const = 0;                         // 208 0x00897940
    virtual bool IsEntityMarriedToHero(const CScriptThing&) const = 0;                                   // 209 0x008979C0
    virtual bool IsEntityMarriable(const CScriptThing&) const = 0;                                       // 210 0x00897A30
    virtual bool GetHeroHasMarried() const = 0;                                                          // 211 0x00897AA0
    virtual bool GetHeroHasCurrentMarriage() const = 0;                                                  // 212 0x00897B10
    virtual bool GetHeroHasDivorcedMarriage() const = 0;                                                 // 213 0x00897B70
    virtual bool GetHeroHasChildren() const = 0;                                                         // 214 0x00897BD0
    virtual bool GetHeroHasMurderedWife() const = 0;                                                     // 215 0x00897C30
    virtual bool IsHeroChild() const = 0;                                                                // 216 0x008902B0
    virtual void CancelHeroTeleportEffects() const = 0;                                                  // 217 0x008A0820
    virtual long GetNumberOfTimesHeroHasHadSex() const = 0;                                              // 218 0x00897C90
    virtual void SetNumberOfTimesHeroHasHadSex(long) const = 0;                                          // 219 0x00897CF0
    virtual void SetHeroAsHavingHadSex(bool) const = 0;                                                  // 220 0x00897D50
    virtual void SetHeroAsHavingHadGaySex(bool) const = 0;                                               // 221 0x00897DB0
    virtual void GiveThingHeroRewardItem(const CScriptThing&, const CCharString&, const CCharString&) const = 0; // 222 0x00898AC0
    virtual void GiveThingItemInHand(const CScriptThing&, const CCharString&, bool) const = 0;           // 223 0x0088DE10
    virtual void GiveThingItemInSlot(const CScriptThing&, const CCharString&, const CCharString&) const = 0; // 224 0x00895190
    virtual void GiveHeroExpression(const CCharString&, long, bool) const = 0;                           // 225 0x0088FC60
    virtual bool HeroHasExpression(const CCharString&) const = 0;                                        // 226 0x0088FC90
    virtual bool IsHeroPerformingExpression(const CCharString&) const = 0;                               // 227 0x008953A0
    virtual bool IsDPadButtonHeldForExpression(const CCharString&) const = 0;                            // 228 0x00895430
    virtual void EntityFollowThing(const CScriptThing&, const CScriptThing&, float, bool) const = 0;     // 229 0x008954A0
    virtual void EntityStopFollowing(const CScriptThing&) const = 0;                                     // 230 0x008A7980
    virtual bool GetFollowingEntityList(const CScriptThing&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; // 231 0x008A80F0
    virtual bool GetHeroSummonedCreaturesList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; // 232 0x008A8260
    virtual bool GetPerceivingHeroEntityList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; // 233 0x008A8440
    virtual bool IsEntityFollowingHero(const CScriptThing&) const = 0;                                   // 234 0x008955C0
    virtual void EntitySetAsAllowedToFollowHero(const CScriptThing&, bool) const = 0;                    // 235 0x0088DE90
    virtual void EntitySetAsAllowedToChangeRegionFollowingState(const CScriptThing&, bool) const = 0;    // 236 0x0088DED0
    virtual void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing&, bool) const = 0;   // 237 0x0088DF20
    virtual void EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(const CScriptThing&, bool) const = 0; // 238 0x00895650
    virtual void TeleportAllFollowersToHeroPosition() const = 0;                                         // 239 0x008A7A70
    virtual void EntityTeleportToHeroPosition(const CScriptThing&) const = 0;                            // 240 0x008956B0
    virtual void SendEntityEvent(NEntityEvents::EEventType, const CScriptThing&, const CScriptThing&, CThing*) const = 0; // 241 0x008957C0
    virtual float GetWaterHeightAtPosition(const C3DVector&) const = 0;                                  // 242 0x0088DF60
    virtual bool IsFishingSpotEnabled(const CScriptThing&) const = 0;                                    // 243 0x008958F0
    virtual void DisableFishingSpot(const CScriptThing&) const = 0;                                      // 244 0x00895950
    virtual void UpdateFishWeight(const CScriptThing&) const = 0;                                        // 245 0x008959B0
    virtual float GetBestFishWeight(const CCharString&) const = 0;                                       // 246 0x00895A10
    virtual void HeroGoFishing(bool) const = 0;                                                          // 247 0x00895A90
    virtual long GetHeroFishingLevel() const = 0;                                                        // 248 0x00895B00
    virtual void HeroGoDigging() const = 0;                                                              // 249 0x0088DF80
    virtual void HeroStopDigging() const = 0;                                                            // 250 0x0088DFA0
    virtual void HeroPlayOracleMinigame() const = 0;                                                     // 251 0x00895B60
    virtual bool IsHeroPlayingOracleMinigame() const = 0;                                                // 252 0x00895BC0
    virtual bool HasHeroWonOracleMinigame() const = 0;                                                   // 253 0x00895C30
    virtual void HeroPlayFireheartMinigame() const = 0;                                                  // 254 0x00895C90
    virtual void HeroQuitFireheartMinigame() const = 0;                                                  // 255 0x00895CF0
    virtual bool HasHeroForceQuitFireheartMinigame() const = 0;                                          // 256 0x00895D50
    virtual float GetHeroHealth() const = 0;                                                             // 257 0x00890370
    virtual float GetHeroHealthMax() const = 0;                                                          // 258 0x008903A0
    virtual float GetHeroHealthPercentage() const = 0;                                                   // 259 0x008903D0
    virtual long GetHeroWillEnergy() const = 0;                                                          // 260 0x0089A150
    virtual long GetHeroWillEnergyMax() const = 0;                                                       // 261 0x0089A1B0
    virtual long GetHeroWillEnergyPercentage() const = 0;                                                // 262 0x0089A210
    virtual void ChangeHeroHealthBy(float, bool, bool) const = 0;                                        // 263 0x00891720
    virtual float GetHealth(const CScriptThing&) const = 0;                                              // 264 0x00890400
    virtual void ModifyThingHealth(const CScriptThing&, float, bool) const = 0;                          // 265 0x00890430
    virtual void EntitySetMaxHealth(const CScriptThing&, float, bool) const = 0;                         // 266 0x0088E1C0
    virtual void SetThingAsKilled(const CScriptThing&) const = 0;                                        // 267 0x0089A270
    virtual void GiveHeroNewQuestObjective(const CCharString&, unsigned long) const = 0;                 // 268 0x008917F0
    virtual void TellHeroQuestObjectiveCompleted(unsigned long) const = 0;                               // 269 0x00891800
    virtual void TellHeroQuestObjectiveFailed(unsigned long) const = 0;                                  // 270 0x00891810
    virtual void AddQuestRegion(const CCharString&, const CCharString&) const = 0;                       // 271 0x00891820
    virtual void SetQuestWorldMapOffset(const CCharString&, const C2DCoordI&) const = 0;                 // 272 0x00891860
    virtual bool IsHeroOnQuest() const = 0;                                                              // 273 0x00891870
    virtual void HeroReceiveMessageFromGuildMaster(const CCharString&, const CCharString&, bool, bool) const = 0; // 274 0x008918C0
    virtual void SetGuildMasterMessages(bool) const = 0;                                                 // 275 0x0088E200
    virtual void ActivateQuest(const CCharString&) const = 0;                                            // 276 0x00892E80
    virtual void ActivateMultipleQuests(const CArray<CCharString>&) const = 0;                           // 277 0x00892EA0
    virtual void ActivateQuestWithoutLoadingResources(const CCharString&) const = 0;                     // 278 0x00892EC0
    virtual void ActivateMultipleQuestsWithoutLoadingResources(const CArray<CCharString>&) const = 0;    // 279 0x00892EE0
    virtual void DeactivateQuest(const CCharString&, unsigned long) const = 0;                           // 280 0x00892F00
    virtual void DeactivateQuestLater(const CCharString&, unsigned long) const = 0;                      // 281 0x00892F10
    virtual void PrepareQuestsWhenFinalQuestIsActivated(const CCharString&) const = 0;                   // 282 0x00892F20
    virtual void PrepareQuestsWhenFinalQuestIsCompleted() const = 0;                                     // 283 0x00892F30
    virtual bool IsQuestActive(const CCharString&) const = 0;                                            // 284 0x00892F40
    virtual bool IsQuestRegistered(const CCharString&) const = 0;                                        // 285 0x00892F50
    virtual bool IsQuestCompleted(const CCharString&) const = 0;                                         // 286 0x00892F60
    virtual bool IsQuestFailed(const CCharString&) const = 0;                                            // 287 0x00892F70
    virtual void SetQuestAsCompleted(const CCharString&, bool, bool, bool) const = 0;                    // 288 0x00892F80
    virtual void SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool) const = 0;         // 289 0x00892FD0
    virtual void SetQuestAsPersistent(const CCharString&, bool) const = 0;                               // 290 0x00893020
    virtual const CCharString& GetExclusiveQuestScriptName() const = 0;                                  // 291 0x00893030
    virtual void AddQuestCard(const CCharString&, const CCharString&, bool, bool) const = 0;             // 292 0x008913F0
    virtual void RemoveQuestCardFromGuild(const CCharString&) const = 0;                                 // 293 0x00891490
    virtual void RemoveQuestCardFromHero(const CCharString&) const = 0;                                  // 294 0x008914E0
    virtual void GiveHeroQuestCardDirectly(const CCharString&, const CCharString&, bool) const = 0;      // 295 0x008968C0
    virtual void SetQuestCardObjective(const CCharString&, const CCharString&, const CCharString&, const CCharString&) const = 0; // 296 0x00896A30
    virtual void SetQuestCardGoldReward(const CCharString&, long) const = 0;                             // 297 0x008914F0
    virtual void SetQuestCardRenownReward(const CCharString&, long) const = 0;                           // 298 0x00891500
    virtual void RemoveAllAvailableQuestCardsFromGuild() const = 0;                                      // 299 0x00891510
    virtual void FailAllActiveQuests() const = 0;                                                        // 300 0x00891520
    virtual void GetAllActiveQuestInfo(std::vector<CCharString,std::allocator<CCharString> >&, std::vector<CCharString,std::allocator<CCharString> >&) const = 0; // 301 0x00891530
    virtual void AddFeatCard(long, const CCharString&, const CCharString&) const = 0;                    // 302 0x00891540
    virtual void AddBoast(const CCharString&, long, long, long, bool, const CCharString&, long) const = 0; // 303 0x00893060
    virtual void RemoveBoast(long, const CCharString&) const = 0;                                        // 304 0x00891580
    virtual void SetBoastAsFailed(long, const CCharString&) const = 0;                                   // 305 0x008915B0
    virtual void SetBoastAsCompleted(long, const CCharString&) const = 0;                                // 306 0x008915E0
    virtual bool IsBoastTaken(long, const CCharString&) const = 0;                                       // 307 0x00891550
    virtual void AddLogBookEntry(const CWideString&, const CWideString&, const CWideString&, long) const = 0; // 308 0x0088FE00
    virtual void KickOffQuestStartScreen(const CCharString&, bool, bool) const = 0;                      // 309 0x00896B60
    virtual void KickOffDeathScreen() const = 0;                                                         // 310 0x00896D20
    virtual void KickOffCreditsScreen(CCharString&) const = 0;                                           // 311 0x00896DF0
    virtual void SetPreferredQuickAccessItem(const CCharString&, long, long) const = 0;                  // 312 0x00896E60
    virtual CCharString GetDeathRecoveryMarkerName() const = 0;                                          // 313 0x00896EE0
    virtual void SetDeathRecoveryMarkerName(const CCharString&) const = 0;                               // 314 0x00896F60
    virtual void ResetDeathRecoveryMarkerNameToDefault() const = 0;                                      // 315 0x00896FD0
    virtual bool IsToFailQuestOnDeath() const = 0;                                                       // 316 0x00897030
    virtual void SetWhetherToFailQuestOnDeath(bool) const = 0;                                           // 317 0x00897090
    virtual void ResetWhetherToFailQuestOnDeathToDefault() const = 0;                                    // 318 0x008970F0
    virtual CScriptThing GetMostRecentValidUsedTarget() const = 0;                                       // 319 0x00891610
    virtual CCharString GetMostRecentValidUsedTargetName() const = 0;                                    // 320 0x00891680
    virtual void DisplayQuestInfo(bool) const = 0;                                                       // 321 0x00891920
    virtual void SetQuestInfoName(const char*) const = 0;                                                // 322 0x00891940
    virtual void SetQuestInfoText(const char*) const = 0;                                                // 323 0x00891A00
    virtual long AddQuestInfoBar(float, float, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float) const = 0; // 324 0x00891AC0
    virtual long AddQuestInfoBarHealth(const CScriptThing&, const CRGBColour&, const CCharString&, float) const = 0; // 325 0x00891AD0
    virtual long AddQuestInfoTimer(const CTimer&, const CCharString&, float) const = 0;                  // 326 0x00891B20
    virtual long AddQuestInfoCounter(const CCounter&, const CCharString&, float) const = 0;              // 328 0x00891B30
    virtual long AddQuestInfoCounter(const CCharString&, long, float) const = 0;                         // 327 0x00891B40
    virtual long AddQuestInfoCounterList(const CCharString&, long, float) const = 0;                     // 329 0x00891B50
    virtual long AddQuestInfoTick(const CCharString&, bool, float) const = 0;                            // 331 0x00891B60
    virtual long AddQuestInfoTick(EGameAction, bool, float) const = 0;                                   // 330 0x00891B70
    virtual void UpdateQuestInfoBar(long, float, float, float) const = 0;                                // 332 0x00891B80
    virtual void ChangeQuestInfoBarColour(long, const CRGBColour&, const CRGBColour&) const = 0;         // 333 0x00891BA0
    virtual void UpdateQuestInfoTimer(long, float) const = 0;                                            // 334 0x00891BB0
    virtual void UpdateQuestInfoCounter(long, long, long) const = 0;                                     // 335 0x00891BD0
    virtual void UpdateQuestInfoCounterList(long, long, long) const = 0;                                 // 336 0x00891BE0
    virtual void UpdateQuestInfoTick(long, bool) const = 0;                                              // 337 0x00891BF0
    virtual void RemoveQuestInfoElement(long) const = 0;                                                 // 338 0x00891C10
    virtual void RemoveAllQuestInfoElements() const = 0;                                                 // 339 0x00891C30
    virtual void DisplayTime(bool) const = 0;                                                            // 340 0x0088E210
    virtual void DisplayMoneyBag(bool) const = 0;                                                        // 341 0x00891C40
    virtual void DisplayMiniGameInfo(bool, EMiniGameType) const = 0;                                     // 342 0x00891C60
    virtual void UpdateMiniGameInfoBar(float) const = 0;                                                 // 343 0x00891C80
    virtual bool IsEntityPickPocketable(const CScriptThing&) const = 0;                                  // 344 0x0088E220
    virtual bool IsEntityPickLockable(const CScriptThing&) const = 0;                                    // 345 0x0089A2D0
    virtual bool IsEntityStealable(const CScriptThing&) const = 0;                                       // 346 0x0089A330
    virtual void EntitySetAsPickPocketed(const CScriptThing&) const = 0;                                 // 347 0x00890470
    virtual void EntitySetAsPickLocked(const CScriptThing&) const = 0;                                   // 348 0x0089A390
    virtual void EntitySetAsStolen(const CScriptThing&) const = 0;                                       // 349 0x008904E0
    virtual void MiniMapAddMarker(const CScriptThing&, const CCharString&) const = 0;                    // 350 0x00891F50
    virtual void MiniMapSetMarkerGraphic(const CScriptThing&, const CCharString&) const = 0;             // 351 0x00891FA0
    virtual void MiniMapRemoveMarker(const CScriptThing&) const = 0;                                     // 352 0x00891FF0
    virtual void MiniMapRemoveAllMarkers() const = 0;                                                    // 353 0x00892030
    virtual void MiniMapAllowRouteBetweenRegions(const CCharString&, const CCharString&, bool) const = 0; // 354 0x0088E3D0
    virtual void MiniMapSetAsEnabled(bool) const = 0;                                                    // 355 0x0088E400
    virtual void EntitySetAsHiddenOnMiniMap(const CScriptThing&, bool) const = 0;                        // 356 0x0088E410
    virtual void SetHUDEnabled(bool) const = 0;                                                          // 357 0x00892050
    virtual bool TextEntryExists(const CCharString&) const = 0;                                          // 358 0x00892070
    virtual CCharString GetValidTextEntryNameWithAttitude(const CScriptThing&, const CCharString&) const = 0; // 359 0x008920C0
    virtual void SetThingHasInformation(const CScriptThing&, bool, bool, bool) const = 0;                // 360 0x0088E440
    virtual void ClearThingHasInformation(const CScriptThing&) const = 0;                                // 361 0x0088E480
    virtual void EntitySetWillBeUsingNarrator(const CScriptThing&, const CCharString&) const = 0;        // 362 0x0089B050
    virtual void EntityResetAsPureAINarrator(const CScriptThing&) const = 0;                             // 363 0x0089B0B0
    virtual long AddNewConversation(const CScriptThing&, bool, bool) const = 0;                          // 364 0x008906C0
    virtual void AddPersonToConversation(long, const CScriptThing&) const = 0;                           // 365 0x00890710
    virtual void AddLineToConversation(long, const CCharString&, bool, const CScriptThing&, const CScriptThing&) const = 0; // 366 0x00890750
    virtual void RemoveConversation(long, bool) const = 0;                                               // 367 0x008907C0
    virtual bool IsConversationActive(long) const = 0;                                                   // 368 0x008907D0
    virtual void PlayAVIMovie(const CCharString&) const = 0;                                             // 369 0x0088F890
    virtual void StartMovieSequence(const CCharString&, CScriptGameResourceObjectMovieBase&) = 0;        // 370 0x0089B110
    virtual void FixMovieSequenceCamera(bool) const = 0;                                                 // 371 0x0088E500
    virtual void FadeScreenOutUntilNextCallToFadeScreenIn(float, float) = 0;                             // 372 0x008907E0
    virtual bool FadeScreenOut(float, float, CRGBColour) = 0;                                            // 373 0x00890820
    virtual void FadeScreenIn() = 0;                                                                     // 374 0x0088E4C0
    virtual bool IsScreenFadingOut() const = 0;                                                          // 375 0x0088E4D0
    virtual void Pause(float) const = 0;                                                                 // 376 0x0088E4F0
    virtual void EndCutFade() = 0;                                                                       // 377 0x0088E520
    protected: virtual void EndLetterBox() = 0; public:                                                  // 378 0x006E7290
    virtual void PauseAllNonScriptedEntities(bool) = 0;                                                  // 379 0x00890AB0
    virtual void PauseAllEntities(bool) = 0;                                                             // 380 0x00890B00
    virtual void SetAllowScreenFadingOnNextRegionChange(bool) = 0;                                       // 381 0x0088E910
    virtual void SetAllowScreenFadingIfAlreadyFaded(bool) = 0;                                           // 382 0x0088E920
    virtual void SetToKeepHeroAbilitiesDuringCutscenes(bool) const = 0;                                  // 383 0x00890B50
    virtual void SetToDisplayTutorialsDuringCutscenes(bool) const = 0;                                   // 384 0x0088E930
    virtual void SetCutsceneMode(bool, bool) const = 0;                                                  // 385 0x0089D600
    virtual bool IsInCutscene() const = 0;                                                               // 386 0x00892270
    virtual void SetCutsceneSkippable(bool) const = 0;                                                   // 387 0x00892280
    virtual void SetCutsceneSkippableWhilePaused(bool) const = 0;                                        // 388 0x008922A0
    virtual void SetCutsceneActionMode(bool, const CCharString&) const = 0;                              // 389 0x008922C0
    virtual void PreloadNewScene(float) const = 0;                                                       // 390 0x00890B70
    virtual void StartProgressDisplay() const = 0;                                                       // 391 0x0088E950
    virtual void StopProgressDisplay() const = 0;                                                        // 392 0x0088E970
    virtual void SetScreenMessagesEnabled(bool) const = 0;                                               // 393 0x008922D0
    virtual bool IsHeroControlledByPlayer() const = 0;                                                   // 394 0x0088E980
    virtual bool IsInMovieSequence() const = 0;                                                          // 395 0x0088E9E0
    virtual void CancelAbilitiesForCutscene() const = 0;                                                 // 396 0x0089D690
    virtual void ResumeAbilitiesForCutscene() const = 0;                                                 // 397 0x0089D6F0
    virtual void CancelUsingAbility(EHeroAbility) const = 0;                                             // 398 0x0089D750
    virtual void SetAbilityAvailability(EHeroAbility, bool) const = 0;                                   // 399 0x0089D7C0
    virtual void SetEnvironmentalEffectsAlwaysUpdate(bool) const = 0;                                    // 400 0x00890B90
    virtual void SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool) const = 0;                   // 401 0x00894EC0
    virtual void RemoveDeadCreature(const CCharString&) const = 0;                                       // 402 0x00894490
    virtual void CameraSetCameraPreloadFlag(bool) const = 0;                                             // 403 0x0088EC90
    virtual void CameraCircleAroundThing(const CScriptThing&, const C3DVector&, float) const = 0;        // 404 0x00892B50
    virtual void CameraCircleAroundPos(const C3DVector&, const C3DVector&, float) const = 0;             // 405 0x00892C20
    virtual void CameraMoveToPosAndLookAtPos(const C3DVector&, const C3DVector&, float) const = 0;       // 406 0x00892410
    virtual void CameraMoveToPosAndLookAtThing(const C3DVector&, const CScriptThing&, float) const = 0;  // 407 0x008924B0
    virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const C3DVector&, float, float) const = 0; // 409 0x00892530
    virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const CScriptThing&, float, float) const = 0; // 408 0x008926D0
    virtual void CameraMoveBetweenLookFromAndLookTo(const C3DVector&, const C3DVector&, const C3DVector&, const C3DVector&, float) const = 0; // 410 0x00892610
    virtual void CameraUseCameraPoint(const CCharString&, const CScriptThing&, float, long, long) const = 0; // 414 0x008922F0
    virtual void CameraUseCameraPoint(const CScriptThing&, const CScriptThing&, float, long, long) const = 0; // 413 0x00891070
    virtual void CameraUseCameraPoint(const CCharString&, const C3DVector*, const CRightHandedSet*, float, long, long) const = 0; // 412 0x00892380
    virtual void CameraUseCameraPoint(const CScriptThing&, const C3DVector*, const CRightHandedSet*, float, long, long) const = 0; // 411 0x0089E190
    virtual void CameraDoConversation(const CScriptThing&, const CScriptThing&, ECameraOp, bool) const = 0; // 415 0x0088EDC0
    virtual void CameraDefault() = 0;                                                                    // 416 0x00890FF0
    virtual void CameraResetToViewBehindHero(float) = 0;                                                 // 417 0x00890FC0
    virtual bool IsCameraInScriptedMode() const = 0;                                                     // 418 0x0088ECC0
    virtual void CameraUseScreenEffect(float, float, float) const = 0;                                   // 419 0x0088EDA0
    virtual void CameraCancelScreenEffect() const = 0;                                                   // 420 0x0088EDB0
    virtual bool IsCameraPosOnScreen(const C3DVector&) const = 0;                                        // 421 0x006E7300
    virtual float GetGameAngleXY() const = 0;                                                            // 422 0x006E7340
    virtual void CameraEarthquakeIntensityAtPos(const C3DVector&, float, float) const = 0;               // 423 0x0088ED20
    virtual void CameraShake(float, float) const = 0;                                                    // 424 0x0088ECE0
    virtual void OpenDoor(const CScriptThing&) const = 0;                                                // 425 0x00895DB0
    virtual void CloseDoor(const CScriptThing&) const = 0;                                               // 426 0x00895DE0
    virtual void OpenHouseDoors(const CScriptThing&) const = 0;                                          // 427 0x0088DFD0
    virtual void CloseHouseDoors(const CScriptThing&) const = 0;                                         // 428 0x0088E000
    virtual void JamDoor(const CScriptThing&) const = 0;                                                 // 429 0x00895E10
    virtual void SetDoorTriggerType(const CScriptThing&, EDoorTriggerType) const = 0;                    // 430 0x00895E70
    virtual void OverrideAutomaticHouseLocking(const CScriptThing&, bool) const = 0;                     // 431 0x0088E030
    virtual void SetHouseOwnedByPlayer(const CScriptThing&, bool, bool) const = 0;                       // 432 0x00895ED0
    virtual void SetBuyableHouseAsScripted(const CScriptThing&, bool) const = 0;                         // 433 0x00896000
    virtual bool IsChestOpen(const CScriptThing&) const = 0;                                             // 434 0x00896330
    virtual bool OpenChest(const CScriptThing&, bool) const = 0;                                         // 435 0x00896390
    virtual void CloseChest(const CScriptThing&) const = 0;                                              // 436 0x00896660
    virtual long GetNumberOfKeysNeededToUnlockChest(const CScriptThing&, CCharString&) const = 0;        // 437 0x008966C0
    virtual void DisplayLockedChestMessage(const CScriptThing&) const = 0;                               // 438 0x00896730
    virtual void SetTrophyAsMountable(const CScriptThing&, bool) const = 0;                              // 439 0x00896790
    virtual void SetVillageLimbo(const CScriptThing&, bool) const = 0;                                   // 440 0x00896060
    virtual void SetCreatureNotReload(const CScriptThing&) const = 0;                                    // 441 0x0088FD90
    virtual bool IsSleepingTime(const CScriptThing&) const = 0;                                          // 442 0x00896210
    virtual void EnableGuards(const CScriptThing&, bool) const = 0;                                      // 443 0x00896270
    virtual void EnableVillagerDefTypes(const CScriptThing&, bool, const CCharString&) const = 0;        // 444 0x008962D0
    virtual CScriptThing TryToRespawnDefNamed(const CScriptThing&, const CCharString&, const C3DVector&) const = 0; // 445 0x00896120
    virtual void ClearHeroEnemyOfGuards(const CScriptThing&) const = 0;                                  // 446 0x008961B0
    virtual void SetThingAsUsable(const CScriptThing&, bool) const = 0;                                  // 447 0x008967F0
    virtual void SetThingHomeBuilding(const CScriptThing&, const CScriptThing&) const = 0;               // 448 0x00892E30
    virtual void SetVillageAttitude(const CScriptThing&, EScriptVillageAttitude) const = 0;              // 449 0x0089FE20
    virtual void AddCrimeCommitted(const CScriptThing&, ECrime, bool, const CScriptThing*, const CScriptThing*, EOpinionPostDeedType) const = 0; // 450 0x0089FEB0
    virtual void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) const = 0;           // 451 0x008A0260
    virtual void ClearThingBestEnemyTarget(const CScriptThing&) const = 0;                               // 452 0x008A03F0
    virtual void EntitySetInLimbo(const CScriptThing&, bool, bool) const = 0;                            // 453 0x0088FD00
    virtual bool IsEntityInLimbo(const CScriptThing&) const = 0;                                         // 454 0x0088FD60
    virtual bool EntityGetShotStrikePos(const CScriptThing&, C3DVector&) const = 0;                      // 455 0x0089DF70
    virtual void EntitySetNegateAllHits(const CScriptThing&, bool) const = 0;                            // 456 0x00890E80
    virtual void EntitySetEvadeAllHits(const CScriptThing&, bool) const = 0;                             // 457 0x00890EC0
    virtual void EntitySetAbleToBeEngagedInCombat(const CScriptThing&, bool) const = 0;                  // 458 0x00890F00
    virtual void EntitySetAlwaysBlockAttacksFromThing(const CScriptThing&, const CScriptThing&, bool) const = 0; // 459 0x0089DFF0
    virtual void EntitySetAttackThingImmediately(const CScriptThing&, const CScriptThing&, bool, bool) const = 0; // 460 0x008A8EB0
    virtual void EntitySetCombatType(const CScriptThing&, const CCharString&) const = 0;                 // 461 0x0089E080
    virtual void EntityResetCombatTypeToDefault(const CScriptThing&) const = 0;                          // 462 0x00890F40
    virtual void EntitySetMaxNumberOfAttackers(const CScriptThing&, long) const = 0;                     // 463 0x0089E130
    virtual void EntityClearMaxNumberOfAttackers(const CScriptThing&) const = 0;                         // 464 0x0088EC70
    virtual void EntityAttachToScript(const CScriptThing&, const CCharString&) const = 0;                // 465 0x00890690
    virtual void EntitySetCombatAbility(const CScriptThing&, float) const = 0;                           // 466 0x0089AFF0
    virtual void EntitySetRangedTarget(const CScriptThing&, const CScriptThing&) const = 0;              // 467 0x008A04C0
    virtual void EntityClearRangedTarget(const CScriptThing&) const = 0;                                 // 468 0x008A0540
    virtual void EntitySetTargetable(const CScriptThing&, bool) const = 0;                               // 469 0x008A05A0
    virtual void EntitySetTargetingType(const CScriptThing&, ETargetingType) const = 0;                  // 470 0x008A0600
    virtual void EntitySetTargetingValidTargetWithoutLOS(const CScriptThing&, bool) const = 0;           // 471 0x008A0660
    virtual void EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool) const = 0;        // 472 0x0088E540
    virtual void EntityTeleportToPosition(const CScriptThing&, const C3DVector&, float, bool, bool) const = 0; // 473 0x0089B780
    virtual void EntitySetFacingAngle(const CScriptThing&, float, bool) const = 0;                       // 474 0x0089BDF0
    virtual void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) const = 0; // 475 0x0088E620
    virtual void EntitySetPerceptionVariables(const CScriptThing&, float, float, float) const = 0;       // 476 0x0089BF80
    virtual void SetThingPersistent(const CScriptThing&, bool) const = 0;                                // 477 0x0088E5A0
    virtual void EntitySetThingAsWantingMoney(const CScriptThing&, long) const = 0;                      // 478 0x0089BB30
    virtual void EntitySetAppearanceMorphSeed(const CScriptThing&, long) const = 0;                      // 479 0x008A0200
    virtual void SetEntityAsRegionFollowing(const CScriptThing&, const CScriptThing&, bool) const = 0;   // 480 0x008A0B10
    virtual void SetEntityAsFollowingHeroThroughTeleporters(const CScriptThing&, bool) const = 0;        // 481 0x0088F670
    virtual void EntitySetAppearanceSeed(const CScriptThing&, unsigned long) const = 0;                  // 482 0x008A0BC0
    virtual void EntityGetAppearanceSeed(const CScriptThing&, unsigned long&) const = 0;                 // 483 0x008A0C10
    virtual void EntitySetAsForSale(const CScriptThing&, bool) const = 0;                                // 484 0x008A00E0
    virtual void EntitySetStockItemPrice(const CScriptThing&, long) const = 0;                           // 485 0x008A0140
    virtual long EntityGetStockItemPrice(const CScriptThing&) const = 0;                                 // 486 0x008A01A0
    virtual void EntityPlayObjectAnimation(const CScriptThing&, const CCharString&, bool) const = 0;     // 487 0x0089BBC0
    virtual void EntitySetMaxRunningSpeed(const CScriptThing&, float) const = 0;                         // 488 0x0088E750
    virtual void EntitySetMaxWalkingSpeed(const CScriptThing&, float) const = 0;                         // 489 0x0088E7B0
    virtual void EntityResetMaxRunningSpeed(const CScriptThing&) const = 0;                              // 490 0x0088E810
    virtual void EntityResetMaxWalkingSpeed(const CScriptThing&) const = 0;                              // 491 0x0088E870
    virtual void EntityAttachToVillage(const CScriptThing&, const CScriptThing&) const = 0;              // 492 0x0089D3A0
    virtual void EntitySetAsSittingOnFloor(const CScriptThing&, bool) const = 0;                         // 493 0x008A8C60
    virtual void EntitySetAsHavingBoundHands(CScriptThing&, bool) const = 0;                             // 494 0x0089D420
    virtual void EntitySetAsRemoveAllMovementBlockingModes(CScriptThing&) const = 0;                     // 495 0x0088E8D0
    virtual void EntitySetAsScared(const CScriptThing&, bool) const = 0;                                 // 496 0x0089D4C0
    virtual void EntitySetAsDrunk(CScriptThing&, bool) const = 0;                                        // 497 0x0089D560
    virtual void EntityForceToLookAtThing(const CScriptThing&, const CScriptThing&) const = 0;           // 498 0x0089D830
    virtual void EntityForceToLookAtCamera(const CScriptThing&, bool) const = 0;                         // 499 0x0089D8B0
    virtual void EntityForceToLookAtNothing(const CScriptThing&) const = 0;                              // 500 0x0089D910
    virtual void EntityResetForceToLookAt(const CScriptThing&) const = 0;                                // 501 0x0089D970
    virtual void EntitySetShotAccuracyPercentage(const CScriptThing&, long) const = 0;                   // 502 0x0088E5F0
    virtual CScriptThing EntityGetStandingOnThing(const CScriptThing&) const = 0;                        // 503 0x00892220
    virtual CScriptThing EntityGetStandingInsideBuilding(const CScriptThing&) const = 0;                 // 504 0x0089BC50
    virtual void EntityDropGenericBox(const CScriptThing&) const = 0;                                    // 505 0x0089D9D0
    virtual void EntitySheatheWeapons(const CScriptThing&, bool) const = 0;                              // 506 0x00890BA0
    virtual void EntityUnsheatheWeapons(const CScriptThing&, bool) const = 0;                            // 507 0x00890C10
    virtual void EntityUnsheatheMeleeWeapon(const CScriptThing&, bool) const = 0;                        // 508 0x00890C80
    virtual void EntityUnsheatheRangedWeapon(const CScriptThing&, bool) const = 0;                       // 509 0x00890D80
    virtual void EntitySetAlpha(const CScriptThing&, float, bool) const = 0;                             // 510 0x0089DA70
    virtual void EntitySetAsDrawable(const CScriptThing&, bool) const = 0;                               // 511 0x0088E9F0
    virtual void EntitySetCutsceneBehaviour(const CScriptThing&, ECutsceneBehaviour) const = 0;          // 512 0x0088EA20
    virtual ESex EntityGetSex(const CScriptThing&) const = 0;                                            // 513 0x0088EA50
    virtual void EntitySetAsAbleToWalkThroughSolidObjects(const CScriptThing&, bool) const = 0;          // 514 0x0089DBB0
    virtual void EntitySetAsRespondToHit(const CScriptThing&, bool) const = 0;                           // 515 0x0088EA90
    virtual void EntitySetAsDamageable(const CScriptThing&, bool) const = 0;                             // 516 0x0088EAD0
    virtual void EntitySetAsKillable(const CScriptThing&, bool, bool) const = 0;                         // 517 0x0088EB30
    virtual void EntitySetAsLocked(const CScriptThing&, bool) const = 0;                                 // 518 0x0089DC10
    virtual void EntityDecapitate(const CScriptThing&) const = 0;                                        // 519 0x008A8E40
    virtual void EntityGiveGold(const CScriptThing&, long) const = 0;                                    // 520 0x0089DC70
    virtual void EntitySetAllowBossPhaseChanges(const CScriptThing&, bool) const = 0;                    // 521 0x0089DCD0
    virtual long EntityGetBossPhase(const CScriptThing&) const = 0;                                      // 522 0x0089DD30
    virtual void EntitySetBossPhase(const CScriptThing&, long) const = 0;                                // 523 0x0089DD90
    virtual void EntityResetCreatureMode(const CScriptThing&) const = 0;                                 // 524 0x0089DDF0
    virtual bool EntitySetAsReceivingEvents(const CScriptThing&, bool) const = 0;                        // 525 0x00895010
    virtual void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool) const = 0;          // 526 0x0088EBC0
    virtual void EntitySetAsToAddToStatChangesWhenHit(const CScriptThing&, bool) const = 0;              // 527 0x0088EBF0
    virtual void EntityLeaveCombatStance(const CScriptThing&) const = 0;                                 // 528 0x0089DE50
    virtual void EntitySetAsUseMovementInActions(const CScriptThing&, bool) const = 0;                   // 529 0x0088EC20
    virtual void EntitySetAsDisplayingEmoteIcon(const CScriptThing&, bool) const = 0;                    // 530 0x0089DEB0
    virtual void EntitySetAsCollidableToThings(const CScriptThing&, bool) const = 0;                     // 531 0x00891120
    virtual void EntityEnableGravity(const CScriptThing&, bool) const = 0;                               // 532 0x0088F4D0
    virtual void EntitySetLightAsOn(const CScriptThing&, bool) const = 0;                                // 533 0x008A06C0
    virtual void EntityFadeOut(const CScriptThing&, float) const = 0;                                    // 534 0x008A0720
    virtual void EntityFadeIn(const CScriptThing&, float) const = 0;                                     // 535 0x008A07C0
    virtual void EntityBeginLoadingAnimation(const CScriptThing&, const CCharString&) const = 0;         // 536 0x0088F500
    virtual void EntityBeginLoadingBasicAnimations(const CScriptThing&) const = 0;                       // 537 0x0088F530
    virtual bool EntityCastForcePush(const CScriptThing&, bool) const = 0;                               // 538 0x008A9320
    virtual void EntityCastLightningAtTarget(const CScriptThing&, const CScriptThing&) const = 0;        // 539 0x0088F560
    virtual void BeginLoadingMesh(const CCharString&) const = 0;                                         // 540 0x00894AA0
    virtual bool EntityWillTeleportToArea(const CScriptThing&, C3DVector, float, float) const = 0;       // 541 0x008A93A0
    virtual void EntityStartScreamerSuperAttackThing(const CScriptThing&, const CScriptThing&) const = 0; // 542 0x008A08A0
    virtual void EntityEndScreamerSuperAttackThing(const CScriptThing&) const = 0;                       // 543 0x008A0950
    virtual void SetHeroGuideToShowQuestCardsWhenSpokenTo(bool) = 0;                                     // 544 0x0088EC60
    virtual void SetLightColour(const CScriptThing&, const CRGBColour&) const = 0;                       // 545 0x0089DF10
    virtual void CreatureGeneratorSetFamily(const CScriptThing&, const CCharString&) const = 0;          // 546 0x008A9020
    virtual void CreatureGeneratorTrigger(const CScriptThing&) const = 0;                                // 547 0x0089E8E0
    virtual void CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(const CScriptThing&, bool) const = 0; // 548 0x0089E950
    virtual bool CreatureGeneratorIsDepleted(const CScriptThing&) const = 0;                             // 549 0x0089E9C0
    virtual bool CreatureGeneratorIsDestroyed(const CScriptThing&) const = 0;                            // 550 0x0089EA30
    virtual void CreatureGeneratorSetGeneratedCreatureScriptName(const CScriptThing&, const CCharString&) const = 0; // 551 0x0089EAA0
    virtual void CreatureGeneratorSetNumTriggers(const CScriptThing&, long) const = 0;                   // 552 0x0089EB10
    virtual long CreatureGeneratorGetNumGeneratedCreatures(const CScriptThing&) const = 0;               // 553 0x0089EB80
    virtual bool CreatureGeneratorAreAllCreaturesAlive(const CScriptThing&) const = 0;                   // 554 0x0089EBF0
    virtual void CreatureGeneratorAddTriggerer(const CScriptThing&, const CScriptThing&) const = 0;      // 555 0x0089EC60
    virtual void CreatureGeneratorRemoveTriggerer(const CScriptThing&, const CScriptThing&) const = 0;   // 556 0x0089ECC0
    virtual void SetCreatureGeneratorEnabled(const CScriptThing&, bool) const = 0;                       // 557 0x0089ED20
    virtual void SetCreatureGeneratorsEnabled(const CCharString&, bool) const = 0;                       // 558 0x00892CF0
    virtual void SetCreatureGeneratorsEnabledDuringScript(const CCharString&, bool) const = 0;           // 559 0x00892D70
    virtual void SetCreatureGeneratorsCreatureGroupAsEnabled(ECreatureGroup, bool) const = 0;            // 560 0x0088F400
    virtual bool IsCreatureGenerationEnabledForRegion(const CCharString&) const = 0;                     // 561 0x00892D80
    virtual bool IsCreatureFlying(const CScriptThing&) const = 0;                                        // 562 0x0089ED80
    virtual void SetTeleporterAsActive(const CScriptThing&, bool) const = 0;                             // 563 0x0089EE00
    virtual bool IsTeleporterActive(const CScriptThing&) const = 0;                                      // 564 0x0089EE60
    virtual void SetTeleportingAsActive(bool) const = 0;                                                 // 565 0x0088F410
    virtual bool IsTeleportingActive() const = 0;                                                        // 566 0x0088F420
    virtual void SetRegionExitAsActive(const CScriptThing&, bool) const = 0;                             // 567 0x0089EEC0
    virtual void SetRegionEntranceAsActive(const CScriptThing&, bool) const = 0;                         // 568 0x0089EF00
    virtual void SetRegionTextDisplayAsActive(bool) const = 0;                                           // 569 0x0088F430
    virtual void SetHeroSleepingAsEnabled(bool) const = 0;                                               // 570 0x0088F440
    virtual bool IsHeroSleepingEnabled() const = 0;                                                      // 571 0x0088F450
    virtual void SetExperienceSpendingAsEnabled(bool) const = 0;                                         // 572 0x0088F460
    virtual void SetMoralityChangingAsEnabled(bool) const = 0;                                           // 573 0x0089EF30
    virtual void SetSummonerDeathExplosionAffectsHero(bool) const = 0;                                   // 574 0x0088F470
    virtual CScriptThing GetNearestEnabledDiggingSpot(const CScriptThing&) const = 0;                    // 575 0x0089EF90
    virtual bool IsDiggingSpotEnabled(const CScriptThing&) const = 0;                                    // 576 0x0089F1E0
    virtual bool IsDiggingSpotHidden(const CScriptThing&) const = 0;                                     // 577 0x0089F240
    virtual void SetDiggingSpotAsHidden(const CScriptThing&, bool) const = 0;                            // 578 0x0089F2A0
    virtual bool CheckForCameraMessage(const CCharString&) const = 0;                                    // 579 0x006E7920
    virtual void WaitForCameraMessage(const CCharString&) const = 0;                                     // 580 0x006E7820
    virtual void SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const = 0;           // 581 0x008A9610
    virtual void SetFireToThing(const CScriptThing&) const = 0;                                          // 582 0x0089BCD0
    virtual void ExtinguishFiresOnThing(const CScriptThing&) const = 0;                                  // 583 0x0089BD30
    virtual bool IsThingOnFire(const CScriptThing&) const = 0;                                           // 584 0x0089BD90
    virtual void AddItemToContainer(const CScriptThing&, const CCharString&) const = 0;                  // 585 0x0089E780
    virtual void RemoveItemFromContainer(const CScriptThing&, const CCharString&) const = 0;             // 586 0x0089E810
    virtual void EntitySetDeathContainerAsEnabled(const CScriptThing&, bool) const = 0;                  // 587 0x0089E880
    virtual void GetItemDefNamesFromContainer(const CScriptThing&, std::vector<CCharString,std::allocator<CCharString> >&) const = 0; // 588 0x008A8F20
    virtual void SetCreatureBrain(const CScriptThing&, const CCharString&) const = 0;                    // 589 0x00894B90
    virtual void EntitySetStategroupEnabled(const CScriptThing&, const CCharString&, bool) const = 0;    // 590 0x0088F6B0
    virtual void EntitySetAllStategroupsEnabled(const CScriptThing&, bool) const = 0;                    // 591 0x0088F6E0
    virtual void EntitySetCombatEnabled(const CScriptThing&, bool) const = 0;                            // 592 0x0088F710
    virtual void EntitySetSleepEnabled(const CScriptThing&, bool) const = 0;                             // 593 0x0088F7D0
    virtual void EntitySetOpinionReactionsEnabled(const CScriptThing&, bool) const = 0;                  // 594 0x0088F810
    virtual void EntitySetDeedReactionsEnabled(const CScriptThing&, bool) const = 0;                     // 595 0x0088F850
    virtual void DebugGetAllTextEntriesForTargetedThing(std::set<unsigned long,std::less<unsigned long>,std::allocator<unsigned long> >&) const = 0; // 596 0x008A0CB0
    virtual void EntitySetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const = 0;       // 597 0x0089C000
    virtual void EntityUnsetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const = 0;     // 598 0x0089C080
    virtual void EntitySetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const = 0;        // 599 0x008A7EC0
    virtual void EntityUnsetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const = 0;      // 600 0x008A7EE0
    virtual void EntitySetInFaction(const CScriptThing&, const CCharString&) const = 0;                  // 601 0x0089C190
    virtual void SetFactionAsAlliedToFaction(const CCharString&, const CCharString&) const = 0;          // 602 0x00890870
    virtual void SetFactionAsNeutralToFaction(const CCharString&, const CCharString&) const = 0;         // 603 0x008908C0
    virtual void SetFactionAsEnemyToFaction(const CCharString&, const CCharString&) const = 0;           // 604 0x00890910
    virtual bool AreEntitiesEnemies(const CScriptThing&, const CScriptThing&) const = 0;                 // 605 0x0088E6F0
    virtual EOpinionAttitudeType GetNextInOpinionAttitudeGraph(EOpinionAttitudeType) const = 0;          // 606 0x00890960
    virtual void GetOpinionAttitudeAsString(EOpinionAttitudeType, CCharString&) const = 0;               // 607 0x00890970
    virtual EOpinionAttitudeType EntityGetOpinionAttitudeToPlayer(const CScriptThing&) const = 0;        // 608 0x00890990
    virtual void EntityGetOpinionAttitudeToPlayerAsString(const CScriptThing&, CCharString&) const = 0;  // 609 0x008909C0
    virtual float EntityGetOpinionOfPlayer(const CScriptThing&, EOpinion) const = 0;                     // 610 0x008909F0
    virtual void EntitySetOpinionReactionMask(const CScriptThing&, long) const = 0;                      // 612 0x0089C1F0
    virtual void EntitySetOpinionReactionMask(const CScriptThing&, const CCharString&) const = 0;        // 611 0x0089C240
    virtual void EntitySetOpinionDeedMask(const CScriptThing&, long) const = 0;                          // 614 0x0089C290
    virtual void EntitySetOpinionDeedMask(const CScriptThing&, const CCharString&) const = 0;            // 613 0x0089C2E0
    virtual void EntitySetOpinionDeedTypeEnabled(const CScriptThing&, EOpinionDeedType, bool) const = 0; // 615 0x0089C330
    virtual void EntitySetOpinionAttitudeEnabled(const CScriptThing&, EOpinionAttitudeType, bool) const = 0; // 616 0x0089C380
    virtual void EntitySetOpinionReactionEnabled(const CScriptThing&, EOpinionReactionType, bool) const = 0; // 617 0x0089C3D0
    virtual void EntitySetPersonalityOverride(const CScriptThing&, const CCharString&) const = 0;        // 619 0x0089C420
    virtual void EntitySetPersonalityOverride(const CScriptThing&, long) const = 0;                      // 618 0x0089C470
    virtual void EntityClearPersonalityOverride(const CScriptThing&) const = 0;                          // 620 0x0089C4C0
    virtual void EntitySetAsOpinionSource(const CScriptThing&, const CCharString&) const = 0;            // 622 0x00890A20
    virtual void EntitySetAsOpinionSource(const CScriptThing&, long) const = 0;                          // 621 0x00890A50
    virtual void EntityUnsetAsOpinionSource(const CScriptThing&, bool) const = 0;                        // 623 0x00890A80
    virtual void OpinionSourceSetAsExclusive(const CScriptThing&, bool) const = 0;                       // 624 0x00894E60
    virtual void OpinionSourceSetAsAttentionGrabbing(const CScriptThing&, bool) const = 0;               // 625 0x0089C510
    virtual void EntityPostOpinionDeedToAll(const CScriptThing&, EOpinionDeedType) const = 0;            // 626 0x0089C560
    virtual void EntityPostOpinionDeedToRecipient(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const = 0; // 627 0x0089C670
    virtual void EntityPostOpinionDeedToRecipientVillage(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const = 0; // 628 0x0089C710
    virtual long EntityPostOpinionDeedKeepSearchingForWitnesses(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const = 0; // 629 0x0089C7B0
    virtual void RemoveOpinionDeedStillSearchingForWitnesses(const CScriptThing&, long) const = 0;       // 630 0x0089C850
    virtual bool IsDeedWitnessed(long) const = 0;                                                        // 631 0x0089C8A0
    virtual bool CanThingBe_Seen_ByOtherThing(const CScriptThing&, const CScriptThing&) const = 0;       // 632 0x0089CD10
    virtual bool CanThingBe_NearlySeen_ByOtherThing(const CScriptThing&, const CScriptThing&) const = 0; // 633 0x0089CD30
    virtual bool CanThingBe_Smelled_ByOtherThing(const CScriptThing&, const CScriptThing&) const = 0;    // 634 0x0089CD50
    virtual bool CanThingBe_Heard_ByOtherThing(const CScriptThing&, const CScriptThing&) const = 0;      // 635 0x0089CD70
    virtual bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing&, const CScriptThing&) const = 0;   // 636 0x0089CE70
    virtual void EntitySetAsAwareOfThing(const CScriptThing&, const CScriptThing&) const = 0;            // 637 0x0089D0E0
    virtual void EntitySetSoundRadius(const CScriptThing&, float) const = 0;                             // 638 0x0089D160
    virtual void EntitySetSmellRadius(const CScriptThing&, float) const = 0;                             // 639 0x0089D1C0
    virtual void EntitySetSightRadius(const CScriptThing&, float) const = 0;                             // 640 0x0089D220
    virtual void EntitySetExtendedSightRadius(const CScriptThing&, float) const = 0;                     // 641 0x0089D280
    virtual void EntitySetGiveUpChaseRadius(const CScriptThing&, float) const = 0;                       // 642 0x0089D2E0
    virtual float EntityGetHearingRadius(const CScriptThing&) const = 0;                                 // 643 0x0089D340
    virtual bool ManuallyTriggerTrap(const CScriptThing&) const = 0;                                     // 644 0x00895090
    virtual bool ManuallyResetTrap(const CScriptThing&) const = 0;                                       // 645 0x008950E0
    virtual void SetTimeOfDay(float) const = 0;                                                          // 646 0x0088FDC0
    virtual long GetTimeOfDay() const = 0;                                                               // 647 0x0088FE30
    virtual void SetTimeAsStopped(bool, long&) const = 0;                                                // 648 0x0088FE80
    virtual void FastForwardTimeTo(float, float) const = 0;                                              // 649 0x0088FE90
    virtual bool IsTimeOfDayBetween(long, long) const = 0;                                               // 650 0x0088FE40
    virtual long GetDayOfWeek() const = 0;                                                               // 651 0x0088FE50
    virtual long GetDayCount() const = 0;                                                                // 652 0x0088FE70
    virtual long GetWorldFrame() const = 0;                                                              // 653 0x0088E070
    virtual long GetConstantFPS() const = 0;                                                             // 654 0x0088E080
    virtual CCharString GetActiveQuestName() const = 0;                                                  // 655 0x00891880
    virtual void TransitionToTheme(const CCharString&, float) const = 0;                                 // 656 0x0088FEE0
    virtual void ResetToDefaultTheme(float) const = 0;                                                   // 657 0x0088FF10
    virtual void TransitionToThemeAllInternals(const CCharString&, float) const = 0;                     // 658 0x0088FF30
    virtual void ResetToDefaultThemeAllInternals(float) const = 0;                                       // 659 0x0088FF80
    virtual void TransitionToThemeExternals(const CCharString&, float) const = 0;                        // 660 0x0088FFB0
    virtual void ResetToDefaultThemeExternals(float) const = 0;                                          // 661 0x0088FFF0
    virtual void SetEnvironmentThemeWeightAllChannels(const CCharString&, float) const = 0;              // 662 0x00890010
    virtual void SetEnvironmentThemeWeightAllInternals(const CCharString&, float) const = 0;             // 663 0x00890040
    virtual void SetEnvironmentThemeWeightExternals(const CCharString&, float) const = 0;                // 664 0x00890090
    virtual void SetSoundThemesAsEnabledForRegion(const CCharString&, bool) const = 0;                   // 665 0x0088E0B0
    virtual void SetAllSoundsAsMuted(bool) const = 0;                                                    // 666 0x0088E090
    virtual void* RadialBlurFadeTo(float, C3DVector, float, C3DVector, float, float, float) const = 0;   // 668 0x008900D0
    virtual void* RadialBlurFadeTo(float, float, float, float, float, float, float) const = 0;           // 667 0x00890110
    virtual void RadialBlurFadeOut(float, void*) const = 0;                                              // 669 0x00890150
    virtual bool IsRadialBlurFadeActive() const = 0;                                                     // 670 0x00890170
    virtual void CancelRadialBlurFade() const = 0;                                                       // 671 0x00890180
    virtual void RadialBlurSetCenterWorldPos(void*, const C3DVector&) const = 0;                         // 672 0x008901A0
    virtual void* DisplacementMonochromeEffectColourFadeTo(float, const CRGBFloatColour&) const = 0;     // 673 0x008901C0
    virtual void DisplacementMonochromeEffectColourFadeOut(float, void*) const = 0;                      // 674 0x008901E0
    virtual void* ScreenFilterFadeTo(float, float, float, float, float, const CRGBFloatColour&, const std::vector<CScreenFilterSThingByPass,std::allocator<CScreenFilterSThingByPass> >&) const = 0; // 675 0x008A9880
    virtual void ScreenFilterFadeOut(float, void*) const = 0;                                            // 676 0x00890200
    virtual void SetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&, void*) const = 0;       // 677 0x008A99F0
    virtual void UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&) const = 0;            // 678 0x00897150
    virtual bool IsGiftRomantic(const CCharString&) const = 0;                                           // 679 0x00894BF0
    virtual bool IsGiftFriendly(const CCharString&) const = 0;                                           // 680 0x00894C90
    virtual bool IsGiftOffensive(const CCharString&) const = 0;                                          // 681 0x00894D30
    virtual bool IsThingABed(const CScriptThing&) const = 0;                                             // 682 0x008A0E80
    virtual bool IsThingAChest(const CScriptThing&) const = 0;                                           // 683 0x008A0EB0
    virtual bool IsThingADoor(const CScriptThing&) const = 0;                                            // 684 0x008A0EE0
    virtual bool IsThingSmashable(const CScriptThing&) const = 0;                                        // 685 0x008A0F10
    virtual bool IsThingSearchable(const CScriptThing&) const = 0;                                       // 686 0x008A0F40
    virtual void ApplyScriptBrush(const CCharString&) const = 0;                                         // 687 0x0088F480
    virtual void EnableDecals(bool) const = 0;                                                           // 688 0x0088F4A0
    virtual unsigned long PlayCriteriaSoundOnThing(const CScriptThing&, const CCharString&) const = 0;   // 689 0x008A09B0
    virtual unsigned long PlaySoundOnThing(const CScriptThing&, const CCharString&) const = 0;           // 690 0x008A0A20
    virtual unsigned long PlaySoundAtPos(const C3DVector&, const CCharString&) const = 0;                // 691 0x00891170
    virtual unsigned long Play2DSound(const CCharString&) const = 0;                                     // 692 0x0088F570
    virtual bool IsSoundPlaying(unsigned long) const = 0;                                                // 693 0x0088F610
    virtual void EnableSounds(bool) const = 0;                                                           // 694 0x0088F620
    virtual void StopSound(unsigned long) const = 0;                                                     // 695 0x0088F660
    virtual void OverrideMusic(EMusicSetType, bool, bool) const = 0;                                     // 696 0x00891220
    virtual void StopOverrideMusic(bool) const = 0;                                                      // 697 0x008912A0
    virtual void CacheMusicSet(EMusicSetType) const = 0;                                                 // 698 0x00891310
    virtual void EnableDangerMusic(bool) const = 0;                                                      // 699 0x00891320
    virtual bool IsDangerMusicEnabled() const = 0;                                                       // 700 0x00891330
    virtual void StartCountdownTimer(float) const = 0;                                                   // 701 0x0088F8D0
    virtual float GetCountdownTimer() const = 0;                                                         // 702 0x0088F900
    virtual void AutoSaveCheckPoint() const = 0;                                                         // 703 0x0088F910
    virtual void AutoSaveQuestStart() const = 0;                                                         // 704 0x0088F920
    virtual void AutoSave() const = 0;                                                                   // 705 0x0088F930
    virtual void SetSavingAsEnabled(bool) const = 0;                                                     // 706 0x0088F940
    virtual bool IsSavingEnabled() const = 0;                                                            // 707 0x0088F950
    virtual void SetSaveGameMarkerPos(const C3DVector&) const = 0;                                       // 708 0x0088F960
    virtual void ResetToFrontEnd() const = 0;                                                            // 709 0x0088F970
    virtual void SetGuildSealRecallLocation(const C3DVector&, float) const = 0;                          // 710 0x0088F980
    virtual C3DVector GetGuildSealRecallPos() const = 0;                                                 // 711 0x0088F9B0
    virtual float GetGuildSealRecallAngleXY() const = 0;                                                 // 712 0x0088F9E0
    virtual void SetReadableObjectText(const CScriptThing&, const CWideString&) const = 0;               // 713 0x008A0F70
    virtual void SetReadableObjectTextTag(const CScriptThing&, const CCharString&) const = 0;            // 714 0x008A0FD0
    virtual CWideString GetFormattedString(const CCharString&, const std::vector<CWideString,std::allocator<CWideString> >&) const = 0; // 715 0x00891340
    virtual CWideString GetTextString(const CCharString&) const = 0;                                     // 716 0x008913A0
    virtual void AddRumourCategory(const CCharString&) const = 0;                                        // 717 0x008A9DB0
    virtual void AddNewRumourToCategory(const CCharString&, const CCharString&) const = 0;               // 718 0x008A9DD0
    virtual void RemoveRumourCategory(const CCharString&) const = 0;                                     // 719 0x008A9E00
    virtual void SetCategoryActivity(const CCharString&, bool) const = 0;                                // 720 0x008A96C0
    virtual void AddGossipVillage(const CCharString&, const CCharString&) const = 0;                     // 721 0x008A9E30
    virtual void AddGossipFactionToCategory(const CCharString&, const CCharString&) const = 0;           // 722 0x008A9E60
    virtual void SetIsGossipForPlayer(const CCharString&, bool) const = 0;                               // 724 0x008A96F0
    virtual void SetIsGossipForPlayer(CCharString, bool) = 0;                                            // 723 0x008A97D0
    virtual void UpdateOnlineScore_Archery(float) const = 0;                                             // 725 0x008A1040
    virtual void UpdateOnlineScore_ChickenKick(float) const = 0;                                         // 726 0x008A10C0
    virtual void UpdateOnlineScore_ChapelOrTemple(float) const = 0;                                      // 727 0x008A1140
    virtual void UpdateOnlineScore_FishingCompo(float) const = 0;                                        // 728 0x008A11C0
    virtual void UpdateScore_FishingCompetition(float) const = 0;                                        // 729 0x008A1240
    virtual float GetBestTimePairs() const = 0;                                                          // 730 0x008A1590
    virtual float GetBestTimeSorting() const = 0;                                                        // 731 0x008A15F0
    virtual long GetBestScoreBlackjack() const = 0;                                                      // 732 0x008A1650
    virtual long GetBestScoreCoinGolfOakVale() const = 0;                                                // 733 0x008A16B0
    virtual long GetBestScoreCoinGolfSnowSpire() const = 0;                                              // 734 0x008A1710
    virtual long GetBestScoreShoveHaPenny() const = 0;                                                   // 735 0x008A1770
    virtual float GetBestTimeGuessTheAddition() const = 0;                                               // 736 0x008A17D0
    virtual bool IsHeroInTavernGame() const = 0;                                                         // 737 0x008913C0
    virtual long GetNumHousesOwned() const = 0;                                                          // 738 0x008A12A0
    virtual void StartSneaking() const = 0;                                                              // 739 0x008A1300
    virtual long GetStealDuration(const CScriptThing&) const = 0;                                        // 740 0x008A1360
    virtual void SetUseableByHero(const CScriptThing&, bool) const = 0;                                  // 741 0x008A1830
    virtual void SetOwnedByHero(const CScriptThing&, bool) const = 0;                                    // 742 0x008A18F0
    virtual void SetTavernTableAvailableForUse(const CScriptThing&, bool) const = 0;                     // 743 0x008A1890
    virtual void SetIsThingTurncoatable(const CScriptThing&, bool) const = 0;                            // 744 0x008A1950
    virtual void SetIsThingForcePushable(const CScriptThing&, bool) const = 0;                           // 745 0x0088F9F0
    virtual void SetIsThingLightningable(const CScriptThing&, bool) const = 0;                           // 746 0x0088FA20
    virtual void SetIsThingEpicSpellable(const CScriptThing&, bool) const = 0;                           // 747 0x0088FA50
    virtual bool IsThingTurncoated(const CScriptThing&) const = 0;                                       // 748 0x008A19B0
    virtual void AddCreatureScriptedMode(const CScriptThing&, const CCharString&) const = 0;             // 749 0x008A1A10
    virtual void RemoveCreatureScriptedMode(const CScriptThing&) const = 0;                              // 750 0x008A1AB0
    virtual void ForceShipsVisible() const = 0;                                                          // 751 0x008A7F60
    virtual bool GetSleepingPositionAndOrientationFromBed(const CScriptThing&, const CScriptThing&, C3DVector&, C3DVector&) = 0; // 752 0x008A1B10
    virtual void SetBedAvailability(const CScriptThing&, bool) = 0;                                      // 753 0x008A1BF0
    virtual void RepopulateVillage(const CScriptThing&) const = 0;                                       // 754 0x008A1C50
    virtual void SmashAllWindowsWithinRadiusOfPoint(const C3DVector&, float) = 0;                        // 755 0x008A8010
    virtual CScriptThing SetResidency(const CScriptThing&, bool) = 0;                                    // 756 0x008A1CC0
    virtual void SetThankingPhrase(CScriptThing, unsigned long) = 0;                                     // 757 0x008A1E50
    virtual unsigned long GetThankingPhrase(CScriptThing) = 0;                                           // 758 0x008A1F30
    virtual void ResetThankingPhrase(CScriptThing) = 0;                                                  // 759 0x008A2020
    virtual void SetIgnoringPhrase(CScriptThing, unsigned long) = 0;                                     // 760 0x008A2100
    virtual unsigned long GetIgnoringPhrase(CScriptThing) = 0;                                           // 761 0x008A21E0
    virtual void ResetIgnoringPhrase(CScriptThing) = 0;                                                  // 762 0x008A22D0
    virtual void SetWanderCentrePoint(CScriptThing, C3DVector) = 0;                                      // 763 0x008A23B0
    virtual C3DVector GetWanderCentrePoint(CScriptThing) = 0;                                            // 764 0x008A24C0
    virtual void ResetWanderCentrePoint(CScriptThing) = 0;                                               // 765 0x008A2590
    virtual void SetWanderMinDistance(CScriptThing, float) = 0;                                          // 766 0x008A2690
    virtual float GetWanderMinDistance(CScriptThing) = 0;                                                // 767 0x008A2770
    virtual void ResetWanderMinDistance(CScriptThing) = 0;                                               // 768 0x008A2870
    virtual void SetWanderMaxDistance(CScriptThing, float) = 0;                                          // 769 0x008A2950
    virtual float GetWanderMaxDistance(CScriptThing) = 0;                                                // 770 0x008A2A30
    virtual void ResetWanderMaxDistance(CScriptThing) = 0;                                               // 771 0x008A2B30
    virtual void SetGossipCounter(CScriptThing, long) = 0;                                               // 772 0x008A2C10
    virtual long GetGossipCounter(CScriptThing) = 0;                                                     // 773 0x008A2CF0
    virtual void ResetGossipCounter(CScriptThing) = 0;                                                   // 774 0x008A2DE0
    virtual void SetMaxGossipPhrase(CScriptThing, long) = 0;                                             // 775 0x008A2EC0
    virtual long GetMaxGossipPhrase(CScriptThing) = 0;                                                   // 776 0x008A2FA0
    virtual void ResetMaxGossipPhrase(CScriptThing) = 0;                                                 // 777 0x008A3090
    virtual void SetWarningPhrase(CScriptThing, unsigned long) = 0;                                      // 778 0x008A3170
    virtual unsigned long GetWarningPhrase(CScriptThing) = 0;                                            // 779 0x008A3250
    virtual void ResetWarningPhrase(CScriptThing) = 0;                                                   // 780 0x008A3340
    virtual void SetBeerRequestPhrase(CScriptThing, unsigned long) = 0;                                  // 781 0x008A3420
    virtual unsigned long GetBeerRequestPhrase(CScriptThing) = 0;                                        // 782 0x008A3500
    virtual void ResetBeerRequestPhrase(CScriptThing) = 0;                                               // 783 0x008A35F0
    virtual void SetScriptingStateGroup(CScriptThing, EScriptingStateGroups) = 0;                        // 784 0x008A36D0
    virtual EScriptingStateGroups GetScriptingStateGroup(CScriptThing) = 0;                              // 785 0x008A37B0
    virtual void ResetScriptingStateGroup(CScriptThing) = 0;                                             // 786 0x008A38A0
    virtual void SetMaxHeroReactionDistance(CScriptThing, float) = 0;                                    // 787 0x008A3980
    virtual float GetMaxHeroReactionDistance(CScriptThing) = 0;                                          // 788 0x008A3A60
    virtual void ResetMaxHeroReactionDistance(CScriptThing) = 0;                                         // 789 0x008A3B60
    virtual void SetActionFrequency(CScriptThing, long) = 0;                                             // 790 0x008A3C40
    virtual long GetActionFrequency(CScriptThing) = 0;                                                   // 791 0x008A3D20
    virtual void ResetActionFrequency(CScriptThing) = 0;                                                 // 792 0x008A3E10
    virtual void SetActionFrequencyVariation(CScriptThing, float) = 0;                                   // 793 0x008A3EF0
    virtual float GetActionFrequencyVariation(CScriptThing) = 0;                                         // 794 0x008A3FD0
    virtual void ResetActionFrequencyVariation(CScriptThing) = 0;                                        // 795 0x008A40D0
    virtual void SetAction(CScriptThing, CCharString) = 0;                                               // 796 0x008A41B0
    virtual CCharString GetAction(CScriptThing) = 0;                                                     // 797 0x008A42C0
    virtual void ResetAction(CScriptThing) = 0;                                                          // 798 0x008A4390
    virtual void SetFaceHeroForAction(CScriptThing, bool) = 0;                                           // 799 0x008A4480
    virtual bool GetFaceHeroForAction(CScriptThing) = 0;                                                 // 800 0x008A4560
    virtual void ResetFaceHeroForAction(CScriptThing) = 0;                                               // 801 0x008A4650
    virtual void SetTargetName(CScriptThing, CCharString) = 0;                                           // 802 0x008A4730
    virtual CCharString GetTargetName(CScriptThing) = 0;                                                 // 803 0x008A4840
    virtual void ResetTargetName(CScriptThing) = 0;                                                      // 804 0x008A4910
    virtual void SetFollowDistance(CScriptThing, float) = 0;                                             // 805 0x008A4A00
    virtual float GetFollowDistance(CScriptThing) = 0;                                                   // 806 0x008A4AE0
    virtual void ResetFollowDistance(CScriptThing) = 0;                                                  // 807 0x008A4BE0
    virtual void SetAttackHeroOnSight(CScriptThing, bool) = 0;                                           // 808 0x008A4CC0
    virtual bool GetAttackHeroOnSight(CScriptThing) = 0;                                                 // 809 0x008A4DA0
    virtual void ResetAttackHeroOnSight(CScriptThing) = 0;                                               // 810 0x008A4E90
    virtual void SetTimeToSpendHarassingHero(CScriptThing, long) = 0;                                    // 811 0x008A4F70
    virtual long GetTimeToSpendHarassingHero(CScriptThing) = 0;                                          // 812 0x008A5050
    virtual void ResetTimeToSpendHarassingHero(CScriptThing) = 0;                                        // 813 0x008A5140
    virtual void SetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing, float) = 0;                      // 814 0x008A5220
    virtual float GetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) = 0;                            // 815 0x008A5300
    virtual void ResetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) = 0;                           // 816 0x008A5400
    virtual void SetCombatNearbyBreakOffRange(CScriptThing, float) = 0;                                  // 817 0x008A54E0
    virtual float GetCombatNearbyBreakOffRange(CScriptThing) = 0;                                        // 818 0x008A55C0
    virtual void ResetCombatNearbyBreakOffRange(CScriptThing) = 0;                                       // 819 0x008A56C0
    virtual void SetStealStealableItems(CScriptThing, bool) = 0;                                         // 820 0x008A57B0
    virtual bool GetStealStealableItems(CScriptThing) = 0;                                               // 821 0x008A5890
    virtual void ResetStealStealableItems(CScriptThing) = 0;                                             // 822 0x008A5980
    virtual void SetRecoverStealableItems(CScriptThing, bool) = 0;                                       // 823 0x008A5A70
    virtual bool GetRecoverStealableItems(CScriptThing) = 0;                                             // 824 0x008A5B50
    virtual void ResetRecoverStealableItems(CScriptThing) = 0;                                           // 825 0x008A5C40
    virtual void SetTakeStealableItemToRandomDestination(CScriptThing, bool) = 0;                        // 826 0x008A5D30
    virtual bool GetTakeStealableItemToRandomDestination(CScriptThing) = 0;                              // 827 0x008A5E10
    virtual void ResetTakeStealableItemToRandomDestination(CScriptThing) = 0;                            // 828 0x008A5F00
    virtual void SetKillSelfAndStealableItemAfterReachingDestination(CScriptThing, bool) = 0;            // 829 0x008A5FF0
    virtual bool GetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) = 0;                  // 830 0x008A60D0
    virtual void ResetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) = 0;                // 831 0x008A61C0
    virtual void SetAllowedToFollow(CScriptThing, bool) = 0;                                             // 832 0x008A62B0
    virtual bool GetAllowedToFollow(CScriptThing) = 0;                                                   // 833 0x008A6390
    virtual void ResetAllowedToFollow(CScriptThing) = 0;                                                 // 834 0x008A6480
    virtual void SetTableName(CScriptThing, CCharString) = 0;                                            // 835 0x008A6570
    virtual CCharString GetTableName(CScriptThing) = 0;                                                  // 836 0x008A6680
    virtual void ResetTableName(CScriptThing) = 0;                                                       // 837 0x008A6750
    virtual void SetSeatName(CScriptThing, CCharString) = 0;                                             // 838 0x008A6840
    virtual CCharString GetSeatName(CScriptThing) = 0;                                                   // 839 0x008A6950
    virtual void ResetSeatName(CScriptThing) = 0;                                                        // 840 0x008A6A20
    virtual void SetDisableHeadLooking(CScriptThing, bool) = 0;                                          // 841 0x008A6B10
    virtual bool GetDisableHeadLooking(CScriptThing) = 0;                                                // 842 0x008A6BF0
    virtual void ResetDisableHeadLooking(CScriptThing) = 0;                                              // 843 0x008A6CE0
    virtual void SetIsPushableByHero(CScriptThing, bool) = 0;                                            // 844 0x008A6DD0
    virtual bool GetIsPushableByHero(CScriptThing) = 0;                                                  // 845 0x008A6EB0
    virtual void ResetIsPushableByHero(CScriptThing) = 0;                                                // 846 0x008A6FA0
    virtual void SetLookForFiniteTime(CScriptThing, bool) = 0;                                           // 847 0x008A7090
    virtual bool GetLookForFiniteTime(CScriptThing) = 0;                                                 // 848 0x008A7170
    virtual void ResetLookForFiniteTime(CScriptThing) = 0;                                               // 849 0x008A7260
    virtual void SetAvoidRegionExits(CScriptThing, bool) = 0;                                            // 850 0x008A7350
    virtual bool GetAvoidRegionExits(CScriptThing) = 0;                                                  // 851 0x008A7430
    virtual void ResetAvoidRegionExits(CScriptThing) = 0;                                                // 852 0x008A7520
    virtual void SetTargetingDistanceOffset(CScriptThing, float) = 0;                                    // 853 0x008A7610
    virtual float GetTargetingDistanceOffset(CScriptThing) = 0;                                          // 854 0x008A76F0
    virtual void ResetTargetingDistanceOffset(CScriptThing) = 0;                                         // 855 0x008A77F0
    virtual void SetPlayerUsingMeleeDummies(bool) = 0;                                                   // 856 0x0088FA80
    virtual bool GetPlayerUsingMeleeDummies() = 0;                                                       // 857 0x0088FA90
    virtual void SetPlayerUsingRangedDummies(bool) = 0;                                                  // 858 0x0088FAA0
    virtual bool GetPlayerUsingRangedDummies() = 0;                                                      // 859 0x0088FAB0
    virtual void SetPlayerUsingWillDummies(bool) = 0;                                                    // 860 0x0088FAC0
    virtual bool GetPlayerUsingWillDummies() = 0;                                                        // 861 0x0088FAD0
    virtual void SetCheapHeadLooking(bool) = 0;                                                          // 862 0x0088FAE0
    virtual bool GetCheapHeadLooking() = 0;                                                              // 863 0x0088FAF0
    virtual void SetQuitTavernGame(bool) = 0;                                                            // 864 0x0088FB00
    virtual bool GetQuitTavernGame() = 0;                                                                // 865 0x0088FB10
    virtual void SetPrizeTavernTable(bool) = 0;                                                          // 866 0x0088FB20
    virtual bool GetPrizeTavernTable() = 0;                                                              // 867 0x0088FB30
    virtual void SetBettingActive(bool) = 0;                                                             // 868 0x0088FB40
    virtual bool GetBettingActive() = 0;                                                                 // 869 0x0088FB50
    virtual void SetBettingAccept(bool) = 0;                                                             // 870 0x0088FB60
    virtual bool GetBettingAccept() = 0;                                                                 // 871 0x0088FB70
    virtual void SetBettingAmount(long) = 0;                                                             // 872 0x0088FB80
    virtual long GetBettingAmount() = 0;                                                                 // 873 0x0088FB90
    virtual void SetCountBetMoneyDown(bool) = 0;                                                         // 874 0x0088FBA0
    virtual bool GetCountBetMoneyDown() = 0;                                                             // 875 0x0088FBB0
    virtual void SetSpotTheAdditionBeaten(bool) = 0;                                                     // 876 0x0088FBC0
    virtual bool GetSpotTheAdditionBeaten() = 0;                                                         // 877 0x0088FBD0
    virtual void SetGlobalTargetingDistanceOffset(float) = 0;                                            // 878 0x0088FBE0
    virtual float GetGlobalTargetingDistanceOffset() = 0;                                                // 879 0x0088FBF0
    virtual void SetTradingPriceMult(float) = 0;                                                         // 880 0x0088FC00
    virtual float GetTradingPriceMult() = 0;                                                             // 881 0x0088FC10
    virtual void SetBoastingEnabled(bool) = 0;                                                           // 882 0x0088FC20
    virtual bool GetBoastingEnabled() = 0;                                                               // 883 0x0088FC30
    virtual void SetActiveGossipCategories(CCharString, bool) = 0;                                       // 884 0x008A9720
    virtual bool* GetActiveGossipCategories(CCharString) = 0;                                            // 886 0x008A9760
    virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetActiveGossipCategories() = 0; // 885 0x0088FC40
    virtual int GetActiveGossipCategoriesSize() = 0;                                                     // 887 0x00893040
    virtual void ClearActiveGossipCategories() = 0;                                                      // 888 0x008A78E0
    virtual bool* GetIsGossipForPlayer(CCharString) = 0;                                                 // 890 0x008A9810
    virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetIsGossipForPlayer() = 0; // 889 0x0088FC50
    virtual int GetIsGossipForPlayerSize() = 0;                                                          // 891 0x00893050
    virtual void ClearIsGossipForPlayer() = 0;                                                           // 892 0x008A7930
    virtual void SetGossip(CCharString, CCharString, long) = 0;                                          // 893 0x008A9E90
    virtual CCharString GetGossip(CCharString, long) = 0;                                                // 895 0x008A9F00
    virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossip(CCharString) = 0;     // 894 0x008A9F50
    virtual int GetGossipSize(CCharString) = 0;                                                          // 896 0x008A9FC0
    virtual void ClearGossip(CCharString) = 0;                                                           // 897 0x008AA010
    virtual void RemoveGossip(CCharString) = 0;                                                          // 898 0x008AA050
    virtual void AddGossip(CCharString, CCharString) = 0;                                                // 900 0x008AA090
    virtual void AddGossip(CCharString) = 0;                                                             // 899 0x008AA0D0
    virtual void SetGossipVillages(CCharString, CCharString, long) = 0;                                  // 901 0x008AA0F0
    virtual CCharString GetGossipVillages(CCharString, long) = 0;                                        // 903 0x008AA160
    virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipVillages(CCharString) = 0; // 902 0x008AA1B0
    virtual int GetGossipVillagesSize(CCharString) = 0;                                                  // 904 0x008AA220
    virtual void ClearGossipVillages(CCharString) = 0;                                                   // 905 0x008AA270
    virtual void RemoveGossipVillages(CCharString) = 0;                                                  // 906 0x008AA2B0
    virtual void AddGossipVillages(CCharString, CCharString) = 0;                                        // 908 0x008AA2F0
    virtual void AddGossipVillages(CCharString) = 0;                                                     // 907 0x008AA330
    virtual void SetGossipFactions(CCharString, CCharString, long) = 0;                                  // 909 0x008AA350
    virtual CCharString GetGossipFactions(CCharString, long) = 0;                                        // 911 0x008AA3C0
    virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipFactions(CCharString) = 0; // 910 0x008AA410
    virtual int GetGossipFactionsSize(CCharString) = 0;                                                  // 912 0x008AA480
    virtual void ClearGossipFactions(CCharString) = 0;                                                   // 913 0x008AA4D0
    virtual void RemoveGossipFactions(CCharString) = 0;                                                  // 914 0x008AA510
    virtual void AddGossipFactions(CCharString, CCharString) = 0;                                        // 916 0x008AA550
    virtual void AddGossipFactions(CCharString) = 0;                                                     // 915 0x008AA590
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CGameScriptInterfaceBase) == 0x4);
#endif
#endif

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CGAMESCRIPTINTERFACEBASE_H

// End shared header engine/CGameScriptInterfaceBase.h
// Begin shared header engine/CTimer.h
#ifndef FABLE_ENGINE_CTIMER_H
#define FABLE_ENGINE_CTIMER_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration): a 4-byte non-polymorphic handle (Ego_r.pdb CTimer
// {long TimerIndex}; ??1CTimer@@QAE@XZ is non-virtual). Ctor 0x00CD4450 = TimerIndex = interface slot 87 RegisterTimer();
// dtor 0x00CD4470 = interface slot 88 DeregisterTimer(TimerIndex) (both through the global interface pointer 0x0143E8F8).
// The operators are the Ego_r.pdb set; retail expands operator= inline (slot 89 SetTimer): a row that needs the inline body
// defines it out of class after the include.
// Wave 8 (family TIMER, 185 rows): rows whose retail expands the ctor (RegisterTimer), the dtor (DeregisterTimer),
// GetTime (GetTimer) or operator= (SetTimer) define exactly those inline after the include; the invented setters
// SetTime / Set are this operator= (Ego_r.pdb ??4CTimer@@QAEAAV0@J@Z; no SetTime / Set in either PDB).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CTimer.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
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

class CTimer {
public:
    CTimer();                                                            // 0x00CD4450
    ~CTimer();                                                           // 0x00CD4470
    CTimer& operator=(long);
    bool operator==(long) const;
    bool operator!=(long) const;
    bool operator<(long) const;
    bool operator>(long) const;
    bool operator<=(long) const;
    bool operator>=(long) const;
    long GetTime() const;
    long GetIndex() const;
    void Pause();
    void Unpause();
    long TimerIndex;                                                     // +0x00
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CTimer) == 0x4);
#endif
#endif

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CTIMER_H

// End shared header engine/CTimer.h
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
class CCharStringData;
class CCharString {public:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
// Layout: donor member0x1084 minus46 vector differences(4 each), and base difference4, equals native0xfc8.
class CScriptDef:public CParentDefClassBase {unsigned char _pad_0x39[0xfc8-0x39];public:long CoreQuestReminderIntervalSeconds;};
extern CGameScriptInterfaceBase*pDSTGame;
extern CDefPointer<const CScriptDef>pGlobalScriptDef;
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()){}
inline CTimer::~CTimer(){pDSTGame->DeregisterTimer(TimerIndex);}
inline CTimer&CTimer::operator=(long value){pDSTGame->SetTimer(TimerIndex,value);return *this;}
inline long CTimer::GetTime()const{return pDSTGame->GetTimer(TimerIndex);}
namespace NScript {
class CQ_SunnyvaleMasterData;
// Layout: shared level-script base0x48; original public CoreQuestWaiting at native0x48, donor0x5c.
class CGameflowScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:CGameflowScript(CGameScriptInterfaceBase&g):CLevelScriptBase<CQ_SunnyvaleMasterData>(0,g),CoreQuestWaiting(0){} unsigned long CoreQuestWaiting;void CoreQuestReminder();};
void CGameflowScript::CoreQuestReminder(){
 while(!IsActiveThreadTerminating()){
  while(!CoreQuestWaiting){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
  if(IsActiveThreadTerminating())return;
  {
   CTimer timer;timer=pGlobalScriptDef->CoreQuestReminderIntervalSeconds;
   while(timer.GetTime()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
   if(IsActiveThreadTerminating())return;
   if(CoreQuestWaiting&&!Game->IsHeroOnQuest()){
    if(IsActiveThreadTerminating())
#pragma inline_depth(0)
      return;
#pragma inline_depth(255)
    CCharString empty("");CCharString name("TEXT_QST_078_GM_MSG_NEW_QUEST_AT_GUILD");
    Game->HeroReceiveMessageFromGuildMaster(name,empty,true,false);
   }
  }
  Game->NewScriptFrame();
 }
}
}

class C3DVector{public:float X,Y,Z;};
class CRGBColour{public:unsigned char R,G,B,A;};
class CWideStringData;class CWideString{protected:CWideStringData*PStringData;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;};
// Begin shared header engine/CScriptThing.h
#ifndef FABLE_ENGINE_CSCRIPTTHING_H
#define FABLE_ENGINE_CSCRIPTTHING_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CScriptThing = 0xc.
// Retail evidence: 55 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
// Retail opt-in: define FABLE_RETAIL_NSCRIPT for the polymorphic retail class (wave 1 class-model migration, 2026-10-29).
// RTTI .?AVCScriptThing@@ : CBaseClass; vtable 0x01238C8C, 79 slots: slot 0 scalar deleting dtor 0x004AB170 (~CScriptThing
// 0x004AA840), slots 1-78 below with their retail addresses. Every slot name is the retail label of its address and the same
// mangled name is in FableWin.pdb (77 also in Ego_r.pdb; slot 78 IsEqualTo, protected, is FableWin-only); each declaration is
// the VC7.1 undname spelling. The probe build of this header emits all 78 mangled names in retail slot order (HEADERS.md).
// Non-virtual members: ctor 0x006E7B40, copy ctor 0x004ABE90, operator= 0x008AB980, operator== 0x008AAE20, operator!=
// 0x008AAE40 (Ego_r.pdb + FableWin.pdb spellings), NullString 0x0143E8EC. Retail expands the dtor, the copy and the slot
// wrappers inline at some call sites: a row that needs an inline body defines it out of class after the include
// (`inline CScriptThing::~CScriptThing() { }`). Contract: the includer defines CCountedPointer<T> before the include
// (PImp is a by-value member; the row's own CCountedPointer model decides its inline expansion).
#ifdef FABLE_RETAIL_NSCRIPT
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
// Begin shared header engine/fable_nscript_forward.h
#ifndef FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H
#define FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H
// Forward declarations shared by the FABLE_RETAIL_NSCRIPT headers (CScriptThing, CGameScriptInterfaceBase, CScriptBase,
// CEntityScriptBase, CTimer, the NScript predicates, CEntityScriptBinding<T>, CLevelScriptBase<T>). Class keys are the
// ones the FableWin.pdb / Ego_r.pdb mangled names carry (V = class, U = struct: CScreenFilterSThingByPass, std::pair,
// std::less). Enums: VC7.1 accepts an MS forward enum declaration (the row defines the enumerators it uses, before or
// after the include); C++98 has none, so under g++ (syntax checks only) each enum gets an empty placeholder definition.
// std templates: STLport 4.6 parameter lists without defaults (a row's own definition may add the defaults).

class C2DCoordI;
class C2DVector;
class C3DVector;
class CCharString;
class CCounter;
class CRGBColour;
class CRGBFloatColour;
class CRightHandedSet;
class CScriptGameResourceObjectMovieBase;
class CScriptGameResourceObjectScriptedThingBase;
class CScriptThing;
class CThing;
class CTimer;
class CWideString;
class CPersistContext;
class CScriptBase;
class CSpawnedFuncBase;
class CEntityScriptBindingBase;
class CScriptDataBase;
class CActiveEntityScriptBase;
struct CScreenFilterSThingByPass;
class CHeroLogBook;
template <class T> class CArray;
template <class T> class CCountedPointer;
namespace NScript { class CPredicateBase; }
namespace std {
template <class T> class allocator;
template <class T> struct less;
template <class T1, class T2> struct pair;
template <class T, class A> class vector;
template <class T, class A> class list;
template <class K, class C, class A> class set;
template <class K, class V, class C, class A> class map;
}
#if defined(_MSC_VER)
typedef unsigned __int64 FableNScriptUInt64;                           // the PDB's unsigned __int64 (thing unique IDs)
enum EAIStateGroupType;
enum ECameraOp;
enum EClothingCoversArea;
enum ECreatureGroup;
enum ECrime;
enum ECutsceneBehaviour;
enum EDoorTriggerType;
enum EGameAction;
enum EHeroAbility;
enum EHeroTitle;
enum EHeroTrainableStatType;
enum EMiniGameType;
enum EMorality;
enum EMusicSetType;
enum EOpinion;
enum EOpinionAttitudeType;
enum EOpinionDeedType;
enum EOpinionPostDeedType;
enum EOpinionReactionType;
enum EScriptAIPriority;
enum EScriptVillageAttitude;
enum EScriptingStateGroups;
enum ESex;
enum ETargetingType;
enum ETextGroupSelectionMethod;
enum ETutorialCategory;
namespace NEntityEvents { enum EEventType; }
#else
typedef unsigned long FableNScriptUInt64;
enum EAIStateGroupType { };
enum ECameraOp { };
enum EClothingCoversArea { };
enum ECreatureGroup { };
enum ECrime { };
enum ECutsceneBehaviour { };
enum EDoorTriggerType { };
enum EGameAction { };
enum EHeroAbility { };
enum EHeroTitle { };
enum EHeroTrainableStatType { };
enum EMiniGameType { };
enum EMorality { };
enum EMusicSetType { };
enum EOpinion { };
enum EOpinionAttitudeType { };
enum EOpinionDeedType { };
enum EOpinionPostDeedType { };
enum EOpinionReactionType { };
enum EScriptAIPriority { };
enum EScriptVillageAttitude { };
enum EScriptingStateGroups { };
enum ESex { };
enum ETargetingType { };
enum ETextGroupSelectionMethod { };
enum ETutorialCategory { };
namespace NEntityEvents { enum EEventType { }; }
#endif

#endif // FABLE_ENGINE_FABLE_NSCRIPT_FORWARD_H

// End shared header engine/fable_nscript_forward.h
class CScriptThing : public CBaseClass {
public:
    CScriptThing();                                                      // 0x006E7B40
    CScriptThing(const CScriptThing&);                                   // 0x004ABE90
    virtual ~CScriptThing();                                             // slot 0  0x004AB170 (scalar deleting); 0x004AA840
    virtual const CCharString& GetName() const;                                                              //  1 0x004AA8B0
    virtual CCharString GetDefName() const;                                                                  //  2 0x004AA8D0
    virtual CCharString GetDataString() const;                                                               //  3 0x004AA900
    virtual void SetDataString(const CCharString&);                                                          //  4 0x004AA930
    virtual C3DVector GetFocalPos() const;                                                                   //  5 0x004AA940
    virtual const C3DVector& GetPos() const;                                                                 //  6 0x004AA980
    virtual C3DVector GetHomePos() const;                                                                    //  7 0x004AA9A0
    virtual CCharString GetCurrentMapName() const;                                                           //  8 0x004AA9E0
    virtual CCharString GetHomeMapName() const;                                                              //  9 0x004AAA10
    virtual float GetAngleXY() const;                                                                        // 10 0x004AAA40
    virtual CThing* GetPThing() const;                                                                       // 11 0x004AAA60
    virtual FableNScriptUInt64 GetPThingUniqueID() const;                                                     // 12 0x004AAA70
    virtual EAIStateGroupType GetCurrentStateGroupType() const;                                              // 13 0x004AAA90
    virtual EScriptAIPriority GetCurrentScriptPriority() const;                                              // 14 0x004AAAA0
    virtual bool IsSneaking() const;                                                                         // 15 0x004AAAB0
    virtual bool IsAwareOfHero() const;                                                                      // 16 0x004AAAC0
    virtual bool MsgIsTriggeredBy(const CCharString&) const;                                                 // 17 0x004AAAD0
    virtual bool MsgIsKilledBy(const CCharString&) const;                                                    // 18 0x004AAAF0
    virtual CScriptThing MsgWhoKilledMe() const;                                                             // 19 0x004ABE60
    virtual bool MsgIsUsedBy(const CCharString&) const;                                                      // 20 0x004AAB10
    virtual bool MsgIsHitBy(const CCharString&) const;                                                       // 21 0x004AAB30
    virtual bool MsgIsHitByWithFlourish(const CCharString&) const;                                           // 22 0x004AAB50
    virtual bool MsgIsHitByWithDecapitate(const CCharString&) const;                                         // 23 0x004AAB70
    virtual bool MsgIsHitByWithWeapon(const CCharString&, const CCharString&) const;                         // 24 0x004AAB90
    virtual bool MsgIsHitByWithProjectileWeapon(const CCharString&, float&) const;                           // 25 0x004AABB0
    virtual CScriptThing MsgWhoHitMe() const;                                                                // 26 0x004ABEC0
    virtual bool MsgIsTalkedToBy(const CCharString&) const;                                                  // 27 0x004AABD0
    virtual CScriptThing MsgWhoTalkedToMe() const;                                                           // 28 0x004ABF00
    virtual bool MsgExpressionPerformedTo(CCharString&) const;                                               // 29 0x004AABF0
    virtual CScriptThing MsgWhoExpressedToMe() const;                                                        // 30 0x004ABF40
    virtual long MsgHowLongWasExpressionPerformed() const;                                                   // 31 0x004AAC10
    virtual bool MsgIsKnockedOutBy(const CCharString&) const;                                                // 32 0x004AAC20
    virtual CScriptThing MsgWhoKnockedMeOut() const;                                                         // 33 0x004ABF80
    virtual bool MsgReceivedMoney(long&) const;                                                              // 34 0x004AAC40
    virtual bool MsgIsPresentedWithItem(CCharString&) const;                                                 // 35 0x004AAC60
    virtual bool MsgReceivedInventoryItem() const;                                                           // 36 0x004AAC80
    virtual bool MsgPerformedSpecialAbility(EHeroAbility) const;                                             // 37 0x004AACA0
    virtual bool MsgPerformedAnySpecialAbility() const;                                                      // 38 0x004AACC0
    virtual bool MsgPerformedAnyAggressiveSpecialAbility() const;                                            // 39 0x004AACE0
    virtual bool MsgPerformedAnyNonAggressiveSpecialAbility() const;                                         // 40 0x004AAD00
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility, const CCharString&) const;                       // 41 0x004AAD20
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&) const;                                  // 42 0x004AAD40
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString&) const;                        // 43 0x004AAD60
    virtual bool MsgIsHitByAnyNonAggressiveSpecialAbilityFrom(const CCharString&) const;                     // 44 0x004AAD80
    virtual bool MsgOpenedChest() const;                                                                     // 45 0x004AADA0
    virtual bool MsgHitEnemyWithMeleeWeapon() const;                                                         // 46 0x004AADC0
    virtual bool MsgHitFriendWithMeleeWeapon() const;                                                        // 47 0x004AADE0
    virtual bool MsgHitEnemyWithRangedWeapon() const;                                                        // 48 0x004AAE00
    virtual bool MsgHitFriendWithRangedWeapon() const;                                                       // 49 0x004AAE20
    virtual bool MsgHitEnemyWithBareHands() const;                                                           // 50 0x004AAE40
    virtual bool MsgHitFriendWithBareHands() const;                                                          // 51 0x004AAE60
    virtual bool MsgAttackedWithMeleeWeaponWithoutHittingAnything() const;                                   // 52 0x004AAE80
    virtual bool MsgAttackedWithBareHandsWithoutHittingAnything() const;                                     // 53 0x004AAEA0
    virtual bool MsgTalkedToAnyone() const;                                                                  // 54 0x004AAEC0
    virtual bool MsgGetThingsKilled(std::vector<unsigned long,std::allocator<unsigned long> >&) const;       // 55 0x004AAEE0
    virtual bool MsgPerformedFlourish() const;                                                               // 56 0x004AAF00
    virtual bool MsgPerformedSuccessfulFlourish() const;                                                     // 57 0x004AAF20
    virtual bool MsgOnMoralityChanged(long&) const;                                                          // 58 0x004AAF40
    virtual bool MsgOnCutSceneAnimEvent(CCharString&) const;                                                 // 59 0x004AAF60
    virtual bool MsgIsKicked() const;                                                                        // 60 0x004AAF80
    virtual bool IsUnconscious() const;                                                                      // 61 0x004AAFA0
    virtual bool IsUsable() const;                                                                           // 62 0x004AAFC0
    virtual bool IsBeingCarriedBy(const CCharString&) const;                                                 // 63 0x004AAFE0
    virtual bool IsOpenDoor() const;                                                                         // 64 0x004AB000
    virtual bool IsSummonedCreature() const;                                                                 // 65 0x004AB020
    virtual void SetAsUsable(bool);                                                                          // 66 0x004AB040
    virtual void SetFriendsWithEverythingFlag(bool);                                                         // 67 0x004AB060
    virtual bool GetActivationTriggerStatus() const;                                                         // 68 0x004AB080
    virtual void SetActivationTriggerStatus(bool);                                                           // 69 0x004AB0A0
    virtual void SetToKillOnLevelUnload(bool);                                                               // 70 0x004AB0C0
    virtual void UpdateThingAttachment();                                                                    // 71 0x004AB0E0
    virtual void IncrementScriptCounter();                                                                   // 72 0x004AB0F0
    virtual void DecrementScriptCounter();                                                                   // 73 0x004AB100
    virtual long GetScriptCounter();                                                                         // 74 0x004AB110
    virtual bool IsAlive() const;                                                                            // 75 0x004AB130
    virtual bool IsDead() const;                                                                             // 76 0x004AA890
    virtual bool IsNull() const;                                                                             // 77 0x004AB150
    protected: virtual bool IsEqualTo(const CScriptThing&) const; public:                                    // 78 0x004AA8A0 (FableWin.pdb only)
    CScriptThing& operator=(const CScriptThing&);                        // 0x008AB980
    bool operator==(const CScriptThing&) const;                          // 0x008AAE20
    bool operator!=(const CScriptThing&) const;                          // 0x008AAE40
    void SetImplementation(CCountedPointer<CScriptThing>);               // Ego_r.pdb / FableWin.pdb
    CCountedPointer<CScriptThing> PImp;                                  // +0x04 (Data +0x04, Info +0x08)
    static CCharString NullString;                                       // 0x0143E8EC
private:
    void ValidatePImp() const;                                           // FableWin.pdb ?ValidatePImp@CScriptThing@@ABEXXZ
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CScriptThing) == 0xc);
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

class CCPPointerInfo;

#pragma pack(push, 1)
class CScriptThing {
public:
    void*           __vftable;                               // +0x00 vptr, or first dword of a flattened base subobject
    CScriptThing*   PImp_Data;                               // +0x04 CCountedPointer<CScriptThing>::Data
    CCPPointerInfo* PImp_Info;                               // +0x08 CCountedPointer<CScriptThing>::Info
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CScriptThing) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CScriptThing, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CScriptThing, PImp_Data) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CScriptThing, PImp_Info) == 0x8);
#endif
#endif

#endif // FABLE_RETAIL_NSCRIPT

#endif // FABLE_ENGINE_CSCRIPTTHING_H

// End shared header engine/CScriptThing.h
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CGameScriptInterfaceBase::~CGameScriptInterfaceBase(){}
CScriptBase::CScriptBase(){} CScriptBase::~CScriptBase(){}
void CScriptBase::RegisterMain(){}void CScriptBase::Main(){}void CScriptBase::Init(){}void CScriptBase::OnPersist(CPersistContext&){}
template<class T>CLevelScriptBase<T>::~CLevelScriptBase(){}
CGameScriptInterfaceBase*pDSTGame=0;CDefPointer<const CScriptDef>pGlobalScriptDef;
static int query,limit,frames,activate_after,ticks,initial_ticks,registered,deregistered,messages,string_live,hero_queries;
static bool live,on_quest,clear_during_timer;
static NScript::CGameflowScript*active;
bool CScriptBase::IsActiveThreadTerminating(){return query++>=limit;}
CCharString::CCharString(const char*p,long n){require(n==-1);PStringData=reinterpret_cast<CCharStringData*>(const_cast<char*>(p));++string_live;}
CCharString::~CCharString(){--string_live;}
class TestGame:public CGameScriptInterfaceBase{public:
virtual void Error(const CCharString&, const CCharString&, const CCharString&, unsigned long) const {abort();}
virtual void TraceMessage(const CCharString&) const {abort();}
virtual void Validate() const {abort();}
virtual void SetDebugCameraType(long) const {abort();}
virtual void DeactivateBoastUi() const {abort();}
virtual bool IsXbox() const {abort();}
virtual void NewScriptFrame()const{++frames;if(frames>=activate_after)active->CoreQuestWaiting=1;if(live&&clear_during_timer)active->CoreQuestWaiting=0;require(frames<200);}
virtual bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority) {abort();}
virtual bool IsEntityUnderScriptedControl(const CScriptThing&) const {abort();}
virtual bool IsActiveThreadTerminating() const {abort();}
virtual bool IsLevelLoaded(const CCharString&) const {abort();}
virtual bool IsRegionLoaded(const CCharString&) const {abort();}
virtual bool IsRegionLoadedAndPreloaded(const CCharString&) const {abort();}
virtual bool IsRegionDefLoaded(const CCharString&) const {abort();}
virtual const CCharString& GetRegionName() const {abort();}
virtual bool MsgIsLevelLoaded(const CCharString&) const {abort();}
virtual bool MsgIsLevelUnloaded(const CCharString&) const {abort();}
virtual bool MsgOnLevelLoaded(std::list<CCharString,std::allocator<CCharString> >&) const {abort();}
virtual bool MsgOnLevelUnloaded(std::list<CCharString,std::allocator<CCharString> >&) const {abort();}
virtual bool MsgIsRegionLoaded(const CCharString&) const {abort();}
virtual bool MsgIsRegionUnloaded(const CCharString&) const {abort();}
virtual bool MsgOnRegionLoaded(CCharString&) const {abort();}
virtual bool MsgOnRegionUnloaded(CCharString&) const {abort();}
virtual bool MsgOnRegionPreunload(CCharString&) const {abort();}
virtual bool MsgOnQuestCompleted(const CCharString&) const {abort();}
virtual bool MsgOnAnyQuestCompleted(CCharString&) const {abort();}
virtual bool MsgOnQuestFailed(const CCharString&) const {abort();}
virtual bool MsgOnQuestCompletedBeforeScreenShown(const CCharString&) const {abort();}
virtual bool MsgOnQuestFailedBeforeScreenShown(const CCharString&) const {abort();}
virtual bool MsgOnQuestAccepted(CCharString&) const {abort();}
virtual bool MsgOnFeatAccepted(long&) const {abort();}
virtual bool MsgIsBoastMade(long) const {abort();}
virtual bool MsgOnBoastMade(long&, CCharString&) const {abort();}
virtual bool MsgOnBoastsMade(std::vector<std::pair<long,CCharString>,std::allocator<std::pair<long,CCharString> > >&) const {abort();}
virtual void RemoveBoastMessage() const {abort();}
virtual bool IsQuestStartScreenActive(CCharString&) const {abort();}
virtual bool MsgOnLeavingQuestStartScreen(CCharString&) const {abort();}
virtual bool MsgOnLeavingExperienceSpendingScreen() const {abort();}
virtual long MsgIsQuestionAnsweredYesOrNo() const {abort();}
virtual bool MsgIsGameInfoClickedPast() const {abort();}
virtual bool MsgIsTutorialClickedPast() const {abort();}
virtual bool MsgIsActionModeButtonPressed() const {abort();}
virtual bool MsgOnExpressionPerformed(CCharString&) const {abort();}
virtual bool MsgIsCutSceneSkipped() const {abort();}
virtual void RemoveAllCutSceneSkippedMessages() const {abort();}
virtual bool MsgOnHeroHairTypeChanged(EClothingCoversArea, CCharString&) const {abort();}
virtual bool MsgOnHeroUsedTeleporter(CCharString&) const {abort();}
virtual bool MsgOnHeroUsedGuildSeal() const {abort();}
virtual bool MsgOnGameSavedManually() const {abort();}
virtual bool MsgOnHeroSlept(bool&) const {abort();}
virtual bool MsgOnHeroFiredRangedWeapon() const {abort();}
virtual bool MsgOnHeroCastSpell(EHeroAbility&) const {abort();}
virtual bool MsgOnHeroPickedPocket(CScriptThing&) const {abort();}
virtual bool MsgOnHeroPickedLock(CScriptThing&) const {abort();}
virtual bool MsgOnFishingGameFinished(CScriptThing&) const {abort();}
virtual bool MsgOnTavernGameFinished(const CScriptThing&) const {abort();}
virtual bool MsgOnHeroRewardedWithItemsFrom(CCharString&) const {abort();}
virtual bool MsgOnChestOpeningCancelled() const {abort();}
virtual void DontPopulateNextLoadedRegion() const {abort();}
virtual void GetWanderingPopulationScriptDefNameInCurrentRegion(CCharString&) const {abort();}
virtual void GetWanderingPopulationScriptDefNameInRegion(const CCharString&, CCharString&) const {abort();}
virtual bool IsHeroAllowedHenchmenInCurrentRegion() const {abort();}
virtual bool IsHeroAllowedHenchmenInRegion(const CCharString&) const {abort();}
virtual void PostAddScriptedEntities() const {abort();}
virtual bool IsPlayerHoldingUnsheatheRangedWeaponButton() const {abort();}
virtual bool IsPlayerHoldingLockTargetButton() const {abort();}
virtual bool IsPlayerHoldingFireRangedWeaponButton() const {abort();}
virtual bool IsPlayerHoldingFirstPersonTargetingButton() const {abort();}
virtual bool IsHeroInProjectileWeaponMode() const {abort();}
virtual CScriptThing& GetHero() {abort();}
virtual CScriptThing GetHeroTargetedThing() const {abort();}
virtual CScriptThing GetThingWithScriptName(const CCharString&, const CCharString&) const {abort();}
virtual CScriptThing GetThingWithScriptName(const CCharString&) const {abort();}
virtual CScriptThing GetRandomThingWithScriptName(const CCharString&) const {abort();}
virtual long GetAllThingsWithScriptName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const {abort();}
virtual long GetAllCreaturesInAreaWithScriptName(const CCharString&, const C3DVector&, float, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const {abort();}
virtual CScriptThing GetNearestWithScriptName(const CScriptThing&, const CCharString&) const {abort();}
virtual CScriptThing GetFurthestWithScriptName(const CScriptThing&, const CCharString&) const {abort();}
virtual long GetAllThingsWithDefNameByDistanceFrom(const CScriptThing&, const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const {abort();}
virtual long GetAllThingsWithDefName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const {abort();}
virtual CScriptThing GetNearestWithDefName(const CScriptThing&, const CCharString&) const {abort();}
virtual CScriptThing GetFurthestWithDefName(const CScriptThing&, const CCharString&) const {abort();}
virtual CScriptThing GetThingWithUID(unsigned __int64) const {abort();}
virtual long GetAllCreaturesExcludingHero(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const {abort();}
virtual long GetAllThingsInLevel(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const {abort();}
virtual bool IsThingWithThisUIDAlive(unsigned __int64) const {abort();}
virtual long RegisterTimer(){require(!live);live=true;++registered;return 17;}
virtual void DeregisterTimer(long index){require(index==17&&live);live=false;++deregistered;}
virtual void SetTimer(long index,long n){require(index==17&&live&&n==initial_ticks);ticks=(int)n;}
virtual long GetTimer(long index)const{require(index==17&&live);if(ticks>0)return ticks--;return 0;}
virtual CScriptThing CreateCreature(const CCharString&, const C3DVector&, const CCharString&, bool) const {abort();}
virtual CScriptThing CreateCreatureNearby(const CCharString&, const C3DVector&, float, const CCharString&, bool) const {abort();}
virtual CScriptThing CreateCreatureOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const {abort();}
virtual CScriptThing TurnCreatureInto(const CScriptThing&, const CCharString&) const {abort();}
virtual void SetCreatureCreationDelayFrames(long) {abort();}
virtual void ResetCreatureCreationDelayFrames() {abort();}
virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, const CCharString&) const {abort();}
virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, float, const CCharString&) const {abort();}
virtual CScriptThing CreateObjectOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const {abort();}
virtual CScriptThing CreateEffect(const CCharString&, const CScriptThing&, const CCharString&, const CCharString&, bool, bool) const {abort();}
virtual CScriptThing CreateEffect(const CCharString&, const C3DVector&, const CCharString&, float, bool, bool) const {abort();}
virtual CScriptThing CreateLight(const C3DVector&, const CRGBColour&, const CCharString&, float, float, bool) const {abort();}
virtual CScriptThing CreateExperienceOrb(const C3DVector&, long) const {abort();}
virtual CScriptThing CreateExplosion(const CCharString&, const C3DVector&, const C3DVector*, CCharString) const {abort();}
virtual CScriptThing CreatePhysicalBarrier(float, const C3DVector&, const C3DVector*, CCharString) const {abort();}
virtual CScriptThing CreateRumble(const C3DVector&, float, float, CCharString) const {abort();}
virtual void ClearAllRumbles() const {abort();}
virtual void RemoveThing(const CScriptThing&, bool, bool) const {abort();}
virtual void ShowOnScreenMessage(const CCharString&, float) const {abort();}
virtual void ShowOnScreenMessage(const CCharString&, const CCharString&, const CCharString&, float) const {abort();}
virtual void ShowOnScreenMessage(const C2DVector&, const CCharString&, const CRGBColour&, const CCharString&) const {abort();}
virtual void AddScreenMessage(const CCharString&, ETextGroupSelectionMethod) const {abort();}
virtual void AddScreenTitleMessage(const CCharString&, float, bool) const {abort();}
virtual void GiveHeroYesNoQuestion(const CCharString&, const CCharString&, const CCharString&, const CCharString&, bool) const {abort();}
virtual void DisplayGameInfo(const CCharString&) const {abort();}
virtual void DisplayGameInfoText(const CWideString&) const {abort();}
virtual bool IsSafeToDisplayGameInfo() const {abort();}
virtual bool DisplayTutorial(ETutorialCategory) const {abort();}
virtual bool IsTutorialSystemEnabled() const {abort();}
virtual void GiveHeroWeapon(const CCharString&, bool) const {abort();}
virtual void GiveHeroObject(const CCharString&, long, bool) const {abort();}
virtual void SetWeaponAsHerosActiveWeapon(const CCharString&) const {abort();}
virtual void GiveHeroItem(const CScriptThing&) const {abort();}
virtual bool GiveHeroItemsFromContainer(const CScriptThing&, bool) const {abort();}
virtual void TakeObjectFromHero(const CCharString&) const {abort();}
virtual void GiveHeroGold(long) const {abort();}
virtual long GetHeroGold() const {abort();}
virtual void GiveHeroExperience(long) const {abort();}
virtual void SetHeroAbleToGainExperience(bool) const {abort();}
virtual void SheatheHeroWeapons() const {abort();}
virtual void SetHeroWillAsUsable(bool) const {abort();}
virtual void SetHeroWeaponsAsUsable(bool) const {abort();}
virtual void SetWeaponOutCrimeEnabled(bool) const {abort();}
virtual void SetGuardsIgnoreCrimes(bool) const {abort();}
virtual void RemoveAllHeroWeapons() const {abort();}
virtual bool IsReportedOrUnreportedCrimeKnown(const CScriptThing&) const {abort();}
virtual void ConfiscateAllHeroItems() const {abort();}
virtual void ConfiscateAllHeroWeapons() const {abort();}
virtual void ConfiscateItemsOfTypeFromHero(const CCharString&) const {abort();}
virtual void ReturnAllConfiscatedItemsToHero() const {abort();}
virtual bool GiveHeroTutorial(ETutorialCategory) const {abort();}
virtual void MakeHeroCarryItemInHand(const CCharString&) const {abort();}
virtual void MakeHeroCarryItemInHand(const CScriptThing&, bool, bool) const {abort();}
virtual void AddTattooToHero(const CCharString&) const {abort();}
virtual void GiveHeroAbility(EHeroAbility, bool) const {abort();}
virtual bool IsPlayerZTargetingThing(const CScriptThing&) const {abort();}
virtual bool IsPlayerCreatureBlocking() const {abort();}
virtual bool IsPlayerCreatureReadyToFireProjectileWeapon(float&) const {abort();}
virtual long GetPlayerCreatureCombatMultiplier() const {abort();}
virtual long GetPlayerCreatureCombatMultiplierRunningNumHits() const {abort();}
virtual void ResetPlayerCreatureCombatMultiplier() const {abort();}
virtual bool IsPlayerCreatureFlourishEnabled() const {abort();}
virtual void SetPlayerCreatureOnlyTarget(const CScriptThing&) const {abort();}
virtual void ResetPlayerCreatureOnlyTarget() const {abort();}
virtual void RespawnHero(bool) const {abort();}
virtual void GiveHeroMorality(float) const {abort();}
virtual float GetHeroMorality() const {abort();}
virtual EMorality GetHeroMoralityCategory() const {abort();}
virtual void ChangeHeroMoralityDueToTheft() const {abort();}
virtual void ChangeHeroMoralityDueToPicklock() const {abort();}
virtual void GiveHeroRenownPoints(long) const {abort();}
virtual long GetHeroRenownLevel() const {abort();}
virtual bool IsHeroRenownLevelFull() const {abort();}
virtual void IncreaseHeroRenownLevel() const {abort();}
virtual long GetHeroStrengthLevel() const {abort();}
virtual long GetHeroSkillLevel() const {abort();}
virtual long GetHeroWillLevel() const {abort();}
virtual long GetHeroStatLevel(EHeroTrainableStatType) const {abort();}
virtual long GetHeroStatMax(EHeroTrainableStatType) const {abort();}
virtual void SetHeroAge(float) const {abort();}
virtual float GetHeroAge() const {abort();}
virtual void SetHeroAsTeenager(bool) const {abort();}
virtual void SetHeroAsApprentice(bool) const {abort();}
virtual float GetDistanceHeroCanBeHeardFrom() const {abort();}
virtual long GetHeroRoughExperienceLevel() const {abort();}
virtual long GetHeroExperienceAvailableToSpend() const {abort();}
virtual float GetHeroFatness() const {abort();}
virtual float GetHeroScariness() const {abort();}
virtual float GetHeroAttractiveness() const {abort();}
virtual float GetHeroWillEnergyLevel() const {abort();}
virtual void SetHeroWillEnergyLevel(float) const {abort();}
virtual void SetHeroWillEnergyAsAbleToRefill(bool) const {abort();}
virtual long GetNumberOfItemsOfTypeInInventory(const CCharString&) const {abort();}
virtual bool IsObjectInThingsPossession(const CCharString&, const CScriptThing&) const {abort();}
virtual bool IsHeroHandLampLit() const {abort();}
virtual void SetHeroHandLampAsLit(bool) const {abort();}
virtual bool IsWearingClothingItem(const CScriptThing&, const CCharString&) const {abort();}
virtual bool IsHeroNaked() const {abort();}
virtual void RemoveHeroClothing() const {abort();}
virtual void SetHeroAsWearing(const CCharString&) const {abort();}
virtual void ChangeHeroHairstyle(const CCharString&) const {abort();}
virtual void RemoveHeroHairstyle() const {abort();}
virtual bool IsWearingHairstyle(CScriptThing&, const CCharString&) const {abort();}
virtual bool IsPlayerCarryingItemOfType(const CCharString&) const {abort();}
virtual bool IsPlayerWieldingWeapon() const {abort();}
virtual bool IsEntityWieldingWeapon(const CScriptThing&) const {abort();}
virtual bool IsEntityWieldingMeleeWeapon(const CScriptThing&) const {abort();}
virtual bool IsEntityWieldingRangedWeapon(const CScriptThing&) const {abort();}
virtual bool GetPreviouslyWieldedMeleeWeaponName(CCharString&) const {abort();}
virtual bool GetPreviouslyWieldedRangedWeaponName(CCharString&) const {abort();}
virtual bool IsEntityAbleToAttack(const CScriptThing&) const {abort();}
virtual CScriptThing EntityGetThingInPrimarySlot(const CScriptThing&) const {abort();}
virtual void ApplyHeroPenaltyForDeath() const {abort();}
virtual void GiveHeroTitle(const CCharString&) const {abort();}
virtual EHeroTitle GetHeroTitle() const {abort();}
virtual void EntitySetAsMarryable(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing&, bool) const {abort();}
virtual void EntityForceMarriageToHero(const CScriptThing&, bool) const {abort();}
virtual bool IsEntityMarriedToHero(const CScriptThing&) const {abort();}
virtual bool IsEntityMarriable(const CScriptThing&) const {abort();}
virtual bool GetHeroHasMarried() const {abort();}
virtual bool GetHeroHasCurrentMarriage() const {abort();}
virtual bool GetHeroHasDivorcedMarriage() const {abort();}
virtual bool GetHeroHasChildren() const {abort();}
virtual bool GetHeroHasMurderedWife() const {abort();}
virtual bool IsHeroChild() const {abort();}
virtual void CancelHeroTeleportEffects() const {abort();}
virtual long GetNumberOfTimesHeroHasHadSex() const {abort();}
virtual void SetNumberOfTimesHeroHasHadSex(long) const {abort();}
virtual void SetHeroAsHavingHadSex(bool) const {abort();}
virtual void SetHeroAsHavingHadGaySex(bool) const {abort();}
virtual void GiveThingHeroRewardItem(const CScriptThing&, const CCharString&, const CCharString&) const {abort();}
virtual void GiveThingItemInHand(const CScriptThing&, const CCharString&, bool) const {abort();}
virtual void GiveThingItemInSlot(const CScriptThing&, const CCharString&, const CCharString&) const {abort();}
virtual void GiveHeroExpression(const CCharString&, long, bool) const {abort();}
virtual bool HeroHasExpression(const CCharString&) const {abort();}
virtual bool IsHeroPerformingExpression(const CCharString&) const {abort();}
virtual bool IsDPadButtonHeldForExpression(const CCharString&) const {abort();}
virtual void EntityFollowThing(const CScriptThing&, const CScriptThing&, float, bool) const {abort();}
virtual void EntityStopFollowing(const CScriptThing&) const {abort();}
virtual bool GetFollowingEntityList(const CScriptThing&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const {abort();}
virtual bool GetHeroSummonedCreaturesList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const {abort();}
virtual bool GetPerceivingHeroEntityList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const {abort();}
virtual bool IsEntityFollowingHero(const CScriptThing&) const {abort();}
virtual void EntitySetAsAllowedToFollowHero(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAsAllowedToChangeRegionFollowingState(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(const CScriptThing&, bool) const {abort();}
virtual void TeleportAllFollowersToHeroPosition() const {abort();}
virtual void EntityTeleportToHeroPosition(const CScriptThing&) const {abort();}
virtual void SendEntityEvent(NEntityEvents::EEventType, const CScriptThing&, const CScriptThing&, CThing*) const {abort();}
virtual float GetWaterHeightAtPosition(const C3DVector&) const {abort();}
virtual bool IsFishingSpotEnabled(const CScriptThing&) const {abort();}
virtual void DisableFishingSpot(const CScriptThing&) const {abort();}
virtual void UpdateFishWeight(const CScriptThing&) const {abort();}
virtual float GetBestFishWeight(const CCharString&) const {abort();}
virtual void HeroGoFishing(bool) const {abort();}
virtual long GetHeroFishingLevel() const {abort();}
virtual void HeroGoDigging() const {abort();}
virtual void HeroStopDigging() const {abort();}
virtual void HeroPlayOracleMinigame() const {abort();}
virtual bool IsHeroPlayingOracleMinigame() const {abort();}
virtual bool HasHeroWonOracleMinigame() const {abort();}
virtual void HeroPlayFireheartMinigame() const {abort();}
virtual void HeroQuitFireheartMinigame() const {abort();}
virtual bool HasHeroForceQuitFireheartMinigame() const {abort();}
virtual float GetHeroHealth() const {abort();}
virtual float GetHeroHealthMax() const {abort();}
virtual float GetHeroHealthPercentage() const {abort();}
virtual long GetHeroWillEnergy() const {abort();}
virtual long GetHeroWillEnergyMax() const {abort();}
virtual long GetHeroWillEnergyPercentage() const {abort();}
virtual void ChangeHeroHealthBy(float, bool, bool) const {abort();}
virtual float GetHealth(const CScriptThing&) const {abort();}
virtual void ModifyThingHealth(const CScriptThing&, float, bool) const {abort();}
virtual void EntitySetMaxHealth(const CScriptThing&, float, bool) const {abort();}
virtual void SetThingAsKilled(const CScriptThing&) const {abort();}
virtual void GiveHeroNewQuestObjective(const CCharString&, unsigned long) const {abort();}
virtual void TellHeroQuestObjectiveCompleted(unsigned long) const {abort();}
virtual void TellHeroQuestObjectiveFailed(unsigned long) const {abort();}
virtual void AddQuestRegion(const CCharString&, const CCharString&) const {abort();}
virtual void SetQuestWorldMapOffset(const CCharString&, const C2DCoordI&) const {abort();}
virtual bool IsHeroOnQuest()const{++hero_queries;require(live&&active->CoreQuestWaiting);return on_quest;}
virtual void HeroReceiveMessageFromGuildMaster(const CCharString&a,const CCharString&b,bool c,bool e)const{require(live&&active->CoreQuestWaiting&&!on_quest&&c&&!e&&string_live==2);require(strcmp(reinterpret_cast<const char*>(a.PStringData),"TEXT_QST_078_GM_MSG_NEW_QUEST_AT_GUILD")==0);require(strcmp(reinterpret_cast<const char*>(b.PStringData),"")==0);++messages;}
virtual void SetGuildMasterMessages(bool) const {abort();}
virtual void ActivateQuest(const CCharString&) const {abort();}
virtual void ActivateMultipleQuests(const CArray<CCharString>&) const {abort();}
virtual void ActivateQuestWithoutLoadingResources(const CCharString&) const {abort();}
virtual void ActivateMultipleQuestsWithoutLoadingResources(const CArray<CCharString>&) const {abort();}
virtual void DeactivateQuest(const CCharString&, unsigned long) const {abort();}
virtual void DeactivateQuestLater(const CCharString&, unsigned long) const {abort();}
virtual void PrepareQuestsWhenFinalQuestIsActivated(const CCharString&) const {abort();}
virtual void PrepareQuestsWhenFinalQuestIsCompleted() const {abort();}
virtual bool IsQuestActive(const CCharString&) const {abort();}
virtual bool IsQuestRegistered(const CCharString&) const {abort();}
virtual bool IsQuestCompleted(const CCharString&) const {abort();}
virtual bool IsQuestFailed(const CCharString&) const {abort();}
virtual void SetQuestAsCompleted(const CCharString&, bool, bool, bool) const {abort();}
virtual void SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool) const {abort();}
virtual void SetQuestAsPersistent(const CCharString&, bool) const {abort();}
virtual const CCharString& GetExclusiveQuestScriptName() const {abort();}
virtual void AddQuestCard(const CCharString&, const CCharString&, bool, bool) const {abort();}
virtual void RemoveQuestCardFromGuild(const CCharString&) const {abort();}
virtual void RemoveQuestCardFromHero(const CCharString&) const {abort();}
virtual void GiveHeroQuestCardDirectly(const CCharString&, const CCharString&, bool) const {abort();}
virtual void SetQuestCardObjective(const CCharString&, const CCharString&, const CCharString&, const CCharString&) const {abort();}
virtual void SetQuestCardGoldReward(const CCharString&, long) const {abort();}
virtual void SetQuestCardRenownReward(const CCharString&, long) const {abort();}
virtual void RemoveAllAvailableQuestCardsFromGuild() const {abort();}
virtual void FailAllActiveQuests() const {abort();}
virtual void GetAllActiveQuestInfo(std::vector<CCharString,std::allocator<CCharString> >&, std::vector<CCharString,std::allocator<CCharString> >&) const {abort();}
virtual void AddFeatCard(long, const CCharString&, const CCharString&) const {abort();}
virtual void AddBoast(const CCharString&, long, long, long, bool, const CCharString&, long) const {abort();}
virtual void RemoveBoast(long, const CCharString&) const {abort();}
virtual void SetBoastAsFailed(long, const CCharString&) const {abort();}
virtual void SetBoastAsCompleted(long, const CCharString&) const {abort();}
virtual bool IsBoastTaken(long, const CCharString&) const {abort();}
virtual void AddLogBookEntry(const CWideString&, const CWideString&, const CWideString&, long) const {abort();}
virtual void KickOffQuestStartScreen(const CCharString&, bool, bool) const {abort();}
virtual void KickOffDeathScreen() const {abort();}
virtual void KickOffCreditsScreen(CCharString&) const {abort();}
virtual void SetPreferredQuickAccessItem(const CCharString&, long, long) const {abort();}
virtual CCharString GetDeathRecoveryMarkerName() const {abort();}
virtual void SetDeathRecoveryMarkerName(const CCharString&) const {abort();}
virtual void ResetDeathRecoveryMarkerNameToDefault() const {abort();}
virtual bool IsToFailQuestOnDeath() const {abort();}
virtual void SetWhetherToFailQuestOnDeath(bool) const {abort();}
virtual void ResetWhetherToFailQuestOnDeathToDefault() const {abort();}
virtual CScriptThing GetMostRecentValidUsedTarget() const {abort();}
virtual CCharString GetMostRecentValidUsedTargetName() const {abort();}
virtual void DisplayQuestInfo(bool) const {abort();}
virtual void SetQuestInfoName(const char*) const {abort();}
virtual void SetQuestInfoText(const char*) const {abort();}
virtual long AddQuestInfoBar(float, float, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float) const {abort();}
virtual long AddQuestInfoBarHealth(const CScriptThing&, const CRGBColour&, const CCharString&, float) const {abort();}
virtual long AddQuestInfoTimer(const CTimer&, const CCharString&, float) const {abort();}
virtual long AddQuestInfoCounter(const CCounter&, const CCharString&, float) const {abort();}
virtual long AddQuestInfoCounter(const CCharString&, long, float) const {abort();}
virtual long AddQuestInfoCounterList(const CCharString&, long, float) const {abort();}
virtual long AddQuestInfoTick(const CCharString&, bool, float) const {abort();}
virtual long AddQuestInfoTick(EGameAction, bool, float) const {abort();}
virtual void UpdateQuestInfoBar(long, float, float, float) const {abort();}
virtual void ChangeQuestInfoBarColour(long, const CRGBColour&, const CRGBColour&) const {abort();}
virtual void UpdateQuestInfoTimer(long, float) const {abort();}
virtual void UpdateQuestInfoCounter(long, long, long) const {abort();}
virtual void UpdateQuestInfoCounterList(long, long, long) const {abort();}
virtual void UpdateQuestInfoTick(long, bool) const {abort();}
virtual void RemoveQuestInfoElement(long) const {abort();}
virtual void RemoveAllQuestInfoElements() const {abort();}
virtual void DisplayTime(bool) const {abort();}
virtual void DisplayMoneyBag(bool) const {abort();}
virtual void DisplayMiniGameInfo(bool, EMiniGameType) const {abort();}
virtual void UpdateMiniGameInfoBar(float) const {abort();}
virtual bool IsEntityPickPocketable(const CScriptThing&) const {abort();}
virtual bool IsEntityPickLockable(const CScriptThing&) const {abort();}
virtual bool IsEntityStealable(const CScriptThing&) const {abort();}
virtual void EntitySetAsPickPocketed(const CScriptThing&) const {abort();}
virtual void EntitySetAsPickLocked(const CScriptThing&) const {abort();}
virtual void EntitySetAsStolen(const CScriptThing&) const {abort();}
virtual void MiniMapAddMarker(const CScriptThing&, const CCharString&) const {abort();}
virtual void MiniMapSetMarkerGraphic(const CScriptThing&, const CCharString&) const {abort();}
virtual void MiniMapRemoveMarker(const CScriptThing&) const {abort();}
virtual void MiniMapRemoveAllMarkers() const {abort();}
virtual void MiniMapAllowRouteBetweenRegions(const CCharString&, const CCharString&, bool) const {abort();}
virtual void MiniMapSetAsEnabled(bool) const {abort();}
virtual void EntitySetAsHiddenOnMiniMap(const CScriptThing&, bool) const {abort();}
virtual void SetHUDEnabled(bool) const {abort();}
virtual bool TextEntryExists(const CCharString&) const {abort();}
virtual CCharString GetValidTextEntryNameWithAttitude(const CScriptThing&, const CCharString&) const {abort();}
virtual void SetThingHasInformation(const CScriptThing&, bool, bool, bool) const {abort();}
virtual void ClearThingHasInformation(const CScriptThing&) const {abort();}
virtual void EntitySetWillBeUsingNarrator(const CScriptThing&, const CCharString&) const {abort();}
virtual void EntityResetAsPureAINarrator(const CScriptThing&) const {abort();}
virtual long AddNewConversation(const CScriptThing&, bool, bool) const {abort();}
virtual void AddPersonToConversation(long, const CScriptThing&) const {abort();}
virtual void AddLineToConversation(long, const CCharString&, bool, const CScriptThing&, const CScriptThing&) const {abort();}
virtual void RemoveConversation(long, bool) const {abort();}
virtual bool IsConversationActive(long) const {abort();}
virtual void PlayAVIMovie(const CCharString&) const {abort();}
virtual void StartMovieSequence(const CCharString&, CScriptGameResourceObjectMovieBase&) {abort();}
virtual void FixMovieSequenceCamera(bool) const {abort();}
virtual void FadeScreenOutUntilNextCallToFadeScreenIn(float, float) {abort();}
virtual bool FadeScreenOut(float, float, CRGBColour) {abort();}
virtual void FadeScreenIn() {abort();}
virtual bool IsScreenFadingOut() const {abort();}
virtual void Pause(float) const {abort();}
virtual void EndCutFade() {abort();}
virtual void EndLetterBox() {abort();} public:
virtual void PauseAllNonScriptedEntities(bool) {abort();}
virtual void PauseAllEntities(bool) {abort();}
virtual void SetAllowScreenFadingOnNextRegionChange(bool) {abort();}
virtual void SetAllowScreenFadingIfAlreadyFaded(bool) {abort();}
virtual void SetToKeepHeroAbilitiesDuringCutscenes(bool) const {abort();}
virtual void SetToDisplayTutorialsDuringCutscenes(bool) const {abort();}
virtual void SetCutsceneMode(bool, bool) const {abort();}
virtual bool IsInCutscene() const {abort();}
virtual void SetCutsceneSkippable(bool) const {abort();}
virtual void SetCutsceneSkippableWhilePaused(bool) const {abort();}
virtual void SetCutsceneActionMode(bool, const CCharString&) const {abort();}
virtual void PreloadNewScene(float) const {abort();}
virtual void StartProgressDisplay() const {abort();}
virtual void StopProgressDisplay() const {abort();}
virtual void SetScreenMessagesEnabled(bool) const {abort();}
virtual bool IsHeroControlledByPlayer() const {abort();}
virtual bool IsInMovieSequence() const {abort();}
virtual void CancelAbilitiesForCutscene() const {abort();}
virtual void ResumeAbilitiesForCutscene() const {abort();}
virtual void CancelUsingAbility(EHeroAbility) const {abort();}
virtual void SetAbilityAvailability(EHeroAbility, bool) const {abort();}
virtual void SetEnvironmentalEffectsAlwaysUpdate(bool) const {abort();}
virtual void SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool) const {abort();}
virtual void RemoveDeadCreature(const CCharString&) const {abort();}
virtual void CameraSetCameraPreloadFlag(bool) const {abort();}
virtual void CameraCircleAroundThing(const CScriptThing&, const C3DVector&, float) const {abort();}
virtual void CameraCircleAroundPos(const C3DVector&, const C3DVector&, float) const {abort();}
virtual void CameraMoveToPosAndLookAtPos(const C3DVector&, const C3DVector&, float) const {abort();}
virtual void CameraMoveToPosAndLookAtThing(const C3DVector&, const CScriptThing&, float) const {abort();}
virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const C3DVector&, float, float) const {abort();}
virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const CScriptThing&, float, float) const {abort();}
virtual void CameraMoveBetweenLookFromAndLookTo(const C3DVector&, const C3DVector&, const C3DVector&, const C3DVector&, float) const {abort();}
virtual void CameraUseCameraPoint(const CCharString&, const CScriptThing&, float, long, long) const {abort();}
virtual void CameraUseCameraPoint(const CScriptThing&, const CScriptThing&, float, long, long) const {abort();}
virtual void CameraUseCameraPoint(const CCharString&, const C3DVector*, const CRightHandedSet*, float, long, long) const {abort();}
virtual void CameraUseCameraPoint(const CScriptThing&, const C3DVector*, const CRightHandedSet*, float, long, long) const {abort();}
virtual void CameraDoConversation(const CScriptThing&, const CScriptThing&, ECameraOp, bool) const {abort();}
virtual void CameraDefault() {abort();}
virtual void CameraResetToViewBehindHero(float) {abort();}
virtual bool IsCameraInScriptedMode() const {abort();}
virtual void CameraUseScreenEffect(float, float, float) const {abort();}
virtual void CameraCancelScreenEffect() const {abort();}
virtual bool IsCameraPosOnScreen(const C3DVector&) const {abort();}
virtual float GetGameAngleXY() const {abort();}
virtual void CameraEarthquakeIntensityAtPos(const C3DVector&, float, float) const {abort();}
virtual void CameraShake(float, float) const {abort();}
virtual void OpenDoor(const CScriptThing&) const {abort();}
virtual void CloseDoor(const CScriptThing&) const {abort();}
virtual void OpenHouseDoors(const CScriptThing&) const {abort();}
virtual void CloseHouseDoors(const CScriptThing&) const {abort();}
virtual void JamDoor(const CScriptThing&) const {abort();}
virtual void SetDoorTriggerType(const CScriptThing&, EDoorTriggerType) const {abort();}
virtual void OverrideAutomaticHouseLocking(const CScriptThing&, bool) const {abort();}
virtual void SetHouseOwnedByPlayer(const CScriptThing&, bool, bool) const {abort();}
virtual void SetBuyableHouseAsScripted(const CScriptThing&, bool) const {abort();}
virtual bool IsChestOpen(const CScriptThing&) const {abort();}
virtual bool OpenChest(const CScriptThing&, bool) const {abort();}
virtual void CloseChest(const CScriptThing&) const {abort();}
virtual long GetNumberOfKeysNeededToUnlockChest(const CScriptThing&, CCharString&) const {abort();}
virtual void DisplayLockedChestMessage(const CScriptThing&) const {abort();}
virtual void SetTrophyAsMountable(const CScriptThing&, bool) const {abort();}
virtual void SetVillageLimbo(const CScriptThing&, bool) const {abort();}
virtual void SetCreatureNotReload(const CScriptThing&) const {abort();}
virtual bool IsSleepingTime(const CScriptThing&) const {abort();}
virtual void EnableGuards(const CScriptThing&, bool) const {abort();}
virtual void EnableVillagerDefTypes(const CScriptThing&, bool, const CCharString&) const {abort();}
virtual CScriptThing TryToRespawnDefNamed(const CScriptThing&, const CCharString&, const C3DVector&) const {abort();}
virtual void ClearHeroEnemyOfGuards(const CScriptThing&) const {abort();}
virtual void SetThingAsUsable(const CScriptThing&, bool) const {abort();}
virtual void SetThingHomeBuilding(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void SetVillageAttitude(const CScriptThing&, EScriptVillageAttitude) const {abort();}
virtual void AddCrimeCommitted(const CScriptThing&, ECrime, bool, const CScriptThing*, const CScriptThing*, EOpinionPostDeedType) const {abort();}
virtual void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void ClearThingBestEnemyTarget(const CScriptThing&) const {abort();}
virtual void EntitySetInLimbo(const CScriptThing&, bool, bool) const {abort();}
virtual bool IsEntityInLimbo(const CScriptThing&) const {abort();}
virtual bool EntityGetShotStrikePos(const CScriptThing&, C3DVector&) const {abort();}
virtual void EntitySetNegateAllHits(const CScriptThing&, bool) const {abort();}
virtual void EntitySetEvadeAllHits(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAbleToBeEngagedInCombat(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAlwaysBlockAttacksFromThing(const CScriptThing&, const CScriptThing&, bool) const {abort();}
virtual void EntitySetAttackThingImmediately(const CScriptThing&, const CScriptThing&, bool, bool) const {abort();}
virtual void EntitySetCombatType(const CScriptThing&, const CCharString&) const {abort();}
virtual void EntityResetCombatTypeToDefault(const CScriptThing&) const {abort();}
virtual void EntitySetMaxNumberOfAttackers(const CScriptThing&, long) const {abort();}
virtual void EntityClearMaxNumberOfAttackers(const CScriptThing&) const {abort();}
virtual void EntityAttachToScript(const CScriptThing&, const CCharString&) const {abort();}
virtual void EntitySetCombatAbility(const CScriptThing&, float) const {abort();}
virtual void EntitySetRangedTarget(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void EntityClearRangedTarget(const CScriptThing&) const {abort();}
virtual void EntitySetTargetable(const CScriptThing&, bool) const {abort();}
virtual void EntitySetTargetingType(const CScriptThing&, ETargetingType) const {abort();}
virtual void EntitySetTargetingValidTargetWithoutLOS(const CScriptThing&, bool) const {abort();}
virtual void EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool) const {abort();}
virtual void EntityTeleportToPosition(const CScriptThing&, const C3DVector&, float, bool, bool) const {abort();}
virtual void EntitySetFacingAngle(const CScriptThing&, float, bool) const {abort();}
virtual void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) const {abort();}
virtual void EntitySetPerceptionVariables(const CScriptThing&, float, float, float) const {abort();}
virtual void SetThingPersistent(const CScriptThing&, bool) const {abort();}
virtual void EntitySetThingAsWantingMoney(const CScriptThing&, long) const {abort();}
virtual void EntitySetAppearanceMorphSeed(const CScriptThing&, long) const {abort();}
virtual void SetEntityAsRegionFollowing(const CScriptThing&, const CScriptThing&, bool) const {abort();}
virtual void SetEntityAsFollowingHeroThroughTeleporters(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAppearanceSeed(const CScriptThing&, unsigned long) const {abort();}
virtual void EntityGetAppearanceSeed(const CScriptThing&, unsigned long&) const {abort();}
virtual void EntitySetAsForSale(const CScriptThing&, bool) const {abort();}
virtual void EntitySetStockItemPrice(const CScriptThing&, long) const {abort();}
virtual long EntityGetStockItemPrice(const CScriptThing&) const {abort();}
virtual void EntityPlayObjectAnimation(const CScriptThing&, const CCharString&, bool) const {abort();}
virtual void EntitySetMaxRunningSpeed(const CScriptThing&, float) const {abort();}
virtual void EntitySetMaxWalkingSpeed(const CScriptThing&, float) const {abort();}
virtual void EntityResetMaxRunningSpeed(const CScriptThing&) const {abort();}
virtual void EntityResetMaxWalkingSpeed(const CScriptThing&) const {abort();}
virtual void EntityAttachToVillage(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void EntitySetAsSittingOnFloor(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAsHavingBoundHands(CScriptThing&, bool) const {abort();}
virtual void EntitySetAsRemoveAllMovementBlockingModes(CScriptThing&) const {abort();}
virtual void EntitySetAsScared(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAsDrunk(CScriptThing&, bool) const {abort();}
virtual void EntityForceToLookAtThing(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void EntityForceToLookAtCamera(const CScriptThing&, bool) const {abort();}
virtual void EntityForceToLookAtNothing(const CScriptThing&) const {abort();}
virtual void EntityResetForceToLookAt(const CScriptThing&) const {abort();}
virtual void EntitySetShotAccuracyPercentage(const CScriptThing&, long) const {abort();}
virtual CScriptThing EntityGetStandingOnThing(const CScriptThing&) const {abort();}
virtual CScriptThing EntityGetStandingInsideBuilding(const CScriptThing&) const {abort();}
virtual void EntityDropGenericBox(const CScriptThing&) const {abort();}
virtual void EntitySheatheWeapons(const CScriptThing&, bool) const {abort();}
virtual void EntityUnsheatheWeapons(const CScriptThing&, bool) const {abort();}
virtual void EntityUnsheatheMeleeWeapon(const CScriptThing&, bool) const {abort();}
virtual void EntityUnsheatheRangedWeapon(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAlpha(const CScriptThing&, float, bool) const {abort();}
virtual void EntitySetAsDrawable(const CScriptThing&, bool) const {abort();}
virtual void EntitySetCutsceneBehaviour(const CScriptThing&, ECutsceneBehaviour) const {abort();}
virtual ESex EntityGetSex(const CScriptThing&) const {abort();}
virtual void EntitySetAsAbleToWalkThroughSolidObjects(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAsRespondToHit(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAsDamageable(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAsKillable(const CScriptThing&, bool, bool) const {abort();}
virtual void EntitySetAsLocked(const CScriptThing&, bool) const {abort();}
virtual void EntityDecapitate(const CScriptThing&) const {abort();}
virtual void EntityGiveGold(const CScriptThing&, long) const {abort();}
virtual void EntitySetAllowBossPhaseChanges(const CScriptThing&, bool) const {abort();}
virtual long EntityGetBossPhase(const CScriptThing&) const {abort();}
virtual void EntitySetBossPhase(const CScriptThing&, long) const {abort();}
virtual void EntityResetCreatureMode(const CScriptThing&) const {abort();}
virtual bool EntitySetAsReceivingEvents(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAsToAddToStatChangesWhenHit(const CScriptThing&, bool) const {abort();}
virtual void EntityLeaveCombatStance(const CScriptThing&) const {abort();}
virtual void EntitySetAsUseMovementInActions(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAsDisplayingEmoteIcon(const CScriptThing&, bool) const {abort();}
virtual void EntitySetAsCollidableToThings(const CScriptThing&, bool) const {abort();}
virtual void EntityEnableGravity(const CScriptThing&, bool) const {abort();}
virtual void EntitySetLightAsOn(const CScriptThing&, bool) const {abort();}
virtual void EntityFadeOut(const CScriptThing&, float) const {abort();}
virtual void EntityFadeIn(const CScriptThing&, float) const {abort();}
virtual void EntityBeginLoadingAnimation(const CScriptThing&, const CCharString&) const {abort();}
virtual void EntityBeginLoadingBasicAnimations(const CScriptThing&) const {abort();}
virtual bool EntityCastForcePush(const CScriptThing&, bool) const {abort();}
virtual void EntityCastLightningAtTarget(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void BeginLoadingMesh(const CCharString&) const {abort();}
virtual bool EntityWillTeleportToArea(const CScriptThing&, C3DVector, float, float) const {abort();}
virtual void EntityStartScreamerSuperAttackThing(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void EntityEndScreamerSuperAttackThing(const CScriptThing&) const {abort();}
virtual void SetHeroGuideToShowQuestCardsWhenSpokenTo(bool) {abort();}
virtual void SetLightColour(const CScriptThing&, const CRGBColour&) const {abort();}
virtual void CreatureGeneratorSetFamily(const CScriptThing&, const CCharString&) const {abort();}
virtual void CreatureGeneratorTrigger(const CScriptThing&) const {abort();}
virtual void CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(const CScriptThing&, bool) const {abort();}
virtual bool CreatureGeneratorIsDepleted(const CScriptThing&) const {abort();}
virtual bool CreatureGeneratorIsDestroyed(const CScriptThing&) const {abort();}
virtual void CreatureGeneratorSetGeneratedCreatureScriptName(const CScriptThing&, const CCharString&) const {abort();}
virtual void CreatureGeneratorSetNumTriggers(const CScriptThing&, long) const {abort();}
virtual long CreatureGeneratorGetNumGeneratedCreatures(const CScriptThing&) const {abort();}
virtual bool CreatureGeneratorAreAllCreaturesAlive(const CScriptThing&) const {abort();}
virtual void CreatureGeneratorAddTriggerer(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void CreatureGeneratorRemoveTriggerer(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void SetCreatureGeneratorEnabled(const CScriptThing&, bool) const {abort();}
virtual void SetCreatureGeneratorsEnabled(const CCharString&, bool) const {abort();}
virtual void SetCreatureGeneratorsEnabledDuringScript(const CCharString&, bool) const {abort();}
virtual void SetCreatureGeneratorsCreatureGroupAsEnabled(ECreatureGroup, bool) const {abort();}
virtual bool IsCreatureGenerationEnabledForRegion(const CCharString&) const {abort();}
virtual bool IsCreatureFlying(const CScriptThing&) const {abort();}
virtual void SetTeleporterAsActive(const CScriptThing&, bool) const {abort();}
virtual bool IsTeleporterActive(const CScriptThing&) const {abort();}
virtual void SetTeleportingAsActive(bool) const {abort();}
virtual bool IsTeleportingActive() const {abort();}
virtual void SetRegionExitAsActive(const CScriptThing&, bool) const {abort();}
virtual void SetRegionEntranceAsActive(const CScriptThing&, bool) const {abort();}
virtual void SetRegionTextDisplayAsActive(bool) const {abort();}
virtual void SetHeroSleepingAsEnabled(bool) const {abort();}
virtual bool IsHeroSleepingEnabled() const {abort();}
virtual void SetExperienceSpendingAsEnabled(bool) const {abort();}
virtual void SetMoralityChangingAsEnabled(bool) const {abort();}
virtual void SetSummonerDeathExplosionAffectsHero(bool) const {abort();}
virtual CScriptThing GetNearestEnabledDiggingSpot(const CScriptThing&) const {abort();}
virtual bool IsDiggingSpotEnabled(const CScriptThing&) const {abort();}
virtual bool IsDiggingSpotHidden(const CScriptThing&) const {abort();}
virtual void SetDiggingSpotAsHidden(const CScriptThing&, bool) const {abort();}
virtual bool CheckForCameraMessage(const CCharString&) const {abort();}
virtual void WaitForCameraMessage(const CCharString&) const {abort();}
virtual void SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const {abort();}
virtual void SetFireToThing(const CScriptThing&) const {abort();}
virtual void ExtinguishFiresOnThing(const CScriptThing&) const {abort();}
virtual bool IsThingOnFire(const CScriptThing&) const {abort();}
virtual void AddItemToContainer(const CScriptThing&, const CCharString&) const {abort();}
virtual void RemoveItemFromContainer(const CScriptThing&, const CCharString&) const {abort();}
virtual void EntitySetDeathContainerAsEnabled(const CScriptThing&, bool) const {abort();}
virtual void GetItemDefNamesFromContainer(const CScriptThing&, std::vector<CCharString,std::allocator<CCharString> >&) const {abort();}
virtual void SetCreatureBrain(const CScriptThing&, const CCharString&) const {abort();}
virtual void EntitySetStategroupEnabled(const CScriptThing&, const CCharString&, bool) const {abort();}
virtual void EntitySetAllStategroupsEnabled(const CScriptThing&, bool) const {abort();}
virtual void EntitySetCombatEnabled(const CScriptThing&, bool) const {abort();}
virtual void EntitySetSleepEnabled(const CScriptThing&, bool) const {abort();}
virtual void EntitySetOpinionReactionsEnabled(const CScriptThing&, bool) const {abort();}
virtual void EntitySetDeedReactionsEnabled(const CScriptThing&, bool) const {abort();}
virtual void DebugGetAllTextEntriesForTargetedThing(std::set<unsigned long,std::less<unsigned long>,std::allocator<unsigned long> >&) const {abort();}
virtual void EntitySetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void EntityUnsetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void EntitySetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void EntityUnsetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void EntitySetInFaction(const CScriptThing&, const CCharString&) const {abort();}
virtual void SetFactionAsAlliedToFaction(const CCharString&, const CCharString&) const {abort();}
virtual void SetFactionAsNeutralToFaction(const CCharString&, const CCharString&) const {abort();}
virtual void SetFactionAsEnemyToFaction(const CCharString&, const CCharString&) const {abort();}
virtual bool AreEntitiesEnemies(const CScriptThing&, const CScriptThing&) const {abort();}
virtual EOpinionAttitudeType GetNextInOpinionAttitudeGraph(EOpinionAttitudeType) const {abort();}
virtual void GetOpinionAttitudeAsString(EOpinionAttitudeType, CCharString&) const {abort();}
virtual EOpinionAttitudeType EntityGetOpinionAttitudeToPlayer(const CScriptThing&) const {abort();}
virtual void EntityGetOpinionAttitudeToPlayerAsString(const CScriptThing&, CCharString&) const {abort();}
virtual float EntityGetOpinionOfPlayer(const CScriptThing&, EOpinion) const {abort();}
virtual void EntitySetOpinionReactionMask(const CScriptThing&, long) const {abort();}
virtual void EntitySetOpinionReactionMask(const CScriptThing&, const CCharString&) const {abort();}
virtual void EntitySetOpinionDeedMask(const CScriptThing&, long) const {abort();}
virtual void EntitySetOpinionDeedMask(const CScriptThing&, const CCharString&) const {abort();}
virtual void EntitySetOpinionDeedTypeEnabled(const CScriptThing&, EOpinionDeedType, bool) const {abort();}
virtual void EntitySetOpinionAttitudeEnabled(const CScriptThing&, EOpinionAttitudeType, bool) const {abort();}
virtual void EntitySetOpinionReactionEnabled(const CScriptThing&, EOpinionReactionType, bool) const {abort();}
virtual void EntitySetPersonalityOverride(const CScriptThing&, const CCharString&) const {abort();}
virtual void EntitySetPersonalityOverride(const CScriptThing&, long) const {abort();}
virtual void EntityClearPersonalityOverride(const CScriptThing&) const {abort();}
virtual void EntitySetAsOpinionSource(const CScriptThing&, const CCharString&) const {abort();}
virtual void EntitySetAsOpinionSource(const CScriptThing&, long) const {abort();}
virtual void EntityUnsetAsOpinionSource(const CScriptThing&, bool) const {abort();}
virtual void OpinionSourceSetAsExclusive(const CScriptThing&, bool) const {abort();}
virtual void OpinionSourceSetAsAttentionGrabbing(const CScriptThing&, bool) const {abort();}
virtual void EntityPostOpinionDeedToAll(const CScriptThing&, EOpinionDeedType) const {abort();}
virtual void EntityPostOpinionDeedToRecipient(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const {abort();}
virtual void EntityPostOpinionDeedToRecipientVillage(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const {abort();}
virtual long EntityPostOpinionDeedKeepSearchingForWitnesses(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const {abort();}
virtual void RemoveOpinionDeedStillSearchingForWitnesses(const CScriptThing&, long) const {abort();}
virtual bool IsDeedWitnessed(long) const {abort();}
virtual bool CanThingBe_Seen_ByOtherThing(const CScriptThing&, const CScriptThing&) const {abort();}
virtual bool CanThingBe_NearlySeen_ByOtherThing(const CScriptThing&, const CScriptThing&) const {abort();}
virtual bool CanThingBe_Smelled_ByOtherThing(const CScriptThing&, const CScriptThing&) const {abort();}
virtual bool CanThingBe_Heard_ByOtherThing(const CScriptThing&, const CScriptThing&) const {abort();}
virtual bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void EntitySetAsAwareOfThing(const CScriptThing&, const CScriptThing&) const {abort();}
virtual void EntitySetSoundRadius(const CScriptThing&, float) const {abort();}
virtual void EntitySetSmellRadius(const CScriptThing&, float) const {abort();}
virtual void EntitySetSightRadius(const CScriptThing&, float) const {abort();}
virtual void EntitySetExtendedSightRadius(const CScriptThing&, float) const {abort();}
virtual void EntitySetGiveUpChaseRadius(const CScriptThing&, float) const {abort();}
virtual float EntityGetHearingRadius(const CScriptThing&) const {abort();}
virtual bool ManuallyTriggerTrap(const CScriptThing&) const {abort();}
virtual bool ManuallyResetTrap(const CScriptThing&) const {abort();}
virtual void SetTimeOfDay(float) const {abort();}
virtual long GetTimeOfDay() const {abort();}
virtual void SetTimeAsStopped(bool, long&) const {abort();}
virtual void FastForwardTimeTo(float, float) const {abort();}
virtual bool IsTimeOfDayBetween(long, long) const {abort();}
virtual long GetDayOfWeek() const {abort();}
virtual long GetDayCount() const {abort();}
virtual long GetWorldFrame() const {abort();}
virtual long GetConstantFPS() const {abort();}
virtual CCharString GetActiveQuestName() const {abort();}
virtual void TransitionToTheme(const CCharString&, float) const {abort();}
virtual void ResetToDefaultTheme(float) const {abort();}
virtual void TransitionToThemeAllInternals(const CCharString&, float) const {abort();}
virtual void ResetToDefaultThemeAllInternals(float) const {abort();}
virtual void TransitionToThemeExternals(const CCharString&, float) const {abort();}
virtual void ResetToDefaultThemeExternals(float) const {abort();}
virtual void SetEnvironmentThemeWeightAllChannels(const CCharString&, float) const {abort();}
virtual void SetEnvironmentThemeWeightAllInternals(const CCharString&, float) const {abort();}
virtual void SetEnvironmentThemeWeightExternals(const CCharString&, float) const {abort();}
virtual void SetSoundThemesAsEnabledForRegion(const CCharString&, bool) const {abort();}
virtual void SetAllSoundsAsMuted(bool) const {abort();}
virtual void* RadialBlurFadeTo(float, C3DVector, float, C3DVector, float, float, float) const {abort();}
virtual void* RadialBlurFadeTo(float, float, float, float, float, float, float) const {abort();}
virtual void RadialBlurFadeOut(float, void*) const {abort();}
virtual bool IsRadialBlurFadeActive() const {abort();}
virtual void CancelRadialBlurFade() const {abort();}
virtual void RadialBlurSetCenterWorldPos(void*, const C3DVector&) const {abort();}
virtual void* DisplacementMonochromeEffectColourFadeTo(float, const CRGBFloatColour&) const {abort();}
virtual void DisplacementMonochromeEffectColourFadeOut(float, void*) const {abort();}
virtual void* ScreenFilterFadeTo(float, float, float, float, float, const CRGBFloatColour&, const std::vector<CScreenFilterSThingByPass,std::allocator<CScreenFilterSThingByPass> >&) const {abort();}
virtual void ScreenFilterFadeOut(float, void*) const {abort();}
virtual void SetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&, void*) const {abort();}
virtual void UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&) const {abort();}
virtual bool IsGiftRomantic(const CCharString&) const {abort();}
virtual bool IsGiftFriendly(const CCharString&) const {abort();}
virtual bool IsGiftOffensive(const CCharString&) const {abort();}
virtual bool IsThingABed(const CScriptThing&) const {abort();}
virtual bool IsThingAChest(const CScriptThing&) const {abort();}
virtual bool IsThingADoor(const CScriptThing&) const {abort();}
virtual bool IsThingSmashable(const CScriptThing&) const {abort();}
virtual bool IsThingSearchable(const CScriptThing&) const {abort();}
virtual void ApplyScriptBrush(const CCharString&) const {abort();}
virtual void EnableDecals(bool) const {abort();}
virtual unsigned long PlayCriteriaSoundOnThing(const CScriptThing&, const CCharString&) const {abort();}
virtual unsigned long PlaySoundOnThing(const CScriptThing&, const CCharString&) const {abort();}
virtual unsigned long PlaySoundAtPos(const C3DVector&, const CCharString&) const {abort();}
virtual unsigned long Play2DSound(const CCharString&) const {abort();}
virtual bool IsSoundPlaying(unsigned long) const {abort();}
virtual void EnableSounds(bool) const {abort();}
virtual void StopSound(unsigned long) const {abort();}
virtual void OverrideMusic(EMusicSetType, bool, bool) const {abort();}
virtual void StopOverrideMusic(bool) const {abort();}
virtual void CacheMusicSet(EMusicSetType) const {abort();}
virtual void EnableDangerMusic(bool) const {abort();}
virtual bool IsDangerMusicEnabled() const {abort();}
virtual void StartCountdownTimer(float) const {abort();}
virtual float GetCountdownTimer() const {abort();}
virtual void AutoSaveCheckPoint() const {abort();}
virtual void AutoSaveQuestStart() const {abort();}
virtual void AutoSave() const {abort();}
virtual void SetSavingAsEnabled(bool) const {abort();}
virtual bool IsSavingEnabled() const {abort();}
virtual void SetSaveGameMarkerPos(const C3DVector&) const {abort();}
virtual void ResetToFrontEnd() const {abort();}
virtual void SetGuildSealRecallLocation(const C3DVector&, float) const {abort();}
virtual C3DVector GetGuildSealRecallPos() const {abort();}
virtual float GetGuildSealRecallAngleXY() const {abort();}
virtual void SetReadableObjectText(const CScriptThing&, const CWideString&) const {abort();}
virtual void SetReadableObjectTextTag(const CScriptThing&, const CCharString&) const {abort();}
virtual CWideString GetFormattedString(const CCharString&, const std::vector<CWideString,std::allocator<CWideString> >&) const {abort();}
virtual CWideString GetTextString(const CCharString&) const {abort();}
virtual void AddRumourCategory(const CCharString&) const {abort();}
virtual void AddNewRumourToCategory(const CCharString&, const CCharString&) const {abort();}
virtual void RemoveRumourCategory(const CCharString&) const {abort();}
virtual void SetCategoryActivity(const CCharString&, bool) const {abort();}
virtual void AddGossipVillage(const CCharString&, const CCharString&) const {abort();}
virtual void AddGossipFactionToCategory(const CCharString&, const CCharString&) const {abort();}
virtual void SetIsGossipForPlayer(const CCharString&, bool) const {abort();}
virtual void SetIsGossipForPlayer(CCharString, bool) {abort();}
virtual void UpdateOnlineScore_Archery(float) const {abort();}
virtual void UpdateOnlineScore_ChickenKick(float) const {abort();}
virtual void UpdateOnlineScore_ChapelOrTemple(float) const {abort();}
virtual void UpdateOnlineScore_FishingCompo(float) const {abort();}
virtual void UpdateScore_FishingCompetition(float) const {abort();}
virtual float GetBestTimePairs() const {abort();}
virtual float GetBestTimeSorting() const {abort();}
virtual long GetBestScoreBlackjack() const {abort();}
virtual long GetBestScoreCoinGolfOakVale() const {abort();}
virtual long GetBestScoreCoinGolfSnowSpire() const {abort();}
virtual long GetBestScoreShoveHaPenny() const {abort();}
virtual float GetBestTimeGuessTheAddition() const {abort();}
virtual bool IsHeroInTavernGame() const {abort();}
virtual long GetNumHousesOwned() const {abort();}
virtual void StartSneaking() const {abort();}
virtual long GetStealDuration(const CScriptThing&) const {abort();}
virtual void SetUseableByHero(const CScriptThing&, bool) const {abort();}
virtual void SetOwnedByHero(const CScriptThing&, bool) const {abort();}
virtual void SetTavernTableAvailableForUse(const CScriptThing&, bool) const {abort();}
virtual void SetIsThingTurncoatable(const CScriptThing&, bool) const {abort();}
virtual void SetIsThingForcePushable(const CScriptThing&, bool) const {abort();}
virtual void SetIsThingLightningable(const CScriptThing&, bool) const {abort();}
virtual void SetIsThingEpicSpellable(const CScriptThing&, bool) const {abort();}
virtual bool IsThingTurncoated(const CScriptThing&) const {abort();}
virtual void AddCreatureScriptedMode(const CScriptThing&, const CCharString&) const {abort();}
virtual void RemoveCreatureScriptedMode(const CScriptThing&) const {abort();}
virtual void ForceShipsVisible() const {abort();}
virtual bool GetSleepingPositionAndOrientationFromBed(const CScriptThing&, const CScriptThing&, C3DVector&, C3DVector&) {abort();}
virtual void SetBedAvailability(const CScriptThing&, bool) {abort();}
virtual void RepopulateVillage(const CScriptThing&) const {abort();}
virtual void SmashAllWindowsWithinRadiusOfPoint(const C3DVector&, float) {abort();}
virtual CScriptThing SetResidency(const CScriptThing&, bool) {abort();}
virtual void SetThankingPhrase(CScriptThing, unsigned long) {abort();}
virtual unsigned long GetThankingPhrase(CScriptThing) {abort();}
virtual void ResetThankingPhrase(CScriptThing) {abort();}
virtual void SetIgnoringPhrase(CScriptThing, unsigned long) {abort();}
virtual unsigned long GetIgnoringPhrase(CScriptThing) {abort();}
virtual void ResetIgnoringPhrase(CScriptThing) {abort();}
virtual void SetWanderCentrePoint(CScriptThing, C3DVector) {abort();}
virtual C3DVector GetWanderCentrePoint(CScriptThing) {abort();}
virtual void ResetWanderCentrePoint(CScriptThing) {abort();}
virtual void SetWanderMinDistance(CScriptThing, float) {abort();}
virtual float GetWanderMinDistance(CScriptThing) {abort();}
virtual void ResetWanderMinDistance(CScriptThing) {abort();}
virtual void SetWanderMaxDistance(CScriptThing, float) {abort();}
virtual float GetWanderMaxDistance(CScriptThing) {abort();}
virtual void ResetWanderMaxDistance(CScriptThing) {abort();}
virtual void SetGossipCounter(CScriptThing, long) {abort();}
virtual long GetGossipCounter(CScriptThing) {abort();}
virtual void ResetGossipCounter(CScriptThing) {abort();}
virtual void SetMaxGossipPhrase(CScriptThing, long) {abort();}
virtual long GetMaxGossipPhrase(CScriptThing) {abort();}
virtual void ResetMaxGossipPhrase(CScriptThing) {abort();}
virtual void SetWarningPhrase(CScriptThing, unsigned long) {abort();}
virtual unsigned long GetWarningPhrase(CScriptThing) {abort();}
virtual void ResetWarningPhrase(CScriptThing) {abort();}
virtual void SetBeerRequestPhrase(CScriptThing, unsigned long) {abort();}
virtual unsigned long GetBeerRequestPhrase(CScriptThing) {abort();}
virtual void ResetBeerRequestPhrase(CScriptThing) {abort();}
virtual void SetScriptingStateGroup(CScriptThing, EScriptingStateGroups) {abort();}
virtual EScriptingStateGroups GetScriptingStateGroup(CScriptThing) {abort();}
virtual void ResetScriptingStateGroup(CScriptThing) {abort();}
virtual void SetMaxHeroReactionDistance(CScriptThing, float) {abort();}
virtual float GetMaxHeroReactionDistance(CScriptThing) {abort();}
virtual void ResetMaxHeroReactionDistance(CScriptThing) {abort();}
virtual void SetActionFrequency(CScriptThing, long) {abort();}
virtual long GetActionFrequency(CScriptThing) {abort();}
virtual void ResetActionFrequency(CScriptThing) {abort();}
virtual void SetActionFrequencyVariation(CScriptThing, float) {abort();}
virtual float GetActionFrequencyVariation(CScriptThing) {abort();}
virtual void ResetActionFrequencyVariation(CScriptThing) {abort();}
virtual void SetAction(CScriptThing, CCharString) {abort();}
virtual CCharString GetAction(CScriptThing) {abort();}
virtual void ResetAction(CScriptThing) {abort();}
virtual void SetFaceHeroForAction(CScriptThing, bool) {abort();}
virtual bool GetFaceHeroForAction(CScriptThing) {abort();}
virtual void ResetFaceHeroForAction(CScriptThing) {abort();}
virtual void SetTargetName(CScriptThing, CCharString) {abort();}
virtual CCharString GetTargetName(CScriptThing) {abort();}
virtual void ResetTargetName(CScriptThing) {abort();}
virtual void SetFollowDistance(CScriptThing, float) {abort();}
virtual float GetFollowDistance(CScriptThing) {abort();}
virtual void ResetFollowDistance(CScriptThing) {abort();}
virtual void SetAttackHeroOnSight(CScriptThing, bool) {abort();}
virtual bool GetAttackHeroOnSight(CScriptThing) {abort();}
virtual void ResetAttackHeroOnSight(CScriptThing) {abort();}
virtual void SetTimeToSpendHarassingHero(CScriptThing, long) {abort();}
virtual long GetTimeToSpendHarassingHero(CScriptThing) {abort();}
virtual void ResetTimeToSpendHarassingHero(CScriptThing) {abort();}
virtual void SetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing, float) {abort();}
virtual float GetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) {abort();}
virtual void ResetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) {abort();}
virtual void SetCombatNearbyBreakOffRange(CScriptThing, float) {abort();}
virtual float GetCombatNearbyBreakOffRange(CScriptThing) {abort();}
virtual void ResetCombatNearbyBreakOffRange(CScriptThing) {abort();}
virtual void SetStealStealableItems(CScriptThing, bool) {abort();}
virtual bool GetStealStealableItems(CScriptThing) {abort();}
virtual void ResetStealStealableItems(CScriptThing) {abort();}
virtual void SetRecoverStealableItems(CScriptThing, bool) {abort();}
virtual bool GetRecoverStealableItems(CScriptThing) {abort();}
virtual void ResetRecoverStealableItems(CScriptThing) {abort();}
virtual void SetTakeStealableItemToRandomDestination(CScriptThing, bool) {abort();}
virtual bool GetTakeStealableItemToRandomDestination(CScriptThing) {abort();}
virtual void ResetTakeStealableItemToRandomDestination(CScriptThing) {abort();}
virtual void SetKillSelfAndStealableItemAfterReachingDestination(CScriptThing, bool) {abort();}
virtual bool GetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) {abort();}
virtual void ResetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) {abort();}
virtual void SetAllowedToFollow(CScriptThing, bool) {abort();}
virtual bool GetAllowedToFollow(CScriptThing) {abort();}
virtual void ResetAllowedToFollow(CScriptThing) {abort();}
virtual void SetTableName(CScriptThing, CCharString) {abort();}
virtual CCharString GetTableName(CScriptThing) {abort();}
virtual void ResetTableName(CScriptThing) {abort();}
virtual void SetSeatName(CScriptThing, CCharString) {abort();}
virtual CCharString GetSeatName(CScriptThing) {abort();}
virtual void ResetSeatName(CScriptThing) {abort();}
virtual void SetDisableHeadLooking(CScriptThing, bool) {abort();}
virtual bool GetDisableHeadLooking(CScriptThing) {abort();}
virtual void ResetDisableHeadLooking(CScriptThing) {abort();}
virtual void SetIsPushableByHero(CScriptThing, bool) {abort();}
virtual bool GetIsPushableByHero(CScriptThing) {abort();}
virtual void ResetIsPushableByHero(CScriptThing) {abort();}
virtual void SetLookForFiniteTime(CScriptThing, bool) {abort();}
virtual bool GetLookForFiniteTime(CScriptThing) {abort();}
virtual void ResetLookForFiniteTime(CScriptThing) {abort();}
virtual void SetAvoidRegionExits(CScriptThing, bool) {abort();}
virtual bool GetAvoidRegionExits(CScriptThing) {abort();}
virtual void ResetAvoidRegionExits(CScriptThing) {abort();}
virtual void SetTargetingDistanceOffset(CScriptThing, float) {abort();}
virtual float GetTargetingDistanceOffset(CScriptThing) {abort();}
virtual void ResetTargetingDistanceOffset(CScriptThing) {abort();}
virtual void SetPlayerUsingMeleeDummies(bool) {abort();}
virtual bool GetPlayerUsingMeleeDummies() {abort();}
virtual void SetPlayerUsingRangedDummies(bool) {abort();}
virtual bool GetPlayerUsingRangedDummies() {abort();}
virtual void SetPlayerUsingWillDummies(bool) {abort();}
virtual bool GetPlayerUsingWillDummies() {abort();}
virtual void SetCheapHeadLooking(bool) {abort();}
virtual bool GetCheapHeadLooking() {abort();}
virtual void SetQuitTavernGame(bool) {abort();}
virtual bool GetQuitTavernGame() {abort();}
virtual void SetPrizeTavernTable(bool) {abort();}
virtual bool GetPrizeTavernTable() {abort();}
virtual void SetBettingActive(bool) {abort();}
virtual bool GetBettingActive() {abort();}
virtual void SetBettingAccept(bool) {abort();}
virtual bool GetBettingAccept() {abort();}
virtual void SetBettingAmount(long) {abort();}
virtual long GetBettingAmount() {abort();}
virtual void SetCountBetMoneyDown(bool) {abort();}
virtual bool GetCountBetMoneyDown() {abort();}
virtual void SetSpotTheAdditionBeaten(bool) {abort();}
virtual bool GetSpotTheAdditionBeaten() {abort();}
virtual void SetGlobalTargetingDistanceOffset(float) {abort();}
virtual float GetGlobalTargetingDistanceOffset() {abort();}
virtual void SetTradingPriceMult(float) {abort();}
virtual float GetTradingPriceMult() {abort();}
virtual void SetBoastingEnabled(bool) {abort();}
virtual bool GetBoastingEnabled() {abort();}
virtual void SetActiveGossipCategories(CCharString, bool) {abort();}
virtual bool* GetActiveGossipCategories(CCharString) {abort();}
virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetActiveGossipCategories() {abort();}
virtual int GetActiveGossipCategoriesSize() {abort();}
virtual void ClearActiveGossipCategories() {abort();}
virtual bool* GetIsGossipForPlayer(CCharString) {abort();}
virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetIsGossipForPlayer() {abort();}
virtual int GetIsGossipForPlayerSize() {abort();}
virtual void ClearIsGossipForPlayer() {abort();}
virtual void SetGossip(CCharString, CCharString, long) {abort();}
virtual CCharString GetGossip(CCharString, long) {abort();}
virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossip(CCharString) {abort();}
virtual int GetGossipSize(CCharString) {abort();}
virtual void ClearGossip(CCharString) {abort();}
virtual void RemoveGossip(CCharString) {abort();}
virtual void AddGossip(CCharString, CCharString) {abort();}
virtual void AddGossip(CCharString) {abort();}
virtual void SetGossipVillages(CCharString, CCharString, long) {abort();}
virtual CCharString GetGossipVillages(CCharString, long) {abort();}
virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipVillages(CCharString) {abort();}
virtual int GetGossipVillagesSize(CCharString) {abort();}
virtual void ClearGossipVillages(CCharString) {abort();}
virtual void RemoveGossipVillages(CCharString) {abort();}
virtual void AddGossipVillages(CCharString, CCharString) {abort();}
virtual void AddGossipVillages(CCharString) {abort();}
virtual void SetGossipFactions(CCharString, CCharString, long) {abort();}
virtual CCharString GetGossipFactions(CCharString, long) {abort();}
virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipFactions(CCharString) {abort();}
virtual int GetGossipFactionsSize(CCharString) {abort();}
virtual void ClearGossipFactions(CCharString) {abort();}
virtual void RemoveGossipFactions(CCharString) {abort();}
virtual void AddGossipFactions(CCharString, CCharString) {abort();}
virtual void AddGossipFactions(CCharString) {abort();}
};
union DefStorage{double alignment;unsigned char bytes[sizeof(CScriptDef)];};
int main(){TestGame game;pDSTGame=&game;DefStorage storage;memset(&storage,0,sizeof(storage));CScriptDef*def=reinterpret_cast<CScriptDef*>(storage.bytes);pGlobalScriptDef.Object=reinterpret_cast<CDefPointeeBase*>(def);int cases=0,total_messages=0,total_timers=0;
 for(int cut=0;cut<41;++cut)for(int delay=0;delay<3;++delay)for(int activation=0;activation<4;++activation)for(int busy=0;busy<2;++busy)for(int clear=0;clear<2;++clear){
 NScript::CGameflowScript script(game);active=&script;query=frames=registered=deregistered=messages=string_live=hero_queries=0;live=false;limit=cut;initial_ticks=delay;activate_after=activation;on_quest=busy!=0;clear_during_timer=clear!=0;script.CoreQuestWaiting=activation==0;def->CoreQuestReminderIntervalSeconds=delay;
 script.CoreQuestReminder();
require(query==cut+1);
require(!live&&registered==deregistered&&string_live==0);require(messages<=registered);
if(busy||(clear&&delay>0))require(messages==0);if(cut==0)require(registered==0&&frames==0&&messages==0);
if(cut==2&&activation==0&&delay==0&&!busy&&!clear)require(registered==1&&messages==0);total_messages+=messages;total_timers+=registered;++cases;
 }
 pGlobalScriptDef.Object=0;require(cases==1968&&total_messages>0&&total_timers>0);puts("00cef3b0_TEST PASS");return 0;
}