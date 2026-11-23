#ifndef FABLE_ENGINE_NHEROINFORMATIONSCREENS_CMANAGER_H
#define FABLE_ENGINE_NHEROINFORMATIONSCREENS_CMANAGER_H
#ifndef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#error "NHeroInformationScreens_CManager.h: define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER (retail model only)"
#endif
// NHeroInformationScreens::CManager (chunk 735, row 0x00577AC0 CTCInventoryExperience::OpenInventoryInternalPC): the hero
// information screens manager CPlayerGui owns through scoped_ptr (CPlayerGui::GetPHeroInfoScreensManager 0x00437CD0). Neither
// PDB gives its data layout (not in struct_layouts_egor.tsv), so only the two methods the row calls are declared, with their
// Ego_r.pdb / FableWin.pdb spellings:
//   ?GetCurrentScreen@CManager@NHeroInformationScreens@@QAEPBVCBase@2@XZ, out of line at 0x0066E7BD (landed);
//   ?SelectScreenOfType@CManager@NHeroInformationScreens@@QAEXW4EScreenType@2@@Z, out of line at 0x0066E799: a `ret 4`
//   thiscall that stores its one argument at +0x28; the row passes INVENTORY_EXPERIENCE_SCREEN (2). The landed label
//   "CObservable::AddExclusiveObserver" (propagated from a byte-identical setter) does not fit this class.
#include "engine/NHeroInformationScreens_CBase.h"
#ifdef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#include "stlport/fable_containers.h"
class CWorld;
#endif
namespace NHeroInformationScreens {
class CManager {
public:
    const CBase* GetCurrentScreen();                    // 0x0066E7BD
    void SelectScreenOfType(EScreenType type);          // 0x0066E799
#ifdef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGIONLOAD
    // Opt-in (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad): Ego_r.pdb ?RemoveAllPlayerModes@CManager@NHeroInformationScreens@@QBEXXZ
    // (0x0066E985) and ?Close@CManager@NHeroInformationScreens@@QAEXXZ (0x0066E73D), called; FableWin.pdb ?IsOpen@CManager@NHeroInformationScreens@@QBE_NXZ,
    // inline (the byte +0x34 tested in place, `mov cl,[eax+34h]`, 0x004A48F3). Needs _LAYOUT without _CTOR (adds the +0x2c..+0x34 bytes).
    void RemoveAllPlayerModes() const;                  // 0x0066E985
    void Close();                                       // 0x0066E73D
    bool IsOpen() const { return Open; }
#endif
#ifdef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGISTER
    // Retail opt-in (chunk 1023, row 0x0061A6A0): Ego_r.pdb / FableWin.pdb ?RegisterScreen@CManager@NHeroInformationScreens@@QAEXPBVCBase@2@@Z,
    // out of line at 0x0066EB11 (every inventory screen's OnCreate registers itself; the landed bound spelling). Declaration only.
#ifdef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGISTER_REFERENCE
    // NF300 add-only opt-in (2026-11-23): Ego_r.pdb + FableWin.pdb spell it ?RegisterScreen@CManager@NHeroInformationScreens@@QAEXABVCBase@2@@Z
    //   (a const reference; the self row 0x0066EB11 defines that), so the pointer line below is not the PDB spelling. Declaration only.
    void RegisterScreen(const CBase& screen);           // 0x0066EB11
#else
    void RegisterScreen(const CBase* screen);           // 0x0066EB11
#endif
#endif
#ifdef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_DEREGISTERALL
    // NF79 opt-in (round 7, row 0x0089F4A0 CGameScriptInterface::TurnCreatureInto): Ego_r.pdb + FableWin.pdb
    // ?DeregisterAllScreens@CManager@NHeroInformationScreens@@QAEXXZ, out of line at 0x0066EBE0. Declaration only.
    void DeregisterAllScreens();                        // 0x0066EBE0
#endif
#ifdef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_INITIALISESCROLLBARINDICES
    // NF94 opt-in (round 8, q11238 row 0x0066EA85): Ego_r.pdb + FableWin.pdb ?InitialiseScrollBarIndices@CManager@NHeroInformationScreens@@QAEXJ@Z,
    // the row itself (out of line at 0x0066EA85). Declaration only; the row also needs _LAYOUT (ScrollBarIndices +0x1c).
    void InitialiseScrollBarIndices(long count);        // 0x0066EA85
#endif
#ifdef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
    // Retail opt-in (chunk 853, row 0x0043A570 ~CPlayerGui): the inline ~CManager (Ego_r.pdb ??1CManager@NHeroInformationScreens@@QAE@XZ,
    // non-virtual; boost::checked_delete then operator delete, no vtable store) frees three STLport vectors at +0x1C, +0x10, +0x04 in that
    // order (0x0043A5AD..0x0043A5DD) and SelectScreenOfType stores at +0x28. Member names and element types are DESCRIPTIVE (neither PDB
    // gives the layout; the PDB's GetAllRegisteredScreens fills a vector<const CBase*>); the destructor is element-type independent.
    ~CManager() { }
#ifdef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_CTOR
    // Retail opt-in (chunk 863, row 0x0043B570 CPlayerGui::CPlayerGui): Ego_r.pdb ??0CManager@NHeroInformationScreens@@QAE@AAVCWorld@@@Z,
    // out of line at 0x0066EAB7 (`push [esi+4] ; mov ecx,eax ; call` after `new(0x54)`). Declaration only.
    CManager(CWorld& world);                                              // 0x0066EAB7
#endif
#ifdef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_ISOPEN
    // Retail opt-in (wfA1, row 0x006D0830 CTCWife::GlobalUpdate): FableWin.pdb ?IsOpen@CManager@NHeroInformationScreens@@QBE_NXZ
    // (0x01A700B4), inline in retail: `call 0x00437CD0 (GetPHeroInfoScreensManager) ; mov cl,[eax+34h] ; test cl,cl` (0x006D102D).
    // Needs ..._LAYOUT and not ..._CTOR (the bool sits inside the CTOR pad). Member name DESCRIPTIVE (no PDB gives the layout).
    bool IsOpen() const { return Open; }
#endif
private:
    CWorld* PWorld;                                     // +0x00 descriptive (ctor ??0CManager@..@QAE@AAVCWorld@@@Z)
    std::vector<const CBase*> RegisteredScreens;        // +0x04 descriptive
    std::vector<const CBase*> DisplayedScreens;         // +0x10 descriptive
    std::vector<long> ScrollBarIndices;                 // +0x1c descriptive
    long CurrentScreenIndex;                            // +0x28 descriptive (SelectScreenOfType's store)
#if defined(FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_ISOPEN) && !defined(FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_CTOR)
    unsigned char _pad_0x2c[0x34 - 0x2c];               // +0x2c (opt-in ISOPEN, wfA1)
    bool Open;                                          // +0x34 descriptive (IsOpen's read, 0x006D1032)
#endif
#ifdef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_CTOR
    unsigned char _pad_0x2c[0x54 - 0x2c];               // +0x2c (chunk 863: retail allocates 0x54 bytes, CPlayerGui ctor 0x0043BA68)
#endif
#if defined(FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGIONLOAD) && !defined(FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_CTOR)
    unsigned char _pad_0x2c[0x34 - 0x2c];               // +0x2c
    bool Open;                                          // +0x34 descriptive (IsOpen's byte)
#endif
#endif
};
}
#endif
