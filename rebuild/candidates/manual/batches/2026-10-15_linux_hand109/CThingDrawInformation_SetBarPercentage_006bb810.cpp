// CThingDrawInformation::SetBarPercentage(float percentage) -- store the bar fill fraction, clamped to [0, 1]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BB810, length 59 bytes.
// Callees: none (float pool 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f).
// Layout: Ego_r.pdb CThingDrawInformation (0x14): DrawString +0, RelativeSize +4, BarPercentage +8, Colour +0xc, BarOn +0x10,
// StringOn +0x11 (no engine header; the retail offsets agree for the member used).
// Shape: the clamp is an inline helper taking the member by REFERENCE (a direct `if` on the member compares the argument
// register instead); the argument is stored first with an integer move, then re-read from the member for each compare
// (`fld [ecx+8] ; fcomp [0.0]` / `test ah,5 ; jp` is `< 0`, `test ah,41h ; jne` is `> 1`); the clamp values are
// immediate stores (`mov dword ptr [ecx+8],0` / `3F800000h`), each arm returning on its own.
inline void ClampFloat(float& value, float lo, float hi)
{
    if (value < lo)
        value = lo;
    else if (value > hi)
        value = hi;
}

struct CThingDrawInformation {
    unsigned char _pad00[8];
    float         BarPercentage;                // +0x08
    void SetBarPercentage(float percentage);
};

void CThingDrawInformation::SetBarPercentage(float percentage)
{
    BarPercentage = percentage;
    ClampFloat(BarPercentage, 0.0f, 1.0f);
}
