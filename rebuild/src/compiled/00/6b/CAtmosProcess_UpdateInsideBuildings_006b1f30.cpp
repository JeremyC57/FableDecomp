// CAtmosProcess::UpdateInsideBuildings() -- take the reverb environment from the current region's def; every fourth frame test whether the camera is inside a building near it (whose reverb then wins) and record it; when the inside flag changes, cross-fade the four atmos sound groups and touch every SOUND_THEME def; when the reverb changed, store it and apply it
// Leaf: ?UpdateInsideBuildings@CAtmosProcess@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B1F30, length 811 bytes, plain `ret` (thiscall, no arguments).
// Name: Ego_r.pdb ?UpdateInsideBuildings@CAtmosProcess@@AAEXXZ (private); callee ApplyReverbSettings 0x006B1600 is the PDB's
//   private ?ApplyReverbSettings@CAtmosProcess@@AAEXXZ (its body indexes a 0x54-byte reverb preset table by ReverbType and
//   hands ReverbLevel to the sound manager; its two callers are CAtmosProcess::Update 0x006B19FE and this row after the
//   ReverbType/ReverbLevel stores). The binding LoadMainTheme_006b1600 is descriptive and wrong (NAME_FIXES.tsv).
// Callees: CMainGameComponent::GetWorld (inline, PWorld +0x24), CWorld slot 12 GetWorldMap (+0x30, landed 0x004AE8B0),
//   CWorldMap::GetLoadedRegion 0x004FB150, CWorldMap::GetRegion(long) 0x004FB490, CWorld::GetFrame 0x0049D870,
//   CAIGameCameraBase slot 1 GetRenderCamera(float) (+0x04, CCamera by value; the model of the landed CWorld::GetGameLODCentre
//   0x0049DBE0), __ftol2 0x00BFEA70, CThingSearchTools::GetAllThingsInRoughArea<const CThingBuilding,
//   vector<const CThingBuilding*>, CThingFilter_None> 0x006B2720 (landed), CThingBuilding::IsPointInside 0x0082D000,
//   CThingBuilding::GetReverbSettings 0x0082D8C0, free 0x00BFEA14 (vector storage), CGameDefinitionManager::Get 0x0044C6B0,
//   CASoundManager slot 46 SetGroupVolume (+0xB8, the landed CNewFrontendGameComponent::InitialiseSounds model; no PDB
//   symbol; slot name descriptive) on g_pSoundManager 0x013B8394 (labels DB spelling), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CDefinitionManager::GetNumberOfDefsOfType 0x009AD3B0 (landed), CDefinitionManager::GetDef<CSoundThemeDef> 0x006B25E0.
// Layout (local models; the shared headers differ from retail):
//   CAtmosProcess: retail InsideBuilding +0x14, ReverbType +0x18, ReverbLevel +0x1c (Ego_r +0x10/+0x14/+0x18; retail keeps
//     one more dword ahead of them). No engine header exists.
//   CRegion: PDef +0x24 (Ego_r +0x2c past two 16-byte vectors; engine/CRegion.h carries the Ego_r offsets).
//   CRegionDef: ReverbEnvironmentType +0x7c, ReverbEnvironmentLevel +0x80 (Ego_r +0x88/+0x8c, CSoundMap 0xc smaller).
//   CGameDefinitionManager: PGlobalDef +0xd0 (landed CTCFollowed rows); CGlobalDef SoundGainLowerValueWhenInsideOutside
//     +0x138 (Ego_r +0x148, retail 0x10 lower as the landed FollowerIndicatorIcon +0x12c read shows).
//   CWorld / CAIGameCameraBase: the engine headers flatten the vptr, so local models carry the retail virtual slots.
//   CWorldMap / CThingSearchTools / CDefinitionManager / CRegion: the engine headers are included under the layout-define
//     pattern; the local models carry only what this row touches (methods, PDef at its retail offset).
// Shape:
//   - the camera query is GetRenderCamera(1.0f).WorldPosition: a whole 0x38-byte CCamera return temporary sits above the
//     search box (+0x54..+0x8b), which is retail's 0x7c frame (a C3DVector-returning call packs to 0x44).
//   - GetAllThingsInRoughArea<.., CThingFilter_None> is a same-TU stand-in body (not a reconstruction; the real instance is
//     the landed 0x006B2720 in this retail TU). It never reads the empty filter, as the real instance's inline
//     CThingFilter_None::operator() does not; VC7.1 then homes the filter at the `checked` byte (+0x13) as retail does.
//   - The region def is held as a raw pointer with the counted-pointee AddRef/Release written out (`inc [ebp+4]` ...
//     `dec [ebp+4] ; jne`). A CDefPointer<const CRegionDef> copy, or its pointer constructor over PDef.Get(), leaves a dead
//     `add eax,24h` (the PDef address) after GetRegion: retail reads the member word directly.
//   - GetDef<CSoundThemeDef> is a same-TU stand-in body too (it only fills the out-parameter), which keeps the SOUND_THEME
//     loop's CCharString and CDefPointer in retail's slots; the loop's def handle is the local CDefPointer model.
#include <stdlib.h>
// CDefPointer: a local model of engine/CDefPointer.h over the shared engine/CDefPointeeBase.h (header debt: the shared
// Release() holds the pointee in a local and stores Object = 0, which VC7.1 compiles to `mov edx,[eax+4] ; dec edx ;
// mov [eax],edx` here; retail's two releases are `dec dword ptr [ecx+4] ; jne`).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointeeBase.h"                        // CIVCountedPointeeBase: slot 1 Delete, IVRefCount +0x04
template <class T> class CDefPointer { public:
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->IVRefCount; }
    ~CDefPointer() { if (Object) { if (--Object->IVRefCount == 0) Object->Delete(); } }
    bool IsNull() const { return Object == 0; }
    T* operator->() const { return static_cast<T*>(Object); }
    CDefPointeeBase* Object;                                // +0x00
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CRightHandedSet { public: C3DVector Up, Forward; };
class CCamera { public:                                   // 0x38 bytes (Ego_r.pdb)
    C3DVector       WorldPosition;                 // +0x00
    CRightHandedSet RHSet;                         // +0x0c
    bool            HeightLocked;                  // +0x24
    bool            ViewVecZLocked;                // +0x25
    unsigned long   FOVFlags;                      // +0x28
    float           HorizontalFOV;                 // +0x2c
    float           VerticalFOV;                   // +0x30
    float           Zoom;                          // +0x34
};
class C2DBoxI { public:                            // the model of the landed 0x006B2720 (same retail TU)
    long X0, Y0, X1, Y1;
    C2DBoxI() { }
};
class CCharString { public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    void* PStringData;
};

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    vector() { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
}

enum EReverbEnvironmentType { REVERB_ENVIRONMENT_TYPE_UNPROVEN_0 };   // Ego_r.pdb enum; enumerators unused here

class CThing;
class CThingBuilding { public:
    bool IsPointInside(const C3DVector& point) const;                                          // 0x0082D000
    void GetReverbSettings(EReverbEnvironmentType& type, float& level) const;                  // 0x0082D8C0
};
class CThingFilter_None { };
// Shared layout: engine/CThingSearchTools.h (layout-define pattern); the local model below keeps only the members/methods this row touches.
#define CThingSearchTools CThingSearchToolsLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
class CThingSearchTools { public:
    template <class TThing, class TContainer, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things) const;
};
// Same-TU stand-in for 0x006B2720 (see the header): the result vector's words go out by value so it does not escape.
void GetAllThingsInRoughAreaStandIn_006b2720(const CThingSearchTools* tools, const C2DBoxI* area, const CThingBuilding** start,
                                             const CThingBuilding** finish, const CThingBuilding** end);
const CThingBuilding** GetAllThingsInRoughAreaStandInResult_006b2720(long word);
#pragma auto_inline(off)
template <> __declspec(noinline) void CThingSearchTools::GetAllThingsInRoughArea<const CThingBuilding,
    std::vector<const CThingBuilding*>, CThingFilter_None>(const C2DBoxI& area, const CThingFilter_None&,
                                                            std::vector<const CThingBuilding*>& things) const
{
    GetAllThingsInRoughAreaStandIn_006b2720(this, &area, things._M_start, things._M_finish, things._M_end_of_storage._M_data);
    things._M_start = GetAllThingsInRoughAreaStandInResult_006b2720(0);
    things._M_finish = GetAllThingsInRoughAreaStandInResult_006b2720(1);
    things._M_end_of_storage._M_data = GetAllThingsInRoughAreaStandInResult_006b2720(2);
}
#pragma auto_inline(on)

class CRegionDef : public CDefPointeeBase { public:
    unsigned char          _pad1c[0x7c - 0x1c];
    EReverbEnvironmentType ReverbEnvironmentType;           // +0x7c
    float                  ReverbEnvironmentLevel;          // +0x80
};
// Shared layout: engine/CRegion.h (layout-define pattern); the local model below keeps only the members/methods this row touches.
#define CRegion CRegionLayout
#include "engine/CRegion.h"
#undef CRegion
class CRegion { public:
    unsigned char                 _pad00[0x24];
    CDefPointer<const CRegionDef> PDef;                     // +0x24
};
// Shared layout: engine/CWorldMap.h (layout-define pattern); the local model below keeps only the members/methods this row touches.
#define CWorldMap CWorldMapLayout
#include "engine/CWorldMap.h"
#undef CWorldMap
class CWorldMap { public:
    long GetLoadedRegion() const;                           // 0x004FB150
    CRegion& GetRegion(long region);                        // 0x004FB490
};
// Shared layout: engine/CAIGameCameraBase.h (layout-define pattern); the local model below keeps header flattens the vptr into `void* __vftable`.
#define CAIGameCameraBase CAIGameCameraBaseLayout
#include "engine/CAIGameCameraBase.h"
#undef CAIGameCameraBase
class CAIGameCameraBase { public:
    virtual ~CAIGameCameraBase();
    virtual CCamera GetRenderCamera(float interpolation) const = 0;   // slot 1 (+0x04)
};
class CAFile; class CThingManager; class CInterpolationInfo; class CDisplayEngine; class CEnginePrimitivePrepareInfo;
// Shared layout: engine/CWorld.h (layout-define pattern); the local model below keeps header flattens the vptr into `void* __vftable`.
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
class CWorld { public:
    virtual ~CWorld();                                                          // slot 0
protected:
    virtual void PostLoadInit();                                                // slot 1
public:
    virtual void LoadWorldFromFile(CAFile& file, long version);                 // slot 2
    virtual CThingManager& GetThingManager();                                   // slot 3
    virtual const CThingManager& PeekThingManager() const;                      // slot 4
    virtual CCamera GetCurrentRenderCamera() const;                             // slot 6 (overloads go in reverse order)
    virtual CCamera GetCurrentRenderCamera(const CInterpolationInfo& info) const;   // slot 5
    virtual void PostInit(CDisplayEngine& display);                             // slot 7
    virtual C3DVector GetGameLODCentre() const;                                 // slot 8
    virtual void Init();                                                        // slot 9
    virtual void Uninitialise();                                                // slot 10
    virtual void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info);   // slot 11
    virtual CWorldMap& GetWorldMap();                                           // slot 12 (+0x30)
    static long __fastcall GetFrame();                                          // 0x0049D870
    unsigned char      _pad04[0x20 - 0x04];
    CThingSearchTools* PThingSearchTools;                                       // +0x20
    unsigned char      _pad24[0x34 - 0x24];
    CAIGameCameraBase* PCurrentGameCamera;                                      // +0x34
};
class CMainGameComponent { public:
    unsigned char _pad00[0x24];
    CWorld*       PWorld;                                   // +0x24 (scoped_ptr<CWorld>)
    CWorld& GetWorld() { return *PWorld; }
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

class CGlobalDef : public CDefPointeeBase { public:
    unsigned char _pad1c[0x138 - 0x1c];
    float         SoundGainLowerValueWhenInsideOutside;     // +0x138
};
class CSoundThemeDef : public CDefPointeeBase { };
// Shared layout: engine/CDefinitionManager.h (layout-define pattern); the local model below keeps only the members/methods this row touches.
#define CDefinitionManager CDefinitionManagerLayout
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
class CDefinitionManager { public:
    long GetNumberOfDefsOfType(const CCharString& typeName);                                       // 0x009AD3B0
    template <class T> bool GetDef(const CCharString& name, long index, CDefPointer<const T>* def) const;   // <CSoundThemeDef> 0x006B25E0
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();        // 0x0044C6B0
    unsigned char _pad00[0xd0];
    CGlobalDef*   PGlobalDef;                               // +0xd0
};

// Same-TU stand-in for GetDef<CSoundThemeDef> 0x006B25E0 (not a reconstruction): it only fills its out-parameter.
CDefPointeeBase* GetDefStandIn_006b25e0(const CDefinitionManager* manager, const CCharString* name, long index);
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefinitionManager::GetDef<CSoundThemeDef>(const CCharString& name, long index,
                                                                                CDefPointer<const CSoundThemeDef>* def) const
{
    def->Object = GetDefStandIn_006b25e0(this, &name, index);
    return true;
}
#pragma auto_inline(on)

class CASoundManager { public:                            // RTTI base of CSoundManagerLH (vtable 0x0129D2C4)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void SetGroupVolume(unsigned long groupMask, float volume, float fadeTime);   // slot 46 (+0xB8) 0x00A3E9D0
};
extern CASoundManager* g_pSoundManager;                     // 0x013B8394 (labels DB: g_pSoundManager, CASoundManager*)

class CAtmosProcess { public:
    unsigned char          _pad00[0x14];
    long                   InsideBuilding;                  // +0x14
    EReverbEnvironmentType ReverbType;                      // +0x18
    float                  ReverbLevel;                     // +0x1c
private:
    void UpdateInsideBuildings();
    void ApplyReverbSettings();                             // 0x006B1600
};

void CAtmosProcess::UpdateInsideBuildings()
{
    CWorld& world = g_pMainGameComponent->GetWorld();
    EReverbEnvironmentType reverbType = ReverbType;
    float reverbLevel = ReverbLevel;
    const CRegionDef* def = static_cast<const CRegionDef*>(world.GetWorldMap().GetRegion(world.GetWorldMap().GetLoadedRegion()).PDef.Object);
    if (def) {
        ++def->IVRefCount;
        reverbType = def->ReverbEnvironmentType;
        reverbLevel = def->ReverbEnvironmentLevel;
    }
    long wasInside = InsideBuilding;
    bool checked = false;
    if (CWorld::GetFrame() % 4 == 0) {
        checked = true;
        C3DVector pos = world.PCurrentGameCamera->GetRenderCamera(1.0f).WorldPosition;
        std::vector<const CThingBuilding*> buildings;
        long x = (long)pos.X;
        long y = (long)pos.Y;
        C2DBoxI area;
        area.X0 = x - 3;
        area.Y0 = y - 3;
        area.X1 = x + 3;
        area.Y1 = y + 3;
        CThingFilter_None filter;
        world.PThingSearchTools->GetAllThingsInRoughArea<const CThingBuilding>(area, filter, buildings);
        long inside = 0;
        for (std::vector<const CThingBuilding*>::iterator it = buildings.begin(); it != buildings.end(); ++it) {
            const CThingBuilding* building = *it;
            if (building->IsPointInside(pos)) {
                building->GetReverbSettings(reverbType, reverbLevel);
                inside = 1;
                break;
            }
        }
        InsideBuilding = inside;
    }
    if (wasInside != InsideBuilding) {
        float gain = CGameDefinitionManager::Get().PGlobalDef->SoundGainLowerValueWhenInsideOutside;
        if (InsideBuilding != 0) {
            g_pSoundManager->SetGroupVolume(0x800, 1.0f, 1000.0f);
            g_pSoundManager->SetGroupVolume(0x1000, 0.0f, 1000.0f);
            g_pSoundManager->SetGroupVolume(0x2000, 1.0f, 1000.0f);
            g_pSoundManager->SetGroupVolume(0x4000, gain, 1000.0f);
        } else {
            g_pSoundManager->SetGroupVolume(0x1000, 1.0f, 1000.0f);
            g_pSoundManager->SetGroupVolume(0x800, 0.0f, 1000.0f);
            g_pSoundManager->SetGroupVolume(0x4000, 1.0f, 1000.0f);
            g_pSoundManager->SetGroupVolume(0x2000, gain, 1000.0f);
        }
        long count;
        {
            CCharString type("SOUND_THEME");
            count = CGameDefinitionManager::Get().GetNumberOfDefsOfType(type);
        }
        for (long i = 1; i < count; ++i) {
            CDefPointer<const CSoundThemeDef> theme;
            CCharString type("SOUND_THEME");
            CGameDefinitionManager::Get().GetDef<CSoundThemeDef>(type, i, &theme);
        }
    }
    if (checked) {
        if (reverbType != ReverbType || reverbLevel != ReverbLevel) {
            ReverbType = reverbType;
            ReverbLevel = reverbLevel;
            ApplyReverbSettings();
        }
    }
    if (def) {
        if (--def->IVRefCount == 0)
            def->Delete();
    }
}