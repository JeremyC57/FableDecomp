// CFileDataOutputStream::CFileDataOutputStream(void* file) -- the CDataOutputStream base (CBase, its vtable, the "\r\n" line ending at +4), the file at +8, this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994400, length 45 bytes.
// Callees: CBase::CBase @ 0x0099A2F0, CCharString::CCharString(const char*, long) @ 0x0099EBF0 (the literal @ 0x012397F8, -1). Vtables 0x0129A6E0 = CDataOutputStream,
// 0x0129A6F8 = CFileDataOutputStream (rtti-vtables.tsv). Shape: the base's vtable store is scheduled between the two pushes and the string constructor, the
// derived vtable after the file store; `this` is returned, `ret 4`.
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
};

class CDataOutputStream : public CBase { public:                 // vtable 0x0129A6E0
    CCharString m_lineEnding;                      // +0x04
    virtual void v00();
    CDataOutputStream() : CBase(), m_lineEnding("\r\n", -1) { }
};

struct CFileDataOutputStream : CDataOutputStream { // vtable 0x0129A6F8
    void* m_pFile;                                 // +0x08
    virtual void v00();
    CFileDataOutputStream(void* file);
};

CFileDataOutputStream::CFileDataOutputStream(void* file)
    : CDataOutputStream()
    , m_pFile(file)
{
}