// CTCInventoryTrade::ApplyTattooCard(long defIndex) -- applies a bought tattoo card to the hero morph and re-uploads its tattoo texture layers
// Grade: DIFFER(620v620) -- frame 0x40 vs 0x3c: the tattoo-card vector takes [ebp-28h] instead of [ebp-24h] (one extra frame word from a temporary that retail shares)
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
struct CIEngine;
struct CDefBase {
    virtual void v00();
    virtual void Destroy();
    long m_refCount;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
template <class T> struct CDefBasePointer {
    T* Object;
    __forceinline ~CDefBasePointer()
    {
        if (Object) {
            T* p = Object;
            --p->m_refCount;
            if (p->m_refCount == 0)
                p->Destroy();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
struct CTattooDef : CDefBase {
    unsigned char _pad08[0x30 - 0x08];
    long Layer;                                         // +0x30
    unsigned char _pad34[0x3c - 0x34];
    bool Permanent;                                     // +0x3c
    bool Exclusive;                                     // +0x3d
};
struct CThingBaseDef : CDefBase {
    bool GetSubDef(CDefPointer<CTattooDef>* out) const; // 0x005B1EFC
};
void* FindDef_a();
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<CTattooDef>* out) const
{
    void* def = FindDef_a();
    if (!def)
        return false;
    out->Object = (CTattooDef*)def;
    return true;
}
#pragma auto_inline(on)
template <class T> struct CThingDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
typedef CThingDefPointer<CThingBaseDef> CTattooThing;
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector();                                          // 0x005B31AC
    unsigned int size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    T& operator[](unsigned int i) { return _M_start[i]; }
    void push_back(const T& v);                         // 0x005B3325
};
struct CLongSetIterator { void* _M_node; };
struct CInsertResult { CLongSetIterator first; bool second; };
struct CLongSet {                                       // std::set<long>
    void* _M_header;
    unsigned int _M_node_count;
    long _M_key_compare;
    CLongSet();                                         // 0x005157F0
    ~CLongSet();                                        // 0x004AC140
    CInsertResult insert(const long& v);                // 0x00514230
};
CTattooDef* g_unused;
CDefBasePointer<CTattooDef> __fastcall GetTattooCardDef(long defIndex);      // 0x005D9AA0
struct CTCAppearanceMorphBase {
    void UploadTextureMorphsToEngine(CIEngine& engine, const CLongSet* layers, bool b, float f);   // 0x006CA7F0
};
struct CTCHeroMorph : CTCAppearanceMorphBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void UpdateTattoos();                       // slot 9 (+0x24), name unproven
    void GetTattooThings(vector<CTattooThing>& out) const;   // 0x0071DE20
    void AddTattoo(long defIndex);                      // 0x0071DA70
    void AddTattoo(CTattooThing& def);                  // 0x0071D940
};
struct CEngineHolder { unsigned char _pad[0x2c]; CIEngine* Engine; };   // +0x2c
struct CMainGameComponent { unsigned char _pad[0x28]; CEngineHolder* Display; };   // +0x28
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CPresenceBits { bool test(long kind) const; };   // 0x00410DE0
struct CThing {
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                           // +0x20
    unsigned char  _pad21[0x44 - 0x21];
    CInterfaceMap  m_interfaces;                        // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long kind)
    {
        if (m_present.test(kind)) {
            long key = kind;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long kind) const
    {
        if (m_present.test(kind)) {
            long key = kind;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCInventoryTrade {
    void*   __vftable;
    CThing* ParentThing;                                // +0x04
    void ApplyTattooCard(long defIndex);
};
void CTCInventoryTrade::ApplyTattooCard(long defIndex)
{
    vector<CTattooThing> exclusive;
    vector<CTattooThing> tattoos;
    {
        CTCHeroMorph* morph;
        if (static_cast<const CThing*>(ParentThing)->GetInterface(&morph, 3))
            morph->GetTattooThings(tattoos);
    }
    for (unsigned int i = 0; i < tattoos.size(); ++i) {
        CDefPointer<CTattooDef> def;
        if (tattoos[i]->GetSubDef(&def) && def->Exclusive)
            exclusive.push_back(tattoos[i]);
    }
    CTCHeroMorph* heroMorph = 0;
    if (ParentThing->GetInterface(&heroMorph, 3)) {
        heroMorph->AddTattoo(defIndex);
        heroMorph->UpdateTattoos();
    }
    CTCAppearanceMorphBase* morph;
    if (ParentThing->GetInterface(&morph, 3)) {
        CDefBasePointer<CTattooDef> tattoo = GetTattooCardDef(defIndex);
        CLongSet layers;
        if (tattoo->Permanent) {
            morph->UploadTextureMorphsToEngine(*g_pMainGameComponent->Display->Engine, 0, false, 0.0f);
            unsigned int count = exclusive.size();
            CTattooThing* items = exclusive._M_start;
            for (unsigned int i = 0; i < count; ++i) {
                if (heroMorph) {
                    heroMorph->AddTattoo(items[i]);
                    heroMorph->UpdateTattoos();
                }
                CDefPointer<CTattooDef> def;
                if (items[i]->GetSubDef(&def))
                    layers.insert(def->Layer);
            }
            if (!exclusive.empty())
                morph->UploadTextureMorphsToEngine(*g_pMainGameComponent->Display->Engine, &layers, false, 0.0f);
        } else {
            layers.insert(tattoo->Layer);
            morph->UploadTextureMorphsToEngine(*g_pMainGameComponent->Display->Engine, &layers, false, 0.0f);
        }
    }
}
