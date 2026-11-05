// Leaf: ?CThing_SetCurrentMapNumber@@YIXPAVCThing@@HF@Z
class CThing { public: char pad[0x8e]; short currentMapNumber; };
void __fastcall CThing_SetCurrentMapNumber(CThing *self, int /*edx*/, short mapNumber) {
    self->currentMapNumber = mapNumber;
}