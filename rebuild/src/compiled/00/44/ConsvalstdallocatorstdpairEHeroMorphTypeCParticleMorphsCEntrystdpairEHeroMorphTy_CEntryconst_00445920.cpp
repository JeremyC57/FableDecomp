enum EHeroMorphType { MORPH_A_DUMMY };
// Leaf: CEntry>_const&>

class CParticleMorphs
{
public:
    class CEntry;
};

class CParticleMorphs::CEntry
{
public:
    CEntry();
    int field;
};

template<class _Ty1, class _Ty2>
struct fpair_00a03bb0
{
    fpair_00a03bb0();
    _Ty1 first;
    _Ty2 second;
};

typedef fpair_00a03bb0<EHeroMorphType, CParticleMorphs::CEntry> PairT;

// Not defined in this TU: forces a real call, matching retail (the default
// constructor is a separately compiled function, not inlined here).
template<> fpair_00a03bb0<EHeroMorphType, CParticleMorphs::CEntry>::fpair_00a03bb0();

// VC7.1 STL <xmemory> stateless allocator: Cons_val is a member function
// (this = the allocator instance, in ecx, unused because the type carries
// no state) taking the single pointer to default-construct as its only
// real (stack) argument.
struct DummyAllocator
{
    void Cons_val_pair_morph_a(PairT* _Ptr);
};

void DummyAllocator::Cons_val_pair_morph_a(PairT* _Ptr)
{
    _Ptr->fpair_00a03bb0<EHeroMorphType, CParticleMorphs::CEntry>::fpair_00a03bb0();
}