#include <cstdio>
#include "engine/CInputTypeKeyboardKeyEvent.h"
enum EInputEventType { INPUT_EVENT_TYPE_NONE };
enum EInputKey { INPUT_KEY_NONE };
class CInputEvent {
public:
    EInputEventType GetType() const;                     // 0x00A03B40
    EInputKey GetKey() const;             // 0x00A03B70
};
struct CInputTypeKeyboardKeyEvent_Methods : CInputTypeKeyboardKeyEvent {
    bool IsEventOfThisType(const CInputEvent& event) const;
};
bool CInputTypeKeyboardKeyEvent_Methods::IsEventOfThisType(const CInputEvent& event) const
{
    if (event.GetType() == 1 || event.GetType() == 2 || event.GetType() == 3)
        return event.GetKey() == *reinterpret_cast<const EInputKey*>(KeyboardKey);
    return false;
}


#include <cstring>
static int g_type, g_value, g_type_calls;
EInputEventType CInputEvent::GetType() const { ++g_type_calls; return (EInputEventType)g_type; }
EInputKey CInputEvent::GetKey() const { return (EInputKey)g_value; }
int main()
{
    static unsigned char raw[sizeof(CInputTypeKeyboardKeyEvent)]; std::memset(raw, 0, sizeof raw);
    CInputTypeKeyboardKeyEvent* base = (CInputTypeKeyboardKeyEvent*)raw; *reinterpret_cast<long*>(base->KeyboardKey) = 42;
    const CInputTypeKeyboardKeyEvent_Methods* self = static_cast<const CInputTypeKeyboardKeyEvent_Methods*>(base);
    CInputEvent e;
    bool ok = true;
    int types[3] = { 1, 2, 3 };
    for (int i = 0; i < 3; ++i) { g_type = types[i]; g_value = 42; ok = ok && self->IsEventOfThisType(e); g_value = 41; ok = ok && !self->IsEventOfThisType(e); }
    g_type = 99; g_value = 42; g_type_calls = 0; ok = ok && !self->IsEventOfThisType(e) && g_type_calls == 3;
    if (ok) std::printf("0066ee20_TEST PASS\n");
    return 0;
}