#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
#define private public
#define protected public
#define CHECK(x) do{if(!(x)){fprintf(stderr,"CHECK line %d\n",__LINE__);abort();}}while(0)
static int charLive,wideLive,createCalls,findCalls,mainReads,frontReads,fallbacks,constructs,sets,initialises,changes,joyGets,joyReads;
static bool frontPresent,mainHit,frontHit;static long joystickNumber;static int seenMarker;static int order;
// 0041db84 NUISystem::CManager::ShowErrorMessage
// Leaf: ?ShowErrorMessage@CManager@NUISystem@@QAEXW4ERRORMESSAGE_TYPE@@@Z
// Flags: /O1 /Oy-
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
#define CManager CManagerFields
// Begin shared header engine/CManager.h
#ifndef FABLE_ENGINE_CMANAGER_H
#define FABLE_ENGINE_CMANAGER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CManager = 0x100.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CManager = 0xd0.
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

class CCPPointerInfo;
struct CComponent;
class CDefinitionManager;
class CGraphicDataBank;
class CInputManager;
class CMeshDataBank;
class CTCHeroExperience;
class CTCInventory;
class CTCInventoryAbilities;
class CTCInventoryClothing;
class CTCInventoryExperience;
class CTCInventoryMap;
class CTCInventoryQuests;
class CTCInventoryTrade;
class CTCInventoryWeapons;
class CThing;

#pragma pack(push, 1)
struct CManager {
    void*                   __vftable;                       // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char           _pad_0x04[0x4];                  // +0x04
    unsigned char           _pad_0x08[0x4];                  // +0x08
    unsigned char           _pad_0x0c[0x4];                  // +0x0c
    CGraphicDataBank*       PGraphicsBank_Data;              // +0x10 CCountedPointer<CGraphicDataBank>::Data
    CCPPointerInfo*         PGraphicsBank_Info;              // +0x14 CCountedPointer<CGraphicDataBank>::Info
    CMeshDataBank*          PMeshBank_Data;                  // +0x18 CCountedPointer<CMeshDataBank>::Data
    CCPPointerInfo*         PMeshBank_Info;                  // +0x1c CCountedPointer<CMeshDataBank>::Info
    CInputManager*          PInputManager;                   // +0x20
    unsigned char           Keys[0xc];                       // +0x24 map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
    unsigned char           InputType[0x4];                  // +0x30 EInputType
    CTCHeroExperience*      PTCHeroExperience;               // +0x34
    CTCInventoryAbilities*  PAbilitiesInventory;             // +0x38
    CTCInventoryWeapons*    PWeaponsInventory;               // +0x3c
    CTCInventoryMap*        PMapInventory;                   // +0x40
    CTCInventoryQuests*     PQuestInventory;                 // +0x44
    CTCInventoryExperience* PExperienceInventory;            // +0x48
    CTCInventory*           PInventory;                      // +0x4c
    CTCInventoryClothing*   PInventoryClothing;              // +0x50
    CTCInventoryTrade*      PInventoryTrade;                 // +0x54
    CThing*                 PPlayer;                         // +0x58
    unsigned char           ThingActionParams[0x4];          // +0x5c list<long,std::allocator<long>_>
    unsigned char           UIActionParams[0x4];             // +0x60 list<NUISystem::CActionParam_*,std::allocator<NUISystem::CActionParam_*>_>
    CDefinitionManager*     m_pDefinitionManager;            // +0x64
    unsigned long           NumberAugmentations;             // +0x68
    unsigned char           Change[0x4];                     // +0x6c EChange
    unsigned char           Layers[0xc];                     // +0x70 map<long,EEngine2DLayer,std::less<long>,std::allocator<std::pair<long_const_,EEngine2DLayer>_>_>
    long                    MetaLayer;                       // +0x7c
    CComponent*             BastardChild_Data;               // +0x80 CCountedPointer<NUISystem::CComponent>::Data
    CCPPointerInfo*         BastardChild_Info;               // +0x84 CCountedPointer<NUISystem::CComponent>::Info
    unsigned char           BastardChildren[0xc];            // +0x88 map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_>
    float                   RightStickForce_X;               // +0x94 C2DVector::X
    float                   RightStickForce_Y;               // +0x98 C2DVector::Y
    bool                    m_bHasErrorMessage;              // +0x9c
    unsigned char           _pad_0x9d[0x3];                  // +0x9d
    CComponent*             m_pErrorMessage;                 // +0xa0
    bool                    Restoring;                       // +0xa4
    unsigned char           _pad_0xa5[0x3];                  // +0xa5
    double                  m_fLastRenderTime;               // +0xa8
    float                   MouseMovement_X;                 // +0xb0 C2DVector::X
    float                   MouseMovement_Y;                 // +0xb4 C2DVector::Y
    CComponent*             PMouseCursor;                    // +0xb8
    CComponent*             PDropped;                        // +0xbc
    wchar_t                 KeyPressed;                      // +0xc0
    unsigned char           _pad_0xc2[0x2];                  // +0xc2
    float                   DescriptionScrollingOffset;      // +0xc4
    bool                    DescriptionScrollingToMax;       // +0xc8
    bool                    MouseDisabled;                   // +0xc9
    bool                    AssignmentItemsExpressionsSwapping; // +0xca
    bool                    AssignmentSpellsSwapping;        // +0xcb
    unsigned char           _pad_0xcc[0x4];                  // +0xcc
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CManager) == 0xd0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PGraphicsBank_Data) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PGraphicsBank_Info) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PMeshBank_Data) == 0x18);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PMeshBank_Info) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PInputManager) == 0x20);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, Keys) == 0x24);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, InputType) == 0x30);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PTCHeroExperience) == 0x34);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PAbilitiesInventory) == 0x38);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PWeaponsInventory) == 0x3c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PMapInventory) == 0x40);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PQuestInventory) == 0x44);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PExperienceInventory) == 0x48);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PInventory) == 0x4c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PInventoryClothing) == 0x50);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PInventoryTrade) == 0x54);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PPlayer) == 0x58);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, ThingActionParams) == 0x5c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, UIActionParams) == 0x60);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, m_pDefinitionManager) == 0x64);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, NumberAugmentations) == 0x68);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, Change) == 0x6c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, Layers) == 0x70);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, MetaLayer) == 0x7c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, BastardChild_Data) == 0x80);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, BastardChild_Info) == 0x84);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, BastardChildren) == 0x88);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, RightStickForce_X) == 0x94);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, RightStickForce_Y) == 0x98);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, m_bHasErrorMessage) == 0x9c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, m_pErrorMessage) == 0xa0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, Restoring) == 0xa4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, m_fLastRenderTime) == 0xa8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, MouseMovement_X) == 0xb0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, MouseMovement_Y) == 0xb4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PMouseCursor) == 0xb8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, PDropped) == 0xbc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, KeyPressed) == 0xc0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, DescriptionScrollingOffset) == 0xc4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, DescriptionScrollingToMax) == 0xc8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, MouseDisabled) == 0xc9);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, AssignmentItemsExpressionsSwapping) == 0xca);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CManager, AssignmentSpellsSwapping) == 0xcb);
#endif
#endif

#endif // FABLE_ENGINE_CMANAGER_H

// End shared header engine/CManager.h
#undef CManager
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
#define CGameJoystickManager CGameJoystickManagerFields
// Begin shared header engine/CGameJoystickManager.h
#ifndef FABLE_ENGINE_CGAMEJOYSTICKMANAGER_H
#define FABLE_ENGINE_CGAMEJOYSTICKMANAGER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CGameJoystickManager = 0xc.
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
class CGameJoystickManager {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    long          MainPlayerJoystickDeviceNumber;            // +0x04
    bool          MainPlayerJoystickDeviceNumberSet;         // +0x08
    unsigned char _pad_0x09[0x3];                            // +0x09
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CGameJoystickManager) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameJoystickManager, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameJoystickManager, MainPlayerJoystickDeviceNumber) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CGameJoystickManager, MainPlayerJoystickDeviceNumberSet) == 0x8);
#endif
#endif

#endif // FABLE_ENGINE_CGAMEJOYSTICKMANAGER_H

// End shared header engine/CGameJoystickManager.h
#undef CGameJoystickManager
class CCharStringData;class CWideStringData;
class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);CCharString(const CCharString&);~CCharString();};
class CWideString{protected:CWideStringData*PStringData;public:CWideString():PStringData(0){}static CWideString NullString;CWideString(const CWideString&);~CWideString();bool operator!=(const CWideString&)const;static CWideString __fastcall CreateFromCharString(const CCharString&);};
namespace NGameText{class CDataBank{public:const CWideString GetTextBySymbol(const CCharString&)const;};bool __fastcall ConstructString(CWideString&,long);}
// Layout: original native game component text-bank field14 and static singleton, confirmed original types.
class CMainGameComponent{protected:static CMainGameComponent*PMainGameComponent;unsigned char unreadMainGame[0x14];CCountedPointer<NGameText::CDataBank>PTextBank;public:static CMainGameComponent*__fastcall Get(){return PMainGameComponent;}const NGameText::CDataBank&GetTextBank()const{return *PTextBank;}};
// Layout: original frontend text-bank field60 and private singleton; native text-bank helper00497b40 loads singleton013b871c then field60.
class CNewFrontendGameComponent{private:static CNewFrontendGameComponent*m_pNewFrontendGameComponent;unsigned char unreadFrontend[0x60];CCountedPointer<NGameText::CDataBank>m_pTextBank;public:static CNewFrontendGameComponent*__fastcall Get(){return m_pNewFrontendGameComponent;}const NGameText::CDataBank&GetTextBank()const{return *m_pTextBank;}};
static __declspec(noinline) CWideString __fastcall GetTextBySymbol(const CCharString&symbol){
 if(CMainGameComponent::Get()){CWideString text=CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(symbol);if(text!=CWideString::NullString)return text;}
 if(CNewFrontendGameComponent::Get()){CWideString text=CNewFrontendGameComponent::Get()->GetTextBank().GetTextBySymbol(symbol);if(text!=CWideString::NullString)return text;}
 return CWideString::CreateFromCharString(symbol);
}
class CGameJoystickManager:public CGameJoystickManagerFields{public:static CGameJoystickManager&__fastcall Get();long GetMainPlayerJoystickDeviceNumber()const;};
class C2DBoxF;class C2DVector;class CEnginePrimitiveHandle;class CIEngine;class CRGBColour;class CFontBank;
template<class T>class CDefPointer;
namespace std{template<class T>class allocator;template<class T>struct less;template<class A,class B>struct pair;template<class T,class A>class list;template<class T,class A>class vector;template<class K,class V,class C,class A>class map;}
namespace NUISystem{class CComponent;class CAction;class CDeletion;class CUIState;class CUIDef;enum ETextAlignement{};enum EType{};
class CUIInterface{
public:virtual ~CUIInterface();
public:
public:
};
class CComponent : public CUIInterface{
public:virtual NUISystem::CComponent* FindChild(CCharString);
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
public:virtual void Initialise();
public:
public:
public:virtual void ChangeState(unsigned long);
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
private:unsigned char unreadComponentMembers[sizeof(CComponentFields)-4];
};
class CChangingStateComponent : public CComponent{
public:
public:
public:
public:
public:
protected:
protected:
protected:
};
class CText : public CChangingStateComponent{
public:
public:
public:virtual void SetValue(const CWideString&,bool,bool);
public:
};
}

enum ERRORMESSAGE_TYPE{};
namespace NUISystem{
// Layout: supplied manager fields9c/a0; global flattened component pointer is projected back to the original namespace at each use.
class CManager:public CManagerFields{protected:using CManagerFields::m_bHasErrorMessage;using CManagerFields::m_pErrorMessage;public:CComponent*CreateComponent(const CCharString&,bool);void ShowErrorMessage(ERRORMESSAGE_TYPE);};
void CManager::ShowErrorMessage(ERRORMESSAGE_TYPE error){
 switch(error){case 0:CText*text;
 if(CNewFrontendGameComponent::Get()){
 m_pErrorMessage=reinterpret_cast< ::CComponent*>(CreateComponent("UI_DIALOG_CONTROLLERDISCONNECT",false));
 text=static_cast<CText*>(reinterpret_cast<CComponent*>(m_pErrorMessage)->FindChild("UI_CONTROLLERDISCONNECT_TEXT"));
 }else{
 m_pErrorMessage=reinterpret_cast< ::CComponent*>(CreateComponent("UI_DIALOG_CONTROLLERDISCONNECT_GAME",false));
 text=static_cast<CText*>(reinterpret_cast<CComponent*>(m_pErrorMessage)->FindChild("UI_CONTROLLERDISCONNECT_TEXT_GAME"));
 }
 CWideString message=GetTextBySymbol("TEXT_GUI_CONTROLLER_DISCONNECT");
 NGameText::ConstructString(message,CGameJoystickManager::Get().GetMainPlayerJoystickDeviceNumber()+1);
 text->SetValue(message,true,true);
 reinterpret_cast<CComponent*>(m_pErrorMessage)->Initialise();
 reinterpret_cast<CComponent*>(m_pErrorMessage)->ChangeState(1UL);
 break;}
 m_bHasErrorMessage=true;
}
}

#undef private
#undef protected
// Test boundary payloads represent string values without relying on string ABI or copy elision.
class CCharStringData{public:const char*Text;explicit CCharStringData(const char*t):Text(t){}};
class CWideStringData{public:int Marker;long Device;CWideStringData(int m,long d):Marker(m),Device(d){}};
CCharString::CCharString(const char*t,long n):PStringData(new CCharStringData(t)){CHECK(n==-1&&t!=0);++charLive;}
CCharString::CCharString(const CCharString&s):PStringData(new CCharStringData(s.PStringData->Text)){++charLive;}
CCharString::~CCharString(){CHECK(PStringData!=0);delete PStringData;--charLive;}
CWideString::CWideString(const CWideString&s):PStringData(s.PStringData?new CWideStringData(s.PStringData->Marker,s.PStringData->Device):0){if(PStringData)++wideLive;}
CWideString::~CWideString(){if(PStringData){delete PStringData;--wideLive;}}
bool CWideString::operator!=(const CWideString&s)const{if(!PStringData||!s.PStringData)return PStringData!=s.PStringData;return PStringData->Marker!=s.PStringData->Marker||PStringData->Device!=s.PStringData->Device;}
CWideString CWideString::NullString;
static CWideString MakeWide(int marker){CWideString s;if(marker){s.PStringData=new CWideStringData(marker,0);++wideLive;}return s;}
CWideString __fastcall CWideString::CreateFromCharString(const CCharString&s){CHECK(strcmp(s.PStringData->Text,"TEXT_GUI_CONTROLLER_DISCONNECT")==0);++fallbacks;return MakeWide(30);}
static NGameText::CDataBank*mainBank;static NGameText::CDataBank*frontBank;
const CWideString NGameText::CDataBank::GetTextBySymbol(const CCharString&s)const{CHECK(strcmp(s.PStringData->Text,"TEXT_GUI_CONTROLLER_DISCONNECT")==0);if(this==mainBank){++mainReads;return MakeWide(mainHit?10:0);}CHECK(this==frontBank);++frontReads;return MakeWide(frontHit?20:0);}
bool __fastcall NGameText::ConstructString(CWideString&s,long n){CHECK(s.PStringData&&n==joystickNumber+1);CHECK(order==2);order=3;s.PStringData->Device=n;++constructs;return true;}
CMainGameComponent*CMainGameComponent::PMainGameComponent=0;
CNewFrontendGameComponent*CNewFrontendGameComponent::m_pNewFrontendGameComponent=0;
static CGameJoystickManager joystick;
CGameJoystickManager&__fastcall CGameJoystickManager::Get(){++joyGets;return joystick;}
long CGameJoystickManager::GetMainPlayerJoystickDeviceNumber()const{CHECK(this==&joystick);++joyReads;return joystickNumber;}
NUISystem::CUIInterface::~CUIInterface(){}
static NUISystem::CComponent root;static NUISystem::CText text;
NUISystem::CComponent*NUISystem::CManager::CreateComponent(const CCharString&s,bool activate){CHECK(!activate&&order==0);order=1;++createCalls;CHECK(strcmp(s.PStringData->Text,frontPresent?"UI_DIALOG_CONTROLLERDISCONNECT":"UI_DIALOG_CONTROLLERDISCONNECT_GAME")==0);return &root;}
NUISystem::CComponent*NUISystem::CComponent::FindChild(CCharString s){CHECK(this==&root&&order==1);order=2;++findCalls;CHECK(strcmp(s.PStringData->Text,frontPresent?"UI_CONTROLLERDISCONNECT_TEXT":"UI_CONTROLLERDISCONNECT_TEXT_GAME")==0);return &text;}
void NUISystem::CText::SetValue(const CWideString&s,bool a,bool b){CHECK(this==&text&&a&&b&&order==3);order=4;++sets;CHECK(s.PStringData&&s.PStringData->Device==joystickNumber+1);seenMarker=s.PStringData->Marker;}
void NUISystem::CComponent::Initialise(){CHECK(this==&root&&order==4);order=5;++initialises;}
void NUISystem::CComponent::ChangeState(unsigned long state){CHECK(this==&root&&state==1&&order==5);order=6;++changes;}
int main(){int cases=0;
 for(int hasMain=0;hasMain<2;++hasMain)for(int hasFront=0;hasFront<2;++hasFront)for(int mh=0;mh<2;++mh)for(int fh=0;fh<2;++fh)for(int joy=-1;joy<3;++joy)for(int kind=-1;kind<3;++kind)for(int prior=0;prior<2;++prior){
  NGameText::CDataBank mainData,frontData;CMainGameComponent mainGame;CNewFrontendGameComponent frontGame;NUISystem::CManager manager;
  mainBank=&mainData;frontBank=&frontData;mainGame.PTextBank.Data=mainBank;frontGame.m_pTextBank.Data=frontBank;
  CMainGameComponent::PMainGameComponent=hasMain?&mainGame:0;CNewFrontendGameComponent::m_pNewFrontendGameComponent=hasFront?&frontGame:0;
  frontPresent=hasFront!=0;mainHit=mh!=0;frontHit=fh!=0;joystickNumber=joy;
  manager.m_bHasErrorMessage=prior!=0;manager.m_pErrorMessage=reinterpret_cast< ::CComponent*>(&text);
  createCalls=findCalls=mainReads=frontReads=fallbacks=constructs=sets=initialises=changes=joyGets=joyReads=order=seenMarker=0;
  CHECK(charLive==0&&wideLive==0);manager.ShowErrorMessage(static_cast<ERRORMESSAGE_TYPE>(kind));CHECK(manager.m_bHasErrorMessage);CHECK(charLive==0&&wideLive==0);
  if(kind==0){CHECK(manager.m_pErrorMessage==reinterpret_cast< ::CComponent*>(&root));CHECK(createCalls==1&&findCalls==1&&constructs==1&&sets==1&&initialises==1&&changes==1&&joyGets==1&&joyReads==1&&order==6);int expected=hasMain&&mh?10:hasFront&&fh?20:30;CHECK(seenMarker==expected);CHECK(mainReads==hasMain);CHECK(frontReads==((!hasMain||!mh)&&hasFront?1:0));CHECK(fallbacks==(expected==30?1:0));}
  else{CHECK(manager.m_pErrorMessage==reinterpret_cast< ::CComponent*>(&text));CHECK(createCalls+findCalls+mainReads+frontReads+fallbacks+constructs+sets+initialises+changes+joyGets+joyReads+order==0);}
  CMainGameComponent::PMainGameComponent=0;CNewFrontendGameComponent::m_pNewFrontendGameComponent=0;++cases;
 }CHECK(cases==512);puts("0041db84_TEST PASS");return 0;}