// CTCWeaponTrail::OnSetInLimbo(bool) -- calls the member method @ 0x0076E150 when the flag argument is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076E260, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov al, byte ptr [esp + 4] ; test al, al ; je 0x76e26d ; call 0x76e150 ; ret 4
// Tiny-body shape `UpdateIf` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CTCWeaponTrail { public:
    void OnSetInLimbo(bool enable);
public:
    void ClearAllWeaponTrails();   // 0x0076E150
};

void CTCWeaponTrail::OnSetInLimbo(bool enable)
{
    if (enable)
        ClearAllWeaponTrails();
}