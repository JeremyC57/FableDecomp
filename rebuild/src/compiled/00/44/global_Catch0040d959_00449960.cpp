struct CPlayer;
// Leaf: Catch@0040d959
struct CPlayerManager449960 { unsigned char unused[0x1c]; int index; CPlayer* GetPlayer(int value); };
void __fastcall Catch_0040d959(CPlayerManager449960* self) { self->GetPlayer(self->index); }