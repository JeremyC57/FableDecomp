// Grade: DIFFER(232v239) -- ours CSEs Position + offset across the first throw (lea edi once, test/jge); retail adds into ecx for the sign test (jns), keeps offset in eax and re-reads Position for the second sum, with an 8-byte frame (message + exception) | tried: throw temporary, named exception local, inline ThrowFileError helper (not inlined) | next: model the throw path as not noreturn (e.g. an inline helper whose throw sits behind a condition), else lane gap: dead-argument-slot temporaries
// CCachedFile::MovePosition(long offset) -- moves the read position by `offset`, throwing CBBBFileException(11) with a message before the start or past the disk file's length
// Leaf: MovePosition
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5B0A0, 240 bytes incl. padding (ret 4).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, operator+(const CCharString&, const char*) @ 0x0099F600
//          (__fastcall, result in ecx), ~CCharString @ 0x0099EAE0, CCharString(const CCharString&) @ 0x0099EC30,
//          operator const char* @ 0x0099E4C0, _CxxThrowException @ 0x00BFEB84 (ThrowInfo 0x01369380 = CBBBFileException),
//          the disk file's slot 9 (+0x24, GetLength). Strings: "" 0x0122D70E, "Cannot move before beginning of file"
//          0x0129A8B8, "Cannot seek past end of file" 0x0129A898.
// Layout (landed CCachedFile models): +4 Position, +0x1C PDiskFile {object, count}.
// Shape: the error macro builds `CCharString("") + text`, hands a by-value copy to an inline trace helper whose body
//          only converts it to const char* (release build), then throws the 4-byte exception by value; no EH frame, so
//          the message is never destroyed.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    operator const char*() const;                       // 0x0099E4C0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

inline void TraceFileError(CCharString text) { (const char*)text; }

struct CBBBFileException {
    long Code;
    CBBBFileException(long code) : Code(code) { }
};

#define BBB_FILE_ERROR(text) \
    { CCharString message = CCharString("") + text; TraceFileError(message); throw CBBBFileException(11); }

struct CAFile {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual long GetLength();                   // slot 9 (+0x24)
};

template <class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    T* operator->() const { return m_pObject; }
};

struct CCachedFile {
    void*                   _vptr;              // +0x00
    long                    Position;           // +0x04
    long                    BufferStartPosition;// +0x08
    unsigned char           DataBuffer[0x0c];   // +0x0C
    void*                   Pathname;           // +0x18
    CCountedPointer<CAFile> PDiskFile;          // +0x1C
    void MovePosition(long offset);
};

void CCachedFile::MovePosition(long offset)
{
    if (Position + offset < 0)
        BBB_FILE_ERROR("Cannot move before beginning of file")
    long position = offset + Position;
    if (position > PDiskFile->GetLength())
        BBB_FILE_ERROR("Cannot seek past end of file")
    Position = position;
}
