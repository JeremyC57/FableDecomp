#pragma optimize("s",on)
// CClass_005b1fbc::CClass_005b1fbc() -- constructor: the base is built out of line (@ 0x0099AED0), then the member at +0x4 is zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B1FBC, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push esi ; mov esi, ecx ; call 0x99aed0 ; and dword ptr [esi + 4], 0 ; mov eax, esi ; pop esi ; ret 
// Tiny-body shape `CtorBaseThenZero`: a derived constructor over an out-of-line base constructor
// followed by one zeroed member (`and [esi+N], 0`). The class comes from the manifest label.
struct CBase_005b1fbc {
    unsigned char _body[0x4];
    CBase_005b1fbc();                             // 0x0099AED0
};

struct CClass_005b1fbc : CBase_005b1fbc {
    long m_value;                          // +0x4
    CClass_005b1fbc();
};

CClass_005b1fbc::CClass_005b1fbc()
    : m_value(0)
{
}