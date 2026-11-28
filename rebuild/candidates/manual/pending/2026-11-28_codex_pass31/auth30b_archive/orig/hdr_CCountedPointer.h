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
// Private source-call-graph restoration, Pass21: donor Movie02468220/Thing018F0A90 destructors call ReleaseData.
// Runtime sol_donor_countedptr_lifetimes.disasm and sol_donor_countedptr_constructors.disasm retain actual bodies.
// Both original PDB class/public records identify these existing members; no substitute helper is introduced.
// The following retail machine patterns do not distinguish a direct release from an expanded ReleaseData call.
// Retail bytes (no ICF: 732 separate /O2 dtor COMDATs, one per T):
//   ~CCountedPointer = 0x006E7AB0 (MovieBase) / 0x00CE1000 (CEntityScriptBase) and the other 730: `if (Info) { dec [Info];
//     reload Info; cmp [Info],0; jne; ecx = Info->Data; call Info->DeleteFunc; operator delete(Info) }`, then Data = 0
//     and Info = 0 in that order. This private source spells the proven original ReleaseData call.
//   Legacy shared baseline spells ReleaseData Info-first; the selected Data-first opt-in follows original donor02468530.
//   Retail never emits ReleaseData out of line (no
//     release COMDAT with the Info-first tail exists); expanded at 0x00A45330 / 0x00A5F250 / 0x006633A0 (`mov [x+4],edi;
//     mov [x],edi`). Reset and ShareData are written over it (their dead zero stores vanish):
//   Reset(T*) = 0x008AB2B0 (and 197 more): <ReleaseData> leaves only `mov [esi+4],0`, then Data = data; if (data)
//     Info = new CCPPointerInfo(1, DeleteData, Data) (the block reads Data back; a failed new leaves Info 0).
//   ShareData = 0x00CE1040 (CCountedPointer<CEntityScriptBase>: every landed caller binds that T, FableWin has the symbol; the method name is Ego_r.pdb's): `if (Info != info) { <release>; Data = data;
//     Info = info; if (info) ++info->RefCount; }`.
//   operator= = 0x00704580 (CScriptThing): loads o.Data / o.Info first, then the ShareData body, returns *this.
//   The (T*) ctor is the allocation without the release (out of line at 0x008C58F0 / 0x008C5980, `ret 4`).
//   Copy ctor: Data, Info copied, then `if (Info) ++Info->RefCount` (expanded in every CScriptThing copy 0x004ABE90).
//   DeleteData = `delete static_cast<T*>(data)`: 0x00CDEE00 (polymorphic T: `test ecx,ecx; je; mov eax,[ecx]; push 1;
//     call [eax]`), 0x00674E80 (CExpressionSet: the inline implicit dtor, then operator delete). Only its address is
//     stored (the count block); the landed DeleteData rows explicitly instantiate it.
// Legacy baseline model test (wave 9, 955 landed rows, logs/mc_cpvar_*.txt): direct dtor release, Info-first ReleaseData
//   gives 15 DIFFER; a dtor that calls a zeroing ReleaseData gives the same leaf bytes at more sites but names ReleaseData
//   where retail calls the Data-first dtor COMDAT, and `ReleaseData(); Data = 0; Info = 0;` gives 20 DIFFER.
// Inlining: every member is plain `inline` (in-class), never __forceinline / noinline. Retail calls the COMDAT copies at
//   some sites and expands them at others; VC7.1 RTM 3077 reproduces that per-site choice from these bodies on the landed
//   rows (wave 9 HEADERS.md lists the shapes it does not reproduce). unproven: the source order of Reset's two stores and
//   the bodies of GetReferenceCount / reset / Get / get / IsNull (FableWin names only, no retail out-of-line copy found).
#include "rebuild_abi.h"

class CCPPointerInfo {
public:
    unsigned long RefCount;                                              // +0x0
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             // +0x4
    void* Data;                                                          // +0x8
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     // FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z
};
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0xc);

#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22
// All real instantiated member signatures from both PDB23-member records.
// Recorded group/field order is a reconstruction hypothesis, not textual source proof.
// Uninstantiated converting member templates are not inferred absent in original generic source.
template <class T> class CCountedPointer {
public:
 typedef T element_type;
 CCountedPointer(const CCountedPointer& other) : Data(other.Data),Info(other.Info) { if(Info) ++Info->RefCount; }
 CCountedPointer(T* data);
 CCountedPointer() : Data(0),Info(0) { }
 ~CCountedPointer() { ReleaseData(); }
 T& operator*() const { return *Data; }
 T* operator->() const { return Data; }
 CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data,other.Info); return *this; }
 bool IsNull() const { return Data==0; }
 T* Get() const { return Data; }
 void Reset(T* data);
 void Swap(CCountedPointer& other);
 unsigned long GetReferenceCount() const;
 T* get() const { return Data; }
 void reset(T* data);
 void ReleaseData() {
  if(Info) {
   --Info->RefCount;
   if(Info->RefCount==0) { Info->DeleteFunc(Info->Data); delete Info; }
  }
  Data=0;
  Info=0;
 }
 void ShareData(T* data,CCPPointerInfo* info);
 static void FABLE_FASTCALL DeleteData(void* data);
 static void FABLE_FASTCALL DeleteNull(void* data);
 void Validate() const;
 T* Data;
 CCPPointerInfo* Info;
};
#else
template <class T> class CCountedPointer {
public:
    T* Data;                                                             // +0x0
    CCPPointerInfo* Info;                                                // +0x4

    CCountedPointer() : Data(0), Info(0) { }
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR) && !defined(FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR)
    // Chunk 764 (row 0x0096DBC0 CCreatureAction_ScreamerIntoAggressive::Initialise): there retail calls the (T*) constructor's COMDAT
    // (0x00718A00, ??0?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@QAE@PAVCModeBase@CTCCreatureModeManager@@@Z) where VC7.1
    // expands the in-class body; the landed same-module rows (0x008C1A10, 0x008C1900) declare it without a body. Declaration only.
    CCountedPointer(T* data);
#else
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR) && defined(_MSC_VER)
    // Inline-budget (row 0x0041E6D3 NUISystem::CManager::Action): retail CALLS this ctor's same-TU COMDATs at every site
    // (CComponent 0x00429C15 x25, CAction 0x0042AA29 x17, CTexture 0x0041C030 in LOAD_PHOTO; body = the default one below:
    // 0x0041C030 `test eax,eax; mov [esi],eax; je` then `new CCPPointerInfo(1, DeleteData, Data)` reading the stored member).
    // A declaration-only ctor (FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR alone) exposes each object's address above c2's
    // 10000-tuple limit, so it never shares a frame slot (LOAD_PHOTO's texture: ours -3C8h alone, retail -268h shared).
    // A visible body that is not expanded keeps the object tracked. Default path unchanged.
    __declspec(noinline)
#endif
#ifdef FABLE_RETAIL_JACKDRAGON_HOT_CTORS_PASS27
    // Inline-budget: UpdateHover initializes the counted mode pointer in place before AddMode.
    FABLE_FORCEINLINE
#endif
    CCountedPointer(T* data) : Data(data)
    {
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER
        // Retail (Codex pass 11): the ctor tests the member it just stored -- 0x00626E40 spills the pointer and reloads it
        // for the test and the CCPPointerInfo store, which `if (data)` (the landed default) does not reproduce.
        if (Data)
#else
        if (data)
#endif
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_COPY_DECL_ONLY
    // NF76 per-TU opt-in (row 0x00B42530 CEngineMap::OpenDevelopmentStaticMap): that TU calls the copy constructor COMDAT
    // (??0?$CCountedPointer@VCBankFileAsyncEntry@@@@QAE@ABV0@@Z 0x009D69A0, Ego_r.pdb / FableWin.pdb name set) for a by-value argument
    // where the body below would be expanded. Declaration only; the retail body is the default one.
    CCountedPointer(const CCountedPointer& other);
#else
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
#endif
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA) || defined(FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY) || defined(FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_NOINLINE)
    // Per-TU opt-in (agentNF 2026-11-10, the NUISystem TUs): the destructor is `{ ReleaseData(); }` and ReleaseData is the release with
    // Data zeroed before Info. Evidence: Ego_r.pdb ??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ is a 5-byte function (0x00438DDC,
    // next symbol 0x00438DE1) and retail has the same 5-byte `jmp` COMDAT at 0x00429C53 (-> 0x004291DE), called at the scope ends of
    // locals (0x0054522E, 0x0056BBB9); the 44-byte release 0x004291DE is called on live pointers (ClearBastardChild 0x0041DEED loop,
    // Reset 0x00429C72, ShareData 0x00429217) = ?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ (FableWin.pdb: 420
    // ReleaseData instantiations for 419 destructors; Ego_r.pdb keeps ReleaseData<CEventBase> out of line, 44 bytes). Same pair for
    // CAction (0x00429160 / jmp 0x00429B72) and CMeshDataBank (0x0042918C / jmp 0x00429BF9). With _DECL_ONLY ReleaseData is declared
    // only (the TU calls it out of line); the destructor stays inline and expands to that call.
    ~CCountedPointer() { ReleaseData(); }
#elif defined(FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY)
    // Per-TU opt-in (Codex name fix 2026-11-10, ~CCountedPointer<NUISystem::CComponent> 0x004291DE): where retail calls the
    // destructor COMDAT from a speed-optimised TU (0x00429E16, the map pair destructor) the body above would be expanded.
    // Declaration only; the retail body is the #else one (0x004291DE: Data zeroed before Info).
    ~CCountedPointer();
#else
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR) && defined(_MSC_VER)
    // Per-TU opt-in (chunk 1010b, row 0x008A8C60 CGameScriptInterface::EntitySetAsSittingOnFloor): retail CALLS this destructor's
    // same-TU COMDAT (0x008AB3D0 ??1?$CCountedPointer@VCModeSitBase@NCreatureMode@@@@QAE@XZ, the body below) yet packs the counted
    // pointer into the frame slot of an earlier temporary and copies it without re-reading Info (0x008A8D8C..0x008A8DA2): the
    // body is visible to c2 (it does not keep the address) but not expanded. Pair with FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR.
    __declspec(noinline)
#endif
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
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_ASSIGN_DECL_ONLY
    // Retail opt-in (Pass22, whole parent00D29880): native00D29C17 calls the
    // original CCountedPointer<CScriptGameResourceObjectScriptedThingBase>
    // assignment COMDAT008AB1E0 after expanding its enclosing implicit assignment.
    // Both PDBs identify this member; default inline body remains unchanged below.
    CCountedPointer& operator=(const CCountedPointer& other);
#else
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CONVERTING_ASSIGN_DECL_ONLY
    // NF222 per-TU opt-in (2026-11-22, row 0x0042DDB3 CNewFrontendGameComponent::InitialiseGraphics): that TU calls the converting
    // assignment COMDAT out of line (0x0042FE34 ??$?4VCFontBank@@@?$CCountedPointer@$$CBVCFontBank@@@@QAEAAV0@ABV?$CCountedPointer@VCFontBank@@@@@Z,
    // FableWin.pdb), and its frame-pointer TU (optimize("y", off)) would emit a pick-any copy of the body below with `push ebp` that
    // the retail owner lacks. Declaration only; the retail body is the default one below.
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other);
#else
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
#endif
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   // unproven body (FableWin name only)
    void Validate() const;                                               // FableWin.pdb only (debug build check)
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_NOINLINE)
    // NF73 opt-in (row 0x00597006 CFrontEndManager::RefreshAvailableProfilesForDelete): the _RELEASEDATA body (Data zeroed before Info),
    // visible to the TU but kept out of line, as the retail CFrontEndManager TU calls its COMDATs (0x004291DE CComponent, 0x00429160
    // CAction, 0x0059A994 CList) yet keeps the local CCountedPointer<CList>'s data pointer in ebx across the button loop
    // (0x005970F7 `mov ebx,[ebp-34h]` ... 0x005971CE `mov ecx,ebx`): c2 knows the release keeps no address of the pointer.
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    void ReleaseData()
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
#elif defined(FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY)
    void ReleaseData();                                                  // out of line (0x004291DE for CComponent): the _RELEASEDATA body
#elif defined(FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA)
    void ReleaseData()                                                   // 0x004291DE (CComponent): Data zeroed before Info
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
#else
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
#endif
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_VISIBLE_PASS26)
    // The original SetStaticCast calls this visible ShareData body out of line in CList::Initialise.
#ifdef _MSC_VER
    __declspec(noinline)
#endif
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
#elif defined(FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY)
    // Per-TU opt-in (NF194, 2026-11-20; the CEntityScriptBinding<T>::CreateActiveEntityScript family, q5992): retail CALLS the
    // out-of-line copy 0x00CE1040 ?ShareData@?$CCountedPointer@VCEntityScriptBase@@@@QAEXPAVCEntityScriptBase@@PAVCCPPointerInfo@@@Z
    // from the speed-optimised binding TUs, where the in-class body below would be expanded. Declaration only; the retail body is
    // the #else one (landed self row 0x00CE1040 instantiates it).
    void ShareData(T* data, CCPPointerInfo* info);
#else
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
            // Original donor027C8B40 reloads the stored Info; legacy default retained below.
            if (Info)
                ++Info->RefCount;
#else
            if (info)
                ++info->RefCount;
#endif
        }
    }
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
    void Reset(T* data);
#elif defined(FABLE_RETAIL_CCOUNTEDPOINTER_RESET_ELSE_NULL)
    // Opt-in (acctC agentC4, rows 0x005FF190 CreateMapMarkers / 0x0060EEB0 ConstructPCMapList, one TU): the same release and adopt
    // with the count block built from the argument and an explicit `else Info = 0;`. Same expanded bytes; c2 then costs the
    // inline body 108 (the default text 95), which alone reproduces retail's 9 Reset sites in 0x005FF190 (called at the hero site,
    // the three loop-1 sites and loop-3 sites 2-4, expanded at loop-3 site 1 0x005FF8C7 and the teleport site 0x005FFD4D: score
    // c1*(1/f+2)/3 against 74). FableWin.pdb ?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@ is 69i/207b (ours 44i at /Od /EHsc).
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)data);
        else
            Info = 0;
    }
#elif defined(FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CALLS_DTOR)
    // Per-TU opt-in (Codex name fix 2026-11-10, row 0x00429C6F Reset<NUISystem::CComponent>, size TU): retail's out-of-line Reset
    // calls 0x004291DE, the Data-first release body that Ego_r.pdb names ??1?$CCountedPointer@VCComponent@NUISystem@@@@ (the
    // only out-of-line release that PDB has for this T: no ReleaseData). The release is spelled as that destructor; the
    // original spelling of the call is unproven.
    void Reset(T* data)
    {
        this->~CCountedPointer();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
#elif defined(FABLE_RETAIL_CCOUNTEDPOINTER_RESET_ELSE_KEEPS_INFO)
    // Inline-budget opt-in (acctC agentC21, the CTCInventoryMap TU: rows 0x00603000 / 0x005FF190; FITTED, see hdr/README.md). The
    // default text plus an else arm that stores Info's own value. It emits the default bytes wherever retail can be compared (the
    // /O1 out-of-line body = 0x00429C6F, 59 bytes; every expansion on a local = the default expansion), but VC7.1's own /O2 COMDAT
    // of it is 114 bytes instead of 103 (the null path keeps a `mov [esi+4],0` and its own exit), and c2 prices an inline callee by
    // the bytes of that COMDAT: 106 instead of 95. Retail's price for Reset<CComponent> in this TU is 108 or 109 (measured on three
    // rows); the missing 2 are covered per row by `// Flags: /d2inlT72`. NOT claimed to be the original text.
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = Info;
    }
#elif defined(FABLE_RETAIL_CCOUNTEDPOINTER_RESET_NOINLINE) && defined(_MSC_VER)
    // Per-TU opt-in (NF62, row 0x00506E30 CWorldMap::GetLevelDimensions(const CCharString&)): the default Reset body below, visible but kept
    // out of line -- that TU calls CCountedPointer<CAFile>::Reset as a COMDAT (0x0050F550, both arms) where VC7.1 expands it; seeing that it
    // writes only the two members, VC7.1 keeps the pointer's address private and releases it with `dec dword ptr [esi] ; jne` (a
    // declaration-only Reset reloads the count: `dec [eax] ; cmp [esi],0`).
    __declspec(noinline) void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
#else
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
#endif
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   // FableWin.pdb ??$SetStaticCast@..@?$CCountedPointer@..@@QAEXABV?$CCountedPointer@..@@@Z (63)
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              // landed 0x00A32620 (C3DMeshFile::GetRoot) expands it
    }
    void reset(T* data) { Reset(data); }                                 // unproven body (FableWin name only)
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
    // NF222 per-TU opt-in (2026-11-22, the CCountedPointer<T>::ReleaseData family Reset rows 0x00419428 / 0x0041947A / 0x004194D1 /
    // 0x0042FC13 / 0x0042FC86): those TUs only store the deleter's address (retail 0x0041901D / 0x00419028 / 0x00419036 / 0x0042F864 /
    // 0x00428AE7, each the virtual delete `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret`) while T is only declared, so the
    // inline body below would emit a pick-any copy that is a plain operator delete. Declaration only (as the rows declared it at HEAD);
    // the retail body is the default one with T complete.
    static void FABLE_FASTCALL DeleteData(void* data);
#else
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    // per-T deleter, address stored in the count block
#endif
};

#endif
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_CFONTBANK_SHAREDATA_SIZE_BODY) || defined(FABLE_RETAIL_CCOUNTEDPOINTER_CFONTBANK_ASSIGN_SIZE_BODY) || defined(FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_RELEASEDATA_SIZE_BODY) || defined(FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_SHAREDATA_SIZE_BODY) || defined(FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_CONVERTING_ASSIGN_SIZE_BODY) || defined(FABLE_RETAIL_CCOUNTEDPOINTER_CGRAPHICDATABANK_RELEASEDATA_SIZE_BODY) || defined(FABLE_RETAIL_CCOUNTEDPOINTER_CGRAPHICDATABANK_RESET_SIZE_BODY) || defined(FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_RESET_SIZE_BODY) || defined(FABLE_RETAIL_CCOUNTEDPOINTER_CMESHDATABANK_RELEASEDATA_SIZE_BODY) || defined(FABLE_RETAIL_CCOUNTEDPOINTER_CMESHDATABANK_RESET_SIZE_BODY)
// NF225 opt-ins (2026-11-22, q12562; rows 0x00494840 CFrontendGameComponent::InitGraphics, 0x0054ED90 NUISystem::CText::GetDataFromDef).
// Fidelity: scaffolding, not retail. Those /O2 TUs expand every CCountedPointer<CFontBank> / <const CFontBank> member at the call
// site (as retail does), and VC7.1 still emits each expanded inline as a pick-any COMDAT. Retail kept the out-of-line copies from the
// 0x00419xxx / 0x0042Fxxx size TUs. These explicit specializations compile the header's own bodies (ReleaseData: the
// FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA body, Data zeroed before Info; ShareData / operator=: the default bodies, which then call
// ReleaseData / ShareData out of line) under optimize("s", on), as each owner's TU did, so each COMDAT is the owner's body; callers
// still expand them. Owner check: the owner rows set `#pragma optimize("s", on)` themselves (0x0042F948, 0x0042F9F2, 0x0042FE34,
// 0x004190E2, 0x00419463; catalog flags /O2). ReleaseData<CFontBank> (0x004057A0, an /O2 body, no pragma) needs no opt-in here: a row
// takes it from FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA. `#pragma optimize("", on)` below returns to the command-line settings
// (VC7.1 has no push / pop for optimize), so a row that uses these opt-ins must not carry a row-head optimize pragma before its include
// of this header (nf189). Needs no use of the specialised member before this point (define the macros before the first include of
// this header). Not with FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22 (that class has no member-template operator=).
class CFontBank;
#pragma optimize("s", on)
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CGRAPHICDATABANK_RELEASEDATA_SIZE_BODY
class CGraphicDataBank;
// NF234 opt-in (2026-11-22, q12650; row 0x00495780 CFrontendGameComponent::Shutdown), the same scaffolding as the CONSTCFONTBANK one below:
// ?ReleaseData@?$CCountedPointer@VCGraphicDataBank@@@@QAEXXZ = 0x00419108 (owner row: optimize("s", on) + FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA).
template<> inline void CCountedPointer<CGraphicDataBank>::ReleaseData()
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
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_RELEASEDATA_SIZE_BODY
// ?ReleaseData@?$CCountedPointer@$$CBVCFontBank@@@@QAEXXZ = 0x0042F948 (44 bytes, owner row: optimize("s", on)).
template<> inline void CCountedPointer<const CFontBank>::ReleaseData()
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
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CFONTBANK_SHAREDATA_SIZE_BODY
// ?ShareData@?$CCountedPointer@VCFontBank@@@@QAEXPAVCFontBank@@PAVCCPPointerInfo@@@Z = 0x004190E2 (38 bytes, calls ReleaseData; owner row: optimize("s", on)).
template<> inline void CCountedPointer<CFontBank>::ShareData(CFontBank* data, CCPPointerInfo* info)
{
    if (Info != info) {
        ReleaseData();
        Data = data;
        Info = info;
        if (info)
            ++info->RefCount;
    }
}
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_SHAREDATA_SIZE_BODY
// ?ShareData@?$CCountedPointer@$$CBVCFontBank@@@@QAEXPBVCFontBank@@PAVCCPPointerInfo@@@Z = 0x0042F9F2 (38 bytes, calls ReleaseData; owner row: optimize("s", on)).
template<> inline void CCountedPointer<const CFontBank>::ShareData(const CFontBank* data, CCPPointerInfo* info)
{
    if (Info != info) {
        ReleaseData();
        Data = data;
        Info = info;
        if (info)
            ++info->RefCount;
    }
}
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CFONTBANK_ASSIGN_SIZE_BODY
// ??4?$CCountedPointer@VCFontBank@@@@QAEAAV0@ABV0@@Z = 0x00419463 (23 bytes, calls ShareData; owner row: optimize("s", on)).
template<>
#ifdef FABLE_RETAIL_MOVIE_PARSE_PASS29
#ifdef _MSC_VER
// Inline-budget: The original subtitle parser calls the 23-byte font assignment at 00419463.
__declspec(noinline)
#endif
#endif
inline CCountedPointer<CFontBank>& CCountedPointer<CFontBank>::operator=(const CCountedPointer<CFontBank>& other)
{
    ShareData(other.Data, other.Info);
    return *this;
}
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_CONVERTING_ASSIGN_SIZE_BODY
// ??$?4VCFontBank@@@?$CCountedPointer@$$CBVCFontBank@@@@QAEAAV0@ABV?$CCountedPointer@VCFontBank@@@@@Z = 0x0042FE34 (23 bytes, calls ShareData; owner row: optimize("s", on)).
template<> template<> inline CCountedPointer<const CFontBank>& CCountedPointer<const CFontBank>::operator=<CFontBank>(const CCountedPointer<CFontBank>& other)
{
    ShareData(other.Data, other.Info);
    return *this;
}
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CGRAPHICDATABANK_RESET_SIZE_BODY
// NF234 opt-in (2026-11-22, q12650; row 0x00495780): ?Reset@?$CCountedPointer@VCGraphicDataBank@@@@QAEXPAVCGraphicDataBank@@@Z = 0x004194D1
// (57 bytes, calls ReleaseData 0x00419108, stores DeleteData 0x00419036; owner row: optimize("s", on)). The default Reset body.
class CGraphicDataBank;
template<> inline void CCountedPointer<CGraphicDataBank>::Reset(CGraphicDataBank* data)
{
    ReleaseData();
    Data = data;
    if (data)
        Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
}
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_RESET_SIZE_BODY
// NF234 opt-in (2026-11-22, q12650; row 0x00495780): ?Reset@?$CCountedPointer@$$CBVCFontBank@@@@QAEXPBVCFontBank@@@Z = 0x0042FC13
// (calls ReleaseData 0x0042F948, stores DeleteData 0x0042F864; owner row: optimize("s", on)). The default Reset body.
template<> inline void CCountedPointer<const CFontBank>::Reset(const CFontBank* data)
{
    ReleaseData();
    Data = data;
    if (data)
        Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
}
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CMESHDATABANK_RELEASEDATA_SIZE_BODY
class CMeshDataBank;
// NF235 opt-in (2026-11-22, q12657; row 0x00A0A150 ~CMeshBankManager), the same scaffolding as the CGRAPHICDATABANK one above:
// ?ReleaseData@?$CCountedPointer@VCMeshDataBank@@@@QAEXXZ = 0x0042918C (53 bytes: the Data-first FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
// body under optimize("s", on), `and dword ptr [esi],0 ; and dword ptr [esi+4],0`).
template<> inline void CCountedPointer<CMeshDataBank>::ReleaseData()
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
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CMESHDATABANK_RESET_SIZE_BODY
// NF235 opt-in (2026-11-22, q12657; row 0x00A0A150): ?Reset@?$CCountedPointer@VCMeshDataBank@@@@QAEXPAVCMeshDataBank@@@Z = 0x0042FC86
// (calls ReleaseData 0x0042918C, stores DeleteData 0x00428AE7; landed owner row 0x0042FC86). The default Reset body.
class CMeshDataBank;
template<> inline void CCountedPointer<CMeshDataBank>::Reset(CMeshDataBank* data)
{
    ReleaseData();
    Data = data;
    if (data)
        Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
}
#endif
#pragma optimize("", on)
#endif
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY) || defined(FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RELEASEDATA_SIZE_BODY)
// NF352 opt-ins (2026-11-24; T3 nf340 0x00429C6F; rows 0x00605570 CTCInventoryMap::ConstructRegionDescription, 0x00626110
// CTCQuickAccessMenu::ContructItemLists), the size-pragma scaffolding of EXTRA_RULES 2026-11-20 / 2026-11-21 condition 2 in the per-type
// form of NF349 (engine/CDefinitionManager.h GETDEF_<T>_SIZE_BODY). Fidelity: scaffolding, not retail: it is for link determinism and
// makes no claim about retail's text. Those /O2 TUs call CCountedPointer<NUISystem::CComponent>::Reset out of line and emit their own
// speed-mode pick-any copy of it (103 bytes; ReleaseData 53), while retail's linker kept the copies of the 0x00429xxx size TU. The owner
// rows set `#pragma optimize("s", on)` themselves and build at the catalog default /O2 /Oy /W3 (no CompilerFlags entry):
//   Reset         0x00429C6F  rebuild/src/compiled/00/42/CCountedPointerNUISystemCComponent_Reset_00429c6f.cpp (+ FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA)
//   ReleaseData   0x004291DE  rebuild/src/compiled/00/42/CCountedPointerNUISystemCComponent_ReleaseData_004291de.cpp (+ _RELEASEDATA) -- since nf356 this owner uses the opt-in itself (its pragma now follows the include); condition (b) was proved on its pre-port text
// VC7.1 takes a template instance's optimize mode at the end of the TU, so a pragma around the class template does nothing; an
// explicit specialization is an ordinary definition and takes the mode at its own point. Each body is the owner's configuration of the
// class template text above, token for token (Reset: the default body; ReleaseData: the _RELEASEDATA body, Data zeroed before Info).
// The (T*) constructor 0x00429C15 and destructor 0x00429C53 were measured in this form and are NOT here: noinline or inline, either
// moves the 0x00626110 / 0x00626750 leaf (NF352 hand-back). Each pair ends with optimize("", on), which restores the command-line
// settings (VC7.1 has no push / pop for optimize), so a row that uses these opt-ins must not carry a row-head optimize pragma before its
// include of this header (nf189). Define the macros before the first include of this header (no use of a specialised member may
// precede it). Not with FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22.
namespace NUISystem { class CComponent; }
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RELEASEDATA_SIZE_BODY
// ?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ = 0x004291DE (44 bytes). Before Reset, which calls it.
#if defined(_MSC_VER)
#pragma optimize("s", on)
template<> __declspec(noinline) inline
#else
template<> inline
#endif
void CCountedPointer<NUISystem::CComponent>::ReleaseData()
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
#if defined(_MSC_VER)
#pragma optimize("", on)
#endif
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY
// ?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z = 0x00429C6F (59 bytes, calls ReleaseData 0x004291DE,
// stores DeleteData 0x00428B10).
#if defined(_MSC_VER)
#pragma optimize("s", on)
template<> __declspec(noinline) inline
#else
template<> inline
#endif
void CCountedPointer<NUISystem::CComponent>::Reset(NUISystem::CComponent* data)
{
    ReleaseData();
    Data = data;
    if (data)
        Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
}
#if defined(_MSC_VER)
#pragma optimize("", on)
#endif
#endif
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY
#error FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU and FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY define the same specialization: use one
#endif
// NF364 opt-in (2026-11-24; T3 nf358 0x00429C6F; NF362 proposal P1, advisor decision, owner delegated). Fidelity: scaffolding, not
// retail: it is for link determinism and makes no claim about retail's text. The sibling of FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY
// for TUs whose row head sets `#pragma optimize("y", off)` + `("s", on)` ABOVE their include of this header (the CreateTickSlider,
// CTCInventory, CTCInventoryTrade and CTCInventoryStats rows). VC7.1 compiles their pick-any Reset<CComponent> at the end-of-TU state
// (s on, y off: 62 bytes, frame pointer); the owner 0x00429C6F compiles it at s on, y on (59 bytes). The NF352 block does not serve these
// TUs: after the row's pragmas it still compiles at y off, and its optimize("", on) returns the rest of the TU (the row's own leaf) to the
// command line (NF362). This block switches to the owner's mode (y on, s on; the owner row sets `#pragma optimize("s", on)` itself and
// builds at the catalog default /O2 /Oy /W3, no CompilerFlags entry), defines the same specialization (the NF352 body, token for token),
// then RESTORES y off + s on instead of the command line (VC7.1 has no push / pop for optimize). The restore is hard-coded, so a row may
// define this macro only where the optimize state here is exactly y off + s on with every other setting at the command line: pragscan the
// row's TU and check that the state before this block equals the state after it. Define the macro before the first include of this
// header. Not with FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY (#error above) or FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22.
// ?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z = 0x00429C6F (59 bytes, calls ReleaseData 0x004291DE,
// stores DeleteData 0x00428B10).
namespace NUISystem { class CComponent; }
#if defined(_MSC_VER)
#pragma optimize("y", on)
#pragma optimize("s", on)
template<> __declspec(noinline) inline
#else
template<> inline
#endif
void CCountedPointer<NUISystem::CComponent>::Reset(NUISystem::CComponent* data)
{
    ReleaseData();
    Data = data;
    if (data)
        Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
}
#if defined(_MSC_VER)
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
#endif
#endif // FABLE_ENGINE_CCOUNTEDPOINTER_H
