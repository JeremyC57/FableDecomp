// Behaviour test for NScript::CQ_UndeadRisingScript::ActivateNextLamp 0x00E0E040
#include <cstdio>
#include <cstring>
class C3DVector { public: float X, Y, Z; };
class CScriptThing { public: int Id; C3DVector Pos; const C3DVector& GetPos() const { return Pos; } };
class CGame { public:
    float OnScreenMaxX;                                 // positions with X < this are "on screen"
    int Effects;
    char LastEffect[64];
    C3DVector LastPos;
    bool IsCameraPosOnScreen(const C3DVector& p) const { return p.X < OnScreenMaxX; }
    CScriptThing CreateEffect(const char* name, const C3DVector& pos, const char* bone, float, bool, bool)
    {
        ++Effects;
        std::strcpy(LastEffect, name);
        LastPos = pos;
        CScriptThing t; t.Id = 99; t.Pos = pos;
        return t;
    }
};
struct CQ_UndeadRisingScript {
    CGame* Game;
    bool Terminating;
    unsigned long number_of_zombie_tokens_in_right_area;
    bool LightsActivated[8];
    CScriptThing Points[8];
    unsigned int NumPoints;
    bool IsActiveThreadTerminating() { return Terminating; }
    void ActivateNextLamp()
    {
        if (number_of_zombie_tokens_in_right_area < NumPoints) {
            long lamp = -1;
            long i;
            for (i = 0; i < 8; ++i) {
                if (IsActiveThreadTerminating()) return;
                if (!LightsActivated[i] && Game->IsCameraPosOnScreen(Points[i].GetPos())) {
                    if (IsActiveThreadTerminating()) return;
                    lamp = i;
                    break;
                }
            }
            if (IsActiveThreadTerminating()) return;
            if (lamp == -1) {
                if (IsActiveThreadTerminating()) return;
                for (i = 0; i < 8; ++i) {
                    if (IsActiveThreadTerminating()) return;
                    if (!LightsActivated[i]) {
                        if (IsActiveThreadTerminating()) return;
                        lamp = i;
                        break;
                    }
                }
                if (IsActiveThreadTerminating()) return;
            }
            LightsActivated[lamp] = true;
            CScriptThing point = Points[lamp];
            CScriptThing effect;
            effect = Game->CreateEffect("GRAVEYARD_CIRCLE_LIGHT_SMALL", point.GetPos(), "", 0.0f, false, false);
            ++number_of_zombie_tokens_in_right_area;
        }
    }
};
int main()
{
    CGame g; g.OnScreenMaxX = 5.0f; g.Effects = 0;
    CQ_UndeadRisingScript s;
    s.Game = &g; s.Terminating = false; s.number_of_zombie_tokens_in_right_area = 0; s.NumPoints = 8;
    for (int i = 0; i < 8; ++i) {
        s.LightsActivated[i] = false;
        s.Points[i].Id = i; s.Points[i].Pos.X = (float)(10 - i); s.Points[i].Pos.Y = 0.0f; s.Points[i].Pos.Z = 0.0f;
    }
    // points 6 and 7 (X = 4, 3) are on screen: the first unlit on-screen point wins
    s.ActivateNextLamp();
    if (!s.LightsActivated[6] || g.Effects != 1 || s.number_of_zombie_tokens_in_right_area != 1
        || std::strcmp(g.LastEffect, "GRAVEYARD_CIRCLE_LIGHT_SMALL") != 0 || g.LastPos.X != 4.0f) {
        std::printf("00e0e040_TEST FAIL\n"); return 1;
    }
    s.ActivateNextLamp();
    if (!s.LightsActivated[7] || g.Effects != 2) { std::printf("00e0e040_TEST FAIL\n"); return 1; }
    // nothing unlit on screen: fall back to the first unlit point
    s.ActivateNextLamp();
    if (!s.LightsActivated[0] || g.Effects != 3 || s.number_of_zombie_tokens_in_right_area != 3) {
        std::printf("00e0e040_TEST FAIL\n"); return 1;
    }
    // terminating: nothing changes
    s.Terminating = true;
    s.ActivateNextLamp();
    if (s.LightsActivated[1] || g.Effects != 3) { std::printf("00e0e040_TEST FAIL\n"); return 1; }
    // all tokens used: nothing happens
    s.Terminating = false; s.number_of_zombie_tokens_in_right_area = 8;
    s.ActivateNextLamp();
    if (s.LightsActivated[1] || g.Effects != 3) { std::printf("00e0e040_TEST FAIL\n"); return 1; }
    std::printf("00e0e040_TEST PASS\n");
    return 0;
}