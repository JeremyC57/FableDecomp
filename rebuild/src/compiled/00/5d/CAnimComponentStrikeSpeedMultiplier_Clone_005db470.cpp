// Leaf: ?CAnimComponentStrikeSpeedMultiplier_Clone@@YIPAVCAnimComponentStrikeSpeedMultiplier@@PAV1@@Z
class CAnimComponentStrikeSpeedMultiplier; typedef CAnimComponentStrikeSpeedMultiplier* (__fastcall* VFn)(CAnimComponentStrikeSpeedMultiplier*);
class CAnimComponentStrikeSpeedMultiplier { public: VFn* vtbl; char pad[4]; void* f8; };
CAnimComponentStrikeSpeedMultiplier* __fastcall CAnimComponentStrikeSpeedMultiplier_Clone(CAnimComponentStrikeSpeedMultiplier* component){ CAnimComponentStrikeSpeedMultiplier* p = component->vtbl[0x10/4](component); p->f8 = component->f8; return p; }