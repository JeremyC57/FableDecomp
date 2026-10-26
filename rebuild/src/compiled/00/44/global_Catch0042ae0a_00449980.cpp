class CPlayer {
public:
    long GetJoystickDeviceNumber() const;   // 0x00487510
};
struct CPlayerManager449980 { unsigned char unused[0x1c]; int index; CPlayer* GetPlayer(int value); };
void __fastcall Catch_0042ae0a(CPlayerManager449980* self) { ((CPlayer*)(self->GetPlayer(self->index)))->GetJoystickDeviceNumber(); }