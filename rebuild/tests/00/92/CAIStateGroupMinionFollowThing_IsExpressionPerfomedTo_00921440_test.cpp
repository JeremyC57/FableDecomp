// Behaviour test for CAIStateGroup_MinionFollowThing::IsExpressionPerfomedTo (0x00921440): model copy with stub callees.
#include <cstdio>
#include <cstring>

class CThing { public: int id; };
class CCharString { public:
    const char* text;                          // null = empty string
    bool operator==(const char* t) const
    {
        if (text == 0) {
            if (std::strcmp(t, "") == 0)
                return true;
            return false;
        }
        return std::strcmp(text, t) == 0;
    }
};
class CDefString { public:
    const char* name;
    CDefString(const char* n) : name(n) { }
    CDefString(const CDefString& o) : name(o.name) { }
    bool operator==(const CCharString& s) const { return std::strcmp(name, s.text ? s.text : "") == 0; }
};
static int g_defReleased;
class CExpressionDef { public:
    const char* name;
    long RefCount;
    CDefString GetName() const { return CDefString(name); }
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) ++g_defReleased; }
};
struct CDefPointerExpression {
    CExpressionDef* m_p;
    CDefPointerExpression(CExpressionDef* p) : m_p(p) { }
    CDefPointerExpression(const CDefPointerExpression& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    ~CDefPointerExpression() { m_p->Release(); }
    CExpressionDef* operator->() const { return m_p; }
};
struct CIntelligentPointer { CThing* item; CThing* GetPItem() const { return item; } };
struct CEventExpressionPerformedTo {
    CIntelligentPointer PerformedBy;
    CDefPointerExpression PExpressionDef;
    long ExtraData1, ExtraData2;
    CEventExpressionPerformedTo(CExpressionDef* d) : PExpressionDef(d) { }
    CDefPointerExpression GetPExpressionDef() const { return PExpressionDef; }
};
class CIsEventOfType { public: long Type; CIsEventOfType(long t) : Type(t) { } };
class CTCEntityEvents { public:
    CEventExpressionPerformedTo* pending;
    long lastType; int removed;
    bool GetEvent(CEventExpressionPerformedTo** out, const CIsEventOfType* f) { lastType = f->Type; *out = pending; return pending != 0; }
    void RemoveEvent(void* e) { if (e == pending) ++removed; }
};
class CTCAIScratchpad { public: bool AllowedToFollow; };
struct CCreature {
    CTCEntityEvents* events; CTCAIScratchpad* scratchpad;
    bool GetInterface(CTCEntityEvents** out, long) { if (!events) return false; *out = events; return true; }
    bool GetInterface(CTCAIScratchpad** out, long) const { if (!scratchpad) return false; *out = scratchpad; return true; }
};
class CAIBrain { public: CCreature* PCreature; };

struct CAIStateGroup_MinionFollowThing {
    CAIBrain* PBrain;
    CThing* PFollowee;
    float FollowDistance;
    bool AvoidDynamicObstacles;
    bool IsExpressionPerfomedTo(const CCharString& expression);
};

bool CAIStateGroup_MinionFollowThing::IsExpressionPerfomedTo(const CCharString& expression)
{
    CTCEntityEvents* events;
    if (PBrain->PCreature->GetInterface(&events, 0x42)) {
        CEventExpressionPerformedTo* event = 0;
        CIsEventOfType filter(3);
        if (events->GetEvent(&event, &filter)) {
            if (event->GetPExpressionDef()->GetName() == expression) {
                const CCreature* creature = PBrain->PCreature;
                CTCAIScratchpad* scratchpad;
                if (!creature->GetInterface(&scratchpad, 0xaa) || scratchpad->AllowedToFollow) {
                    if (expression == "EXPRESSION_FOLLOW")
                        PFollowee = event->PerformedBy.GetPItem();
                    if (event->ExtraData1 > -1)
                        FollowDistance = (float)event->ExtraData1;
                    if (event->ExtraData2 > -1)
                        AvoidDynamicObstacles = event->ExtraData2 != 0;
                    events->RemoveEvent(event);
                    return true;
                }
                events->RemoveEvent(event);
            }
        }
    }
    return false;
}

int main()
{
    int ok = 1;
    CThing performer = { 7 };
    CExpressionDef def = { "EXPRESSION_FOLLOW", 1 };
    CEventExpressionPerformedTo ev(&def); ev.PerformedBy.item = &performer; ev.ExtraData1 = 12; ev.ExtraData2 = 0;
    CTCEntityEvents events = { &ev, 0, 0 };
    CTCAIScratchpad pad = { true };
    CCreature creature = { &events, &pad };
    CAIBrain brain = { &creature };
    CAIStateGroup_MinionFollowThing s = { &brain, 0, 3.0f, true };
    CCharString follow = { "EXPRESSION_FOLLOW" };
    CCharString other = { "EXPRESSION_WAVE" };

    if (!s.IsExpressionPerfomedTo(follow)) ok = 0;
    if (s.PFollowee != &performer || s.FollowDistance != 12.0f || s.AvoidDynamicObstacles || events.removed != 1 || events.lastType != 3) ok = 0;
    if (def.RefCount != 1) ok = 0;                                     // the getter's add-ref copy was released
    s.PFollowee = 0; ev.ExtraData1 = -1; ev.ExtraData2 = -1; s.FollowDistance = 3.0f;
    if (s.IsExpressionPerfomedTo(other)) ok = 0;                       // name mismatch: not consumed
    if (events.removed != 1) ok = 0;
    def.name = "EXPRESSION_WAVE";
    if (!s.IsExpressionPerfomedTo(other) || s.PFollowee != 0 || s.FollowDistance != 3.0f || events.removed != 2) ok = 0;
    pad.AllowedToFollow = false;
    if (s.IsExpressionPerfomedTo(other) || events.removed != 3) ok = 0;   // consumed but refused
    creature.scratchpad = 0;
    if (!s.IsExpressionPerfomedTo(other)) ok = 0;                      // no scratchpad: allowed
    events.pending = 0;
    if (s.IsExpressionPerfomedTo(other)) ok = 0;
    creature.events = 0;
    if (s.IsExpressionPerfomedTo(other)) ok = 0;
    if (!ok) { std::printf("00921440_TEST FAIL\n"); return 1; }
    std::printf("00921440_TEST PASS\n");
    return 0;
}