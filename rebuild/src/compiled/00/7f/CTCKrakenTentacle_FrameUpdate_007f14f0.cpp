// CTCKrakenTentacle::FrameUpdate() -- per frame (virtual slot 9): dies with its kraken; while the tentacle thing's emergence value is still below one, makes it untargetable, spawns the head effect and clears the obstruction (once); otherwise tracks its target or returns to its rest position; then fires the queued strike when its frame arrives
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F14F0, length 224 bytes (+ int3 padding); speed TU (no frame pointer). The oracle label
// is CTCKrakenTentacle::vfunc_9 (the component FrameUpdate slot).
// Callees: CTCBase::FrameUpdate @ 0x006868A0 (catalog "$E2"), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice on PKraken),
// CThing::Kill(bool) @ 0x004C9B80, CVectorMap::LowerBound @ 0x0040F020 (the inline GetInterface for type 9; the bitset test folds
// to `test ah,2` on the first word), CTCKrakenTentacle::CreateHeadEffect @ 0x007F0B10, RemoveObstruction @ 0x007F0810, TrackTarget
// @ 0x007F0C70, ReturnToDesiredPos(bool) @ 0x007F0F40, CWorld::GetWorldFrame @ 0x0049D870, PerformStrikeDamage @ 0x007F11D0.
// Constant 1.0f @ 0x0122DED8.
// Layout (PDB = retail): ParentThing +0x04, PKraken +0x10, CanStrike +0x28, Tracking +0x29, IsStriking +0x2A, QueuedStrikeTick +0x2C,
// DoneDeathStuff +0x4C; the parent thing's emergence fraction at +0xB4 (a float beyond CThingPhysical; PDB name unresolved);
// the interface-9 component's targetable flag at +0x10 (the landed CTCCreatureFlags9 view). CThing Dead = +0x91 bit 0.
// Shape: CanStrike is cleared right after the base update; the kraken test is the landed inline IS_THING_ALIVE (two GetPItem calls)
// negated into an early `Kill ; return` that keeps its own epilogue in place; the emergence test is `fraction < 1.0f` (`fcomp ; test ah,5 ; jp` skips to the tracking arm);
// Tracking is cleared between the queued-tick load and its test, and the tick is re-read after the world-frame call.
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
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits UsingComponents;                   // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char Dead : 1;                          // +0x91 bit 0
    unsigned char _pad_0x92[0xb4 - 0x92];
    float         EmergeFraction;                    // +0xb4
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
    void Kill(bool immediately);                     // 0x004C9B80
};
struct CTCCreatureFlags9 {                           // the interface-9 component
    unsigned char _pad[0x10];
    unsigned char m_canBeTargeted;                   // +0x10
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CThing* GetPItem() const;                        // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)   // the pointer is set and the thing's dead bit is clear (inlined)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->Dead)
            return true;
    }
    return false;
}
struct CWorld {
    static long GetWorldFrame();                     // 0x0049D870
};
extern const float g_one;                            // 0x0122DED8

struct CTCKrakenTentacle {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void FrameUpdate();                      // slot 9
    CThing*       ParentThing;                       // +0x04
    unsigned char _pad08[0x08];
    CBaseIntelligentPointer PKraken;                 // +0x10
    unsigned char _pad18[0x28 - 0x18];
    bool          CanStrike;                         // +0x28
    bool          Tracking;                          // +0x29
    bool          IsStriking;                        // +0x2a
    long          QueuedStrikeTick;                  // +0x2c
    unsigned char _pad30[0x4c - 0x30];
    bool          DoneDeathStuff;                    // +0x4c
    void FrameUpdate_Base();                         // 0x006868A0 (CTCBase::FrameUpdate)
    void CreateHeadEffect();                         // 0x007F0B10
    void RemoveObstruction();                        // 0x007F0810
    void TrackTarget();                              // 0x007F0C70
    void ReturnToDesiredPos(bool immediately);       // 0x007F0F40
    void PerformStrikeDamage();                      // 0x007F11D0
};
void CTCKrakenTentacle::FrameUpdate()
{
    FrameUpdate_Base();
    CanStrike = false;
    if (!IS_THING_ALIVE(PKraken)) {
        ParentThing->Kill(false);
        return;
    }
    {
        if (!DoneDeathStuff) {
            CThing* thing = ParentThing;
            if (thing->EmergeFraction < g_one) {
                CTCCreatureFlags9* flags;
                if (thing->GetInterface(&flags, 9))
                    flags->m_canBeTargeted = 0;
                CreateHeadEffect();
                RemoveObstruction();
                DoneDeathStuff = true;
            } else if (!IsStriking) {
                if (Tracking)
                    TrackTarget();
                else
                    ReturnToDesiredPos(false);
            }
        }
        Tracking = false;
        if (QueuedStrikeTick > 0 && CWorld::GetWorldFrame() >= QueuedStrikeTick)
            PerformStrikeDamage();
    }
}