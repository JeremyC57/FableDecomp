// CAIStateGroup_BoastingCrowdMember::Update() -- while the boasting hero is alive, stand in the crowd: when the hero moves more than 2.5 away (or on the first pass, unless the crowd is off the podium) pick a directed spot facing the hero and wander near it, otherwise look at the hero
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095E900, 752 bytes.
// Callees: CBaseIntelligentPointer GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 (CIntelligentPointer<const
//   CThing> vtable 0x01237F1C), CVectorMap::LowerBound 0x0040F020 / const 0x004365B0, CThing::GetPos 0x004C73D0,
//   CTCCreatureGroupBoastingCrowd::CurrentStateIsOffPodiumType 0x007841C0, CTCCreatureNavigation GetNearbyDirectedPosition
//   0x006CB780 / GetNearbyWanderPosition 0x006CB6E0, GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0, AIAction_LookAtThing
//   0x0097D6F0, AIAction_MoveToPosProximity 0x0097E890, Terminate 0x008FCF40. Floats 2.5 @ 0x0129532C, 4.0 @ 0x0123088C,
//   0.3 @ 0x01238170, 3.0 @ 0x01295338.
// Layout: Ego_r.pdb CAIStateGroup_BoastingCrowdMember PCrowdThing +0x14; CTCCreatureGroupBoastingCrowd (interface 0x74)
//   PHero retail +0x24 (PDB +0x28); CThing interface bitset +0x20, map +0x44, flags +0x91.
// Shape: the interface lookups are inline with the found flag written through a bool* and a named map pointer (the crowd
//   through the non-const LowerBound, the navigation through the const one); the brain pointer is named so both creature
//   loads read it; delta/away vectors use the constructor-argument operator- (Z first, dead Z dropped for the 2D test);
//   the look-at speed literal is 0.0027777f (0x3B360A13, not 1/360).
#include <math.h>
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    float Get2DLength() const { return (float)sqrt(X * X + Y * Y); }
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
float __fastcall GFFloatRandom(float range, unsigned long* seed);    // 0x004978A0
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    virtual void v01();
};
class CThing { public:
    virtual void v00();
    unsigned char _pad04[0x20 - 4];
    unsigned long UsingComponents[9];           // +0x20
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91
    const C3DVector& GetPos() const;       // 0x004C73D0
    bool TestTC(long type) const { return ((UsingComponents[type >> 5] >> (type & 31)) & 1) != 0; }
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType, bool* has)
    {
        *has = TestTC(interfaceType);
        if (*has) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
        }
    }
    template<class T> __forceinline void GetInterfaceC(T** ppOut, long interfaceType, bool* has) const
    {
        *has = TestTC(interfaceType);
        if (*has) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
        }
    }
};
class CThingCreatureBase : public CThing { public: };
class CTCCreatureGroupBoastingCrowd { public:           // interface 0x74
    unsigned char _pad00[0x24];
    CBaseIntelligentPointer PHero;              // +0x24
    bool CurrentStateIsOffPodiumType() const;   // 0x007841C0
};
class CTCCreatureNavigation { public:                  // interface 7
    bool GetNearbyDirectedPosition(const C3DVector& from, float distance, const C3DVector& direction, float spread,
                                   C3DVector* out);     // 0x006CB780
    bool GetNearbyWanderPosition(const C3DVector& from, float radius, C3DVector* out, bool flag);   // 0x006CB6E0
};
class CAIBrain { public:
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT, MOVEMENT_WALK = 3, MOVEMENT_RUN = 5 };
class CAIStateGroupBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsRunnable();                  // slot 5 (+0x14)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, long frames,
                                         float speed);   // 0x0097D6F0
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float proximity, bool& moving,
                                                EMovementType anim, bool b1, bool b2, bool b3, const CThing* target,
                                                bool b4, float f);     // 0x0097E890
};
struct CAIStateGroup_BoastingCrowdMember : CAIStateGroupBase {
    CBaseIntelligentPointer PCrowdThing;        // +0x14
    void Update();
};

void CAIStateGroup_BoastingCrowdMember::Update()
{
    if (IsRunnable()) {
        CTCCreatureGroupBoastingCrowd* crowd = 0;
        bool hasCrowd;
        ((CThing*)PCrowdThing.GetPItem())->GetInterface(&crowd, 0x74, &hasCrowd);
        CThing* hero = ((CThing*)crowd->PHero.GetPItem());
        if (hero && !(hero->m_flags91 & 1)) {
            bool moving = false;
            bool first = true;
            C3DVector heroPos = hero->GetPos();
            C3DVector dest;
            for (;;) {
                EAIActionReturn result;
                if (first || !crowd->CurrentStateIsOffPodiumType()) {
                    C3DVector delta = hero->GetPos() - heroPos;
                    if (first || delta.Get2DLength() > 2.5f) {
                        first = false;
                        CTCCreatureNavigation* nav = 0;
                        bool hasNav;
                        CAIBrain* brain = PBrain;
                        brain->Creature->GetInterfaceC(&nav, 7, &hasNav);
                        CThingCreatureBase* creature = brain->Creature;
                        C3DVector away = creature->GetPos() - hero->GetPos();
                        C3DVector directed;
                        if (nav->GetNearbyDirectedPosition(hero->GetPos(), 7.0f, away, 0.33333334f, &directed)) {
                            heroPos = hero->GetPos();
                            if ((GetCreature()->GetPos() - directed).GetLength() > 4.0f
                                && nav->GetNearbyWanderPosition(directed,
                                       (GFFloatRandom(0.7f, GetPWorldSeed()) + 0.3f) * 3.0f, &dest, false))
                                moving = true;
                            else
                                moving = false;
                        }
                    }
                }
                if (moving) {
                    bool walking;
                    result = AIAction_MoveToPosProximity(AI_ACTION_REPEAT_POLL, dest, 1.0f, walking, MOVEMENT_RUN, true, true,
                                                         true, 0, true, 0.0f);
                    if (result == AI_ACTION_DONE) {
                        moving = false;
                        continue;
                    }
                } else
                    result = AIAction_LookAtThing(AI_ACTION_REPEAT_POLL, CIntelligentPointer<const CThing>(hero), 1,
                                                  0.0027777f);
                if (result != AI_ACTION_RUNNING && result != AI_ACTION_DONE)
                    break;
            }
        }
    }
    Terminate();
}