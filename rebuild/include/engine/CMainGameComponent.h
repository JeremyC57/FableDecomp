#ifndef FABLE_ENGINE_CMAINGAMECOMPONENT_H
#define FABLE_ENGINE_CMAINGAMECOMPONENT_H
// Retail class (header wave 2026-11-03, from Codex pass 12): base CGameComponent (0x10), members in Ego_r.pdb order and
// spelling (ghidra_out/struct_layouts_egor.tsv) with retail sizes; sizeof 0x161E8 (caller 0x0042F4B1 allocates 0x161E8 before
// the ctor 0x00418DCA; the ctor stores the members through +0x161E4; Ego_r's 0x38 CFrameRateSmoother is 0x30 in retail).
// Statics: ?PMainGameComponent@CMainGameComponent@@1PAV1@A and ?ConstantFPS@CMainGameComponent@@1JA 0x01375550 (protected),
// read through the inline public accessors Get() / GetConstantFPS() (Ego_r + FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ).
// Members whose classes have no shared header are opaque `unsigned char X[N]` carrying their real type in a comment.
#include "engine/CGameComponent.h"
class CGame;
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT
class CGameDefinitionManager;
#endif
class CASoundBank;
#if defined(FABLE_RETAIL_CMAINGAMECOMPONENT) && defined(FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER)
class CPlayerManager;
#endif
#if defined(FABLE_RETAIL_CMAINGAMECOMPONENT) && defined(FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD)
class CWorld;
#endif
class CMainGameComponentInit;
class CMainGameComponent : public CGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;
    static long ConstantFPS;
    CASoundBank* PSampleBank;                       // +0x10
    unsigned char PTextBank[8];                     // +0x14 CCountedPointer<NGameText::CDataBank>
    unsigned char PPlayerManager[4];                // +0x1C boost::scoped_ptr<CPlayerManager>
    unsigned char PPlayerInterface[4];              // +0x20 boost::scoped_ptr<CGamePlayerInterface>
    unsigned char PWorld[4];                        // +0x24 boost::scoped_ptr<CWorld>
    unsigned char PDisplayEngine[4];                // +0x28 boost::scoped_ptr<CDisplayEngine>
    unsigned char PLUA[8];                          // +0x2C CCountedPointer<CLUA>
    bool ForceUpdateTick;                           // +0x34
    float ForceUpdateTickSpeedMultipler;            // +0x38
    float ForceUpdateTickSpeedDesiredFramerate;     // +0x3C
    long ForceUpdateNoFailedUpdates;                // +0x40
    bool FirstWorldFrameUpdate;                     // +0x44
    long CurrentServerFrame;                        // +0x48
    long InputServerFrame;                          // +0x4C
    long LastGameTurnForceRendered;                 // +0x50
    double CurrentFrameStartGameTime;               // +0x58
    double GameStartTime;                           // +0x60
    double LastFrameRenderDuration;                 // +0x68
    unsigned char LastInterpolationInfo[0x34];      // +0x70 CInterpolationInfo
    unsigned char EventPackageSet[0x13A14];         // +0xA4 CGameEventPackageSet
    unsigned char Client[0x2680];                   // +0x13AB8 CNetworkClient
    unsigned long NoRenderFramesSinceLastGameUpdate; // +0x16138
    unsigned long WorldSeed;                        // +0x1613C
    unsigned long LocalSeed;                        // +0x16140
    unsigned char PDebugFont[8];                    // +0x16144 CCountedPointer<CFontBank>
    unsigned char PCutSceneMainFont[8];             // +0x1614C CCountedPointer<CFontBank>
    unsigned char EventPackageFile[0x1C];           // +0x16154 CDiskFileWin32
    bool LoadingEventPackages;                      // +0x16170
    bool SavingEventPackages;                       // +0x16171
    unsigned char _pad_0x16172[2];
    unsigned char EventPackageFileHeader[4];        // +0x16174 CEventPackageFileHeader
    unsigned char FrameRateSmoother[0x30];          // +0x16178 CFrameRateSmoother
    double LastRenderFrameStartTime;                // +0x161A8
    float TimePassedSinceLastUpdate;                // +0x161B0
    float LastUpdateTime;                           // +0x161B4
    bool WorldUpdateTurn;                           // +0x161B8
    unsigned char _pad_0x161B9[3];
    unsigned char RoughFPSCounter[8];               // +0x161BC CRoughFrameCounter
    CGameComponent* NextComponentToRun;             // +0x161C4
    unsigned char PMainGraphicBank[8];              // +0x161C8 CCountedPointer<CGraphicDataBank>
    unsigned char InitStructure[0x10];              // +0x161D0 CMainGameComponentInit
    bool Initialised;                               // +0x161E0
    bool AllowRender;                               // +0x161E1
    bool Rendered;                                  // +0x161E2
    long DebugNoFramesUnableToRender;               // +0x161E4
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
    static long __fastcall GetConstantFPS() { return ConstantFPS; }
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT
    // Retail opt-in (chunk 728, row 0x0074B770): Ego_r.pdb (0x00415162) / FableWin.pdb
    // ?PeekDefinitionManager@CMainGameComponent@@QBEABVCGameDefinitionManager@@XZ, out of line in retail (the jmp thunk 0x00415FBC
    // every caller calls, the only spelling bound there). A declaration only: no layout or vtable change.
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x00415FBC
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
    // Nested opt-in (chunk 755, row 0x00775B40 CTCShop::FrameUpdate): FableWin.pdb ?PeekPlayerManager@CMainGameComponent@@QBEABVCPlayerManager@@XZ
    // (0x018846C0; inline in Ego_r and retail: the caller reads the scoped_ptr word +0x1C in place, 0x00775B4B). PPlayerManager
    // stays opaque here (boost::scoped_ptr<CPlayerManager>, one pointer word), read as that word.
    const CPlayerManager& PeekPlayerManager() const { return **reinterpret_cast<CPlayerManager* const*>(PPlayerManager); }
#endif
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
    // Nested opt-in (chunk 755, row 0x0090C540 CAIStateGroup_MinionGuardFollowPlayer::Update): FableWin.pdb
    // ?PeekWorld@CMainGameComponent@@QBEABVCWorld@@XZ (0x0187EFF0; inline in Ego_r and retail: the caller reads the scoped_ptr word
    // +0x24 in place, 0x0090C55D). PWorld stays opaque here (boost::scoped_ptr<CWorld>, one pointer word), read as that word.
    const CWorld& PeekWorld() const { return **reinterpret_cast<CWorld* const*>(PWorld); }
#endif
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_WORLDFRAME
    // Nested opt-in (chunk 755, row 0x008AF500 CombatDialogueManager::AttemptToSpeak): Ego_r.pdb
    // ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ, out of line in retail at 0x004167C8 (landed rows 0x008AF5F0 / 0x00751AD0).
    long GetCurrentWorldFrame() const;                             // 0x004167C8
#endif
#endif
    CMainGameComponent(CGame&,const CMainGameComponentInit&);
    virtual ~CMainGameComponent();
    virtual bool Run(CGameComponent**);
    virtual void ChangeTextureColourDepth(long);
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
};
FABLE_STATIC_ASSERT(sizeof(CMainGameComponent)==0x161E8);
#endif
