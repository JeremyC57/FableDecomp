// <unattributed>::PerformAction (mode 16) -- Lookup_00692930(16, 0)->Method_006345c0()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008424D0, oracle length 17 bytes.
// Grade: candidate -- still DIFFER under the Linux lane (Wine + DDK 13.10.4035 cl.exe), see batch README.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  push 0 ; push 16 ; call Lookup_00692930 ; mov ecx,eax ; call Method_006345c0 ; ret
// A two-argument __stdcall lookup (landed @ 0x00692930 under a propagated template label) returns an
// object on which a no-argument __thiscall method @ 0x006345C0 is invoked. `this` is unused.
// Sibling 0x008424D0 does the same with mode 0x10.
struct CLookedUp {
    void Method_006345c0();                 // 0x006345C0
};

extern CLookedUp* __stdcall Lookup_00692930(long mode, long flags);   // 0x00692930

struct CActionLike_Retail {
    void PerformAction_Mode10();
};

void CActionLike_Retail::PerformAction_Mode10()
{
    Lookup_00692930(16, 0)->Method_006345c0();
}
