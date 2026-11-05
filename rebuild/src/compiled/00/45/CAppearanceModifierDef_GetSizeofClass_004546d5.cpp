// Leaf: ?CAppearanceModifierDef_GetSizeofClass@@YIKPBVCSpecialAbilitiesDrainLifeDef@@@Z
#pragma optimize("s",on)
class CSpecialAbilitiesDrainLifeDef { public: char pad[0x70]; };

unsigned long __fastcall CAppearanceModifierDef_GetSizeofClass(const CSpecialAbilitiesDrainLifeDef* self)
{
    (void)self;
    return 0x70;
}
// Stand-in (owner rule 2026-11-05): CAppearanceModifierDef_GetSizeofClass for CAppearanceModifierDef::GetSizeofClass; RTTI vtable slot ?GetSizeofClass@CAppearanceModifierDef@@UAEKXZ + PDB name set (fablewin).
