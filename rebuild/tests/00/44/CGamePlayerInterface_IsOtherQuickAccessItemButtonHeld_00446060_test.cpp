#include <cstdio>
enum EXboxControllerButton { XBOX_CONTROLLER_BUTTON_NONE };
enum EMouseButtonControl { MOUSE_BUTTON_CONTROL_NONE };
enum EInputButtonEventType { INPUT_BUTTON_EVENT_TYPE_HELD = 2 };   // value as pushed (name not confirmed)
class CInputEvent;
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual void _slot01() const;                        // GetType
    virtual void _slot02() const;                        // GetControlDirection
    virtual bool IsEventOfThisType(const CInputEvent& event) const;                    // slot 3 (+0xc)
    virtual void _slot04() const;
    virtual bool IsEventOfThisTypeInQueue(EInputButtonEventType type, long player, float& value) const;   // slot 5 (+0x14)
    virtual bool IsInputTypeWithButton(EXboxControllerButton button) const;            // slot 6 (+0x18)
    virtual bool IsInputTypeWithMouseButton(EMouseButtonControl button) const;         // slot 7 (+0x1c)
};
template <class T> class CCountedPointer {
public:
    T*    Data;                                          // +0x00
    void* Info;                                          // +0x04
    T* operator->() const { return Data; }
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};
}
class CGamePlayerInterface {
public:
    unsigned char _pad0000[0x7b8];
    std::vector<CCountedPointer<const CInputTypeBase> > AssignableSpecialMoveButtons;   // +0x7b8
    std::vector<CCountedPointer<const CInputTypeBase> > QuickAccessItemButtons;         // +0x7c4
    bool IsOtherQuickAccessItemButtonHeld(long index, long player) const;
};
bool CGamePlayerInterface::IsOtherQuickAccessItemButtonHeld(long index, long player) const
{
    for (unsigned long i = 0; i < QuickAccessItemButtons.size(); ++i)
    {
        if (i != index)
        {
            const CInputTypeBase* button = QuickAccessItemButtons[i].Data;
            float value = 0.0f;
            return button->IsEventOfThisTypeInQueue(INPUT_BUTTON_EVENT_TYPE_HELD, player, value);
        }
    }
    return false;
}



class CInputEvent { public: int Code; };
CInputTypeBase::~CInputTypeBase() {}
void CInputTypeBase::_slot01() const {}
void CInputTypeBase::_slot02() const {}
bool CInputTypeBase::IsEventOfThisType(const CInputEvent&) const { return false; }
void CInputTypeBase::_slot04() const {}
bool CInputTypeBase::IsEventOfThisTypeInQueue(EInputButtonEventType, long, float&) const { return false; }
bool CInputTypeBase::IsInputTypeWithButton(EXboxControllerButton) const { return false; }
bool CInputTypeBase::IsInputTypeWithMouseButton(EMouseButtonControl) const { return false; }
static int g_lastType, g_asked; static long g_lastPlayer; static float g_lastValue;
class CTestInput : public CInputTypeBase {
public:
    int Code;
    bool IsEventOfThisType(const CInputEvent& e) const { return e.Code == Code; }
    bool IsEventOfThisTypeInQueue(EInputButtonEventType type, long player, float& value) const
    { g_lastType = type; g_lastPlayer = player; g_lastValue = value; g_asked = Code; value = 5.0f; return Code == 2; }
    bool IsInputTypeWithButton(EXboxControllerButton b) const { return (int)b == Code; }
    bool IsInputTypeWithMouseButton(EMouseButtonControl b) const { return (int)b == Code; }
};
int main()
{
    static unsigned char raw[sizeof(CGamePlayerInterface)];
    CGamePlayerInterface* g = (CGamePlayerInterface*)raw;
    CTestInput k[3]; k[0].Code = 1; k[1].Code = 2; k[2].Code = 2;
    CCountedPointer<const CInputTypeBase> p[3];
    for (int i = 0; i < 3; ++i) { p[i].Data = &k[i]; p[i].Info = 0; }
    g->AssignableSpecialMoveButtons._M_start = 0; g->AssignableSpecialMoveButtons._M_finish = 0;
    g->QuickAccessItemButtons._M_start = p; g->QuickAccessItemButtons._M_finish = p + 3; g->QuickAccessItemButtons._M_end_of_storage = p + 3;
    bool r = g->IsOtherQuickAccessItemButtonHeld(0, 4);           // first other button is #1 (Code 2) -> true
    bool ok = r && g_asked == 2 && g_lastType == 2 && g_lastPlayer == 4 && g_lastValue == 0.0f;
    k[1].Code = 3; g_asked = 0;
    r = g->IsOtherQuickAccessItemButtonHeld(0, 1);                  // #1 now Code 3 -> false, #2 never asked
    ok = ok && !r && g_asked == 3;
    r = g->IsOtherQuickAccessItemButtonHeld(1, 1);                  // first other is #0 (Code 1) -> false
    ok = ok && !r && g_asked == 1;
    g->QuickAccessItemButtons._M_finish = p + 1;
    g_asked = 0; r = g->IsOtherQuickAccessItemButtonHeld(0, 1);     // only the excluded one -> false, nothing asked
    ok = ok && !r && g_asked == 0;
    std::printf(ok ? "00446060_TEST PASS\n" : "00446060_TEST FAIL\n");
    return ok ? 0 : 1;
}