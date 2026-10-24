// NGraphicAppearance::CTCGraphicAppearance::GetPMeshEffect() -- the mesh effect with the highest priority among the registered ones (a counted copy), or a null pointer when none is registered
// Grade: DIFFER(141v141) -- register swap only: retail keeps the header (end) in esi and the best entry in edi, ours the reverse (four declaration orders tried)
// Leaf: GetPMeshEffect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BCF50, length 141 bytes, `ret 4` (hidden CIVCountedPointer result).
// Ego_r.pdb: `?GetPMeshEffect@CTCGraphicAppearance@NGraphicAppearance@@QAE?AV?$CIVCountedPointer@VCEngineMeshEffect@@@@XZ`.
// Callees: the MeshEffects iterator's out-of-line `operator++()` @ 0x004C2560 (thiscall on the iterator, returns it; the
// catalog "RBTree_FindSuccessor"; the hint insert 0x004C5480 calls it too).
// Label: the catalog "Appearance_OnAppearanceDrawHelper" is behavioural.
// Layout: as the landed ClearMeshEffect 0x004BFE20 / SetPMeshEffect 0x004C0240: MeshEffects +0x0c (STLport map, header
// pointer first); a node holds the owner key at +0x10 and CMeshEffectPriorityEntry at +0x14 (PEffect +0, Priority +4).
// Shape: `it = begin(); if (it == end()) return 0; best = &it->second; ++it` (the prefix operator out of line, so the
// iterator lives in the frame) then `for (; it != end(); it++)` with the postfix increment's _M_increment inlined; a
// later entry wins only on a strictly greater priority (`jle`); the result is the counted copy of best->PEffect
// (null test + `inc [p+4]`).
class CEngineMeshEffect {
public:
    virtual void v000();
    long RefCount;                                          // +0x04
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    __forceinline CIVCountedPointer() : Object(0) {}
    __forceinline CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};

namespace NGraphicAppearance {

struct CTCGraphicAppearance {
    struct CMeshEffectPriorityEntry {
        CIVCountedPointer<CEngineMeshEffect> PEffect;      // +0x0
        long          Priority;                             // +0x4 EAppearanceMeshEffectPriority
        bool          EffectsCarriedItems;                  // +0x8
        bool          EffectsWeapons;                       // +0x9
        bool          InheritedFromCarrier;                 // +0xa
    };
    struct SNode {                                          // STLport _Rb_tree_node<pair<void* const, entry>>
        bool   _M_color;                                    // +0x00
        SNode* _M_parent;                                   // +0x04
        SNode* _M_left;                                     // +0x08
        SNode* _M_right;                                    // +0x0c
        void*                    first;                     // +0x10
        CMeshEffectPriorityEntry second;                    // +0x14
    };
    struct CMeshEffectMap {                                 // STLport map: the header-node pointer, count, comparator
        SNode*        _M_header;
        unsigned long _M_node_count;
        unsigned long _M_compare;
        struct iterator {
            SNode* _M_node;
            __forceinline iterator(SNode* node) : _M_node(node) {}
            __forceinline iterator(const iterator& other) : _M_node(other._M_node) {}
            __forceinline SNode* operator->() const { return _M_node; }
            __forceinline bool operator==(const iterator& other) const { return _M_node == other._M_node; }
            __forceinline bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
            __forceinline void _M_increment()
            {
                if (_M_node->_M_right != 0) {
                    _M_node = _M_node->_M_right;
                    while (_M_node->_M_left != 0)
                        _M_node = _M_node->_M_left;
                }
                else {
                    SNode* y = _M_node->_M_parent;
                    while (_M_node == y->_M_right) {
                        _M_node = y;
                        y = y->_M_parent;
                    }
                    if (_M_node->_M_right != y)
                        _M_node = y;
                }
            }
            iterator& operator++();                         // 0x004C2560
            __forceinline iterator operator++(int) { iterator tmp = *this; _M_increment(); return tmp; }
        };
        __forceinline iterator begin() { return iterator(_M_header->_M_left); }
        __forceinline iterator end() { return iterator(_M_header); }
    };

    void*          __vftable;                               // +0x00 (CTCBase)
    void*          ParentThing;                             // +0x04
    unsigned char  _pad08[0x0c - 0x08];
    CMeshEffectMap MeshEffects;                             // +0x0c
    CIVCountedPointer<CEngineMeshEffect> GetPMeshEffect();
};

CIVCountedPointer<CEngineMeshEffect> CTCGraphicAppearance::GetPMeshEffect()
{
    CMeshEffectMap::iterator it = MeshEffects.begin();
    CMeshEffectMap::iterator last = MeshEffects.end();
    if (it == last)
        return CIVCountedPointer<CEngineMeshEffect>();
    CMeshEffectPriorityEntry* best = &it->second;
    ++it;
    for (CMeshEffectMap::iterator i = it; i != last; i++)
        if (i->second.Priority > best->Priority)
            best = &i->second;
    return best->PEffect;
}

}
