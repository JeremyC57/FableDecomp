// Leaf: ?CTCOnHeroPush_IsAngry@@YI_NPAVCTCOnHeroPush@@@Z
class CTCOnHeroPush { public:
  char pad[0x3c];
  float f3c;
  float f40;
};
bool __fastcall CTCOnHeroPush_IsAngry(CTCOnHeroPush* self) {
  return self->f3c >= self->f40;
}