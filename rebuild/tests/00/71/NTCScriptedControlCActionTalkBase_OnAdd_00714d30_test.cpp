// Behaviour test for 0x00714D30 NTCScriptedControl::CActionTalkBase::OnAdd
#include <cstdio>
class CThing { public: bool hasControl; struct CControl* control; };
class CConversation { public: int id; };
struct CActionTalkedTo { CThing* talker; CConversation* conversation; };
struct CControl {
    int added; CActionTalkedTo* last;
    void AddAction(CActionTalkedTo* a) { ++added; delete last; last = a; }
};
struct CTalk {
    CThing* parent; CThing* target; bool MakeTargetListen; CConversation* conversation; int resets;
    void Reset(CConversation* c) { delete conversation; conversation = c; ++resets; }
    void OnAdd()
    {
        if (!MakeTargetListen)
            return;
        CThing* p = parent;
        if (target == p)
            return;
        if (!target->hasControl)
            return;
        CControl* control = target->control;
        Reset(new CConversation);
        CActionTalkedTo* a = new CActionTalkedTo;
        a->talker = parent; a->conversation = conversation;
        control->AddAction(a);
    }
};
int main()
{
    CControl ctl; ctl.added = 0; ctl.last = 0;
    CThing me; me.hasControl = true; me.control = &ctl;
    CThing other; other.hasControl = true; other.control = &ctl;
    CTalk t; t.parent = &me; t.target = &other; t.MakeTargetListen = false; t.conversation = 0; t.resets = 0;
    t.OnAdd();
    int ok = ctl.added == 0 && t.resets == 0;
    t.MakeTargetListen = true; t.target = &me;
    t.OnAdd();
    ok = ok && ctl.added == 0;
    t.target = &other; other.hasControl = false;
    t.OnAdd();
    ok = ok && ctl.added == 0 && t.resets == 0;
    other.hasControl = true;
    t.OnAdd();
    ok = ok && ctl.added == 1 && t.resets == 1 && t.conversation != 0 && ctl.last->talker == &me && ctl.last->conversation == t.conversation;
    delete ctl.last; delete t.conversation;
    if (!ok) { std::printf("00714d30_TEST FAIL\n"); return 1; }
    std::printf("00714d30_TEST PASS\n");
    return 0;
}