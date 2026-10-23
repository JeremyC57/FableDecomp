// Grade: DIFFER(911v951) -- first drafts: flow/calls match; the early clear block uses a fresh zero register (retail xor ebx) and handle in edi, the def flag byte hoisted across branches (retail reloads ParentThing->PDef per branch), projectile list address kept in a frame slot in retail, loop Trail[i] indexing and frame 0x60 v 0x64
// CTCWeaponTrail::DrawAddWeaponTrails(CEnginePrimitivePrepareInfo*) -- unless the thing is beyond its def draw distance from the camera, rebuild the trail primitives: a projectile trail when the projectile trail list holds points, otherwise one weapon trail primitive per trail with points (alpha fading out with inactive ticks)
// Leaf: ?DrawAddWeaponTrails@CTCWeaponTrail
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076E6A0, 951 bytes (+ int3 padding), `ret 4`, frame 0x64.
// Callees: the camera position (slot 8 of the game world behind 0x013B86A0 +0x24); CThing::GetPosition 0x004C73D0;
//   CTCBase::GetWorld 0x00686A80; CWorld::GetWorldFrame 0x0049D870 (static); CTCBase::GetDefinitionManager 0x006869D0;
//   vector<CEnginePrimitiveHandle> erase(first, last) 0x00440D70 / Resize 0x00443F90; LowerBound 0x004365B0 (const,
//   interface 0x77); CThingGameObject::GetWeaponTrailGraphicIndex 0x00839100; GFLimit 0x0048DAE0 (`__fastcall`);
//   __ftol2; CEnginePrimitiveWeaponTrail ctor 0x007703B0; its member dtor 0x004DB5BB; renderer slot 23 (+0x5c).
// Layout (retail CTCWeaponTrail, 12-byte vectors/lists): NoTrails +0x0c, ProjectileWeaponTrailHandle +0x18,
//   WeaponTrailHandles +0x20, TrailType +0x2c, ActiveTrail (bytes) +0x30, InactiveTicks +0x3c, Trail +0x48,
//   ProjectileWeaponTrail +0x60. Def draw distance +0x48, def flags +0x42 (descriptive).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float MagSq() const { return X * X + Y * Y + Z * Z; }
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CTCWeaponGraphicSource {                         // interface 0x77
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual long GetWeaponTrailGraphic();               // slot 32 (+0x80)
};
struct CThingDef {
    virtual void v00();
    virtual void Delete();
    long RefCount;                                      // +0x04
    unsigned char _pad08[0x42 - 0x08];
    unsigned char Flags42;                              // +0x42
    unsigned char _pad43[0x48 - 0x43];
    float DrawDistance;                                 // +0x48
};
struct CThingDefPointer {
    CThingDef* Object;
    CThingDefPointer(CThingDef* p) : Object(p) { if (Object) ++Object->RefCount; }
    ~CThingDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Delete(); } }
    CThingDef* operator->() const { return Object; }
    bool IsSet() const { return Object != 0; }
};
struct CThing {
    unsigned char _pad00[0x18];
    unsigned __int64 GlobalID;                          // +0x18
    unsigned char _pad20[0x2c - 0x20];
    unsigned long Flags2C;                              // +0x2c
    unsigned char _pad30[0x44 - 0x30];
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CThingDef* PDef;                                    // +0x70
    const C3DVector& GetPosition() const;               // 0x004C73D0
    long GetWeaponTrailGraphicIndex();                  // 0x00839100
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || wanted < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
struct CGameWorldCalls {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07();
    virtual void GetCameraPosition(C3DVector& out) const;   // slot 8 (+0x20)
};
struct CEngineCalls {
    unsigned char _pad00[0x24];
    CGameWorldCalls* m_pWorld;                          // +0x24
};
extern CEngineCalls* g_pEngine;                         // 0x013B86A0
extern long g_WeaponTrailAlphaScale;                    // 0x013828A0

struct CEngineInternalPrimitive {
    virtual void v00();
    virtual void Delete();
    long RefCount;                                      // +0x04
    virtual void v02();
    virtual void Remove(struct CEnginePrimitiveHandle* handle);   // slot 3 (+0xc)
};
struct CEnginePrimitiveHandle {
    long m_id;
    CEngineInternalPrimitive* m_pPrimitive;
    void Reset()
    {
        if (m_pPrimitive && m_pPrimitive->RefCount == 1)
            m_pPrimitive->Remove(this);
        CEngineInternalPrimitive* primitive = m_pPrimitive;
        if (primitive) {
            if (--primitive->RefCount == 0)
                primitive->Delete();
            m_pPrimitive = 0;
        }
        m_pPrimitive = 0;
    }
};
struct CHandleVector {
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    CEnginePrimitiveHandle* erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last);   // 0x00440D70
    void Resize(long count);                            // 0x00443F90
    void clear() { erase(_M_start, _M_finish); }
    CEnginePrimitiveHandle& operator[](long i) { return _M_start[i]; }
};
struct CTrailList {                                     // list<CWeaponTrailEntry>: sentinel pointer only
    struct node { node* next; node* prev; };
    node* head;
    bool empty() const { return head->next == head; }
};
struct CMemberMap {
    unsigned char _body[0x0c];
    ~CMemberMap();                                      // 0x004DB5BB
};
struct CEnginePrimitive { };
struct CEnginePrimitiveWeaponTrail : CEnginePrimitive {
    unsigned char _pad00[0x0c];
    CMemberMap Map;                                     // +0x0c
    unsigned char _rest[0x34 - 0x18];
    CEnginePrimitiveWeaponTrail(unsigned __int64 id, CTrailList* trail, unsigned long graphic, unsigned long flags,
                                long type, unsigned char alpha, float fade, float time, float width, float f4,
                                long interpolation, long extra);   // 0x007703B0
};
struct CEnginePrimitiveRenderer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive);   // slot 23 (+0x5c)
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[4];
    float Fade;                                         // +0x04
    unsigned char _pad08[0x34 - 0x08];
    CEnginePrimitiveRenderer* Renderer;                 // +0x34
};
struct CWeaponTrailGraphicDefs {
    unsigned char _pad00[0x64];
    long ProjectileGraphic;                             // +0x64
    long TrailAlpha;                                    // +0x68
    float TrailWidth;                                   // +0x6c
    unsigned char ProjectileAlpha;                      // +0x70
    unsigned char _pad71[3];
    float ProjectileWidth;                              // +0x74
    long FadeTicks;                                     // +0x78
};
struct CDefinitionManager {
    unsigned char _pad00[0xd0];
    CWeaponTrailGraphicDefs* Graphics;                  // +0xd0
};
extern long g_FrameRate;                                // 0x01375550
struct CWorld {
    static long GetWorldFrame();                        // 0x0049D870
    float GetWorldTime() { long frameRate = g_FrameRate; return (float)GetWorldFrame() / frameRate; }
};
void __fastcall GFLimit(float& value, const float& lo, const float& hi);   // 0x0048DAE0

struct CTCWeaponTrail {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x0c - 0x08];
    long NoTrails;                                      // +0x0c
    long NoProjectileTrails;                            // +0x10
    long WeaponTrailGraphicIndex;                       // +0x14
    CEnginePrimitiveHandle ProjectileWeaponTrailHandle; // +0x18
    CHandleVector WeaponTrailHandles;                   // +0x20
    long TrailType;                                     // +0x2c
    unsigned char* ActiveTrail;                         // +0x30
    unsigned char _pad34[0x3c - 0x34];
    long* InactiveTicks;                                // +0x3c
    unsigned char _pad40[0x48 - 0x40];
    CTrailList* Trail;                                  // +0x48
    unsigned char _pad4c[0x60 - 0x4c];
    CTrailList ProjectileWeaponTrail;                   // +0x60
    CWorld* GetWorld();                                 // 0x00686A80
    CDefinitionManager* GetDefinitionManager();         // 0x006869D0
    __forceinline void ClearHandles()
    {
        ProjectileWeaponTrailHandle.Reset();
        WeaponTrailHandles.clear();
    }
    void DrawAddWeaponTrails(CEnginePrimitivePrepareInfo* info);
};

void CTCWeaponTrail::DrawAddWeaponTrails(CEnginePrimitivePrepareInfo* info)
{
    if (CThingDefPointer(ParentThing->PDef).IsSet()) {
        C3DVector camera;
        g_pEngine->m_pWorld->GetCameraPosition(camera);
        C3DVector offset = ParentThing->GetPosition() - camera;
        bool tooFar;
        {
            CThingDefPointer def(ParentThing->PDef);
            tooFar = offset.MagSq() > def->DrawDistance * def->DrawDistance;
        }
        if (tooFar) {
            ClearHandles();
            return;
        }
    }
    float time = GetWorld()->GetWorldTime();
    CWeaponTrailGraphicDefs* defs = GetDefinitionManager()->Graphics;
    ClearHandles();
    CTrailList* projectileTrail = &ProjectileWeaponTrail;
    if (!projectileTrail->empty()) {
        if (ParentThing->PDef->Flags42 & 0x10) {
            long graphic = defs->ProjectileGraphic;
            if (ParentThing->Flags2C & 0x800000)
                graphic = ParentThing->GetTC<CTCWeaponGraphicSource>(0x77)->GetWeaponTrailGraphic();
            float fade = 1.0f - info->Fade;
            if (graphic > 0)
                info->Renderer->AddPrimitive(ProjectileWeaponTrailHandle,
                    CEnginePrimitiveWeaponTrail(ParentThing->GlobalID, projectileTrail, graphic, 0, 1,
                                                defs->ProjectileAlpha, fade, time, defs->ProjectileWidth, 0.0f, 0, 0));
        }
        return;
    }
    if (ParentThing->PDef->Flags42 & 8 && NoTrails > 0) {
        long graphicIndex = ParentThing->GetWeaponTrailGraphicIndex();
        WeaponTrailHandles.Resize(NoTrails);
        for (long i = 0; i < NoTrails; ++i) {
            if (!Trail[i].empty() && graphicIndex > 0) {
                float alpha = 1.0f;
                float fade;
                if (ActiveTrail[i]) {
                    fade = (1.0f - info->Fade) * g_WeaponTrailAlphaScale;
                } else {
                    fade = 0.0f;
                    alpha = 1.0f - (float)InactiveTicks[i] / defs->FadeTicks;
                }
                GFLimit(alpha, 0.0f, 1.0f);
                info->Renderer->AddPrimitive(WeaponTrailHandles[i],
                    CEnginePrimitiveWeaponTrail(ParentThing->GlobalID, &Trail[i], graphicIndex, 0, TrailType,
                                                (unsigned char)(defs->TrailAlpha * alpha), fade, time, defs->TrailWidth,
                                                0.0f, 0, 0));
            }
        }
    }
}
