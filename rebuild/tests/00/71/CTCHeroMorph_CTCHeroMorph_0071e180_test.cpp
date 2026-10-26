// Behaviour test for 0071e180: body copied from src/0071e180.cpp (CTCHeroMorph ctor), callees stubbed.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_baseCtor = 0, g_resets = 0;
class CThing;
class CTCBase { public:
    virtual void v00() {}
    CThing* ParentThing;                             // +0x04
    bool    ToBeRemoved;                             // +0x08
    bool    IsInGlobalUpdateTCs;                     // +0x09
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { ++g_baseCtor; }
};

template <class P> struct CAllocProxy {              // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    P p;
    CAllocProxy(P v) : p(v) { }
};
template <class T> struct CRetailVector {            // STLport _Vector_base, 12 bytes
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T*> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct CMeshMorphSet {                               // CTCAppearanceMorphBase::CMeshMorphSet, 24 bytes: two vectors
    CRetailVector<long> Morphs;
    CRetailVector<long> Weights;
    CMeshMorphSet() { }
};

struct CCountedPointerMeshMorphSet {                 // CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>
    CMeshMorphSet* Data;
    void*          Info;
    CCountedPointerMeshMorphSet() : Data(0), Info(0) {}
    void Reset(CMeshMorphSet* p) { ++g_resets; Data = p; }
};

struct SEmitterNode { SEmitterNode* _M_next; SEmitterNode* _M_prev; unsigned char _M_data[8]; };
struct CEmitterList {                                // std::list<CIntelligentPointer<CThing>>: the header-node pointer
    SEmitterNode* _M_node;
    CEmitterList() : _M_node(0)
    {
        SEmitterNode* node = (SEmitterNode*)malloc(sizeof(SEmitterNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};

class CThingBaseDef;
struct CTattooVector {                               // std::vector<CDefPointer<const CThingBaseDef>>
    CThingBaseDef** _M_start;
    CThingBaseDef** _M_finish;
    CThingBaseDef** _M_end_of_storage;
    CTattooVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};

struct STattooMapNode { bool _M_color; STattooMapNode* _M_parent; STattooMapNode* _M_left; STattooMapNode* _M_right; unsigned char _M_value[0x24]; };
struct CTattooMap {                                  // std::map<CCharString, pair<long, CDateAndTime>>
    STattooMapNode* _M_header;
    unsigned long   _M_node_count;
    unsigned char   _M_key_compare;                  // empty less<CCharString> (padded to +0x0C)
    CTattooMap() : _M_header(0)
    {
        _M_header = (STattooMapNode*)malloc(sizeof(STattooMapNode));
        _M_node_count = 0;
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};

class CTCHeroMorph : public CTCBase { public:                      // vtable 0x01265CF4
    CCountedPointerMeshMorphSet PMeshMorphSet;       // +0x0C
    CEmitterList  ParticleEmitters;                  // +0x14
    CEmitterList  IdleParticleEmitters;              // +0x18
    CTattooVector Tattoos;                           // +0x1C
    CTattooMap    CustomToBankIndex;                 // +0x28
    long          FrameTattoosChanged;               // +0x34
    long          FrameHeroBecameIdle;               // +0x38
    bool          IdleParticlesAdded;                // +0x3C
    bool          Changed;                           // +0x3D
    float         Strength;                          // +0x40
    float         Berserk;                           // +0x44
    float         Will;                              // +0x48
    float         Skill;                             // +0x4C
    float         Age;                               // +0x50
    float         Morality;                          // +0x54
    float         Fatness;                           // +0x58
    float         Tan;                               // +0x5C
    bool          Teenager;                          // +0x60
    virtual void v00() {}
    CTCHeroMorph(CThing* thing);
};

CTCHeroMorph::CTCHeroMorph(CThing* thing)
    : CTCBase(thing), FrameTattoosChanged(0), FrameHeroBecameIdle(0), IdleParticlesAdded(false), Changed(false),
      Strength(0.0f), Berserk(0.0f), Will(0.0f), Skill(0.0f), Age(0.0f), Morality(0.5f), Fatness(0.0f), Tan(0.0f),
      Teenager(false)
{
    PMeshMorphSet.Reset(new CMeshMorphSet);
}

int main()
{
    unsigned char raw[sizeof(CTCHeroMorph)]; memset(raw, 0xAA, sizeof(raw)); int thing = 0;
    CTCHeroMorph* tc = new (raw) CTCHeroMorph((CThing*)&thing);
    CHECK(g_baseCtor == 1 && tc->ParentThing == (CThing*)&thing && g_resets == 1 && tc->PMeshMorphSet.Data != 0 && tc->PMeshMorphSet.Info == 0);
    CMeshMorphSet* set = tc->PMeshMorphSet.Data;
    CHECK(set->Morphs._M_start == 0 && set->Morphs._M_finish == 0 && set->Morphs._M_end_of_storage.p == 0 && set->Weights._M_start == 0 && set->Weights._M_finish == 0 && set->Weights._M_end_of_storage.p == 0);
    CHECK(tc->ParticleEmitters._M_node != 0 && tc->ParticleEmitters._M_node->_M_next == tc->ParticleEmitters._M_node && tc->IdleParticleEmitters._M_node->_M_prev == tc->IdleParticleEmitters._M_node);
    CHECK(tc->Tattoos._M_start == 0 && tc->Tattoos._M_finish == 0 && tc->Tattoos._M_end_of_storage == 0);
    CHECK(tc->CustomToBankIndex._M_node_count == 0 && tc->CustomToBankIndex._M_header->_M_left == tc->CustomToBankIndex._M_header && tc->CustomToBankIndex._M_header->_M_parent == 0 && tc->CustomToBankIndex._M_header->_M_color == false);
    CHECK(tc->FrameTattoosChanged == 0 && tc->FrameHeroBecameIdle == 0 && tc->IdleParticlesAdded == false && tc->Changed == false);
    CHECK(tc->Strength == 0.0f && tc->Berserk == 0.0f && tc->Will == 0.0f && tc->Skill == 0.0f && tc->Age == 0.0f && tc->Morality == 0.5f && tc->Fatness == 0.0f && tc->Tan == 0.0f && tc->Teenager == false);
    if (fails == 0) printf("0071e180_TEST PASS\n"); else printf("0071e180_TEST FAIL\n");
    return fails;
}