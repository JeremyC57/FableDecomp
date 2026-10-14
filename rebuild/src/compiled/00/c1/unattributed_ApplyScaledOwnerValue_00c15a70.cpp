// _unattributed::ApplyScaledOwnerValue_00c15a70() -- owner->target->Apply(m_scale * owner->def->value, m_index)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C15A70, oracle length 35 bytes.
// Callees: thiscall (float, unsigned short) @ 0x00C0B420 (manifest "Render_ClampAlphaValue", no trusted identity;
// it clamps its float argument against 0.0/1.0 after a lookup), called on the owner's +0x34 object.
// The manifest label "C3DMeshObject::GetBoundingRadius" is propagated and rejected: C3DMeshObject has
// no ushort at +0xC and the body is a forwarder, not a radius getter. Neighbours (0x00C15AA0
// CCreatureActionHeroJump::FrameUpdate, 0x00C15B00 CAnimateThing_HasAnimationTriggers) do not pin the class.
//
// Retail bytes:  mov eax,[ecx+4] ; xor edx,edx ; mov dx,[ecx+0Ch] ; push edx ; mov edx,[eax+38h] ; fld [edx+7C0h] ; push ecx ;
//                fmul [ecx+10h] ; mov ecx,[eax+34h] ; fstp [esp] ; call 0xC0B420 ; ret
// Shape: `xor edx,edx ; mov dx,[mem] ; push edx` is an unsigned short passed to an unsigned short
// parameter (pushed first = last argument); the float argument is `m_scale * value` (`fld a ; fmul b` is
// b*a) built in its pushed slot. The owner at +4 holds a def-like object at +0x38 whose float sits at
// +0x7C0 and the call target at +0x34. The callee cleans the stack (`ret` with no adjustment here).
struct CValueSource {
    unsigned char _pad[0x7c0];
    float         m_value;                 // +0x7C0
};

struct CTarget {
    void Apply(float value, unsigned short index);   // 0x00C0B420
};

struct COwner {
    unsigned char _pad[0x34];
    CTarget*      m_pTarget;               // +0x34
    CValueSource* m_pSource;               // +0x38
};

struct CUnattributed_00c15a70 {
    unsigned char  _pad_0x00[4];
    COwner*        m_pOwner;               // +0x04
    unsigned char  _pad_0x08[4];
    unsigned short m_index;                // +0x0C
    unsigned char  _pad_0x0e[2];
    float          m_scale;                // +0x10

    void ApplyScaledOwnerValue();
};

void CUnattributed_00c15a70::ApplyScaledOwnerValue()
{
    m_pOwner->m_pTarget->Apply(m_scale * m_pOwner->m_pSource->m_value, m_index);
}