// CControlsDef::Transfer(CPersistContext& context) -- transfer the control bindings and the six toggle flags
// Leaf: ?Transfer@CControlsDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe @ 0x004330F0, 90 bytes, `ret 4`; slot 0 of the CControlsDef vtable 0x01230ECC (slots.py: Copy 0x00431E70,
// GetSizeofClass 0x00430470 follow), so the label is confirmed. Body: CPersistContext::TransferVectorBinary<CActionInputControl>
// (0x0043318D, Controls +0x3c), then the context's bool transfer (0x0043314A: CheckCRC, then load/save by Mode) on the six
// flags +0x48..+0x4d. No base-class Transfer call.
// Layout: retail CControlsDef: CParentDefClassBase 0x39 bytes padded to 0x3c, Controls +0x3c (12-byte STLport vector),
// ToggleZTarget..FlourishNeedsAttackButtonHeld +0x48..+0x4d (Ego_r.pdb +0x40/+0x50..+0x55; the sibling CFrontEndDef
// 0x004331C0 has the same 4-byte shift).
// Shape: /O2.
class CActionInputControl;
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
class CPersistContext {
public:
    template <class T> void TransferVectorBinary(std::vector<T>& v);   // 0x0043318D for CActionInputControl
    void TransferBool(bool& value);                      // 0x0043314A
};
class CControlsDef {
public:
    virtual void Transfer(CPersistContext& context);     // slot 0
    unsigned char _base[0x38];                           // rest of CParentDefClassBase (0x39 bytes) and padding
    std::vector<CActionInputControl> Controls;           // +0x3c
    bool ToggleZTarget;                                  // +0x48
    bool ToggleSpells;                                   // +0x49
    bool ToggleSneak;                                    // +0x4a
    bool ToggleExpressionMenu;                           // +0x4b
    bool ToggleExpressionShift;                          // +0x4c
    bool FlourishNeedsAttackButtonHeld;                  // +0x4d
};
void CControlsDef::Transfer(CPersistContext& context)
{
    context.TransferVectorBinary(Controls);
    context.TransferBool(ToggleZTarget);
    context.TransferBool(ToggleSpells);
    context.TransferBool(ToggleSneak);
    context.TransferBool(ToggleExpressionMenu);
    context.TransferBool(ToggleExpressionShift);
    context.TransferBool(FlourishNeedsAttackButtonHeld);
}