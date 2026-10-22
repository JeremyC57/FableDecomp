// CAIStateGroup_MinionHawkingFromStall::Update() -- walk to the stall (1.5m), shout the shop's hawking speech at it and pick up a new customer; schedule the next hawk 75..149 frames ahead and terminate
// Grade: RELOCATION_MATCH -- the presence test reads the bit word into a local and shifts it (`mov ecx,[eax+30h] ; shr ecx,10h ; and cl,1`; the masked form narrows to a byte load); the thing's def is a CDefPointer member (+0x70) reached through operator-> on a named shopThing local, which loads it into eax ahead of the holder's address
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094BB20, length 384 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x90 = CTCShopKeeper, bit 16 of +0x30 as a
//          bitset::test byte), CTCShopKeeper::GetShopTC @ 0x00798F50 / AcquireNewCustomer @ 0x00799900,
//          CDefClassBase::GetSubDef<...> @ 0x005130A0 on the shop thing's def (+0x04 -> +0x70), CBaseIntelligentPointer
//          ctor/GetPItem/SetPItem (by-value pointers: CIntelligentPointer<const CThing> vtable 0x01237F1C,
//          CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8), CAIStateGroupBase::AIAction_MoveToThingProximity
//          @ 0x0097E380 / AIAction_SpeakAISpeechToTarget @ 0x009812C0, GetPWorld @ 0x008FCFC0, GetPWorldSeed @ 0x008FCFD0,
//          GFROR13 @ 0x00497890, CWorld::GetWorldFrame @ 0x0049D870 (static), Terminate @ 0x008FCF40; the def's virtual
//          slot 1 (destroy on the last release).
// Shape: the keeper out pointer shares the key's home; GetSubDef is the same-TU noinline stand-in of the landed CTCShop rows; the def holder is a function-scope CDefPointer whose
//          destructor releases without a null test after Terminate; the speech id is read once into a dead slot.
// Layout: no engine header for this class; modelled locally like the landed OnReplace 0x0094BAD0 (next hawk frame
//          +0x14, PStall +0x18); the sub-def's +0x2c speech id is descriptive.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    void Release() { if (--m_refCount == 0) Destroy(); }
};
struct CHawkingDef : CDefPointee {
    unsigned char _pad08[0x2c - 0x08];
    unsigned long HawkSpeech;                   // +0x2c (descriptive)
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CHawkingDef>& out);   // 0x005130A0
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CHawkingDef>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.Object = (CHawkingDef*)def;
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    bool TestTC(long interfaceType) const       // a bool masked test: VC7.1 narrows it to `shr ; and cl,1`
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
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
};
struct CTCShop {
    void* __vftable;
    CThing* PThing;                             // +0x04
};
struct CTCShopKeeper {                          // interface 0x90
    CTCShop* GetShopTC();                       // 0x00798F50
    void AcquireNewCustomer();                  // 0x00799900
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    __forceinline CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    __forceinline CIntelligentPointer(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
struct CThingCreatureBase;
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        return *seed % range;
    }
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointer<const CThing> target, float proximity,
        bool* reached, long movement, bool a, bool b, bool c, bool d, float speed);   // 0x0097E380
    long AIAction_SpeakAISpeechToTarget(long repeat, CIntelligentPointer<CThingCreatureBase> speaker,
        CIntelligentPointer<const CThing> target, unsigned long speech, bool interrupt, long priority);   // 0x009812C0
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_MinionHawkingFromStall : CAIStateGroupBase {
    long NextHawkFrame;                         // +0x14 (descriptive)
    CBaseIntelligentPointer PStall;             // +0x18
    void Update();
};

void CAIStateGroup_MinionHawkingFromStall::Update()
{
    CTCShopKeeper* keeper;
    GetCreature()->GetInterfaceT(&keeper, 0x90);
    CTCShop* shop = keeper->GetShopTC();
    CThing* shopThing = shop->PThing;
    {
    CDefPointer<CHawkingDef> def;
    shopThing->PDef->GetSubDef(def);
    unsigned long speech = def->HawkSpeech;
    bool reached;
    if (AIAction_MoveToThingProximity(1, CIntelligentPointer<const CThing>(PStall), 1.5f, &reached, 3,
            true, false, true, true, 0.0f) == 2) {
        AIAction_SpeakAISpeechToTarget(1, CIntelligentPointer<CThingCreatureBase>((CThingCreatureBase*)GetCreature()),
            CIntelligentPointer<const CThing>(PStall), speech, false, 0x93);
        keeper->AcquireNewCustomer();
    }
    NextHawkFrame = GetPWorld()->GetWorldFrame() + Random(75) + 75;
    Terminate();
    }
}