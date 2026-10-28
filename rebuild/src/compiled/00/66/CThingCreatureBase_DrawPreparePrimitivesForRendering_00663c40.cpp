// CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- let the fly-on-spline TC prepare, run the CThingPhysical pass (false stops here); with a graphic appearance, test the game LOD centre against the creature position within the scaled fade distance: in range draw the appearance, add the weapon trails and the hit notification when trails are shown, let the emote icon prepare, and for a firefly-property creature add the firefly sprite at the interpolated physics position; out of range clear the appearance's primitives and the emote icon's
// Leaf: ?DrawPreparePrimitivesForRendering@CThingCreatureBase@@MAE_NABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663C40, 577 bytes, `ret 4`; CThingCreatureBase vtable 0x0125AC0C slot 36
//   (CThingPhysical 0x01244E84 slot 36 = 0x006A64D0). Ego_r.pdb / FableWin.pdb: protected virtual (MAE).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 / const 0x004365B0 (inline CThing::GetTC<T> /
//   PeekTC<const T>), CThingPhysical::DrawPreparePrimitivesForRendering 0x006A64D0,
//   NGraphicAppearance::CTCGraphicAppearance::GetFadeDistance 0x004BC180 / Draw(const CEnginePrimitivePrepareInfo&)
//   0x004BE4E0 (builds the two interpolators, calls the 3-argument Draw 0x004BD6E0) / ClearAllPrimitivesDueToBeingOutOfRange
//   0x004BF960, CThing::GetWorld 0x004C7AA0 (Ego_r order of the CThing world getters), CThing::GetPos 0x004C73D0,
//   CWorld::GetGameLODCentre (vtable slot 8), GFWorld3DDistanceSmallerThan 0x006614F0, CTCWeaponTrail::DrawAddWeaponTrails
//   0x0076E6A0, CTCCreatureHitNotification::DrawHitNotification 0x00864FF0, CTCEmoteIcon::ClearPrimitives 0x00669390,
//   CThing::PeekDefinitionManager 0x004C7990, CTCPhysicsBase::GetPosition(float) 0x0049D150, CEnginePrimitiveSprite ctor
//   0x006558CE, CIEngine::AddPrimitive (vtable slot 20).
// TC kinds (each class's GetInterfaceType, RTTI slot 21): 0xD9 CTCCreatureFlyOnSpline, 0x4C CTCWeaponTrail,
//   0xCD CTCCreatureHitNotification, 0x45 CTCEmoteIcon.
// Layout: retail CThing interface bitset +0x20, CVectorMap +0x44, PhysicsTC +0x60, PTCGraphicAppearance +0x64;
//   CThingCreatureBase PDef +0xE0 (engine/CThingCreatureBase.h PDef_Object); retail CThingCreatureDef::Property +0xF0
//   (Ego_r +0xF8); CGameDefinitionManager PGlobalDef +0xD0; retail CGlobalDef::E3BodgeFireflyGraphic +0xD4 (Ego_r +0xD8,
//   the CGlobalDef -4 shift of InvalidThemeStandin 0x54 -> 0x50). CEnginePrimitivePrepareInfo: Interpolate.Current
//   .WFInterpolate +0x04, Engine +0x34.
// Shape: the two non-virtual TC calls use the PDB inline out-pointer accessors CThing::GetTC<T>(T**) / PeekTC<const T>
//   (bit test, the key as a temporary, LowerBound, `it == end || key < it->first` -> end), so the component reaches the
//   call through eax (`mov eax,[eax+4] ; push ebx ; mov ecx,eax`). The two virtual DrawPreparePrimitivesForRendering
//   calls use the PDB inline `CTCBase& CThing::GetTC(ETCInterfaceType)` after `HasTC(ETCInterfaceType)`, written as the
//   pick form (copied key, reference to Components, `(miss ? end : it)->second`), which loads the component straight
//   into ecx. The parked source (one accessor for all four) missed the eax form by 4 bytes.
class C3DVector {
public:
    float X;
    float Y;
    float Z;
};
class CEnginePrimitive {
public:
    unsigned char _body[0x48];
};
enum EEnginePrimitiveBlendMode { };
enum EEngineInterpolationSet { };
enum EEnginePrimitiveBlendOp { };
class CEnginePrimitiveSprite : public CEnginePrimitive {
public:
    CEnginePrimitiveSprite(const C3DVector& position, unsigned long graphic, unsigned long frame, unsigned short subFrame,
                           float angle, float width, float height, EEnginePrimitiveBlendMode blendMode,
                           unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, float zBias,
                           EEngineInterpolationSet interpolationSet, EEnginePrimitiveBlendOp blendOp);   // 0x006558CE
};
class CIEngine {
public:
    virtual ~CIEngine();
    virtual void Validate() const;                                             // CInitBaseClass slot 1
    virtual bool Initialise(const class CEngineInit& init);
    virtual void Release();
    virtual void SetCamera(const class CCamera* camera) = 0;
    virtual void SetWindow(const class C2DBoxF* window) = 0;
    virtual void SetEnvironmentalEffectsAlwaysUpdate(bool update);
    virtual void SetWorld(const class CIDrawWorld* world);
    virtual void Render(const class CEngineInterpolationInfo& info, unsigned long flags) = 0;
    virtual void WorldFrameUpdate(bool paused);
    virtual void PausedWorldFrameUpdate();
    virtual bool ConvertScreenPosToWorldPos(const class C2DVector& screen, C3DVector& world) const = 0;
    virtual bool ConvertScreenPosToWorldPosAtViewDistance(float distance, const class C2DVector& screen, C3DVector& world) const = 0;
    virtual bool ConvertWorldPosToScreenPos(const C3DVector& world, class C2DVector& screen) const = 0;
    virtual bool SetVideoOptions(const class CEngineVideoOptions& options);
    virtual bool PreChangeResolution() = 0;
    virtual bool PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth) = 0;
    virtual void Refresh();
    virtual bool CheckPrimitiveVisibility(unsigned __int64 id);
    virtual float GetPrimitiveDrawDistanceFactor() const;
    virtual void AddPrimitive(const CEnginePrimitive& primitive);                 // slot 20 (+0x50)
};
class CInterpolationInfoSet {
public:
    float GTPredictedTimeSinceLastRenderFrame;
    float WFInterpolate;
};
class CInterpolationInfo {
public:
    CInterpolationInfoSet Current;
    CInterpolationInfoSet Last;
    CInterpolationInfoSet PausedCurrent;
    CInterpolationInfoSet PausedLast;
    CInterpolationInfoSet BulletTimeCurrent;
    CInterpolationInfoSet BulletTimeLast;
    long WFServerCurrentTime;
};
class CEnginePrimitivePrepareInfo {
public:
    CInterpolationInfo Interpolate;                     // +0x00
    CIEngine*          Engine;                          // +0x34
};

class CPersistContext;
class CTCParamsBase;
class CTCBase {
public:
    virtual ~CTCBase();                                                          // 0
protected:
    virtual void OnSerialise(CPersistContext& context, bool loading);            // 1
public:
    virtual void OnRemove();                                                     // 2
    virtual void ConstructFromParams(const CTCParamsBase& params);               // 3
    virtual void OnCreate();                                                     // 4
    virtual void InitialActivate();                                              // 5
    virtual void OnInitialActivate();                                            // 6
    virtual void RenderUpdate(float elapsed);                                    // 7
    virtual bool HasFrameUpdate() const;                                         // 8
    virtual void FrameUpdate();                                                  // 9
    virtual void BulletTimeFrameUpdate();                                        // 10
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 11 (+0x2c)
};
enum ETCInterfaceType {
    TC_EMOTE_ICON = 0x45,
    TC_WEAPON_TRAIL = 0x4c,
    TC_CREATURE_HIT_NOTIFICATION = 0xcd,
    TC_CREATURE_FLY_ON_SPLINE = 0xd9
};
class CTCCreatureFlyOnSpline : public CTCBase {
public:
    enum { InterfaceType = TC_CREATURE_FLY_ON_SPLINE };
};
class CTCWeaponTrail : public CTCBase {
public:
    enum { InterfaceType = TC_WEAPON_TRAIL };
    void DrawAddWeaponTrails(const CEnginePrimitivePrepareInfo& info) const;   // 0x0076E6A0
};
class CTCCreatureHitNotification : public CTCBase {
public:
    enum { InterfaceType = TC_CREATURE_HIT_NOTIFICATION };
    void DrawHitNotification(const CEnginePrimitivePrepareInfo& info) const;   // 0x00864FF0
};
class CTCEmoteIcon : public CTCBase {
public:
    enum { InterfaceType = TC_EMOTE_ICON };
    void ClearPrimitives();                                                    // 0x00669390
};
class CTCPhysicsBase {
public:
    C3DVector GetPosition(float interpolate) const;                            // 0x0049D150
};
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    float GetFadeDistance() const;                                             // 0x004BC180
    void ClearAllPrimitivesDueToBeingOutOfRange();                             // 0x004BF960
    void Draw(const CEnginePrimitivePrepareInfo& info);                        // 0x004BE4E0
};
}
class CAFile;
class CThingManager;
class CCamera;
class CDisplayEngine;
class CWorld {
public:
    virtual ~CWorld();                                                         // 0
protected:
    virtual void PostLoadInit();                                               // 1
public:
    virtual void LoadWorldFromFile(CAFile& file, long region);                 // 2
    virtual CThingManager& GetThingManager();                                  // 3
    virtual const CThingManager& PeekThingManager() const;                     // 4
    virtual CCamera GetCurrentRenderCamera() const;                            // 6 (overloads take reverse order)
    virtual CCamera GetCurrentRenderCamera(const CInterpolationInfo& info) const;   // 5
    virtual void PostInit(CDisplayEngine& displayEngine);                      // 7
    virtual C3DVector GetGameLODCentre() const;                                // 8 (+0x20)
};
enum EThingCreatureProperty { };
class CThingCreatureDef {
public:
    unsigned char          _pad00[0xf0];
    EThingCreatureProperty Property;                    // +0xF0 (Ego_r +0xF8)
};
class CGlobalDef {
public:
    unsigned char _pad00[0xd4];
    long          E3BodgeFireflyGraphic;                // +0xD4 (Ego_r +0xD8)
};
class CGameDefinitionManager {
public:
    unsigned char _pad00[0xd0];
    CGlobalDef*   PGlobalDef;                           // +0xD0 CDefPointer<const CGlobalDef>::Object
};
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance);   // 0x006614F0
extern float g_primitiveDrawDistanceScale;              // 0x013B8774
extern bool  g_ShowWeaponTrails;                        // 0x0137548E

namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    value_type* _M_start;
    value_type* _M_finish;
    value_type* _M_end_of_storage;
    iterator LowerBound(const K& key);                  // 0x0040F020
    const_iterator LowerBound(const K& key) const;      // 0x004365B0
    iterator end() { return _M_finish; }
    const_iterator end() const { return _M_finish; }
};

class CThing {
public:
    virtual ~CThing();
    unsigned char _pad_0x04[0x20 - 0x04];
    unsigned long UsingComponents[9];                   // +0x20 bitset
    CVectorMap<ETCInterfaceType, CTCBase*> Components;  // +0x44
    unsigned char _pad_0x50[0x60 - 0x50];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    const C3DVector& GetPos() const;                    // 0x004C73D0
    const CGameDefinitionManager& PeekDefinitionManager() const;      // 0x004C7990
    bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type / 32] & (1UL << (type % 32)))
            return true;
        return false;
    }
    __forceinline CTCBase& GetTC(ETCInterfaceType type)
    {
        ETCInterfaceType key = type;
        CVectorMap<ETCInterfaceType, CTCBase*>& components = Components;
        CVectorMap<ETCInterfaceType, CTCBase*>::iterator p = components.LowerBound(key);
        return *(p == components.end() || type < p->first ? components.end() : p)->second;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!(UsingComponents[T::InterfaceType / 32] & (1UL << (T::InterfaceType % 32))))
            return false;
        CVectorMap<ETCInterfaceType, CTCBase*>& components = Components;
        CVectorMap<ETCInterfaceType, CTCBase*>::iterator p = components.LowerBound((ETCInterfaceType)T::InterfaceType);
        if (p == components.end() || (ETCInterfaceType)T::InterfaceType < p->first)
            p = components.end();
        *out = (T*)p->second;
        return true;
    }
    template <class T> __forceinline bool PeekTC(T** out) const              // T = const X (PDB ??$PeekTC@$$CBV...)
    {
        if (!(UsingComponents[T::InterfaceType / 32] & (1UL << (T::InterfaceType % 32))))
            return false;
        const CVectorMap<ETCInterfaceType, CTCBase*>& components = Components;
        CVectorMap<ETCInterfaceType, CTCBase*>::const_iterator p = components.LowerBound((ETCInterfaceType)T::InterfaceType);
        if (p == components.end() || (ETCInterfaceType)T::InterfaceType < p->first)
            p = components.end();
        *out = (T*)p->second;
        return true;
    }
protected:
    CWorld& GetWorld();                                 // 0x004C7AA0
};
class CThingPhysical : public CThing {
public:
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x006A64D0
};
class CThingCreatureBase : public CThingPhysical {
public:
    unsigned char _pad_0x68[0xe0 - 0x68];
    const CThingCreatureDef* PDef;                      // +0xE0 CDefPointer<const CThingCreatureDef>::Object
protected:
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (HasTC(TC_CREATURE_FLY_ON_SPLINE))
        GetTC(TC_CREATURE_FLY_ON_SPLINE).DrawPreparePrimitivesForRendering(info);
    if (CThingPhysical::DrawPreparePrimitivesForRendering(info)) {
    if (PTCGraphicAppearance) {
        float distance = g_primitiveDrawDistanceScale;
        distance = PTCGraphicAppearance->GetFadeDistance() * (distance + 1.0f);
        CWorld& world = GetWorld();
        if (GFWorld3DDistanceSmallerThan(world.GetGameLODCentre(), GetPos(), distance)) {
            PTCGraphicAppearance->Draw(info);
            if (g_ShowWeaponTrails) {
                CTCWeaponTrail* weaponTrail;
                if (GetTC(&weaponTrail))
                    weaponTrail->DrawAddWeaponTrails(info);
                const CTCCreatureHitNotification* hitNotification;
                if (PeekTC(&hitNotification))
                    hitNotification->DrawHitNotification(info);
            }
            if (HasTC(TC_EMOTE_ICON))
                GetTC(TC_EMOTE_ICON).DrawPreparePrimitivesForRendering(info);
            if (PDef->Property == (EThingCreatureProperty)4) {
                long graphic = PeekDefinitionManager().PGlobalDef->E3BodgeFireflyGraphic;
                CTCPhysicsBase* physics = PhysicsTC;
                info.Engine->AddPrimitive(CEnginePrimitiveSprite(physics->GetPosition(info.Interpolate.Current.WFInterpolate),
                    graphic, 0, 0, 0.0f, 0.4f, 0.4f, (EEnginePrimitiveBlendMode)3, 0xff, 0xff, 0xff, 0xff, 0.0f,
                    (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0));
            }
        } else {
            PTCGraphicAppearance->ClearAllPrimitivesDueToBeingOutOfRange();
            CTCEmoteIcon* emoteIcon;
            if (GetTC(&emoteIcon))
                emoteIcon->ClearPrimitives();
        }
    }
    return true;
    }
    return false;
}