// Behaviour test for 00919a70: src/00919a70.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CAIStateGroup_BuyFromSeller::GetSellersReply(CThingCreatureBase* seller)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_BuyFromSeller::GetSellersReply(CThingCreatureBase* seller) -- waits for a type-0x10 message from the seller addressed to this creature; returns it when the wait action reports 2, else null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00919A70, length 105 bytes (+ int3 padding), `ret 4`.
// Callees: two thing-filter constructors @ 0x006AF290 / 0x006AF2B0 (thiscall, CThing* argument, copy the
// thing's +0x18/+0x1c handle pair and return `this`; the manifest labels both CMessageEventFilter_IsOriginatedByThing),
// the templated message-wait action @ 0x0091A670 (thiscall: repeat 2, the bound filter by const reference, the
// CMessageEvent** result; the manifest's "CCOE_AltarEvil::Main" label is propagated).
// Shape: a CFilter_Bind3 of three pointers at frame +0x14 built directly from the IsOfType(0x10),
// IsOriginatedByThing(seller) and IsForThing(creature) temporaries at +0, +4, +0xc (creature a named local, loaded
// and pushed before the first constructor; each constructor's returned `this` is the stored pointer). The wait
// action is a same-TU out-of-line stand-in that writes the out pointer (so the reply is homed in the dead `seller`
// argument slot, `lea eax,[esp+2Ch]`) and reads the bound filter's first word (the landed 0x00922600 B-17 idiom:
// &IsOfType is then stored right after the first filter constructor, as in retail); `status == 2 ? reply : 0`
// lowers to `sub eax,2 ; neg ; sbb ; not ; and`.
// Model: local (engine/CAIStateGroup_BuyFromSeller.h flattens the base).
class CThing;
class CThingCreatureBase;
class CMessageEvent;

class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_IsForThing {
    long m_handle[2];
    CMessageEventFilter_IsForThing(CThing* thing);              // 0x006AF290
};

class CMessageEventFilter_IsOriginatedByThing { public:
    long m_handle[2];
    CMessageEventFilter_IsOriginatedByThing(CThing* thing);     // 0x006AF2B0
};

template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
    CFilter_Bind3(const A& a, const B& b, const C& c) : m_pFirst(&a), m_pSecond(&b), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing, CMessageEventFilter_IsForThing> CReplyFilter;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
};

enum EAIActionRepeat { AI_ACTION_REPEAT_0, AI_ACTION_REPEAT_1, AI_ACTION_REPEAT_2 };

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    long AIAction_WaitForMessage(EAIActionRepeat repeat, const CReplyFilter& filter, CMessageEvent** ppMessage);   // 0x0091A670
};

struct CAIStateGroup_BuyFromSeller : CAIStateGroupBase {
    long FrameLastBought;                       // +0x14
    CMessageEvent* GetSellersReply(CThingCreatureBase* seller);
};

long TmplSink_00919a70(const void* self, long seen);   // stand-in: the message search AIAction_WaitForMessage does
#pragma auto_inline(off)
__declspec(noinline) long CAIStateGroupBase::AIAction_WaitForMessage(EAIActionRepeat repeat, const CReplyFilter& filter, CMessageEvent** ppMessage)
{
    long result;
    do {
        *ppMessage = (CMessageEvent*)TmplSink_00919a70(this, *(const long*)&filter);
        if (*ppMessage) {
            result = 2;
            break;
        }
        result = 4;
    } while (repeat == AI_ACTION_REPEAT_1);
    return result;
}
#pragma auto_inline(on)
CMessageEvent* CAIStateGroup_BuyFromSeller::GetSellersReply(CThingCreatureBase* seller)
{
    CThing* creature = PBrain->Creature;
    CMessageEvent* reply;
    long status = AIAction_WaitForMessage(AI_ACTION_REPEAT_2,
        CReplyFilter(CMessageEventFilter_IsOfType(0x10), CMessageEventFilter_IsOriginatedByThing((CThing*)seller),
            CMessageEventFilter_IsForThing(creature)),
        &reply);
    if (status == 2)
        return reply;
    return 0;
}

// ---- stubs and driver ----
class CThing { public: long h[2]; };
class CThingCreatureBase { public: long h[2]; };
static CThing* g_for; static CThing* g_orig;
CMessageEventFilter_IsForThing::CMessageEventFilter_IsForThing(CThing* thing) { g_for = thing; m_handle[0] = thing->h[0]; m_handle[1] = thing->h[1]; }
CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing(CThing* thing) { g_orig = thing; m_handle[0] = thing->h[0]; m_handle[1] = thing->h[1]; }
CAIStateGroupBase::~CAIStateGroupBase() { }
static CMessageEvent* g_found = 0; static long g_type = -1; static int g_calls = 0; static const void* g_self;
long TmplSink_00919a70(const void* self, long seen)
{
    ++g_calls; g_self = self;
    g_type = ((const CMessageEventFilter_IsOfType*)seen)->m_type;
    return (long)g_found;
}
int main()
{
    static long storage[0x10];
    CAIStateGroup_BuyFromSeller* group = (CAIStateGroup_BuyFromSeller*)storage;
    static CAIBrain brain; static CThing creature; static CThingCreatureBase seller;
    brain.Creature = &creature; group->PBrain = &brain;
    g_found = (CMessageEvent*)0x4321;
    CHECK(group->GetSellersReply(&seller) == (CMessageEvent*)0x4321);
    CHECK(g_calls == 1 && g_self == group && g_type == 0x10 && g_for == &creature && g_orig == (CThing*)&seller);
    g_found = 0;
    CHECK(group->GetSellersReply(&seller) == 0 && g_calls == 2);   // repeat 2: one search, status 4
    if (fails == 0) printf("00919a70_TEST PASS\n");
    return fails;
}
