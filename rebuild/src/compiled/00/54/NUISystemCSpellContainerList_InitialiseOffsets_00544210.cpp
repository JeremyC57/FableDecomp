// NUISystem::CSpellContainerList::InitialiseOffsets(unsigned long count) -- centre the spell containers: half = count/2 (odd) or count/2 - 1 (even); position = {PositionOffset.X, -half * PositionOffset.Y}, alpha = 1 - half * AlphaOffset, through virtual slots 149 (SetAlpha) and 150 (SetPosition)
// Retry 10b: the alpha is a const REFERENCE to its float temporary (`const float& alpha = 1.0f - half * AlphaOffset;`, B-12 sweep), so it is homed in the dead `count` slot and pushed through eax as in retail; `half` is converted in each if/else arm (the two `mov [count],eax ; fild` pairs).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00544210, length 124 bytes (`ret 4`), slot 148 of vtable 0x012475C4 (NUISystem::CSpellContainerList).
// Float pool: 0x0122DCB4 = 2^32 (unsigned-to-float fix-up), 0x0122DED8 = 1.0f. Layout as 0x00544480 (PositionOffset +0x180, AlphaOffset +0x188).
// Leaf: InitialiseOffsets
namespace NUISystem {

struct C2DVector { float X, Y; };

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                      // slot 1
};
#pragma pack(push, 1)
struct CSpellContainerListDef : CDefVirtual, CRefCounted {
    unsigned char _def[0x13a];                   // +0x08
    C2DVector     PositionOffset;                // +0x142 (packed def)
    unsigned char Alpha;                         // +0x14a (0..255)
    unsigned char _def2[0x39];
    float         InputDelay;                    // +0x184
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
#pragma pack(pop)
struct CDefPointer {
    CSpellContainerListDef* m_p;
    ~CDefPointer() { m_p->ReleaseTemporary(); }
    CSpellContainerListDef* operator->() const { return m_p; }
};

struct CComponent;
template <class T> struct CRetailVector { T* m_start; T* m_finish; T* m_end; };

struct CSpellContainerList {                     // vtable 0x012475C4 (primary)
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59) V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69) V(70) V(71) V(72) V(73) V(74) V(75) V(76) V(77) V(78) V(79) V(80) V(81) V(82) V(83) V(84) V(85) V(86) V(87) V(88) V(89) V(90) V(91) V(92) V(93) V(94) V(95) V(96) V(97) V(98) V(99) V(100) V(101) V(102) V(103) V(104) V(105) V(106) V(107) V(108) V(109) V(110) V(111) V(112) V(113) V(114) V(115) V(116) V(117) V(118) V(119) V(120) V(121) V(122) V(123) V(124) V(125) V(126) V(127) V(128) V(129) V(130) V(131) V(132) V(133) V(134) V(135) V(136) V(137) V(138) V(139) V(140) V(141) V(142) V(143) V(144) V(145) V(146) V(147) V(148)
#undef V
    virtual void SetAlpha(float alpha);          // slot 149 (+0x254)
    virtual void SetPosition(const C2DVector& position);   // slot 150 (+0x258)
    unsigned char _component[0x158];             // +0x004 (retail component base 0x15C)
    float         LastInputTime;                 // +0x15c
    unsigned long SpellIndex;                    // +0x160
    unsigned long SelectedChild;                 // +0x164
    CComponent*   SpellSelectionChild;           // +0x168
    CComponent*   SpellIndicationChild;          // +0x16c
    CRetailVector<CComponent*> SpellContainers;  // +0x170
    long          SpellToAssign;                 // +0x17c
    C2DVector     PositionOffset;                // +0x180
    float         AlphaOffset;                   // +0x188
    void InitialiseOffsets(unsigned long count);
};

void CSpellContainerList::InitialiseOffsets(unsigned long count)
{
    C2DVector position;
    position.X = PositionOffset.X;
    float half;
    if (count & 1) {
        count = count >> 1;
        half = (float)count;
    } else {
        count = (count >> 1) - 1;
        half = (float)count;
    }
    position.Y = -(half * PositionOffset.Y);
    const float& alpha = 1.0f - half * AlphaOffset;
    SetAlpha(alpha);
    SetPosition(position);
}

} // namespace NUISystem