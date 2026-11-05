// CWeaponDef::GetSizeofClass @ 0x004e26b6
// Leaf: ?CWeaponDef_GetSizeofClass@@YIKPAVCWeaponDef@@@Z
// Retail: mov eax, 0xe4 ; ret  (returns sizeof-of-class constant)
class CWeaponDef;

unsigned long __fastcall CWeaponDef_GetSizeofClass(CWeaponDef *self)
{
    (void)self;
    return 0xe4;
}