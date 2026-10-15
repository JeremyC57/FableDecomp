// CTCSmokeGenerator::CTCSmokeGenerator(CThing* parent) -- thing-component base, zero generation distance, empty position vector, positions not created
// Leaf: CTCSmokeGenerator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00722540, length 48 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite", propagated).
// Layout: Ego_r.pdb CTCSmokeGenerator (0x24 bytes) with the retail 12-byte vector: GenerationDistance +0x0c,
// GenerationPositions (vector<CGenerationPos>) +0x10/+0x14/+0x18, PositionsCreated +0x1c. Vtable 0x01265D74 =
// CTCSmokeGenerator (rtti-vtables.tsv).
// Shape: the distance (an init-list member) is stored before the vtable; the vector is a member object with an inline
// constructor and so is stored after it.
struct CThing;

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* parent);                                // 0x00686800
};

struct CGenerationPos;

struct CGenerationPosVector {                               // retail std::vector: first, last, end
    CGenerationPos* m_pFirst;
    CGenerationPos* m_pLast;
    CGenerationPos* m_pEnd;
    CGenerationPosVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CTCSmokeGenerator : CTCBase {
    float                GenerationDistance;                // +0x0c
    CGenerationPosVector GenerationPositions;               // +0x10
    bool                 PositionsCreated;                  // +0x1c
    virtual void v00();                                     // own vtable 0x01265D74
    CTCSmokeGenerator(CThing* parent);
};

CTCSmokeGenerator::CTCSmokeGenerator(CThing* parent)
    : CTCBase(parent)
    , GenerationDistance(0)
    , PositionsCreated(false)
{
}
