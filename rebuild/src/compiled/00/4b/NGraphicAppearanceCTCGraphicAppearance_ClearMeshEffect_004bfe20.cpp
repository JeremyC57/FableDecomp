// NGraphicAppearance::CTCGraphicAppearance::ClearMeshEffect(void* owner) -- remove the mesh effect registered by the owner, first clearing it from the appearance of every carried thing it spread to (weapons only when it affects weapons), then re-render
// Leaf: ClearMeshEffect
//
// Retail 0x004BFE20, 320 bytes incl. padding (`ret 4`). Ego_r.pdb `?ClearMeshEffect@CTCGraphicAppearance@NGraphicAppearance@@
// QAEXPAX@Z`.
// Callees: the MeshEffects map's find(const key&) @ 0x004C3760 (iterator by hidden pointer; landed as
// CRetailTree_ulongkey::find; its body sits above as a same-TU stand-in), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined out-parameter
// GetInterface with the end/hit ternary in the store: CTCCarrying type 0x46, the carried thing's CTCGraphicAppearance type
// 0x5B; the weapon test is type 0x48, `test dh,1`), CTCCarrying::GetAllCarriedThings @ 0x006A2220, itself recursively, free
// @ 0x00BFEA14, the map's erase(iterator) @ 0x004C4C70 (iterator by value, built in its argument slot),
// CThing::AddToRenderList @ 0x004C7AC0.
// Layout: retail CTCGraphicAppearance MeshEffects +0x0c (STLport map: header-node pointer first), ParentThing +0x04,
// UpdatePrimitiveReasons +0x58; the map node holds the key at +0x10 and CMeshEffectPriorityEntry at +0x14
// (EffectsCarriedItems +0x1c, EffectsWeapons +0x1d), as the landed InheritMeshEffectsFromCarrier 0x004C0430.
// Shape: the SetLightingChannel (0x004BFB40) carried-thing walk, guarded by the found entry's EffectsCarriedItems and
// re-reading the iterator (a frame local) for the weapons flag each pass; the iterator class has a user copy ctor so the
// erase argument is built in its stack slot (`push ecx ; mov eax,esp ; mov [eax],ebp`).
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    void AddToRenderList();                                 // 0x004C7AC0
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

template <class T> struct CAllocator { CAllocator() { } };  // STLport allocator<T> (empty)
template <class T> struct CAllocProxy : CAllocator<T> {     // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                     // STLport _Vector_base<T, allocator<T> >
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
    ~CVectorBase() { if (_M_start) free(_M_start); }
};
template <class T> struct CRetailVector : CVectorBase<T> {  // STLport vector<T> (12 bytes)
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) { }
    T* begin() const { return this->_M_start; }
    unsigned int size() const { return this->_M_finish - this->_M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};

struct CTCCarrying {
    void GetAllCarriedThings(CRetailVector<CThing*>& things) const;   // 0x006A2220
};

namespace NGraphicAppearance {

struct CTCGraphicAppearance {
    struct CMeshEffectPriorityEntry {
        void*         PEffect;                              // +0x0 CIVCountedPointer<CEngineMeshEffect>
        long          Priority;                             // +0x4 EAppearanceMeshEffectPriority
        bool          EffectsCarriedItems;                  // +0x8
        bool          EffectsWeapons;                       // +0x9
        bool          InheritedFromCarrier;                 // +0xa
    };
    struct SNode {                                          // STLport _Rb_tree_node<pair<void* const, entry>>
        long   _M_color;                                    // +0x00
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
            iterator(SNode* node) : _M_node(node) { }
            iterator(const iterator& other) : _M_node(other._M_node) { }
            SNode* operator->() const { return _M_node; }
            bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
        };
        iterator end() { return iterator(_M_header); }
        iterator find(void* const& key);                    // 0x004C3760
        void erase(iterator position);                      // 0x004C4C70
    };

    void*          __vftable;                               // +0x00 (CTCBase)
    CThing*        ParentThing;                             // +0x04
    unsigned char  _pad08[0x0c - 0x08];
    CMeshEffectMap MeshEffects;                             // +0x0c
    unsigned char  _pad18[0x58 - 0x18];
    unsigned char  UpdatePrimitiveReasons;                  // +0x58
    void ClearMeshEffect(void* owner);
};

#pragma auto_inline(off)
// Same-TU stand-in for the map's find 0x004C3760 (the landed CRetailTree_ulongkey::find body: Dinkumware-style lower bound,
// then end() when the bound's key is greater): compiled above the caller so VC7.1 knows, as retail's link-time code
// generation did, that the key's address does not escape -- the owner argument is then kept in a register across the loop.
CTCGraphicAppearance::CMeshEffectMap::iterator CTCGraphicAppearance::CMeshEffectMap::find(void* const& key)
{
    SNode* result = _M_header;
    SNode* n = _M_header->_M_parent;
    while (n) {
        if (!((unsigned long)n->first < (unsigned long)key)) {
            result = n;
            n = n->_M_left;
        }
        else
            n = n->_M_right;
    }
    SNode* found = result;
    if (found == _M_header || (unsigned long)key < (unsigned long)found->first)
        found = _M_header;
    return iterator(found);
}
#pragma auto_inline(on)

void CTCGraphicAppearance::ClearMeshEffect(void* owner)
{
    CMeshEffectMap::iterator it = MeshEffects.find(owner);
    if (it != MeshEffects.end()) {
        CTCCarrying* carrying;
        if (it->second.EffectsCarriedItems && ParentThing->GetInterface(&carrying, 0x46)) {
            CRetailVector<CThing*> carried;
            carrying->GetAllCarriedThings(carried);
            for (unsigned int i = 0; i < carried.size(); ++i) {
                CThing* thing = carried[i];
                CTCGraphicAppearance* appearance;
                if ((it->second.EffectsWeapons || !thing->HasTC(0x48)) && thing->GetInterface(&appearance, 0x5b))
                    appearance->ClearMeshEffect(owner);
            }
        }
        MeshEffects.erase(it);
    }
    ParentThing->AddToRenderList();
    UpdatePrimitiveReasons |= 4;
}

}