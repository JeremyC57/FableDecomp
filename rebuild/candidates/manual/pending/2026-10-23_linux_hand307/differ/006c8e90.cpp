// Grade: DIFFER(796v804) -- register/frame allocation: retail keeps thing in ebp and this in edi (spilled to the push-ecx slot and reloaded into ebx for the unknown-threat block) and homes every lookup key/out-pointer in the dead thing argument slot; ours swaps ebp/edi and opens an 8-byte frame, so the faction call loads differ and the early false exit is placed inline; the control flow (active test, mode-13 test, opinions/deed/is-not/faction filters, hearing-or-perceiving test, IgnoreFOV sees test, unknown-threat target-awareness and sight-radius/nav line-of-sight test) is modelled
// CThingFilter_IsEligibleWitness::operator()(CThing* thing) const -- whether a thing can witness the deed: an active
// thing not in creature mode 13, with opinions, the deed type enabled, optionally not the recipient, optionally
// allied with (or, reversed, an enemy of) the faction, and optionally perceiving the perpetrator; an unknown-threat
// search instead wants a witness that does NOT see the perpetrator but is aware of the deed's target or has a
// navigation line of sight to the deed position within its extended sight radius
// Leaf: ??RCThingFilter_IsEligibleWitness
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C8E90, `ret 4`.
// Callees: CInterfaceMap::LowerBound const 0x004365B0, CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 (TC 0x31),
//   CThingFilter_HasTC::operator() 0x006A80A0, CThingFilter_IsDeedTypeEnabled::operator() 0x006C8D40,
//   CThingFilter_IsNot::operator() 0x006A76A0, CTCEnemy 0x0076C7D0 (enemy) / 0x0076C7F0 (allied) (TC 0x49),
//   IsAwareOfThingImplementation 0x00864700 (__fastcall), CTCPerceiveThing::GetPerceivedThing 0x007446A0 /
//   GetExtendedSightRadius 0x007448F0 (TC 0x34), IS_THING_ALIVE 0x0069CE20, CBaseIntelligentPointer::GetPItem
//   0x00A01B50, CThing::GetPosition 0x004C73D0, CTCCreatureNavigation::IsNavLineOfSight 0x006CB610 (TC 7).
// Layout: Ego_r.pdb CThingFilter_IsEligibleWitness (42 bytes, as the landed ctor 0x006C8DE0); retail CThing
//   interface bitset +0x20 / map +0x44 (PDB view is 0x10 larger, so modelled locally); COpinionDeed Position +8,
//   PTarget +0x18; CTCPerceiveThing LineOfSight +0x2d, IgnoreFOVLineOfSight +0x2f, Smelt +0x31, Heard +0x32.
struct CTCBase;
struct C3DVector { float x, y, z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char State;                        // +0x10 (1 = active)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline bool GetInterface(T** out, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingAICreature;
class CFaction;
struct CBaseIntelligentPointer {
    void* _p[2];
    CThing* GetPItem() const;                   // 0x00A01B50
};
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer& p);                                    // 0x0069CE20
bool __fastcall IsAwareOfThingImplementation(const CThingAICreature* creature, const CThing* thing, bool flag); // 0x00864700
struct COpinionDeed {
    long Type;                                  // +0x00
    long DeedHandle;                            // +0x04
    C3DVector Position;                         // +0x08
    long FrameHappened;                         // +0x14
    CBaseIntelligentPointer PTarget;            // +0x18
};
struct CMode {
    virtual void v0();
    virtual long GetType();                     // slot 1 (+4)
};
struct CTCCreatureModeManager {
    CMode* GetCurrentMode();                    // 0x0073A6E0
};
struct CTCEnemy {
    bool IsEnemyOfFaction(const CFaction* faction) const;   // 0x0076C7D0
    bool IsAlliedWithFaction(const CFaction* faction) const; // 0x0076C7F0
};
struct CTCPerceiveThing {
    unsigned char _pad00[0x2c];
    bool Perceiving;                            // +0x2c
    bool LineOfSight;                           // +0x2d
    bool ExtendedLineOfSight;                   // +0x2e
    bool IgnoreFOVLineOfSight;                  // +0x2f
    bool DirectAwareness;                       // +0x30
    bool Smelt;                                 // +0x31
    bool Heard;                                 // +0x32
    CThing* GetPerceivedThing();                // 0x007446A0
    float GetExtendedSightRadius();             // 0x007448F0
};
struct CTCCreatureNavigation {
    bool IsNavLineOfSight(const C3DVector* from, const C3DVector& to);   // 0x006CB610
};

struct CThingFilter_HasTC {
    long Type;
    bool operator()(const CThing* thing) const;     // 0x006A80A0
};
struct CThingFilter_IsDeedTypeEnabled {
    long DeedType;
    bool operator()(const CThing* thing) const;     // 0x006C8D40
};
struct CThingFilter_IsPerceivingThingAndIsThingPerceived {
    CThing* PerceivedThing;
    bool IncludeHearing;
    __forceinline bool operator()(const CThing* thing) const
    {
        CTCPerceiveThing* perceive;
        thing->GetInterface(&perceive, 0x34);
        return perceive->GetPerceivedThing() == PerceivedThing
            && ((IncludeHearing && perceive->Heard) || perceive->LineOfSight || perceive->Smelt);
    }
};
struct CThingFilter_IsNot {
    const CThing* Thing;
    bool operator()(const CThing* thing) const;     // 0x006A76A0
};
struct CThingFilter_IsAlliedWithFaction {
    CFaction* PFaction;
    __forceinline bool operator()(const CThing* thing) const
    {
        CTCEnemy* enemy;
        if (thing->GetInterface(&enemy, 0x49))
            return enemy->IsAlliedWithFaction(PFaction);
        return false;
    }
};
struct CThingFilter_IsEnemyOfFaction {
    CFaction* PFaction;
    __forceinline bool operator()(const CThing* thing) const
    {
        CTCEnemy* enemy;
        if (thing->GetInterface(&enemy, 0x49))
            return enemy->IsEnemyOfFaction(PFaction);
        return false;
    }
};

struct CThingFilter_IsEligibleWitness {
    CThing*                                           Perpetrator;             // +0x00
    COpinionDeed*                                     PDeed;                   // +0x04
    CThingFilter_HasTC                                HasOpinions;             // +0x08
    CThingFilter_IsDeedTypeEnabled                    DeedTypeEnabled;         // +0x0c
    CThingFilter_IsPerceivingThingAndIsThingPerceived IsPerceivingPerpetrator; // +0x10
    CThingFilter_IsNot                                IsNotRecipient;          // +0x18
    CThingFilter_IsAlliedWithFaction                  IsAllied;                // +0x1c
    CThingFilter_IsEnemyOfFaction                     IsEnemy;                 // +0x20
    bool UnknownThreatSearch;                                                  // +0x24
    bool UseHearing;                                                           // +0x25
    bool UsePerceiving;                                                        // +0x26
    bool UseIsNot;                                                             // +0x27
    bool UseFaction;                                                           // +0x28
    bool ReverseFactionSearch;                                                 // +0x29
    __forceinline bool IsUnknownThreatWitness(CThing* thing) const
    {
        if (PDeed && IS_THING_ALIVE(PDeed->PTarget)
            && IsAwareOfThingImplementation((const CThingAICreature*)thing, PDeed->PTarget.GetPItem(), false))
            return true;
        if (PDeed && thing->HasTC(0x34)) {
            CTCPerceiveThing* perceive = thing->GetTC<CTCPerceiveThing>(0x34);
            if (thing->HasTC(7)) {
                CTCCreatureNavigation* nav = thing->GetTC<CTCCreatureNavigation>(7);
                float radius = perceive->GetExtendedSightRadius();
                const C3DVector& deedPos = PDeed->Position;
                const C3DVector& pos = thing->GetPosition();
                float dx = pos.x - deedPos.x;
                float dy = pos.y - deedPos.y;
                if (dy * dy + dx * dx < radius * radius
                    && nav->IsNavLineOfSight(&thing->GetPosition(), PDeed->Position))
                    return true;
            }
        }
        return false;
    }
    __forceinline bool SeesPerpetrator(const CThing* thing) const
    {
        if (thing->HasTC(0x34)) {
            CTCPerceiveThing* perceive = thing->GetTC<CTCPerceiveThing>(0x34);
            if (perceive->GetPerceivedThing() == Perpetrator && perceive->IgnoreFOVLineOfSight)
                return true;
        }
        return false;
    }
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsEligibleWitness::operator()(CThing* thing) const
{
    if (thing->State != 1)
        return false;
    if (thing->HasTC(0x31) && thing->GetTC<CTCCreatureModeManager>(0x31)->GetCurrentMode()->GetType() == 13)
        return false;
    if (!HasOpinions(thing))
        return false;
    if (!DeedTypeEnabled(thing))
        return false;
    if (UseIsNot && !IsNotRecipient(thing))
        return false;
    bool result = true;
    if (UseFaction)
        result = ReverseFactionSearch ? IsEnemy(thing) : IsAllied(thing);
    if (result && UsePerceiving) {
        bool perceiving = UseHearing ? IsAwareOfThingImplementation((const CThingAICreature*)thing, Perpetrator, false)
                                     : IsPerceivingPerpetrator(thing);
        bool sees = perceiving && SeesPerpetrator(thing);
        if (UnknownThreatSearch) {
            bool unseen = !sees;
            return unseen && IsUnknownThreatWitness(thing);
        }
        return sees;
    }
    return result;
}
