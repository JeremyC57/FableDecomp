// CFileDataOutputStream::~CFileDataOutputStream() -- the file output stream's destructor: the CDataOutputStream new-line string, then the CDataStream root
// Leaf: ??1CFileDataOutputStream@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00994430, 31 bytes, thiscall.
// Identity: the first store is vtable 0x0129A6F8, which rtti-vtables.tsv gives to CFileDataOutputStream, before any
//   teardown; the second store, after the derived body, is 0x0129A69C = CDataStream (the inlined base destructor), so
//   this is CFileDataOutputStream's own destructor. Ego_r.pdb: CDataOutputStream {vptr, CCharString NewLine +4},
//   CFileDataOutputStream {.., CAFile* File +8}; name set ??1CFileDataOutputStream@@UAE@XZ, ??1CDataOutputStream@@UAE@XZ,
//   ??1CDataStream@@UAE@XZ. Callees: ~CCharString 0x0099EAE0 (NewLine), ~CBaseClassNonCopyable 0x0099A300 (tail jump).
// Label: landed as CFileDataInputStream::~CFileDataInputStream with fastcall stand-ins; the catalog already says
//   CFileDataOutputStream::~CFileDataOutputStream. CDataOutputStream stores no vtable of its own here, so it is modelled
//   `__declspec(novtable)` (as the landed def-base models do).
class CCharString {
public:
    ~CCharString();                                   // 0x0099EAE0
    void* PStringData;
};

class CAFile;

class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                 // 0x0099A300
};

class CDataStream : public CBaseClassNonCopyable {
public:
    virtual ~CDataStream() { }
};

class __declspec(novtable) CDataOutputStream : public CDataStream {
public:
    virtual ~CDataOutputStream() { }
    CCharString NewLine;                              // +0x04
};

class CFileDataOutputStream : public CDataOutputStream {
public:
    virtual ~CFileDataOutputStream();
    CAFile* File;                                     // +0x08
};

CFileDataOutputStream::~CFileDataOutputStream()
{
}
