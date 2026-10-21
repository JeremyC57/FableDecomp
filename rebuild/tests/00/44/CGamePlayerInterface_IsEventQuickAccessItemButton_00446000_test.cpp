#include <cstdio>
enum EXboxControllerButton { XBOX_CONTROLLER_BUTTON_NONE };
enum EMouseButtonControl { MOUSE_BUTTON_CONTROL_NONE };
class CInputEvent;
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual void _slot01() const;                        // GetType
    virtual void _slot02() const;                        // GetControlDirection
    virtual bool IsEventOfThisType(const CInputEvent& event) const;                    // slot 3 (+0xc)
    virtual void _slot04() const;
    virtual void _slot05() const;                        // IsEventOfThisTypeInQueue
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
    bool IsEventQuickAccessItemButton(const CInputEvent& event, long& index) const;
};
bool CGamePlayerInterface::IsEventQuickAccessItemButton(const CInputEvent& event, long& index) const
{
    for (unsigned long i = 0; i < QuickAccessItemButtons.size(); ++i)
    {
        if (QuickAccessItemButtons[i]->IsEventOfThisType(event))
        {
            index = i;
            return true;
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
void CInputTypeBase::_slot05() const {}
bool CInputTypeBase::IsInputTypeWithButton(EXboxControllerButton) const { return false; }
bool CInputTypeBase::IsInputTypeWithMouseButton(EMouseButtonControl) const { return false; }
class CTestInput : public CInputTypeBase {
public:
    int Code;
    bool IsEventOfThisType(const CInputEvent& e) const { return e.Code == Code; }
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
    long idx = 77;
    CInputEvent e; e.Code = 2; bool r1 = g->IsEventQuickAccessItemButton(e, idx);
    bool ok = r1 && idx == 1;
    idx = 77; e.Code = 9; bool r2 = g->IsEventQuickAccessItemButton(e, idx);
    ok = ok && !r2 && idx == 77;
    g->QuickAccessItemButtons._M_finish = p; ok = ok && !g->IsEventQuickAccessItemButton(e, idx);
    std::printf(ok ? "00446000_TEST PASS\n" : "00446000_TEST FAIL\n");
    return ok ? 0 : 1;
}