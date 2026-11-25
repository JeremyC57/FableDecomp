// Constant-return member: returns 0x12b5b78. __fastcall this=ecx.
// Literal (NF415, 2026-11-25): was the bare integer 0x12b5b78 (the retail VA of "Win32"); now the literal is spelled, cast to the
//   placeholder's existing int return type so the self symbol does not change (its name and return type are T3). Retail 0x012B5B78: 57 69 6E 33 32 00.
struct T { int Get(); };
int T::Get() { return (int)"Win32"; }