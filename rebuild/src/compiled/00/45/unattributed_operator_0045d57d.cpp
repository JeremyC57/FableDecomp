#pragma optimize("s",on)
// CBase_00431ed0_Derived_0045d57d::operator=(const CBase_00431ed0_Derived_0045d57d&) -- CBase_00431ed0::operator= @ 0x00431ED0, then 4 members copied in declaration order
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045D57D, oracle length 45 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,[esp+0Ch] ; push edi ; mov esi,ecx ; call CBase_00431ed0::operator= ; mov eax,[edi+0x3c] ; mov [esi+0x3c],eax ; mov eax,[edi+0x40] ; mov [esi+0x40],eax ; mov eax,[edi+0x44] ; mov [esi+0x44],eax ; mov eax,[edi+0x48] ; mov [esi+0x48],eax ; pop edi ; mov eax,esi ; pop esi ; ret 4
// Copy-assignment of a class derived from CBase_00431ed0 (whose own operator= @ 0x00431ED0 is out of
// line): the base assignment runs first, then this class's 4 trailing members are copied in
// declaration order and `this` is returned. Member types are not recoverable beyond their sizes
// (`long` / `unsigned char`). The manifest label (`CThingBuildingDef::operator=`) is propagated;
// the class is named by its base. One of 14 bodies of this shape.
struct CBase_00431ed0 {
    unsigned char _body[0x3c];
    CBase_00431ed0& operator=(const CBase_00431ed0& other);   // 0x00431ED0
};

struct CBase_00431ed0_Derived_0045d57d : CBase_00431ed0 {
    long m_3c;                          // +0x3c
    long m_40;                          // +0x40
    long m_44;                          // +0x44
    long m_48;                          // +0x48
    CBase_00431ed0_Derived_0045d57d& operator=(const CBase_00431ed0_Derived_0045d57d& other);
};

CBase_00431ed0_Derived_0045d57d& CBase_00431ed0_Derived_0045d57d::operator=(const CBase_00431ed0_Derived_0045d57d& other)
{
    CBase_00431ed0::operator=(other);
    m_3c = other.m_3c;
    m_40 = other.m_40;
    m_44 = other.m_44;
    m_48 = other.m_48;
    return *this;
}