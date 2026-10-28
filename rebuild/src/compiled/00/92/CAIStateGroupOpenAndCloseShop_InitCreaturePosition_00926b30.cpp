// CAIStateGroup_OpenAndCloseShop::InitCreaturePosition() -- sync the shop building with the
// shopkeeper: mark the building as having its keeper, lock/unlock its doors by the shop's opening hours and set the
// sign component's closed flag; never repositions the creature (returns false).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00926B30, length 274 bytes (+ int3 padding). Vtable slot 3 of
// CAIStateGroup_OpenAndCloseShop (0x0128AC9C).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the creature's +0x170 shop pointer, four times),
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0x90 = CTCShopKeeper bit 16 of +0x30 on the creature,
// 0xBD = the shop sign bit 29 of +0x34 on the shop), CTCShopKeeper::GetShopTC @ 0x00798F50 (x2), CTCShop::
// IsOpeningHours @ 0x00773950, CThingBuilding::LockAllDoors(bool, bool) @ 0x0082DB00.
// Shape: a __forceinline UpdateShopState() shared with InitStateGroup (0x00926A10, the same body); the constant
// 1 lives in bl (dead-bit test, the keeper-present `or` through a NAMED building local -- the member of a call result
// gives `mov dl ; add eax ; or dl ; mov` -- and the open flag); `!open` is passed straight to LockAllDoors and the
// sign (`sete bl ; push ebx`): a named `bool closed` round-trips through a frame slot.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x4c];
    void* m_pSub60;                             // +0x60 (typed per use below)
    unsigned char _pad2[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char m_flags92;                    // +0x92
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    bool TestTC(long interfaceType) const       // std::bitset::test: `shr ; and cl,1` (kept as a byte when reused)
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
    // the bitset::test form: the test's bool is materialised (`shr ; and dl,1 ; mov [esp+x],dl`) and returned
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
    // the out-parameter form whose result is ignored (no bool to keep: the component lands in its use register)
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class CTCShop { public:
    bool IsOpeningHours();                      // 0x00773950
};
class CTCShopKeeper { public:
    CTCShop* GetShopTC();                       // 0x00798F50
};
struct CTCShopSign {                            // interface 0xBD on the shop building
    unsigned char _pad[0x0c];
    bool m_closed;                              // +0x0c
};
class CThingBuilding : public CThing { public:
    unsigned char _pad94[0xd4 - 0x94];
    unsigned char m_buildingFlags;              // +0xd4 (bit 0: keeper present)
    void LockAllDoors(bool locked, bool immediately);   // 0x0082DB00
};
struct CThingShopkeeper : CThing {
    unsigned char _pad94[0x170 - 0x94];
    CBaseIntelligentPointer PShop;              // +0x170 CIntelligentPointer<CThingBuilding>
    CThingBuilding* GetPShop() const { return (CThingBuilding*)PShop.GetPItem(); }
};
class CThingAICreature;

struct CAIStateGroup_OpenAndCloseShop : CAIStateGroupBase {
    CThingShopkeeper* GetCreature() const { return (CThingShopkeeper*)PBrain->PCreature; }
    __forceinline void UpdateShopState()
    {
        CThingBuilding* shop = GetCreature()->GetPShop();
        if (shop && !(shop->m_flags91 & 1)) {
            CThingBuilding* building = GetCreature()->GetPShop();
            building->m_buildingFlags |= 1;
            CTCShopKeeper* keeper;
            bool open = GetCreature()->GetInterface(&keeper, 0x90) && keeper->GetShopTC()
                && keeper->GetShopTC()->IsOpeningHours();
            GetCreature()->GetPShop()->LockAllDoors(!open, false);
            CTCShopSign* sign;
            if (GetCreature()->GetPShop()->GetInterface(&sign, 0xbd))
                sign->m_closed = !open;
        }
    }
    virtual bool InitCreaturePosition();
};

bool CAIStateGroup_OpenAndCloseShop::InitCreaturePosition()
{
    UpdateShopState();
    return false;
}