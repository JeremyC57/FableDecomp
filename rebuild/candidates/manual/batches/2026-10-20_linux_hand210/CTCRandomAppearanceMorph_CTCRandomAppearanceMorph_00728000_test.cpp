// Behaviour test for the CTCRandomAppearanceMorph constructor (0x00728000): base owner set, members cleared, a fresh empty morph
// set handed to the counted pointer, Seed = the world seed before the step, the world seed advanced (x*0x24A1+0x24DF, ror 13)
// and the appearance-component subobject registered with the owner's graphic appearance.
#include <cstdio>
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
unsigned long GFROR13(unsigned long value) { return Ror13(value) & 0xFFFFFFFFUL; }
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct CMainGameComponent { unsigned long WorldSeed; };
static CMainGameComponent g_component;
namespace NGraphicAppearance {
    struct CPrimitive;
    struct CAGraphicAppearanceComponent {
        virtual void v00() { }
        CAGraphicAppearanceComponent() { }
    };
}
static const void* g_registered = 0; static int g_registrations = 0;
struct CTCGraphicAppearance {
    void RegisterComponent(NGraphicAppearance::CAGraphicAppearanceComponent* component) { g_registered = component; ++g_registrations; }
};
struct CThing {
    CTCGraphicAppearance* PTCGraphicAppearance;
    const CMainGameComponent& PeekComponent() const { return g_component; }
    unsigned long* GetPWorldSeed() const { return const_cast<unsigned long*>(&PeekComponent().WorldSeed); }
};
inline unsigned long GFNextRandomSeed(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}
struct CTCBase {
    virtual void v00() { }
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing* owner) : ParentThing(owner), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};
struct CTCSkeletalMorph;
struct CTCAppearanceMorphBase {
    struct CMeshMorphSet { vector<long> MeshMorphs; vector<long> TextureMorphs; };
};
static int g_resets = 0;
template <class T> struct CCountedPointer {
    T*    Object;
    void* Info;
    CCountedPointer() : Object(0), Info(0) { }
    void Reset(T* object) { ++g_resets; Object = object; }
};
struct CTCRandomAppearanceMorph : CTCBase, NGraphicAppearance::CAGraphicAppearanceComponent {
    CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet> PMeshMorphSet;
    unsigned long     Seed;
    vector<NGraphicAppearance::CPrimitive*> PrimHandles;
    CTCSkeletalMorph* PTCSkeletalMorph;
    virtual void v00() { }
    CTCRandomAppearanceMorph(CThing& thing);
};

CTCRandomAppearanceMorph::CTCRandomAppearanceMorph(CThing& thing)
    : CTCBase(&thing), CAGraphicAppearanceComponent(), PTCSkeletalMorph(0)
{
    PMeshMorphSet.Reset(new CTCAppearanceMorphBase::CMeshMorphSet);
    Seed = *thing.GetPWorldSeed();
    GFNextRandomSeed(thing.GetPWorldSeed());
    ParentThing->PTCGraphicAppearance->RegisterComponent(this);
}

int main()
{
    int bad = 0;
    CTCGraphicAppearance appearance; CThing thing; thing.PTCGraphicAppearance = &appearance;
    g_component.WorldSeed = 12345;
    CTCRandomAppearanceMorph* m = new CTCRandomAppearanceMorph(thing);
    unsigned long expected = GFROR13(12345UL * 0x24a1 + 0x24df);
    if (m->ParentThing != &thing || m->Seed != 12345 || g_component.WorldSeed != expected) bad |= 1;
    if (g_resets != 1 || m->PMeshMorphSet.Object == 0 || m->PMeshMorphSet.Info != 0) bad |= 2;
    if (m->PMeshMorphSet.Object->MeshMorphs._M_start != 0 || m->PMeshMorphSet.Object->TextureMorphs._M_end_of_storage != 0) bad |= 4;
    if (m->PrimHandles._M_start != 0 || m->PrimHandles._M_finish != 0 || m->PTCSkeletalMorph != 0) bad |= 8;
    if (g_registrations != 1 || g_registered != static_cast<NGraphicAppearance::CAGraphicAppearanceComponent*>(m)) bad |= 16;
    delete m->PMeshMorphSet.Object;
    delete m;
    if (bad) { std::printf("00728000_TEST FAIL\n"); return 1; }
    std::printf("00728000_TEST PASS\n");
    return 0;
}
