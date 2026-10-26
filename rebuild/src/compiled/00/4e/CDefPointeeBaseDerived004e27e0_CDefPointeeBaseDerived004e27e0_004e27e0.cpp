// CDefPointeeBase_Derived_004e27e0::~CDefPointeeBase_Derived_004e27e0() -- destroy the three container members at +0x44, +0x38, +0x2c, then the inlined CDefPointeeBase destructor stores its vtable and tail-calls the CMemoryBlock base destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E27E0, length 41 bytes.
// Callees: CContainer_004dd42f::~CContainer_004dd42f @ 0x004DD42F (x2: +0x44, +0x2c),
// CContainer_004dd2b1::~CContainer_004dd2b1 @ 0x004DD2B1 (+0x38) -- both destroy their elements
// and free the buffer (manifest labels "CActiveFile::OnReadFinished" are propagated garbage);
// CBase_009fc550::~CBase_009fc550 @ 0x009FC550 tail-called. Vtable 0x01230BA0 = CDefPointeeBase.
// The manifest label "~CBossDef" is propagated (the PDB CBossDef has its vectors at +0x28/+0x38/
// +0x48, not +0x2c/+0x38/+0x44); the derived class is named by its address. Same shape as the
// landed 0x004D9861: `__declspec(novtable)` derived class, members destroyed in reverse order.
struct CContainer_004dd42f {
    unsigned char _body[0x0c];
    ~CContainer_004dd42f();                     // 0x004DD42F
};

struct CContainer_004dd2b1 {
    unsigned char _body[0x0c];
    ~CContainer_004dd2b1();                     // 0x004DD2B1
};

struct CBase_009fc550 {
    virtual void v00();
    ~CBase_009fc550();                          // 0x009FC550
};

class CDefPointeeBase : public CBase_009fc550 { public:       // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }                      // inline: inlined into every derived destructor
};

struct __declspec(novtable) CDefPointeeBase_Derived_004e27e0 : CDefPointeeBase {
    unsigned char        _pad0[0x28];           // +0x04..+0x2b
    CContainer_004dd42f  m_items0;              // +0x2c
    CContainer_004dd2b1  m_items1;              // +0x38
    CContainer_004dd42f  m_items2;              // +0x44
    virtual void v00();
    ~CDefPointeeBase_Derived_004e27e0();
};

CDefPointeeBase_Derived_004e27e0::~CDefPointeeBase_Derived_004e27e0()
{
}