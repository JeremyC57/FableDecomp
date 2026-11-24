#include <cstdio>

class CProceduralTexture;
class CWorld;

// NF335 (NF324 Q2): the row now defines the PDB member CWorld::DrawGetProceduralTexture
// (?DrawGetProceduralTexture@CWorld@@UBEPBVCProceduralTexture@@XZ).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CWorld { public:
    virtual const CProceduralTexture* DrawGetProceduralTexture() const;
};
// NF335: `this` was a literal pointer; it is now real, zero-initialised storage (the member reads none of it, so the
// call stays the case the landed test checked, without a call through an invalid pointer).
static unsigned long g_CWorldStorage[4];

int main()
{
    CWorld *w = (CWorld*)g_CWorldStorage;
    CProceduralTexture const *r = ((CWorld*)(w))->CWorld::DrawGetProceduralTexture();
    if (r == 0)
        std::printf("004ae890_TEST PASS\n");
    else
        std::printf("FAIL %p\n", (const void*)r);
    return 0;
}