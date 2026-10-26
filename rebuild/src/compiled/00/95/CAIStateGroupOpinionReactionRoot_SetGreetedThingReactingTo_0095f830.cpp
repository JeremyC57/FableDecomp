// CAIStateGroup_OpinionReactionRoot::SetGreetedThingReactingTo() -- when the reaction is to the player, mark the creature's opinion-of-hero component as interacted with and greeted
// Leaf: ?SetGreetedThingReactingTo@CAIStateGroup_OpinionReactionRoot
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095F830, 101 bytes, plain `ret`.
// Callees: CVectorMap LowerBound @ 0x0040F020 (the inlined GetInterface of type 0x4F = CTCCreatureOpinionOfHero: bit 15
//   of the bitset word +0x28, tested as `test ch,ch ; jns`).
// Shape: `if (ReactingToPlayer) { GetInterface(&opinion, 0x4F); opinion->InteractedFlag = true; opinion->GreetedFlag =
//   true; }` -- the GetInterface result is not tested, so the miss path stores through the never-written out pointer
//   (`mov eax,[esp]`, the frame word shared with the lookup key) and VC7.1 tail-duplicates the two flag stores into both
//   paths (`mov cl,1` + two byte stores each).
// Layout: Ego_r.pdb CAIStateGroup_OpinionReactionRoot: ReactionFlags bool[79] at +0x1c, ThingReactingTo (an 8-byte
//   CIntelligentPointer) packed at +0x6b, CurrentUpdateFunc (16-byte member-function pointer) +0x78,
//   LastRunWasTryAndLook +0x88, ReactingToPlayer +0x89. The unaligned members are held as byte arrays so no pack pragma
//   is needed (same layout as the landed sibling 0x0095F7D0). CTCCreatureOpinionOfHero InteractedFlag / GreetedFlag are
//   adjacent bools, at +0x340 / +0x341 in retail (Ego_r.pdb's debug layout has them at +0x64C / +0x64D).
// Label: catalog "CThing_UpdateSkillState" -> Ego_r.pdb ?SetGreetedThingReactingTo@CAIStateGroup_OpinionReactionRoot@@AAEXXZ.
// Build: the lane defaults (/O2 /Oy /W3), no pragma.
// Layout (supersedes any local-model note above): engine/CTCCreatureOpinionOfHero.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCCreatureOpinionOfHero CTCCreatureOpinionOfHeroDonor
#include "engine/CTCCreatureOpinionOfHero.h"
#undef CTCCreatureOpinionOfHero
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 presence bitset of the interface types
    CInterfaceMap m_interfaces;                 // +0x44 sorted {type, component} vector map
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
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
};
class CTCCreatureOpinionOfHero : public CTCCreatureOpinionOfHeroDonor {
 public:               // interface 0x4F
};
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase {
public:
    void* PTCTalk;                              // +0x14 CTCTalk*
    long CurrentReactionType;                   // +0x18 EOpinionReactionType
    bool ReactionFlags[79];                     // +0x1c
    unsigned char ThingReactingTo[8];           // +0x6b CIntelligentPointer<CThing> (packed, unaligned)
    unsigned char _pad73[5];                    // +0x73
    unsigned char CurrentUpdateFunc[0x10];      // +0x78 member-function pointer
    bool LastRunWasTryAndLook;                  // +0x88
    bool ReactingToPlayer;                      // +0x89
private:
    void SetGreetedThingReactingTo();
};

void CAIStateGroup_OpinionReactionRoot::SetGreetedThingReactingTo()
{
    if (ReactingToPlayer) {
        CTCCreatureOpinionOfHero* opinion;
        PBrain->PCreature->GetInterface(&opinion, 0x4F);
        opinion->InteractedFlag = true;
        opinion->GreetedFlag = true;
    }
}