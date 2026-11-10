#pragma once

#include "fable_string.h"
#include "rebuild_abi.h"

class CGame;

#ifdef FABLE_RETAIL_CGAMECOMPONENT
// Retail opt-in (Codex pass 11 consolidation 2, merged with retry11a rows 0x00520670 / 0x00521620): the retail CGameComponent.
// RTTI CGameComponent : CBaseClassNonCopyable (vptr +0), CDeviceResetCallback (vptr +4); Ego_r.pdb CGameComponent (16 bytes):
// Quit +0x08, Running +0x09, Game +0x0C (`CGame *`; the layout dump renders no reference member as a pointer). Member set and
// order: Ego_r.pdb. CMainGameComponent (below, FABLE_RETAIL_CMAINGAMECOMPONENT) derives from this model. The frontend
// stand-ins further down assume the 4-byte stand-in base, so they are left out on this path.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CDeviceResetCallback.h"
class CGameComponent : public CBaseClassNonCopyable, public CDeviceResetCallback {
public:
    CGameComponent(CGame&);
    virtual ~CGameComponent();
    virtual void Init();
    virtual bool Run(CGameComponent**)=0;
    virtual void ChangeTextureColourDepth(long)=0;
protected:
    virtual void SetQuit();
    bool Quit;                                                           // +0x08
    bool Running;                                                        // +0x09
    CGame* Game;                                                         // +0x0C
};
FABLE_STATIC_ASSERT(sizeof(CGameComponent) == 0x10);
#else
class CGameComponent
{
public:
    virtual ~CGameComponent();
    virtual void Init();
    virtual bool Run(CGameComponent** nextComponent);
};
#endif // FABLE_RETAIL_CGAMECOMPONENT

class CMainGameComponentInit
{
public:
    CWideString firstWideString;
    CWideString secondWideString;
    CCharString narrowString;
    CWideString fourthWideString;
};

class CFrontendGameComponentInit
{ public:
    fable_u32 value;
};

typedef CFrontendGameComponentInit CNewFrontendGameComponentInit;

#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT
// Retail opt-in (Codex pass 11 consolidation 2, merged with retry11a rows 0x00520670 / 0x00521620): the retail
// CMainGameComponent : CGameComponent (needs FABLE_RETAIL_CGAMECOMPONENT and, for the scoped_ptr accessors,
// FABLE_RETAIL_SCOPED_PTR). Members +0x10..+0x68: Ego_r.pdb CMainGameComponent, typed as the PDB types them
// (CCountedPointer 8 bytes, boost::scoped_ptr 4 bytes); everything from LastInterpolationInfo on is opaque. sizeof 0x161E8:
// caller 0x0042F4B1 allocates 0x161E8 before the constructor 0x00418DCA. PMainGameComponent 0x013B86A0 (ESTABLISHED_SPELLINGS).
// GenerateMetFilesFromLugFiles (static, 0x00418C3B): the stand-in declares it; kept so neither path loses a member.
#include "boost/scoped_ptr.h"
#include "engine/CCountedPointer.h"
class CASoundBank; class CDisplayEngine; class CGamePlayerInterface; class CLUA; class CPlayerManager; class CWorld; class CFontBank;
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_ENGINEGRAPHICS
class CGraphicDataBank;
#endif
namespace NGameText { class CDataBank; }
class CMainGameComponent : public CGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;
    // ConstantFPS 0x01375550 (2026-11-03, row 0x005EB9E0): the protected static long ?ConstantFPS@CMainGameComponent@@1JA
    // (NAME_FIXES_QUEUE codex50 / c654 / cg10: Ego_r static member, read only through the inline static accessor
    // ?GetConstantFPS@CMainGameComponent@@SIJXZ in FableWin.pdb; retail `fild dword ptr [01375550h]` in place).
    static long ConstantFPS;
    CASoundBank* PSampleBank;                                            // +0x10
    CCountedPointer<NGameText::CDataBank> PTextBank;                     // +0x14
    boost::scoped_ptr<CPlayerManager> PPlayerManager;                    // +0x1C
    boost::scoped_ptr<CGamePlayerInterface> PPlayerInterface;            // +0x20
    boost::scoped_ptr<CWorld> PWorld;                                    // +0x24
    boost::scoped_ptr<CDisplayEngine> PDisplayEngine;                    // +0x28
    CCountedPointer<CLUA> PLUA;                                          // +0x2C
    bool ForceUpdateTick;                                                // +0x34
    float ForceUpdateTickSpeedMultipler;                                 // +0x38
    float ForceUpdateTickSpeedDesiredFramerate;                          // +0x3C
    long ForceUpdateNoFailedUpdates;                                     // +0x40
    bool FirstWorldFrameUpdate;                                          // +0x44
    long CurrentServerFrame;                                             // +0x48
    long InputServerFrame;                                               // +0x4C
    long LastGameTurnForceRendered;                                      // +0x50
    double CurrentFrameStartGameTime;                                    // +0x58
    double GameStartTime;                                                // +0x60
    double LastFrameRenderDuration;                                      // +0x68
    unsigned char _pad_0x70[0x160D4]; // Original members LastInterpolationInfo through LocalSeed.
    // Ego_r.pdb font pointers typed (chunk 728 round 2 / chunk 732): PDebugFont +0x16144 and PCutSceneMainFont +0x1614C, both
    // CCountedPointer<CFontBank> (same offsets as engine/CMainGameComponent.h; layout and size unchanged).
    CCountedPointer<CFontBank> PDebugFont;                               // +0x16144
    CCountedPointer<CFontBank> PCutSceneMainFont;                        // +0x1614C
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_ENGINEGRAPHICS
    // Nested opt-in (chunk 1011, row 0x005FBAA0): PMainGraphicBank, the CCountedPointer<CGraphicDataBank> at +0x161C8 (engine/CMainGameComponent.h
    // and the landed ctor 0x00418B7A assert that offset), read through FableWin.pdb ?PeekEngineGraphics@CMainGameComponent@@QBEABVCGraphicDataBank@@XZ
    // (0x01ACCBD0; inline in retail: `mov eax,[ecx+161C8h]` at 0x005FBC51). Same size: the opaque run is split around it.
    unsigned char _pad_0x16154[0x74]; // Original members EventPackageFile through the member before PMainGraphicBank.
    CCountedPointer<CGraphicDataBank> PMainGraphicBank;                  // +0x161C8
    unsigned char _pad_0x161d0[0x18]; // Original members after PMainGraphicBank through DebugNoFramesUnableToRender.
public:
    const CGraphicDataBank& PeekEngineGraphics() const { return *PMainGraphicBank; }
protected:
#else
    unsigned char _pad_0x16154[0x94]; // Original members EventPackageFile through DebugNoFramesUnableToRender.
#endif
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // FableWin.pdb ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
    // Retail opt-in (chunk 718, row 0x0089E190): Ego_r.pdb / FableWin.pdb ?ConvertSecondsToWF@CMainGameComponent@@SIJM@Z, expanded
    // in place in retail (`fild dword ptr [ConstantFPS] ; fmul ; call __ftol2`, as the landed CameraMoveBetweenLookingAt 0x00892530).
    static long __fastcall ConvertSecondsToWF(float seconds) { return (long)(GetConstantFPS() * seconds); }
    CMainGameComponent(CGame&,const CMainGameComponentInit&);
    static void FABLE_FASTCALL GenerateMetFilesFromLugFiles();
    virtual ~CMainGameComponent();
    virtual bool Run(CGameComponent**);
    virtual void ChangeTextureColourDepth(long);
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
    virtual void Init();                                                  // ?Init@CMainGameComponent@@UAEXXZ (Ego_r + FableWin; slot 1, 0x004184BD)
    virtual void SetQuit();                                               // ?SetQuit@CMainGameComponent@@UAEXXZ (FableWin; slot 4)
    // New virtuals, retail vtable 0x0122F180 slots 5..9 in PDB order (2026-11-03, row 0x00522720 calls slot 9):
protected:
    virtual void Update();                                                // slot 5 0x00418289 ?Update@CMainGameComponent@@MAEXXZ (Ego_r + FableWin)
    virtual void GetInputs();                                             // slot 6 0x00416E78 (Ego_r + FableWin)
    virtual void Render();                                                // slot 7 0x00417001 (Ego_r + FableWin)
    virtual void PostInit();                                              // slot 8 0x00416953 (Ego_r + FableWin)
public:
    virtual bool IsEditorActive() const;                                  // slot 9 0x004197B0 ?IsEditorActive@CMainGameComponent@@UBE_NXZ (FableWin 0x018A1210)
    CPlayerManager& GetPlayerManager() { return *PPlayerManager; }
    // Chunk 734 (row 0x004A5A40 CWorld::UpdateFrame): FableWin.pdb ?GetDisplayEngine@CMainGameComponent@@QAEAAVCDisplayEngine@@XZ, inline in
    // retail (`mov eax,[esi+8] ; mov ecx,[eax+28h]` at 0x004A5C34).
    CDisplayEngine& GetDisplayEngine() { return *PDisplayEngine; }
    const NGameText::CDataBank& GetTextBank() const { return *PTextBank; }   // FableWin ?GetTextBank@CMainGameComponent@@QBEABVCDataBank@NGameText@@XZ (inline; 0x006286F0 reads PTextBank +0x14 in place)
    const CWorld& PeekWorld() const { return *PWorld; }
    // Chunk 732 (row 0x006AA2E0): FableWin.pdb ?PeekDebugFont@CMainGameComponent@@QBEABVCFontBank@@XZ (0x018A3AC0), inline in retail
    // (`mov eax,[eax+16144h]` straight after PeekComponent).
    const CFontBank& PeekDebugFont() const { return *PDebugFont; }
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_CUTSCENEFONT
    // Nested opt-in (chunk 1011, row 0x005EB180 CTCTavernGame::SetStateAfterInstructions): FableWin.pdb
    // ?PeekCutSceneMainFont@CMainGameComponent@@QBEABVCFontBank@@XZ, inline in retail (`mov ecx,[013B86A0h] ; mov esi,[ecx+1614Ch]`
    // at 0x005EB276; engine/CMainGameComponent.h carries the same accessor for its own layout). Declaration-level only.
    const CFontBank& PeekCutSceneMainFont() const { return *PCutSceneMainFont; }
#endif
};
#else
class CMainGameComponent : public CGameComponent
{
public:
    static void FABLE_FASTCALL GenerateMetFilesFromLugFiles();

    CMainGameComponent(
        CGame& game,
        const CMainGameComponentInit& init);

private:
    fable_u8 unknown04_[0x161E4];
};
#endif // FABLE_RETAIL_CMAINGAMECOMPONENT

#ifndef FABLE_RETAIL_CGAMECOMPONENT
class CFrontendGameComponent : public CGameComponent
{
public:
    CFrontendGameComponent(
        CGame& game,
        const CFrontendGameComponentInit& init);

private:
    fable_u8 unknown04_[0x1E5C];
};

class CNewFrontendGameComponent : public CGameComponent
{
public:
    CNewFrontendGameComponent(
        CGame& game,
        const CNewFrontendGameComponentInit& init);

    static void CompileDefs();

private:
    fable_u8 unknown04_[0x144];
};
#endif // !FABLE_RETAIL_CGAMECOMPONENT

class CGame
{
public:
    void Play();

private:
    fable_u8 unknown00_[8];
    CGameComponent* component_;
    fable_u8 unknown0C_[0x200];
    bool quit_;
};

extern fable_u8 g_FableCompileFrontendDefinitions_013B8648;
extern fable_u8 g_FableStartMainGame_013B8605;
extern fable_u8 g_FableUseLegacyFrontend_013B8642;
extern CGameComponent* g_FableRetiredGameComponent_013B7D58;
extern CWideString g_FableMainGameStartupPath_013B7D5C;

FABLE_STATIC_ASSERT(sizeof(CMainGameComponentInit) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CFrontendGameComponentInit) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CMainGameComponent) == 0x161E8);
#ifndef FABLE_RETAIL_CGAMECOMPONENT
FABLE_STATIC_ASSERT(sizeof(CFrontendGameComponent) == 0x1E60);
FABLE_STATIC_ASSERT(sizeof(CNewFrontendGameComponent) == 0x148);
#endif
FABLE_STATIC_ASSERT(sizeof(CGame) == 0x210);
