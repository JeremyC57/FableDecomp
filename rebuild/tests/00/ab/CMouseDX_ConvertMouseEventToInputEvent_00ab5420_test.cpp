// Behaviour test for CMouseDX::ConvertMouseEventToInputEvent (0x00AB5420): button presses map to the pressed setters at the
// current position and event time; releases take their start time from the maintained held event of the matching type
// (4 -> 5, 6 -> 8, 5 -> 11, extra 20..24 -> 27..31) or the event time; 10 is the wheel; anything else is not converted.
#include <cstdio>
struct Vec { float X, Y; };
struct Maintained { int type; float startTime; };
struct InputEvent { int which; float x; double start; double time; long button; float wheel; };
struct MouseEvent { double Time; int Type; float Movement; };
struct Mouse {
    Vec Pos; Maintained held[3]; int nheld;
    bool Convert(const MouseEvent* e, InputEvent* out)
    {
        float startTime = (float)e->Time; long heldType = 0; bool converted = false; Vec pos = Pos;
        switch (e->Type) {
        case 4: heldType = 5; break; case 6: heldType = 8; break; case 5: heldType = 11; break;
        case 20: case 21: case 22: case 23: case 24: heldType = e->Type + 7; break;
        }
        switch (e->Type) {
        case 4: case 5: case 6: case 20: case 21: case 22: case 23: case 24:
            for (int i = 0; i < nheld; ++i) if (held[i].type == heldType) { startTime = held[i].startTime; break; }
            break;
        }
        out->x = pos.X; out->time = e->Time; out->start = startTime;
        switch (e->Type) {
        case 1: out->which = 1; converted = true; break;
        case 3: out->which = 3; converted = true; break;
        case 2: out->which = 2; converted = true; break;
        case 15: case 16: case 17: case 18: case 19: out->which = 15; out->button = e->Type - 15; converted = true; break;
        case 4: out->which = 4; converted = true; break;
        case 6: out->which = 6; converted = true; break;
        case 5: out->which = 5; converted = true; break;
        case 20: case 21: case 22: case 23: case 24: out->which = 20; out->button = e->Type - 20; converted = true; break;
        case 10: out->which = 10; out->wheel = e->Movement; converted = true; break;
        }
        return converted;
    }
};
int main()
{
    Mouse m; m.Pos.X = 12.0f; m.Pos.Y = 34.0f; m.nheld = 2;
    m.held[0].type = 11; m.held[0].startTime = 2.5f; m.held[1].type = 29; m.held[1].startTime = 1.5f;
    InputEvent out; MouseEvent e;
    e.Time = 4.0; e.Type = 5; e.Movement = 0;
    if (!m.Convert(&e, &out) || out.which != 5 || out.start != 2.5 || out.time != 4.0 || out.x != 12.0f) { std::printf("00ab5420_TEST FAIL\n"); return 1; }
    e.Type = 4;
    if (!m.Convert(&e, &out) || out.which != 4 || out.start != 4.0) { std::printf("00ab5420_TEST FAIL\n"); return 1; }
    e.Type = 22;
    if (!m.Convert(&e, &out) || out.which != 20 || out.button != 2 || out.start != 1.5) { std::printf("00ab5420_TEST FAIL\n"); return 1; }
    e.Type = 17;
    if (!m.Convert(&e, &out) || out.which != 15 || out.button != 2) { std::printf("00ab5420_TEST FAIL\n"); return 1; }
    e.Type = 10; e.Movement = -120.0f;
    if (!m.Convert(&e, &out) || out.which != 10 || out.wheel != -120.0f) { std::printf("00ab5420_TEST FAIL\n"); return 1; }
    e.Type = 7;
    if (m.Convert(&e, &out)) { std::printf("00ab5420_TEST FAIL\n"); return 1; }
    std::printf("00ab5420_TEST PASS\n");
    return 0;
}