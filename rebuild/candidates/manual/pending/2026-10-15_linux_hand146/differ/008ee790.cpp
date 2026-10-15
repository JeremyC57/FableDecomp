// CMaterialDef::GetBlendedSwordHitEffect(long material, float speed, long* particle1, long* particle2, float* blend) -- look up the blended sword-hit particle pair for the struck material (falling back to the first entry when the material is absent), return both particle indices and a blend factor tanh(|speed| * 1.83 / max sword-hit speed)
// Grade: DIFFER(152v152) -- frame 0xc v 8 (retail shares the iterator's frame word with the 8-byte double spill, ours keeps them apart, shifting every [esp+N]) and the x87 operand order: retail `fld speed ; fabs ; fld 1.83 ; fdiv [def+14Ch]`, ours `fld 1.83 ; fdiv` first
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EE790, length 152 bytes.
// Callees: the STLport map<long, CBlendedParticleEffectSet>::find @ 0x008EE890 (the manifest's BinarySearchTree_FindLE; iterator by hidden
//   pointer), CGameDefinitionManager::Get @ 0x0044C6B0 (`__fastcall`), _CItanh @ 0x00BFECE2 (the tanh intrinsic); float 0x01288228 = 1.83f.
// Layout: the blended-effect map sits at +0x8c in retail (header +0x8c, node count +0x90; STLport node value at +0x10, mapped set at +0x14);
//   CBlendedParticleEffectSet (PDB) Particle1Index +0, Particle1Value +4, Particle2Index +8, Particle2Value +0xc.
// Tried: tanh over an inline float expression (fabsf call + float spill), a named double local (right spill, 0xc frame), the iterator in its
//   own `{}` scope, the operands swapped, the iterator as a temporary (`find(k)._M_node`: frame still 0xc, node read through eax).
#include <math.h>
struct CBlendedParticleEffectSet {
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
struct CGameDefinitionManager {
    unsigned char _pad[0xd0];
    CCombatConstantsDef* m_pCombatConstants;
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
struct CMaterialDef {
    unsigned char _pad[0x8c];
    CBlendedEffectMap BlendedSwordHitEffects;
    bool GetBlendedSwordHitEffect(long material, float speed, long* particle1, long* particle2, float* blend);
};
bool CMaterialDef::GetBlendedSwordHitEffect(long material, float speed, long* particle1, long* particle2, float* blend)
{
    const CBlendedParticleEffectSet* set = 0;
    SMapIterator it = BlendedSwordHitEffects.find(material);
    if (it._M_node != BlendedSwordHitEffects._M_header)
        set = &it._M_node->second;
    else if (!BlendedSwordHitEffects.empty())
        set = &BlendedSwordHitEffects._M_header->_M_left->second;
    if (set) {
        double x = fabs((double)speed) * (1.83f / CGameDefinitionManager::Get().m_pCombatConstants->m_maxSwordHitSpeed);
        *particle1 = set->Particle1Index;
        *particle2 = set->Particle2Index;
        *blend = (float)tanh(x);
        return true;
    }
    return false;
}
