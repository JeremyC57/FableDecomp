// ResetActionSpeedMultiplierOverride_007fffa0() -- switch the CTCHaste action-speed override off (-1.0f)
// Leaf: ?ResetActionSpeedMultiplierOverride_007fffa0@@YAXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007fffa0, 11 bytes, cdecl, no arguments.
// Identity (descriptive): an external void() function at the head of the CTCHaste code (0x007FFFA0, before CTCHaste::GetActionSpeedMultiplier 0x007FFFB0) called directly from Init_RegisterGameComponents 0x004EE23F (call at 0x004EE277); it stores -1.0f into ?g_ActionSpeedMultiplierOverride@@3MA 0x013BAF18 (bound by the landed GetActionSpeedMultiplier, which reads it at 0x007FFFC0/0x007FFFD3). Function name unrecovered.
// Label correction: the catalog's `$E2` is propagated by shape; a compiler dynamic initializer has internal linkage and
//   sits in .text$yc at the end of .text, while this body is in its module's code and is called directly from another TU.
extern float g_ActionSpeedMultiplierOverride;                  // 0x013BAF18, landed spelling (read by CTCHaste::GetActionSpeedMultiplier)
void ResetActionSpeedMultiplierOverride_007fffa0()
{
    g_ActionSpeedMultiplierOverride = -1.0f;
}