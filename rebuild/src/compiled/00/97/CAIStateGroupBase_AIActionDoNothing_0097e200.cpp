// CAIStateGroupBase::AIAction_DoNothing() -- idle action: unless stopping, point the creature's look component at the nearest interesting thing and tick the brain; returns 1 (stopping) or 4 (keep going)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097E200, length 112 bytes.
// Callees: CAIStateGroupBase::IsStopping @ 0x008FD010 (twice), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (the inlined GetInterface, interface 0x43 = CTCLook: bitset word 2, bit 3 -> `test byte ptr [eax+28h],8`),
// CTCLook::TrackNearestInterestingThing(bool) @ 0x00753840, the brain's virtual slot 2 (+0x08).
// Shape: the result is an action-status enum: an early `return 1` (`mov eax,1`) and a final
// `IsStopping() ? 1 : 4` lowered branch-free (`neg al ; sbb eax,eax ; and eax,-3 ; add eax,4`).
// The look component comes from the landed GetInterface out-parameter template inside the `if`; it must be
// `__forceinline` here (plain inline emits it out of line next to the two IsStopping calls).
// Model: local (engine/CAIStateGroupBase.h and engine/CAIBrain.h carry the vptr as a plain word, and the brain
// call here is virtual); PDB member names kept.
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThingAICreature {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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

struct CTCLook {
    bool TrackNearestInterestingThing(bool onlyIfNone);   // 0x00753840
};

struct CAIBrain {
    virtual void v00(); virtual void v01();
    virtual void Update();                      // slot 2 (+0x08)
    unsigned char _pad[0x14];
    CThingAICreature* Creature;                 // +0x18
};

enum EAIActionResult { AI_ACTION_FINISHED = 1, AI_ACTION_CONTINUE = 4 };

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    bool IsStopping();                          // 0x008FD010
    EAIActionResult AIAction_DoNothing();
};

EAIActionResult CAIStateGroupBase::AIAction_DoNothing()
{
    if (IsStopping())
        return AI_ACTION_FINISHED;
    CTCLook* look;
    if (PBrain->Creature->GetInterface(&look, 0x43))
        look->TrackNearestInterestingThing(true);
    PBrain->Update();
    return IsStopping() ? AI_ACTION_FINISHED : AI_ACTION_CONTINUE;
}