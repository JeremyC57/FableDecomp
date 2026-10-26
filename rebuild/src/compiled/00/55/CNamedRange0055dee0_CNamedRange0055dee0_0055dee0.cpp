// CNamedRange_0055dee0::CNamedRange_0055dee0() -- default constructor of a plain 0x14-byte record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055DEE0, length 42 bytes.
// Callees: CWideString::CWideString() @ 0x0099AED0 (members +0x0 and +0x8; the counterpart of
// the destructor @ 0x0099B510), CCharString::CCharString() @ 0x0099E4B0 (member +0xc).
// No vtable, no base: five members initialised in declaration order -- wide string, long -1,
// wide string, char string, long -1 (retail stores each -1 right after the preceding
// constructor call, i.e. initialiser order, not body assignments). The manifest label
// (CAIStateGroup_BalverineCornered ctor) is propagated garbage; the class is named by shape.
class CWideString { public:
    void* m_pData;
    CWideString();                          // 0x0099AED0
};

class CCharString { public:
    char* m_pData;
    CCharString();                          // 0x0099E4B0
};

struct CNamedRange_0055dee0 {
    CWideString m_firstName;                // +0x00
    long        m_firstIndex;               // +0x04
    CWideString m_secondName;               // +0x08
    CCharString m_tag;                      // +0x0c
    long        m_secondIndex;              // +0x10
    CNamedRange_0055dee0();
};

CNamedRange_0055dee0::CNamedRange_0055dee0()
    : m_firstName()
    , m_firstIndex(-1)
    , m_secondName()
    , m_tag()
    , m_secondIndex(-1)
{
}