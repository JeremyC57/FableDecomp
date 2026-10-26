// CThingFilter_In2DRangeLimit::operator()(CThing* thing) -- true when the thing has a physics component and its XY distance from Pos is at least MinRange and below MaxRange
// Leaf: ??RCThingFilter_In2DRangeLimit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7700, length 115 bytes (+ int3 padding).
// Callees: none (x87 only).
// Layout: PDB CThingFilter_In2DRangeLimit { C3DVector Pos; float MinRange (+0x0c); float MaxRange (+0x10); } (20 bytes).
// Retail CThing holds PhysicsTC at +0x60; the component's position is the C3DVector at +0x0C (same model as the
// landed sibling CThingFilter_In2DRange 0x006A76B0).
// Shape: each range test recomputes both deltas from memory, so the test is a file-static inline helper over two
// C3DVector references (a member helper or one function body lets VC7.1 CSE the deltas through the frame);
// `return !A && B;` gives the int `mov eax,1` / `xor eax,eax` tails, the absent component the byte `xor al,al`.
class C3DVector { public:
    float x, y, z;
};

class CTCPhysicsBase { public:
    void*     __vftable;
    unsigned char _pad04[8];
    C3DVector m_position;                           // +0x0c
};

class CThing { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
};

class CThingFilter_In2DRangeLimit { public:
    C3DVector Pos;                                  // +0x00
    float     MinRange;                             // +0x0c
    float     MaxRange;                             // +0x10
    bool operator()(CThing* thing) const;
};

static inline bool In2DRange(const C3DVector& a, const C3DVector& b, float range)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx * dx + dy * dy < range * range;
}

bool CThingFilter_In2DRangeLimit::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (physics) {
        return !In2DRange(Pos, physics->m_position, MinRange) && In2DRange(Pos, physics->m_position, MaxRange);
    }
    return false;
}