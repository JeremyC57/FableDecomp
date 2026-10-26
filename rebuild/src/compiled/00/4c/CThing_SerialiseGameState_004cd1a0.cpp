// Constant-return member: returns true. __fastcall this=ecx.
class CThing { public: bool SerialiseGameState(int context); };
bool CThing::SerialiseGameState(int context) { return true; }