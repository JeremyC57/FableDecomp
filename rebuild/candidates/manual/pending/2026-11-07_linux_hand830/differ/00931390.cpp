// CAIStateGroup_MinionReactToCriminal::Update() -- a villager whose home the hero has broken into: scream and herd the occupants, lock the doors, hide inside away from the hero (or at a gaze-out marker), cower and grovel while the hero stays near
// Grade: DIFFER(3836v3863) -- register colouring (retail keeps result in ebx and this in edi; ours spills result to [esp+20h] and, with this spelling of the scream loops, puts this in esi), the cower-branch CTCTalk lookup retail expands with an inline binary search and a `test ch,ch` bit test, loop-head alignment of the outer loop
// Next idea: find the spelling of the cower-site lookup (an accessor whose lower_bound VC7.1 expands, unlike the 0x0040F020 / 0x004365B0 calls elsewhere in the row); v8 (690 diff lines, 3977 bytes) is the for(;;) spelling of the scream loops with this in edi
// Leaf: ?Update@CAIStateGroup_MinionReactToCriminal@@UAEXXZ
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#include "engine/CMainGameComponent.h"
#include "stlport/fable_containers.h"

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const;
};
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
inline float C3DVector::GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
class CCharString;

class CTCBase;
enum ETCInterfaceType { };
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PData;                         // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    __forceinline CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    template <class U> __forceinline CIntelligentPointer(const CIntelligentPointer<U>& other) { SetPItem((CBaseObject*)other.GetPItem()); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    T* operator->() const { return GetPItem(); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem((CBaseObject*)other.GetPItem()); return *this; }
};
class CThing;
template <> class CIntelligentPointer<CThing> : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    __forceinline CIntelligentPointer(int) { SetPItem(0); }
    CIntelligentPointer(CThing* item);          // 0x004A96F0
    CThing* GetPItem() const { return (CThing*)CBaseIntelligentPointer::GetPItem(); }
    CThing* operator->() const { return GetPItem(); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem((CBaseObject*)other.GetPItem()); return *this; }
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long PresentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad54[0x8c - 0x54];
    unsigned short DefIndex;                    // +0x8c
    unsigned char _pad8e[0x91 - 0x8e];
    unsigned char Flags91;                      // +0x91 bit 0: dead
    const C3DVector& GetPos() const;            // 0x004C73D0
    C3DVector GetPhysPos() const;               // 0x004C7950
    __forceinline bool HasTC(long id) const { return ((PresentBits[id >> 5] >> (id & 31)) & 1) != 0; }
    template <class T> __forceinline bool GetTC(long id, T** out)
    {
        bool present = HasTC(id);
        if (present) {
            ETCInterfaceType type = (ETCInterfaceType)id;
            CInterfaceMap* map = &Components;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *out = (T*)((it == map->end() || id < it->first) ? map->end() : it)->second;
        }
        return present;
    }
    template <class T> __forceinline bool PeekTC(long id, T** out) const
    {
        bool present = HasTC(id);
        if (present) {
            ETCInterfaceType type = (ETCInterfaceType)id;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            *out = (T*)((it == map->end() || id < it->first) ? map->end() : it)->second;
        }
        return present;
    }
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
template <class P> __forceinline bool IS_ALIVE(const P& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }
// Ego_r.pdb / FableWin.pdb ??$IS_THING_ALIVE@...: a __fastcall template, out of line here for CIntelligentPointer<CThing> (0x004CC340)
// and CIntelligentPointer<CThingBuilding> (0x0065F8CA).
template <class P> bool __fastcall IS_THING_ALIVE(const P& p);
class CThingBuilding;

class CThingBuilding : public CThing { public:
    unsigned char _pad92[0xd4 - 0x92];
    bool OverrideAutomaticLocking : 1;          // +0xd4 (Ego_r.pdb bool:1)
    void SetOverrideAutomaticLocking(bool value) { OverrideAutomaticLocking = value; }   // FableWin inline
    bool IsPointInside(const C3DVector& pos) const;   // 0x0082D000
    void LockAllDoors(bool lock, bool b);       // 0x0082DB00
    bool HasOccupants();                        // 0x0082DD90
    bool HasBusyOccupants();                    // 0x0082DE20
};
class CThingPlayerCreature : public CThing { };
class CThingAICreature : public CThing { };

typedef std::list<CIntelligentPointer<CThing> > COccupantList;
class CTCBuilding { public:                     // interface 0x21
    unsigned char _pad[0x0c];
    COccupantList Occupants;                    // +0x0C (STLport list: node next +0, prev +4, item +8)
};
class CTCBuildingSize { public:                 // interface 0x02
    unsigned char _pad[0x34];
    float Radius;                               // +0x34
};
class CTCHouseOwner { public:                   // interface 0x75 on the hero
    unsigned char _pad[0x0c];
    CIntelligentPointer<CThingBuilding> PHouse; // +0x0C
};
class CTCCreatureNavigation { public:           // interface 0x07
    bool GetNearbyWanderPosition(const C3DVector& pos, float radius, C3DVector& out, bool b) const;   // 0x006CB6E0
    bool GetNearbyDirectedPosition(const C3DVector& pos, float radius, const C3DVector& dir, float angle, C3DVector* out);   // 0x006CB780
    bool IsPosNavigable(const C3DVector& pos) const;   // 0x006CBA50
};
class CTCTalk { public:                         // interface 0x0F
    bool IsTalking() const;                     // 0x006E28A0
    long PlayAISpeech(const CCharString& text, bool a, long layer, bool b, bool c);   // 0x006E3B30
};
class CThingFilter_IsNotBeingExclusivelyUsed { public:
    bool operator()(const CThing* thing) const; // 0x006A8AF0
};
namespace NCreatureAI { bool __fastcall IsAwareOfThingIndirectly(CThingAICreature* creature, CThing* thing, bool b); }   // 0x00864700

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};

class CWorld { public:
    static long __fastcall GetFrame();          // 0x0049D870
};
class CSoundPair { public:
    long FirstSound, LastSound;
    CSoundPair(long first, long last) : FirstSound(first), LastSound(last) { }
};
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();             // 0x00693EF0
    unsigned char _pad04[0xb0];
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    CCreatureAction_PlayAnimation(CThingAICreature* creature, const CCharString& animation, const CSoundPair& sound,
                                  bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

enum EAIActionReturn { };
enum EAIActionRepeat { };
enum EMovementType { };
unsigned long __fastcall GFROR13(unsigned long value);       // 0x00497890
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
bool __fastcall CompareOccupants(CThing* a, CThing* b);      // 0x00930D90

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5 (+0x14)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name;                                 // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    __forceinline unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
    void Terminate();                           // 0x008FCF40
    EAIActionReturn AIAction_DoNothing();       // 0x0097E200
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
    EAIActionReturn AIAction_PerformActionTargetFace(const CCreatureActionBase& action, CIntelligentPointer<const CThing> target);   // 0x0097DF60
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, float proximity,
                                                  bool& arrived, EMovementType movementType, bool a, bool b, bool c, bool d,
                                                  float e);   // 0x0097E380
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float proximity, bool& arrived,
                                                EMovementType movementType, bool a, bool b, bool c, const CThing* avoid, bool d,
                                                float e);   // 0x0097E890
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& arrived, EMovementType movementType, bool a,
                                       bool b, const CThing* avoid, bool c, float d);   // 0x0097EC00
};
class CAIStateGroup_MinionReactToCriminal : public CAIStateGroupBase { public:
    CIntelligentPointer<CThingPlayerCreature> PHero;   // +0x14
    CIntelligentPointer<CThingBuilding> PHome;  // +0x1C
    long FrameLastLeftStategroup;               // +0x24
    long FrameToNextSpeak;                      // +0x28
    bool OverrodeLocking;                       // +0x2C
    bool HeroInsideHouse;                       // +0x2D
    bool MeInsideHouse;                         // +0x2E
    bool GuardInsideHouse;                      // +0x2F
    virtual void Update();
    __forceinline void SetNextSpeakFrame(float minSeconds, float range)
    {
        float delay = (GFFloatRandom(range, GetPWorldSeed()) + minSeconds) * CMainGameComponent::GetConstantFPS();
        FrameToNextSpeak = (long)(CWorld::GetFrame() + delay);
    }
};

void CAIStateGroup_MinionReactToCriminal::Update()
{
    if (!IsStillValid()) {
        Terminate();
        return;
    }
    long result = 3;
    do {
        if (!MeInsideHouse && IS_ALIVE(PHome) && PHome->HasBusyOccupants()) {
            std::vector<CThing*> occupants;
            const CTCBuilding* building;
            if (PHome->PeekTC(0x21, &building)) {
                for (COccupantList::const_iterator it = building->Occupants.begin(); it != building->Occupants.end(); ++it) {
                    if (IS_ALIVE((*it)) && (*it).GetPItem()->HasTC(0x26))
                        occupants.push_back((*it).GetPItem());
                }
            }
            if (occupants.size() != 0) {
                std::sort(occupants.begin(), occupants.end(), CompareOccupants);
                CIntelligentPointer<const CThing> target(occupants[0]);
                do {
                    if (CWorld::GetFrame() > FrameToNextSpeak) {
                        CTCTalk* talk;
                        if (GetCreature()->GetTC(0x0F, &talk) && !talk->IsTalking()) {
                            long spoken;
                            do
                                spoken = talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF", true, 2, false, true);
                            while (spoken == 2 && AIAction_DoNothing() == 4);
                            if (spoken == 2) {
                                Terminate();
                                return;
                            }
                        }
                        SetNextSpeakFrame(5.0f, 10.0f);
                    }
                    if (!IS_ALIVE(target)) {
                        Terminate();
                        break;
                    }
                    bool arrived;
                    result = AIAction_MoveToThingProximity((EAIActionRepeat)2, target.GetPItem(), 1.2f, arrived, (EMovementType)5, true, false, true, true, 0.0f);
                } while (result == 4 && !HeroInsideHouse);
                if (result == 1) {
                    Terminate();
                    return;
                }
                if (result == 2) {
                    OverrodeLocking = true;
                    PHome->SetOverrideAutomaticLocking(true);
                    PHome->LockAllDoors(false, true);
                }
            }
        }
        if (!MeInsideHouse && IS_ALIVE(PHome) && !PHome->HasBusyOccupants()) {
            const CTCBuildingSize* size;
            PHome->PeekTC(0x02, &size);
            const CTCCreatureNavigation* navigation;
            GetCreature()->PeekTC(0x07, &navigation);
            C3DVector pos;
            if (navigation->GetNearbyWanderPosition(PHome->GetPhysPos(), size->Radius * 0.5f, pos, false) &&
                PHome->IsPointInside(pos) && navigation->IsPosNavigable(pos)) {
                do {
                    if (CWorld::GetFrame() > FrameToNextSpeak) {
                        CTCTalk* talk;
                        if (GetCreature()->GetTC(0x0F, &talk) && !talk->IsTalking()) {
                            long spoken;
                            do
                                spoken = talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF", true, 2, false, true);
                            while (spoken == 2 && AIAction_DoNothing() == 4);
                            if (spoken == 2) {
                                Terminate();
                                return;
                            }
                        }
                        SetNextSpeakFrame(5.0f, 10.0f);
                    }
                    bool arrived;
                    result = AIAction_MoveToPosProximity((EAIActionRepeat)2, pos, 1.0f, arrived, (EMovementType)5, true, true, true, 0, true, 0.0f);
                } while (result == 4 && !HeroInsideHouse);
                if (result == 1) {
                    Terminate();
                    return;
                }
            }
        }
        if (MeInsideHouse && !HeroInsideHouse && !GuardInsideHouse && IS_ALIVE(PHome) && PHome->HasOccupants() &&
            !PHome->HasBusyOccupants()) {
            CTCHouseOwner* owner;
            if (PHero->GetTC(0x75, &owner) && owner->PHouse.GetPItem() != PHome.GetPItem() && !PHome->HasBusyOccupants()) {
                PHome->SetOverrideAutomaticLocking(true);
                OverrodeLocking = true;
                PHome->LockAllDoors(true, true);
            }
            CIntelligentPointer<CThing> marker(0);
            CTCBuilding* building;
            if (PHome->GetTC(0x21, &building)) {
                std::vector<CThing*> markers;
                long markerDef = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GAZE_OUT_OF_BUILDING_MARKER");
                CThingFilter_IsNotBeingExclusivelyUsed notUsed;
                for (COccupantList::const_iterator it = building->Occupants.begin(); it != building->Occupants.end(); ++it) {
                    if (IS_THING_ALIVE((*it)) && (*it).GetPItem()->DefIndex == markerDef &&
                        PHome->IsPointInside((*it).GetPItem()->GetPos()) && notUsed((*it).GetPItem())) {
                        CThing* thing = (*it).GetPItem();
                        markers.push_back(thing);
                    }
                }
                if (!markers.empty())
                    marker = CIntelligentPointer<CThing>(markers[GFRandom(markers.size(), GetPWorldSeed())]);
            }
            if (IS_ALIVE(marker) && !HeroInsideHouse) {
                bool arrived;
                do
                    result = AIAction_MoveToPos((EAIActionRepeat)2, marker->GetPos(), arrived, (EMovementType)5, true, true, 0, true, 0.0f);
                while (result == 4 && !HeroInsideHouse);
                if (result != 2 && !HeroInsideHouse) {
                    Terminate();
                    return;
                }
            }
            if (!HeroInsideHouse) {
                long until = (long)(CWorld::GetFrame() + (GFFloatRandom(3.0f, GetPWorldSeed()) + 2.0f) * CMainGameComponent::GetConstantFPS());
                do {
                    result = AIAction_LookAtThing((EAIActionRepeat)2, PHero, 0x27, 1.0f / 360.0f);
                    if (result != 4 && result != 2)
                        break;
                } while (CWorld::GetFrame() < until && !HeroInsideHouse);
                if (result != 2 && CWorld::GetFrame() < until && !HeroInsideHouse) {
                    Terminate();
                    return;
                }
            }
        } else if (IS_ALIVE(PHero) && NCreatureAI::IsAwareOfThingIndirectly(GetCreature(), (CThing*)PHero.GetPItem(), false)) {
            const CTCCreatureNavigation* navigation;
            GetCreature()->PeekTC(0x07, &navigation);
            C3DVector away = GetCreature()->GetPos() - PHero->GetPos();
            C3DVector dest;
            if (((CTCCreatureNavigation*)navigation)->GetNearbyDirectedPosition(GetCreature()->GetPos(), 5.0f, away, 60.0f, &dest) &&
                (GetCreature()->GetPos() - dest).GetLength() > 0.5f &&
                (!MeInsideHouse || HeroInsideHouse || !IS_THING_ALIVE(PHome) || PHome->IsPointInside(dest))) {
                if (CWorld::GetFrame() > FrameToNextSpeak) {
                    CTCTalk* talk;
                    if (GetCreature()->GetTC(0x0F, &talk) && !talk->IsTalking()) {
                        long spoken;
                        do
                            spoken = talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF", true, 2, false, true);
                        while (spoken == 2 && AIAction_DoNothing() == 4);
                        if (spoken == 2) {
                            Terminate();
                            return;
                        }
                    }
                    SetNextSpeakFrame(5.0f, 10.0f);
                }
                bool arrived;
                result = AIAction_MoveToPos((EAIActionRepeat)1, dest, arrived, (EMovementType)5, true, true, 0, true, 0.0f);
            } else {
                do {
                    if (CWorld::GetFrame() > FrameToNextSpeak) {
                        CTCTalk* talk;
                        if (GetCreature()->GetTC(0x0F, &talk) && !talk->IsTalking()) {
                            for (;;) {
                                long spoken;
                                if (Random(5) > 0)
                                    spoken = talk->PlayAISpeech("TEXT_AI_OPINION_ACT_COWER_TO_SELF", true, 2, false, true);
                                else
                                    spoken = talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SERVILE_GROVELLING_LARGE_TO_HERO", false, 1, false, true);
                                if (spoken != 2)
                                    break;
                                if (AIAction_DoNothing() != 4) {
                                    Terminate();
                                    return;
                                }
                            }
                        }
                        SetNextSpeakFrame(5.0f, 10.0f);
                    }
                    result = AIAction_PerformActionTargetFace(CCreatureAction_PlayAnimation(GetCreature(), "ST_IDLE_SUBTLE",
                        CSoundPair(0, 0), false, true, false, 0, 0), PHero);
                } while (NCreatureAI::IsAwareOfThingIndirectly(GetCreature(), (CThing*)PHero.GetPItem(), false) &&
                         (result == 4 || result == 2));
            }
        } else
            result = AIAction_DoNothing();
    } while (result == 4 || result == 2);
    Terminate();
}
