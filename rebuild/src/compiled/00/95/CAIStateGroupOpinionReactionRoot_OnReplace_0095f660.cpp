// CAIStateGroup_OpinionReactionRoot::OnReplace() -- stop any speech, and when a reaction to a known thing was in
// progress log it with the world's opinion-reaction manager; remember whether the last run was a "try and look".
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095F660, length 131 bytes (+ int3 padding). Vtable slot 10.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0xF = CTCTalk, bit 15 of the bitset
// dword at +0x20: `test ch,ch ; jns`), CTCTalk::StopCurrentSpeech @ 0x006E2E60, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 on ThingReactingTo at +0x6b, CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (opinion-reaction manager
// at +0x74), COpinionReactionManager::LogReaction(EOpinionReactionType, CThingAICreature*, bool) @ 0x00700940.
// Shape: `if (GetInterface(&talk, 0xf)) talk->StopCurrentSpeech();` then `if (CurrentReactionType != 0 &&
// ThingReactingTo.GetPItem() != 0) { manager->LogReaction(type, creature, false); LastRunWasTryAndLook = false; }
// else LastRunWasTryAndLook = true;` (the world fetched before the argument loads, the manager read between the
// pushes). Members are the engine header names (CurrentReactionType +0x18, ThingReactingTo +0x6b -- the class is
// packed to 1, LastRunWasTryAndLook +0x88); the header flattens the base so the class is modelled locally.
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

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
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

class CTCTalk { public:                                // interface type 0xF
    void StopCurrentSpeech();                   // 0x006E2E60
};

class COpinionReactionManager { public:
    void LogReaction(long type, CThing* creature, bool flag);    // 0x00700940
};

struct CWorldR {
    unsigned char _pad[0x74];
    COpinionReactionManager* m_pOpinionReactionManager;     // +0x74
};

#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase { public:
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18 EOpinionReactionType
    bool ReactionFlags[79];                     // +0x1c
    CIntelligentPointer<CThing> ThingReactingTo;    // +0x6b CIntelligentPointer<CThing>
    unsigned char _pad_0x73[0x5];               // +0x73
    unsigned char CurrentUpdateFunc[0x10];      // +0x78 pointer to member
    bool LastRunWasTryAndLook;                  // +0x88
    bool ReactingToPlayer;                      // +0x89
    bool AllowSpeechOnNonPureAISpeaker;         // +0x8a
    virtual void OnReplace();
};
#pragma pack(pop)

void CAIStateGroup_OpinionReactionRoot::OnReplace()
{
    CTCTalk* pTalk;
    if (PBrain->PCreature->GetInterface(&pTalk, 0xf))
        pTalk->StopCurrentSpeech();
    if (CurrentReactionType != 0 && ThingReactingTo.GetPItem() != 0) {
        CWorldR* world = (CWorldR*)GetPWorld();    // named: fetched before the argument loads (landed 24a lesson)
        world->m_pOpinionReactionManager->LogReaction(CurrentReactionType, PBrain->PCreature, false);
        LastRunWasTryAndLook = false;
    } else {
        LastRunWasTryAndLook = true;
    }
}