// CThemeDisplay_00c21a20::ActivateThemeDensityDisplay(long arg) -- dispatch state 2 on this object, set the global state value from the argument, then process the argument on this object
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C21A20, length 35 bytes.
// Callees: the state dispatch @ 0x00C20720 (on `this`, one argument: 2; the switch over the word at +0xac), the `__stdcall` global setter @ 0x00C20920
// (one argument; manifest "Thiscall_SetStateValue_1" is propagated), the processor @ 0x00C21780 (on `this`, the argument; manifest
// "CInputProcessRebootGame_HandleInputEvent" is propagated). Shape: the argument is read once into edi after the first call and pushed twice.
// The chunk label "CEditControlCentre::ActivateThemeDensityDisplay" keeps the method name; the class is named by address.
void __stdcall SetStateValue_00c20920(long value);   // 0x00C20920

struct CThemeDisplay_00c21a20 {
    void Dispatch(long state);                     // 0x00C20720
    void Process(long arg);                        // 0x00C21780
    void ActivateThemeDensityDisplay(long arg);
};

void CThemeDisplay_00c21a20::ActivateThemeDensityDisplay(long arg)
{
    Dispatch(2);
    SetStateValue_00c20920(arg);
    Process(arg);
}
