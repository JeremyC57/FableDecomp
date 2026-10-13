// CBase_00431f10_Derived_0045403d::operator=(const CBase_00431f10_Derived_0045403d&) -- CBase_00431f10::operator= @ 0x00431F10, then 4 members copied in declaration order
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045403D, oracle length 45 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,[esp+0Ch] ; push edi ; mov esi,ecx ; call CBase_00431f10::operator= ; mov eax,[edi+0x28] ; mov [esi+0x28],eax ; mov eax,[edi+0x2c] ; mov [esi+0x2c],eax ; mov eax,[edi+0x30] ; mov [esi+0x30],eax ; mov eax,[edi+0x34] ; mov [esi+0x34],eax ; pop edi ; mov eax,esi ; pop esi ; ret 4
// Copy-assignment of a class derived from CBase_00431f10 (whose own operator= @ 0x00431F10 is out of
// line): the base assignment runs first, then this class's 4 trailing members are copied in
// declaration order and `this` is returned. Member types are not recoverable beyond their sizes
// (`long` / `unsigned char`). The manifest label (`CThingBuildingDef::operator=`) is propagated;
// the class is named by its base. One of 14 bodies of this shape.
struct CBase_00431f10 {
    unsigned char _body[0x28];
    CBase_00431f10& operator=(const CBase_00431f10& other);   // 0x00431F10
};

struct CBase_00431f10_Derived_0045403d : CBase_00431f10 {
    long m_28;                          // +0x28
    long m_2c;                          // +0x2c
    long m_30;                          // +0x30
    long m_34;                          // +0x34
    CBase_00431f10_Derived_0045403d& operator=(const CBase_00431f10_Derived_0045403d& other);
};

CBase_00431f10_Derived_0045403d& CBase_00431f10_Derived_0045403d::operator=(const CBase_00431f10_Derived_0045403d& other)
{
    CBase_00431f10::operator=(other);
    m_28 = other.m_28;
    m_2c = other.m_2c;
    m_30 = other.m_30;
    m_34 = other.m_34;
    return *this;
}
