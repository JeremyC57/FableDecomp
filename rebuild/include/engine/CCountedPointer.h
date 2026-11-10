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
//   ShareData = 0x00CE1040 (CCountedPointer<CEntityScriptBase>: every landed caller binds that T, FableWin has the symbol; the method name is Ego_r.pdb's): `if (Info != info) { <release>; Data = data;
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
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA) || defined(FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY)
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
#if defined(FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY)
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
#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CALLS_DTOR
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
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    // per-T deleter, address stored in the count block
};

#endif // FABLE_ENGINE_CCOUNTEDPOINTER_H
