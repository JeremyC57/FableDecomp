// Behaviour test for CSubtitleRenderer::GetGraphicFromEmbeddedTag (0x00490BE0): upper-case the tag, map the controller
// button names (both spellings of the two-name buttons) to the def's controller graphics, else look the tag up in the
// script-sprite table (0 when absent). Standalone model of the same decision chain.
#include <cstdio>
#include <cstring>
#include <cctype>

struct GuiDefModel {
    long A, B, X, Y, DPad, DPadDown, DPadLeft, DPadRight, DPadUp, ThumbLeft, ThumbRight, ThumbClickLeft, ThumbClickRight,
         Back, Start, TriggerLeft, TriggerRight, TriggerBlack, TriggerWhite;
    const char* SpriteNames[3];
    long SpriteValues[3];
};

static long Lookup(const GuiDefModel& def, const char* tag)
{
    char up[64]; unsigned i = 0;
    for (; tag[i] && i < 63; ++i) up[i] = (unsigned char)tag[i] < 0x80 ? (char)toupper((unsigned char)tag[i]) : tag[i];
    up[i] = 0;
    struct Entry { const char* name; long value; } table[] = {
        {"A", def.A}, {"B", def.B}, {"X", def.X}, {"Y", def.Y}, {"DPAD", def.DPad}, {"D-PAD", def.DPad},
        {"DPAD DOWN", def.DPadDown}, {"D-PAD DOWN", def.DPadDown}, {"DPAD LEFT", def.DPadLeft}, {"D-PAD LEFT", def.DPadLeft},
        {"DPAD RIGHT", def.DPadRight}, {"D-PAD RIGHT", def.DPadRight}, {"DPAD UP", def.DPadUp}, {"D-PAD UP", def.DPadUp},
        {"LEFT STICK", def.ThumbLeft}, {"LEFT THUMBSTICK", def.ThumbLeft}, {"RIGHT STICK", def.ThumbRight},
        {"RIGHT THUMBSTICK", def.ThumbRight}, {"LEFT STICK CLICK", def.ThumbClickLeft}, {"LEFT THUMBSTICK CLICK", def.ThumbClickLeft},
        {"RIGHT STICK CLICK", def.ThumbClickRight}, {"RIGHT THUMBSTICK CLICK", def.ThumbClickRight}, {"BACK", def.Back},
        {"START", def.Start}, {"LEFT TRIGGER", def.TriggerLeft}, {"RIGHT TRIGGER", def.TriggerRight}, {"BLACK", def.TriggerBlack},
        {"WHITE", def.TriggerWhite} };
    for (unsigned k = 0; k < sizeof(table) / sizeof(table[0]); ++k)
        if (strcmp(up, table[k].name) == 0) return table[k].value;
    for (unsigned k = 0; k < 3; ++k)
        if (def.SpriteNames[k] && strcmp(up, def.SpriteNames[k]) == 0) return def.SpriteValues[k];
    return 0;
}

int main()
{
    GuiDefModel def = { 10, 11, 12, 13, 20, 21, 22, 23, 24, 30, 31, 32, 33, 40, 41, 50, 51, 52, 53,
                        { "COIN", "SKULL", 0 }, { 7, 9, 0 } };
    struct Case { const char* tag; long want; } cases[] = {
        {"a", 10}, {"B", 11}, {"x", 12}, {"y", 13}, {"dpad", 20}, {"D-Pad", 20}, {"dpad down", 21}, {"d-pad down", 21},
        {"Dpad Left", 22}, {"D-PAD RIGHT", 23}, {"dpad up", 24}, {"left stick", 30}, {"Left Thumbstick", 30}, {"right stick", 31},
        {"right thumbstick click", 33}, {"left stick click", 32}, {"back", 40}, {"start", 41}, {"left trigger", 50},
        {"right trigger", 51}, {"black", 52}, {"white", 53}, {"coin", 7}, {"Skull", 9}, {"unknown", 0}, {"", 0} };
    for (unsigned k = 0; k < sizeof(cases) / sizeof(cases[0]); ++k) {
        if (Lookup(def, cases[k].tag) != cases[k].want) { std::printf("00490be0_TEST FAIL\n"); return 1; }
    }
    std::printf("00490be0_TEST PASS\n");
    return 0;
}
