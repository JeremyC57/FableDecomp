// Grade: DIFFER(ours v retail) -- 114v116: the shop-keeper component is loaded straight into ecx (retail: mov eax,[eax+4] ; pop esi ; mov ecx,eax) and the shared 'xor al,al' false tail sits after IsOpeningHours instead of at the end
// CAIStateGroup_GazeAtWork::IsRunnable() -- runnable when the creature has a workplace, its shop (if it keeps one) is open and a free gaze position exists
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00918A40, length 116 bytes (+ int3 padding). Vtable slot 11
// of CAIStateGroup_GazeAtWork (0x0128FEC4).
// Callees: virtual GetOwner (slot 14, +0x38, 0x00919020), CVectorMap::LowerBound @ 0x0040F020 (inlined
// interface lookup, type 0x90 = CTCShopKeeper, bit 16 of the presence dword at +0x30),
// CTCShopKeeper::GetShopTC @ 0x00798F50, CTCShop::IsOpeningHours @ 0x00773950,
// CAIStateGroup_Gaze::FindFreeGazePos @ 0x00918880.
//
// Shape: three early `return false` exits share one `xor al,al` tail; the last test is `return
// FindFreeGazePos() != 0;` (the `neg ; sbb ; neg` bool). The shop-keeper lookup is the inlined GetInterface
// template (key temporary in the `push ecx` slot); the shop pointer is a named local.
// The engine headers are flattened data-only layouts without the vptr chain: `this` is a local model.
class CTCBase;
class CThingBuilding;
class CThingGameObject;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase;

struct CTCShop {
    bool IsOpeningHours();                      // 0x00773950
};

struct CTCShopKeeper {                          // interface type 0x90
    CTCShop* GetShopTC();                       // 0x00798F50
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void v12(); virtual void v13();
    virtual CThingBuilding* GetOwner();         // slot 14 (+0x38)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_Gaze : CAIStateGroupBase {
    unsigned char GazeMarker[0x08];             // +0x14 CIntelligentPointer<CThingGameObject>
    float ChanceOfRunning;                      // +0x1C
    float MinJourneyForRunning;                 // +0x20
    float MaxDistanceForCreaturesToLookAt;      // +0x24
    long PrimaryMarkerDef;                      // +0x28
    long SecondaryMarkerDef;                    // +0x2C
    CThingGameObject* FindFreeGazePos();        // 0x00918880
};

struct CAIStateGroup_GazeAtWork : CAIStateGroup_Gaze {
    virtual bool IsRunnable();
};

bool CAIStateGroup_GazeAtWork::IsRunnable()
{
    if (!GetOwner())
        return false;
    CTCShopKeeper* keeper;
    if (PBrain->PCreature->GetInterface(&keeper, 0x90)) {
        CTCShop* shop = keeper->GetShopTC();
        if (!shop)
            return false;
        if (!shop->IsOpeningHours())
            return false;
    }
    return FindFreeGazePos() != 0;
}
