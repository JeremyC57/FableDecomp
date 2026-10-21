// CTCLightningOrb::FrameUpdate() -- move the orb along its spline (eased); when it reaches the end, outlives LifeTime or enters the target's bounding sphere it explodes (when the def names an explosion) and the orb thing is killed
// Grade: DIFFER(453v453) -- scheduling only: the hitLocations loads are ordered differently
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);   // slot 30 (+0x78)
    void SetAsMoving(bool moving);                      // 0x006B0130
};
class CThing {
public:
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];                   // +0x20 interface-present bitset
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCHitLocations {                                // interface 0x7F
    void GetBoundingSphere(C3DVector* centre, float* radius, bool flag);   // 0x00815420
};
CThing* __fastcall CreateExplosion(long defIndex, const C3DVector* position);   // 0x0080C460

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* PItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
namespace NBBBMaths {
struct CLinearSpline {
    C3DVector GetPointWithParam(float t);               // 0x00A57F70
};
}
struct CLightningOrbDef {
    unsigned char _pad00[0x28];
    long  LightningDefIndex;                            // +0x28
    float LifeTime;                                     // +0x2c
    float Speed;                                        // +0x30
    float ExplosionRange;                               // +0x34
    long  ExplosionDefIndex;                            // +0x38
};
struct CTCLightningOrb {
    void*                    __vftable;                 // +0x00
    CThing*                  ParentThing;               // +0x04
    unsigned char            _pad08[0x0c - 0x08];
    CBaseIntelligentPointer  PParent;                   // +0x0c
    CBaseIntelligentPointer  PTarget;                   // +0x14
    const CLightningOrbDef*  PDef;                      // +0x1c
    long                     Timer;                     // +0x20
    long                     LifeTime;                  // +0x24
    long                     NumFramesForSplineTraversal;   // +0x28
    NBBBMaths::CLinearSpline* PSpline;                  // +0x2c (CCountedPointer data)
    void*                    PSplineBlock;              // +0x30
    void FrameUpdate();
};

void CTCLightningOrb::FrameUpdate()
{
    if (PDef) {
        bool hit = false;
        CTCHitLocations* hitLocations;
        if (IS_THING_ALIVE(PTarget) && PTarget.GetPItem()->GetInterface(&hitLocations, 0x7f)) {
            float range = PDef->ExplosionRange;
            float radius;
            C3DVector centre;
            hitLocations->GetBoundingSphere(&centre, &radius, true);
            if ((centre - ParentThing->GetPosition()).GetLengthSquared() < range * range)
                hit = true;
        }
        if ((LifeTime >= 0 && Timer > LifeTime) || hit) {
            const CLightningOrbDef* def = PDef;
            if (def->ExplosionDefIndex > 0)
                CreateExplosion(def->ExplosionDefIndex, &ParentThing->GetPosition());
        } else {
            ++Timer;
            float t = (float)Timer / NumFramesForSplineTraversal;
            t = (t * t + (t + t)) * 0.33333334f;
            if (!(t > 1.0f)) {
                if (t < 0.0f)
                    t = 0.0f;
                else if (t > 1.0f)
                    t = 1.0f;
                C3DVector position;
                position = PSpline->GetPointWithParam(t);
                ParentThing->PhysicsTC->SetPosition(position);
                ParentThing->PhysicsTC->SetAsMoving(true);
                return;
            }
        }
        ParentThing->Kill(true);
    }
}
