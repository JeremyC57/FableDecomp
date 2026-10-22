// Grade: DIFFER(856v863) -- navigation component kept in a register (retail homes it at [esp+0x14]) so the faction pointer and brain take other registers; map-id compare now loads all four dwords like retail
// CAIStateGroup_OpinionReactionRoot::FleeUpdate() -- flee from the thing reacted to: towards the nearest navigable ally of the creature's faction when the threat is on the same map, else to the nearest flee position; run there in flee mode for 4..10 seconds, screaming once on the way; true while the run succeeded or is still going
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00984F60, length 864 bytes (+ int3 padding), plain `ret`, frame 0x34.
// Callees: CVectorMap::LowerBound @ 0x0040F020 / its const twin @ 0x004365B0 (7 navigation, 0x49 faction, 0x31 mode manager),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0, PeekThingSearchTools @ 0x008FD000,
// GetPBestThing<Bind2<IsNot, IsAlliedWithFaction>, Nearest> @ 0x00986D90, CTCCreatureNavigation::IsPosNavigable @ 0x006CBA50 /
// GetNearbyDirectedPosition @ 0x006CB780 / GetNearestFleePosition @ 0x006CB840, CTCCreatureModeManager::AddMode @ 0x0073AE10 /
// RemoveMode @ 0x0073ABA0, GetPWorldSeed @ 0x008FCFD0, GFFloatRandom @ 0x004978A0, GFGetCurrentFrame @ 0x0049D870, __ftol2
// @ 0x00BFEA70, AIAction_MoveToPosProximity @ 0x0097E890, CCharString ctor @ 0x0099EBF0 / dtor @ 0x0099EAE0,
// WaitForTextToLoadThenSpeak @ 0x0095ED80.
// Layout: CAIStateGroup_OpinionReactionRoot packed as the engine header (ThingReactingTo at +0x6b), as the landed
// TryAndLookOnAwarenessUpdate 0x009830C0; CThing map id pair +0x18/+0x1c; faction component's faction +0x18.
// Shape: the component lookups for 7, 0x49 and 0x31 are the word-local HasTC (byte homed), the final 0x31 lookup the masked
// test; the arrival flag of the run is the same bool as the flee-position result.
extern long g_FramesPerSecond;                  // 0x01375550
long __fastcall GFGetCurrentFrame();             // 0x0049D870
float __fastcall GFFloatRandom(unsigned long* seed, float range);   // 0x004978A0
class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
private:
    void* PStringData;
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CMapCoords { long X, Y; };
inline bool operator==(CMapCoords a, CMapCoords b) { return a.X == b.X && a.Y == b.Y; }
struct CThing {
    unsigned char _pad0[0x18];
    CMapCoords MapID;                           // +0x18
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceWord(T** ppOut, long interfaceType)
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
    template<class T> __forceinline bool GetInterfaceWord(const T** ppOut, long interfaceType) const
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (const T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing { };
struct CThingAICreature : CThingCreatureBase { };
struct CFaction;
struct CTCFaction {                             // interface 0x49
    unsigned char _pad[0x18];
    CFaction* PFaction;                         // +0x18
};
struct CTCCreatureNavigation {                  // interface 7
    bool IsPosNavigable(const C3DVector& pos);                                                // 0x006CBA50
    bool GetNearbyDirectedPosition(const C3DVector& pos, float radius, const C3DVector& direction, float spread,
                                   C3DVector* out);                                           // 0x006CB780
    bool GetNearestFleePosition(const C3DVector& pos, float radius, C3DVector* out);         // 0x006CB840
};
struct CTCCreatureModeManager {                 // interface 0x31
    bool AddMode(unsigned long mode);           // 0x0073AE10
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CThingFilter_IsNot { const CThing* Thing; };
struct CThingFilter_IsAlliedWithFaction { CFaction* Faction; };
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
template<class F1, class F2> struct CFilter_Bind2 {
    const F1* PF1; const F2* PF2;
    CFilter_Bind2(const F1& f1, const F2& f2) : PF1(&f1), PF2(&f2) { }
};
typedef CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsAlliedWithFaction> CAllyFilter;
struct CThingSearchTools {
    bool GetPBestThing(const CAllyFilter& filter, const CThingCompare_Nearest& compare, CThingAICreature** best);   // 0x00986D90
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float proximity, bool* arrived, long movement,
                                     bool a, bool b, bool c, const CThing* ignore, bool d, float e);   // 0x0097E890
};
struct CTCTalk;
#pragma pack(push, 1)
struct CAIStateGroup_OpinionReactionRoot : CAIStateGroupBase {
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    CBaseIntelligentPointer ThingReactingTo;    // +0x6b CIntelligentPointer<CThing>
    long WaitForTextToLoadThenSpeak(const CCharString& text, long layer);   // 0x0095ED80
    bool FleeUpdate();
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::FleeUpdate()
{
    CTCCreatureNavigation* nav;
    GetCreature()->GetInterfaceWord(&nav, 7);
    bool ok = false;
    C3DVector fleePos;
    CMapCoords threatMap = ThingReactingTo.GetPItem()->MapID;
    CMapCoords myMap = GetCreature()->MapID;
    if (threatMap.X == myMap.X && threatMap.Y == myMap.Y) {
        const CTCFaction* faction;
        GetCreature()->GetInterfaceWord(&faction, 0x49);
        CThingCompare_Nearest nearest(GetCreature()->GetPosition());
        CThingFilter_IsAlliedWithFaction allied = { faction->PFaction };
        CThingFilter_IsNot notMe = { GetCreature() };
        CThingAICreature* ally;
        if (PeekThingSearchTools()->GetPBestThing(CAllyFilter(notMe, allied), nearest, &ally)
                && nav->IsPosNavigable(ally->GetPosition())) {
            C3DVector direction = ally->GetPosition() - GetCreature()->GetPosition();
            ok = nav->GetNearbyDirectedPosition(GetCreature()->GetPosition(), 15.0f, direction, 0.083333336f, &fleePos);
        }
    }
    if (!ok) {
        ok = nav->GetNearestFleePosition(GetCreature()->GetPosition(), 15.0f, &fleePos);
        if (!ok)
            return false;
    }
    CTCCreatureModeManager* modes;
    GetCreature()->GetInterfaceWord(&modes, 0x31);
    bool modeAdded = modes->AddMode(0xf);
    long framesPerSecond = g_FramesPerSecond;
    float delay = (GFFloatRandom(GetPWorldSeed(), 6.0f) + 4.0f) * framesPerSecond;
    unsigned long endFrame = (unsigned long)(GFGetCurrentFrame() + delay);
    bool screamed = false;
    long result;
    do {
        result = AIAction_MoveToPosProximity(2, fleePos, 1.0f, &ok, 5, true, false, true, 0, true, 0.0f);
        if (!screamed && ok) {
            screamed = true;
            if (WaitForTextToLoadThenSpeak(CCharString("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF"), 2) != 2)
                return false;
        }
    } while (result == 4 && (unsigned long)GFGetCurrentFrame() < endFrame);
    if (modeAdded) {
        GetCreature()->GetInterface(&modes, 0x31);
        modes->RemoveMode(0xf);
    }
    return result == 2 || result == 4;
}
