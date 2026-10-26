// NPlayerGui::CAutoPickupBar::AddObject(long defIndex) -- count another auto-pickup of a def: bump the existing entry (and restart its display time) or append a new entry with a count of 1, then wake the bar if it is idle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00647511, 100 bytes, __fastcall, ret 4.
// Callee: the pickup list's push_back(const CAutoPickup&) 0x00655F62 (labelled ListPushFrontWrapper; called on the
// list with the record built on the stack).
// Layout: as the landed constructor 0x006543FF (Ego_r.pdb less 0x10: the shipped CStatBase is smaller) --
// the stat-bar info pointer at +4 (its pickup display time at +0x520), the bar state at +0x28,
// Pickups (the 4-byte sentinel-pointer list) at +0x68; CAutoPickup is {DefIndex, Count, TimeRemaining}
// (node: next, prev, value +8).
// Shape: frame-pointer size TU (`push ebp ; mov ebp,esp ... leave` with esi used freely):
// `optimize("y",off)` + `("s",on)` like the landed constructor. The list end is re-read every
// iteration (`cmp eax,[ecx]`); the new record is built field by field in the frame and passed by
// reference; a found entry gets its time reset and its count bumped. The time assignment is written
// FIRST in both paths: the size scheduler then stores the new record's time before the `push` and the
// other two fields after it, and in the found path loads the time before the `inc` and stores it after
// (the source order count-then-time gives the opposite). Both paths share the idle -> showing state
// step (1 -> 2), the found path jumping back to it.
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace NPlayerGui {

struct CStatBarInfo {
    unsigned char _pad[0x520];
    float PickupDisplayTime;                                // +0x520
};

struct CAutoPickup {
    long DefIndex;                                          // +0
    long Count;                                             // +4
    float TimeRemaining;                                    // +8
};

struct CRetailList_AutoPickup {                             // retail list<CAutoPickup>: the sentinel pointer
    struct SNode {
        SNode* next;
        SNode* prev;
        CAutoPickup value;
    };
    SNode* Node;
    void push_back(const CAutoPickup& value);               // 0x00655F62
};

class CAutoPickupBar { public:
    void* __vftable;                                        // +0x00
    CStatBarInfo* m_pInfo;                                  // +0x04
    unsigned char _pad_0x08[0x20];                          // +0x08
    long m_state;                                           // +0x28 (1 = idle, 2 = showing)
    unsigned char _pad_0x2c[0x3c];                          // +0x2C
    CRetailList_AutoPickup Pickups;                         // +0x68
    long CurrentDefIndex;                                   // +0x6C
    long CurrentCount;                                      // +0x70
    float CurrentTimeLeft;                                  // +0x74
    float WaitingTimeLeft;                                  // +0x78
    void AddObject(long defIndex);
};

void CAutoPickupBar::AddObject(long defIndex)
{
    CRetailList_AutoPickup::SNode* node;
    for (node = Pickups.Node->next; node != Pickups.Node; node = node->next) {
        if (node->value.DefIndex == defIndex) {
            node->value.TimeRemaining = m_pInfo->PickupDisplayTime;
            ++node->value.Count;
            goto wake;
        }
    }
    {
        CAutoPickup pickup;
        pickup.TimeRemaining = m_pInfo->PickupDisplayTime;
        pickup.DefIndex = defIndex;
        pickup.Count = 1;
        Pickups.push_back(pickup);
    }
wake:
    if (m_state == 1)
        m_state = 2;
}

}