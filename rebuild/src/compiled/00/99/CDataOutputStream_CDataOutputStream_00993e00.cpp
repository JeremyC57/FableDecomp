// CDataOutputStream::CDataOutputStream() -- CBase(), own vtable, then the NewLine string member built from the "\r\n" literal
// Leaf: ??0CDataOutputStream@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00993E00, length 33 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal 0x012397F8 "\r\n", length -1).
// Vtable 0x0129A6E0 = CDataOutputStream (rtti-vtables.tsv). Shape: the vtable store sits between the two pushes and
// the member constructor call (VC stores the vtable before the member initialisers); `NewLine("\r\n")` is the
// implicit conversion through the (const char*, long = -1) constructor, hence `push -1 ; push offset`.
// Layout from engine/CDataOutputStream.h (PDB): vptr +0, NewLine +4.
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                   // 0x0099A2F0
};

class CCharString { public:
    void* m_pStringData;                       // +0x00 CCharString::PStringData
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
};

class CDataOutputStream : public CBaseClassNonCopyable { public:             // vtable 0x0129A6E0
    CCharString NewLine;                       // +0x04
    CDataOutputStream();
};

CDataOutputStream::CDataOutputStream()
    : CBaseClassNonCopyable(), NewLine("\r\n")
{
}