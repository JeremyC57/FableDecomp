#include <cstdio>
// Behaviour test for CTCHitLocations::GetPCombatFirstHitLocationPosInside (retail 0x008153C0).
class CThing { public: int id; };
class C3DVector { public: float X, Y, Z; };

namespace NHitLocations {
class CPhysicalPrimitiveBase { public:
    float m_minX, m_maxX;                               // a slab test along X stands in for the shape
    virtual bool IsPosInside(const CThing& owner, const C3DVector& pos) const
    {
        (void)owner;
        return pos.X >= m_minX && pos.X <= m_maxX;
    }
    virtual ~CPhysicalPrimitiveBase() { }
};
struct CHitLocationDef { CPhysicalPrimitiveBase* PhysicalPrimitive; };
class CHitLocation { public:
    CThing*                POwnerThing;
    const CHitLocationDef* PDef;
    void*                  PArmourDef;
    long                   HitResponseAction;
    long                   BlockResponseAction;
    bool                   Enabled;
};
}

struct SHitLocationEntry { NHitLocations::CHitLocation first; long second; };

class CTCHitLocations { public:
    SHitLocationEntry* HitLocations_First;
    SHitLocationEntry* HitLocations_Last;
    SHitLocationEntry* HitLocations_End;
    NHitLocations::CHitLocation* GetPCombatFirstHitLocationPosInside(const C3DVector& pos);
};

NHitLocations::CHitLocation* CTCHitLocations::GetPCombatFirstHitLocationPosInside(const C3DVector& pos)
{
    NHitLocations::CHitLocation* best = 0;
    long bestPriority = 0x7fffffff;
    for (SHitLocationEntry* it = HitLocations_First; it != HitLocations_Last; ++it) {
        if (it->first.Enabled && it->first.PDef->PhysicalPrimitive->IsPosInside(*it->first.POwnerThing, pos)) {
            if (best == 0 || it->second < bestPriority) {
                bestPriority = it->second;
                best = &it->first;
            }
        }
    }
    return best;
}

static int Fail() { std::printf("008153c0_TEST FAIL\n"); return 1; }

int main()
{
    CThing owner; owner.id = 1;
    NHitLocations::CPhysicalPrimitiveBase prims[4];
    NHitLocations::CHitLocationDef defs[4];
    SHitLocationEntry e[4];
    float ranges[4][2] = { { 0, 10 }, { 0, 10 }, { 5, 6 }, { 0, 10 } };
    long priorities[4] = { 5, 3, 1, 3 };
    bool enabled[4] = { true, true, false, true };
    for (int i = 0; i < 4; ++i) {
        prims[i].m_minX = ranges[i][0]; prims[i].m_maxX = ranges[i][1];
        defs[i].PhysicalPrimitive = &prims[i];
        e[i].first.POwnerThing = &owner; e[i].first.PDef = &defs[i]; e[i].first.PArmourDef = 0;
        e[i].first.HitResponseAction = 0; e[i].first.BlockResponseAction = 0;
        e[i].first.Enabled = enabled[i]; e[i].second = priorities[i];
    }
    CTCHitLocations h; h.HitLocations_First = e; h.HitLocations_Last = e + 4; h.HitLocations_End = e + 4;
    C3DVector p; p.X = 5.5f; p.Y = 0; p.Z = 0;
    // entry 2 has the lowest priority but is disabled; entries 1 and 3 tie at 3: the first wins
    if (h.GetPCombatFirstHitLocationPosInside(p) != &e[1].first) return Fail();
    e[2].first.Enabled = true;
    if (h.GetPCombatFirstHitLocationPosInside(p) != &e[2].first) return Fail();
    p.X = 20.0f;
    if (h.GetPCombatFirstHitLocationPosInside(p) != 0) return Fail();
    h.HitLocations_Last = e;
    p.X = 5.5f;
    if (h.GetPCombatFirstHitLocationPosInside(p) != 0) return Fail();
    std::printf("008153c0_TEST PASS\n");
    return 0;
}