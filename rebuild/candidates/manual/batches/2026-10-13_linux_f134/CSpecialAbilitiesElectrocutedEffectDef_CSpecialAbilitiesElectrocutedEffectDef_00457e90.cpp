// CSpecialAbilitiesElectrocutedEffectDef::CSpecialAbilitiesElectrocutedEffectDef() -- base CBase_00430370(), this vtable, then 6 members zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00457E90, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_00430370::CBase_00430370 ; xor eax,eax ; mov dword ptr [esi],offset CSpecialAbilitiesElectrocutedEffectDef::`vftable' ; mov [esi+0x48],eax ; mov [esi+0x4c],eax ; mov [esi+0x50],eax ; mov [esi+0x54],eax ; mov [esi+0x58],eax ; mov [esi+0x5c],eax ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_00430370() @ 0x00430370, this class's vtable (0x01234744, named
// through rtti-vtables.tsv), then the 6 consecutive dword members at +0x48..+0x5c
// are zeroed from one cleared register. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of 9 bodies.
struct CBase_00430370 {
    virtual void v00();
    CBase_00430370();                           // 0x00430370
};

struct SZeroed {                        // the zeroed block is one embedded member object
    long m_00;                          // +0x48
    long m_01;                          // +0x4c
    long m_02;                          // +0x50
    long m_03;                          // +0x54
    long m_04;                          // +0x58
    long m_05;                          // +0x5c
    SZeroed() : m_00(0), m_01(0), m_02(0), m_03(0), m_04(0), m_05(0) { }           // inlined into every constructor that embeds it
};

struct CSpecialAbilitiesElectrocutedEffectDef : CBase_00430370 {
    unsigned char _pad[0x44];
    SZeroed m_data;                        // +0x48
    virtual void v00();
    CSpecialAbilitiesElectrocutedEffectDef();
};

CSpecialAbilitiesElectrocutedEffectDef::CSpecialAbilitiesElectrocutedEffectDef()
    : CBase_00430370()
    , m_data()
{
}
