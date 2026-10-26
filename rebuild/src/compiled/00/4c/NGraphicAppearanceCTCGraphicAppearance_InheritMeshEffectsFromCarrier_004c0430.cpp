// NGraphicAppearance::CTCGraphicAppearance::InheritMeshEffectsFromCarrier(const CTCGraphicAppearance*) -- copy every carrier mesh effect that also affects carried items onto this appearance, marked as inherited from the carrier
// Leaf: InheritMeshEffectsFromCarrier
//
// Retail 0x004C0430, 133 bytes (`ret 4`). Ego_r.pdb `?InheritMeshEffectsFromCarrier@CTCGraphicAppearance@
// NGraphicAppearance@@QAEXPBV12@@Z`; the catalog "CTCGraphicAppearance::ApplyAllMeshEffects" is a
// descriptive guess. Callee: CTCGraphicAppearance::SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect>,
// bool, bool, bool, EAppearanceMeshEffectPriority, void*) @ 0x004C0240 (`ret 18h`).
// Layout: retail CTCGraphicAppearance MeshEffects (STLport map<void*, CMeshEffectPriorityEntry>, its
// header-node pointer) at +0xc; the tree node is {colour, parent +4, left +8, right +0xc, key +0x10,
// entry +0x14}; CMeshEffectPriorityEntry (Ego_r.pdb, 0xc): PEffect +0, Priority +4,
// EffectsCarriedItems +8, EffectsWeapons +9, InheritedFromCarrier +0xa.
// Shape: the STLport in-order increment `_M_node = _M_increment(_M_node)` inlined on a copy of the node
// (right then leftmost, else climb while a right child; the result moved back into esi);
// end() re-read from the carrier each pass; the by-value CIVCountedPointer is built in the argument area
// by its declared copy constructor (`push ecx ; mov ecx,esp ; mov [ecx],eax ; inc [eax+4]`); the bool
// argument read from memory is `xor edx,edx ; mov dl`.
class CEngineMeshEffect { public:
    virtual void v00();
    long RefCount;                                          // +0x04
};

template<class T> class CIVCountedPointer { public:
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CIVCountedPointer();
};

namespace NGraphicAppearance {

enum EAppearanceMeshEffectPriority { MESH_EFFECT_PRIORITY_DEFAULT };

class CTCGraphicAppearance { public:
    struct CMeshEffectPriorityEntry {
        CIVCountedPointer<CEngineMeshEffect> PEffect;       // +0x0
        EAppearanceMeshEffectPriority        Priority;      // +0x4
        bool                                 EffectsCarriedItems;   // +0x8
        bool                                 EffectsWeapons;        // +0x9
        bool                                 InheritedFromCarrier;  // +0xa
    };
    struct SNode {                                          // STLport _Rb_tree_node<pair<void* const, entry>>
        long   _M_color;                                    // +0x00
        SNode* _M_parent;                                   // +0x04
        SNode* _M_left;                                     // +0x08
        SNode* _M_right;                                    // +0x0c
        void*                    first;                     // +0x10
        CMeshEffectPriorityEntry second;                    // +0x14
    };
    struct CMeshEffectMap {                                 // STLport map: the header-node pointer, count
        SNode*        _M_header;
        unsigned long _M_node_count;
        unsigned long _M_compare;
        struct const_iterator {
            SNode* _M_node;
            const_iterator(SNode* node) : _M_node(node) { }
            bool operator!=(const const_iterator& other) const { return _M_node != other._M_node; }
            const SNode* operator->() const { return _M_node; }
            static SNode* Increment(SNode* x)                // STLport _Rb_global<bool>::_M_increment
            {
                if (x->_M_right != 0) {
                    x = x->_M_right;
                    while (x->_M_left != 0)
                        x = x->_M_left;
                } else {
                    SNode* y = x->_M_parent;
                    while (x == y->_M_right) {
                        x = y;
                        y = y->_M_parent;
                    }
                    if (x->_M_right != y)
                        x = y;
                }
                return x;
            }
            const_iterator& operator++() { _M_node = Increment(_M_node); return *this; }
        };
        const_iterator begin() const { return const_iterator(_M_header->_M_left); }
        const_iterator end() const { return const_iterator(_M_header); }
    };

    virtual void v00();
    void*          ParentThing;                             // +0x04 (CTCBase)
    unsigned char  _pad08[0x0c - 0x08];
    CMeshEffectMap MeshEffects;                             // +0x0c
    void SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect> effect, bool effectsCarriedItems, bool effectsWeapons,
                        bool inheritedFromCarrier, EAppearanceMeshEffectPriority priority, void* owner);   // 0x004C0240
    void InheritMeshEffectsFromCarrier(const CTCGraphicAppearance* carrier);
};

void CTCGraphicAppearance::InheritMeshEffectsFromCarrier(const CTCGraphicAppearance* carrier)
{
    for (CMeshEffectMap::const_iterator it = carrier->MeshEffects.begin(); it != carrier->MeshEffects.end(); ++it)
    {
        if (it->second.EffectsCarriedItems)
            SetPMeshEffect(it->second.PEffect, true, it->second.EffectsWeapons, true, it->second.Priority, it->first);
    }
}

}