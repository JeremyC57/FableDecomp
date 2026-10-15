// Behaviour test for CTCSmokeGenerator::CTCSmokeGenerator (0x00722540): zero distance, empty vector, not created.
#include <cstdio>

struct CThing { int id; };
static CThing* g_baseParent = 0;

struct CTCBase {
    CThing* ParentThing;
    virtual ~CTCBase() { }
    CTCBase(CThing* parent) : ParentThing(parent) { g_baseParent = parent; }
};

struct CGenerationPos;

struct CGenerationPosVector {
    CGenerationPos* m_pFirst;
    CGenerationPos* m_pLast;
    CGenerationPos* m_pEnd;
    CGenerationPosVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CTCSmokeGenerator : CTCBase {
    float                GenerationDistance;
    CGenerationPosVector GenerationPositions;
    bool                 PositionsCreated;
    CTCSmokeGenerator(CThing* parent);
};

CTCSmokeGenerator::CTCSmokeGenerator(CThing* parent)
    : CTCBase(parent)
    , GenerationDistance(0)
    , PositionsCreated(false)
{
}

int main()
{
    CThing thing;
    CTCSmokeGenerator g(&thing);
    if (g_baseParent != &thing || g.ParentThing != &thing || g.GenerationDistance != 0.0f || g.GenerationPositions.m_pFirst
        || g.GenerationPositions.m_pLast || g.GenerationPositions.m_pEnd || g.PositionsCreated) {
        std::printf("00722540_TEST FAIL\n"); return 1;
    }
    std::printf("00722540_TEST PASS\n");
    return 0;
}