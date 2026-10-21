// CTCShop::GetAttitudeSellPriceMultiplier() const -- 1 for a house the player owns; else the shop def's attitude sell multiplier for the keeper's attitude to the player, eased toward 1 by the hero's trade skill
// Grade: DIFFER(443v448) -- twin of 0x007735D0 with the Sell offsets (+0x70/+0x74); same register-web priority tie-break: retail this=ebp, thing=edi, zero=ebx; ours this=edi, thing=ebp, zero=esi (both sub esp,0x10). Manifest label GetAttitudeBuyPriceMultiplier is wrong for this copy: it reads AttitudeSellPriceMultiplier.
struct CTCBase;
struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short DefIndex;                        // +0x8c
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline const T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (const T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                       // 0x00A01B50
};
struct CTCBuyableHouse {
    unsigned char _pad00[0x11];
    bool OwnedByPlayer;                             // +0x11
};
struct CTCHeroStats {
    float GetTradeSkill() const;                    // 0x0057C239
};
// Def pointers (retail release: `dec [obj+4] ; jne ; call [vtbl+4]`).
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long RefCount;                                  // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE = 0 };
struct SAttitudeNode {                              // STLport map node
    long color;
    SAttitudeNode* parent;
    SAttitudeNode* left;
    SAttitudeNode* right;
    EOpinionAttitudeType first;                     // +0x10
    float second;                                   // +0x14
};
struct CAttitudeIterator {
    SAttitudeNode* m_pNode;
    CAttitudeIterator() {}
    CAttitudeIterator(SAttitudeNode* node) : m_pNode(node) {}
    SAttitudeNode* operator->() const { return m_pNode; }
    bool operator!=(const CAttitudeIterator& other) const { return m_pNode != other.m_pNode; }
};
struct CAttitudeMap {                               // map<EOpinionAttitudeType, float> (12 bytes in retail)
    SAttitudeNode* m_pHeader;
    unsigned long _pad[2];
    CAttitudeIterator find(const EOpinionAttitudeType& key) const;   // 0x00701FC0
    CAttitudeIterator end() const { return CAttitudeIterator(m_pHeader); }
};
struct CShopDef : CDefBase {
    unsigned char _pad08[0x58 - 0x08];
    float BuyPriceMultiplier;                       // +0x58
    float SellPriceMultiplier;                      // +0x5c
    float AttitudeBuyPriceMultiplierDefault;        // +0x60
    CAttitudeMap AttitudeBuyPriceMultiplier;        // +0x64
    float AttitudeSellPriceMultiplierDefault;       // +0x70
    CAttitudeMap AttitudeSellPriceMultiplier;       // +0x74
};
struct CDefClassBase : CDefBase {
    bool GetSubDef_005130a0(CDefPointer<CShopDef>& out);    // 0x005130A0
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_005130a0(CDefPointer<CShopDef>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.Object = (CShopDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CDefinitionManager {
    bool GetDef_0042af3c(long index, CDefPointer<CDefClassBase>& out);   // 0x0042AF3C
};
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index);
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef_0042af3c(long index, CDefPointer<CDefClassBase>& out)
{
    void* def = Lookup_CDefinitionManager_GetDef(this, index);
    if (!def)
        return false;
    out.Object = (CDefClassBase*)def;
    return true;
}
#pragma auto_inline(on)
struct COpinionReactionManager {
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing* thing) const;   // 0x00701840
};
struct CWorld {
    unsigned char _pad00[0x74];
    COpinionReactionManager* OpinionReactionManager;   // +0x74
};
struct CPlayer {
    CThing* GetHeroThing() const;                   // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                 // 0x00449960
};
extern float g_TradeSkillAttitudeScale;             // 0x01382998 (0.84f)

struct CTCShop {
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x10 - 0x08];
    CBaseIntelligentPointer PShopKeeper;            // +0x10
    CDefinitionManager* GetDefinitionManager() const;    // 0x006869D0
    const CWorld* GetWorld() const;                 // 0x00686A80
    const CPlayerManager* GetPlayerManager() const; // 0x00686D50
    float SellMultiplierFromDefs(const CThing* thing) const;
    float GetAttitudeSellPriceMultiplier() const;
};

__forceinline float CTCShop::SellMultiplierFromDefs(const CThing* thing) const
{
    CDefPointer<CDefClassBase> thingDef;
    GetDefinitionManager()->GetDef_0042af3c(thing->DefIndex, thingDef);
    CDefPointer<CShopDef> shopDef;
    if (!thingDef->GetSubDef_005130a0(shopDef))
        return 1.0f;
    float multiplier = shopDef->AttitudeSellPriceMultiplierDefault;
    if (PShopKeeper.GetPItem() && PShopKeeper.GetPItem()->HasTC(0x4f)) {
        const COpinionReactionManager* reactions = GetWorld()->OpinionReactionManager;
        EOpinionAttitudeType attitude = reactions->GetThingAttitudeToPlayer(PShopKeeper.GetPItem());
        CAttitudeIterator it = shopDef->AttitudeSellPriceMultiplier.find(attitude);
        if (it != shopDef->AttitudeSellPriceMultiplier.end())
            multiplier = it->second;
    }
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    if (hero && hero->HasTC(4)) {
        float skill = hero->GetTC<CTCHeroStats>(4)->GetTradeSkill();
        multiplier += (1.0f - multiplier) * (skill * g_TradeSkillAttitudeScale);
    }
    return multiplier;
}

float CTCShop::GetAttitudeSellPriceMultiplier() const
{
    const CThing* thing = ParentThing;
    if (thing->HasTC(0xd5)) {
        if (thing->GetTC<CTCBuyableHouse>(0xd5)->OwnedByPlayer)
            return 1.0f;
    }
    return SellMultiplierFromDefs(thing);
}
