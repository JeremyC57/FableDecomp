// behaviour test for 0x0090F9C0 CAIStateGroup_MinionSitOnSeatBase::IsRunnable (body verbatim, callees stubbed)
#include <stdio.h>
#include <string.h>
// CAIStateGroup_MinionSitOnSeatBase::IsRunnable() -- when it is time to sit and an owner object exists, join the owner's gather point (if the group uses one), pick a seat from the owner and check the seat is valid.
// Grade: RELOCATION_MATCH -- (1) same-TU LowerBound body + a ternary-pick FindTC fed into a free __forceinline wrapper AddAsUserTo(gp, user) (component lands in ecx before the creature argument, as 0072af30); (2) the body nested inside `if (IsTimeToSit()) { if (GetPOwnerObject(&owner)) { ... } } return false;` so every false exit shares the trailing `xor al,al` block
// Leaf: ?IsRunnable@CAIStateGroup_MinionSitOnSeatBase@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F9C0, length 148 bytes (+ int3 padding). Vtable slot 11.
// Callees: this class's virtuals slot 14 (+0x38, bool "is it sit time"), slot 16 GetPOwnerObject(CThing**) (+0x40,
// 0x0090FAE0 in MinionSitAtInn), slot 17 (+0x44, bool "uses a gather point"), slot 18 GetSeat(CThing*) (+0x48,
// 0x009104D0 here); CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface on the OWNER, type 0x7B =
// CTCGatherPointBase, bit 27 of the bitset dword at +0x2c), CTCGatherPointBase::AddAsUser(CThing*) @ 0x007FFEB0
// with PBrain->PCreature, CAIStateGroup_UsesSeat::IsSeatValid @ 0x00938580 (tail, its bool returned).
// Shape: nested ifs with the trailing `return false` LAST (the merged `xor al,al` block sits after the
// IsSeatValid tail); the owner is a frame local passed by address (re-read from its slot for GetSeat); the
// gather-point join is `if (UsesGatherPoint() && owner && owner->HasTC(0x7b)) { gp = FindTC(0x7b); if
// (!gp->AddAsUser(creature)) return false; }` with the pointer-returning lookup in a named local (the component
// lands in ecx before the creature argument is loaded); then `GetSeat(owner); return IsSeatValid();`.
// Layout local (the engine header flattens the base and has no vtable).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

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


struct CThing {
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
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
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

struct CTCGatherPointBase {                     // interface type 0x7B
    bool AddAsUser(CThing* user);               // 0x007FFEB0
};

struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    unsigned char _seat[0x20];                  // +0x14 PSeat, GotSeatPos, Sitting, SeatPosIndex, SeatPos, SittingOnBench
    bool IsSeatValid();                         // 0x00938580
};

struct CAIStateGroup_MinionSitOnSeatBase : CAIStateGroup_UsesSeat {
    CBaseIntelligentPointer POwner;             // +0x34 CIntelligentPointer<CThing>
    virtual bool IsTimeToSit();                 // slot 14 (+0x38)
    virtual void v15();                         // slot 15
    virtual bool GetPOwnerObject(CThing** ppOwner);     // slot 16 (+0x40)
    virtual bool UsesGatherPoint();             // slot 17 (+0x44)
    virtual void GetSeat(CThing* owner);        // slot 18 (+0x48)
    virtual bool IsRunnable();
};

__forceinline bool AddAsUserTo(CTCGatherPointBase* gatherPoint, CThing* user) { return gatherPoint->AddAsUser(user); }

bool CAIStateGroup_MinionSitOnSeatBase::IsRunnable()
{
    if (IsTimeToSit()) {
        CThing* owner;
        if (GetPOwnerObject(&owner)) {
            if (UsesGatherPoint()) {
                if (owner != 0) {
                    if (owner->HasTC(0x7b)) {
                        if (!AddAsUserTo(owner->FindTC<CTCGatherPointBase>(0x7b), PBrain->PCreature))
                            return false;
                    }
                }
            }
            GetSeat(owner);
            return IsSeatValid();
        }
    }
    return false;
}


static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)

static bool timeToSit, ownerFound, usesGather, addOk, seatValid;
static CThing* ownerResult;
static int addCalls, seatCalls, validCalls, gatherCalls, ownerCalls;
static CTCGatherPointBase* addThis; static CThing* addUser; static CThing* seatOwner;
static char log_[256];

CAIStateGroupBase::~CAIStateGroupBase() { }
void CAIStateGroupBase::v01() { } void CAIStateGroupBase::v02() { } void CAIStateGroupBase::v03() { }
void CAIStateGroupBase::v04() { } void CAIStateGroupBase::v05() { } void CAIStateGroupBase::v06() { }
void CAIStateGroupBase::v07() { } void CAIStateGroupBase::v08() { }
void CAIStateGroupBase::Cleanup() { } void CAIStateGroupBase::OnReplace() { }
bool CAIStateGroupBase::IsRunnable() { return false; }
void CAIStateGroupBase::Update() { } void CAIStateGroupBase::InitStateGroup() { }
void CBaseIntelligentPointer::v00() { }
bool CAIStateGroup_MinionSitOnSeatBase::IsTimeToSit() { strcat(log_, "T"); return timeToSit; }
void CAIStateGroup_MinionSitOnSeatBase::v15() { }
bool CAIStateGroup_MinionSitOnSeatBase::GetPOwnerObject(CThing** ppOwner) { strcat(log_, "O"); ++ownerCalls; *ppOwner = ownerResult; return ownerFound; }
bool CAIStateGroup_MinionSitOnSeatBase::UsesGatherPoint() { strcat(log_, "U"); ++gatherCalls; return usesGather; }
void CAIStateGroup_MinionSitOnSeatBase::GetSeat(CThing* owner) { strcat(log_, "S"); ++seatCalls; seatOwner = owner; }
bool CAIStateGroup_UsesSeat::IsSeatValid() { strcat(log_, "V"); ++validCalls; return seatValid; }
bool CTCGatherPointBase::AddAsUser(CThing* user) { strcat(log_, "A"); ++addCalls; addThis = this; addUser = user; return addOk; }

static CThing owner, creature;
static CAIBrain brain;
static CInterfaceMap::SEntry entries[3];
static CTCGatherPointBase gatherObj[4], otherObj[4];

static void reset(bool sit, bool found, CThing* own, bool gather, bool hasBit, bool ok, bool valid)
{
    timeToSit = sit; ownerFound = found; ownerResult = own; usesGather = gather; addOk = ok; seatValid = valid;
    addCalls = seatCalls = validCalls = gatherCalls = ownerCalls = 0; addThis = 0; addUser = 0; seatOwner = 0; log_[0] = 0;
    memset(&owner, 0, sizeof(owner));
    entries[0].type = 0x10; entries[0].component = (CTCBase*)otherObj;
    entries[1].type = 0x7b; entries[1].component = (CTCBase*)gatherObj;
    entries[2].type = 0x7fff; entries[2].component = (CTCBase*)otherObj;
    owner.m_interfaces.first = entries; owner.m_interfaces.last = entries + 2;
    if (hasBit) owner.m_presentBits[0x7b >> 5] |= 1UL << (0x7b & 31);
}

int main()
{
    CAIStateGroup_MinionSitOnSeatBase g; g.PBrain = &brain; brain.PCreature = &creature;
    // 1: not time to sit -> false, nothing else
    reset(false, true, &owner, true, true, true, true); CHECK(!g.IsRunnable() && strcmp(log_, "T") == 0);
    // 2: no owner object -> false
    reset(true, false, &owner, true, true, true, true); CHECK(!g.IsRunnable() && strcmp(log_, "TO") == 0);
    // 3: no gather point used -> seat from the owner, its validity returned
    reset(true, true, &owner, false, true, true, true); CHECK(g.IsRunnable() && strcmp(log_, "TOUSV") == 0 && seatOwner == &owner);
    reset(true, true, &owner, false, true, true, false); CHECK(!g.IsRunnable() && strcmp(log_, "TOUSV") == 0);
    // 4: gather point used but a null owner -> straight to the seat
    reset(true, true, 0, true, true, true, true); CHECK(g.IsRunnable() && strcmp(log_, "TOUSV") == 0 && seatOwner == 0);
    // 5: owner without interface 0x7b -> straight to the seat
    reset(true, true, &owner, true, false, true, true); CHECK(g.IsRunnable() && strcmp(log_, "TOUSV") == 0);
    // 6: join the gather point with the brain's creature: accepted -> seat; refused -> false, no seat
    reset(true, true, &owner, true, true, true, true);
    CHECK(g.IsRunnable() && strcmp(log_, "TOUASV") == 0 && addThis == gatherObj && addUser == &creature);
    reset(true, true, &owner, true, true, false, true); CHECK(!g.IsRunnable() && strcmp(log_, "TOUA") == 0);

    if (fails == 0) printf("0090f9c0_TEST PASS\n");
    return fails;
}