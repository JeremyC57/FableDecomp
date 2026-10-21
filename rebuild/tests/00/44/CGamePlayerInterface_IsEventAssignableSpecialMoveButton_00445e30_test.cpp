#include <cstdio>
class CInputEvent;
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual void _slot01() const;
    virtual void _slot02() const;
    virtual bool IsEventOfThisType(const CInputEvent& event) const;   // slot 3 (+0xc)
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
    bool IsEventAssignableSpecialMoveButton(const CInputEvent& event) const;
};
bool CGamePlayerInterface::IsEventAssignableSpecialMoveButton(const CInputEvent& event) const
{
    for (unsigned long i = 0; i < AssignableSpecialMoveButtons.size(); ++i)
    {
        if (AssignableSpecialMoveButtons[i]->IsEventOfThisType(event))
            return true;
    }
    return false;
}

class CInputEvent { public: int Code; };
CInputTypeBase::~CInputTypeBase() {}
void CInputTypeBase::_slot01() const {}
void CInputTypeBase::_slot02() const {}
bool CInputTypeBase::IsEventOfThisType(const CInputEvent&) const { return false; }
static int g_checked;
class CKeyInput : public CInputTypeBase {
public:
    int Code;
    bool IsEventOfThisType(const CInputEvent& e) const { ++g_checked; return e.Code == Code; }
};
int main()
{
    static unsigned char raw[sizeof(CGamePlayerInterface)];
    CGamePlayerInterface* g = (CGamePlayerInterface*)raw;
    CKeyInput k[3]; k[0].Code = 1; k[1].Code = 2; k[2].Code = 3;
    CCountedPointer<const CInputTypeBase> p[3];
    for (int i = 0; i < 3; ++i) { p[i].Data = &k[i]; p[i].Info = 0; }
    g->AssignableSpecialMoveButtons._M_start = p; g->AssignableSpecialMoveButtons._M_finish = p + 3; g->AssignableSpecialMoveButtons._M_end_of_storage = p + 3;
    CInputEvent e; e.Code = 2;
    bool ok = g->IsEventAssignableSpecialMoveButton(e) && g_checked == 2;
    e.Code = 9; g_checked = 0; ok = ok && !g->IsEventAssignableSpecialMoveButton(e) && g_checked == 3;
    g->AssignableSpecialMoveButtons._M_finish = p; g_checked = 0; ok = ok && !g->IsEventAssignableSpecialMoveButton(e) && g_checked == 0;
    std::printf(ok ? "00445e30_TEST PASS\n" : "00445e30_TEST FAIL\n");
    return ok ? 0 : 1;
}