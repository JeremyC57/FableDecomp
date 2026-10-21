// NAnimationEvents::CManager::SaveBinary(const CWideString& fileName) -- serialise every animation's extra event list (animation id, event count, then each event's index, two flag bits and time) into a memory stream and write it to a new disk file
// Leaf: SaveBinary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F92A0, length 390 bytes (padded to 400), `ret 4`.
// Ego_r.pdb: `?SaveBinary@CManager@NAnimationEvents@@AAEXABVCWideString@@@Z` (the catalog class "CManager" is this one).
// Callees: CMemoryDataOutputStream ctor @ 0x00996AF0 / dtor @ 0x00996B30 (manifest "~CCompressedMemoryDataInputStream" is a twin),
//   CDataOutputStream::WriteSLONG @ 0x00993EB0, WriteULONG @ 0x00993EA0, WriteEBOOL @ 0x00993E30, WriteFLOAT @ 0x00993EE0 (the manifest
//   names all three 4-byte writers "WriteCBYTE"), CMemoryDataOutputStream::GetSize @ 0x00994B40 (manifest "GetFramesRemaining" is a
//   twin) and PeekData @ 0x00995E70, CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) @ 0x0099AD80, Write @ 0x0099AA90,
//   the inlined ~CDiskFileWin32 (vtable 0x0122D06C, Close @ 0x0099A920, ~CWideString @ 0x0099B510, ~CBase @ 0x0099A300) as the landed
//   CInputManager::StreamInputsFromFile 0x009F5E10.
// Layout: the manager's per-animation extra events (STLport vector of 12-byte entries: animation id, CEvent array, count) at +0x2c;
//   CEvent (8 bytes): packed word (index in the low 30 bits, two flags in bits 30/31) and time.
// Shape: size() is the signed /12 of the pointer difference compared unsigned; the inner count is re-read each pass; the flag bits
//   are written as `(word & bit) == bit`; the file is a scoped stack object written as Write(PeekData(), GetSize(), 0).
struct CWideString {
    void* PStringData;                              // +0x00
    ~CWideString();                                 // 0x0099B510
};
struct CBase {
    virtual void v00();
    ~CBase();                                       // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;

struct CDiskFileWin32 : CBase {                     // vtable 0x0122D06C, 0x1c bytes
    virtual void v00();
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                           // 0x0099A920 (slot 2)
    long        AccessType;                         // +0x04
    long        Position;                           // +0x08
    long        Length;                             // +0x0c
    void*       WinFileHandle;                      // +0x10
    bool        Opened;                             // +0x14
    CWideString Pathname;                           // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
    void Write(const void* data, long size, CASuspendableProcess* process);   // 0x0099AA90
};

struct CDataOutputStream {
    void WriteEBOOL(bool value);                    // 0x00993E30
    void WriteULONG(unsigned long value);           // 0x00993EA0
    void WriteSLONG(long value);                    // 0x00993EB0
    void WriteFLOAT(float value);                   // 0x00993EE0
};

struct CMemoryDataOutputStream : CDataOutputStream {
    unsigned char _body[0x18];
    CMemoryDataOutputStream();                      // 0x00996AF0
    ~CMemoryDataOutputStream();                     // 0x00996B30
    long GetSize() const;                           // 0x00994B40
    const void* PeekData() const;                   // 0x00995E70
};

namespace NAnimationEvents {

struct CEvent {                                     // 8 bytes
    unsigned long Packed;                           // index (30 bits), flags in bits 30 and 31
    float         Time;
};

struct CEventList {                                 // 8 bytes: the event array and its count
    CEvent* Data;
    long    Count;
    long size() const { return Count; }
    const CEvent& operator[](long n) const { return Data[n]; }
};

struct CAnimEvents {                                // 12 bytes
    unsigned long AnimID;
    CEventList    Events;
};

template <class T> struct vector {                  // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return _M_start[n]; }
};

class CManager {
public:
    virtual void v00();
    unsigned char       _pad04[0x28];
    vector<CAnimEvents> ExtraAnimEvents;            // +0x2c
    void SaveBinary(const CWideString& fileName);
};

void CManager::SaveBinary(const CWideString& fileName)
{
    CMemoryDataOutputStream stream;
    stream.WriteSLONG(ExtraAnimEvents.size());
    for (unsigned long i = 0; i < ExtraAnimEvents.size(); ++i) {
        stream.WriteSLONG(ExtraAnimEvents[i].AnimID);
        const CEventList& events = ExtraAnimEvents[i].Events;
        stream.WriteSLONG(events.size());
        for (long j = 0; j < events.size(); ++j) {
            stream.WriteULONG(events[j].Packed & 0x3fffffff);
            stream.WriteEBOOL((events[j].Packed & 0x40000000) == 0x40000000);
            stream.WriteEBOOL((events[j].Packed & 0x80000000) == 0x80000000);
            stream.WriteFLOAT(events[j].Time);
        }
    }
    CDiskFileWin32 file(fileName, FILE_ACCESS_WRITE, 2);
    file.Write(stream.PeekData(), stream.GetSize(), 0);
}

}
