// CCompare_FurthestFromInitPos2D::operator()(CThing* a, CThing* b) const -- sort predicate: live creatures first, then the one further (2D, squared) from its initial position
// Grade: RELOCATION_MATCH -- the 2D offset is a 12-byte vector (X, Y, Z with Z never written: retail used a C3DVector-sized temporary) and its length is accumulated `r = Y*Y; r += X*X` (retail squares Y first)
// Leaf: CCompare_FurthestFromInitPos2D
// Retail: Fable.exe (TLC, VC7.1) @ 0x007483E0, 233 bytes (240 with padding), `ret 8`.
// Callees: CThing::GetPos 0x004C73D0 (const C3DVector&; manifest "CVertexBufferWin32::DoSizeof" is propagated).
// Layout: the thing's flag byte +0x6c (bit 3: is a creature), dead bit +0x91 bit 0; the creature's initial position
//   +0xe8 (X) / +0xec (Y).
// Shape: an inline creature cast `c = 0; if (t && (t->flags & 8)) c = t;` per argument; the alive tests are
//   `!alive(a) && !alive(b) -> false`, `!alive(a) -> false`, `!alive(b) -> true`; the 2D offsets are C2DVector
//   temporaries (the first spilled across the second GetPos call), squared as X*X + Y*Y and compared `>`.
class C3DVector { public:
    float X, Y, Z;
};

class C2DVector { public:
    float X, Y, Z;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLengthSquared() const { float r = Y * Y; r += X * X; return r; }
};

class CThing { public:
    unsigned char _pad00[0x6c];
    unsigned char m_flags6c;                    // +0x6c bit 3: creature
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
};

class CThingCreatureBase : public CThing { public:
    unsigned char _pad92[0xe8 - 0x92];
    float InitialPosX;                          // +0xe8
    float InitialPosY;                          // +0xec
    C2DVector GetOffsetFromInitialPos2D() const
    {
        const C3DVector& position = GetPos();
        return C2DVector(InitialPosX - position.X, InitialPosY - position.Y);
    }
};

inline CThingCreatureBase* AsCreature(CThing* thing)
{
    CThingCreatureBase* creature = 0;
    if (thing && (thing->m_flags6c & 8))
        creature = (CThingCreatureBase*)thing;
    return creature;
}

inline bool IsAlive(const CThing* thing)
{
    return thing && !(thing->m_flags91 & 1);
}

struct CCompare_FurthestFromInitPos2D {
    bool operator()(CThing* a, CThing* b) const;
};

bool CCompare_FurthestFromInitPos2D::operator()(CThing* a, CThing* b) const
{
    if (a == b)
        return false;
    CThingCreatureBase* creatureA = AsCreature(a);
    CThingCreatureBase* creatureB = AsCreature(b);
    if (IsAlive(creatureA) || IsAlive(creatureB)) {
        if (IsAlive(creatureA)) {
            if (!IsAlive(creatureB))
                return true;
            C2DVector offsetA = creatureA->GetOffsetFromInitialPos2D();
            C2DVector offsetB = creatureB->GetOffsetFromInitialPos2D();
            if (offsetA.GetLengthSquared() > offsetB.GetLengthSquared())
                return true;
        }
    }
    return false;
}