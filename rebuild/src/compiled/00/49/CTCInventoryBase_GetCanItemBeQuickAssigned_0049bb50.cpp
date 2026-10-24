struct CTCInventoryBase {
// Leaf: GetCanItemBeQuickAssigned
    char pad[8];
    bool canQuickAssign;
};

bool __fastcall CTCInventoryBase_GetCanItemBeQuickAssigned(const CTCInventoryBase* self) {
    return self->canQuickAssign;
}