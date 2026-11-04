// Behaviour test for the CWorld vector-deleting-destructor adjustor thunk 0x004AE8A0: CWorld derives from CBaseClassNonCopyable and
// CIDrawWorld (at +4, whose virtual destructor is declared after its three draw accessors); deleting the world through a CIDrawWorld
// pointer must run ~CWorld once and free the whole object (the thunk subtracts 4 before jumping to the deleting destructor).
#include <cstdio>
static int g_dtors = 0;
struct CBaseClass { virtual ~CBaseClass() {} };
struct CBaseClassNonCopyable : CBaseClass { };
struct CIDrawWorld {
    virtual const void* DrawGetProceduralTexture() const = 0;
    virtual const void* DrawGetWorldMap() const = 0;
    virtual const void* DrawGetEnvironment() const = 0;
    virtual ~CIDrawWorld() {}
};
struct CWorld : CBaseClassNonCopyable, CIDrawWorld {
    int members[0x40];
    const void* DrawGetProceduralTexture() const { return 0; }
    const void* DrawGetWorldMap() const { return 0; }
    const void* DrawGetEnvironment() const { return 0; }
    ~CWorld() { ++g_dtors; }
};
int main()
{
    CWorld* world = new CWorld;
    CIDrawWorld* draw = world;
    if ((void*)draw == (void*)world) { std::printf("004ae8a0_TEST FAIL\n"); return 1; }
    delete draw;
    if (g_dtors != 1) { std::printf("004ae8a0_TEST FAIL\n"); return 1; }
    std::printf("004ae8a0_TEST PASS\n");
    return 0;
}