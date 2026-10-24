// CIsCreatureIsIntersecting::operator()(CThing* thing) -- predicate: does `thing` (a collidable thing) overlap the owner creature's bounding sphere -- false again when it is an AI creature that is blocking while the owner is busy with an action
// Leaf: ??RCIsCreatureIsIntersecting
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007249C0, 138 bytes (144 with padding), `ret 4`.
// Label correction: the catalog "_Find_if<CThing**,CIsCreatureIsIntersecting>" names the find_if that calls it; the body
//   is the predicate's operator() (Ego_r.pdb: CIsCreatureIsIntersecting { CThing* Owner; }, used by
//   _Find_if / _Remove_if<CThing**, CIsCreatureIsIntersecting>; the retail callers are 0x00724A90 / 0x00724A50).
// Callees: CThing position accessor @ 0x004C73D0 (`return PhysicsTC->Position`, catalog label propagated),
//   CThingCreatureBase::IsPerformingAnyAction() @ 0x006622A0, CThingCreatureBase::IsBlocking() @ 0x00664D00.
// Layout (retail CThing, landed models): m_type +0x10 (2 = an AI creature), PhysicsTC +0x60, ThingTypeFlags +0x6c;
//   the physics component holds the position at +0x0c and the collision radius at +0x34.
// Shape: the result byte lives in bl (false, set true once the spheres overlap); `thing->PhysicsTC` is read before
//   the flag test; the squared distance of the position difference is compared with the squared radius sum
//   (`fcompp ; test ah,41h ; jne`), Owner re-read from `this` for its own radius; the creature check returns false
//   through its own tail.
struct C3DVector {
    float X, Y, Z;
};
struct CTCPhysicsBase {                        // retail layout, only the used fields
    unsigned char _pad00[0xc];
    C3DVector Position;          // +0x0c
    unsigned char _pad18[0x34 - 0x18];
    float Radius;                // +0x34
};
class CThing {                                  // retail layout (landed models), only the used fields
public:
    unsigned char _pad00[0x10];
    unsigned char m_type;        // +0x10
    unsigned char _pad11[0x60 - 0x11];
    CTCPhysicsBase* PhysicsTC;   // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned long ThingTypeFlags; // +0x6c
    const C3DVector& GetPosition() const;   // 0x004C73D0
};
class CThingCreatureBase : public CThing {
public:
    bool IsPerformingAnyAction() const;     // 0x006622A0
    bool IsBlocking() const;                // 0x00664D00
};
struct CIsCreatureIsIntersecting {             // 4 bytes (PDB)
    CThing* Owner;                              // +0x00
    bool operator()(CThing* thing) const;
};
bool CIsCreatureIsIntersecting::operator()(CThing* thing) const
{
    bool result = false;
    CTCPhysicsBase* phys = thing->PhysicsTC;
    if (thing->ThingTypeFlags & 8) {
        const C3DVector& pos = Owner->GetPosition();
        float dx = pos.X - phys->Position.X;
        float dy = pos.Y - phys->Position.Y;
        float dz = pos.Z - phys->Position.Z;
        float r = Owner->PhysicsTC->Radius + phys->Radius;
        if (dx * dx + dy * dy + dz * dz < r * r) {
            result = true;
            if (thing->m_type == 2 && ((CThingCreatureBase*)Owner)->IsPerformingAnyAction() && ((CThingCreatureBase*)thing)->IsBlocking())
                return false;
        }
    }
    return result;
}