#include <cstdio>
#include <cstdlib>

// Host model of NUISystem::CAction::AddParam(CActionParam*, CCountedPointer<CAction>&): a parameter of type 1 (a
// component) is first told it is a parameter of the action (AddActionIAmParamOf), then every parameter is appended to
// UIActionParams (STLport list push_back: a 12-byte node linked before the sentinel).
struct CAction;
struct CActionPointer { CAction* Data; void* Info; };

struct CActionParam {
    virtual ~CActionParam() { }
    long ActionParamType;
};
struct CComponentBase {
    virtual ~CComponentBase() { }
    virtual void AddActionIAmParamOf(CActionPointer& action) { LastAction = action.Data; ++Calls; }
    CAction* LastAction;
    int      Calls;
};
struct CComponent : CComponentBase, CActionParam {
    CComponent() { LastAction = 0; Calls = 0; ActionParamType = 1; }
};
struct CActionParamLong : CActionParam {
    long Value;
    CActionParamLong(long v) : Value(v) { ActionParamType = 3; }
};

struct ParamNode { ParamNode* Next; ParamNode* Prev; CActionParam* Data; };
struct CActionParamList {
    ParamNode* Head;
    CActionParamList() { Head = (ParamNode*)std::malloc(sizeof(ParamNode)); Head->Next = Head->Prev = Head; }
    ~CActionParamList()
    {
        for (ParamNode* p = Head->Next; p != Head;) { ParamNode* n = p->Next; std::free(p); p = n; }
        std::free(Head);
    }
    void push_back(CActionParam* const& x)
    {
        ParamNode* head = Head;
        ParamNode* n = (ParamNode*)std::malloc(sizeof(ParamNode));
        n->Data = x;
        ParamNode* prev = head->Prev;
        n->Next = head; n->Prev = prev; prev->Next = n; head->Prev = n;
    }
};
struct CAction {
    long             ActionType;
    CActionParamList UIActionParams;
    void AddParam(CActionParam* param, CActionPointer& self)
    {
        if (param->ActionParamType == 1)
            static_cast<CComponent&>(*param).AddActionIAmParamOf(self);
        UIActionParams.push_back(param);
    }
};

int main()
{
    CAction action;
    CActionPointer self = { &action, 0 };
    CComponent component;
    CActionParamLong value(42);
    action.AddParam(&value, self);
    action.AddParam(static_cast<CActionParam*>(&component), self);
    action.AddParam(&value, self);
    if (component.Calls != 1 || component.LastAction != &action) { std::printf("0053fde0_TEST FAIL\n"); return 1; }
    ParamNode* p = action.UIActionParams.Head->Next;
    CActionParam* want[3] = { &value, static_cast<CActionParam*>(&component), &value };
    for (int i = 0; i < 3; ++i, p = p->Next)
        if (p == action.UIActionParams.Head || p->Data != want[i]) { std::printf("0053fde0_TEST FAIL\n"); return 1; }
    if (p != action.UIActionParams.Head) { std::printf("0053fde0_TEST FAIL\n"); return 1; }
    std::printf("0053fde0_TEST PASS\n");
    return 0;
}