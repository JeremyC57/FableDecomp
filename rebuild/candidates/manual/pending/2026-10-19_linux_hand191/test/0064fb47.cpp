// Behaviour test for NPlayerGui::CDrawQuestInfo::AddTick (0x0064FB47): primary then secondary binding, then the name;
// -1 when unbound or unnamed, else the new tick element's ID.
#include <cstdio>
struct C2DVector { float X, Y; };
enum EGameAction { GAME_ACTION_NONE = 0, GAME_ACTION_JUMP = 3, GAME_ACTION_ROLL = 4, GAME_ACTION_SECRET = 5 };
struct CActionInputControl {
    long GameAction, ControllerType, KeyboardKey, XboxButton, MouseButton; C2DVector ControlDirection;
    CActionInputControl() : GameAction(0), ControllerType(0), KeyboardKey(0), XboxButton(0), MouseButton(0) { ControlDirection.X = 0.0f; ControlDirection.Y = 0.0f; }
};
struct CWideString { int length; CWideString() : length(0) {} ~CWideString() {} };
static int g_lookups = 0;
struct CUserProfileManager {
    bool GetAssignedInputForAction(EGameAction action, bool primary, CActionInputControl* c)
    {
        ++g_lookups;
        if (action == GAME_ACTION_JUMP && primary) { c->KeyboardKey = 32; return true; }
        if ((action == GAME_ACTION_ROLL || action == GAME_ACTION_SECRET) && !primary) { c->KeyboardKey = action == GAME_ACTION_ROLL ? 17 : 0; return true; }
        return false;
    }
};
static CUserProfileManager g_profiles;
template <class T> struct CTBaseSingleton { static T& Get() { return g_profiles; } };
static bool GetActionName(const CActionInputControl& c, CWideString& name, bool) { name.length = (int)c.KeyboardKey; return c.KeyboardKey != 0; }
static long g_nextID = 10;
struct CQuestInfoElement { long ID; CQuestInfoElement() : ID(g_nextID++) {} };
struct CQuestInfoElementTick : CQuestInfoElement { int nameLength; bool ticked; float scale;
    CQuestInfoElementTick(const CWideString& n, bool t, float s) : nameLength(n.length), ticked(t), scale(s) {} };
template <class T> struct CCountedPointer {
    T* m_pObject;
    explicit CCountedPointer(T* o) : m_pObject(o) {}
    template <class U> CCountedPointer(const CCountedPointer<U>& o) : m_pObject(o.m_pObject) {}
    ~CCountedPointer() {}
    T* operator->() const { return m_pObject; }
};
static CQuestInfoElementTick* g_last = 0;
struct CDrawQuestInfo {
    int added;
    void AddElement(CCountedPointer<CQuestInfoElement> e) { ++added; g_last = static_cast<CQuestInfoElementTick*>(e.m_pObject); }
    long AddTick(EGameAction action, bool ticked, float scale);
};
long CDrawQuestInfo::AddTick(EGameAction action, bool ticked, float scale)
{
    CActionInputControl control;
    if (CTBaseSingleton<CUserProfileManager>::Get().GetAssignedInputForAction(action, true, &control)
        || CTBaseSingleton<CUserProfileManager>::Get().GetAssignedInputForAction(action, false, &control)) {
        CWideString name;
        if (GetActionName(control, name, true)) {
            CCountedPointer<CQuestInfoElementTick> element(new CQuestInfoElementTick(name, ticked, scale));
            AddElement(element);
            return element->ID;
        }
    }
    return -1;
}
int main()
{
    CDrawQuestInfo q; q.added = 0; int bad = 0;
    if (q.AddTick(GAME_ACTION_JUMP, true, 1.0f) != 10 || g_lookups != 1 || g_last->nameLength != 32 || !g_last->ticked) bad = 1;
    if (q.AddTick(GAME_ACTION_ROLL, false, 1.0f) != 11 || g_lookups != 3 || g_last->nameLength != 17) bad = 2;
    if (q.AddTick(GAME_ACTION_NONE, true, 1.0f) != -1 || g_lookups != 5) bad = 3;
    if (q.AddTick(GAME_ACTION_SECRET, true, 1.0f) != -1 || q.added != 2) bad = 4;   // bound but no name
    if (bad) { std::printf("0064fb47_TEST FAIL\n"); return 1; }
    std::printf("0064fb47_TEST PASS\n");
    return 0;
}
