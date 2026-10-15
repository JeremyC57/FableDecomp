// CSplineData_00b20b60::CopyFrom(const CSplineData_00b20b60* other) -- copies the two flag bytes (+0x2d, then +0x2c) and assigns the two key vectors at +0x30 / +0x3c out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B20B60, length 49 bytes.
// Leaf: CopyFrom
// Callees: vector<...>::operator= @ 0x0075B8D0 (x2, `__thiscall`, source by address).
// The manifest label "CSimVoice::operator=" is propagated: the body returns nothing (`ret 4` with no `mov eax,esi`), its
// only caller (0x00B22467, in the CPSCSplineBase / CSplinePosParamData code) passes a pointer loaded from +0x24 of
// the source object, and the copied members (+0x2c..+0x47) are not CSimVoice's. Named by address and effect.
// Shape: the bytes copied in source order (+0x2d first) through al / cl, each vector's source formed with `lea`,
// the second by advancing the source register (`add edi,3Ch`).
struct CKeyVector {                             // VC6-era vector (12 bytes)
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CKeyVector& operator=(const CKeyVector& other);   // 0x0075B8D0
};

struct CSplineData_00b20b60 {
    unsigned char _pad00[0x2c];
    bool          m_flagA;                      // +0x2c
    bool          m_flagB;                      // +0x2d
    unsigned char _pad2e[0x2];
    CKeyVector    m_keysA;                      // +0x30
    CKeyVector    m_keysB;                      // +0x3c
    void CopyFrom(const CSplineData_00b20b60* other);
};

void CSplineData_00b20b60::CopyFrom(const CSplineData_00b20b60* other)
{
    m_flagB = other->m_flagB;
    m_flagA = other->m_flagA;
    m_keysA = other->m_keysA;
    m_keysB = other->m_keysB;
}
