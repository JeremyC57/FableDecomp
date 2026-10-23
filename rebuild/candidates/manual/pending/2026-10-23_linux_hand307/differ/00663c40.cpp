// Grade: DIFFER(573v577) -- only residue: the two non-virtual TC calls (0x4c DrawAddWeaponTrails, 0xcd weapon-trail render) - retail loads the component into eax, pushes the context, then mov ecx,eax (2 bytes each); ours loads ecx directly
// CThingCreatureBase::DrawPreparePrimitivesForRendering(context) -- let TC 0xd9 prepare its primitives, run the
// CThingPhysical base pass (false stops here); with a graphic TC, test the camera distance against the fade distance
// (scaled by 1 + the global fade bias): in range, prepare the appearance, add weapon trails when the global trail switch
// is on (TCs 0x4c / 0xcd), let TC 0x45 prepare, and for creature defs of type 4 add a sprite at the interpolated
// position; out of range, clear the appearance's primitives and TC 0x45's handle
// Leaf: ?DrawPreparePrimitivesForRendering@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663C40, `ret 4`.
// Callees: LowerBound 0x0040F020 (const overload 0x004365B0 for TC 0xcd), CThingPhysical::DrawPreparePrimitivesForRendering
//   0x006A64D0, CTCGraphicAppearance GetFadeDistance 0x004BC180 / PrepareForDraw 0x004BE4E0 /
//   ClearAllPrimitivesDueToBeingOutOfRange 0x004BF960, CThing getters 0x004C7AA0 (display engine, slot 8 = camera
//   position by value) and 0x004C7990, CThing::GetPosition 0x004C73D0, GFWorld3DDistanceSmallerThan 0x006614F0
//   (__fastcall), CTCWeaponTrail::DrawAddWeaponTrails 0x0076E6A0, weapon-trail render 0x00864FF0, TC 0x45 handle clear
//   0x00669390, physics interpolated-position getter 0x0049D150, CEnginePrimitiveSprite ctor 0x006558CE.
// Layout: retail CThing interface bitset +0x20 / map +0x44, physics TC +0x60, graphic TC +0x64; retail
//   CThingCreatureBase def +0xe0 (type +0xf0). Context: +0x4 long, +0x34 primitive list (slot 20 adds a primitive).
//   Globals: fade bias float 0x013B8774, weapon-trail switch byte 0x0137548E.
struct C3DVector { float X, Y, Z; };
struct CDrawContext;
struct CTCBase {
#define VF(n) virtual void v##n();
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10)
    virtual void DrawPreparePrimitivesForRendering(CDrawContext* context);   // slot 11 (+0x2c)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    SEntry* LowerBound(const long* type) const;         // 0x004365B0
};
struct CTCGraphicAppearance {
    float GetFadeDistance();                            // 0x004BC180
    void PrepareForDraw(CDrawContext* context);         // 0x004BE4E0
    void ClearAllPrimitivesDueToBeingOutOfRange();      // 0x004BF960
};
struct CTCPhysicsStandard {
    C3DVector GetInterpolatedPosition(float time) const;   // 0x0049D150
};
struct CTCWeaponTrail : CTCBase { void DrawAddWeaponTrails(CDrawContext* context); };    // 0x0076E6A0
struct CTCWeaponTrailRender : CTCBase { void RenderWeaponTrails(CDrawContext* context); };   // 0x00864FF0
struct CTCHandleOwner : CTCBase { void ClearHandle(); };          // 0x00669390
struct CDisplayEngine {
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7)
    virtual C3DVector GetCameraPosition() const;       // slot 8 (+0x20), by value
};
struct CSpriteSource { unsigned char _pad[0xd4]; long Value; };          // +0xd4
struct CSpriteOwner { unsigned char _pad[0xd0]; CSpriteSource* Source; };   // +0xd0
struct CEnginePrimitiveSprite {
    unsigned char _body[0x48];
    CEnginePrimitiveSprite(const C3DVector& position, long a, long c, long d, long e, float width, float height,
                           long mode, long red, long green, long blue, long alpha, long f, long g, long h);   // 0x006558CE
};
struct CPrimitiveList {
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9)
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19)
    virtual void AddPrimitive(const CEnginePrimitiveSprite& sprite);   // slot 20 (+0x50)
};
struct CDrawContext {
    long            _pad00;
    float           Time;                               // +0x04
    unsigned char   _pad08[0x34 - 0x08];
    CPrimitiveList* Primitives;                         // +0x34
};
struct CCreatureDefView { unsigned char _pad[0xf0]; long Type; };   // +0xf0
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance);   // 0x006614F0
extern float g_FadeDistanceBias;                       // 0x013B8774
extern bool  g_DrawWeaponTrails;                       // 0x0137548E

class CThing {
public:
    void* m_vtable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];                   // +0x20
    CInterfaceMap Components;                           // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsStandard*   PhysicsTC;                    // +0x60
    CTCGraphicAppearance* GraphicTC;                    // +0x64
    unsigned char _pad68[0x94 - 0x68];
    __forceinline bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CDisplayEngine* GetDisplayEngine() const;           // 0x004C7AA0
    CSpriteOwner* GetSpriteOwner() const;               // 0x004C7990
};
class CThingPhysical : public CThing {
public:
    bool DrawPreparePrimitivesForRendering(CDrawContext* context);   // 0x006A64D0
};
class CThingCreatureBase : public CThingPhysical {
public:
    unsigned char     _pad94[0xe0 - 0x94];
    CCreatureDefView* Def;                              // +0xe0
    bool DrawPreparePrimitivesForRendering(CDrawContext* context);
};

bool CThingCreatureBase::DrawPreparePrimitivesForRendering(CDrawContext* context)
{
    if (HasTC(0xd9))
        GetTC(0xd9)->DrawPreparePrimitivesForRendering(context);
    if (CThingPhysical::DrawPreparePrimitivesForRendering(context)) {
    if (GraphicTC) {
        float distance = g_FadeDistanceBias;
        distance = GraphicTC->GetFadeDistance() * (distance + 1.0f);
        CDisplayEngine* engine = GetDisplayEngine();
        if (GFWorld3DDistanceSmallerThan(engine->GetCameraPosition(), GetPosition(), distance)) {
            GraphicTC->PrepareForDraw(context);
            if (g_DrawWeaponTrails) {
                if (HasTC(0x4c))
                    static_cast<CTCWeaponTrail*>(GetTC(0x4c))->DrawAddWeaponTrails(context);
                if (HasTC(0xcd))
                    static_cast<CTCWeaponTrailRender*>(((const CThing*)this)->GetTC(0xcd))->RenderWeaponTrails(context);
            }
            if (HasTC(0x45))
                GetTC(0x45)->DrawPreparePrimitivesForRendering(context);
            if (Def->Type == 4) {
                long value = GetSpriteOwner()->Source->Value;
                CTCPhysicsStandard* physics = PhysicsTC;
                context->Primitives->AddPrimitive(CEnginePrimitiveSprite(physics->GetInterpolatedPosition(context->Time),
                    value, 0, 0, 0, 0.4f, 0.4f, 3, 0xff, 0xff, 0xff, 0xff, 0, 0, 0));
            }
        } else {
            GraphicTC->ClearAllPrimitivesDueToBeingOutOfRange();
            if (HasTC(0x45))
                static_cast<CTCHandleOwner*>(GetTC(0x45))->ClearHandle();
        }
    }
    return true;
    }
    return false;
}
