struct CJoystickDX; extern void __fastcall h1(CJoystickDX* joystickDX); extern void __fastcall h2(CJoystickDX* joystickDX);
struct CJoystickDX { int x; };
void __fastcall Close(CJoystickDX* joystickDX){ h1(joystickDX); h2(joystickDX); }