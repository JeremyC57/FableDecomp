// Behaviour test for 0x00691D70 CInputProcessCreatureMovement::ProcessInput (movement event, button-release gate, sneak toggle)
#include <stdio.h>
#include <string.h>

class CInputEvent { public: long DeviceType; long Type; long Key;
    long GetDeviceType() const { return DeviceType; } long GetType() const { return Type; } long GetKey() const { return Key; } };
class CGameEvent { public: long Type; unsigned char Data[32]; int End;
    CGameEvent(long type) : Type(type), End(0) { }
    void AddData(long size, const void* data) { memcpy(Data + End, data, size); End += size; } };
class CProcessedInput { public: int Events; CGameEvent Last; bool Performed;
    CProcessedInput() : Events(0), Last(0), Performed(false) { }
    void AddGameEvent(const CGameEvent& e) { ++Events; Last = e; } void SetAsPerformedEvent() { Performed = true; } };
class CTCStealth { public: bool Sneaking; bool SneakingToggleSet; };
struct Creature { bool Dead; unsigned int UID[2]; bool HasStealth; CTCStealth Stealth; };

struct Process {
    Creature* PCreature; bool HaveMovement; float MX, MY; bool Released, Pressed, SneakAction, ToggleSneak;
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed_input)
    {
        if (!PCreature || PCreature->Dead) return;
        if (HaveMovement) {
            bool button = false;
            if (event.GetDeviceType() == 2) button = true;
            else if (event.GetDeviceType() == 3) {
                switch (event.GetType()) {
                case 4: case 5: case 7: case 8: case 10: case 11:
                case 22: case 23: case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31:
                    button = true; break;
                }
            }
            if (button && !Released) return;
            CGameEvent game_event(0x10);
            int key = -1;
            if (event.GetDeviceType() == 2) key = event.GetKey();
            game_event.AddData(8, PCreature->UID);                 // AddUHUGE: the 64-bit unique id
            game_event.AddData(sizeof(MX), &MX);
            game_event.AddData(sizeof(MY), &MY);
            game_event.AddData(sizeof(key), &key);
            processed_input.AddGameEvent(game_event);
        }
        if (ToggleSneak && Pressed && SneakAction && PCreature->HasStealth) {
            PCreature->Stealth.SneakingToggleSet = !PCreature->Stealth.Sneaking;
            processed_input.SetAsPerformedEvent();
        }
    }
};

int main()
{
    int fails = 0;
    Creature c = { false, { 42, 0 }, true, { false, false } };
    Process p = { &c, true, 0.5f, -1.0f, false, false, false, false };
    CInputEvent pad = { 1, 0, 0 };
    CProcessedInput out;
    p.ProcessInput(pad, out);
    if (out.Events != 1 || out.Last.Type != 0x10 || out.Last.End != 20) ++fails;
    int key; memcpy(&key, out.Last.Data + 16, 4); if (key != -1) ++fails;
    CInputEvent mouse = { 2, 1, 7 };
    CProcessedInput out2; p.ProcessInput(mouse, out2); if (out2.Events != 0) ++fails;   // a held button is gated
    p.Released = true; CProcessedInput out3; p.ProcessInput(mouse, out3);
    memcpy(&key, out3.Last.Data + 16, 4); if (out3.Events != 1 || key != 7) ++fails;
    CInputEvent joy = { 3, 22, 0 }; p.Released = false; CProcessedInput out4; p.ProcessInput(joy, out4); if (out4.Events != 0) ++fails;
    CInputEvent joy2 = { 3, 6, 0 }; CProcessedInput out5; p.ProcessInput(joy2, out5); if (out5.Events != 1) ++fails;
    p.ToggleSneak = p.Pressed = p.SneakAction = true; CProcessedInput out6; p.ProcessInput(pad, out6);
    if (!out6.Performed || !c.Stealth.SneakingToggleSet) ++fails;
    c.Dead = true; CProcessedInput out7; p.ProcessInput(pad, out7); if (out7.Events != 0 || out7.Performed) ++fails;
    if (fails) { printf("00691d70_TEST FAIL %d\n", fails); return 1; }
    printf("00691d70_TEST PASS\n");
    return 0;
}