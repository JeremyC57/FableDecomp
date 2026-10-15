// CTCRockTrollShield::vfunc_9() -- counts the recharge counter down; when it reaches zero the shield is back to 5 hits
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AED80, length 28 bytes (vtable 0x0126D41C slot 9, rtti-vtables.tsv).
// Callees: none.
// Shape: `mov eax,[ecx+10h] ; test eax,eax ; je ; dec eax ; mov [ecx+10h],eax ; jne ; mov [ecx+0Ch],5 ; mov [ecx+10h],0` --
// a non-zero test, a pre-decrement tested for zero, then the state store and an explicit (redundant) reset of the counter.
// Layout = Ego_r.pdb CTCRockTrollShield (0x14): CTCBase 0xc, health +0x0c, rechargeCounter +0x10 (as the landed constructor
// 0x007AEE10 initialises them: health 5, rechargeCounter 0). No engine header; modelled locally.
struct CTCRockTrollShield {
    virtual void v00();
    unsigned char _base[0x8];                  // +0x04 CTCBase body
    long health;                               // +0x0c
    long rechargeCounter;                      // +0x10
    void vfunc_9();
};

void CTCRockTrollShield::vfunc_9()
{
    if (rechargeCounter != 0) {
        if (--rechargeCounter == 0) {
            health = 5;
            rechargeCounter = 0;
        }
    }
}