// NUISystem::CSpellContainerList::GetDataFromDef() -- read the list's layout from its def: the position offset (into both PositionOffsetToUse and PositionOffset), the alpha step (byte / 255 into AlphaOffsetToUse and AlphaOffset) and the input delay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00544480, length 145 bytes. Callees: the component's virtual slot 108 (+0x1b0, GetDef
// by hidden pointer), the def's virtual slot 1 when its count reaches zero. Float pool 0x01231724 = 1/255.
// Layout: Ego_r.pdb NUISystem::CSpellContainerList minus 0x30 (minus 0x34 after the 12-byte SpellContainers vector):
// PositionOffset +0x180, AlphaOffset +0x188, PositionOffsetToUse +0x198, AlphaOffsetToUse +0x1a0, InputDelay +0x1a4; the def
// (packed) holds the offset at +0x142, the alpha byte at +0x14a, the delay at +0x184.
// Shape: as 0x00545720 -- the CDefPointer dtor calls the def's inline ReleaseTemporary() (no null test) whose accessors sit on
// the ref-count base (+4); the alpha is the chained `AlphaOffset = AlphaOffsetToUse = ...` (`fst` then `fstp`).
// Leaf: GetDataFromDef
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
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59) V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69) V(70) V(71) V(72) V(73) V(74) V(75) V(76) V(77) V(78) V(79) V(80) V(81) V(82) V(83) V(84) V(85) V(86) V(87) V(88) V(89) V(90) V(91) V(92) V(93) V(94) V(95) V(96) V(97) V(98) V(99) V(100) V(101) V(102) V(103) V(104) V(105) V(106) V(107)
#undef V
    virtual CDefPointer GetDef();                // slot 108 (+0x1b0)
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
    float         NextSpellContainerAlpha;       // +0x18c
    C2DVector     NextSpellContainerPosition;    // +0x190
    C2DVector     PositionOffsetToUse;           // +0x198
    float         AlphaOffsetToUse;              // +0x1a0
    float         InputDelay;                    // +0x1a4
    void GetDataFromDef();
};

void CSpellContainerList::GetDataFromDef()
{
    CDefPointer def = GetDef();
    PositionOffsetToUse = def->PositionOffset;
    PositionOffset = def->PositionOffset;
    AlphaOffset = AlphaOffsetToUse = (float)def->Alpha * (1.0f / 255.0f);
    InputDelay = def->InputDelay;
}

}