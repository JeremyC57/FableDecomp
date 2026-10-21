// Grade: DIFFER(427v427) -- retail keeps the thing def in edi across GetSubDef and every later call (reusing the zero register, re-zeroed at the loop tail) with no frame copy: whole-program register caching of the def pointer slots; ours spills the copy (frame 0x2c v 0x28)
// CTCHeroStats::GetFavouriteWeapon(long kind) -- the name of the weapon the hero carries most of (kind 1: melee only,
// kind 2: ranged only, else any), or "TEXT_GUI_STATS_LABEL_NONE" when there is none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AEB4, 427 bytes, `ret 8` (hidden CWideString return + kind).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
};
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol); // 0x009C95E0
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank* TextBank;                                    // +0x14
};
extern CMainGameComponent* g_MainGameComponent;             // 0x013B86A0

struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();
    long RefCount;                                          // +0x04
};
struct CWeaponDef : CDefClassBase {
    unsigned char _pad08[0x2c - 0x08];
    long WeaponClass;                                       // +0x2c
};
struct CThingDef : CDefClassBase {
    bool GetSubDef(const CWeaponDef** out) const;           // 0x0042B1A6 (CDefPointer<const T>* out)
};
struct CDefinitionManager {
    bool GetDef(long index, const CThingDef** out) const;   // 0x0042AF3C (CDefPointer<const T>* out)
};

struct CItemRecord {
    unsigned char _pad00[0x0c];
    long Count;                                             // +0x0c
};
template <class T1, class T2> struct pair { T1 first; T2 second; };
typedef pair<long, long> CItem;
struct CItemVector {                                        // STLport vector<pair<long, long> >
    CItem* _M_start;
    CItem* _M_finish;
    CItem* _M_end_of_storage;
    CItemVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CItemVector()
    {
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    CItem& operator[](unsigned long n) { return _M_start[n]; }
};
struct CTCInventoryItem {
    static bool __fastcall IsWeapon(long defIndex);         // 0x005D97F0
    static CWideString __fastcall GetName(long defIndex);   // 0x005D95C0
};
bool __fastcall IsMeleeWeaponClass(long weaponClass);       // 0x005DA510

struct CTCInventoryWeapons {                                // interface type 0x13
    void RefreshActiveWeaponData();                         // 0x005C30D3
    void GetAllItems(CItemVector& items) const;             // 0x005BF482 (CTCInventoryBase)
    CItemRecord* GetItemRecord(long defIndex);              // 0x005BE8D3 (CTCInventoryBase)
};

struct CInterfaceMap {
    struct Entry { long type; void* component; };
    Entry* first; Entry* last;
    Entry* LowerBound(const long*);                         // 0x004365B0
};
struct CPresenceBits { bool test(long kind); };             // bitset<>::test 0x00410DE0
struct CThing {
    unsigned char _pad00[0x20]; CPresenceBits present; unsigned char _pad21[0x23]; CInterfaceMap interfaces;
    template<class T> __forceinline bool GetInterface(T** out, long kind) {
        if (present.test(kind)) {
            long key = kind; CInterfaceMap* map = &interfaces; CInterfaceMap::Entry* it = map->LowerBound(&key);
            if (it == map->last || kind < it->type) it = map->last;
            *out = (T*)it->component; return true;
        }
        return false;
    }
};

struct CTCHeroStats {
    void* __vftable;
    CThing* ParentThing;                                    // +0x04
    const CDefinitionManager* GetDefinitions() const;       // 0x006869D0 (CTCBase)
    CWideString GetFavouriteWeapon(long kind);
};

CWideString CTCHeroStats::GetFavouriteWeapon(long kind)
{
    CTCInventoryWeapons* weapons;
    if (ParentThing->GetInterface(&weapons, 0x13)) {
        weapons->RefreshActiveWeaponData();
        CItemVector items;
        weapons->GetAllItems(items);
        long favourite = -1;
        long mostCarried = 0;
        for (unsigned long i = 0; i < items.size(); ++i) {
            if (CTCInventoryItem::IsWeapon(items[i].first)) {
                const CThingDef* thingDefSlot = 0;
                GetDefinitions()->GetDef(items[i].first, &thingDefSlot);
                const CWeaponDef* weaponDefSlot = 0;
                const CThingDef* thingDef = thingDefSlot;
                thingDef->GetSubDef(&weaponDefSlot);
                const CWeaponDef* weaponDef = weaponDefSlot;
                if (kind == 1) {
                    if (!IsMeleeWeaponClass(weaponDef->WeaponClass))
                        goto release;
                } else if (kind == 2 && IsMeleeWeaponClass(weaponDef->WeaponClass))
                    goto release;
                {
                    CItemRecord* record = weapons->GetItemRecord(items[i].first);
                    if (record && record->Count > mostCarried) {
                        mostCarried = record->Count;
                        favourite = items[i].first;
                    }
                }
            release:
                if (weaponDef && --((CDefClassBase*)weaponDef)->RefCount == 0)
                    ((CDefClassBase*)weaponDef)->Destroy();
                if (thingDef && --((CDefClassBase*)thingDef)->RefCount == 0)
                    ((CDefClassBase*)thingDef)->Destroy();
            }
        }
        if (favourite > 0)
            return CTCInventoryItem::GetName(favourite);
    }
    return g_MainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_STATS_LABEL_NONE");
}
