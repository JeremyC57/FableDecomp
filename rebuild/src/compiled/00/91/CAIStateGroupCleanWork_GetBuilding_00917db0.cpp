// CAIStateGroup_CleanWork::GetBuilding() -- the creature's work building, except that a shopkeeper whose shop is missing, closed or flagged at +0x50 has none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00917DB0, length 119 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x90 =
// CTCShopKeeper: bitset word 4 bit 16 -> `test dword ptr [eax+30h],10000h`), CTCShopKeeper::GetShopTC @ 0x00798F50,
// CTCShop::IsOpeningHours @ 0x00773950, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the creature's building
// pointer at +0x170, re-reached through PBrain->Creature).
// Shape: the landed GetInterface out-parameter template inside the `if`; the shop is a named local (esi); the
// three disqualifying tests share one `return 0` (`xor eax,eax`) placed before the fall-through tail that returns
// the building pointer (the creature is re-read for it); the member is reached through an inline
// accessor returning the item (`lea ecx,[eax+170h]`; direct member access or a pointer-returning accessor
// gives `mov ecx ; add ecx`).
// Model: local (no engine header for this class).
class CTCBase;
class CThingBuilding;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CThingAICreature { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x170 - 0x4c];
    CIntelligentPointer<CThingBuilding> PWorkBuilding;      // +0x170 CIntelligentPointer<CThingBuilding>
    CThingBuilding* GetWorkBuilding() { return PWorkBuilding.GetPItem(); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCShop { public:
    unsigned char _pad[0x50];
    unsigned char m_bClosedDown;                // +0x50
    bool IsOpeningHours();                      // 0x00773950
};

class CTCShopKeeper { public:                          // interface 0x90
    CTCShop* GetShopTC();                       // 0x00798F50
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_CleanWork : CAIStateGroupBase {
    CThingBuilding* GetBuilding();
};

CThingBuilding* CAIStateGroup_CleanWork::GetBuilding()
{
    CTCShopKeeper* shopKeeper;
    if (PBrain->Creature->GetInterface(&shopKeeper, 0x90)) {
        CTCShop* shop = shopKeeper->GetShopTC();
        if (shop == 0 || !shop->IsOpeningHours() || shop->m_bClosedDown)
            return 0;
    }
    return PBrain->Creature->GetWorkBuilding();
}