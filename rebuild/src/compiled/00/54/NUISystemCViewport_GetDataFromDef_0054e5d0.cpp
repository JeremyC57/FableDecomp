// NUISystem::CViewport::GetDataFromDef() -- fetch the component def (virtual GetDef, CDefPointer by value) and cache its first record's +0x3c word at +0x170 (0 when the def's record vector is empty); release the def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054E5D0, length 106 bytes (padded to 112).
// Leaf: GetDataFromDef
// Callees: virtual GetDef() at vtable slot 108 (+0x1b0, hidden CDefPointer result in the `push ecx` frame word);
// the def's Destroy at its slot 1.
// Identity: the catalog label "CParticleEffect::GetDataFromDef" is a byte-twin propagation (+0x170 instead of +0x160);
// the only callers are 0x0054EA54 (CViewport ctor 0x0054EA00) and 0x0054EAB4, so this is the NUISystem::CViewport copy.
// The def type is opaque: a CDefBase-style object (vptr, count at +4) holding a vector of 124-byte records at +0x40.
// Shape: the record count is `size()` over STLport begin()/end() (the /124 magic multiply) and the record is read with
// STLport `operator[]` = `*(begin() + n)`, which re-reads begin (`mov ecx,[esi+40h]`) instead of reusing the
// subtraction's register; the CDefPointer temporary releases through the ref-count base's two inline accessors
// (`lea eax,[esi+4] ; ... dec esi ; mov edx,esi ; test edx,edx ; mov [eax],esi`).
struct CRefCounted {                        // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                 // slot 1
};
struct SDefRecord {                         // 124-byte record
    unsigned char _r[0x3c];
    long          m_value;                  // +0x3c
    unsigned char _r2[0x3c];
};
struct CRecordVector {                      // retail STLport vector (12 bytes)
    SDefRecord* _M_start;
    SDefRecord* _M_finish;
    SDefRecord* _M_end_of_storage;
    SDefRecord* begin() const { return _M_start; }
    SDefRecord* end() const { return _M_finish; }
    unsigned long size() const { return end() - begin(); }
    const SDefRecord& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CUIDef : CDefVirtual, CRefCounted {
    unsigned char _def[0x38];               // +0x08
    CRecordVector Records;                  // +0x40
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CUIDef* m_p;
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CUIDef* operator->() const { return m_p; }
};
namespace NUISystem {
struct CViewport {
    virtual void v00();
    unsigned char _component[0x16c];        // +0x004
    long          m_defValue;               // +0x170
#define V(n) virtual void v##n();
    V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59)
    V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69) V(70) V(71) V(72) V(73) V(74) V(75) V(76) V(77) V(78) V(79)
    V(80) V(81) V(82) V(83) V(84) V(85) V(86) V(87) V(88) V(89) V(90) V(91) V(92) V(93) V(94) V(95) V(96) V(97) V(98) V(99)
    V(100) V(101) V(102) V(103) V(104) V(105) V(106) V(107)
#undef V
    virtual CDefPointer GetDef();           // slot 108 (+0x1b0), result by hidden pointer
    void GetDataFromDef();
};
void CViewport::GetDataFromDef()
{
    CDefPointer def = GetDef();
    if (def->Records.size())
        m_defValue = def->Records[0].m_value;
    else
        m_defValue = 0;
}
}