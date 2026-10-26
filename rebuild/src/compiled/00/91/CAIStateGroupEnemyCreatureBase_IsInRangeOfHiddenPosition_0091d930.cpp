// CAIStateGroup_EnemyCreatureBase::IsInRangeOfHiddenPosition(C3DVector*, bool) -- pick the valid hiding position furthest from the creature (optionally only ones on the same side of the target as the creature)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091D930, 544 bytes (ret 8).
// Callees: CAIStateGroupBase::PeekWorldMap 0x008FCFF0 / PeekThingSearchTools 0x008FD000, the state group's virtual slot 18
//   (+0x48, hiding range), GetValidPositions 0x0074A5D0 (__fastcall), CThing::GetPosition 0x004C73D0,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, free 0x00BFEA14 (STLport vector storage).
// Layout: CAIStateGroupBase (engine header, 0x14); CAIStateGroup_EnemyCreatureBase::PTargetThing +0x14 (Ego_r.pdb).
// Shape: C3DVector has a user copy ctor so the candidate and the target position are frame copies; the furthest
// position is kept in registers (its uninitialised home loaded first); the empty test shares the final exit.
#include <stdlib.h>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CPositionVector {                        // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    CPositionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CPositionVector() { if (_M_start) free(_M_start); }
    C3DVector* begin() const { return _M_start; }
    C3DVector* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
};
class CWorldMap;
class CThingSearchTools;
class CTCShapeManager;
void __fastcall GetValidPositions(CPositionVector& positions, const CWorldMap& map, const CThingSearchTools& search,
                                  const C3DVector& pos, float radius, bool flag, const CTCShapeManager* shapes);   // 0x0074A5D0
class CThing { public:
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CThingCreatureBase : public CThing { public: };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CAIBrain { public:
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
class CAIStateGroupBase { public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c(); virtual void v40(); virtual void v44();
    virtual float GetHidingRange();             // slot 18 (+0x48)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    CWorldMap* PeekWorldMap();                  // 0x008FCFF0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CBaseIntelligentPointer PTargetThing;       // +0x14
    bool IsInRangeOfHiddenPosition(C3DVector* out, bool sameSideOfTarget);
};

bool CAIStateGroup_EnemyCreatureBase::IsInRangeOfHiddenPosition(C3DVector* out, bool sameSideOfTarget)
{
    CPositionVector positions;
    GetValidPositions(positions, *PeekWorldMap(), *PeekThingSearchTools(), GetCreature()->GetPosition(), GetHidingRange(),
                      true, 0);
    bool found = false;
    if (!positions.empty()) {
        C3DVector myPos = GetCreature()->GetPosition();
        float bestDistanceSquared = 0.0f;
        C3DVector bestPos;
        for (C3DVector* it = positions.begin(); it != positions.end(); ++it) {
            C3DVector candidate = *it;
            float dx = myPos.X - candidate.X;
            float dy = myPos.Y - candidate.Y;
            float dz = myPos.Z - candidate.Z;
            float distanceSquared = dz * dz + dy * dy + dx * dx;
            if (distanceSquared > bestDistanceSquared) {
                if (!sameSideOfTarget) {
                    bestDistanceSquared = distanceSquared;
                    bestPos = candidate;
                    found = true;
                } else {
                    C3DVector targetPos = PTargetThing.GetPItem()->GetPosition();
                    C3DVector toMe;
                    toMe.X = myPos.X - targetPos.X;
                    toMe.Y = myPos.Y - targetPos.Y;
                    toMe.Z = myPos.Z - targetPos.Z;
                    C3DVector toCandidate;
                    toCandidate.X = candidate.X - targetPos.X;
                    toCandidate.Y = candidate.Y - targetPos.Y;
                    toCandidate.Z = candidate.Z - targetPos.Z;
                    if (toMe.Z * toCandidate.Z + toMe.Y * toCandidate.Y + toMe.X * toCandidate.X > 0.0f) {
                        bestDistanceSquared = distanceSquared;
                        bestPos = candidate;
                        found = true;
                    }
                }
            }
        }
        if (found) {
            *out = bestPos;
            return true;
        }
    }
    return false;
}