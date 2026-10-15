// CVolumeDerived_00ca7360::CVolumeDerived_00ca7360(a, b, c, d) -- ctor of a class derived from the landed CVolume_00ca61d0 (audio volume) plus two interfaces at +0xe0 / +0xe4: the base from the four arguments, this class's five vtables, +0xec / +0x120 / +0x124 zeroed, then the out-of-line initialiser
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA7360, length 103 bytes (`ret 10h`).
// Callees: CVolume_00ca61d0::CVolume_00ca61d0 @ 0x00CA61D0 (landed, 4 args), the initialiser @ 0x00CA52A0 on `this` (catalog
// "CAIStateGroup_MinionHasInformation::OnActivate" is a propagated name). Vtables 0x012BF5D8 (+0), 0x012BF598 (+0xc),
// 0x012BF584 (+0x10), 0x012BF560 (+0xe0), 0x012BF54C (+0xe4); none has RTTI, so the class is named by address.
// Label correction: the catalog's "CSpellContainerList::CSpellContainerList" is propagated.
// Shape: the two interface bases are `__declspec(novtable)` (no own vtable stores before the derived ones); one zero register for
// the three init-list words; no EH frame in retail (default flags).
// Leaf: ??0CVolumeDerived
struct CVolumeBase_00ca61d0 {              // +0
    virtual ~CVolumeBase_00ca61d0();
    long m_4;
    long m_8;
};
struct CVolumeSide1_00ca61d0 { virtual void s1(); };   // +0x0c
struct CVolumeSide2_00ca61d0 { virtual void s2(); };   // +0x10

struct CVolume_00ca61d0 : CVolumeBase_00ca61d0, CVolumeSide1_00ca61d0, CVolumeSide2_00ca61d0 {   // 0xe0 bytes (landed ctor 0x00CA61D0, dtor 0x00CA6370)
    unsigned char _pad14[0xe0 - 0x14];
    CVolume_00ca61d0(void* a, void* b, void* c, void* d);   // 0x00CA61D0
    virtual ~CVolume_00ca61d0();                            // 0x00CA6370
};

struct __declspec(novtable) CInterfaceE0_00ca7360 { virtual void e0(); };   // +0xe0
struct __declspec(novtable) CInterfaceE4_00ca7360 {                         // +0xe4
    virtual void e4();
    long m_e8;
};

struct CVolumeDerived_00ca7360 : CVolume_00ca61d0, CInterfaceE0_00ca7360, CInterfaceE4_00ca7360 {
    long          m_ec;                    // +0xec
    unsigned char _padf0[0x30];
    long          m_120;                   // +0x120
    long          m_124;                   // +0x124
    virtual ~CVolumeDerived_00ca7360();
    virtual void e0();
    virtual void e4();
    CVolumeDerived_00ca7360(void* a, void* b, void* c, void* d);
    void Initialise();                     // 0x00CA52A0
};

CVolumeDerived_00ca7360::CVolumeDerived_00ca7360(void* a, void* b, void* c, void* d)
    : CVolume_00ca61d0(a, b, c, d), m_ec(0), m_120(0), m_124(0)
{
    Initialise();
}
