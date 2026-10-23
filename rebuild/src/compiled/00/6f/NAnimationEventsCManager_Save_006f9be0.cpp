// NAnimationEvents::CManager::Save(const CWideString& fileName) -- when the event table is dirty, write it as text:
//   BEGIN_ANIMATION_EVENTS, then per animation "BEGIN_EVENTS: <EAnimType name>" (names from meshdata.h), one
//   "<event name> <time> START|STOP" line per event (STOP when the packed word's top bit is set) and END_EVENTS, then
//   END_ANIMATION_EVENTS; save the text to the file and clear the dirty flag
// Leaf: ?Save@CManager@NAnimationEvents
// Retail: Fable.exe @ 0x006F9BE0, 768 bytes, `ret 4`, 8-byte aligned frame, no EH frame.
// Callees: CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) 0x0099AD80 and its inlined dtor (vtable
//   0x0122D06C, Close 0x0099A920, ~CWideString 0x0099B510, ~CBase 0x0099A300), CCharString() 0x0099E4B0 /
//   (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 / SaveToFile(CAFile&) 0x0099E5F0, CPersistContext(CCharString&)
//   0x009BAE10 / Transfer(CCharString&) 0x009BAB50, CSymbolMap() 0x009E5200 / ~ 0x006FD040 / load-from-header 0x009E5C90
//   / FindFirstSymbolWithVal(group, val) 0x009E50C0 / FindFirstSymbolWithVal(val) 0x009E5090, the __fastcall directory
//   getter 0x009A76A0 (name unknown), operator+ 0x0099BF30 (wide), 0x0099F690 / 0x0099F600 / 0x0099F570 (char),
//   CCharString::FormatDouble 0x009A08B0.
// Layout: the landed SaveBinary 0x006F92A0 model: event names CSymbolMap +0x08 (retail 0x24 bytes, as the landed
//   0x00AF1600; the PDB 0x2c is the debug view), ExtraAnimEvents +0x2c (12-byte entries), Dirty +0x3c (retail load).
//   The 0x24 CSymbolMap is what puts the save context below the animTypes temporary in the 0xa4 frame.
#pragma pack(push, 1)
#pragma pack(pop)
struct CWideString {
    void* PStringData;
    ~CWideString();                                 // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const unsigned short* b);   // 0x0099BF30
CWideString __fastcall GetDataDirectory();          // 0x009A76A0 (name descriptive)
namespace N_0099a300 { struct CBase {
    virtual void v00();
    ~CBase();                                       // 0x0099A300
}; } using N_0099a300::CBase;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
struct CAFile : CBase {
};
struct CDiskFileWin32 : CAFile {                    // vtable 0x0122D06C, 0x1c bytes
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
};
struct CCharString {
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    void SaveToFile(CAFile& file) const;               // 0x0099E5F0
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};
CCharString __fastcall operator+(const CCharString& a, const CCharString& b);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& a, const char* b);          // 0x0099F600
CCharString __fastcall operator+(const char* a, const CCharString& b);          // 0x0099F690
struct CPersistContext {                               // retail 0x2c bytes, trivial destructor
    unsigned char _pad00[0x2c];
    CPersistContext(CCharString& output);              // 0x009BAE10
    void Transfer_009bab50(CCharString& value);                 // 0x009BAB50
};
struct CSymbolInfo {
    CCharString GroupName;                             // +0x00
    CCharString EntryName;                             // +0x04
    long        Val;                                   // +0x08
};
struct CSymbolMap {                                    // retail 0x24 bytes (as the landed 0x00af1600)
    unsigned char _body[0x24];
    CSymbolMap();                                      // 0x009E5200
    ~CSymbolMap();                                     // 0x006FD040
    void LoadFromHeaderFile(const CWideString& fileName);              // 0x009E5C90
    const CSymbolInfo* FindFirstSymbolWithVal(const CCharString& group, long value) const;   // 0x009E50C0
    const CSymbolInfo* FindFirstSymbolWithVal(long value) const;       // 0x009E5090
};

namespace NAnimationEvents {

struct CEvent {                                     // 8 bytes
    unsigned long Packed;                           // index (30 bits), flags in bits 30 and 31
    float         Time;
};
struct CEventList {
    CEvent* Data;
    long    Count;
    CEvent* begin() const { return Data; }
    CEvent* end() const { return Data + Count; }
};
struct CAnimEvents {                                // 12 bytes
    unsigned long AnimID;
    CEventList    Events;
};

class CManager {
public:
    virtual void v00();
    unsigned char       _pad04[4];
    CSymbolMap          EventNames;                 // +0x08 (0x24 bytes, ends at +0x2c)
    CAnimEvents*        ExtraAnimEvents_start;      // +0x2c
    CAnimEvents*        ExtraAnimEvents_finish;     // +0x30
    CAnimEvents*        ExtraAnimEvents_end;        // +0x34
    unsigned char       _pad38[4];
    bool                Dirty;                      // +0x3c
    void Save(const CWideString& fileName);
};

void CManager::Save(const CWideString& fileName)
{
    if (!Dirty)
        return;
    {
        CDiskFileWin32 file(fileName, FILE_ACCESS_WRITE, 2);
        CCharString text;
        CPersistContext context(text);
        {
            CCharString line("BEGIN_ANIMATION_EVENTS\r\n");
            context.Transfer_009bab50(line);
        }
        CSymbolMap animTypes;
        animTypes.LoadFromHeaderFile(GetDataDirectory() + (const unsigned short*)L"meshdata.h");
        for (CAnimEvents* it = ExtraAnimEvents_start; it != ExtraAnimEvents_finish; ++it) {
            unsigned long animID = it->AnimID;
            const CSymbolInfo* info = animTypes.FindFirstSymbolWithVal("EAnimType", animID);
            context.Transfer_009bab50("BEGIN_EVENTS: " + info->EntryName + "\r\n");
            for (CEvent* e = it->Events.begin(); e != it->Events.end(); ++e) {
                const CSymbolInfo* name = EventNames.FindFirstSymbolWithVal(e->Packed & 0x3fffffff);
                if (e->Packed & 0x80000000)
                    context.Transfer_009bab50(name->EntryName + " " + CCharString::FormatDouble(6, e->Time) + " STOP\r\n");
                else
                    context.Transfer_009bab50(name->EntryName + " " + CCharString::FormatDouble(6, e->Time) + " START\r\n");
            }
            {
                CCharString line("END_EVENTS\r\n\r\n");
                context.Transfer_009bab50(line);
            }
        }
        {
            CCharString line("END_ANIMATION_EVENTS\r\n");
            context.Transfer_009bab50(line);
        }
        text.SaveToFile(file);
        Dirty = false;
    }
}

}