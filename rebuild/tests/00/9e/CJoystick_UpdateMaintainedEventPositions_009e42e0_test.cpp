// Behaviour test for 009e42e0: src/009e42e0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CJoystick::UpdateMaintainedEventPositions()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CJoystick::UpdateMaintainedEventPositions() -- re-stamp every maintained (held) joystick event with the stick's current position, keeping its button, value and start time
// Leaf: UpdateMaintainedEventPositions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E42E0, length 122 bytes.
// Callees: the joystick's virtual stick-axis getters (slot 3 +0xc, slot 4 +0x10; implemented by CJoystickDX), and on each
// event: CInputEvent::GetJoystickButton @ 0x00A03B90 (`movzx eax,byte ptr [ecx+8]`; PDB ?GetJoystickButton@CInputEvent@@QBEJXZ
// returns LONG -- declaring it `unsigned char` moved `mov ecx,ebx` above `push eax`), GetJoystickButtonPressureFraction @
// 0x00A03BA0 (`fld [ecx+0Ch]`), and the joystick-button setter @ 0x00A040D0 (ret 14h: long, float, const C2DVector&, double;
// it stamps event type 0x14 between its siblings 0x00A04090 (0x13) and 0x00A04110 (0x15), so the "held" variant
// SetAsJoystickButtonHeld of the PDB's Pressed/Held/Released trio).
// Labels: the manifest's "CInputEvent::SetAsJoystickPosition2" for 0x00A040D0 is wrong (SetAsJoystickPosition is 0x00A04060, ret 0Ch,
// type 0x12); "CTCInventoryBase::GetCanItemBeQuickAssigned" / "CMap::DrawGetSeaLevel" for the two getters are propagated.
// Layout: engine/CJoystick.h offsets (MaintainedEventsList, an STLport list, at +0xd0c), modelled locally because the header
//   carries the vptr as a plain word and the list as opaque bytes (the slot 3/4 virtual calls and the node walk need types); CMaintainedEvent (Ego_r.pdb, 56 bytes) holds
// its CInputEvent at +4, so a node (next, prev, value) carries the event at +0xc and its StartTime (+0x30) at +0x3c.
// Shape: the event re-addressed through the node at every use (a reference local moves the registers); `C2DVector position(GetStickX(), GetStickY())` evaluates slot 4 first (right to left) and copies it; the call's
// arguments evaluate right to left (the double start time, the position, then the two getters); the list end is
// re-read every iteration; the double argument gives the 8-aligned frame.
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CInputEvent {
public:
    unsigned char _data[0x30];
    float         StartTime;                       // +0x30
    long          GetJoystickButton() const;       // 0x00A03B90 (PDB ?GetJoystickButton@CInputEvent@@QBEJXZ)
    float         GetJoystickButtonPressureFraction() const;   // 0x00A03BA0
    void SetAsJoystickButtonHeld(long button, float pressure, const C2DVector& position, double time);   // 0x00A040D0 (event type 0x14)
};
class CMaintainedEvent { public:
    long        m_id;                              // +0x00
    CInputEvent Event;                             // +0x04
};
struct SMaintainedNode {                           // STLport list node
    SMaintainedNode* m_pNext;
    SMaintainedNode* m_pPrev;
    CMaintainedEvent m_data;                       // +0x08
};
struct CMaintainedEventList {                      // STLport list<CMaintainedEvent>: the sentinel node
    SMaintainedNode* m_pNode;
    SMaintainedNode* begin() const { return m_pNode->m_pNext; }
    SMaintainedNode* end() const { return m_pNode; }
};
class CInputManager;
class CJoystick {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual float GetStickX();                     // slot 3 (+0xc)
    virtual float GetStickY();                     // slot 4 (+0x10)
    unsigned char        EventStore[0xd00];        // +0x04 CInputEvent[64]
    CInputManager*       InputManager;             // +0xd04
    bool                 ExclusiveMode;            // +0xd08
    unsigned char        _pad_0xd09[3];
    CMaintainedEventList MaintainedEventsList;     // +0xd0c
    void UpdateMaintainedEventPositions();
};

void CJoystick::UpdateMaintainedEventPositions()
{
    for (SMaintainedNode* node = MaintainedEventsList.begin(); node != MaintainedEventsList.end(); node = node->m_pNext) {
        C2DVector position(GetStickX(), GetStickY());
        node->m_data.Event.SetAsJoystickButtonHeld(node->m_data.Event.GetJoystickButton(),
                                                   node->m_data.Event.GetJoystickButtonPressureFraction(), position,
                                                   node->m_data.Event.StartTime);
    }
}

// ---- stubs and driver ----
void CJoystick::v00() {}
void CJoystick::v01() {}
void CJoystick::v02() {}
static int g_calls = 0; static CInputEvent* g_this[4]; static long g_button[4]; static float g_pressure[4]; static float g_px[4], g_py[4]; static double g_time[4];
static int g_axis = 0;
long CInputEvent::GetJoystickButton() const { return _data[8]; }
float CInputEvent::GetJoystickButtonPressureFraction() const { return *(const float*)(_data + 0xc); }
void CInputEvent::SetAsJoystickButtonHeld(long button, float pressure, const C2DVector& position, double time)
{ g_this[g_calls] = this; g_button[g_calls] = button; g_pressure[g_calls] = pressure; g_px[g_calls] = position.X; g_py[g_calls] = position.Y; g_time[g_calls] = time; ++g_calls; }
float CJoystick::GetStickX() { ++g_axis; return 0.25f; }
float CJoystick::GetStickY() { g_axis += 10; return -0.5f; }
static CJoystick g_joy;
int main()
{
    static SMaintainedNode sentinel, n1, n2;
    memset(&n1, 0, sizeof n1); memset(&n2, 0, sizeof n2);
    sentinel.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &sentinel;
    n1.m_data.Event._data[8] = 3; *(float*)(n1.m_data.Event._data + 0xc) = 0.75f; n1.m_data.Event.StartTime = 12.0f;
    n2.m_data.Event._data[8] = 7; *(float*)(n2.m_data.Event._data + 0xc) = 1.0f; n2.m_data.Event.StartTime = 40.5f;
    g_joy.MaintainedEventsList.m_pNode = &sentinel;
    g_joy.UpdateMaintainedEventPositions();
    CHECK(g_calls == 2); CHECK(g_axis == 22);
    CHECK(g_this[0] == &n1.m_data.Event); CHECK(g_button[0] == 3); CHECK(g_pressure[0] == 0.75f); CHECK(g_px[0] == 0.25f); CHECK(g_py[0] == -0.5f); CHECK(g_time[0] == 12.0);
    CHECK(g_this[1] == &n2.m_data.Event); CHECK(g_button[1] == 7); CHECK(g_pressure[1] == 1.0f); CHECK(g_time[1] == 40.5);
    sentinel.m_pNext = &sentinel; g_calls = 0; g_joy.UpdateMaintainedEventPositions(); CHECK(g_calls == 0);   // empty list
    if (fails == 0) printf("009e42e0_TEST PASS\n"); else printf("009e42e0_TEST FAIL\n");
    return fails;
}