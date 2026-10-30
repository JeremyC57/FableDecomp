// CThingFilter_IsValidTargetForPlayerCreature::operator()(CThing* thing) const -- true when the thing is a valid combat target for the player creature (alive, targetable, not an ally, name/key checks)
// Grade: DIFFER(501v488): 83 diff lines (was 91) -- 'return PTargetOnlyThing == thing ? true : false;' gives retail's cmp/sete al exit; residue: this/thing register swap (retail this ebp, thing edi; ours this edi spilled to a 0xc frame, thing ebp) and the temporaries' slots
// Leaf: ??RCThingFilter_IsValidTargetForPlayerCreature
struct CTCBase;
long __fastcall Compare(const char* a, const char* b);      // 0x00411570
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    __forceinline bool operator==(const CBasicStringChar& other) const
    {
        if (StringLength != other.StringLength)
            return false;
        return Compare(PData, other.PData) == 0;
    }
};
struct CCharStringData { CBasicStringChar Data; long NoRefs; };
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
__forceinline bool operator==(const CCharString& a, const CCharString& b)
{
    if (a.PStringData == b.PStringData)
        return true;
    if (a.PStringData && b.PStringData)
        return a.PStringData->Data == b.PStringData->Data;
    return false;
}
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last, end}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long& type) const;             // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char ThingType;                                // +0x10
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad50[0x6c - 0x50];
    unsigned long ThingTypeFlags;                           // +0x6c
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CThingPlayerCreature : CThing {};
struct CTCAction {                                          // TC 0x0d
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual CCharString GetClassName() const;               // slot 22 (+0x58)
};
struct CTCShop;
struct CTCStockItem {                                       // TC 0x54
    unsigned char _pad00[0x0d];
    bool Stealable;                                         // +0x0d
    CTCShop* GetShopTC() const;                             // 0x007F4FC0
};
struct CTCHeroStats {                                       // TC 0x36
    unsigned char _pad00[0x34];
    long Value;                                             // +0x34
};
struct CTCPhysicsInfo {                                     // TC 0x09
    unsigned char _pad00[0x0c];
    unsigned char Flags;                                    // +0x0c
};
extern bool g_bMeleeTargetCheck;                            // 0x013B85F4
bool __fastcall IsThingVulnerableToMeleeStrikesFromThing(const CThing* thing, const CThing* attacker);   // 0x007F59F0

struct CThingFilter_IsValidTargetForPlayerCreature {
    bool AcceptStealableStockItems;                         // +0x00
    bool PlayerIsChild;                                     // +0x01
    bool PlayerCarryingWeapon;                              // +0x02
    CThingPlayerCreature* PlayerCreature;                   // +0x04
    CThing* PTargetOnlyThing;                               // +0x08
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsValidTargetForPlayerCreature::operator()(CThing* thing) const
{
    if (PTargetOnlyThing)
        return PTargetOnlyThing == thing ? true : false;
    if (PlayerIsChild) {
        if (thing->HasTC(0x26))
            return false;
        if (thing->HasTC(0x35))
            return false;
        if (thing->HasTC(0x0d)) {
            CTCAction* action = (CTCAction*)thing->GetTC(0x0d);
            if (action->GetClassName() == CCharString("CTCActionUseWindow"))
                return false;
        }
    }
    if (thing->HasTC(0x54)) {
        CTCStockItem* stock = (CTCStockItem*)thing->GetTC(0x54);
        if (stock->GetShopTC()) {
            if (!stock->Stealable)
                return false;
            if (!AcceptStealableStockItems)
                return false;
        }
    }
    if (thing->ThingType == 0x0d && PlayerCreature->HasTC(0x36) && ((CTCHeroStats*)PlayerCreature->GetTC(0x36))->Value >= 0)
        return false;
    if (!g_bMeleeTargetCheck || !PlayerCarryingWeapon)
        return true;
    if (thing->ThingTypeFlags & 8)
        return IsThingVulnerableToMeleeStrikesFromThing(thing, PlayerCreature);
    if (thing->HasTC(9) && !(((CTCPhysicsInfo*)thing->GetTC(9))->Flags & 0x1c))
        return false;
    return true;
}
