#ifdef FABLE_RETAIL_CTCINGAMEMENU
#ifndef FABLE_ENGINE_CTCINGAMEMENU_H
#define FABLE_ENGINE_CTCINGAMEMENU_H
// Retail opt-in (chunk 733, CUserProfileManager::ApplyVideoValues 0x0040A7F0; the CTCBulletTime pattern): the thing component on
// the polymorphic CTCBase (define FABLE_RETAIL_CTCBASE), InGameMenuOpen +0x0C (the generated header's offset), the rest opaque
// to 0xF8. ?StatGetInterfaceType@CTCInGameMenu@@SI?AW4ETCInterfaceType@@XZ (TCI_IN_GAME_MENU = 198, the `0xC6` key) and
// ?IsOpen@CTCInGameMenu@@QBE_NXZ (FableWin 0x01896870 / 0x01884740) are inline in retail.
// Chunk 735 (row 0x0047F58B AddRegionAndTimeInfo): the per-save-slot region / minimap / play-time vectors at their retail offsets
// (STLport 12-byte vectors; the Ego_r layout's +0x98 .. +0xB0 sits 0x20 higher), ?VerifySignatureCached@CTCInGameMenu@@QAE_NABVCWideString@@@Z
// (0x0047ED0A, landed) and the private ?AddRegionAndTimeInfo@CTCInGameMenu@@AAEXABVCWideString@@@Z (Ego_r + FableWin).
#include "engine/CTCBase.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CTCINGAMEMENU_OPENMENU
#include "stlport/fable_map.h"
#endif
#if defined(FABLE_RETAIL_CTCINGAMEMENU_OPENPC) || defined(FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD)
namespace NUISystem { class CComponent; class CYesNo; }
#endif
#ifdef FABLE_RETAIL_CTCINGAMEMENU_VIDEODIALOGS
#ifndef FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#error "CTCInGameMenu.h: FABLE_RETAIL_CTCINGAMEMENU_VIDEODIALOGS needs FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD (the native member layout)"
#endif
#endif
class CTCInGameMenu : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_IN_GAME_MENU; }
    bool IsOpen() const { return InGameMenuOpen; }
    bool VerifySignatureCached(const CWideString&);                  // 0x0047ED0A
#ifdef FABLE_RETAIL_CTCINGAMEMENU_OPENPC
    // Nested opt-in (chunk 880, row 0x0058AF10 CTCInventory::OpenPCOptionsMenu): ?OpenInGameMenuPC@CTCInGameMenu@@QAEXPAVCComponent@NUISystem@@0@Z
    // (Ego_r.pdb / FableWin.pdb), out of line at 0x004804DE; the options screen passes the inventory's mouse pointer and its list.
    void OpenInGameMenuPC(NUISystem::CComponent* mousePointer, NUISystem::CComponent* list);   // 0x004804DE
#endif
#ifdef FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
    // Retail opt-in (Codex pass19): both original PDB signatures and native offsets.
    void SetToSave(long);
    void PCSetToLoad(long);
#endif
#ifdef FABLE_RETAIL_CTCINGAMEMENU_OPENMENU
    // Retail opt-in (bwf2 row 0x004830A1): Ego_r.pdb ?OpenInGameMenu@CTCInGameMenu@@QAEXXZ (0x0048FB01) / FableWin.pdb (0x01AA64D9) and
    // the private ?GetFileNameList@CTCInGameMenu@@AAEXXZ (Ego_r 0x0048C6D5; retail 0x0047FE83, same TU, out of line); FileNames +0x28
    // and Signatures +0xE0 typed (needs FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD).
    void OpenInGameMenu();                                           // 0x004830A1
private:
    void GetFileNameList();                                          // 0x0047FE83
public:
#endif
#ifdef FABLE_RETAIL_CTCINGAMEMENU_VIDEODIALOGS
    // agentQ7 (2026-11-11, rows 0x0047E651 / 0x0047F30F / 0x0047E9AE): Ego_r.pdb / FableWin.pdb ?VideoOptionsChanged@CTCInGameMenu@@QAEXXZ,
    // ?VideoOptionsNotAccepted@CTCInGameMenu@@QAEXXZ, ?SaveGame@CTCInGameMenu@@QAEXJ@Z (0x0047E196) and the CTCBase slot-26 override
    // ?GlobalUpdate@CTCInGameMenu@@UAEX_N@Z, plus the two video dialogs and the revert timer at their retail offsets (Ego_r +0xD0 /
    // +0xD4 / +0xE0, 0x20 lower with the 12-byte STLport vectors: the rows read +0xB0 / +0xB4 / +0xC0).
    void VideoOptionsChanged();                                      // 0x0047E651
    void VideoOptionsNotAccepted();                                  // 0x0047F30F
    void SaveGame(long);                                             // 0x0047E196
    virtual void GlobalUpdate(bool);                                 // 0x0047E9AE (slot 26)
#endif
private:
    void AddRegionAndTimeInfo(const CWideString&);                   // 0x0047F58B
    bool          InGameMenuOpen;                                    // +0x0c
#ifdef FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#ifdef FABLE_RETAIL_CTCINGAMEMENU_OPENMENU
    unsigned char _pad_0x0d[0x28 - 0x0d];
    std::vector<CWideString> FileNames;                              // +0x28
    unsigned char _pad_0x34[0x58 - 0x34];                            // +0x34 FileTimes, PrimitiveHandles, MenuPrimitiveHandles
#else
    unsigned char _pad_0x0d[0x58 - 0x0d];
#endif
    std::vector<NUISystem::CComponent*> Components;
    long LoadGameIndex;
    double CurStartPauseTime;
    double AccumulativePausedTime;
#else
    unsigned char _pad_0x0d[0x78 - 0x0d];                            // +0x0d PDef .. AccumulativePausedTime (generated header below)
#endif
    std::vector<CCharString> FileRegionNames;                        // +0x78
    std::vector<CCharString> FileRegionMiniMapNames;                 // +0x84
    std::vector<long>        FilePlayTimes;                          // +0x90
#ifdef FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#ifdef FABLE_RETAIL_CTCINGAMEMENU_VIDEODIALOGS
    unsigned char _pad_0x9c[0xb0 - 0x9c];
    NUISystem::CYesNo* PVideoOptionsNotAcceptedDialog;                // +0xb0
    NUISystem::CYesNo* PVideoOptionsChangedDialog;                    // +0xb4
#else
    unsigned char _pad_0x9c[0xb8 - 0x9c];
#endif
    long SaveGameToLoad;
#ifdef FABLE_RETAIL_CTCINGAMEMENU_VIDEODIALOGS
    unsigned char _pad_0xbc[0xc0 - 0xbc];
    double VideoChangesTimer;                                        // +0xc0
#else
    unsigned char _pad_0xbc[0xc8 - 0xbc];
#endif
    long SaveGameIndex;
    long FramesToWaitToSave;
    NUISystem::CComponent* PSaveInProgress;
    unsigned char _pad_0xd4[4];
    double SaveInProgressTime;
#ifdef FABLE_RETAIL_CTCINGAMEMENU_OPENMENU
    std::map<CWideString, bool> Signatures;                          // +0xe0
#else
    unsigned char Signatures[12];
#endif
    NUISystem::CComponent* PMousePointer;
    NUISystem::CYesNo* PYesNo;
    unsigned char _pad_0xf4[4];
#else
    unsigned char _pad_0x9c[0xf8 - 0x9c];                            // +0x9c .. PYesNo (generated header below)
#endif
#ifdef FABLE_RETAIL_CTCINGAMEMENU_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual ETCInterfaceType GetInterfaceType() const;  // slot 21, 0x0048616C
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCInGameMenu) == 0xf8);
#endif
#else
#ifndef FABLE_ENGINE_CTCINGAMEMENU_H
#define FABLE_ENGINE_CTCINGAMEMENU_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCInGameMenu = 0x118.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CTCInGameMenu = 0xf8.
// Retail evidence: 2 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

struct CComponent;
class CDefPointeeBase;
struct CYesNo;

#pragma pack(push, 1)
class CTCInGameMenu {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    long             field4;                                 // +0x04 retail-only long (previous header)
    unsigned char    _pad_0x08[0x4];                         // +0x08
    bool             InGameMenuOpen;                         // +0x0c
    unsigned char    _pad_0x0d[0x3];                         // +0x0d
    CDefPointeeBase* PDef_Object;                            // +0x10 CDefPointer<CInventoryDef_const_>::Object
    long             GlobalDefIndex;                         // +0x14
    unsigned char    ActiveScreen[0x4];                      // +0x18 EScreenType
    long             ItemCount;                              // +0x1c
    long             ItemHighlightIndex;                     // +0x20
    long             ItemTopIndex;                           // +0x24
    unsigned char    FileNames[0xc];                         // +0x28 vector<CWideString,std::allocator<CWideString>_>
    unsigned char    FileTimes[0xc];                         // +0x34 vector<CDateAndTime,std::allocator<CDateAndTime>_>
    unsigned char    PrimitiveHandles[0xc];                  // +0x40 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char    MenuPrimitiveHandles[0xc];              // +0x4c vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char    Components[0xc];                        // +0x58 vector<NUISystem::CComponent_*,std::allocator<NUISystem::CComponent_*>_>
    long             LoadGameIndex;                          // +0x64
    double           CurStartPauseTime;                      // +0x68
    double           AccumulativePausedTime;                 // +0x70
    unsigned char    FileRegionNames[0xc];                   // +0x78 vector<CCharString,std::allocator<CCharString>_>
    unsigned char    FileRegionMiniMapNames[0xc];            // +0x84 vector<CCharString,std::allocator<CCharString>_>
    unsigned char    FilePlayTimes[0xc];                     // +0x90 vector<long,std::allocator<long>_>
    unsigned char    _pad_0x9c[0x4];                         // +0x9c
    double           LastRenderTime;                         // +0xa0
    bool             OpenInventory;                          // +0xa8
    unsigned char    _pad_0xa9[0x3];                         // +0xa9
    CYesNo*          m_pOKDialog;                            // +0xac
    CYesNo*          PVideoOptionsNotAcceptedDialog;         // +0xb0
    CYesNo*          PVideoOptionsChangedDialog;             // +0xb4
    long             SaveGameToLoad;                         // +0xb8
    unsigned char    _pad_0xbc[0x4];                         // +0xbc
    double           VideoChangesTimer;                      // +0xc0
    long             SaveGameIndex;                          // +0xc8
    long             FramesToWaitToSave;                     // +0xcc
    CComponent*      PSaveInProgress;                        // +0xd0
    unsigned char    _pad_0xd4[0x4];                         // +0xd4
    double           SaveInProgressTime;                     // +0xd8
    unsigned char    Signatures[0xc];                        // +0xe0 map<CWideString,bool,std::less<CWideString>,std::allocator<std::pair<CWideString_const_,bool>_>_>
    CComponent*      PMousePointer;                          // +0xec
    CYesNo*          PYesNo;                                 // +0xf0
    unsigned char    _pad_0xf4[0x4];                         // +0xf4
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCInGameMenu) == 0xf8);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, field4) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, InGameMenuOpen) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, PDef_Object) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, GlobalDefIndex) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, ActiveScreen) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, ItemCount) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, ItemHighlightIndex) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, ItemTopIndex) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, FileNames) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, FileTimes) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, PrimitiveHandles) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, MenuPrimitiveHandles) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, Components) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, LoadGameIndex) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, CurStartPauseTime) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, AccumulativePausedTime) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, FileRegionNames) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, FileRegionMiniMapNames) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, FilePlayTimes) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, LastRenderTime) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, OpenInventory) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, m_pOKDialog) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, PVideoOptionsNotAcceptedDialog) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, PVideoOptionsChangedDialog) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, SaveGameToLoad) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, VideoChangesTimer) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, SaveGameIndex) == 0xc8);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, FramesToWaitToSave) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, PSaveInProgress) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, SaveInProgressTime) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, Signatures) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, PMousePointer) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CTCInGameMenu, PYesNo) == 0xf0);

#endif // FABLE_ENGINE_CTCINGAMEMENU_H
#endif
