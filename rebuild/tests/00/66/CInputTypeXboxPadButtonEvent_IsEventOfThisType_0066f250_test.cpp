#include <cstdio>
#include "engine/CInputTypeXboxPadButtonEvent.h"
enum EInputEventType { INPUT_EVENT_TYPE_NONE };
class CInputEvent {
public:
    EInputEventType GetType() const;                     // 0x00A03B40
    long GetJoystickButton() const;       // 0x00A03B90
};
struct CInputTypeXboxPadButtonEvent_Methods : CInputTypeXboxPadButtonEvent {
    bool IsEventOfThisType(const CInputEvent& event) const;
};
bool CInputTypeXboxPadButtonEvent_Methods::IsEventOfThisType(const CInputEvent& event) const
{
    if (event.GetType() == 19 || event.GetType() == 20 || event.GetType() == 21)
        return event.GetJoystickButton() == *reinterpret_cast<const long*>(Button);
    return false;
}


#include <cstring>
static int g_type, g_value, g_type_calls;
EInputEventType CInputEvent::GetType() const { ++g_type_calls; return (EInputEventType)g_type; }
long CInputEvent::GetJoystickButton() const { return (long)g_value; }
int main()
{
    static unsigned char raw[sizeof(CInputTypeXboxPadButtonEvent)]; std::memset(raw, 0, sizeof raw);
    CInputTypeXboxPadButtonEvent* base = (CInputTypeXboxPadButtonEvent*)raw; *reinterpret_cast<long*>(base->Button) = 42;
    const CInputTypeXboxPadButtonEvent_Methods* self = static_cast<const CInputTypeXboxPadButtonEvent_Methods*>(base);
    CInputEvent e;
    bool ok = true;
    int types[3] = { 19, 20, 21 };
    for (int i = 0; i < 3; ++i) { g_type = types[i]; g_value = 42; ok = ok && self->IsEventOfThisType(e); g_value = 41; ok = ok && !self->IsEventOfThisType(e); }
    g_type = 99; g_value = 42; g_type_calls = 0; ok = ok && !self->IsEventOfThisType(e) && g_type_calls == 3;
    if (ok) std::printf("0066f250_TEST PASS\n");
    return 0;
}