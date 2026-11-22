// CMaterialDef::GetBlendedSwordHitEffect(long material, float speed, long& particle1, long& particle2, float& blend) const -- look up the blended sword-hit particle pair for the struck material (falling back to the first entry when the material is absent), return both particle indices and a blend factor tanh(|speed| * 1.83 / max sword-hit speed)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EE790, length 152 bytes.
// Callees: the STLport map<long, CBlendedParticleEffectSet>::find @ 0x008EE890 (the manifest's BinarySearchTree_FindLE; iterator by hidden
//   pointer), CGameDefinitionManager::Get @ 0x0044C6B0 (`__fastcall`), _CItanh @ 0x00BFECE2 (the tanh intrinsic); float 0x01288228 = 1.83f.
// Layout: the blended-effect map sits at +0x8c in retail (header +0x8c, node count +0x90; STLport node value at +0x10, mapped set at +0x14);
//   CBlendedParticleEffectSet (PDB) Particle1Index +0, Particle1Value +4, Particle2Index +8, Particle2Value +0xc.
// Shape: the STLport map find has its same-TU body (auto_inline off), so the returned iterator does not escape and shares its frame
//   word with the double spill; Get() is bound to a reference before the blend expression `fabs(speed) * (1.83f / max)` (float
//   speed through math.h fabs(double)), the particle stores follow, then `*blend = (float)tanh(x)`.
// Leaf: ?GetBlendedSwordHitEffect@CMaterialDef@@QBE_NJMAAJ0AAM@Z
// Name (NF237, land1122auth24): the const member with reference outputs, Ego_r.pdb + FableWin.pdb publics
//   ?GetBlendedSwordHitEffect@CMaterialDef@@QBE_NJMAAJ0AAM@Z (engine/CMaterialDef.h FABLE_RETAIL_CMATERIALDEF_SWORDHITEFFECTS declares it);
//   landed 0x00839420 binds that spelling. The const this reaches the map through the const find, as retail 0x008EE890 is the
//   STLport const _Rb_tree::find (its row leaf). In-place respell of the HEAD local models; bytes unchanged.
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
    SMapIterator find(const long& key) const;      // 0x008EE890
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
    bool GetBlendedSwordHitEffect(long material, float speed, long& particle1, long& particle2, float& blend) const;
};
// STLport _Rb_tree::find (0x008EE890), same TU: lower-bound walk from the root, end() when the key is absent.
#pragma auto_inline(off)
SMapIterator CBlendedEffectMap::find(const long& key) const
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

bool CMaterialDef::GetBlendedSwordHitEffect(long material, float speed, long& particle1, long& particle2, float& blend) const
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
        particle1 = set->Particle1Index;
        particle2 = set->Particle2Index;
        blend = (float)tanh(x);
        return true;
    }
    return false;
}