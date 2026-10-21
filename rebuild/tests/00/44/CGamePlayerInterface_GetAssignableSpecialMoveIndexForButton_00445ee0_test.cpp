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
    long GetAssignableSpecialMoveIndexForButton(EXboxControllerButton button) const;
};
long CGamePlayerInterface::GetAssignableSpecialMoveIndexForButton(EXboxControllerButton button) const
{
    for (unsigned long i = 0; i < AssignableSpecialMoveButtons.size(); ++i)
    {
        if (AssignableSpecialMoveButtons[i]->IsInputTypeWithButton(button))
            return i;
    }
    return -1;
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
    g->AssignableSpecialMoveButtons._M_start = p; g->AssignableSpecialMoveButtons._M_finish = p + 3; g->AssignableSpecialMoveButtons._M_end_of_storage = p + 3;
    long r1 = g->GetAssignableSpecialMoveIndexForButton((EXboxControllerButton)2); long r2 = g->GetAssignableSpecialMoveIndexForButton((EXboxControllerButton)9);
    bool ok = r1 == 1 && r2 == -1;
    std::printf(ok ? "00445ee0_TEST PASS\n" : "00445ee0_TEST FAIL\n");
    return ok ? 0 : 1;
}