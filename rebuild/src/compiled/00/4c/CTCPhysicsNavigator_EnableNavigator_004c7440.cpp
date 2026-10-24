struct CTCPhysicsNavigator { char pad[0x90]; char navigatorEnabled; };
// Leaf: EnableNavigator
void __fastcall EnableNavigator(CTCPhysicsNavigator *self, void *edx, char enable)
{
    (void)edx;
    self->navigatorEnabled = enable;
}