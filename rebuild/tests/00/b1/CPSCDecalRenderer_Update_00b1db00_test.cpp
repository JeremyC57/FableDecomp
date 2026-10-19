#include <cstdio>
// Behaviour test for 00b1db00 CPSCDecalRenderer::Update: fresh decals get 30 fps * life frames (or are pooled and deleted
// when pooling is on and succeeds), expired or inactive decals are deleted, live ones age by one frame (their collision state
// reset on the update tick) and the alive flag is raised.
static int g_pools = 0;
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
    int deleted;
    CDecalIterator DeleteDecal(CDecalIterator& it)
    { ++deleted; _List_node<CDecal*>* n = it._M_node; n->_M_prev->_M_next = n->_M_next; n->_M_next->_M_prev = n->_M_prev; return CDecalIterator(n->_M_next); }
};
long GetUpdateRate_00aea9f0() { return 30; }
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
    bool PoolDecal(CParticleSystem* system, CDecalIterator& it) { ++g_pools; return (*it)->MinLife == 1.0f; }
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

typedef _List_node<CDecal*> Node;
int main()
{
    static CDecal d[5];
    for (int i = 0; i < 5; ++i) { d[i].Active = true; d[i].MaxLife = 100; d[i].Lifetimer = 0; d[i].UpdateTimer = -1; d[i].MinLife = 0; d[i].SizeCollisionThisFrame = true; d[i].AlphaCollisionThisFrame = true; }
    d[0].MaxLife = -1;                               // fresh: life = 30 * 2.0 = 60
    d[1].Lifetimer = 100;                            // expired: deleted
    d[2].Active = false;                             // inactive: deleted
    d[3].UpdateTimer = 0; d[3].SizeFactor[0] = 5; d[3].SizeFactor[1] = 0; d[3].AlphaFactor[0] = 7; d[3].AlphaFactor[1] = 0;
    d[4].MaxLife = -1; d[4].MinLife = 1.0f;          // fresh, pooled when pooling is enabled
    Node header, n[5];
    Node* prev = &header;
    for (int i = 0; i < 5; ++i) { n[i]._M_data = &d[i]; prev->_M_next = &n[i]; n[i]._M_prev = prev; prev = &n[i]; }
    prev->_M_next = &header; header._M_prev = prev;
    static CParticleSystem sys; sys.Decals = &header; sys.deleted = 0;
    static CPSCDecalRenderer r; r.DecalLifeSecs = 2.0f; r.PoolingEnable = 1;
    bool alive = false;
    r.Update(&sys, 0.033f, alive);
    bool ok = alive && sys.deleted == 3 && g_pools == 2;
    ok = ok && d[0].MaxLife == 60 && d[0].Lifetimer == 1;
    ok = ok && d[3].Lifetimer == 1 && d[3].SizeFactor[1] == 5 && d[3].AlphaFactor[1] == 7 && !d[3].SizeCollisionThisFrame && !d[3].AlphaCollisionThisFrame;
    ok = ok && d[0].SizeCollisionThisFrame && header._M_next == &n[0] && n[0]._M_next == &n[3] && n[3]._M_next == &header;
    alive = false;
    header._M_next = &header; header._M_prev = &header;
    r.Update(&sys, 0.033f, alive);
    ok = ok && !alive;
    if (!ok) { std::printf("00b1db00_TEST FAIL\n"); return 1; }
    std::printf("00b1db00_TEST PASS\n");
    return 0;
}