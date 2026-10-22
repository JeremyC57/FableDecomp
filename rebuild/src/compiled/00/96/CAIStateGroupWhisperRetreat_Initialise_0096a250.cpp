struct C { char pad[0x28]; int NumHits; int FrameOfLastHit; void Initialise(); };
extern void __fastcall base_init(C*);
void C::Initialise(){ this->NumHits = 0; this->FrameOfLastHit = 0; base_init(this); }