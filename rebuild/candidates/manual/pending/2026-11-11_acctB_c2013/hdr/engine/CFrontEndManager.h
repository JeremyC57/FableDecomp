#ifdef FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#ifndef FABLE_ENGINE_CFRONTENDMANAGER_H
#define FABLE_ENGINE_CFRONTENDMANAGER_H
#include "engine/NUISystem_CComponent.h"
#include "engine/FRONTEND_SCREEN.h"
#include "engine/CDateAndTime.h"
#include "stlport/fable_deque.h"
class CNewFrontendGameComponent;class CVirtualKeyboard;
namespace NUISystem {
class CFrontEndManager:public CObserver {
public:
 CFrontEndManager();
 virtual ~CFrontEndManager();
 virtual void Action(CCountedPointer<CAction>&);
 static CFrontEndManager* __fastcall GetInstance();
 virtual void ProcessEvent(EEvent);
 void CloseVirtualKeyboard(bool);
#ifdef FABLE_RETAIL_CFRONTENDMANAGER_SAVEDGAMES
 // Opt-in (chunk 2003, row 0x00598463 RefreshAvailableSavedGamesForProfile): Ego_r.pdb / FableWin.pdb spellings of the members that
 // row calls, all out of line in retail (reloc_bindings.tsv; the Ego_r donor order GetProfileNames, ConstructFileDescription, ...,
 // GetSavedGameNames, RefreshAvailableSavedGamesForProfile matches retail's). Declarations only.
 CComponent* CreateComponent(const CCharString&);                                                       // 0x00594F94
 CComponent* CreateFrontEndButtonComponent(CWideString&,float,const CCharString&,const CCharString&,bool); // 0x00596A68
 bool VerifySignatureCached(const CWideString&);                                                         // 0x005957D9
protected:
 void ConstructFileDescription(unsigned long,CComponent*,CComponent*,CWideString&);                      // 0x00595CC1
 void GetSavedGameNames(std::vector<CWideString>&);                                                      // 0x00597DF7
#ifdef FABLE_RETAIL_CFRONTENDMANAGER_ADDREGION
 // Nested opt-in (chunk 2013, row 0x00597DF7): the protected AddRegionAndTimeInfo of the FABLE_RETAIL_CFRONTENDMANAGER branch,
 // declared in the full-layout branch too (Ego_r / FableWin ?AddRegionAndTimeInfo@CFrontEndManager@NUISystem@@IAEXABVCWideString@@@Z).
 void AddRegionAndTimeInfo(const CWideString&);                                                          // 0x00597228
#endif
 long RefreshAvailableSavedGamesForProfile();                                                            // 0x00598463
public:
#endif
protected:
 CWideString MediaPlayerErrorString;
 std::list<CActionParam*> UIActionParams;
 CNewFrontendGameComponent* PGameComponent;
 static CFrontEndManager* m_pInstance;
 std::stack<CComponent*> m_stackScreens;
 std::vector<CComponent*> m_vComponents;
 std::map<FRONTEND_SCREEN,CComponent*> m_mapScreens;
 CVirtualKeyboard* m_pVirtualKeyboard;
 bool m_bStartNewGame;
 std::vector<CDateAndTime> m_vFileTimes;
 std::vector<CCharString> FileRegionNames,FileRegionMiniMapNames;
 std::vector<long> FilePlayTimes;
 CComponent* PScreenToDeactivate;CComponent* PScreenToActivate;
 bool GoToKeyboard;
 CComponent* PScreenToDeactivateForStartGame;CComponent* PScreenToDeactivateForLoadGame;
 CWideString GameToLoad,ProfileToDelete;
 unsigned long BlocksOnUNeeded,BlocksOnTNeeded,NumberSaveGames;
 bool m_bQuitFromGame;
 std::map<CWideString,bool> Signatures;
 double InstallationPauseStartTime;
 bool InstallationPaused,FinalInstallationNeverPause;
};
FABLE_STATIC_ASSERT(sizeof(CFrontEndManager)==224);
}
#endif
#else
#ifdef FABLE_RETAIL_CFRONTENDMANAGER
#ifndef FABLE_ENGINE_CFRONTENDMANAGER_H
#define FABLE_ENGINE_CFRONTENDMANAGER_H
// Retail opt-in (chunk 735, row 0x00597228 AddRegionAndTimeInfo): the PDB class NUISystem::CFrontEndManager (class key V; RTTI
// .?AVCFrontEndManager@NUISystem@@ with the single base NUISystem::CObserver at +0, rtti_bases.tsv; vtable 0x012521A8) at the
// retail STLport offsets of the generated layout below: FileRegionNames +0x74, FileRegionMiniMapNames +0x80 (vector<CCharString>),
// FilePlayTimes +0x8C (vector<long>), sizeof 0xE0. ?VerifySignatureCached@CFrontEndManager@NUISystem@@QAE_NABVCWideString@@@Z
// (0x005957D9, landed) and the protected ?AddRegionAndTimeInfo@CFrontEndManager@NUISystem@@IAEXABVCWideString@@@Z (Ego_r +
// FableWin). Everything else stays opaque; the global `CFrontEndManager` layout of the default path is unchanged.
#include "engine/NUISystem_CComponent.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
namespace NUISystem {
class CFrontEndManager : public CObserver {
public:
    bool VerifySignatureCached(const CWideString&);                  // 0x005957D9
    // Chunk 774 (bake 0x0042DF9E CNewFrontendGameComponent::Draw): Ego_r.pdb ?GetInstance@CFrontEndManager@NUISystem@@SIPAV12@XZ and
    // ?Draw@CFrontEndManager@NUISystem@@QAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@@Z, both out of line. Declarations only.
    static CFrontEndManager* __fastcall GetInstance();                 // 0x00595582
    void Draw(CIEngine&, CEnginePrimitiveHandle*);                     // 0x00595222
protected:
    void AddRegionAndTimeInfo(const CWideString&);                   // 0x00597228
    unsigned char _pad_0x14[0x74 - 0x14];                            // +0x14 MediaPlayerErrorString .. m_vFileTimes (generated layout below)
    std::vector<CCharString> FileRegionNames;                        // +0x74
    std::vector<CCharString> FileRegionMiniMapNames;                 // +0x80
    std::vector<long>        FilePlayTimes;                          // +0x8c
    unsigned char _pad_0x98[0xe0 - 0x98];                            // +0x98 PScreenToDeactivate .. FinalInstallationNeverPause
};
FABLE_STATIC_ASSERT(sizeof(CFrontEndManager) == 0xe0);
}
#endif
#else
#ifndef FABLE_ENGINE_CFRONTENDMANAGER_H
#define FABLE_ENGINE_CFRONTENDMANAGER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CFrontEndManager = 0xf8.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CFrontEndManager = 0xe0.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

struct CComponent;
class CNewFrontendGameComponent;
struct CVirtualKeyboard;
class CWideStringData;

#pragma pack(push, 1)
struct CFrontEndManager {
    void*                      __vftable;                    // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char              _pad_0x04[0xc];               // +0x04
    unsigned char              _pad_0x10[0x1];               // +0x10
    unsigned char              _pad_0x11[0x3];               // +0x11
    CWideStringData*           MediaPlayerErrorString_PStringData; // +0x14 CWideString::PStringData
    unsigned char              UIActionParams[0x4];          // +0x18 list<NUISystem::CActionParam_*,std::allocator<NUISystem::CActionParam_*>_>
    CNewFrontendGameComponent* PGameComponent;               // +0x1c
    unsigned char              m_stackScreens[0x28];         // +0x20 stack<NUISystem::CComponent_*,std::deque<NUISystem::CComponent_*,std::allocator<NUISystem::CComponent_*>_>_>
    unsigned char              m_vComponents[0xc];           // +0x48 vector<NUISystem::CComponent_*,std::allocator<NUISystem::CComponent_*>_>
    unsigned char              m_mapScreens[0xc];            // +0x54 map<FRONTEND_SCREEN,NUISystem::CComponent_*,std::less<FRONTEND_SCREEN>,std::allocator<std::pair<FRONTEND_SCREEN_const_,NUISystem::CComponent_*>_>_>
    CVirtualKeyboard*          m_pVirtualKeyboard;           // +0x60
    bool                       m_bStartNewGame;              // +0x64
    unsigned char              _pad_0x65[0x1];               // +0x65
    unsigned char              _pad_0x66[0x2];               // +0x66
    unsigned char              m_vFileTimes[0xc];            // +0x68 vector<CDateAndTime,std::allocator<CDateAndTime>_>
    unsigned char              FileRegionNames[0xc];         // +0x74 vector<CCharString,std::allocator<CCharString>_>
    unsigned char              FileRegionMiniMapNames[0xc];  // +0x80 vector<CCharString,std::allocator<CCharString>_>
    unsigned char              FilePlayTimes[0xc];           // +0x8c vector<long,std::allocator<long>_>
    CComponent*                PScreenToDeactivate;          // +0x98
    CComponent*                PScreenToActivate;            // +0x9c
    bool                       GoToKeyboard;                 // +0xa0
    unsigned char              _pad_0xa1[0x3];               // +0xa1
    CComponent*                PScreenToDeactivateForStartGame; // +0xa4
    CComponent*                PScreenToDeactivateForLoadGame; // +0xa8
    CWideStringData*           GameToLoad_PStringData;       // +0xac CWideString::PStringData
    CWideStringData*           ProfileToDelete_PStringData;  // +0xb0 CWideString::PStringData
    unsigned long              BlocksOnUNeeded;              // +0xb4
    unsigned long              BlocksOnTNeeded;              // +0xb8
    unsigned long              NumberSaveGames;              // +0xbc
    bool                       m_bQuitFromGame;              // +0xc0
    unsigned char              _pad_0xc1[0x1];               // +0xc1
    unsigned char              _pad_0xc2[0x2];               // +0xc2
    unsigned char              Signatures[0xc];              // +0xc4 map<CWideString,bool,std::less<CWideString>,std::allocator<std::pair<CWideString_const_,bool>_>_>
    double                     InstallationPauseStartTime;   // +0xd0
    bool                       InstallationPaused;           // +0xd8
    bool                       FinalInstallationNeverPause;  // +0xd9
    unsigned char              _pad_0xda[0x6];               // +0xda
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CFrontEndManager) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, MediaPlayerErrorString_PStringData) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, UIActionParams) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, PGameComponent) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, m_stackScreens) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, m_vComponents) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, m_mapScreens) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, m_pVirtualKeyboard) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, m_bStartNewGame) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, m_vFileTimes) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, FileRegionNames) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, FileRegionMiniMapNames) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, FilePlayTimes) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, PScreenToDeactivate) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, PScreenToActivate) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, GoToKeyboard) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, PScreenToDeactivateForStartGame) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, PScreenToDeactivateForLoadGame) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, GameToLoad_PStringData) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, ProfileToDelete_PStringData) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, BlocksOnUNeeded) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, BlocksOnTNeeded) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, NumberSaveGames) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, m_bQuitFromGame) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, Signatures) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, InstallationPauseStartTime) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, InstallationPaused) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CFrontEndManager, FinalInstallationNeverPause) == 0xd9);

#endif // FABLE_ENGINE_CFRONTENDMANAGER_H
#endif

#endif
