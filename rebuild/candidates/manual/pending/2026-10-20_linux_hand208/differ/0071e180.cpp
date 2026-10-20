// CTCHeroMorph::CTCHeroMorph(CThing*) -- empty emitter lists, tattoos and custom-tattoo map, neutral morph values (morality 0.5) and a fresh mesh morph set
// Grade: DIFFER(220v220) -- scheduling only: retail pushes the new CMeshMorphSet and loads `mov ecx,edi` before the sixth zero store of its inline ctor; ours after (tried 6-word array, two vectors, body-assigned last word)
// Leaf: ??0CTCHeroMorph
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071E180, 220 bytes, `ret 4`; vtable 0x01265CF4.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (the two list header nodes and the map
//          header), operator new @ 0x00BFEA1A (the 24-byte CMeshMorphSet), CCountedPointer<CMeshMorphSet>::Reset(T*)
//          @ 0x007222F0 (manifest label is a propagated CIPhysicsFacesSegmentBase instantiation).
// Shape: one zero register (ebx) after the vtable store for every member in declaration order: the counted pointer
// through its address register (edi, reused as `this` of Reset), each STLport list head zeroed then given its
// self-linked 16-byte header, the vector, the map (header zeroed, 0x34-byte header allocated and stored, count
// zeroed, _M_empty_initialize re-reading the header), then the frames, flags and morph floats (Morality 0.5f an
// immediate). The body's `PMeshMorphSet = new CMeshMorphSet` goes through Reset with the null-checked inline
// zeroing constructor -- VC duplicates the Reset call into both arms of the null test.
// Layout: Ego_r.pdb CTCHeroMorph at retail offsets (one-word STLport lists, 12-byte containers): PMeshMorphSet
// +0x0C, ParticleEmitters +0x14, IdleParticleEmitters +0x18, Tattoos +0x1C, CustomToBankIndex +0x28,
// FrameTattoosChanged +0x34 .. Teenager +0x60 -- as in the landed destructor 0x007223D0.
struct CThing;

extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
void* __cdecl operator new(unsigned int size);       // 0x00BFEA1A

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                             // +0x04
    bool    ToBeRemoved;                             // +0x08
    bool    IsInGlobalUpdateTCs;                     // +0x09
    CTCBase(CThing* thing);                          // 0x00686800
};

struct CMeshMorphSet {                               // CTCAppearanceMorphBase::CMeshMorphSet, 24 bytes
    long Words[6];
    CMeshMorphSet() { Words[0] = 0; Words[1] = 0; Words[2] = 0; Words[3] = 0; Words[4] = 0; Words[5] = 0; }
};

struct CCountedPointerMeshMorphSet {                 // CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>
    CMeshMorphSet* Data;
    void*          Info;
    CCountedPointerMeshMorphSet() : Data(0), Info(0) {}
    void Reset(CMeshMorphSet* p);                    // 0x007222F0
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

struct CThingBaseDef;
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

struct CTCHeroMorph : CTCBase {                      // vtable 0x01265CF4
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
    virtual void v00();
    CTCHeroMorph(CThing* thing);
};

CTCHeroMorph::CTCHeroMorph(CThing* thing)
    : CTCBase(thing), FrameTattoosChanged(0), FrameHeroBecameIdle(0), IdleParticlesAdded(false), Changed(false),
      Strength(0.0f), Berserk(0.0f), Will(0.0f), Skill(0.0f), Age(0.0f), Morality(0.5f), Fatness(0.0f), Tan(0.0f),
      Teenager(false)
{
    PMeshMorphSet.Reset(new CMeshMorphSet);
}
