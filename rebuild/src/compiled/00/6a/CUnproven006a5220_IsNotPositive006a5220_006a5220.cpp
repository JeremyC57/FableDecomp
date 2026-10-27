// CUnproven_006a5220::IsNotPositive_006a5220() const -- 1 when the signed word at +0 is zero or negative, else 0
// Leaf: ?IsNotPositive_006a5220@CUnproven_006a5220@@QBEJXZ
// Retail: Fable.exe @ 0x006A5220, 10 bytes: `mov edx,[ecx] ; xor eax,eax ; test edx,edx ; setle al ; ret`.
// Identity (descriptive): an unreferenced gap-scan COMDAT in the CThingPhysical translation unit (0x006A5260 SetMaxHealth,
// 0x006A5460 GetFirstCollisionBetweenPoints); every xref the scan reports is an "abs" false hit inside other instructions'
// bytes, so neither the owning class nor the member name is provable. The member is named after what it computes.
// Shape: the preloaded `xor eax,eax` before `setle al` is a long (not bool) result of `value <= 0`.
class CUnproven_006a5220 { public:                  // descriptive: owner unproven
    long Value;                                     // +0x00 signed
    long IsNotPositive_006a5220() const;
};

long CUnproven_006a5220::IsNotPositive_006a5220() const
{
    return Value <= 0;
}