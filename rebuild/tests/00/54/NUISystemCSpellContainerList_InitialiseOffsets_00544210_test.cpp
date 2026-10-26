// Behaviour test for 00544210: src/00544210.cpp copied verbatim; SetAlpha/SetPosition defined to record their
// arguments so the centring offsets for odd and even counts are checked.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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

class CComponent;
template <class T> struct CRetailVector { T* m_start; T* m_finish; T* m_end; };

class CSpellContainerList { public:                     // vtable 0x012475C4 (primary)
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
// ---- end source ----
namespace NUISystem {
void CSpellContainerList::v00() {} void CSpellContainerList::v01() {} void CSpellContainerList::v02() {} void CSpellContainerList::v03() {} void CSpellContainerList::v04() {} void CSpellContainerList::v05() {} void CSpellContainerList::v06() {} void CSpellContainerList::v07() {} void CSpellContainerList::v08() {} void CSpellContainerList::v09() {} void CSpellContainerList::v10() {} void CSpellContainerList::v11() {} void CSpellContainerList::v12() {} void CSpellContainerList::v13() {} void CSpellContainerList::v14() {} void CSpellContainerList::v15() {} void CSpellContainerList::v16() {} void CSpellContainerList::v17() {} void CSpellContainerList::v18() {} void CSpellContainerList::v19() {} void CSpellContainerList::v20() {} void CSpellContainerList::v21() {} void CSpellContainerList::v22() {} void CSpellContainerList::v23() {} void CSpellContainerList::v24() {} void CSpellContainerList::v25() {} void CSpellContainerList::v26() {} void CSpellContainerList::v27() {} void CSpellContainerList::v28() {} void CSpellContainerList::v29() {} void CSpellContainerList::v30() {} void CSpellContainerList::v31() {} void CSpellContainerList::v32() {} void CSpellContainerList::v33() {} void CSpellContainerList::v34() {} void CSpellContainerList::v35() {} void CSpellContainerList::v36() {} void CSpellContainerList::v37() {} void CSpellContainerList::v38() {} void CSpellContainerList::v39() {} void CSpellContainerList::v40() {} void CSpellContainerList::v41() {} void CSpellContainerList::v42() {} void CSpellContainerList::v43() {} void CSpellContainerList::v44() {} void CSpellContainerList::v45() {} void CSpellContainerList::v46() {} void CSpellContainerList::v47() {} void CSpellContainerList::v48() {} void CSpellContainerList::v49() {} void CSpellContainerList::v50() {} void CSpellContainerList::v51() {} void CSpellContainerList::v52() {} void CSpellContainerList::v53() {} void CSpellContainerList::v54() {} void CSpellContainerList::v55() {} void CSpellContainerList::v56() {} void CSpellContainerList::v57() {} void CSpellContainerList::v58() {} void CSpellContainerList::v59() {} void CSpellContainerList::v60() {} void CSpellContainerList::v61() {} void CSpellContainerList::v62() {} void CSpellContainerList::v63() {} void CSpellContainerList::v64() {} void CSpellContainerList::v65() {} void CSpellContainerList::v66() {} void CSpellContainerList::v67() {} void CSpellContainerList::v68() {} void CSpellContainerList::v69() {} void CSpellContainerList::v70() {} void CSpellContainerList::v71() {} void CSpellContainerList::v72() {} void CSpellContainerList::v73() {} void CSpellContainerList::v74() {} void CSpellContainerList::v75() {} void CSpellContainerList::v76() {} void CSpellContainerList::v77() {} void CSpellContainerList::v78() {} void CSpellContainerList::v79() {} void CSpellContainerList::v80() {} void CSpellContainerList::v81() {} void CSpellContainerList::v82() {} void CSpellContainerList::v83() {} void CSpellContainerList::v84() {} void CSpellContainerList::v85() {} void CSpellContainerList::v86() {} void CSpellContainerList::v87() {} void CSpellContainerList::v88() {} void CSpellContainerList::v89() {} void CSpellContainerList::v90() {} void CSpellContainerList::v91() {} void CSpellContainerList::v92() {} void CSpellContainerList::v93() {} void CSpellContainerList::v94() {} void CSpellContainerList::v95() {} void CSpellContainerList::v96() {} void CSpellContainerList::v97() {} void CSpellContainerList::v98() {} void CSpellContainerList::v99() {} void CSpellContainerList::v100() {} void CSpellContainerList::v101() {} void CSpellContainerList::v102() {} void CSpellContainerList::v103() {} void CSpellContainerList::v104() {} void CSpellContainerList::v105() {} void CSpellContainerList::v106() {} void CSpellContainerList::v107() {} void CSpellContainerList::v108() {} void CSpellContainerList::v109() {} void CSpellContainerList::v110() {} void CSpellContainerList::v111() {} void CSpellContainerList::v112() {} void CSpellContainerList::v113() {} void CSpellContainerList::v114() {} void CSpellContainerList::v115() {} void CSpellContainerList::v116() {} void CSpellContainerList::v117() {} void CSpellContainerList::v118() {} void CSpellContainerList::v119() {} void CSpellContainerList::v120() {} void CSpellContainerList::v121() {} void CSpellContainerList::v122() {} void CSpellContainerList::v123() {} void CSpellContainerList::v124() {} void CSpellContainerList::v125() {} void CSpellContainerList::v126() {} void CSpellContainerList::v127() {} void CSpellContainerList::v128() {} void CSpellContainerList::v129() {} void CSpellContainerList::v130() {} void CSpellContainerList::v131() {} void CSpellContainerList::v132() {} void CSpellContainerList::v133() {} void CSpellContainerList::v134() {} void CSpellContainerList::v135() {} void CSpellContainerList::v136() {} void CSpellContainerList::v137() {} void CSpellContainerList::v138() {} void CSpellContainerList::v139() {} void CSpellContainerList::v140() {} void CSpellContainerList::v141() {} void CSpellContainerList::v142() {} void CSpellContainerList::v143() {} void CSpellContainerList::v144() {} void CSpellContainerList::v145() {} void CSpellContainerList::v146() {} void CSpellContainerList::v147() {} void CSpellContainerList::v148() {}
}
static float g_alpha = -1.0f; static NUISystem::C2DVector g_pos;
void NUISystem::CSpellContainerList::SetAlpha(float alpha) { g_alpha = alpha; }
void NUISystem::CSpellContainerList::SetPosition(const C2DVector& position) { g_pos = position; }
int main()
{
    NUISystem::CSpellContainerList list;
    list.PositionOffset.X = 4.0f; list.PositionOffset.Y = 10.0f; list.AlphaOffset = 0.25f;
    list.InitialiseOffsets(5);                           // odd: half = 2
    CHECK(g_pos.X == 4.0f && g_pos.Y == -20.0f && g_alpha == 0.5f);
    list.InitialiseOffsets(4);                           // even: half = 1
    CHECK(g_pos.X == 4.0f && g_pos.Y == -10.0f && g_alpha == 0.75f);
    list.InitialiseOffsets(2);                           // even: half = 0
    CHECK(g_pos.Y == 0.0f && g_alpha == 1.0f);
    if (fails == 0) printf("00544210_TEST PASS\n"); else printf("00544210_TEST FAIL\n");
    return fails;
}