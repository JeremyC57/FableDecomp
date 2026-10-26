// Behaviour test for 008941a0: src/008941a0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CGameScriptInterface::MsgOnTavernGameFinished(const CScriptThing& winner) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGameScriptInterface::MsgOnTavernGameFinished(const CScriptThing& winner) const -- whether a "tavern game finished"
// message (type 0x40) seen or heard this frame names the given (alive) thing as its target
// Leaf: ?MsgOnTavernGameFinished@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008941A0, length 176 bytes, `ret 4`.
// Callees: GetCurrentWorldFrameForMessages @ 0x006E7510, GetMaxWorldFrameForMessages @ 0x006E7530,
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, CanBeSeenOrHeard>> @ 0x008ABED0 (CWorld+0x60),
// CScriptThing virtual slot 11 GetThing (+0x2C), CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Layout: CMessageEvent PTarget +0x48 (CIntelligentPointer<CThing>, PDB +0x50); CThing dead bit +0x91 bit 0.
// Layout: CThing ThingTypeFlags +0x6c (retail, as the landed 0x008D04E0 model).
// Shape: the Msg-family filter frame (landed 0x00893FF0 model); the message target is the landed typed-item test of
// 0x008D04E0 (MsgIsKilledBy): `GetPItem() != 0 && IsThingOfType<CThing>(GetPItem())` with a zero type mask copied to a
// local, which keeps the dead store of the second GetPItem result in the dead `winner` slot and the constant
// `xor cl,cl ; jne`, then the third GetPItem as the target; `return a && b && c;` gives the full-register 0/1 exits.
class CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    CBaseObject* Item;
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CThing { public:
    enum { ThingTypeMask = 0 };
    unsigned char _pad[0x6c];
    unsigned long ThingTypeFlags;                   // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char m_dead : 1;                       // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
template <class T> inline bool IsThingOfType(const CThing* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}

inline bool IS_THING_DEAD(CThing* const& thing) { return thing->m_dead; }
template<class T> inline bool IS_THING_ALIVE(T* const& thing) { return thing != 0 && !IS_THING_DEAD(thing); }

class CMessageEvent { public:
    unsigned char _pad[0x48];
    CIntelligentPointer<CThing> PTarget;            // +0x48
    CThing* GetTarget() const
    {
        CThing* target = 0;
        if (PTarget.GetPItem() != 0 && IsThingOfType<CThing>(PTarget.GetPItem()))
            target = PTarget.GetPItem();
        return target;
    }
};
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
class CMessageEventFilter_CanBeSeenOrHeard { public:
    long m_frame;
    long m_maxFrame;
    CMessageEventFilter_CanBeSeenOrHeard(long frame, long maxFrame) : m_frame(frame), m_maxFrame(maxFrame) { }
};
template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CSeenOrHeardFilter;
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CSeenOrHeardFilter& filter);   // 0x008ABED0
};
class CWorld { public:
    virtual void w000();
    unsigned char _pad04[0x5c];
    CMessageEventManager* PMessageEventManager_Data;   // +0x60
};

class CScriptThing { public:
    virtual void s000() const; virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const; virtual void s006() const; virtual void s007() const; 
    virtual void s008() const; virtual void s009() const; virtual void s010() const; 
    virtual CThing* GetThing() const;              // slot 11 (+0x2c)
};

class CGameScriptInterface { public:
    virtual bool MsgOnTavernGameFinished(const CScriptThing& winner) const;
    CWorld* World;                                  // +0x04
    long GetCurrentWorldFrameForMessages() const;   // 0x006E7510
    long GetMaxWorldFrameForMessages() const;       // 0x006E7530
};

bool CGameScriptInterface::MsgOnTavernGameFinished(const CScriptThing& winner) const
{
    long frame = GetCurrentWorldFrameForMessages();
    long maxFrame = GetMaxWorldFrameForMessages();
    CMessageEventManager* messages = World->PMessageEventManager_Data;
    CMessageEvent* message = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x40) && CMessageEventFilter_CanBeSeenOrHeard(frame, maxFrame));
    return message && IS_THING_ALIVE(winner.GetThing()) && winner.GetThing() == message->GetTarget();
}

// ---- stubs and driver ----
static int g_getCalls = 0;
CBaseObject* CBaseIntelligentPointer::GetPItem() const { ++g_getCalls; return Item; }
long CGameScriptInterface::GetCurrentWorldFrameForMessages() const { return 70; }
long CGameScriptInterface::GetMaxWorldFrameForMessages() const { return 90; }
static CMessageEvent* g_found = 0; static long g_type = 0, g_f = 0, g_mf = 0;
CMessageEvent* CMessageEventManager::FindMostRecentMessage(const CSeenOrHeardFilter& filter)
{ g_type = filter.m_pFirst->m_type; g_f = filter.m_pSecond->m_frame; g_mf = filter.m_pSecond->m_maxFrame; return g_found; }
void CScriptThing::s000() const { } void CScriptThing::s001() const { } void CScriptThing::s002() const { } void CScriptThing::s003() const { }
void CScriptThing::s004() const { } void CScriptThing::s005() const { } void CScriptThing::s006() const { } void CScriptThing::s007() const { }
void CScriptThing::s008() const { } void CScriptThing::s009() const { } void CScriptThing::s010() const { }
static CThing* g_winnerThing = 0;
CThing* CScriptThing::GetThing() const { return g_winnerThing; }
int main()
{
    static double wb[(sizeof(CWorld) + 7) / 8]; CWorld& world = *(CWorld*)wb; memset(wb, 0, sizeof(wb));
    static CMessageEventManager mgr; world.PMessageEventManager_Data = &mgr;
    static double gb[(sizeof(CGameScriptInterface) + 7) / 8]; CGameScriptInterface& gsi = *(CGameScriptInterface*)gb; memset(gb, 0, sizeof(gb));
    gsi.World = &world;
    CScriptThing winner;
    static double t1b[(sizeof(CThing) + 7) / 8]; CThing& t1 = *(CThing*)t1b; memset(t1b, 0, sizeof(t1b));
    static double t2b[(sizeof(CThing) + 7) / 8]; CThing& t2 = *(CThing*)t2b; memset(t2b, 0, sizeof(t2b));
    static double mb[(sizeof(CMessageEvent) + 7) / 8]; CMessageEvent& msg = *(CMessageEvent*)mb; memset(mb, 0, sizeof(mb));
    // no message
    g_winnerThing = &t1;
    CHECK(gsi.CGameScriptInterface::MsgOnTavernGameFinished(winner) == false);
    CHECK(g_type == 0x40 && g_f == 70 && g_mf == 90);
    // message targets the winner
    g_found = &msg; msg.PTarget.Item = (CBaseObject*)&t1;
    CHECK(gsi.CGameScriptInterface::MsgOnTavernGameFinished(winner) == true);
    // message targets someone else
    msg.PTarget.Item = (CBaseObject*)&t2;
    CHECK(gsi.CGameScriptInterface::MsgOnTavernGameFinished(winner) == false);
    // dead winner
    msg.PTarget.Item = (CBaseObject*)&t1; t1.m_dead = 1;
    CHECK(gsi.CGameScriptInterface::MsgOnTavernGameFinished(winner) == false);
    t1.m_dead = 0;
    // no winner thing
    g_winnerThing = 0;
    CHECK(gsi.CGameScriptInterface::MsgOnTavernGameFinished(winner) == false);
    // message with no target never matches a live winner
    g_winnerThing = &t1; msg.PTarget.Item = 0; g_getCalls = 0;
    CHECK(gsi.CGameScriptInterface::MsgOnTavernGameFinished(winner) == false);
    CHECK(g_getCalls == 1);
    msg.PTarget.Item = (CBaseObject*)&t1; g_getCalls = 0;
    CHECK(gsi.CGameScriptInterface::MsgOnTavernGameFinished(winner) == true);
    CHECK(g_getCalls == 3);
    if (fails == 0) printf("008941a0_TEST PASS\n"); else printf("008941a0_TEST FAIL\n");
    return fails;
}
