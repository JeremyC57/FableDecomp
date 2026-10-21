// Grade: DIFFER(353v353) -- everything but the x87 operand order of three of the six cross-product multiplies (VC7.1 canonicalises commutative fmul operands; tried a free CrossProduct, member Cross, swapped factors, reference locals for Up/Forward)
// CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::CThingFilter_IsAICreatureMatchingTypeAndFactionNearby(const CThingAICreature& creature, float maxDistance, bool restrictAngle) -- capture the searching creature's position, squared search distance, creature type and faction; with an angle restriction also its side vector (flipped for creature type 1) and the 45-degree minimum cosine
// Leaf: ??0CThingFilter_IsAICreatureMatchingTypeAndFactionNearby
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00986ED0, length 352 bytes (+16 int3), `ret 0Ch`, returns `this`.
//   Signature from Ego_r.pdb `??0CThingFilter_IsAICreatureMatchingTypeAndFactionNearby@@QAE@ABVCThingAICreature@@M_N@Z`.
// Callees: CThing::GetPosition() const @ 0x004C73D0, CThingCreatureBase::GetCreatureType() const @ 0x00662000 (PDef
//   +0xc4; manifest "CPixelShader::CountReferences" is propagated), the const CVectorMap LowerBound copy @ 0x004365B0
//   (components 0x49 and 2, read uninitialised when absent), component 2's virtual slot 72 (+0x120, its orientation
//   CRightHandedSet). PI/4 (double) @ 0x01261E50 through the fcos intrinsic.
// Layout: Ego_r.pdb {RestrictAngle +0, OurPos +1, MaxSqDist +0x10, SideVec +0x14, OurCreatureType +0x20, PFaction +0x24,
//   MinCosine +0x28} (44 bytes): OurPos sits at +1, so this TU's C3DVector is byte-aligned (pack(1)) while the filter
//   keeps natural packing; the faction component (0x49 = bit 9 of the bitset word +0x28) holds PFaction at +0x18.
#include <math.h>
#pragma intrinsic(cos)

#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
};
#pragma pack(pop)
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(b.Z * a.Y - a.Z * b.Y, b.X * a.Z - b.Z * a.X, a.X * b.Y - b.X * a.Y);
}
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

struct CTCBase;
struct CFaction;
struct CTCFaction {
    unsigned char _pad00[0x18];
    CFaction*     PFaction;                             // +0x18
};
struct CTCOrientation {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetOrientation() const;   // slot 72 (+0x120)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline const T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (const T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline const T* PeekTC(long type) const
    {
        const T* component;
        if (HasTC(type))
            component = GetTC<T>(type);
        return component;
    }
};
struct CThingCreatureBase : CThing {
    long GetCreatureType() const;                       // 0x00662000
};
struct CThingAICreature : CThingCreatureBase { };

class CThingFilter_IsAICreatureMatchingTypeAndFactionNearby {
public:
    bool      RestrictAngle;                            // +0x00
    C3DVector OurPos;                                   // +0x01
    float     MaxSqDist;                                // +0x10
    C3DVector SideVec;                                  // +0x14
    long      OurCreatureType;                          // +0x20 ECreatureType
    CFaction* PFaction;                                 // +0x24
    float     MinCosine;                                // +0x28
    CThingFilter_IsAICreatureMatchingTypeAndFactionNearby(const CThingAICreature& creature, float maxDistance, bool restrictAngle);
};

CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::CThingFilter_IsAICreatureMatchingTypeAndFactionNearby(
    const CThingAICreature& creature, float maxDistance, bool restrictAngle)
{
    MinCosine = 0.0f;
    RestrictAngle = restrictAngle;
    OurPos = creature.GetPosition();
    MaxSqDist = maxDistance * maxDistance;
    OurCreatureType = creature.GetCreatureType();
    PFaction = creature.PeekTC<CTCFaction>(0x49)->PFaction;
    if (restrictAngle) {
        const CRightHandedSet& orientation = creature.PeekTC<CTCOrientation>(2)->GetOrientation();
        const C3DVector& up = orientation.Up;
        const C3DVector& forward = orientation.Forward;
        SideVec = C3DVector(forward.Y * up.Z - forward.Z * up.Y, forward.Z * up.X - forward.X * up.Z, forward.X * up.Y - forward.Y * up.X);
        if (OurCreatureType == 1)
            SideVec = -SideVec;
        MinCosine = (float)cos(0.78539818525314331);
    }
}
