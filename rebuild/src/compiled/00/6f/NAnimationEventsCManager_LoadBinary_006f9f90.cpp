// NAnimationEvents::CManager::LoadBinary(const CWideString& fileName) -- read the binary extra-event file: load the whole disk file into a zeroed byte buffer, open a memory stream over it, read the animation count and resize the per-animation table, then for each animation read its id and event count, resize its event list and read each event's index, two flag bytes and time
// Leaf: ?LoadBinary@CManager@NAnimationEvents@@AAEXABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F9F90, length 1109 bytes, `ret 4`.
// Ego_r.pdb: `?LoadBinary@CManager@NAnimationEvents@@...` (the catalog class "CManager" is this one; the reader of the landed
//   SaveBinary 0x006F92A0).
// Callees: CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) @ 0x0099AD80, CDiskFileWin32::Read @ 0x0099A960,
//   malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (the STLport byte vector), CDataInputStream::CDataInputStream @ 0x00993B20,
//   CMemoryDataInputStream::OpenMemorySource @ 0x00994570, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0,
//   ~CMemoryDataInputStream @ 0x00993B10, the per-animation vector's resize @ 0x006FCF90, CSmallVector<CEvent,32>::resize
//   @ 0x006FADE0, operator delete[] @ 0x00BFEB1C (the default pair temporary's empty event list), the inlined
//   ~CDiskFileWin32 (vtable 0x0122D06C, Close @ 0x0099A920, ~CWideString @ 0x0099B510, ~CBaseClassNonCopyable @ 0x0099A300).
//   Vtable 0x0123675C CMemoryDataInputStream.
// Layout: the manager's per-animation table at +0x2c (12-byte pairs: animation id, CSmallVector<CEvent> {Start, Size}) and
//   CEvent bitfields (Index:30, ExternallyImported:1, StopEvent:1, Time) as the landed AddEventToAnim 0x006FA3F0; the inline
//   stream reader as the landed CWoundMorphs::TransferBinaryIn 0x00757990; the byte buffer's GetData idiom as the landed
//   CEngineMap::OpenDevelopmentStaticMap 0x00B42530.
// Shape: both resizes get same-TU `auto_inline(off)` stand-ins that only READ their value argument: with the pair stand-in
//   VC7.1 knows the default pair temporary is untouched and folds its dtor to `operator delete[](0)`; with the event stand-in
//   the default `CEvent()` (empty user ctor) shares the spilled table pointer's frame slot (frame 0x74). The reads return
//   through inline ReadSLONG/ReadULONG/ReadFLOAT helpers (so the counts stay in registers), and ReadEBOOL reads a byte and
//   returns `value != 0` (retail `cmp byte ; setne`, which also gives retail's ebp/ebx assignment).
// NF188 (2026-11-20, q4251): the row-local CDataInputStream / CMemoryDataInputStream models (a user-declared destructor that
//   bound a CMemoryDataInputStream dtor spelling at 0x00993B10) are replaced by the shared engine/CDataInputStream.h /
//   engine/CMemoryDataInputStream.h classes, whose compiler-generated destructor calls ~CDataStream 0x00993B10 directly
//   (??1CDataStream@@UAE@XZ, Ego_r.pdb 0x00AFBBB0 + FableWin.pdb 0x03080440; engine/CDataInputStream.h retry11a note).
#include "fable_compiler.h"
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memcpy, memset)
void operator delete[](void* block) throw();            // 0x00BFEB1C

class CWideString { public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#include "engine/CMemoryDataInputStream.h"
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;
class CDiskFileWin32 : public CBaseClassNonCopyable { public:                         // vtable 0x0122D06C, 0x1c bytes
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                               // 0x0099A920 (slot 2)
    long        AccessType;                             // +0x04
    long        Position;                               // +0x08
    long        Length;                                 // +0x0c
    void*       WinFileHandle;                          // +0x10
    bool        Opened;                                 // +0x14
    CWideString Pathname;                               // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
public:
    virtual void Read(void*, long, CASuspendableProcess*);   // 0x0099A960
};

struct CByteVector {                                    // STLport vector<unsigned char>, 12 bytes
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
    static unsigned char* allocate(unsigned long n) { return n != 0 ? (unsigned char*)malloc(n) : 0; }
    static unsigned char* fill(unsigned char* first, unsigned long n) { memset(first, 0, n); return first + n; }
    explicit CByteVector(unsigned long n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        _M_finish = fill(_M_start, n);
    }
    ~CByteVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    unsigned char* GetData() { return (_M_finish - _M_start) ? _M_start : (unsigned char*)this; }
};

struct SZeroed {
    long m_00;
    SZeroed() : m_00(0) { }
};

namespace NAnimationEvents {
class CEvent { public:
    unsigned long Index : 30;
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float         Time;
    CEvent() { }
};
struct EventVector {                                    // CSmallVector<CEvent,32>: {Start, Size}
    CEvent* Start;
    int     Size;
    EventVector() : Start(0), Size(0) { }
    ~EventVector() { delete[] Start; }
    void resize(long count, const CEvent& value = CEvent());   // 0x006FADE0
};
struct EventPair {                                      // 12 bytes
    unsigned long first;
    EventVector   second;
    EventPair() : first(0) { }
};
struct EventPairVector {                                // STLport vector<EventPair>
    EventPair* _M_start;
    EventPair* _M_finish;
    EventPair* _M_end_of_storage;
    EventPair& operator[](unsigned long n) { return _M_start[n]; }
    void resize(unsigned long count, const EventPair& value);   // 0x006FCF90
};
#pragma auto_inline(off)
void EventVector::resize(long count, const CEvent& value)   // same-TU stand-in for 0x006FADE0
{
    if (value.Index == 0 && value.Time == 0.0f)
        Size = count;
}
void EventPairVector::resize(unsigned long count, const EventPair& value)   // same-TU stand-in for 0x006FCF90
{
    if (value.first == 0 && value.second.Start == 0 && value.second.Size == 0)
        _M_finish = _M_start + count;
}
#pragma auto_inline(on)
class CManager {
public:
    virtual void v00();
    unsigned char   _pad04[0x28];
    EventPairVector ExtraAnimEvents;                    // +0x2c
    private:
    void LoadBinary(const CWideString& fileName);
    public:
};

void CManager::LoadBinary(const CWideString& fileName)
{
    CDiskFileWin32 file(fileName, FILE_ACCESS_READ, 2);
    CByteVector buffer(file.Length);
    file.Read(buffer.GetData(), buffer.size(), 0);
    CMemoryDataInputStream stream;
    stream.OpenMemorySource(buffer.GetData(), buffer.size());
    long count = stream.ReadSLONG();
    ExtraAnimEvents.resize(count, EventPair());
    for (long i = 0; i < count; ++i) {
        ExtraAnimEvents[i].first = stream.ReadULONG();
        EventVector& events = ExtraAnimEvents[i].second;
        long eventCount = stream.ReadSLONG();
        events.resize(eventCount);
        for (long j = 0; j < eventCount; ++j) {
            events.Start[j].Index = stream.ReadULONG();
            events.Start[j].ExternallyImported = stream.ReadEBOOL();
            events.Start[j].StopEvent = stream.ReadEBOOL();
            events.Start[j].Time = stream.ReadFloat();
        }
    }
}
}