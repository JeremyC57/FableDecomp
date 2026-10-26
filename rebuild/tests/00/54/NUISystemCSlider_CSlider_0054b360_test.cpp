#include <cstdio>
#include <math.h>
// behaviour test: CSlider::~CSlider re-applies the start value through every action only when the value moved,
// deletes both action lists, then destroys the action range and the sprite base
static const float g_SliderValueEpsilon = 0.001f;
class C2DVector { public:
    float X, Y;
    bool operator==(const C2DVector& v) const
    {
        return fabs(X - v.X) <= g_SliderValueEpsilon && fabs(Y - v.Y) <= g_SliderValueEpsilon;
    }
};
struct CSliderValue { float Value; };
struct CSliderTarget { CSliderValue* Value; };
struct CSliderTargetRef { CSliderTarget* Target; };
struct CAction { CSliderTargetRef* PTarget; };
struct CActionPtr { CAction* Data; CAction* operator->() const { return Data; } };
static int g_executed = 0, g_listsDeleted = 0, g_rangeDestroyed = 0, g_spriteDestroyed = 0;
struct CFrontEndManager {
    static CFrontEndManager* GetInstance() { static CFrontEndManager m; return &m; }
    void ExecuteAction(const CActionPtr&) { ++g_executed; }
};
struct CActionVector {
    CActionPtr* _M_start; CActionPtr* _M_finish;
    unsigned int size() const { return _M_finish - _M_start; }
    CActionPtr& operator[](unsigned int n) const { return *(_M_start + n); }
    ~CActionVector() { ++g_rangeDestroyed; }
};
struct CActionList { ~CActionList() { ++g_listsDeleted; } };
struct CSprite { ~CSprite() { ++g_spriteDestroyed; } };
struct CSlider : CSprite {
    C2DVector Value;
    bool Move;
    C2DVector StartValue;
    CActionVector Actions;
    CActionList* PActionsOnBack;
    CActionList* PActionsOnSelect;
    ~CSlider();
};
CSlider::~CSlider()
{
    if (!(StartValue == Value)) {
        for (unsigned int i = 0; i < Actions.size(); ++i) {
            Actions[i]->PTarget->Target->Value->Value = StartValue.X;
            CFrontEndManager::GetInstance()->ExecuteAction(Actions[i]);
        }
        Move = true;
    }
    delete PActionsOnBack;
    delete PActionsOnSelect;
}
int main()
{
    CSliderValue v1 = { 9 }, v2 = { 9 };
    CSliderTarget t1 = { &v1 }, t2 = { &v2 };
    CSliderTargetRef r1 = { &t1 }, r2 = { &t2 };
    CAction a1 = { &r1 }, a2 = { &r2 };
    CActionPtr ptrs[2] = { { &a1 }, { &a2 } };
    {
        CSlider s; s.Value.X = 3; s.Value.Y = 4; s.StartValue.X = 3.0005f; s.StartValue.Y = 4; s.Move = false;
        s.Actions._M_start = ptrs; s.Actions._M_finish = ptrs + 2;
        s.PActionsOnBack = new CActionList; s.PActionsOnSelect = 0;
    }
    bool ok = g_executed == 0 && v1.Value == 9 && g_listsDeleted == 1 && g_rangeDestroyed == 1 && g_spriteDestroyed == 1;
    {
        CSlider s; s.Value.X = 3; s.Value.Y = 4; s.StartValue.X = 1; s.StartValue.Y = 4; s.Move = false;
        s.Actions._M_start = ptrs; s.Actions._M_finish = ptrs + 2;
        s.PActionsOnBack = new CActionList; s.PActionsOnSelect = new CActionList;
    }
    ok = ok && g_executed == 2 && v1.Value == 1 && v2.Value == 1 && g_listsDeleted == 3 && g_rangeDestroyed == 2
            && g_spriteDestroyed == 2;
    if (!ok) { std::printf("0054b360_TEST FAIL\n"); return 1; }
    std::printf("0054b360_TEST PASS\n");
    return 0;
}