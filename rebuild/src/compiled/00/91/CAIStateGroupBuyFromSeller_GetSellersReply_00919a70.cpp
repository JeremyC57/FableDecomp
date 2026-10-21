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
struct CThing;
struct CThingCreatureBase;
struct CMessageEvent;

struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_IsForThing {
    long m_handle[2];
    CMessageEventFilter_IsForThing(CThing* thing);              // 0x006AF290
};

struct CMessageEventFilter_IsOriginatedByThing {
    long m_handle[2];
    CMessageEventFilter_IsOriginatedByThing(CThing* thing);     // 0x006AF2B0
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
    CFilter_Bind3(const A& a, const B& b, const C& c) : m_pFirst(&a), m_pSecond(&b), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing, CMessageEventFilter_IsForThing> CReplyFilter;

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
};

enum EAIActionRepeat { AI_ACTION_REPEAT_0, AI_ACTION_REPEAT_1, AI_ACTION_REPEAT_2 };

struct CAIStateGroupBase {
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