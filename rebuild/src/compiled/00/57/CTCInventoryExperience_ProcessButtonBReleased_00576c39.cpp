class CTCInventoryExperience { public:
  char pad[0x1c];
  int field_1c;
};
extern "C" void __fastcall Helper_00576c0b(CTCInventoryExperience* experience);
extern "C" void __fastcall Tail_005bc66f(CTCInventoryExperience* experience);
extern "C" void __fastcall ProcessButtonBReleased(CTCInventoryExperience* experience)
{
  if (experience->field_1c == 1) {
    Helper_00576c0b(experience);
  }
  Tail_005bc66f(experience);
}