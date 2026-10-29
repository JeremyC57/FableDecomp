// CTCCreatureGenerator::GetUndeadGenerationPoint(CThing* creature, C3DVector* out) -- up to three tries: pick a random point
// 1.5 .. GenerationRadius in front of the creature, turned by a random angle, that is on the map, passable, outside the
// exclusion area, on the ground within 1 unit of the creature's height and (when it navigates) reachable; true when found
// Leaf: GetUndeadGenerationPoint
//
// Retail 0x0074A260, 630 bytes (`ret 8`, 8-aligned frame).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (CTCCreatureNavigation 7), CTCBase::GetWorld @ 0x00686A70, CWorld::
// GetPRandomSeed @ 0x0049D810, GFROR13 @ 0x00497890 (__fastcall), the physics' slot 70 (+0x118, the forward vector by value),
// C3DVector::RotateXY @ 0x00A14040, CThing::GetPos @ 0x004C73D0, __ftol2 @ 0x00BFEA70, CWorld slot 12 (+0x30, the
// world map), CWorldMap::IsPosInMap @ 0x004FC5B0 / IsPosPassable @ 0x004FCBD0 / GetHeightZBelowPos @ 0x0063E480,
// CTCShapeManager::IsPosInAnyShape @ 0x00760870, CTCCreatureNavigation::IsAbleToNavigateFromPosToPos @ 0x006CB620.
// Constants: 1.5f 0x0125AADC, 0.01f 0x01230A5C, 2^32 0x0122DCB4, 0.2f 0x012305A0, 1.0f 0x0122DED8, 0.5f 0x0122F59C.
// Layout: CTCCreatureGenerator GenParams.GenerationRadius +0x28, PExclusionArea +0xa4 (retail; PDB +0x38 / +0xc0);
// CThing bitset +0x20, map +0x44, PhysicsTC +0x60.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
public:
    void RotateXY(float);   // 0x00A14040
};
inline C3DVector operator*(float s, const C3DVector& v) { return C3DVector(s * v.X, s * v.Y, s * v.Z); }
class C2DCoordI { public: long X, Y; };
#pragma intrinsic(fabs)
extern "C" double __cdecl fabs(double);
long __fastcall GFROR13(unsigned long);       // 0x00497890
inline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}
class CWorldMap { public:
    bool IsPosInMap(const C2DCoordI& pos) const;             // 0x004FC5B0
    bool IsPosPassable(const C2DCoordI& pos) const;          // 0x004FCBD0
    float GetHeightZBelowPos(const C3DVector& pos) const;    // 0x0063E480
};
#define VF(n) virtual void v##n();
class CWorld { public:
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11)
    virtual const CWorldMap& GetWorldMap();                  // slot 12 (+0x30)
    unsigned long* GetPRandomSeed();                         // 0x0049D810
};
class CTCPhysicsBase { public:
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18)
    VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30) VF(31) VF(32) VF(33) VF(34) VF(35)
    VF(36) VF(37) VF(38) VF(39) VF(40) VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49) VF(50) VF(51) VF(52)
    VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59) VF(60) VF(61) VF(62) VF(63) VF(64) VF(65) VF(66) VF(67) VF(68) VF(69)
    virtual C3DVector GetForward();                          // slot 70 (+0x118)
};
#undef VF
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;              // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                        // +0x20
    CInterfaceMap   m_interfaces;                            // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    const C3DVector& GetPos() const;                    // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTCConst(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (T*)((it == map->last || type < it->first) ? map->last : it)->second;
    }
};
class CTCCreatureNavigation { public:
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance);   // 0x006CB620
};
class CTCShapeManager { public:
    bool IsPosInAnyShape(const C3DVector& pos) const;        // 0x00760870
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCCreatureGenerator : public CTCBase { public:
    CThing*          ParentThing;                            // +0x04
    unsigned char    _pad08[0x28 - 0x08];
    float            GenerationRadius;                       // +0x28 GenParams
    unsigned char    _pad2c[0xa4 - 0x2c];
    CTCShapeManager* PExclusionArea;                         // +0xa4
    bool GetUndeadGenerationPoint(CThing* creature, C3DVector* out);
};

bool CTCCreatureGenerator::GetUndeadGenerationPoint(CThing* creature, C3DVector* out)
{
    CTCCreatureNavigation* navigation = 0;
    if (creature->HasTC(7))
        navigation = creature->GetTCConst<CTCCreatureNavigation>(7);
    for (int attempt = 0; attempt < 3; ++attempt) {
        float distance = ((GenerationRadius - 1.5f) * 0.01f) * (float)(GFRandom(GetWorld().GetPRandomSeed()) % 100) + 1.5f;
        float angle = (float)(GFRandom(GetWorld().GetPRandomSeed()) % 10) * 0.2f - 1.0f;
        *out = distance * creature->PhysicsTC->GetForward();
        out->RotateXY(angle * angle * 0.5f);
        const C3DVector& position = creature->GetPos();
        out->X = position.X + out->X;
        out->Y = position.Y + out->Y;
        out->Z = position.Z + out->Z;
        C2DCoordI coord;
        coord.X = (long)out->X;
        coord.Y = (long)out->Y;
        if (!GetWorld().GetWorldMap().IsPosInMap(coord))
            continue;
        if (!GetWorld().GetWorldMap().IsPosPassable(coord))
            continue;
        if (PExclusionArea && PExclusionArea->IsPosInAnyShape(*out))
            continue;
        out->Z += 1.0f;
        float height = GetWorld().GetWorldMap().GetHeightZBelowPos(*out);
        out->Z = height;
        if (!((float)fabs(height - creature->GetPos().Z) < 1.0f))
            continue;
        if (!navigation)
            return true;
        if (navigation->IsAbleToNavigateFromPosToPos(creature->GetPos(), *out, 0.0f))
            return true;
    }
    return false;
}