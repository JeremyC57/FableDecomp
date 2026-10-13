#pragma optimize("s",on)
// CBase_0099b720_Derived_00493617::operator=(const CBase_0099b720_Derived_00493617&) -- CBase_0099b720::operator= @ 0x0099B720, then 4 members copied in declaration order
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00493617, oracle length 45 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,[esp+0Ch] ; push edi ; mov esi,ecx ; call CBase_0099b720::operator= ; mov eax,[edi+0x4] ; mov [esi+0x4],eax ; mov eax,[edi+0x8] ; mov [esi+0x8],eax ; mov eax,[edi+0xc] ; mov [esi+0xc],eax ; mov al,[edi+0x10] ; mov [esi+0x10],al ; pop edi ; mov eax,esi ; pop esi ; ret 4
// Copy-assignment of a class derived from CBase_0099b720 (whose own operator= @ 0x0099B720 is out of
// line): the base assignment runs first, then this class's 4 trailing members are copied in
// declaration order and `this` is returned. Member types are not recoverable beyond their sizes
// (`long` / `unsigned char`). The manifest label (`CThingBuildingDef::operator=`) is propagated;
// the class is named by its base. One of 14 bodies of this shape.
struct CBase_0099b720 {
    unsigned char _body[0x4];
    CBase_0099b720& operator=(const CBase_0099b720& other);   // 0x0099B720
};

struct CBase_0099b720_Derived_00493617 : CBase_0099b720 {
    long m_04;                          // +0x4
    long m_08;                          // +0x8
    long m_0c;                          // +0xc
    unsigned char m_10;                 // +0x10
    CBase_0099b720_Derived_00493617& operator=(const CBase_0099b720_Derived_00493617& other);
};

CBase_0099b720_Derived_00493617& CBase_0099b720_Derived_00493617::operator=(const CBase_0099b720_Derived_00493617& other)
{
    CBase_0099b720::operator=(other);
    m_04 = other.m_04;
    m_08 = other.m_08;
    m_0c = other.m_0c;
    m_10 = other.m_10;
    return *this;
}