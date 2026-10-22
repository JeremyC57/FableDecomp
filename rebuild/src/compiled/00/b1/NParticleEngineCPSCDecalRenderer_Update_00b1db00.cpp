// CPSCDecalRenderer::Update(CParticleSystem*, float, bool&) -- age the particle system's decals: give fresh decals their life (or pool them into an existing one), delete expired and inactive decals, reset a decal's collision state on its update tick, and report whether any decal is still alive
// Leaf: ?Update@CPSCDecalRenderer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1DB00, 219 bytes (+ int3 padding; `ret 0Ch`).
// Callees: the update-rate query 0x00AEA9F0 (a long; manifest "GFGetPreMainMemoryUsed" is propagated), CPSCDecalRenderer::
//   PoolDecal(CParticleSystem*, iterator&) 0x00B1D790, CParticleSystem::DeleteDecal(iterator&) 0x00AF5690 (the next iterator
//   returned through the hidden pointer); __ftol2 0x00BFEA70.
// Layout: Ego_r.pdb CPSCDecalRenderer {DecalLifeSecs +0xc, PoolingEnable bit 4 of +0x34}; CParticleSystem Decals
//   (list<CDecal*>, a 4-byte STLport header pointer) at retail +0x20; CDecal {Lifetimer +0x28, MaxLife +0x30, SizeFactor[2]
//   +0x38, AlphaFactor[2] +0x40, UpdateTimer +0x4c, Active / SizeCollisionThisFrame / AlphaCollisionThisFrame bits 0-2 of
//   +0x58} at retail offsets.
// Shape: the decal life in frames is a long local; the walk's iterator lives in the dead system-argument slot; the three
//   `it = DeleteDecal(it)` exits share one call site (each with its own temporary slot for the hidden result).
struct CDecal;
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;                           // +0x08
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* n) : _M_node(n) { }
    T& operator*() const { return _M_node->_M_data; }
    void operator++() { _M_node = _M_node->_M_next; }
};
typedef _List_iterator<CDecal*> CDecalIterator;
struct CDecal {
    void*         __vftable;                       // +0x00
    unsigned char _pad04[0x24];                    // Position, RHSet
    long          Lifetimer;                       // +0x28
    float         CreationTime;                    // +0x2c
    long          MaxLife;                         // +0x30
    unsigned char FlickerIndex;                    // +0x34
    unsigned char FlickerTimeOffset;               // +0x35
    float         SizeFactor[2];                   // +0x38
    float         AlphaFactor[2];                  // +0x40
    float         GraphicFrame;                    // +0x48
    long          UpdateTimer;                     // +0x4c
    float         MinLife;                         // +0x50
    float         MinSize;                         // +0x54
    bool          Active : 1;                      // +0x58 bit 0
    bool          SizeCollisionThisFrame : 1;      // +0x58 bit 1
    bool          AlphaCollisionThisFrame : 1;     // +0x58 bit 2
    bool          LandscapeOnlyDecal : 1;          // +0x58 bit 3
};
class CParticleSystem {
public:
    unsigned char           _pad00[0x20];
    _List_node<CDecal*>*    Decals;                // +0x20 list<CDecal*> header node
    CDecalIterator DeleteDecal(CDecalIterator& it);   // 0x00AF5690
};
long GetUpdateRate_00aea9f0();                     // 0x00AEA9F0
class CPSCDecalRenderer {
public:
    void*         __vftable;                       // +0x00
    unsigned char _pad04[8];
    float         DecalLifeSecs;                   // +0x0c
    unsigned char _pad10[0x24];
    unsigned char AlphaFadeEnable : 1;             // +0x34 bit 0
    unsigned char SizeFadeEnable : 1;              // +0x34 bit 1
    unsigned char ForceAnimationTime : 1;          // +0x34 bit 2
    unsigned char FlickerEnable : 1;               // +0x34 bit 3
    unsigned char PoolingEnable : 1;               // +0x34 bit 4
    bool PoolDecal(CParticleSystem* system, CDecalIterator& it);   // 0x00B1D790
    void Update(CParticleSystem* system, float timeDelta, bool& anyAlive);
};

void CPSCDecalRenderer::Update(CParticleSystem* system, float timeDelta, bool& anyAlive)
{
    long lifeFrames = (long)(GetUpdateRate_00aea9f0() * DecalLifeSecs);
    for (CDecalIterator it(system->Decals->_M_next); it._M_node != system->Decals; ) {
        CDecal* decal = *it;
        if (decal->MaxLife < 0) {
            if (PoolingEnable && PoolDecal(system, it)) {
                it = system->DeleteDecal(it);
                continue;
            }
            decal->MaxLife = lifeFrames;
        }
        if (decal->MaxLife > -1 && decal->Active) {
            if (decal->Lifetimer >= decal->MaxLife) {
                it = system->DeleteDecal(it);
                continue;
            }
            if (decal->UpdateTimer == decal->Lifetimer) {
                decal->AlphaFactor[1] = decal->AlphaFactor[0];
                decal->SizeFactor[1] = decal->SizeFactor[0];
                decal->SizeCollisionThisFrame = false;
                decal->AlphaCollisionThisFrame = false;
            }
            ++decal->Lifetimer;
            ++it;
            anyAlive = true;
        } else
            it = system->DeleteDecal(it);
    }
}