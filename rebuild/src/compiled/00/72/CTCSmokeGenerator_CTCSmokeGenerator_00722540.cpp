// CTCSmokeGenerator::CTCSmokeGenerator(CThing* parent) -- thing-component base, zero generation distance, empty position vector, positions not created
// Leaf: ??0CTCSmokeGenerator@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCSmokeGenerator@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00722540, length 48 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite", propagated).
// Layout: Ego_r.pdb CTCSmokeGenerator (0x24 bytes) with the retail 12-byte vector: GenerationDistance +0x0c,
// GenerationPositions (vector<CGenerationPos>) +0x10/+0x14/+0x18, PositionsCreated +0x1c. Vtable 0x01265D74 =
// CTCSmokeGenerator (rtti-vtables.tsv).
// Shape: the distance (an init-list member) is stored before the vtable; the vector is a member object with an inline
// constructor and so is stored after it.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

struct CGenerationPos;

struct CGenerationPosVector {                               // retail std::vector: first, last, end
    CGenerationPos* m_pFirst;
    CGenerationPos* m_pLast;
    CGenerationPos* m_pEnd;
    CGenerationPosVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

class CTCSmokeGenerator : public CTCBase { public:
    float                GenerationDistance;                // +0x0c
    CGenerationPosVector GenerationPositions;               // +0x10
    bool                 PositionsCreated;                  // +0x1c
    CTCSmokeGenerator(CThing& parent);
};

CTCSmokeGenerator::CTCSmokeGenerator(CThing& parent)
    : CTCBase(parent)
    , GenerationDistance(0)
    , PositionsCreated(false)
{
}