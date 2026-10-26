// NUISystem::CSpellContainerList::Initialise() -- base Initialise, record the input time, reset the spell index, start with the middle spell container selected and step (virtual slot 151) until the selection is the last container
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00544290, length 120 bytes. Slot 43 of vtable 0x012475C4 (NUISystem::CSpellContainerList).
// Callees: CChangingStateComponent::Initialise @ 0x0052C730 (qualified), the list's virtual slot 151 (+0x25c, the select-next step).
// Layout: as 0x00544480 (Ego_r.pdb minus 0x30): LastInputTime +0x15c, SpellIndex +0x160, SelectedChild +0x164, SpellContainers +0x170;
// the base's current-time float read from +0x30.
// Shape: SelectedChild is zeroed before SpellIndex and then overwritten with size()/2 (retail keeps the dead store); size() is the
// STLport `end() - begin()` (signed `sar`, then the unsigned `/ 2` as `shr`), re-read for the loop test every pass.
// Leaf: Initialise
namespace NUISystem {

class CComponent;
template <class T> struct CRetailVector {        // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned long size() const { return end() - begin(); }
};

class CChangingStateComponent { public:                 // retail 0x15C bytes
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59) V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69) V(70) V(71) V(72) V(73) V(74) V(75) V(76) V(77) V(78) V(79) V(80) V(81) V(82) V(83) V(84) V(85) V(86) V(87) V(88) V(89) V(90) V(91) V(92) V(93) V(94) V(95) V(96) V(97) V(98) V(99) V(100) V(101) V(102) V(103) V(104) V(105) V(106) V(107) V(108) V(109) V(110) V(111) V(112) V(113) V(114) V(115) V(116) V(117) V(118) V(119) V(120) V(121) V(122) V(123) V(124) V(125) V(126) V(127) V(128) V(129) V(130) V(131) V(132) V(133) V(134) V(135) V(136) V(137) V(138) V(139) V(140) V(141) V(142) V(143) V(144) V(145) V(146) V(147) V(148) V(149) V(150)
#undef V
    virtual void SelectNextSpell();              // slot 151 (+0x25c)
    unsigned char _pad04[0x2c];
    float         m_currentTime;                 // +0x30
    unsigned char _pad34[0x128];
    void Initialise();                           // 0x0052C730
};

class CSpellContainerList : public CChangingStateComponent { public:
    float         LastInputTime;                 // +0x15c
    unsigned long SpellIndex;                    // +0x160
    unsigned long SelectedChild;                 // +0x164
    CComponent*   SpellSelectionChild;           // +0x168
    CComponent*   SpellIndicationChild;          // +0x16c
    CRetailVector<CComponent*> SpellContainers;  // +0x170
    void Initialise();
};

void CSpellContainerList::Initialise()
{
    CChangingStateComponent::Initialise();
    LastInputTime = m_currentTime;
    SelectedChild = 0;
    SpellIndex = 0;
    SelectedChild = SpellContainers.size() / 2;
    while (SelectedChild != SpellContainers.size() - 1)
        SelectNextSpell();
}

}