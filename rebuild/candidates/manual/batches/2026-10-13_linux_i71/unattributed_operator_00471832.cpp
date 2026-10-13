// CBase_00471865_Derived_00471832::operator=(const CBase_00471865_Derived_00471832&) -- CBase_00471865::operator= @ 0x00471865, then 5 members copied in declaration order
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471832, oracle length 51 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,[esp+0Ch] ; push edi ; mov esi,ecx ; call CBase_00471865::operator= ; mov al,[edi+0xd] ; mov [esi+0xd],al ; mov eax,[edi+0x10] ; mov [esi+0x10],eax ; mov eax,[edi+0x14] ; mov [esi+0x14],eax ; mov eax,[edi+0x18] ; mov [esi+0x18],eax ; mov eax,[edi+0x1c] ; mov [esi+0x1c],eax ; pop edi ; mov eax,esi ; pop esi ; ret 4
// Copy-assignment of a class derived from CBase_00471865 (whose own operator= @ 0x00471865 is out of
// line): the base assignment runs first, then this class's 5 trailing members are copied in
// declaration order and `this` is returned. Member types are not recoverable beyond their sizes
// (`long` / `unsigned char`). The manifest label (`CThingBuildingDef::operator=`) is propagated;
// the class is named by its base. One of 11 bodies of this shape.
struct CBase_00471865 {
    unsigned char _body[0xd];
    CBase_00471865& operator=(const CBase_00471865& other);   // 0x00471865
};

struct CBase_00471865_Derived_00471832 : CBase_00471865 {
    unsigned char m_0d;                 // +0xd
    unsigned char _gap1[0x2];
    long m_10;                          // +0x10
    long m_14;                          // +0x14
    long m_18;                          // +0x18
    long m_1c;                          // +0x1c
    CBase_00471865_Derived_00471832& operator=(const CBase_00471865_Derived_00471832& other);
};

CBase_00471865_Derived_00471832& CBase_00471865_Derived_00471832::operator=(const CBase_00471865_Derived_00471832& other)
{
    CBase_00471865::operator=(other);
    m_0d = other.m_0d;
    m_10 = other.m_10;
    m_14 = other.m_14;
    m_18 = other.m_18;
    m_1c = other.m_1c;
    return *this;
}
