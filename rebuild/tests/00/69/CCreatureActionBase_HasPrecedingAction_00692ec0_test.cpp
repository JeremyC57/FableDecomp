// behaviour test for 0x00692EC0 CCreatureActionBase::HasPrecedingAction (body verbatim, callees stubbed)
#include <stdio.h>
#include <string.h>
// CCreatureActionBase::HasPrecedingAction() -- true when the action reports a preceding animation (virtual slot 36) and the creature's interface-70 component says it is not busy with one; false otherwise
// Grade: RELOCATION_MATCH -- the && query written inside `if (IsPlayingPrecedingAnimation()) { ... }` with the `return false` after it: the early false becomes retail's own trailing `xor al,al` block (the guard-clause `if (!x) return false;` form returns the tested al through the shared epilogue)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692EC0, length 97 bytes (+ int3 padding).
// Callees: this action's virtual slot 36 (+0x90, bool), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the inlined GetInterface lookup for type 70 = bit 6 of the bitset
// dword at +0x28, `test byte ptr [eax+28h],40h`), the component's bool query @ 0x006A1270 (manifest "CThing_Array_GetFirstPhysicalSwitch"
// is a propagated guess; it walks the component's 12-byte-element vector at +0xc/+0x10).
// Shape: three exits -- `xor al,al` for the trailing false (a bool store), while `!query()` is computed as an int (`mov eax,1` /
// `xor eax,eax`) and the trailing `return false` shares its zero block. One `push ecx` frame slot for the lookup key.
// Layout: engine/CCreatureActionBase.h (PDB) agrees on PCreature at +8; a local model carries the virtual slot.
class CTCBase;
struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*>: the thing's sorted {type, component} map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline long GetInterface(T** ppOut, long interfaceType)   // a BOOL (long) result in this TU: the
    {                                                                                 //   `&&` below is then materialised in eax
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return 1;
        }
        return 0;
    }
};
struct CBaseIntelligentPointer {            // CIntelligentPointer<CThingCreatureBase>: vptr + item
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CTCActionQueue {                     // the interface-70 component
    bool HasPendingActions();               // 0x006A1270
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual bool IsPlayingPrecedingAnimation();   // slot 36 (+0x90)
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    bool HasPrecedingAction();
};

bool CCreatureActionBase::HasPrecedingAction()
{
    if (IsPlayingPrecedingAnimation()) {
        CTCActionQueue* pQueue;
        return PCreature.GetPItem()->GetInterface(&pQueue, 70) && !pQueue->HasPendingActions();
    }
    return false;
}


static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)

static bool playing, pending;
static int playCalls, pendingCalls, lbCalls;
static CTCActionQueue* pendingThis;
static long lbKey;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    ++lbCalls; lbKey = *type;
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::v00() { }
bool CTCActionQueue::HasPendingActions() { ++pendingCalls; pendingThis = this; return pending; }
#define DEFV(n) void CCreatureActionBase::n() { }
DEFV(v00) DEFV(v01) DEFV(v02) DEFV(v03) DEFV(v04) DEFV(v05) DEFV(v06) DEFV(v07) DEFV(v08) DEFV(v09) DEFV(v10) DEFV(v11)
DEFV(v12) DEFV(v13) DEFV(v14) DEFV(v15) DEFV(v16) DEFV(v17) DEFV(v18) DEFV(v19) DEFV(v20) DEFV(v21) DEFV(v22) DEFV(v23)
DEFV(v24) DEFV(v25) DEFV(v26) DEFV(v27) DEFV(v28) DEFV(v29) DEFV(v30) DEFV(v31) DEFV(v32) DEFV(v33) DEFV(v34) DEFV(v35)
bool CCreatureActionBase::IsPlayingPrecedingAnimation() { ++playCalls; return playing; }

static CThing creature;
static CInterfaceMap::SEntry entries[3];
static CTCActionQueue queueObj[4], otherObj[4];

static void reset(bool isPlaying, bool hasBit, bool hasEntry, bool isPending)
{
    playing = isPlaying; pending = isPending; playCalls = pendingCalls = lbCalls = 0; pendingThis = 0; lbKey = 0;
    memset(&creature, 0, sizeof(creature));
    entries[0].type = 12; entries[0].component = (CTCBase*)otherObj;
    entries[1].type = hasEntry ? 70 : 90; entries[1].component = (CTCBase*)queueObj;
    entries[2].type = 0x7fff; entries[2].component = (CTCBase*)otherObj;
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    if (hasBit) creature.m_presentBits[70 >> 5] |= 1UL << (70 & 31);
}

int main()
{
    CCreatureActionBase action; action.PCreature.m_pItem = &creature;
    // 1: not playing a preceding animation -> false, no lookup
    reset(false, true, true, false); CHECK(!action.HasPrecedingAction() && playCalls == 1 && lbCalls == 0 && pendingCalls == 0);
    // 2: playing, no interface 70 -> false, no lookup
    reset(true, false, true, false); CHECK(!action.HasPrecedingAction() && lbCalls == 0 && pendingCalls == 0);
    // 3: playing, queue present and idle -> true; the query runs on the type-70 component
    reset(true, true, true, false); CHECK(action.HasPrecedingAction() && lbCalls == 1 && lbKey == 70 && pendingThis == queueObj);
    // 4: playing, queue busy -> false
    reset(true, true, true, true); CHECK(!action.HasPrecedingAction() && pendingCalls == 1);
    // 5: bit set but no entry: the miss path uses the end entry's component
    reset(true, true, false, false); CHECK(action.HasPrecedingAction() && pendingThis == otherObj);

    if (fails == 0) printf("00692ec0_TEST PASS\n");
    return fails;
}