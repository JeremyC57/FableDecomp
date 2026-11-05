// CInputTypeXboxPadRightStickEvent::IsInputTypeWithMouseButton (0x004456d0)
// Leaf: ?CInputTypeXboxPadRightStickEvent_IsInputTypeWithMouseButton@@YI_NPAX0H@Z
// Retail: 32 c0 c2 04 00  => xor al,al ; ret 4  (returns false)
bool __fastcall CInputTypeXboxPadRightStickEvent_IsInputTypeWithMouseButton(void *event, void *edx, int mouseButton)
{
    (void)event; (void)edx; (void)mouseButton;
    return false;
}
// Stand-in (owner rule 2026-11-05): CInputTypeXboxPadRightStickEvent_IsInputTypeWithMouseButton for CInputTypeXboxPadRightStickEvent::IsInputTypeWithMouseButton; RTTI vtable slot ?IsInputTypeWithMouseButton@CInputTypeXboxPadRightStickEvent@@UBE_NW4EMouseButtonControl@@@Z + PDB name set (fablewin).
