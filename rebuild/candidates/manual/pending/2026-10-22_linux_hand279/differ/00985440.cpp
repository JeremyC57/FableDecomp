// Grade: DIFFER(623v625) -- two hunks left: the zero store of the hero out-pointer is scheduled before the presence-word load (retail after it), and the creature for GetPosition is loaded into esi before the sight-radius call in retail (ours loads ecx from the brain after it)
// CAIStateGroup_OpinionReactionRoot::HeroImitationPlayUpdate() -- imitate the hero at play: look for another creature of type 1 (the hero) within sight, turn to face the thing reacted to, say the hero-imitation line (about the hero when one was found, else to self), then move around the thing reacted to in play mode for 2..5 seconds; true while the run succeeded or is still going
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00985440, length 625 bytes (+ int3 padding), plain `ret`, frame 0x30.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (0x34 perception, 0x31 mode manager), CTCPerceiveThing sight radius
// @ 0x007448F0, CThing::GetPosition @ 0x004C73D0, PeekThingSearchTools @ 0x008FD000,
// GetPFirstThing<Bind3<IsNot, IsCreatureType, In2DRange>, const CThingAICreature> @ 0x00986E30,
// CTCCreatureModeManager::AddMode @ 0x0073AE10 / RemoveMode @ 0x0073ABA0, TurnToFace @ 0x0095EDF0,
// CCharString ctor @ 0x0099EBF0 / dtor @ 0x0099EAE0, WaitForTextToLoadThenSpeak @ 0x0095ED80, GetPWorldSeed
// @ 0x008FCFD0, GFFloatRandom @ 0x004978A0, GFGetCurrentFrame @ 0x0049D870, __ftol2 @ 0x00BFEA70,
// CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90,
// AIAction_MoveToThingProximity @ 0x0097E380.
// Layout: CAIStateGroup_OpinionReactionRoot packed as the engine header (ThingReactingTo at +0x6b), as the landed
// TryAndLookOnAwarenessUpdate 0x009830C0; In2DRange { C3DVector Pos; float Range; } as the landed 0x006A76B0.
// Shape: the 0x34 and 0x31 lookups are the word-local HasTC (byte homed) into zero-initialised pointers, the final
// 0x31 lookup the masked test.
extern long g_FramesPerSecond;                  // 0x01375550
long __fastcall GFGetCurrentFrame();             // 0x0049D870
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
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
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
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
struct CTCPerceiveThing {                       // interface 0x34
    float GetSightRadius();                     // 0x007448F0
};
struct CTCCreatureModeManager {                 // interface 0x31
    bool AddMode(unsigned long mode);           // 0x0073AE10
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CThingFilter_IsNot {
    const CThing* Excluded;
    CThingFilter_IsNot(const CThing* thing) : Excluded(thing) { }
};
struct CThingFilter_IsCreatureType {
    long Type;
    CThingFilter_IsCreatureType(long type) : Type(type) { }
};
struct CThingFilter_In2DRange {
    C3DVector Pos;
    float Range;
    __forceinline CThingFilter_In2DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) { }
};
template<class F1, class F2, class F3> struct CFilter_Bind3 {
    const F1* PF1; const F2* PF2; const F3* PF3;
    CFilter_Bind3(const F1& f1, const F2& f2, const F3& f3) : PF1(&f1), PF2(&f2), PF3(&f3) { }
};
typedef CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsCreatureType, CThingFilter_In2DRange> CHeroFilter;
struct CThingSearchTools {
    bool GetPFirstThing(const CHeroFilter& filter, const CThingAICreature** first);   // 0x00986E30
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {          // CIntelligentPointer<CThing>
    virtual void v00();
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
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
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float proximity,
        bool* arrived, long movement, bool a, bool b, bool c, bool d, float e);    // 0x0097E380
};
struct CTCTalk;
#pragma pack(push, 1)
struct CAIStateGroup_OpinionReactionRoot : CAIStateGroupBase {
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    CIntelligentPointerThing ThingReactingTo;   // +0x6b
    long WaitForTextToLoadThenSpeak(const CCharString& text, long layer);   // 0x0095ED80
    bool TurnToFace();                          // 0x0095EDF0
    bool HeroImitationPlayUpdate();
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::HeroImitationPlayUpdate()
{
    bool foundHero;
    {
        CAIBrain* brain = PBrain;
        CTCPerceiveThing* perceive = 0;
        const CThingAICreature* hero = 0;
        brain->Creature->GetInterfaceWord(&perceive, 0x34);
        CThingFilter_In2DRange inRange(brain->Creature->GetPosition(), perceive->GetSightRadius());
        CThingFilter_IsCreatureType heroType(1);
        CThingFilter_IsNot notMe(GetCreature());
        PeekThingSearchTools()->GetPFirstThing(CHeroFilter(notMe, heroType, inRange), &hero);
        foundHero = hero != 0;
    }
    CTCCreatureModeManager* modes = 0;
    GetCreature()->GetInterfaceWord(&modes, 0x31);
    bool modeAdded = modes->AddMode(0x16);
    if (!TurnToFace())
        return false;
    bool speakFailed;
    {
        CCharString text(foundHero ? "TEXT_AI_OPINION_ACT_HERO_IMITATION_PLAY_ABOUT_HERO"
                                   : "TEXT_AI_OPINION_ACT_HERO_IMITATION_PLAY_TO_SELF");
        speakFailed = WaitForTextToLoadThenSpeak(text, 1) != 2;
    }
    if (speakFailed)
        return false;
    long framesPerSecond = g_FramesPerSecond;
    float delay = (GFFloatRandom(3.0f, GetPWorldSeed()) + 2.0f) * framesPerSecond;
    long endFrame = (long)(GFGetCurrentFrame() + delay);
    bool arrived;
    long result;
    do {
        result = AIAction_MoveToThingProximity(2, ThingReactingTo, 1.0f, &arrived, 3, true, false, true, true, 0.0f);
    } while (result == 4 && GFGetCurrentFrame() < endFrame);
    if (modeAdded) {
        GetCreature()->GetInterface(&modes, 0x31);
        modes->RemoveMode(0x16);
    }
    return result == 2 || result == 4;
}
