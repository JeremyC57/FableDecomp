// CMainGameComponent::InitDisplayEngine() -- construct display engine from current graphics/world/surface settings.
// Native00417418 length217. Actual flattened headers retained; native CMainGameComponent header absent.
// CMain late PMainGraphicBank native161c8 vs donor161d0. Native new size100 matches donor DisplayEngine.
// Calls419270 init ctor;415bf0 height;9f2f90 dimensions;9f2f60 pixel;44c6b0 definition;
// BFEA1A scalar new;434e10 display ctor;41940c scoped reset;99ebf0/99eae0 string;9e9f40 progress;4350d0 Init.
#pragma optimize("s", on)
#pragma optimize("y", off)
#include <new>
#define CMainGameComponent CMainGameComponentDonor
#define CGameComponent CGameComponentDonor
#define CGame CGameDonor
#define CCharString CCharStringDonor
#include "fable_game.h"
#undef CCharString
#undef CGame
#undef CGameComponent
#undef CMainGameComponent
#define CDisplayEngine CDisplayEngineDonor
#include "engine/CDisplayEngine.h"
#undef CDisplayEngine
#define CSurface CSurfaceDonor
#include "engine/CSurface.h"
#undef CSurface
#include "engine/CPixelFormat.h"
#include "engine/CWorld.h"
#define CGameDefinitionManager CGameDefinitionManagerDonor
#pragma push_macro("FABLE_STATIC_ASSERT")
#undef FABLE_STATIC_ASSERT
#define FABLE_STATIC_ASSERT(x)
// Quarantined donor reports sizeof1 despite emitting void*; preserve its declaration, suppress invalid donor assertions only.
#include "engine/_quarantine/CGameDefinitionManager.h"
#pragma pop_macro("FABLE_STATIC_ASSERT")
#undef CGameDefinitionManager
class CCharString { public: CCharStringData* Data; CCharString(const char*,long); CCharString(const CCharString&); ~CCharString(); };
class C2DExtentsI { public: long Width,Height; };
class CSurface : public CSurfaceDonor { public:
 unsigned long GetHeight()const; //00415bf0
 C2DExtentsI GetDimensions()const; //009f2f90
 unsigned long GetWidth()const {return GetDimensions().Width;}
 CPixelFormat GetPixelFormat()const; //009f2f60
};
struct CDisplayDevice { unsigned char DisplayState[0x2c]; CSurface BackSurface; CSurface& GetBackSurface(){return BackSurface;} };
extern CDisplayDevice* g_DisplayDevice; //013b8390
class CBaseClass { public: CBaseClass(); virtual ~CBaseClass(); };
struct CBaseClassNonCopyable : CBaseClass { CBaseClassNonCopyable(); private: CBaseClassNonCopyable(const CBaseClassNonCopyable&); CBaseClassNonCopyable&operator=(const CBaseClassNonCopyable&); };
class CMainGameComponent; class CGamePlayerInterface;
class CDisplayEngineInit { public:
 CMainGameComponent* Component; CGameDefinitionManager* DefinitionManager; CPlayerManager* PlayerManager; CWorld* World;
 CGraphicDataBank* MainGraphicBank; unsigned long Width,Height; CPixelFormat PixelFormat; CMeshDataBank* MeshBank;
 CDisplayEngineInit(); //00419270 signature confirmed PDB
};
class CDisplayEngine : public CBaseClassNonCopyable { public:
 virtual ~CDisplayEngine(); //native slot0 00437070
 virtual bool InitEngine(); //slot1 00435190
 virtual void UninitEngine(); //slot2 004354a0
 unsigned char DisplayState[0xfc];
 CDisplayEngine(const CDisplayEngineInit&); //00434e10 signature confirmed PDB
 void Init(); //004350d0
};
template<class T> struct scoped_ptr { T* ptr; void reset(T*); }; //0041940c
class CMainGameComponent { public:
 unsigned char BaseAndBanks[0x1c]; CPlayerManager* PPlayerManager; CGamePlayerInterface* PPlayerInterface;
 CWorld* PWorld; scoped_ptr<CDisplayEngine> PDisplayEngine;
 unsigned char LUAAndWorldState[0x1619c]; CGraphicDataBank* PMainGraphicBank; void* PMainGraphicBankInfo;
 void InitDisplayEngine();
};
CGameDefinitionManager* __fastcall GetDefinitionManager(); //0044c6b0
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString&,float,bool,bool); } //009e9f40
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDisplayEngine)==0x100);
FABLE_STATIC_ASSERT(sizeof(CDisplayEngine)==sizeof(CDisplayEngineDonor));
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,PWorld)==0x24);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,PMainGraphicBank)==0x161c8);
FABLE_STATIC_ASSERT(sizeof(CDisplayEngineInit)==0x24);
#endif
void CMainGameComponent::InitDisplayEngine(){
 CDisplayEngineInit init;
 init.World=PWorld;
 init.Height=g_DisplayDevice->BackSurface.GetHeight();
 init.Width=g_DisplayDevice->BackSurface.GetWidth();
 init.PixelFormat=g_DisplayDevice->BackSurface.GetPixelFormat();
 init.DefinitionManager=GetDefinitionManager();
 init.PlayerManager=PPlayerManager;
 init.MainGraphicBank=PMainGraphicBank;
 init.Component=this;
 init.MeshBank=PWorld->PMainMeshBank_Data;
 PDisplayEngine.reset(new CDisplayEngine(init));
 {CCharString text("Init Display Engine: Calling Init",-1);NProgressDisplay::DisplayProgress(text,-1.0f,false,false);}
 PDisplayEngine.ptr->Init();
}