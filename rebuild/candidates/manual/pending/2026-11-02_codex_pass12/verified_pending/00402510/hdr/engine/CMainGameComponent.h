#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CMAINGAMECOMPONENT_H
#define FABLE_ENGINE_CMAINGAMECOMPONENT_H
// Original CGameComponent base and native PWorld+24; caller0042f4b1
// allocates0x161e8 before constructor00418dca. The unused remainder is opaque.
#include "engine/CGameComponent.h"
#include "engine/CWorld.h"
namespace NGameText { class CDataBank; }
class CGameDefinitionManager;class CASoundBank; class CMainGameComponentInit; class CDisplayEngine;
#if defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
class CPlayerManager;
#endif
class CMainGameComponent : public CGameComponent {
protected:
#if defined(FABLE_RETAIL_UI_FACTORY) || defined(FABLE_RETAIL_HIT_EFFECT) || defined(FABLE_RETAIL_ENTITY_PERSIST) || (defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE))
    static CMainGameComponent* PMainGameComponent;
#endif
#ifdef FABLE_RETAIL_CAMERA_DRAW
    static long ConstantFPS;
#endif
    CASoundBank* PSampleBank;
#ifdef FABLE_RETAIL_ITEMS_LIST
    CCountedPointer<NGameText::CDataBank> PTextBank;
#else
    unsigned char PTextBank[8]; // CCountedPointer<NGameText::CDataBank> +14
#endif
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    boost::scoped_ptr<CPlayerManager> PPlayerManager;
#else
    unsigned char PPlayerManager[4]; // boost::scoped_ptr<CPlayerManager> +1c
#endif
    unsigned char PPlayerInterface[4]; // boost::scoped_ptr<CGamePlayerInterface> +20
    boost::scoped_ptr<CWorld> PWorld;
    boost::scoped_ptr<CDisplayEngine> PDisplayEngine;
    unsigned char PLUA[8]; // CCountedPointer<CLUA> +2c
    bool ForceUpdateTick;
    float ForceUpdateTickSpeedMultipler;
    float ForceUpdateTickSpeedDesiredFramerate;
    long ForceUpdateNoFailedUpdates;
    bool FirstWorldFrameUpdate;
    long CurrentServerFrame;
    long InputServerFrame;
    long LastGameTurnForceRendered;
    double CurrentFrameStartGameTime;
    double GameStartTime;
    double LastFrameRenderDuration;
#ifdef FABLE_RETAIL_ITEMS_LIST
    // Layout: original Ego_r TPI1a143 names/order; native ctor00418dca..418eed
    // proves these member boundaries. FrameRateSmoother is native0x30, donor0x38.
    unsigned char LastInterpolationInfo[0x34]; // CInterpolationInfo +70; ctor415a60.
    unsigned char EventPackageSet[0x13a14]; // CGameEventPackageSet +a4; ctor9f1a80.
    unsigned char Client[0x2680]; // CNetworkClient +13ab8; ctor419323.
    unsigned long NoRenderFramesSinceLastGameUpdate; // +16138
    unsigned long WorldSeed; // +1613c
    unsigned long LocalSeed; // +16140
    unsigned char PDebugFont[8]; // CCountedPointer<CFontBank> +16144
    unsigned char PCutSceneMainFont[8]; // CCountedPointer<CFontBank> +1614c
    unsigned char EventPackageFile[0x1c]; // CDiskFileWin32 +16154; ctor415c30.
    bool LoadingEventPackages; // +16170
    bool SavingEventPackages; // +16171
    unsigned char _pad_0x16172[2];
    unsigned char EventPackageFileHeader[4]; // CEventPackageFileHeader +16174
    unsigned char FrameRateSmoother[0x30]; // CFrameRateSmoother +16178; ctor419747.
    double LastRenderFrameStartTime; // +161a8; native418e8f.
    float TimePassedSinceLastUpdate; // +161b0; between native-anchored a8 and b8.
    float LastUpdateTime; // +161b4; same original member order and natural sizes.
    bool WorldUpdateTurn; // +161b8; native418e95.
    unsigned char _pad_0x161b9[3];
    unsigned char RoughFPSCounter[8]; // CRoughFrameCounter +161bc; ctor9f8b10.
    CGameComponent* NextComponentToRun; // +161c4; native418eaa.
    unsigned char PMainGraphicBank[8]; // CCountedPointer<CGraphicDataBank> +161c8
    unsigned char InitStructure[0x10]; // CMainGameComponentInit +161d0; copyctor415e17.
    bool Initialised; // +161e0
    bool AllowRender; // +161e1
    bool Rendered; // +161e2
    unsigned char _pad_0x161e3;
    long DebugNoFramesUnableToRender; // +161e4; native418ee3.
#else
    unsigned char _pad_0x70[0x16178]; // All original members LastInterpolationInfo through DebugNoFramesUnableToRender.
#endif
public:
#ifdef FABLE_RETAIL_ITEMS_LIST
    const NGameText::CDataBank& GetTextBank()const{return *PTextBank;}
#endif
#if defined(FABLE_RETAIL_UI_FACTORY) || defined(FABLE_RETAIL_HIT_EFFECT) || defined(FABLE_RETAIL_ENTITY_PERSIST) || (defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE))
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
#endif
#ifdef FABLE_RETAIL_CAMERA_DRAW
    static long __fastcall GetConstantFPS() { return ConstantFPS; }
#endif
#ifdef FABLE_RETAIL_UI_FACTORY
    CDisplayEngine& GetDisplayEngine(){return *PDisplayEngine;}
#endif
    CMainGameComponent(CGame&,const CMainGameComponentInit&);
    virtual ~CMainGameComponent();
    virtual bool Run(CGameComponent**);
    virtual void ChangeTextureColourDepth(long);
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    CPlayerManager& GetPlayerManager(){return *PPlayerManager;}
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
    const CGameDefinitionManager& PeekDefinitionManager() const;
#endif
    const CWorld& PeekWorld() const { return *PWorld; }
};
FABLE_STATIC_ASSERT(sizeof(CMainGameComponent)==0x161e8);
#endif

#else
#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_CMAINGAMECOMPONENT_H
#define FABLE_ENGINE_CMAINGAMECOMPONENT_H
// Original CGameComponent base and native PWorld+24; caller0042f4b1
// allocates0x161e8 before constructor00418dca. The unused remainder is opaque.
#include "engine/CGameComponent.h"
#include "engine/CWorld.h"
class CGameDefinitionManager;class CASoundBank; class CMainGameComponentInit; class CDisplayEngine;
#if defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
class CPlayerManager;
#endif
class CMainGameComponent : public CGameComponent {
protected:
#if defined(FABLE_RETAIL_HIT_EFFECT) || defined(FABLE_RETAIL_ENTITY_PERSIST) || (defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE))
    static CMainGameComponent* PMainGameComponent;
#endif
#if defined(FABLE_RETAIL_CAMERA_DRAW) || defined(FABLE_RETAIL_WILL_BUTTON_TIMER)
    static long ConstantFPS;
#endif
#ifdef FABLE_RETAIL_SOUND_INIT
    void InitSound();
#endif
#ifdef FABLE_RETAIL_GFMAIN
public:static void __fastcall GenerateMetFilesFromLugFiles();
#endif
    CASoundBank* PSampleBank;
    unsigned char PTextBank[8]; // CCountedPointer<NGameText::CDataBank> +14
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    boost::scoped_ptr<CPlayerManager> PPlayerManager;
#else
    unsigned char PPlayerManager[4]; // boost::scoped_ptr<CPlayerManager> +1c
#endif
    unsigned char PPlayerInterface[4]; // boost::scoped_ptr<CGamePlayerInterface> +20
    boost::scoped_ptr<CWorld> PWorld;
    boost::scoped_ptr<CDisplayEngine> PDisplayEngine;
    unsigned char PLUA[8]; // CCountedPointer<CLUA> +2c
    bool ForceUpdateTick;
    float ForceUpdateTickSpeedMultipler;
    float ForceUpdateTickSpeedDesiredFramerate;
    long ForceUpdateNoFailedUpdates;
    bool FirstWorldFrameUpdate;
    long CurrentServerFrame;
    long InputServerFrame;
    long LastGameTurnForceRendered;
    double CurrentFrameStartGameTime;
    double GameStartTime;
    double LastFrameRenderDuration;
    unsigned char _pad_0x70[0x16178]; // All original members LastInterpolationInfo through DebugNoFramesUnableToRender.
public:
#if defined(FABLE_RETAIL_HIT_EFFECT) || defined(FABLE_RETAIL_ENTITY_PERSIST) || (defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE))
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
#endif
#if defined(FABLE_RETAIL_CAMERA_DRAW) || defined(FABLE_RETAIL_WILL_BUTTON_TIMER)
    static long __fastcall GetConstantFPS() { return ConstantFPS; }
#endif
    CMainGameComponent(CGame&,const CMainGameComponentInit&);
    virtual ~CMainGameComponent();
    virtual bool Run(CGameComponent**);
    virtual void ChangeTextureColourDepth(long);
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    CPlayerManager& GetPlayerManager(){return *PPlayerManager;}
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
    const CGameDefinitionManager& PeekDefinitionManager() const;
#endif
    const CWorld& PeekWorld() const { return *PWorld; }
};
FABLE_STATIC_ASSERT(sizeof(CMainGameComponent)==0x161e8);
#endif

#else
#ifndef FABLE_ENGINE_CMAINGAMECOMPONENT_H
#define FABLE_ENGINE_CMAINGAMECOMPONENT_H
// Original CGameComponent base and native PWorld+24; caller0042f4b1
// allocates0x161e8 before constructor00418dca. The unused remainder is opaque.
#include "engine/CGameComponent.h"
#include "engine/CWorld.h"
class CGameDefinitionManager;class CASoundBank; class CMainGameComponentInit; class CDisplayEngine;
#if defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
class CPlayerManager;
#endif
class CMainGameComponent : public CGameComponent {
protected:
#if defined(FABLE_RETAIL_HIT_EFFECT) || defined(FABLE_RETAIL_ENTITY_PERSIST) || (defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE))
    static CMainGameComponent* PMainGameComponent;
#endif
#if defined(FABLE_RETAIL_CAMERA_DRAW) || defined(FABLE_RETAIL_WILL_BUTTON_TIMER)
    static long ConstantFPS;
#endif
    CASoundBank* PSampleBank;
    unsigned char PTextBank[8]; // CCountedPointer<NGameText::CDataBank> +14
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    boost::scoped_ptr<CPlayerManager> PPlayerManager;
#else
    unsigned char PPlayerManager[4]; // boost::scoped_ptr<CPlayerManager> +1c
#endif
    unsigned char PPlayerInterface[4]; // boost::scoped_ptr<CGamePlayerInterface> +20
    boost::scoped_ptr<CWorld> PWorld;
    boost::scoped_ptr<CDisplayEngine> PDisplayEngine;
    unsigned char PLUA[8]; // CCountedPointer<CLUA> +2c
    bool ForceUpdateTick;
    float ForceUpdateTickSpeedMultipler;
    float ForceUpdateTickSpeedDesiredFramerate;
    long ForceUpdateNoFailedUpdates;
    bool FirstWorldFrameUpdate;
    long CurrentServerFrame;
    long InputServerFrame;
    long LastGameTurnForceRendered;
    double CurrentFrameStartGameTime;
    double GameStartTime;
    double LastFrameRenderDuration;
    unsigned char _pad_0x70[0x16178]; // All original members LastInterpolationInfo through DebugNoFramesUnableToRender.
public:
#if defined(FABLE_RETAIL_HIT_EFFECT) || defined(FABLE_RETAIL_ENTITY_PERSIST) || (defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE))
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
#endif
#if defined(FABLE_RETAIL_CAMERA_DRAW) || defined(FABLE_RETAIL_WILL_BUTTON_TIMER)
    static long __fastcall GetConstantFPS() { return ConstantFPS; }
#endif
    CMainGameComponent(CGame&,const CMainGameComponentInit&);
    virtual ~CMainGameComponent();
    virtual bool Run(CGameComponent**);
    virtual void ChangeTextureColourDepth(long);
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    CPlayerManager& GetPlayerManager(){return *PPlayerManager;}
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
    const CGameDefinitionManager& PeekDefinitionManager() const;
#endif
    const CWorld& PeekWorld() const { return *PWorld; }
};
FABLE_STATIC_ASSERT(sizeof(CMainGameComponent)==0x161e8);
#endif

#endif

#endif
