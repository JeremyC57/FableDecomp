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
#if defined(FABLE_RETAIL_CMAINGAMECOMPONENT) && defined(FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE)
class CDisplayEngine;
#endif
#if defined(FABLE_RETAIL_CMAINGAMECOMPONENT) && defined(FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK)
namespace NGameText { class CDataBank; }
class CFontBank;
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
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
    // Nested opt-in (chunk 882, row 0x0061E6D0): FableWin.pdb ?GetWorld@CMainGameComponent@@QAEAAVCWorld@@XZ (0x018A3A80), the non-const
    // twin of PeekWorld; inline in retail (0x0061EEC8: `mov edx,[0x013B86A0] ; mov ecx,[edx+24h]` then CWorld slot 12 GetWorldMap).
    CWorld& GetWorld() { return **reinterpret_cast<CWorld* const*>(PWorld); }
#endif
#endif
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
    // Retail opt-in (chunk 795, row 0x0092FAB0 CAIStateGroup_MinionReactToPlayerInteraction::TestOrDoInteraction): Ego_r.pdb
    // ?GetTextBank@CMainGameComponent@@QBEABVCDataBank@NGameText@@XZ and ?PeekCutSceneMainFont@CMainGameComponent@@QBEABVCFontBank@@XZ,
    // inline in retail (the caller reads the counted pointers' object words +0x14 / +0x1614C in place, 0x0093033B / 0x00930372; as the
    // landed CTCChest::DisplayLockedMessage 0x00750110 model). The members stay opaque (CCountedPointer: object word first).
    const NGameText::CDataBank& GetTextBank() const { return **reinterpret_cast<NGameText::CDataBank* const*>(PTextBank); }
    const CFontBank& PeekCutSceneMainFont() const { return **reinterpret_cast<CFontBank* const*>(PCutSceneMainFont); }
#endif
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
    // Retail opt-in (chunk 795, row 0x005E78F0 CTCQuestCompletionUI::GlobalUpdate): FableWin.pdb
    // ?GetDisplayEngine@CMainGameComponent@@QAEAAVCDisplayEngine@@XZ, inline in retail (the caller reads the scoped_ptr word +0x28 in
    // place, 0x005E790B). PDisplayEngine stays opaque (boost::scoped_ptr<CDisplayEngine>, one pointer word), read as that word.
    CDisplayEngine& GetDisplayEngine() { return **reinterpret_cast<CDisplayEngine* const*>(PDisplayEngine); }
#endif
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_WORLDFRAME
    // Nested opt-in (chunk 755, row 0x008AF500 CombatDialogueManager::AttemptToSpeak): Ego_r.pdb
    // ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ, out of line in retail at 0x004167C8 (landed rows 0x008AF5F0 / 0x00751AD0).
    long GetCurrentWorldFrame() const;                             // 0x004167C8
#endif
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_GAMETIME
    // Nested opt-in (chunk 775, row 0x004167A7 GetFrameDifferenceFromCurrent): Ego_r.pdb / FableWin.pdb protected
    // ?GetCurrentGameTime@CMainGameComponent@@IBENXZ (out of line, 0x00416231: GFGetTime() - GameStartTime),
    // ?ConvertGTToWF@CMainGameComponent@@IBENN@Z (retail inlines it into every caller; its unreferenced out-of-line copy is
    // 0x0041623E: gameTime / (1.0f / ConstantFPS)) and ?GetFrameDifferenceFromCurrent@CMainGameComponent@@IBEJJ@Z.
protected:
    double GetCurrentGameTime() const;                             // 0x00416231
    double ConvertGTToWF(double gameTime) const { return gameTime / (1.0f / GetConstantFPS()); }
    long GetFrameDifferenceFromCurrent(long frame) const;          // 0x004167A7
public:
#endif
#endif
    CMainGameComponent(CGame&,const CMainGameComponentInit&);
    virtual ~CMainGameComponent();
    virtual bool Run(CGameComponent**);
    virtual void ChangeTextureColourDepth(long);
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
    // Nested opt-in (chunk 779, row 0x00837E40): the class's own virtuals after CGameComponent's five (vtable 0x0122F180: 0 dtor,
    // 1 Init 0x004184BD, 2 Run 0x004189C2, 3 ChangeTextureColourDepth 0x0041799B, 4 SetQuit 0x004197A0 `mov byte ptr [ecx+8],1`), in
    // slot order: 5 Update 0x00418289, 6 GetInputs 0x00416E78, 7 Render 0x00417001, 8 PostInit 0x00416953 (Ego_r.pdb protected
    // ?Update@ / ?GetInputs@ / ?Render@ / ?PostInit@CMainGameComponent@@MAEXXZ; ghidra_out/labels_rtti_port.tsv names 6-8) and
    // 9 IsEditorActive 0x004197B0 (`xor al,al ; ret`, Ego_r.pdb ?IsEditorActive@CMainGameComponent@@UBE_NXZ, the only bool const
    // virtual; CThingTrackNode::ConstructFromLevelScript 0x00837EED calls [eax+24h]). OnPre/OnPostDeviceReset above override the
    // CDeviceResetCallback vtable (second vptr). Declarations only: no layout change.
protected:
    virtual void Update();                              // slot 5 0x00418289
    virtual void GetInputs();                           // slot 6 0x00416E78
    virtual void Render();                              // slot 7 0x00417001
    virtual void PostInit();                            // slot 8 0x00416953
public:
    virtual bool IsEditorActive() const;                // slot 9 0x004197B0
#endif
};
FABLE_STATIC_ASSERT(sizeof(CMainGameComponent)==0x161E8);
#endif
