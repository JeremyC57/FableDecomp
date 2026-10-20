// Grade: DIFFER(290v290) -- register swap in the second pass only: retail keeps the list node in edi and the component in ebx (as in the first pass), ours swaps them; frame and order identical. Tried: shared iterator/end locals (fixed the frame slots), function-scope component, lookup on *it.
// CTCInventoryClothing::RefreshDescriptions() -- re-texts the clothing menu: every "CLOTHING TITLE" component gets the title string and every "Clothing_Description" component the description of the item it shows
// Retail 0x005B690B, 290 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: list<NUISystem::CComponent*>(const allocator&) 0x0042ABAF / ~list 0x0042ABCA, CCharString(const char*, long)
// 0x0099EBF0 (built straight in the by-value argument slot), the clothing list's virtual slot 5 (+0x14, collect the
// components named by a CCharString into a list; name descriptive), CWideString() 0x0099AED0 / ~CWideString
// 0x0099B510, ConstructTitleString(CWideString&) 0x005B6881 (landed), the component's slot 144 (+0x240,
// SetText(const CWideString&, bool, bool)), map<CComponent*, long>::operator[] 0x0042D1B7, this's slot 78 (+0x138,
// GetItemDescription(CWideString&, long, bool); name descriptive).
// Shape: one list collects both passes (it is not cleared in between); both passes walk it with the SAME function-scope
// iterator/end locals (the end node cached in one frame word); the item index comes from an inline by-value lookup helper, so the key copy and the looked-up index
// share one frame word; only positive indices get a description.
// Layout (retail): ClothingItemMap +0x160, ClothingList +0x16C (as the landed ctor 0x005B7049).
#pragma optimize("y", off)
#pragma optimize("s", on)

typedef unsigned short wchar_t;                    // VC7.1 without /Zc:wchar_t
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                    // 0x0099EAE0
};
struct CWideString {
    void* m_pStringData;
    CWideString();                                     // 0x0099AED0
    ~CWideString();                                    // 0x0099B510
};
namespace NUISystem {
struct CComponent;
}
struct SAllocator { SAllocator() { } };
struct CComponentList {                                // std::list<NUISystem::CComponent*> (head pointer)
    struct SNode { SNode* m_pNext; SNode* m_pPrev; NUISystem::CComponent* m_value; };
    SNode* m_pNode;
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node) : m_pNode(node) {}
        NUISystem::CComponent* operator*() const { return m_pNode->m_value; }
        iterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    CComponentList(const SAllocator& allocator = SAllocator());   // 0x0042ABAF
    ~CComponentList();                                 // 0x0042ABCA
    iterator begin() { return iterator(m_pNode->m_pNext); }
    iterator end() { return iterator(m_pNode); }
};
namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void FindComponentsByName(CCharString name, CComponentList& found);   // slot 5 (+0x14)
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void v133();
    virtual void v134();
    virtual void v135();
    virtual void v136();
    virtual void v137();
    virtual void v138();
    virtual void v139();
    virtual void v140();
    virtual void v141();
    virtual void v142();
    virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);               // slot 144 (+0x240)
};
}
struct CClothingItemMap {                              // std::map<NUISystem::CComponent*, long> (12 bytes)
    void* _M_header; unsigned long _M_node_count; unsigned char _M_key_compare;
    long& operator[](NUISystem::CComponent* const& key);   // 0x0042D1B7
};

struct CTCInventoryClothing {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void GetItemDescription(CWideString& text, long itemIndex, bool b);   // slot 78 (+0x138)
    unsigned char _pad_0x004[0x160 - 0x004];
    CClothingItemMap ClothingItemMap;                  // +0x160
    NUISystem::CComponent* ClothingList;               // +0x16c
    void ConstructTitleString(CWideString& title);     // 0x005B6881
    __forceinline long GetMenuItemIndex(NUISystem::CComponent* component) { return ClothingItemMap[component]; }
    void RefreshDescriptions();
};

void CTCInventoryClothing::RefreshDescriptions()
{
    if (!ClothingList)
        return;
    CComponentList components;
    ClothingList->FindComponentsByName("CLOTHING TITLE", components);
    CComponentList::iterator it = components.begin();
    CComponentList::iterator end = components.end();
    for (; it != end; ++it) {
        NUISystem::CComponent* component = *it;
        CWideString title;
        ConstructTitleString(title);
        component->SetText(title, true, true);
    }
    ClothingList->FindComponentsByName("Clothing_Description", components);
    for (it = components.begin(), end = components.end(); it != end; ++it) {
        NUISystem::CComponent* component = *it;
        long itemIndex = GetMenuItemIndex(component);
        if (itemIndex > 0) {
            CWideString description;
            GetItemDescription(description, itemIndex, false);
            component->SetText(description, true, true);
        }
    }
}
