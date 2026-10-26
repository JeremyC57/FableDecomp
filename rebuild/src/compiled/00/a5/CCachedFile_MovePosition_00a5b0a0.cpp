// CCachedFile::MovePosition(long offset) -- moves the read position by `offset`, throwing CBBBFileException(11) with a message before the start or past the disk file's length
// Leaf: MovePosition
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5B0A0, 240 bytes incl. padding (ret 4).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, operator+(const CCharString&, const char*) @ 0x0099F600
//          (__fastcall, result in ecx), ~CCharString @ 0x0099EAE0, CCharString(const CCharString&) @ 0x0099EC30,
//          operator const char* @ 0x0099E4C0, _CxxThrowException @ 0x00BFEB84 (ThrowInfo 0x01369380 = CBBBFileException),
//          the disk file's slot 9 (+0x24, GetLength). Strings: "" 0x0122D70E, "Cannot move before beginning of file"
//          0x0129A8B8, "Cannot seek past end of file" 0x0129A898.
// Layout (landed CCachedFile models): +4 Position, +0x1C PDiskFile {object, count} (retail is 4 bytes below Ego_r.pdb from
//          DataBuffer on: the 12-byte STLport array), so the class is modelled locally.
// Shape: the exception model of the landed CMemoryFile::MovePosition 0x009E9140 (CBBBFileException(CCharString by value, type)
//          over an empty CExceptionBase(const char*)): message at [F+0], the exception at [F+4], the "" prefix and the by-value
//          copy in the dead `offset` slot; no EH frame, so the message is never destroyed. Both tests spell `Position + offset`
//          and the store is `Position += offset`: the first sum is only sign-tested (`add ecx,eax ; jns`), the second re-reads
//          Position into edi across GetLength. A shared `position` local CSEs the sum and drops the frame to 4 bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    operator const char*() const;                       // 0x0099E4C0
    friend CCharString __fastcall operator+(const CCharString& a, const char* b);   // 0x0099F600
};
class CExceptionBase {
public:
    CExceptionBase(const char*) { }
};
class CBBBFileException : public CExceptionBase {
public:
    enum EFileExceptionType { SeekOutOfRange = 11 };
    EFileExceptionType Type;
    CBBBFileException(CCharString text, EFileExceptionType type) : CExceptionBase(static_cast<const char*>(text)), Type(type) { }
};
class CAFile { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual long GetLength();                   // slot 9 (+0x24)
};
template <class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
    T* operator->() const { return m_pObject; }
};
class CCachedFile { public:
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
    if (Position + offset < 0) {
        CCharString message = CCharString("", -1) + "Cannot move before beginning of file";
        throw CBBBFileException(message, CBBBFileException::SeekOutOfRange);
    }
    if (Position + offset > PDiskFile->GetLength()) {
        CCharString message = CCharString("", -1) + "Cannot seek past end of file";
        throw CBBBFileException(message, CBBBFileException::SeekOutOfRange);
    }
    Position += offset;
}