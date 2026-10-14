// CWideStringQuintet_0042f7c1::~CWideStringQuintet_0042f7c1() -- implicit destructor of five CWideString members
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042F7C1, length 44 bytes.
// Callees: CWideString::~CWideString @ 0x0099B510 on +0x30, +0x2c, +0x28, +0x24 and +0x20
// (reverse declaration order, the last one tail-called). No vtable store, no base call: a
// non-polymorphic aggregate whose destructor is declared and defined out of line so it is the
// TU's symbol. The manifest label (CarryCrateAndFollow dtor) is propagated garbage; the class is
// named by shape.
struct CWideString {
    void* m_pData;
    ~CWideString();                         // 0x0099B510
};

struct CWideStringQuintet_0042f7c1 {
    unsigned char _pad[0x20];               // +0x00..+0x1f
    CWideString m_text0;                    // +0x20
    CWideString m_text1;                    // +0x24
    CWideString m_text2;                    // +0x28
    CWideString m_text3;                    // +0x2c
    CWideString m_text4;                    // +0x30
    ~CWideStringQuintet_0042f7c1();
};

CWideStringQuintet_0042f7c1::~CWideStringQuintet_0042f7c1()
{
}