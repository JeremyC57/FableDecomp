// behaviour test for 0x008614B0 CVillageTaskCarryCrate::IsCreatureValidForTask (body verbatim, callees stubbed)
#include <stdio.h>
#include <string.h>
// CVillageTaskCarryCrate::IsCreatureValidForTask(CThing* creature) -- a creature suits the task when its carrying state is the opposite of the task's: empty-handed for a pickup, carrying something for a drop-off
// Grade: RELOCATION_MATCH -- retail's `mov bl,1` at entry (compared against hasItem and Pickup, returned as `mov al,bl`) is a `bool valid = true;` result local that the two failing tests set to false in an if/else-if chain and that is returned at the end; returning false directly folds the constant
// Leaf: ?IsCreatureValidForTask@CVillageTaskCarryCrate@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008614B0, length 115 bytes (padded to 128), `ret 4`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (inline GetInterface for TC 0x46 = CTCCarrying,
// present bit `test byte ptr [thing+28h],40h`), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440.
// Layout: engine/CVillageTaskCarryCrate.h (Pickup +0x18).
// Shape: the out pointer is uninitialised (on a miss retail passes the stale argument-slot word); the `== valid` compares
// read the result local while it still holds true (bl), the false arms assign it.
struct CTCBase;
struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;          // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                    // +0x44
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
struct CBaseIntelligentPointer {                   // CIntelligentPointer<T>: vptr + item (8 bytes)
    virtual void v00();
    CThing* PItem;
    CThing* GetPItem() const;                      // 0x00A01B50
};
struct CTCCrateStack {
    long GetNumCrates() const;                     // 0x007B6660
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot() const;         // 0x006A1440
};
struct CVillageTaskCarryCrate {                    // engine/CVillageTaskCarryCrate.h layout (flattened pointers there)
    virtual void v00();
    unsigned char           _pad04[4];             // +0x04 (CVillageTaskBase)
    CBaseIntelligentPointer CreatureAssignedToTask;   // +0x08 CIntelligentPointer<const CThing>
    CBaseIntelligentPointer PCrateStack;           // +0x10 CIntelligentPointer<CThing>
    bool                    Pickup;                // +0x18
    long                    Priority;              // +0x1c
    bool IsValid();
    bool IsCreatureValidForTask(CThing* creature);
};
bool CVillageTaskCarryCrate::IsCreatureValidForTask(CThing* creature)
{
    bool valid = true;
    CTCCarrying* carrying;
    creature->GetInterface(&carrying, 0x46);
    bool hasItem = carrying->GetThingInPrimarySlot() != 0;
    if (hasItem == valid && Pickup == valid)
        valid = false;
    else if (!hasItem && !Pickup)
        valid = false;
    return valid;
}


// ---- stubs ----
static long g_lbKey; static int g_lbOffset; static int g_lbCalls;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { g_lbKey = *type; g_lbCalls++; return first + g_lbOffset; }
void CBaseIntelligentPointer::v00() { }
void CVillageTaskCarryCrate::v00() { }
static CThing* g_slot; static const CTCCarrying* g_slotThis;
CThing* CTCCarrying::GetThingInPrimarySlot() const { g_slotThis = this; return g_slot; }

static int fails;
static void check(int c, const char* w) { if (!c) { fails++; printf("FAIL %s\n", w); } }

static double g_creatureBuf[0x20], g_itemBuf[2];
static double g_carryBuf[1], g_endCarryBuf[1];

int main()
{
    CThing* creature = (CThing*)g_creatureBuf;
    CThing* item = (CThing*)g_itemBuf;
    CTCCarrying* carry = (CTCCarrying*)g_carryBuf;
    CTCCarrying* endCarry = (CTCCarrying*)g_endCarryBuf;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x46; entries[0].component = (CTCBase*)carry;
    entries[1].type = 0; entries[1].component = (CTCBase*)endCarry;
    creature->m_interfaces.first = entries; creature->m_interfaces.last = entries + 1;
    creature->m_presentBits[0x46 >> 5] = 1UL << (0x46 & 31);
    CVillageTaskCarryCrate* task = new CVillageTaskCarryCrate;

    // pickup task: valid only when empty-handed
    task->Pickup = true; g_slot = item;
    check(task->IsCreatureValidForTask(creature) == false && g_lbKey == 0x46 && g_slotThis == carry, "pickup, carrying");
    g_slot = 0;
    check(task->IsCreatureValidForTask(creature) == true, "pickup, empty");
    // drop-off task: valid only when carrying
    task->Pickup = false;
    check(task->IsCreatureValidForTask(creature) == false, "drop-off, empty");
    g_slot = item;
    check(task->IsCreatureValidForTask(creature) == true, "drop-off, carrying");
    // lookup end / greater / hit
    g_lbOffset = 1; task->IsCreatureValidForTask(creature);
    check(g_slotThis == endCarry, "end pick");
    g_lbOffset = 0; entries[0].type = 0x47; task->IsCreatureValidForTask(creature);
    check(g_slotThis == endCarry, "greater-key pick");
    entries[0].type = 0x46; task->IsCreatureValidForTask(creature);
    check(g_slotThis == carry, "hit pick");
    // no carrying component: no lookup, the stale out slot is used (the stub ignores it)
    creature->m_presentBits[0x46 >> 5] = ~(1UL << (0x46 & 31)); int calls = g_lbCalls;
    bool r = task->IsCreatureValidForTask(creature);
    check(g_lbCalls == calls && r == true, "no component");
    if (!fails) printf("008614b0_TEST PASS\n");
    return fails;
}