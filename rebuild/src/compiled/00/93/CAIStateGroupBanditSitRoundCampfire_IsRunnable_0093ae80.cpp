// CAIStateGroup_BanditSitRoundCampfire::IsRunnable() -- (at most every 5 s) a bandit with the 0x4a component, outside its village's non-curfew hours, can sit at a live nearby campfire
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093AE80, 313 bytes (virtual, `ret`). Ego_r.pdb: ?IsRunnable@CAIStateGroup_BanditSitRoundCampfire@@UAE_NXZ.
// Callees: CWorld::GetFrame 0x0049D870 (`__fastcall`, no arguments), CVectorMap::LowerBound 0x0040F020 (inlined
// GetInterface 0x23), CTCVillageMember::GetVillage 0x007EE540 (twice; manifest "IS_THING_ALIVE<..>" is propagated),
// CTCVillage::IsCurfewTime 0x00658D5D, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem
// 0x00A01B50 / dtor 0x00A01C10, GetPNearCampfire 0x0093AB70; frames-per-second global 0x01375550.
// Shape: the landed AI state-group model (0x00950A30): two `PCampfire = CIntelligentPointer<CThingObject>(p)`
// temporaries (vtable 0x0125A858; ctor + SetPItem, operator= = SetPItem(GetPItem()), dtor), then IS_THING_ALIVE
// into a named bool (bl preloaded with 1 serves as the dead-bit mask and the result, so the curfew `return false`
// keeps IsCurfewTime's zero al); a failure re-arms the check 5 seconds later (the frame rate read into a named
// local before the frame call, held in edi).
// Layout: Ego_r.pdb CAIStateGroup_BanditSitRoundCampfire (PCampfire +0x14, SeatPosIndex +0x1c, GotSeatPos +0x20,
// Sitting +0x21, FrameToNextCheckIsRunnable +0x28).
// Leaf: IsRunnable@CAIStateGroup_BanditSitRoundCampfire
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
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
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
};


class CTCVillage { public:
    bool IsCurfewTime();                        // 0x00658D5D
};
class CTCVillageMember { public:
    CTCVillage* GetVillage();                   // 0x007EE540
};
class CThingObject;

struct CIntelligentPointerThingObject : CBaseIntelligentPointer {   // CIntelligentPointer<CThingObject>, vtable 0x0125A858
    virtual void v00();
    CIntelligentPointerThingObject(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThingObject& operator=(const CIntelligentPointerThingObject& other)
    {
        CThing* item = other.GetPItem();       // fetched before the member address is formed (GOTCHAS chunk 148)
        SetPItem(item);
        return *this;
    }
};

extern long g_framesPerSecond;                  // 0x01375550
class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870

struct CAIStateGroup_BanditSitRoundCampfire : CAIStateGroupBase {
    CIntelligentPointerThingObject PCampfire;   // +0x14
    long SeatPosIndex;                          // +0x1c
    bool GotSeatPos;                            // +0x20
    bool Sitting;                               // +0x21
    unsigned long FrameToNextSitOnSeat;         // +0x24
    unsigned long FrameToNextCheckIsRunnable;   // +0x28
    bool WasCarryingWeapon;                     // +0x2c
    CThingObject* GetPNearCampfire() const;     // 0x0093AB70
    virtual bool IsRunnable();
};

bool CAIStateGroup_BanditSitRoundCampfire::IsRunnable()
{
    if (FrameToNextCheckIsRunnable > (unsigned long)CWorld::GetFrame())
        return false;
    CThing* creature = PBrain->PCreature;
    if (!creature->HasTC(0x4a))
        return false;
    CTCVillageMember* member;
    if (creature->GetInterface(&member, 0x23)) {
        if (member->GetVillage() && !member->GetVillage()->IsCurfewTime())
            return false;
    }
    GotSeatPos = false;
    Sitting = false;
    SeatPosIndex = -1;
    PCampfire = CIntelligentPointerThingObject(0);
    PCampfire = CIntelligentPointerThingObject((CThing*)GetPNearCampfire());
    bool alive = IS_THING_ALIVE(PCampfire);
    if (!alive) {
        long framesPerSecond = g_framesPerSecond;
        FrameToNextCheckIsRunnable = CWorld::GetFrame() + framesPerSecond * 5;
    }
    return alive;
}