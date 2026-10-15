// CBankFileAsync::OpenFileReadOnly(const CWideString& filename, unsigned long flags) -- open the bank through the base CBankFile, then create the threaded (asynchronous) reader on the same path and re-enable async reads
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D5820, 122 bytes (+6 int3), __thiscall, ret 8 (virtual override).
// Leaf: ?OpenFileReadOnly@CBankFileAsync@@
// Callees: CBankFile::OpenFileReadOnly 0x009D1090 (qualified base call), operator new 0x00BFEA1A,
// CThreadedFile::CThreadedFile 0x0098DFD0, CCountedPointer<CThreadedFile>::Reset 0x009A9C80 (landed),
// CBankFile::GetBankPathname() 0x009CBF10 (CWideString by hidden pointer, landed), CThreadedFile::Open(const
// CWideString&, bool) 0x0098E1E0 (landed), CWideString::~CWideString 0x0099B510.
// Layout: Ego_r.pdb CBankFileAsync at the retail offsets (0x74 lower than the PDB, the retail CBankFile base
// being smaller): ThreadedFile (CCountedPointer: object, count block) +0x110, NonCached +0x148,
// DisableAsync +0x149.
// Shape: the base call fails straight out; `new CThreadedFile` (0x1C bytes, null-checked) goes into the
// counted pointer through its out-of-line Reset; the file pointer is fetched before the Open arguments are
// evaluated -- NonCached is pushed first, then GetBankPathname builds its temporary in the dead `filename`
// argument slot -- and the temporary dies right after the call.
void* operator new(unsigned int size);              // 0x00BFEA1A

class CWideString {
public:
    void* m_pData;
    CWideString(const CWideString& other);
    ~CWideString();                                 // 0x0099B510
};

class CThreadedFile {
public:
    unsigned char _body[0x1c];
    CThreadedFile();                                // 0x0098DFD0
    bool Open(const CWideString& filename, bool nonCached);   // 0x0098E1E0
};

struct SCountBlock;

class CCountedPointer_CThreadedFile {
public:
    CThreadedFile* m_pObject;                       // +0
    SCountBlock* m_pCount;                          // +4
    void Reset(CThreadedFile* object);              // 0x009A9C80
    CThreadedFile* operator->() const { return m_pObject; }
};

class CBankFile_Retail {                            // retail CBankFile (engine/CBankFile.h is the PDB layout)
public:
    virtual void v00();
    virtual bool OpenFileReadOnly(const CWideString& filename, unsigned long flags);   // 0x009D1090
    unsigned char _body[0x10c];                     // +0x04 .. +0x10F
    CWideString GetBankPathname();                  // 0x009CBF10
};

class CBankFileAsync : public CBankFile_Retail {
public:
    CCountedPointer_CThreadedFile ThreadedFile;     // +0x110
    unsigned char _pad_0x118[0x30];                 // +0x118 LoadingMemoryPool, MemoryFailureHandler, the two sets, ...
    bool NonCached;                                 // +0x148
    bool DisableAsync;                              // +0x149
    virtual bool OpenFileReadOnly(const CWideString& filename, unsigned long flags);
};

bool CBankFileAsync::OpenFileReadOnly(const CWideString& filename, unsigned long flags)
{
    if (!CBankFile_Retail::OpenFileReadOnly(filename, flags))
        return false;
    ThreadedFile.Reset(new CThreadedFile);
    ThreadedFile->Open(GetBankPathname(), NonCached);
    DisableAsync = false;
    return true;
}
