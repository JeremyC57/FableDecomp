// CTCInventoryAbilities::OpenInventory() -- sort the 19 hero abilities into the to-give and to-take lists (by name), create
// and add to the inventory every missing ability item of the first list, remove the held items of the second, open the base
// inventory with the stats-screen animation box and register the screen with the UI manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F05E5, 410 bytes, plain `ret` (frame pointer, `leave`).
// Leaf: OpenInventory@CTCInventoryAbilities
// Callees: AddAbilityToCorrectList @ 0x005EEFB7, CGameDefinitionManager::Get @ 0x0044C6B0, GetDefGlobalIndexFromName
// @ 0x009AD410, GetNumberOfItemsOfTypeInInventory @ 0x005BDF08, CThing::GetPosition @ 0x004C73D0, the thing factory
// @ 0x00835A20 (__fastcall (def index, position, bool)), bitset::test 0x00410DE0 / LowerBound 0x0040F020 (interface 0x1A),
// CTCInventoryItem::AddToInventory @ 0x005D8D50, the inventory's slot 79 (+0x13C), CTCInventoryBase::OpenInventory
// @ 0x005BD4EA, SetInventoryAnimRenderBox @ 0x005BC58F, NUISystem::CManager::GetInstance @ 0x0041E5F2 and its slot 19
// (+0x4C), vector<CCharString> destructor @ 0x00414EA0 (label "CCombatWheel::ResetRings" is propagated).
// Layout: retail CTCInventoryBase PDef +0x68; CInventoryDef animation box +0xA4/+0xAC (as CTCInventoryStats::OpenInventory
// 0x005CCF86).
// Shape: first try. The interface pointer is the uninitialised-local idiom (the absent arm reads the frame word that held
// the lookup key); the render box is the landed OpenInventory twin's two by-value GetPDef() temporaries.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
struct CPresenceBits { bool test(long kind); };             // bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
struct C3DVector { float X, Y, Z; };
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    __forceinline bool HasTC(long type) { return present.test(type); }
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
CThing* __fastcall CreateThing(long defIndex, const C3DVector& position, bool flag);   // 0x00835A20

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
};
struct CCharStringVector {                                  // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CCharStringVector();                                   // 0x00414EA0
    CCharString* begin() { return _M_start; }
    CCharString* end() { return _M_finish; }
};
struct CGameDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410 (CDefinitionManager)
    static CGameDefinitionManager& __fastcall Get();        // 0x0044C6B0
};

struct C2DVector {
    float X, Y;
    __forceinline C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) {}
};
struct C2DBoxF {
    C2DVector TL;
    C2DVector Extents;
    __forceinline C2DBoxF(const C2DVector& tl, const C2DVector& extents) : TL(tl), Extents(extents) {}
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
struct CInventoryDef : CDefPointeeBase {
    unsigned char _pad08[0xa4 - 0x08];
    C2DVector StatsAnimBoxTLPos;                // +0xa4
    C2DVector StatsAnimBoxExtents;              // +0xac
};
template <class T> struct CDefPointer {
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    __forceinline ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};

namespace NInventory { class CTCInventoryBase; }
struct CTCInventoryItem {                                   // interface type 0x1A
    bool AddToInventory(NInventory::CTCInventoryBase* inventory, bool a, bool b, long c, bool d, bool e);   // 0x005D8D50
};
namespace NUISystem {
struct CObserver;
struct CManager {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13(); virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17(); virtual void m18(); 
    virtual void AddScreen_slot19(void* screen);            // slot 19 (+0x4C)
    static CManager* __fastcall GetInstance();              // 0x0041E5F2
};
}

namespace NInventory {
class CTCInventoryBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); 
    virtual void RemoveItems_slot79(long defIndex, bool all);   // slot 79 (+0x13C)
    CThing*       ParentThing;                              // +0x04 CTCBase
    unsigned char _pad08[0x68 - 0x08];
    CDefPointer<CInventoryDef> PDef;                        // +0x68
    __forceinline CDefPointer<CInventoryDef> GetPDef() const { return PDef; }
    void OpenInventory();                                   // 0x005BD4EA
    void SetInventoryAnimRenderBox(const C2DBoxF& box);     // 0x005BC58F
    long GetNumberOfItemsOfTypeInInventory(long defIndex);  // 0x005BDF08
};
}

class CTCInventoryAbilities : public NInventory::CTCInventoryBase {
public:
    void AddAbilityToCorrectList(long ability, CCharStringVector* toGive, CCharStringVector* toTake);   // 0x005EEFB7
    void OpenInventory();
};

void CTCInventoryAbilities::OpenInventory()
{
    CCharStringVector toGive;
    CCharStringVector toTake;
    for (long ability = 1; ability < 20; ++ability)
        AddAbilityToCorrectList(ability, &toGive, &toTake);
    CGameDefinitionManager& defs = CGameDefinitionManager::Get();
    for (CCharString* it = toGive.begin(); it != toGive.end(); ++it) {
        long defIndex = defs.GetDefGlobalIndexFromName(*it);
        if (defIndex && GetNumberOfItemsOfTypeInInventory(defIndex) == 0) {
            CThing* item = CreateThing(defIndex, ParentThing->GetPosition(), true);
            if (item) {
                CTCInventoryItem* inventoryItem;
                if (item->HasTC(0x1a))
                    inventoryItem = item->FindTC<CTCInventoryItem>(0x1a);
                inventoryItem->AddToInventory(this, false, true, 0, true, false);
            }
        }
    }
    for (CCharString* it = toTake.begin(); it != toTake.end(); ++it) {
        long defIndex = defs.GetDefGlobalIndexFromName(*it);
        if (defIndex && GetNumberOfItemsOfTypeInInventory(defIndex) > 0)
            RemoveItems_slot79(defIndex, true);
    }
    NInventory::CTCInventoryBase::OpenInventory();
    SetInventoryAnimRenderBox(C2DBoxF(GetPDef()->StatsAnimBoxTLPos, GetPDef()->StatsAnimBoxExtents));
    NUISystem::CManager::GetInstance()->AddScreen_slot19(this);
}