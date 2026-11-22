struct CTCInventoryBase {
// Leaf: GetCanItemBeQuickAssigned
  char pad[0xe1];
  unsigned char canQuickAssign;
};
char __fastcall GetCanItemBeQuickAssigned(CTCInventoryBase* self) {
  return (char)self->canQuickAssign;
}