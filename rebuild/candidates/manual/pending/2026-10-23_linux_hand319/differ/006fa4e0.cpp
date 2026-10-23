// Grade: DIFFER(1405v1444) -- frame layout (exists flag at +0x2b v +0xf, the event/time slots), the inner END_EVENTS while loop is not inverted (retail duplicates the SkipPastNextItemIfIs test at the bottom), animID copy register
// NAnimationEvents::CManager::Load(const CWideString& fileName) -- load the extra animation events: when the binary switch is
//   set read <drive><dir><name>.bin through LoadBinary and clear the dirty flag; otherwise, if the text file exists, clear the
//   table and parse BEGIN_ANIMATION_EVENTS / BEGIN_EVENTS: <EAnimType> / "<event> <time> [START|STOP]" ... END_EVENTS /
//   END_ANIMATION_EVENTS (anim types from meshdata.h), adding every event not already present; compact the table and, when
//   the save-binary switch is set, write the .bin twin; return whether the text file existed
// Leaf: ?Load@CManager@NAnimationEvents
// Retail: Fable.exe @ 0x006FA4E0, 1456 bytes, `ret 4`.
// Callees: SplitPath 0x00997620 / operator+ 0x0099BE70, 0x0099BF30 (__fastcall, hidden result), CWideString() 0x0099AED0,
//   ~CWideString 0x0099B510, LoadBinary 0x006F9F90, SaveBinary 0x006F92A0, PathExists 0x00999230, the STLport erase pair
//   __copy 0x006FBC50 / _Destroy 0x006FB8A0, CDiskFileWin32 0x0099AD80 (+ inline dtor: Close 0x0099A920, ~CBase 0x0099A300),
//   CCharString(CAFile*, long) 0x0099FA50, CStringParser 0x00404720 (+ inline dtor: ~list 0x004046B0, ~CBaseClass 0x0099A2E0),
//   NextItemIs 0x009B9BA0, SkipPastNextItemIfIs 0x009B9E00, SkipWhitespace 0x009B9C60, ReadAsString 0x009BA6F0,
//   ReadAsFloat 0x009BA650, PeekNextItem 0x009BA8C0, ReadNextItem 0x009BA080, CParsedItem::GetString 0x009B9880,
//   CBasicString<char>::operator== 0x004115A0, CSymbolMap() 0x009E5200 / ~ 0x006FD040 / LoadFromHeaderFile 0x009E5C90 /
//   Lookup(name, long*) 0x009E5170 / Lookup(name) 0x009E5120, GetDataDirectory 0x009A76A0, EventExists 0x006F9430,
//   AddEventToAnim 0x006FA3F0, the table compaction 0x006FD0B0.
// Layout: the landed Save 0x006F9BE0 / LoadBinary 0x006F9F90 models (EventNames CSymbolMap +0x08, ExtraAnimEvents +0x2c,
//   Dirty +0x3c); CStringParser (retail 0x28) as the landed LoadFromGameStateScript 0x00CB8440; CParsedItem and the inline
//   CCharString == const char* as the landed CCRCSymbolMap::ParseEnum 0x00A38670.
#include <string.h>
#pragma intrinsic(strcmp)
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
struct CAFile;
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(CAFile* file, long length);            // 0x0099FA50
    ~CCharString();                                    // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
struct CWideString {
    void* PStringData;
    CWideString();                                  // 0x0099AED0
    ~CWideString();                                 // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);      // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const unsigned short* b);   // 0x0099BF30
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension);   // 0x00997620
bool __fastcall PathExists(const CWideString& path);   // 0x00999230
CWideString __fastcall GetDataDirectory();          // 0x009A76A0 (name descriptive)
struct CBase {
    virtual void v00();
    ~CBase();                                       // 0x0099A300
};
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
struct CBaseClass {
    virtual void v00();
    ~CBaseClass();                                 // 0x0099A2E0
};
struct CStringParserCommentList {
    void* m_pHead;
    ~CStringParserCommentList();                   // 0x004046B0
};
struct CParsedItem {                               // 12 bytes
    long        Type;                              // 1 = symbol, 2 = identifier
    long        Value;
    CCharString Text;
    const CCharString& GetString();                // 0x009B9880
};
struct CStringParser : CBaseClass {                // retail 0x28 bytes
    char*                    String;               // +0x04
    unsigned long            BaseLineOffset;       // +0x08
    CCharString              Filename;             // +0x0c
    CCharString              TempCharString;       // +0x10
    CStringParserCommentList Comments;             // +0x14
    void (*ErrorCallback)(CCharString*);           // +0x18
    long                     CursorPos[3];         // +0x1c
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);            // 0x00404720
    bool NextItemIs(const CCharString& item);              // 0x009B9BA0
    long SkipWhitespace();                                 // 0x009B9C60
    bool SkipPastNextItemIfIs(const CCharString& item);    // 0x009B9E00
    CCharString ReadAsString();                            // 0x009BA6F0
    float ReadAsFloat();                                   // 0x009BA650
    bool PeekNextItem(CParsedItem& item);                  // 0x009BA8C0
    bool ReadNextItem(CParsedItem& item);                  // 0x009BA080
};
struct CSymbolMap {                                    // retail 0x24 bytes
    unsigned char _body[0x24];
    CSymbolMap();                                      // 0x009E5200
    ~CSymbolMap();                                     // 0x006FD040
    void LoadFromHeaderFile(const CWideString& fileName);              // 0x009E5C90
    bool Lookup(const CCharString& name, unsigned long* value) const;  // 0x009E5170
    long Lookup(const CCharString& name) const;                        // 0x009E5120
};
extern bool g_LoadBinaryAnimEvents;                    // 0x013B860A
extern bool g_SaveBinaryAnimEvents;                    // 0x01375459

namespace NAnimationEvents {
struct CEvent {
    unsigned long Index : 30;
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float         Time;
};
struct CAnimEvents { unsigned long AnimID; CEvent* Data; long Count; };   // 12 bytes
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };
CAnimEvents* __fastcall CopyAnimEvents(CAnimEvents* first, CAnimEvents* last, CAnimEvents* result,
                                       const random_access_iterator_tag&, int*);   // 0x006FBC50 (STLport __copy)
void __fastcall DestroyAnimEvents(CAnimEvents* first, CAnimEvents* last, const __false_type&);   // 0x006FB8A0
struct CAnimEventsVector {                             // STLport vector<pair<unsigned long, CSmallVector<CEvent,32>>>
    CAnimEvents* _M_start;
    CAnimEvents* _M_finish;
    CAnimEvents* _M_end_of_storage;
    __forceinline void clear()
    {
        CAnimEvents* i = CopyAnimEvents(_M_finish, _M_finish, _M_start, random_access_iterator_tag(), 0);
        DestroyAnimEvents(i, _M_finish, __false_type());
        _M_finish = i;
    }
    void Compact();                                    // 0x006FD0B0 (name descriptive)
};

class CManager {
public:
    virtual void v00();
    unsigned char       _pad04[4];
    CSymbolMap          EventNames;                 // +0x08
    CAnimEventsVector   ExtraAnimEvents;            // +0x2c
    unsigned char       _pad38[4];
    bool                Dirty;                      // +0x3c
    void SaveBinary(const CWideString& fileName);  // 0x006F92A0
    void LoadBinary(const CWideString& fileName);  // 0x006F9F90
    bool EventExists(long animID, CEvent& event);  // 0x006F9430
    void AddEventToAnim(unsigned long animID, CEvent& event);   // 0x006FA3F0
    bool Load(const CWideString& fileName);
};

bool CManager::Load(const CWideString& fileName)
{
    if (g_LoadBinaryAnimEvents) {
        CWideString drive;
        CWideString directory;
        CWideString name;
        SplitPath(fileName, &drive, &directory, &name, 0);
        CWideString binName = drive + directory + name + (const unsigned short*)L".bin";
        LoadBinary(binName);
        Dirty = false;
        return true;
    }
    bool exists = PathExists(fileName);
    if (exists) {
        ExtraAnimEvents.clear();
        CDiskFileWin32 file(fileName, FILE_ACCESS_READ, 2);
        CCharString text(&file, -1);
        CStringParser parser(text, 0, CCharString(""), 0);
        if (parser.NextItemIs(CCharString("BEGIN_ANIMATION_EVENTS"))) {
            CSymbolMap animTypes;
            animTypes.LoadFromHeaderFile(GetDataDirectory() + (const unsigned short*)L"meshdata.h");
            while (parser.SkipPastNextItemIfIs(CCharString("BEGIN_EVENTS:"))) {
                parser.SkipWhitespace();
                CCharString animName = parser.ReadAsString();
                unsigned long animID;
                if (animTypes.Lookup(animName, &animID)) {
                    while (!parser.SkipPastNextItemIfIs(CCharString("END_EVENTS"))) {
                        CCharString eventName = parser.ReadAsString();
                        long index = EventNames.Lookup(eventName);
                        float time = parser.ReadAsFloat();
                        bool stop = false;
                        CParsedItem item;
                        if (parser.PeekNextItem(item) && item.Type == 2) {
                            if (item.GetString() == "STOP") {
                                stop = true;
                                parser.ReadNextItem(item);
                            } else if (item.GetString() == "START")
                                parser.ReadNextItem(item);
                        }
                        unsigned long id = animID;
                        CEvent event;
                        event.Index = index;
                        event.ExternallyImported = 0;
                        event.StopEvent = stop;
                        event.Time = time;
                        if (!EventExists(id, event))
                            AddEventToAnim(id, event);
                    }
                    Dirty = false;
                    parser.SkipWhitespace();
                } else
                    parser.NextItemIs(CCharString("END_EVENTS"));
            }
            parser.NextItemIs(CCharString("END_ANIMATION_EVENTS"));
        }
    }
    ExtraAnimEvents.Compact();
    if (g_SaveBinaryAnimEvents) {
        CWideString drive;
        CWideString directory;
        CWideString name;
        SplitPath(fileName, &drive, &directory, &name, 0);
        CWideString binName = drive + directory + name + (const unsigned short*)L".bin";
        SaveBinary(binName);
    }
    return exists;
}

}
