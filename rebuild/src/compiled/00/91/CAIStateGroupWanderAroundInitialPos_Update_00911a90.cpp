// CAIStateGroup_WanderAroundInitialPos::Update() -- walk once to a wander position around the creature's initial position (radius = the native scratchpad's WanderMaxDistance), then, if the creature is alive, clear its targeting list and reset its perception, and terminate
// Leaf: ?Update@CAIStateGroup_WanderAroundInitialPos@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Flags: none
// Headers (chunk 757): the shared engine/C3DVector.h replaces the local vector model (same bytes).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00911A90, length 400 bytes (same TU as the landed IsRunnable @ 0x00911A00).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (four inlined interface lookups: 7 = CTCCreatureNavigation, 170 =
//          CTCAIScratchpadNative, 8 = the targeting component, 52 = CTCPerceiveThing), CTCCreatureNavigation::
//          GetNearbyWanderPosition @ 0x006CB6E0, CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00, the type-8
//          component's list clear @ 0x007F6160 (a thunk `add ecx,48h ; jmp 0x4B8050`), CTCPerceiveThing::
//          ResetPerceptionVariables @ 0x00744DB0, CAIStateGroupBase::Terminate @ 0x008FCF40 (manifest label
//          "CXMVPlaybackInfo" is propagated; 47 landed callers name it Terminate).
// Shape: the first two lookups spill the presence bit as a frame byte (the `bool has = test; if (has)` template form,
//          as in the landed ChickenWander/Wander Update bodies) and the result pointer shares its home with the key; the
//          wander centre is the planar initial position at +0xE8 (same descriptive member as the landed IsRunnable), the
//          radius the scratchpad's WanderMaxDistance (+0x24); the MoveToPos result is dropped. The alive test is the
//          inline `(unsigned char)~flags & 1` (`not cl ; test cl,1`, landed 0x00723AC0 idiom); the last two lookups use the masked-dword HasTC form
//          (`test ch,1` / `test dword ptr`) with the component straight into ecx, each re-reading the brain's creature.
// Fix (chunk 757): the creature local, the two lookups and the wander query live in one inner scope with the result in a
//          bool (`found`); one function-wide creature local swapped esi/edi for the creature and the interface map.
// Layout: CAIStateGroupBase per engine/CAIStateGroupBase.h (PBrain +4); creature bitset +0x20, interface map +0x44,
//          dead flag +0x91, initial position +0xE8 (retail CThingAICreature; PDB names differ, see the landed IsRunnable).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values written as casts here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/C3DVector.h"

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad9[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsAlive() const { return ((unsigned char)~m_flags91 & 1) != 0; }
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    // Spilled-bit form (the presence test stored as a frame byte before the lookup).
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
        }
        return has;
    }
    // Masked-dword form (`test ch,1` / `test dword ptr [eax+24h],100000h`).
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CThingAICreature : public CThing { public:
    unsigned char _pad2[0xe8 - sizeof(CThing)];
    C3DVector InitialPos;                       // +0xe8 (descriptive: the wander centre)
};

class CTCCreatureNavigation { public:          // interface 7 (TCI_CREATURE_NAVIGATION)
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector& result, bool onMap) const;   // 0x006CB6E0
};
class CTCAIScratchpadNative { public:                  // interface 170
    unsigned char _pad[0x24];                   // CTCBase + ThankingPhrase, IgnoringPhrase, WanderCentrePoint, WanderMinDistance
    float WanderMaxDistance;                    // +0x24
};
class CTCTargetingBase { public:               // interface 8 (TCI_TARGETING); ClearTargets is descriptive
    void ClearTargets();                        // 0x007F6160 (thunk: list at +0x48 -> clear @ 0x004B8050)
};
class CTCPerceiveThing { public:                       // interface 52
    void ResetPerceptionVariables();            // 0x00744DB0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

enum EAIActionReturn { };
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 1 };
enum EMovementType { MOVEMENT_TYPE_5 = 5 };   // descriptive value
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
protected:
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& reached, EMovementType movement, bool a, bool b,
        const CThing* avoid, bool c, float speed);   // 0x0097EC00 (Ego_r.pdb, protected)
public:
    void Terminate();                           // 0x008FCF40
};

class CAIStateGroup_WanderAroundInitialPos : public CAIStateGroupBase { public:
    virtual void Update();
};

void CAIStateGroup_WanderAroundInitialPos::Update()
{
    C3DVector target;
    bool found;
    {   // the creature's lookups and the wander query in one scope (retail keeps the creature in esi, then reuses it for &InitialPos)
        CThingAICreature* creature = PBrain->PCreature;
        CTCCreatureNavigation* navigation;
        creature->GetInterfaceT(&navigation, 7);
        CTCAIScratchpadNative* scratchpad;
        creature->GetInterfaceT(&scratchpad, 170);
        found = navigation->GetNearbyWanderPosition(creature->InitialPos, scratchpad->WanderMaxDistance, target, true);
    }
    if (found) {
        bool reached = false;
        AIAction_MoveToPos(AI_ACTION_REPEAT_ONCE, target, reached, MOVEMENT_TYPE_5, true, true, 0, true, 0.0f);
    }
    if (PBrain->PCreature->IsAlive()) {
        CTCTargetingBase* targeting;
        PBrain->PCreature->GetInterface(&targeting, 8);
        targeting->ClearTargets();
        CTCPerceiveThing* perceive;
        PBrain->PCreature->GetInterface(&perceive, 52);
        perceive->ResetPerceptionVariables();
    }
    Terminate();
}