#include <cstdio>
enum EInputDeviceType { INPUT_DEVICE_TYPE_KEYBOARD = 2 };   // value as stored (name not confirmed)
enum EInputEventType { INPUT_EVENT_TYPE_KEY_HELD = 2 };     // value as pushed (name not confirmed)
enum EInputKey { INPUT_KEY_FORCE_DWORD = 0x7fffffff };
enum EInputType { INPUT_TYPE_KEYBOARD_AND_MOUSE = 0 };      // value as compared (name not confirmed)
class CInputEvent {
public:
    unsigned char    Data[0x20];                         // +0x00 UInputEventData
    EInputDeviceType DeviceType;                         // +0x20
    long             DeviceNumber;                       // +0x24
    EInputEventType  Type;                               // +0x28
    float            CurrentTime;                        // +0x2c
    float            StartTime;                          // +0x30
    CInputEvent();                                       // 0x00A04410
    ~CInputEvent();                                      // 0x00A03B30
};
class CInputManager {
public:
    bool IsKeyboardEventInQueue(EInputEventType type, EInputKey key, CInputEvent& event) const;   // 0x009F50A0
    void RemoveEventFromQueue(const CInputEvent& event);                                          // 0x009F4F70
};
namespace NUISystem {
class CManager {
public:
    enum EUsedKeys { USED_KEYS_FORCE_DWORD = 0x7fffffff };
    class CKeyMap {                                      // std::map<EUsedKeys, long> (12 bytes)
    public:
        void* _M_header;
        void* _M_node_count;
        void* _M_key_compare;
        long& operator[](const EUsedKeys& key);          // 0x0042D201
    };
    unsigned char  _pad00[0x20];
    CInputManager* PInputManager;                        // +0x20
    CKeyMap        Keys;                                 // +0x24
    EInputType     InputType;                            // +0x30
    bool IsKeyHeld(EUsedKeys key);
};
bool CManager::IsKeyHeld(EUsedKeys key)
{
    if (InputType == INPUT_TYPE_KEYBOARD_AND_MOUSE)
    {
        CInputEvent event;
        CInputManager* input = PInputManager;
        event.DeviceType = INPUT_DEVICE_TYPE_KEYBOARD;
        bool held = input->IsKeyboardEventInQueue(INPUT_EVENT_TYPE_KEY_HELD, (EInputKey)Keys[key], event);
        PInputManager->RemoveEventFromQueue(event);
        return held;
    }
    return false;
}}

static int g_ctor, g_dtor, g_removed; static long g_key; static EInputEventType g_type; static const CInputEvent* g_removedEv; static int g_seenDevice;
CInputEvent::CInputEvent() { ++g_ctor; DeviceType = (EInputDeviceType)0; }
CInputEvent::~CInputEvent() { ++g_dtor; }
bool CInputManager::IsKeyboardEventInQueue(EInputEventType type, EInputKey key, CInputEvent& event) const
{ g_type = type; g_key = key; g_seenDevice = event.DeviceType; return key == 42; }
void CInputManager::RemoveEventFromQueue(const CInputEvent& event) { ++g_removed; g_removedEv = &event; }
static long g_values[4] = { 40, 41, 42, 43 };
long& NUISystem::CManager::CKeyMap::operator[](const EUsedKeys& key) { return g_values[key]; }
int main()
{
    CInputManager im; NUISystem::CManager m; m.PInputManager = &im; m.InputType = (EInputType)1;
    bool ok = !m.IsKeyHeld((NUISystem::CManager::EUsedKeys)2) && g_ctor == 0 && g_removed == 0;
    m.InputType = INPUT_TYPE_KEYBOARD_AND_MOUSE;
    ok = ok && m.IsKeyHeld((NUISystem::CManager::EUsedKeys)2) && g_key == 42 && g_type == 2 && g_seenDevice == 2 && g_removed == 1 && g_ctor == 1 && g_dtor == 1;
    ok = ok && !m.IsKeyHeld((NUISystem::CManager::EUsedKeys)3) && g_key == 43 && g_removed == 2 && g_dtor == 2;
    std::printf(ok ? "0041e16e_TEST PASS\n" : "0041e16e_TEST FAIL\n");
    return ok ? 0 : 1;
}