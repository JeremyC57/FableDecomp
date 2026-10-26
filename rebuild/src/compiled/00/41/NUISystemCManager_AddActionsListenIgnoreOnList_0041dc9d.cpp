// NUISystem::CManager::AddActionsListenIgnoreOnList(CComponent* list, CComponent* first, CComponent* second, EEvent event)
// Leaf: AddActionsListenIgnoreOnList
// Retail: Fable.exe @ 0x0041DC9D, 565 bytes, `ret 10h`; /Os + optimize("y", off) TU (ebp frame closed with `leave`, as the
//   landed CreateLevelSlider 0x00427DC9). `this` is unused.
// Callees: operator new 0x00BFEA1A, CAction::CAction 0x0042BE50, CCountedPointer<CAction>(CAction*) 0x0042AA29 / its dtor
//   0x00429160, CAction::SetTarget 0x0053FDE0 (descriptive, as the landed CScrollableList::Initialise 0x00555EB0),
//   list<CActionParam*>::push_back 0x0042B646 (out of line here); the list's virtual slots 67 (+0x10c) and 70 (+0x118).
// Layout: CAction 0x10 bytes with the param list at +4 and the action-target subobject of a component at +0x18 (landed
//   CScrollableList::Initialise model); the event parameter is a 12-byte CActionParam (vtable 0x0122F844, type 5).
#pragma optimize("s", on)
#pragma optimize("y", off)

namespace NUISystem {

enum EEvent { EVENT_NONE };

class CActionParam { public:
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) {}
    virtual void v00();
};
struct CActionParamEvent : CActionParam {               // vtable 0x0122F844
    EEvent Value;                                       // +0x08
    CActionParamEvent(EEvent value) : CActionParam(5), Value(value) {}
    virtual void v00();
};
struct CActionParamList {                               // STLport list<CActionParam*>
    void* _M_node;
    void push_back(CActionParam* const& value);         // 0x0042B646
};
struct CActionTarget { void* _vf; };
struct CActionPointer;
class CAction { public:                                        // retail 0x10 bytes
    long             ActionType;                        // +0x00
    CActionParamList UIActionParams;                    // +0x04
    void*            ThingActionParams;                 // +0x08
    bool             Valid;                             // +0x0C
    CAction();                                          // 0x0042BE50
    void SetTarget(CActionTarget* target, const CActionPointer& self);   // 0x0053FDE0
    void AddUIActionParam(CActionParam* param) { UIActionParams.push_back(param); }
};
struct SCountBlock;
struct CActionPointer {
    CAction*     m_pObject;
    SCountBlock* m_pCount;
    CActionPointer(CAction* object);                    // 0x0042AA29
    ~CActionPointer();                                  // 0x00429160
    CAction* operator->() const { return m_pObject; }
};

struct CComponentBase {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void AddListenAction(const CActionPointer& action);   // slot 67 (+0x10c) descriptive
    virtual void v068(); virtual void v069();
    virtual void AddIgnoreAction(const CActionPointer& action);   // slot 70 (+0x118) descriptive
    unsigned char _pad04[0x18 - 0x04];
};
class CComponent : public CComponentBase, public CActionTarget { public:     // CActionTarget at +0x18
};

class CManager {
public:
    virtual void AddActionsListenIgnoreOnList(CComponent* list, CComponent* first, CComponent* second, EEvent event);
};

void CManager::AddActionsListenIgnoreOnList(CComponent* list, CComponent* first, CComponent* second, EEvent event)
{
    CActionPointer listenFirst(new CAction);
    listenFirst->ActionType = 0xf5;
    listenFirst->SetTarget(first, listenFirst);
    listenFirst->AddUIActionParam(new CActionParamEvent(event));
    CActionPointer ignoreFirst(new CAction);
    ignoreFirst->ActionType = 0xf9;
    ignoreFirst->SetTarget(first, ignoreFirst);
    ignoreFirst->AddUIActionParam(new CActionParamEvent(event));
    list->AddListenAction(listenFirst);
    list->AddIgnoreAction(ignoreFirst);
    CActionPointer listenSecond(new CAction);
    listenSecond->ActionType = 0xf5;
    listenSecond->SetTarget(second, listenSecond);
    listenSecond->AddUIActionParam(new CActionParamEvent(event));
    CActionPointer ignoreSecond(new CAction);
    ignoreSecond->ActionType = 0xf9;
    ignoreSecond->SetTarget(second, ignoreSecond);
    ignoreSecond->AddUIActionParam(new CActionParamEvent(event));
    list->AddListenAction(ignoreSecond);
    list->AddIgnoreAction(listenSecond);
}

}