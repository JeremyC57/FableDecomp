#define FABLE_RETAIL_CDRAWBETTINGHUD_PASS30
// Standalone behavior cases for the betting arrows and background placement.
#define FABLE_PORTABLE
#define FABLE_RETAIL_C2DVECTOR
#include "engine/C2DVector.h"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace {
struct Drawing {
    C2DVector background;
    C2DVector arrow[4];
    const char* label[4];
    bool queried_betting;
};

Drawing Layout(bool has_game, bool donating, long width, long height,
               float vertical_anchor, unsigned long graphic_width, unsigned long graphic_height,
               const C2DVector* offsets, const float* text_width, float font_height)
{
    const char* labels[2][4] = {
        {"TEXT_GUI_BETTING_ARROW_UP", "TEXT_GUI_BETTING_ARROW_DOWN",
         "TEXT_GUI_BETTING_ARROW_LEFT", "TEXT_GUI_BETTING_ARROW_RIGHT"},
        {"TEXT_GUI_DONATING_ARROW_UP", "TEXT_GUI_DONATING_ARROW_DOWN",
         "TEXT_GUI_DONATING_ARROW_LEFT", "TEXT_GUI_DONATING_ARROW_RIGHT"}
    };
    Drawing drawing;
    drawing.queried_betting = has_game;
    const float font_size = 0.8f;
    const C2DVector base(width * 0.5f, height * vertical_anchor);
    const C2DVector graphic((float)graphic_width, (float)graphic_height);
    drawing.background = base - graphic * 0.5f;
    for (int i = 0; i != 4; ++i) {
        drawing.arrow[i] = base + offsets[i];
        drawing.label[i] = labels[donating][i];
    }
    drawing.arrow[0].X -= text_width[0] * font_size * 0.5f;
    drawing.arrow[0].Y -= font_height * font_size + graphic.Y * 0.5f;
    drawing.arrow[1].X -= text_width[1] * font_size * 0.5f;
    drawing.arrow[1].Y += graphic.Y * 0.5f;
    drawing.arrow[2].X -= text_width[2] * font_size + graphic.X * 0.5f;
    drawing.arrow[2].Y -= font_height * font_size * 0.5f;
    drawing.arrow[3].X += graphic.X * font_size * 0.5f;
    drawing.arrow[3].Y -= font_height * font_size * 0.5f;
    return drawing;
}

void Check(const C2DVector& v, float x, float y)
{
    assert(std::fabs(v.X - x) < 0.0001f);
    assert(std::fabs(v.Y - y) < 0.0001f);
}
}

int main()
{
    const C2DVector offsets[4] = {
        C2DVector(6, -9), C2DVector(-4, 8), C2DVector(-12, 2), C2DVector(7, -3)
    };
    const float width[4] = {40, 55, 80, 130};
    for (int has_game = 0; has_game != 2; ++has_game) {
        for (int donating = 0; donating != 2; ++donating) {
            const Drawing drawing = Layout(has_game != 0, donating != 0, 1000, 720,
                                           0.25f, 100, 40, offsets, width, 20);
            assert(drawing.queried_betting == (has_game != 0));
            Check(drawing.background, 450, 160);
            Check(drawing.arrow[0], 490, 135);
            Check(drawing.arrow[1], 474, 208);
            Check(drawing.arrow[2], 374, 174);
            Check(drawing.arrow[3], 547, 169);
            const char* suffixes[4] = {"UP", "DOWN", "LEFT", "RIGHT"};
            for (int i = 0; i != 4; ++i) {
                char expected[40];
                std::sprintf(expected, "TEXT_GUI_%s_ARROW_%s", donating ? "DONATING" : "BETTING", suffixes[i]);
                assert(std::strcmp(drawing.label[i], expected) == 0);
            }
        }
    }
    const C2DVector zero_offsets[4] = {
        C2DVector(0, 0), C2DVector(0, 0), C2DVector(0, 0), C2DVector(0, 0)
    };
    const float zero_width[4] = {0, 0, 0, 0};
    const Drawing collapsed = Layout(false, true, 800, 600, 0.5f, 0, 0, zero_offsets, zero_width, 0);
    Check(collapsed.background, 400, 300);
    for (int i = 0; i != 4; ++i)
        Check(collapsed.arrow[i], 400, 300);
    std::puts("00649952_TEST PASS");
}