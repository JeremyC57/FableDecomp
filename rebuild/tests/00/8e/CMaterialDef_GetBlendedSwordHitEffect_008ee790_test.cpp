// Behaviour test for 008ee790: src/008ee790.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CMaterialDef::GetBlendedSwordHitEffect(long material, float speed, long* particle1, long* particle2, float* blend)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CMaterialDef::GetBlendedSwordHitEffect(long material, float speed, long* particle1, long* particle2, float* blend) -- look up the blended sword-hit particle pair for the struck material (falling back to the first entry when the material is absent), return both particle indices and a blend factor tanh(|speed| * 1.83 / max sword-hit speed)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EE790, length 152 bytes.
// Callees: the STLport map<long, CBlendedParticleEffectSet>::find @ 0x008EE890 (the manifest's BinarySearchTree_FindLE; iterator by hidden
//   pointer), CGameDefinitionManager::Get @ 0x0044C6B0 (`__fastcall`), _CItanh @ 0x00BFECE2 (the tanh intrinsic); float 0x01288228 = 1.83f.
// Layout: the blended-effect map sits at +0x8c in retail (header +0x8c, node count +0x90; STLport node value at +0x10, mapped set at +0x14);
//   CBlendedParticleEffectSet (PDB) Particle1Index +0, Particle1Value +4, Particle2Index +8, Particle2Value +0xc.
// Shape: the STLport map find has its same-TU body (auto_inline off), so the returned iterator does not escape and shares its frame
//   word with the double spill; Get() is bound to a reference before the blend expression `fabs(speed) * (1.83f / max)` (float
//   speed through math.h fabs(double)), the particle stores follow, then `*blend = (float)tanh(x)`.
// Leaf: GetBlendedSwordHitEffect
#include <math.h>
class CBlendedParticleEffectSet { public:
    long  Particle1Index;
    float Particle1Value;
    long  Particle2Index;
    float Particle2Value;
};
struct SMapNode {
    long      _M_color;
    SMapNode* _M_parent;
    SMapNode* _M_left;
    SMapNode* _M_right;
    long      first;
    CBlendedParticleEffectSet second;
};
struct SMapIterator {
    SMapNode* _M_node;
    SMapIterator() { }
    SMapIterator(const SMapIterator& o) : _M_node(o._M_node) { }
};
struct CBlendedEffectMap {
    SMapNode*     _M_header;
    unsigned long _M_node_count;
    SMapIterator find(const long& key);            // 0x008EE890
    bool empty() const { return _M_node_count == 0; }
};
struct CCombatConstantsDef {
    unsigned char _pad[0x14c];
    float m_maxSwordHitSpeed;
};
class CGameDefinitionManager { public:
    unsigned char _pad[0xd0];
    CCombatConstantsDef* m_pCombatConstants;
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
class CMaterialDef { public:
    unsigned char _pad[0x8c];
    CBlendedEffectMap BlendedSwordHitEffects;
    bool GetBlendedSwordHitEffect(long material, float speed, long* particle1, long* particle2, float* blend);
};
// STLport _Rb_tree::find (0x008EE890), same TU: lower-bound walk from the root, end() when the key is absent.
#pragma auto_inline(off)
SMapIterator CBlendedEffectMap::find(const long& key)
{
    SMapNode* y = _M_header;
    SMapNode* x = _M_header->_M_parent;
    while (x != 0) {
        if (!(x->first < key)) {
            y = x;
            x = x->_M_left;
        } else
            x = x->_M_right;
    }
    if (y == _M_header || key < y->first)
        y = _M_header;
    SMapIterator r;
    r._M_node = y;
    return r;
}
#pragma auto_inline(on)

bool CMaterialDef::GetBlendedSwordHitEffect(long material, float speed, long* particle1, long* particle2, float* blend)
{
    const CBlendedParticleEffectSet* set = 0;
    SMapIterator it = BlendedSwordHitEffects.find(material);
    if (it._M_node != BlendedSwordHitEffects._M_header)
        set = &it._M_node->second;
    else if (!BlendedSwordHitEffects.empty())
        set = &BlendedSwordHitEffects._M_header->_M_left->second;
    if (set) {
        CGameDefinitionManager& mgr = CGameDefinitionManager::Get();
        double x = fabs(speed) * (1.83f / mgr.m_pCombatConstants->m_maxSwordHitSpeed);
        *particle1 = set->Particle1Index;
        *particle2 = set->Particle2Index;
        *blend = (float)tanh(x);
        return true;
    }
    return false;
}

// ---- stubs and driver ----
static CCombatConstantsDef g_constants; static CGameDefinitionManager g_mgr; static int g_getCalls = 0;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { ++g_getCalls; return g_mgr; }
static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }
int main()
{
    g_constants.m_maxSwordHitSpeed = 3.66f; g_mgr.m_pCombatConstants = &g_constants;
    SMapNode header, n3, n7;
    memset(&header, 0, sizeof header); memset(&n3, 0, sizeof n3); memset(&n7, 0, sizeof n7);
    n3.first = 3; n3.second.Particle1Index = 31; n3.second.Particle2Index = 32;
    n7.first = 7; n7.second.Particle1Index = 71; n7.second.Particle2Index = 72;
    n7._M_left = &n3; n3._M_parent = &n7; n7._M_parent = &header;
    header._M_parent = &n7; header._M_left = &n3; header._M_right = &n7;
    CMaterialDef def; def.BlendedSwordHitEffects._M_header = &header; def.BlendedSwordHitEffects._M_node_count = 2;
    long p1 = 0, p2 = 0; float blend = -1.0f;
    // present key: speed -2 -> |speed| * 1.83 / 3.66 = 1 -> tanh(1)
    bool ok = def.GetBlendedSwordHitEffect(7, -2.0f, &p1, &p2, &blend);
    CHECK(ok && p1 == 71 && p2 == 72 && Near(blend, 0.76159416f) && g_getCalls == 1);
    // absent key falls back to the leftmost entry; speed 0 -> tanh(0) = 0
    p1 = p2 = 0; blend = -1.0f;
    ok = def.GetBlendedSwordHitEffect(5, 0.0f, &p1, &p2, &blend);
    CHECK(ok && p1 == 31 && p2 == 32 && blend == 0.0f && g_getCalls == 2);
    // the smaller key is found directly
    ok = def.GetBlendedSwordHitEffect(3, 1.0f, &p1, &p2, &blend);
    CHECK(ok && p1 == 31 && Near(blend, (float)tanh(0.5)));
    // empty map: false, outputs untouched, no definition lookup
    SMapNode emptyHeader; memset(&emptyHeader, 0, sizeof emptyHeader);
    CMaterialDef none; none.BlendedSwordHitEffects._M_header = &emptyHeader; none.BlendedSwordHitEffects._M_node_count = 0;
    p1 = 99; blend = 5.0f;
    ok = none.GetBlendedSwordHitEffect(7, 2.0f, &p1, &p2, &blend);
    CHECK(!ok && p1 == 99 && blend == 5.0f && g_getCalls == 3);
    if (fails == 0) printf("008ee790_TEST PASS\n");
    return fails;
}
