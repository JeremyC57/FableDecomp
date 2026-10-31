// PKCPlaybackTemplate::~PKCPlaybackTemplate() -- libpk destructor: delete every entry the first list still hands out (DeleteData), delete the owned controller (+0x0c) and state (+0x08) clearing each pointer, then the members die in reverse order (three flag patches, the two lists) and the empty base
// Grade: DIFFER(248v248) -- scheduling only: retail forms `lea ebx,[esi+40h]` (HolderB, the next object destroyed) after the +0x90 / +0x84 patch vtable stores, ours before all three; the zero-store residue of the earlier attempts is gone with the real member model
// Leaf: ??1PKCPlaybackTemplate@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C14F90, 248 bytes (+ int3 padding), plain `ret`; called only by its scalar
//   deleting destructor 0x00C15A10 (vtable 0x012B29F8 + 0x44).
// Identity (libpk, no PDB/RTTI): the catalog label "CTCDiceGame::~CTCDiceGame" is propagated (Ego_r.pdb's CTCDiceGame is
//   24 bytes; this object is 0xA4). The layout is the one PKCPlaybackTemplate::ApplyTo 0x00C14A80 (same TU, two functions
//   up) reads: three 12-byte flag patches at +0x78/+0x84/+0x90 and the range words at +0x9C/+0xA0, which the constructor
//   0x00C14E40 zeroes. The constructor (landed as CTCDiceGame), DeleteData 0x00C14A50 (landed as CDataOwner_00c14a50) and
//   the slot-1/2 setters 0x00C14B50/0x00C14BC0 (landed as CSettingsProxy_012b29f8) are the same class: NAME_FIXES.tsv.
// Callees: the list's GetHead (slot 11, +0x2c), DeleteData @ 0x00C14A50, ~CDiceController @ 0x00C2BE10 and
//   ~CDiceGameState @ 0x00C2A9B0 with operator delete @ 0x00BFE9BC, ~CPKListImpl @ 0x00C19250 (each list's +8 member).
// Layout (from the constructor 0x00C14E40 and the stores here): +0x04 parent, +0x08 state, +0x0c controller, lists
//   +0x10 (vtable 0x012B29B0) / +0x40 (0x012B28D8) over the pure CPKList base 0x012B1AB0 (impl pointer +4, impl +8),
//   seed pair +0x70 (trivial), flag patches +0x78/+0x84/+0x90 (CFlagOverride 0x012B4630 over base 0x012B2828), +0x9C/+0xA0.
// Shape: /EHsc destructor (EH state 5 = six destructible subobjects: base, two lists, three patches). Raw-pointer
//   `delete p; p = 0;` with the real member classes schedules the +0x0c zero store after the next load/test like retail.
//   The flag patch's class declares no destructor (compiler-generated: its own vtable store dropped), its base an inline
//   virtual one (the 0x012B2828 store). The lists' user dtors store their vtables, then ~CPKListImpl, then CPKList's.
void operator delete(void* p) throw();
class CThing;
class CData_00c14a50;
class CPKListImpl {
public:
    ~CPKListImpl();                         // 0x00C19250
    unsigned char m_body[0x28];
};
class CPKList {                             // vtable 0x012B1AB0
public:
    virtual long Compare(const void* a, const void* b) const = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual ~CPKList() { }
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void Remove(void* item) = 0;
    virtual void Add(void* item) = 0;
    virtual void v10() = 0;
    virtual CData_00c14a50* GetHead() = 0;
};
class CDiceRuleHolderA : public CPKList {
public:
    virtual ~CDiceRuleHolderA() { }
    virtual long Compare(const void* a, const void* b) const;
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(void* item); virtual void Add(void* item); virtual void v10(); virtual CData_00c14a50* GetHead();
    CPKListImpl* m_pImpl;
    CPKListImpl  m_impl;
};
class CDiceRuleHolderB : public CPKList {
public:
    virtual ~CDiceRuleHolderB() { }
    virtual long Compare(const void* a, const void* b) const;
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(void* item); virtual void Add(void* item); virtual void v10(); virtual CData_00c14a50* GetHead();
    CPKListImpl* m_pImpl;
    CPKListImpl  m_impl;
};
struct CDiceSeedPair { long A, B; };
class CFlagOverrideBase {                   // vtable 0x012B2828
public:
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual ~CFlagOverrideBase() { }
    virtual void v03() = 0;
};
class CFlagOverride : public CFlagOverrideBase {   // vtable 0x012B4630
public:
    virtual void v00(); virtual void v01(); virtual void v03();
    unsigned short m_maskB, m_valueB, m_maskA, m_valueA;
};
class CDiceGameState { public: ~CDiceGameState(); };    // 0x00C2A9B0 (+0x08 owned state)
class CDiceController { public: ~CDiceController(); };  // 0x00C2BE10
class CTCBaseShell {                        // vtable 0x012B27E0
public:
    virtual ~CTCBaseShell() { }
};
class PKCPlaybackTemplate : public CTCBaseShell {   // vtable 0x012B29F8
public:
    virtual ~PKCPlaybackTemplate();
    void DeleteData(CData_00c14a50* data);  // 0x00C14A50
    CThing*          ParentThing;
    CDiceGameState*  State;
    CDiceController* Controller;
    CDiceRuleHolderA HolderA;
    CDiceRuleHolderB HolderB;
    CDiceSeedPair    Seed;
    CFlagOverride    QuadA;
    CFlagOverride    QuadB;
    CFlagOverride    QuadC;
    long             Field9C;
    long             FieldA0;
};

PKCPlaybackTemplate::~PKCPlaybackTemplate()
{
    CData_00c14a50* data;
    while ((data = HolderA.GetHead()) != 0)
        DeleteData(data);
    delete Controller;
    Controller = 0;
    delete State;
    State = 0;
}
