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
class CASoundBank; class CDisplayEngine; class CGamePlayerInterface; class CLUA; class CPlayerManager; class CWorld;
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
    unsigned char _pad_0x70[0x16178]; // All original members LastInterpolationInfo through DebugNoFramesUnableToRender.
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // FableWin.pdb ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
    CMainGameComponent(CGame&,const CMainGameComponentInit&);
    static void FABLE_FASTCALL GenerateMetFilesFromLugFiles();
    virtual ~CMainGameComponent();
    virtual bool Run(CGameComponent**);
    virtual void ChangeTextureColourDepth(long);
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
    CPlayerManager& GetPlayerManager() { return *PPlayerManager; }
    const NGameText::CDataBank& GetTextBank() const { return *PTextBank; }   // FableWin ?GetTextBank@CMainGameComponent@@QBEABVCDataBank@NGameText@@XZ (inline; 0x006286F0 reads PTextBank +0x14 in place)
    const CWorld& PeekWorld() const { return *PWorld; }
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
