// Behaviour test for 0x00982210 CCreatureAction_SendMessage ctor: runs the reconstructed constructor over stubbed base,
// lookup and intelligent-pointer callees; checks the base arguments, the add-ref'd def, the copied record, the recipient.
#include <cstdio>
#include <cstring>
class CThingCreatureBase { public: int id; };
class CThing { public: int id; };
class CAnimationEntry { public: int id; };
static int g_strings = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* text, long = -1) : m_pData(text) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static CAnimationEntry g_idle;
static char g_lookup[32];
CAnimationEntry* GetAnimationFromName(CThingCreatureBase*, const CCharString& name) { std::strcpy(g_lookup, name.m_pData); return &g_idle; }
class CCreatureActionBase { public:
    CThingCreatureBase* o; float a, b; CAnimationEntry* anim; CSoundPair s; long c, d;
    virtual void v00() { }
    CCreatureActionBase(CThingCreatureBase* owner, float a_, float b_, CAnimationEntry* an, const CSoundPair& snd, long c_, long d_)
        : o(owner), a(a_), b(b_), anim(an), s(snd), c(c_), d(d_) { }
};
struct CDefBase { void* v; long m_refCount; };
struct CDefPointer {
    CDefBase* Object;
    CDefPointer(CDefBase* p) : Object(p) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
};
struct SMessageData {
    long m_type, m_value1, m_value2;
    SMessageData(long t, long a, long b) : m_type(t), m_value1(a), m_value2(b) { }
    SMessageData(const SMessageData& o) : m_type(o.m_type), m_value1(o.m_value1), m_value2(o.m_value2) { }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    virtual void v00() { }
    CBaseIntelligentPointer() : m_pItem(0) { }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
class CCreatureAction_SendMessage : public CCreatureActionBase { public:
    CDefPointer m_def; SMessageData m_message; CIntelligentPointer m_recipient; long m_flags;
    CCreatureAction_SendMessage(CThingCreatureBase* owner, const CDefPointer& def, CThing* recipient, const SMessageData& message, long flags);
};
CCreatureAction_SendMessage::CCreatureAction_SendMessage(CThingCreatureBase* owner, const CDefPointer& def, CThing* recipient, const SMessageData& message, long flags)
    : CCreatureActionBase(owner, -1.0f, 0.0f, GetAnimationFromName(owner, "STANDARD_IDLE"), CSoundPair(), 0, 0)
    , m_def(def)
    , m_message(message)
    , m_recipient(recipient)
    , m_flags(flags)
{
}
int main()
{
    CThingCreatureBase owner; CThing who; CDefBase d; d.m_refCount = 3;
    CDefPointer dp(&d); SMessageData m(7, 8, 9);
    CCreatureAction_SendMessage a(&owner, dp, &who, m, 42);
    CDefPointer np((CDefBase*)0); CCreatureAction_SendMessage b(&owner, np, 0, m, 0);
    bool ok = a.o == &owner && a.a == -1.0f && a.b == 0.0f && a.anim == &g_idle && a.s.FirstSound == 0 && a.c == 0 && a.d == 0 &&
              std::strcmp(g_lookup, "STANDARD_IDLE") == 0 && g_strings == 0 && a.m_def.Object == &d && d.m_refCount == 4 &&
              a.m_message.m_type == 7 && a.m_message.m_value1 == 8 && a.m_message.m_value2 == 9 && a.m_recipient.m_pItem == &who &&
              a.m_flags == 42 && b.m_def.Object == 0 && b.m_recipient.m_pItem == 0;
    if (!ok) { std::printf("00982210_TEST FAIL\n"); return 1; }
    std::printf("00982210_TEST PASS\n");
    return 0;
}