// Behaviour test for CTCSmokeGenerator::Copy (0x00722570): GenerationDistance and the GenerationPositions vector are copied
// from the other component; PositionsCreated and the CTCBase part are left alone.
#include <cstdio>
class CThing { public: int id; };
struct CGenerationPos { float x, y, z; CThing* thing; };
struct PosVector {                                      // stand-in for STLport vector<CGenerationPos>
    CGenerationPos items[8]; unsigned count;
    PosVector() : count(0) {}
    PosVector& operator=(const PosVector& o)
    {
        if (&o != this) { count = o.count; for (unsigned i = 0; i < count; ++i) items[i] = o.items[i]; }
        return *this;
    }
};
class CTCBase {
public:
    CTCBase() : ParentThing(0) {}
    virtual ~CTCBase() {}
    virtual void Copy(const CTCBase& other) = 0;
    CThing* ParentThing;
};
class CTCSmokeGenerator : public CTCBase {
public:
    CTCSmokeGenerator() : GenerationDistance(0.0f), PositionsCreated(false) {}
    virtual void Copy(const CTCBase& other)
    {
        const CTCSmokeGenerator& o = (const CTCSmokeGenerator&)other;
        GenerationDistance = o.GenerationDistance;
        GenerationPositions = o.GenerationPositions;
    }
    float GenerationDistance;
    PosVector GenerationPositions;
    bool PositionsCreated;
};
int main()
{
    CThing t;
    CTCSmokeGenerator src, dst;
    src.GenerationDistance = 12.5f; src.PositionsCreated = true; src.ParentThing = &t;
    src.GenerationPositions.count = 2;
    src.GenerationPositions.items[0].x = 1.0f; src.GenerationPositions.items[1].thing = &t;
    CTCBase& base = dst;
    base.Copy(src);
    float diff = dst.GenerationDistance - 12.5f;
    if (diff < -0.001f || diff > 0.001f || dst.GenerationPositions.count != 2 || dst.GenerationPositions.items[1].thing != &t ||
        dst.GenerationPositions.items[0].x != 1.0f || dst.PositionsCreated || dst.ParentThing != 0) {
        std::printf("00722570_TEST FAIL\n"); return 1;
    }
    std::printf("00722570_TEST PASS\n");
    return 0;
}