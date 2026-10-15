// CTCInventoryStats::~CTCInventoryStats() -- kills the stats-screen mannequin if one exists, then the creature-kill map, the CBase interface and the inventory base are torn down
// Leaf: ??1CTCInventoryStats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CD2FE, 65 bytes.
// Callees: CThing::Kill(bool) @ 0x004C9B80, the CreaturesKilled map destructor @ 0x004518E1 (manifest label
//          "CActiveFile::OnReadFinished" is propagated), CTCInventoryBase::~CTCInventoryBase @ 0x005C00A0 (tail jump;
//          label "~CTCActionUseBuyOrSteal" propagated).
//
// Shape: the two derived vtable stores (primary 0x01254ADC, CBase-in-Stats 0x01254AC4 at +0x130) open the body with the
// pMannequin load hoisted above them; the user body is the null-tested Kill(false) (no reset); then the members in
// reverse order -- only CreaturesKilled (+0x134, the first Stats member: PDB +0x14C, a retail 12-byte map) has a
// destructor; then the secondary base's INLINE user destructor, which re-stores the plain
// NHeroInformationScreens::CBase vtable 0x0123A3E0 (a polymorphic class with `~CBase() {}` keeps that store,
// GOTCHAS wave 25); finally the primary base destructor as a tail jump. CBase is a bare vptr (its vtable has no
// destructor slot: slot 0 is the CWideString getter, slots 1/2 return the screen ids 10/9 for Stats, slot 3 a bool).
// Local model (no engine header for CTCInventoryStats / CTCInventoryBase).
struct CThing {
    void Kill(bool immediate);                // 0x004C9B80
};

struct CTCInventoryBase_Retail {              // primary base, retail size 0x130
    virtual ~CTCInventoryBase_Retail();       // 0x005C00A0
    unsigned char _pad_0x04[0x130 - 4];
};

namespace NHeroInformationScreens {
struct CBase {                                // secondary base at +0x130, vptr only
    virtual long vfunc_0();
    virtual long vfunc_1();
    virtual long vfunc_2();
    virtual unsigned char vfunc_3();
    ~CBase() {}
};
}

struct CCreatureKillMap {                     // map<unsigned long, long>, retail 12 bytes
    unsigned char _body[0xc];
    ~CCreatureKillMap();                      // 0x004518E1
};

struct CTCInventoryStats : CTCInventoryBase_Retail, NHeroInformationScreens::CBase {
    CCreatureKillMap CreaturesKilled;         // +0x134
    CThing* pMannequin;                       // +0x140
    bool UseMannequin;                        // +0x144
    virtual ~CTCInventoryStats();
};

CTCInventoryStats::~CTCInventoryStats()
{
    if (pMannequin)
        pMannequin->Kill(false);
}