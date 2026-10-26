// CTCLook::TrackNearestInterestingThing(bool track) -- drives the head-turn state from the creature's targeting: while tracking, keeps the look timer alive as long as there is something interesting, else drops back to the idle turn; starts tracking when something appears; `track` false resets the state (one frame of delay) and clears the stop time. Returns whether the head is tracking
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753840, length 240 bytes (+ int3 padding, `ret 4`); speed TU (no frame pointer).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (the inline GetInterface for type 8 = CTCTargetingBase; the bitset test folds
// to `test ch,1` on the first word), CTCTargetingBase::GetPNearestInterestingThing @ 0x007F5860, CTCLook::UpdateLookTimer
// @ 0x00751AD0 and SetHeadTurnState(long) @ 0x007517B0.
// Layout: retail CTCLook ParentThing +0x04, HeadTurnState +0x48 (PDB), TimeToStopLooking +0xB4 and OneFrameDelay +0xB8 (PDB
// +0xB8/+0xBC; the TurningAnims vector before them is 12 bytes). CThing components bitset +0x20, interface map +0x44.
// Shape: the state is read once into ebx for the tests (bit 0 enabled, bit 2 tracking); every return re-reads it and extracts
// bit 2 (`shr eax,2 ; and eax,1`), a 32-bit result; the bitset word is read as a dword and tested through its high byte.
extern "C" int  __cdecl puts(const char* text);    // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
struct CPresenceBits {                             // std::bitset<274>
    unsigned long words[9];
    bool test(unsigned long position) const
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (words[position >> 5] & (1UL << (position & 31))) != 0;
    }
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits UsingComponents;                   // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (UsingComponents.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
class CTCTargetingBase { public:                            // interface type 8
    CThing* GetPNearestInterestingThing();           // 0x007F5860
};
enum { HEAD_TURN_ENABLED = 1, HEAD_TURN_TRACKING = 4 };

class CTCLook { public:
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                       // +0x04
    unsigned char _pad08[0x48 - 0x08];
    unsigned long HeadTurnState;                     // +0x48 (the bit extraction is unsigned: `shr`)
    unsigned char _pad4c[0xb4 - 0x4c];
    long          TimeToStopLooking;                 // +0xb4
    bool          OneFrameDelay;                     // +0xb8
    void UpdateLookTimer();                          // 0x00751AD0
    void SetHeadTurnState(long state);               // 0x007517B0
    long TrackNearestInterestingThing(bool track);
};
long CTCLook::TrackNearestInterestingThing(bool track)
{
    unsigned long state = HeadTurnState;
    if (!(state & HEAD_TURN_ENABLED))
        return (state >> 2) & 1;
    if (track) {
        CTCTargetingBase* targeting;
        if (ParentThing->GetInterface(&targeting, 8)) {
            if (state & HEAD_TURN_TRACKING) {
                if (targeting->GetPNearestInterestingThing())
                    UpdateLookTimer();
                else
                    SetHeadTurnState(HEAD_TURN_ENABLED);
            } else if (targeting->GetPNearestInterestingThing()) {
                UpdateLookTimer();
                SetHeadTurnState(HEAD_TURN_TRACKING);
            }
        }
    } else {
        if (state != HEAD_TURN_ENABLED) {
            HeadTurnState = HEAD_TURN_ENABLED;
            OneFrameDelay = true;
        }
        TimeToStopLooking = -1;
    }
    return (HeadTurnState >> 2) & 1;
}