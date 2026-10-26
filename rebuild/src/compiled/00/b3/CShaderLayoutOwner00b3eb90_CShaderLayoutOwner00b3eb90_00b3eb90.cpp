// CShaderLayoutOwner_00b3eb90::~CShaderLayoutOwner_00b3eb90() -- four CBaseClass-derived members, then the base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3EB90, length 55 bytes.
// Callees: CBaseClass::~CBaseClass @ 0x0099A2E0 (stores vtable 0x01231710, CBaseClass in
// rtti-vtables.tsv) on the 12-byte members at +0x170, +0x164, +0x158, +0x14c (reverse order),
// then the base destructor @ 0x0098D710 tail-called. No vtable stores anywhere: the derived
// class and the member class are both `__declspec(novtable)` (the members' inline destructors
// collapse to the out-of-line CBaseClass destructor). The manifest label (CarryCrateAndFollow
// dtor) is propagated garbage; names are descriptive.
class CBaseClass { public:
    virtual void v00();
    virtual ~CBaseClass();                          // 0x0099A2E0
};

struct __declspec(novtable) CLayoutEntry : CBaseClass {
    long m_a;                               // +4
    long m_b;                               // +8
};

struct CLayoutBase {
    virtual void v00();
    ~CLayoutBase();                         // 0x0098D710
    unsigned char _pad[0x148];              // +0x4..+0x14b
};

struct __declspec(novtable) CShaderLayoutOwner_00b3eb90 : CLayoutBase {
    CLayoutEntry m_entry0;                  // +0x14c
    CLayoutEntry m_entry1;                  // +0x158
    CLayoutEntry m_entry2;                  // +0x164
    CLayoutEntry m_entry3;                  // +0x170
    ~CShaderLayoutOwner_00b3eb90();
};

CShaderLayoutOwner_00b3eb90::~CShaderLayoutOwner_00b3eb90()
{
}