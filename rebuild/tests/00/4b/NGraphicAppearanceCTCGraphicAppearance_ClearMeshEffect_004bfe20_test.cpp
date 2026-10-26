// Behaviour test for NGraphicAppearance::CTCGraphicAppearance::ClearMeshEffect (retail 0x004BFE20): model copy + stubs.
#include <cstdio>
#include <cstdlib>

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    int renderListAdds;
    void AddToRenderList() { ++renderListAdds; }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
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
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end(0) {}
    ~CRetailVector() { if (_M_start) std::free(_M_start); }
    T* begin() const { return _M_start; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) const { return *(begin() + n); }
    void push_back(T v) { if (!_M_start) { _M_start = _M_finish = (T*)std::malloc(sizeof(T) * 8); _M_end = _M_start + 8; } *_M_finish++ = v; }
};
class CTCCarrying { public:
    CThing* things[4]; int count;
    void GetAllCarriedThings(CRetailVector<CThing*>& out) const { for (int i = 0; i < count; ++i) out.push_back(things[i]); }
};

namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    struct CMeshEffectPriorityEntry { bool EffectsCarriedItems; bool EffectsWeapons; };
    struct SNode { void* first; CMeshEffectPriorityEntry second; bool erased; };
    struct CMeshEffectMap {
        SNode* _M_header;          // sentinel
        SNode* nodes; int count;
        struct iterator {
            SNode* _M_node;
            iterator(SNode* node) : _M_node(node) { }
            iterator(const iterator& other) : _M_node(other._M_node) { }
            SNode* operator->() const { return _M_node; }
            bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
        };
        iterator end() { return iterator(_M_header); }
        iterator find(void* const& key)
        {
            for (int i = 0; i < count; ++i) if (!nodes[i].erased && nodes[i].first == key) return iterator(&nodes[i]);
            return iterator(_M_header);
        }
        void erase(iterator position) { position._M_node->erased = true; }
    };
    CThing*        ParentThing;
    CMeshEffectMap MeshEffects;
    unsigned char  UpdatePrimitiveReasons;
    void ClearMeshEffect(void* owner);
};

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

static void Init(CThing& t) { for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0; t.m_interfaces.first = t.m_interfaces.last = 0; t.renderListAdds = 0; }

int main()
{
    int fail = 0;
    typedef NGraphicAppearance::CTCGraphicAppearance GA;
    int keyA = 0, keyB = 0;
    CThing hero, sword, hat;
    Init(hero); Init(sword); Init(hat);
    GA::SNode sentinel = { 0, { false, false }, false };
    GA::SNode heroNodes[2] = { { &keyA, { true, false }, false }, { &keyB, { false, false }, false } };
    GA::SNode swordNodes[1] = { { &keyA, { false, false }, false } };
    GA::SNode hatNodes[1] = { { &keyA, { false, false }, false } };
    GA gHero, gSword, gHat;
    gHero.ParentThing = &hero; gHero.MeshEffects._M_header = &sentinel; gHero.MeshEffects.nodes = heroNodes; gHero.MeshEffects.count = 2; gHero.UpdatePrimitiveReasons = 0;
    gSword.ParentThing = &sword; gSword.MeshEffects._M_header = &sentinel; gSword.MeshEffects.nodes = swordNodes; gSword.MeshEffects.count = 1; gSword.UpdatePrimitiveReasons = 0;
    gHat.ParentThing = &hat; gHat.MeshEffects._M_header = &sentinel; gHat.MeshEffects.nodes = hatNodes; gHat.MeshEffects.count = 1; gHat.UpdatePrimitiveReasons = 0;
    CTCCarrying carrying = { { &sword, &hat, 0, 0 }, 2 };
    CInterfaceMap::SEntry heroE[1] = { { 0x46, (CTCBase*)&carrying } };
    hero.m_presentBits[2] = 1UL << 6; hero.m_interfaces.first = heroE; hero.m_interfaces.last = heroE + 1;
    CInterfaceMap::SEntry swordE[2] = { { 0x48, 0 }, { 0x5b, (CTCBase*)&gSword } };
    sword.m_presentBits[2] = (1UL << 8) | (1UL << 27); sword.m_interfaces.first = swordE; sword.m_interfaces.last = swordE + 2;
    CInterfaceMap::SEntry hatE[1] = { { 0x5b, (CTCBase*)&gHat } };
    hat.m_presentBits[2] = 1UL << 27; hat.m_interfaces.first = hatE; hat.m_interfaces.last = hatE + 1;

    gHero.ClearMeshEffect(&keyA);        // carried items affected, weapons not
    if (!heroNodes[0].erased || heroNodes[1].erased || !hatNodes[0].erased || swordNodes[0].erased) fail = 1;
    if (hero.renderListAdds != 1 || hat.renderListAdds != 1 || sword.renderListAdds != 0 || !(gHero.UpdatePrimitiveReasons & 4)) fail = 2;
    gHero.ClearMeshEffect(&keyB);        // not spread to carried things
    if (!heroNodes[1].erased || hero.renderListAdds != 2 || hat.renderListAdds != 1) fail = 3;
    gHero.ClearMeshEffect(&keyB);        // already gone: only the re-render
    if (hero.renderListAdds != 3) fail = 4;
    if (fail) { std::printf("004bfe20_TEST FAIL\n"); return 1; }
    std::printf("004bfe20_TEST PASS\n");
    return 0;
}