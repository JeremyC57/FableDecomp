// _global::SetDefaultRange(float rangeMin, float rangeMax) -- stores the parameter record's default range pair in the module globals that CChickenSign::Init (0x00C1BC90) pushes into SetRangeMin/SetRangeMax
// Leaf: SetDefaultRange
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B9A0, 22 bytes, __stdcall `ret 8`.
// Caller: the audio system start-up @ 0x00C09595 (`mov eax,[edi+20h] ; mov ecx,[edi+1Ch] ; push eax ; push ecx ; call`,
//   right after the falloff-exponent setter @ 0x00C1BD80 fed [edi+18h]); ecx carries an argument value, not `this`.
// Globals: 0x0139C96C (default 1.0) and 0x0139C970 (default 30.0), read only by CChickenSign::Init.
// Shape: both arguments load before the two stores (plain dword float copies, no FPU).
// Label correction: the catalog's `$E2` (a dynamic-initializer name) is propagated; descriptive name.
extern float g_defaultRangeMin;                 // 0x0139C96C
extern float g_defaultRangeMax;                 // 0x0139C970

void __stdcall SetDefaultRange(float rangeMin, float rangeMax)
{
    g_defaultRangeMin = rangeMin;
    g_defaultRangeMax = rangeMax;
}