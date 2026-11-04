// CTCOnHeroPush::IsFinished() const -- true when the two floats at +0x20 and +0x24 differ (catalogue label, class unknown)
// Vendor naming V2 (plan phase 1, one address, one name): 0x00C384E0 ?IsFinished@CAmbientSourceDef@@QBE_NXZ (was ?IsAngry@CFloatPair_00c384e0@@QBE_NXZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C384E0, length 42 bytes. The label is propagated: retail CTCOnHeroPush is
// 0x20 bytes, while this body reads +0x20/+0x24; its two callers (0x00C44E8B, 0x00C47322) call it on the object at
// [this+4] of the 0x00C4xxxx shot-definition bodies.
// Shape: an inline `long` equality (`fld [24h] ; fld [20h] ; fucompp ; test ah,44h ; jp` = `m_20 == m_24`, 1/0 in eax)
// negated into a bool with `xor ecx,ecx ; test eax,eax ; sete cl ; mov al,cl`, duplicated into both arms.
struct CAmbientSourceDef {
    unsigned char _pad00[0x20];
    float m_current;                                // +0x20
    float m_target;                                 // +0x24
    long IsAtTarget() const { if (m_current == m_target) return 1; return 0; }
    bool IsFinished() const;
};

bool CAmbientSourceDef::IsFinished() const
{
    return !IsAtTarget();
}