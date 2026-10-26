// CTCBuyableHouse::CreateRentMoney() -- fold the gold bags lying in the house into the virtual count, add the rent bags due since the last payment day (moving that day on), cap at the def's MaxRentBags and lay out a small gold bag per virtual bag, owned by the house's thing and holding one bag's rent
// Leaf: CreateRentMoney
// Retail 0x006C0C90, 624 bytes, `ret`. Callees: CountMoneyBags 0x006C04D0, DestroyMoneyBags(bool) 0x006C0580,
// CTCBase::GetWorld 0x00686A80, GetRentMoneyPos(C3DVector*, long, long) 0x006C06D0, CCharString 0x0099EBF0 /
// ~CCharString 0x0099EAE0, CTCBase::GetDefinitions 0x006869D0, GetDefGlobalIndexFromName 0x009AD410,
// CThingObject::Create 0x00703210 (__fastcall), CThing::AddTC 0x004C9D60, LowerBound 0x0040F020, CTCOwnedEntity::
// SetPOwner 0x007E8590, GetValueOfTrophies 0x006BFFD0, __ftol2. Literals 0x0125D760, 0x0122D70E (""), 0x0123AA74.
// Layout: Ego_r.pdb CTCBuyableHouse (PDef +0x0c, Rented +0x12, DayNextRentIsDue +0x18, CurrentDressLevel +0x1c,
// VirtualMoneyBags +0x44); retail CBuyableHouseDef Rent vector +0x34, DaysBetweenRentPayments +0x40, MaxRentBags
// +0x44 (landed 0x006C0270); world +0x1c -> +0x14 is the day; CTCGold (0xCC) Gold +0x0c.
class CTCBase;
class C3DVector { public: float x, y, z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTCParamsBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
// same-TU stand-in for the interface-map LowerBound (VC7.1 then knows the entry is not derived from the key)
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* lo = first;
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        SEntry* mid = lo + half;
        if (mid->type < *type) {
            lo = mid + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return lo;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
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
class CThingObject : public CThing { public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector* position, long player,
        void* orientation, unsigned long flags, const CCharString& name);   // 0x00703210
};
class CTCOwnedEntity { public: void SetPOwner(CThing* owner); };   // 0x007E8590
class CTCGold { public: unsigned char _pad[0xc]; long Gold; };     // interface 0xCC
class CDefinitionManager { public: long GetDefGlobalIndexFromName(const CCharString& name); };   // 0x009AD410
struct CGameTime { unsigned char _pad[0x14]; long Day; };
class CWorld { public: unsigned char _pad[0x1c]; CGameTime* Time; };
class CBuyableHouseDef { public:
    virtual void v00();
    long          m_refCount;                           // +0x04
    unsigned char _pad08[0x2c];
    long*         Rent_First;                           // +0x34
    long*         Rent_Last;                            // +0x38
    long*         Rent_End;                             // +0x3c
    long          DaysBetweenRentPayments;              // +0x40
    long          MaxRentBags;                          // +0x44
};
template<class T> inline const T& min_of(const T& a, const T& b) { return b < a ? b : a; }

class CTCBuyableHouse { public:
    void*             __vftable;                        // +0x00
    CThing*           ParentThing;                      // +0x04
    unsigned char     _pad08[0x4];
    CBuyableHouseDef* PDef;                             // +0x0c
    bool              IsScripted;                       // +0x10
    bool              OwnedByPlayer;                    // +0x11
    bool              Rented;                           // +0x12
    long              DaySomeoneWillMoveIn;             // +0x14
    long              DayNextRentIsDue;                 // +0x18
    long              CurrentDressLevel;                // +0x1c
    unsigned char     _pad20[0x44 - 0x20];
    long              VirtualMoneyBags;                 // +0x44
    CWorld* GetWorld() const;                           // 0x00686A80
    CDefinitionManager* GetDefinitions() const;         // 0x006869D0
    long CountMoneyBags() const;                        // 0x006C04D0
    void DestroyMoneyBags(bool giveToHero);             // 0x006C0580
    bool GetRentMoneyPos(C3DVector* pos, long index, long count);   // 0x006C06D0
    long GetValueOfTrophies() const;                    // 0x006BFFD0
    CBuyableHouseDef* GetDef() const { return PDef; }
    long GetNumberOfRentBagsDue() const
    {
        if (PDef && Rented && GetWorld()->Time->Day >= DayNextRentIsDue) {
            long due = (GetWorld()->Time->Day - DayNextRentIsDue) / PDef->DaysBetweenRentPayments + 1;
            if (due > 0)
                return due;
        }
        return 0;
    }
    long GetRentPerBag() const
    {
        const long* rents = PDef->Rent_First;
        return (long)(GetValueOfTrophies() * 0.1f + rents[CurrentDressLevel]);
    }
    void CreateRentMoney();
};

void CTCBuyableHouse::CreateRentMoney()
{
    VirtualMoneyBags += CountMoneyBags();
    DestroyMoneyBags(false);
    VirtualMoneyBags += GetNumberOfRentBagsDue();
    DayNextRentIsDue += PDef->DaysBetweenRentPayments * GetNumberOfRentBagsDue();
    VirtualMoneyBags = min_of(VirtualMoneyBags, PDef->MaxRentBags);
    if (PDef->Rent_Last - PDef->Rent_First == 0)
        return;
    for (long i = 0; i < VirtualMoneyBags; ++i) {
        C3DVector pos;
        if (!GetRentMoneyPos(&pos, i, GetDef()->MaxRentBags))
            continue;
        long index;
        {
            CCharString name("OBJECT_GOLDBAG_SMALL_WITH_COINS");
            index = GetDefinitions()->GetDefGlobalIndexFromName(name);
        }
        if (index <= 0)
            continue;
        CThingObject* bag = CThingObject::Create(index, &pos, 5, 0, 0, "");
        bag->AddTC("CTCOwnedEntity", false, 0);
        CTCOwnedEntity* owned;
        bag->GetInterface(&owned, 0x33);
        owned->SetPOwner(ParentThing);
        CTCGold* gold;
        bag->GetInterface(&gold, 0xcc);
        if (gold)
            gold->Gold = GetRentPerBag();
    }
    VirtualMoneyBags = 0;
}