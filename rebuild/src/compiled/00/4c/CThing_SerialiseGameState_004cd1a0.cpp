// Constant-return member: returns true. __fastcall this=ecx.
struct CThing { bool SerialiseGameState(int context); };
bool CThing::SerialiseGameState(int context) { return true; }