// CTCLightningOrb::FrameUpdate() -- move the orb along its spline (eased); when it reaches the end, outlives LifeTime or enters the target's bounding sphere it explodes (when the def names an explosion) and the orb thing is killed
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Shape: early `if (!PDef) return;` and the explode/kill and spline-overrun paths each `Kill(true); return;` (VC7.1 cross-jumps
//   the two Kill calls; the early-out joins the final SetAsMoving epilogue as retail); ONE function-scope C3DVector serves as
//   the hit-test sphere centre and the new spline position (same frame slot as retail), and the CVectorMap LowerBound
//   instantiation is defined in this TU (noinline).
#include "engine/C3DVector.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCPhysicsBase { public:
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
    unsigned char   _pad54[0xc];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    const C3DVector& GetPos() const;               // 0x004C73D0
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
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CTCHitLocations { public:                                // interface 0x7F
    void GetBoundingSphere(C3DVector* centre, float* radius, bool flag);   // 0x00815420
};
CThing* __fastcall CreateExplosion(long defIndex, const C3DVector* position);   // 0x0080C460

class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* PItem;
    CBaseObject* GetPItem() const;                           // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}
namespace NBBBMaths {
class CLinearSpline { public:
    C3DVector GetPointWithParam(float t);               // 0x00A57F70
};
}
class CLightningOrbDef { public:
    unsigned char _pad00[0x28];
    long  LightningDefIndex;                            // +0x28
    float LifeTime;                                     // +0x2c
    float Speed;                                        // +0x30
    float ExplosionRange;                               // +0x34
    long  ExplosionDefIndex;                            // +0x38
};
class CTCLightningOrb { public:
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
    if (!PDef)
        return;
    bool hit = false;
    CTCHitLocations* hitLocations;
    C3DVector centre;
    float radius;
    if (IS_THING_ALIVE(PTarget) && ((CThing*)PTarget.GetPItem())->GetInterface(&hitLocations, 0x7f)) {
        float range = PDef->ExplosionRange;
        hitLocations->GetBoundingSphere(&centre, &radius, true);
        if ((centre - ParentThing->GetPos()).GetSquaredMagnitude() < range * range)
            hit = true;
    }
    if ((LifeTime >= 0 && Timer > LifeTime) || hit) {
        const CLightningOrbDef* def = PDef;
        if (def->ExplosionDefIndex > 0)
            CreateExplosion(def->ExplosionDefIndex, &ParentThing->GetPos());
        ParentThing->Kill(true);
        return;
    }
    ++Timer;
    float t = (float)Timer / NumFramesForSplineTraversal;
    t = (t * t + (t + t)) * 0.33333334f;
    if (t > 1.0f) {
        ParentThing->Kill(true);
        return;
    }
    if (t < 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;
    centre = PSpline->GetPointWithParam(t);
    ParentThing->PhysicsTC->SetPosition(centre);
    ParentThing->PhysicsTC->SetAsMoving(true);
}