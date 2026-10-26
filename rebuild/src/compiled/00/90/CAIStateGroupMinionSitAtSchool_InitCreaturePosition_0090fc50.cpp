// CAIStateGroup_MinionSitAtSchool::InitCreaturePosition() -- during school time, place the creature near its home building (radius 5).
// Grade: RELOCATION_MATCH -- same-TU LowerBound body; the && chain inside `if (HasTC(0x23)) { return ...; } return false;` (trailing byte false block, as retail); the creature passed to an inline PlaceNearHome(creature) helper so it is loaded once AFTER the school-time call (a named local loads it before)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090FC50, length 127 bytes (+ int3 padding). Vtable slot 3.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x23 = CTCVillager, bit 3 of the bitset
// dword at +0x24), GFGetPVillageInterface @ 0x007EE540 (__fastcall on the villager component), the village's
// school-time test @ 0x00658A35 (manifest "CTCVillage::IsCookTime" is a propagated label on this body),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on the creature's PHome at +0x170, CThingCreatureBase::
// AutoPlaceNearBuilding(CThingBuilding*, float) @ 0x00666EF0 with 5.0f.
// Shape: a trailing `return false` for the interface miss (byte `xor al,al`; HasTC on the masked word), then a bool
// `&&` chain returned through the full register (`mov eax,1` / `xor eax,eax`) whose first call takes the
// pointer-returning FindTC result straight into ecx; the creature is the argument of an inline helper for the home
// lookup and the call (esi). Layout local (the engine header flattens the base).
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
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)


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
    // the pointer-returning lookup: the component lands straight in ecx for the call that follows
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
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
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CTCVillager;
class CTCVillage { public:
    bool IsSchoolTime();                        // 0x00658A35
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

class CThingBuilding;
struct CThingCreatureHome {
    unsigned char _pad[0x170];
    CBaseIntelligentPointer PHome;              // +0x170 CIntelligentPointer<CThingBuilding>
    bool AutoPlaceNearBuilding(CThingBuilding* building, float radius);   // 0x00666EF0
};

struct CAIStateGroup_MinionSitAtSchool : CAIStateGroupBase {
    virtual bool InitCreaturePosition();
};

__forceinline bool PlaceNearHome(CThingCreatureHome* creature)
{
    return creature->AutoPlaceNearBuilding((CThingBuilding*)creature->PHome.GetPItem(), 5.0f);
}

bool CAIStateGroup_MinionSitAtSchool::InitCreaturePosition()
{
    if (PBrain->PCreature->HasTC(0x23)) {
        return GFGetPVillageInterface(PBrain->PCreature->FindTC<CTCVillager>(0x23))->IsSchoolTime()
            && PlaceNearHome((CThingCreatureHome*)PBrain->PCreature);
    }
    return false;
}