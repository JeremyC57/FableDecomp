#include <cstdio>

class CEngineBillboardGenerator;

// NF335 (NF324 Q2): the row now defines the PDB member CEngineBillboardGenerator::PeekSceneFilterFlags
// (?PeekSceneFilterFlags@CEngineBillboardGenerator@@UBEKXZ).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CEngineBillboardGenerator { public:
    virtual unsigned long PeekSceneFilterFlags() const;
};
// NF335: `this` was a literal pointer; it is now real, zero-initialised storage (the member reads none of it, so the
// call stays the case the landed test checked, without a call through an invalid pointer).
static unsigned long g_CEngineBillboardGeneratorStorage[4];

int main()
{
    unsigned long r = ((CEngineBillboardGenerator*)(g_CEngineBillboardGeneratorStorage))->CEngineBillboardGenerator::PeekSceneFilterFlags();
    if (r == 1)
        std::printf("00b8c780_TEST PASS\n");
    else
        std::printf("FAIL %lu\n", r);
    return 0;
}