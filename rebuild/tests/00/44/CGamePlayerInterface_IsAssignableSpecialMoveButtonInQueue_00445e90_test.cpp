#include <cstdio>
enum EInputButtonEventType { INPUT_BUTTON_EVENT_TYPE_NONE };
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual void _slot01() const; virtual void _slot02() const; virtual void _slot03() const; virtual void _slot04() const;
    virtual bool IsEventOfThisTypeInQueue(EInputButtonEventType button_event_type, long player, float& value) const;   // +0x14
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
    bool IsAssignableSpecialMoveButtonInQueue(long index, EInputButtonEventType button_event_type, long player) const;
};
bool CGamePlayerInterface::IsAssignableSpecialMoveButtonInQueue(long index, EInputButtonEventType button_event_type, long player) const
{
    if (index >= 0 && (unsigned long)index < AssignableSpecialMoveButtons.size())
    {
        const CInputTypeBase* input = AssignableSpecialMoveButtons[index].Data;
        float value = 0.0f;
        return input->IsEventOfThisTypeInQueue(button_event_type, player, value);
    }
    return false;
}


static int g_type, g_player; static float g_value = -1.0f;
class CInputTypeTest : public CInputTypeBase {
public:
    int Id;
    virtual bool IsEventOfThisTypeInQueue(EInputButtonEventType button_event_type, long player, float& value) const
    { g_type = button_event_type; g_player = player; g_value = value; return Id == 2; }
};
CInputTypeBase::~CInputTypeBase() {}
void CInputTypeBase::_slot01() const {} void CInputTypeBase::_slot02() const {} void CInputTypeBase::_slot03() const {} void CInputTypeBase::_slot04() const {}
bool CInputTypeBase::IsEventOfThisTypeInQueue(EInputButtonEventType, long, float&) const { return false; }
int main()
{
    static CGamePlayerInterface gpi;
    CInputTypeTest a, b; a.Id = 1; b.Id = 2;
    CCountedPointer<const CInputTypeBase> items[2]; items[0].Data = &a; items[1].Data = &b;
    gpi.AssignableSpecialMoveButtons._M_start = items; gpi.AssignableSpecialMoveButtons._M_finish = items + 2;
    bool ok = gpi.IsAssignableSpecialMoveButtonInQueue(1, (EInputButtonEventType)3, 5) && g_type == 3 && g_player == 5 && g_value == 0.0f;
    ok = ok && !gpi.IsAssignableSpecialMoveButtonInQueue(0, (EInputButtonEventType)1, 0);
    g_type = -7; ok = ok && !gpi.IsAssignableSpecialMoveButtonInQueue(2, (EInputButtonEventType)1, 0) && !gpi.IsAssignableSpecialMoveButtonInQueue(-1, (EInputButtonEventType)1, 0) && g_type == -7;
    if (ok) std::printf("00445e90_TEST PASS\n");
    return 0;
}