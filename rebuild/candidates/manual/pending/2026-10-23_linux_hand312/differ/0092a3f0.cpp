// CAIStateGroup_ApprenticeHero::PracticeAgainstApprentice() -- find the MeleePractice dummy, walk near it, then pair up with another apprentice through two shared slots (the first to arrive waits up to the practice time for a partner, the second joins), the joiner picks one of two practice-fight interactions and 10..19 rounds; both fight with an iron longsword, swapping the submissive role half-way, until a round fails or the partner sends the "stop" message (type 0x2f); a failed fight tells the partner to stop
// Grade: DIFFER(1209v1188) -- first pass: the pairing loop's 'first to arrive' block is sunk below the return (retail keeps it after the wait branch and sinks the join branch), Random()%10+10 folds differently (add edx,ecx vs lea), operator= chain register (edi vs ebx); filters hold the thing's 64-bit id (+0x18), hence the 8-aligned ebp frame
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092A3F0, 1188 bytes, plain ret, 8-aligned ebp frame.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / copy ctor 0x0099EC30 / operator=(const char*)
//   0x0099EFE0 / operator=(const CCharString&) 0x0099EFB0, the dummy finder 0x00929FC0, CThing::GetPosition 0x004C73D0,
//   GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0 (__fastcall), GFROR13 0x00497890, AIAction_MoveToPosProximity 0x0097E890,
//   GFGetCurrentFrame 0x0049D870, IsStopping 0x008FD010, AIAction_DoNothing 0x0097E200, Terminate 0x008FCF40, the world
//   accessor 0x008FCFC0 (message manager +0x60), CMessageEventManager::AddMessage 0x00687540, the carried-object giver
//   0x008FD0A0 / DestroyItem 0x008FD1B0, AIAction_InteractWith 0x00981410, the two message-origin filters 0x006AF290 /
//   0x006AF2B0, FindMostRecentMessage<Bind3> 0x0091A5B0, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90,
//   atexit 0x004012BC (function-static interaction name). Globals: g_FramesPerSecond 0x01375550, the practice wait
//   seconds 0x0138CBAC, the waiting/joining slots 0x013BB69C / 0x013BB698, the shared round count 0x013BB6A0.
#include "engine/CAIStateGroup_ApprenticeHero.h"

struct CThing;
struct CThingCreatureBase;
struct CCharString {
    void* PStringData;
    CCharString();                                       // 0x0099EB?? (unused)
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    CCharString& operator=(const char* text);            // 0x0099EFE0
    CCharString& operator=(const CCharString& other);    // 0x0099EFB0
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CThingGameObject {
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {    // vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
};
struct CMessageEvent;
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
struct CMessageEventFilter_IsOriginatedByThing {
    __int64 m_thingID;                          // the thing's 64-bit id (+0x18)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF290
};
struct CMessageEventFilter_IsForThing {
    __int64 m_thingID;
    CMessageEventFilter_IsForThing(const CThing* thing);             // 0x006AF2B0
};
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B, class C> struct CFilter_Bind3 {
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsForThing, CMessageEventFilter_IsOriginatedByThing> CStopFilter;
struct CMessageEventManager {
    CMessageEvent* AddMessage(long type, unsigned long delay, const C3DVector* position, CThing* from, CThing* to,
                              float a, float b, long data);     // 0x00687540
    CMessageEvent* FindMostRecentMessage(const CStopFilter& filter);    // 0x0091A5B0
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageManager;      // +0x60
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
float __fastcall GFFloatRandom(unsigned long* seed, float range);   // 0x004978A0
long GFGetCurrentFrame();                       // 0x0049D870
extern long g_FramesPerSecond;                  // 0x01375550
extern long g_ApprenticePracticeWaitSeconds;    // 0x0138CBAC
static CThing* s_pWaitingApprentice;            // 0x013BB69C
static CThing* s_pJoiningApprentice;            // 0x013BB698
static long s_NumFights;                        // 0x013BB6A0

struct CAIStateGroupApprenticeHeroMethods : CAIStateGroup_ApprenticeHero {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CThing* Opponent() const { return (CThing*)PracticeOpponent; }
    CCharString& Interaction() { return *(CCharString*)&Interaction_PStringData; }
    CThingGameObject* FindDummy(const CCharString& name);   // 0x00929FC0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float distance, bool* reached, long movement,
        bool a, bool b, bool c, CThing* avoid, bool d, float speed);        // 0x0097E890
    long AIAction_InteractWith(long repeat, CIntelligentPointerThing target, CCharString interaction, long frames,
        bool submissive);                       // 0x00981410
    void GiveCarriedObject(const CCharString& object, const CCharString& slot);   // 0x008FD0A0
    void DestroyItem(const CCharString& slot);  // 0x008FD1B0
    __forceinline unsigned long Random()
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        return *seed = GFROR13(*seed);
    }
    bool PracticeAgainstApprentice();
};

bool CAIStateGroupApprenticeHeroMethods::PracticeAgainstApprentice()
{
    CThingGameObject* dummy;
    {
        CCharString name("MeleePractice");
        dummy = FindDummy(name);
    }
    if (!dummy)
        return true;
    bool reached;
    if (AIAction_MoveToPosProximity(1, dummy->GetPosition(), GFFloatRandom(GetPWorldSeed(), 15.0f) + 5.0f, &reached, 3,
            true, false, true, 0, true, 0.0f) != 2)
        return false;
    long fps = g_FramesPerSecond;
    long endFrame = GFGetCurrentFrame() + g_ApprenticePracticeWaitSeconds * fps;
    for (;;) {
        if (IsStopping())
            return false;
        if (s_pWaitingApprentice) {
            if (!s_pJoiningApprentice) {
                s_pJoiningApprentice = GetCreature();
                PracticeOpponent = (CThingAICreature*)s_pWaitingApprentice;
                break;
            }
            AIAction_DoNothing();
            if (endFrame < GFGetCurrentFrame())
                Terminate();
            if (IsStopping())
                break;
        } else {
            s_pWaitingApprentice = GetCreature();
            for (;;) {
                if (s_pJoiningApprentice) {
                    PracticeOpponent = (CThingAICreature*)s_pJoiningApprentice;
                    s_pJoiningApprentice = 0;
                    s_pWaitingApprentice = 0;
                    break;
                }
                AIAction_DoNothing();
                if (endFrame < GFGetCurrentFrame())
                    Terminate();
                if (IsStopping())
                    break;
            }
            if (IsStopping()) {
                if (s_pJoiningApprentice) {
                    CThing* creature = GetCreature();
                    C3DVector origin(0.0f, 0.0f, 0.0f);
                    CWorld* world = GetPWorld();
                    world->PMessageManager->AddMessage(0x2f, 1000, &origin, creature, Opponent(), 0, 0, 0);
                    s_pJoiningApprentice = 0;
                }
                s_pWaitingApprentice = 0;
                return false;
            }
            break;
        }
    }
    if (IsStopping())
        return false;
    static CCharString s_Interaction("");
    if (s_pWaitingApprentice && s_pJoiningApprentice) {
        long which = Random() & 1;
        Interaction() = s_Interaction = which ? "VILLAGER_INTERACTION_HERO_APPRENTICE_PRACTICE_FIGHT_02"
                                              : "VILLAGER_INTERACTION_HERO_APPRENTICE_PRACTICE_FIGHT_01";
        Submissive = true;
        NumFights = s_NumFights = Random() % 10 + 10;
    } else {
        Interaction() = s_Interaction;
        Submissive = false;
        NumFights = s_NumFights;
    }
    long result = 3;
    Critical = true;
    GiveCarriedObject("OBJECT_IRON_LONGSWORD", "CARRY_SLOT_RIGHT_HAND");
    CMessageEvent* stop = 0;
    for (long i = 0; i < NumFights; ++i) {
        if (i == NumFights / 2)
            Submissive = !Submissive;
        for (;;) {
            result = AIAction_InteractWith(2, CIntelligentPointerThing(Opponent()), Interaction(), 0x1e, Submissive);
            stop = GetPWorld()->PMessageManager->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x2f)
                && CMessageEventFilter_IsForThing(Opponent()) && CMessageEventFilter_IsOriginatedByThing(GetCreature()));
            if (result == 4 && !stop && !IsStopping())
                continue;
            break;
        }
        if (result != 2 || stop)
            break;
        AIAction_DoNothing();
    }
    DestroyItem("CARRY_SLOT_RIGHT_HAND");
    Critical = false;
    if (!stop && (result != 2 || IsStopping())) {
        CThing* creature = GetCreature();
        C3DVector origin(0.0f, 0.0f, 0.0f);
        CWorld* world = GetPWorld();
        world->PMessageManager->AddMessage(0x2f, 1000, &origin, creature, Opponent(), 0, 0, 0);
        AIAction_DoNothing();
        return false;
    }
    AIAction_DoNothing();
    return true;
}
