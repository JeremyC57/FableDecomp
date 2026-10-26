// CAIStateGroup_PrepareMeal::IsTimeToPrepareMeal() -- the villager's village says it is cook time (the village's
// counted def held across the call).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009462A0, length 138 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (inlined GetInterface, type 0x23 = CTCVillager, bit 3 of the bitset
// dword at +0x24; the miss path reads the never-written slot), GFGetPVillageInterface @ 0x007EE540 (three times:
// null test, the def copy, the call object), CTCVillage::IsCookTime @ 0x00658B6B, the def's virtual slot 1
// (destroy at reference count zero).
// Shape: `if (village != 0) { ... return IsCookTime(); } return false;` (the byte exit at the end), a local
// CDefPointer built from the village's raw def pointer at +0x94 (`if (p) ++p->count`, landed 0x00838B60 shape), the call result kept in bl across the copy's
// destructor (`test esi,esi ; dec [esi+4] ; jne ; call [vt+4]`), `mov al,bl`. Layout local (the engine header
// flattens the base; the village's def member is descriptive).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
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

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CVillageDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};

struct CDefPointer {                            // CDefPointer<const CVillageDef>
    CVillageDef* m_p;
    CDefPointer(CVillageDef* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }   // from the raw def pointer: a direct load, no address formed
    ~CDefPointer()
    {
        if (m_p) {
            if (--m_p->m_refCount == 0)
                m_p->Destroy();
        }
    }
};

struct CTCVillager;
class CTCVillage { public:
    unsigned char _pad[0x94];
    CDefPointer PDef;                           // +0x94 the village's counted def
    bool IsCookTime_00658b6b();                          // 0x00658B6B
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

struct CInterfaceMapB {                         // the same vector map, this module's LowerBound instantiation
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThingB {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMapB m_interfaces;                // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMapB* map = &m_interfaces;
            CInterfaceMapB::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CAIStateGroup_PrepareMeal : CAIStateGroupBase {
    bool IsTimeToPrepareMeal();
};

bool CAIStateGroup_PrepareMeal::IsTimeToPrepareMeal()
{
    CTCVillager* villager;
    ((CThingB*)PBrain->PCreature)->GetInterface(&villager, 0x23);
    if (GFGetPVillageInterface(villager) != 0) {
        CDefPointer def(GFGetPVillageInterface(villager)->PDef.m_p);
        return GFGetPVillageInterface(villager)->IsCookTime_00658b6b();
    }
    return false;
}