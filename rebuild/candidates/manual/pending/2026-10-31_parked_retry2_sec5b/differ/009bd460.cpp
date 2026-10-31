// CMovieSubtitles::ParseSubtitles(const CWideString& filename) -- set the word-wrap options, build "<drive><dir><name>.txt" from the movie path, clear the subtitles and, when there is a text bank and the file exists, load it and parse "key m:ss.s m:ss.s" lines: each key's CRC is looked up in the text bank and a CSubTitle (text entry, frame start/end at the movie fps, start/end seconds) is appended; finally pick up the subtitle font ("ENG_ARIAL_18") and the sound manager
// Grade: DIFFER(1174v1193) 223/338 -- every call, constant and the inline CRC loop / CIVCountedPointer assignment / list push_back / CDiskFileWin32 and CStringParser destructors match in order; residue: the frame is one dword larger (0xA0 vs 0x9C) so the stack slots shift, and ebx/esi/edi are pushed after the text-bank test instead of in the prologue (the early-out path then needs no pops)
// Leaf: ?ParseSubtitles@CMovieSubtitles@@QAEXABVCWideString@@@Z
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BD460, 1193 bytes, `ret 4`.
// Name: Ego_r.pdb ?ParseSubtitles@CMovieSubtitles@@QAEXABVCWideString@@@Z; the callback is ?GetCharWidthW@CMovieSubtitles@@SII_W@Z
//   (0x009BCEF0); the font global at 0x013CA7F8 (g_SubtitleFont) is unnamed in the PDBs -- review at landing.
// Callees: WordWrap_SetOption 0x00A0FDB0 / WordWrap_SetCallback 0x00A0FD90 (fastcall), CAFile::SplitPath 0x00997620 /
//   MakePath 0x00997780 / PathExists 0x00999230, list<CSubTitle>::clear 0x009BE060, CDiskFileWin32(path, access, flags)
//   0x0099AD80 / Close 0x0099A920, CCharString::LoadFromFile 0x0099F2E0, CStringParser ctor 0x00404720 / NextItemExists
//   0x009BA4F0 / ReadAsString 0x009BA6F0 / ReadAsInteger 0x009BA540 / ReadNextItem 0x009BA080 / ReadAsFloat 0x009BA650,
//   NGameText::CDataBank::LookupValue 0x009CC410 / GetTextInfo 0x009C7D00 / GetPTextEntry 0x009C9070, CTextEntry::GetText
//   0x009C7A40, CWideString::GetLength 0x0099B220, CSubTitle copy 0x009BDE00, GFGetSystemManager 0x009A4EC0,
//   CFontManager::GetFont 0x009E2C80, CCountedPointer<>::operator= 0x00419463 (shared body), __ftol2, malloc.
// Layout: Ego_r.pdb CMovieSubtitles with a 4-byte STLport list: pTextBank +0x04, Fps +0x08, PSoundManager +0x0C,
//   IsSubtitleTime +0x10, FrameCounter +0x28; CSubTitle (0x1C) as the PDB; CStringParser 0x28 (engine/CStringParser.h);
//   CDiskFileWin32 0x1C (engine/CDiskFileWin32.h). The `L"txt"` extension is 0x0129B3B0, the empty filename 0x0122D70E.
// Levers: first reconstruction 214/338 (CRC helper out of line, this in edi, every callee-saved push deferred);
//   __forceinline CRC with retail operand order (this in ebp, 1174 bytes: 223/338 -- closer shape, slots shifted by the extra
//   dword); start/end products reordered (239, worse).
#include "engine/CCountedPointer.h"
typedef unsigned short wchar_t;
extern "C" void* __cdecl malloc(unsigned int);
extern const unsigned long g_CRCTable[256];             // 0x0129A168
class CCharStringData { public: const char* Data; long Length; };
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    void LoadFromFile(class CAFile& file, class CASuspendableProcess* process, long size);   // 0x0099F2E0
};
__forceinline unsigned long GetCRC32(const CCharString& s)
{
    const char* data;
    unsigned long length;
    if (s.PStringData == 0) {
        length = 0;
        data = "";
    } else {
        length = s.PStringData->Length;
        data = s.PStringData->Data;
    }
    unsigned long crc = 0;
    if (length != 0) {
        for (unsigned long i = 0; i < length; ++i)
            crc = (crc >> 8) ^ g_CRCTable[(unsigned char)data[i] ^ (crc & 0xff)];
    }
    return crc;
}
class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const wchar_t* text);                   // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    long GetLength() const;                             // 0x0099B220
};
class CBaseClass { public:
    virtual ~CBaseClass();                              // 0x0099A300
};
class CBaseClassNonCopyable { public:
    virtual ~CBaseClassNonCopyable();                   // 0x0099A2E0
};
class CAFile : public CBaseClass { public:
    static void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* dir, CWideString* fname, CWideString* ext);   // 0x00997620
    static CWideString __fastcall MakePath(const CWideString& drive, const CWideString& dir, const CWideString& fname, const CWideString& ext);   // 0x00997780
    static bool __fastcall PathExists(const CWideString& path);   // 0x00999230
};
class CDiskFileWin32 : public CAFile { public:
    long AccessType;
    long Position;
    long Length;
    void* WinFileHandle;
    bool Opened;
    CWideString Pathname;                               // +0x18
    CDiskFileWin32(const CWideString& path, long access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
    void Close();                                       // 0x0099A920
};
class CParsedItem { public:
    long Type;
    long Value;
    CCharString String;
};
class CStringParserCommentList { public:
    void* _M_node;
    ~CStringParserCommentList();                        // 0x004046B0
};
class CStringParser : public CBaseClassNonCopyable { public:
    char* String;
    unsigned long BaseLineOffset;
    CCharString Filename;
    CCharString TempCharString;
    CStringParserCommentList Comments;
    void (__fastcall* ErrorCallback)(const CCharString&);
    unsigned char CursorPos[0xc];
    CStringParser(const CCharString& text, void (__fastcall* callback)(const CCharString&), const CCharString& filename, long baseLineOffset);   // 0x00404720
    bool NextItemExists();                              // 0x009BA4F0
    CCharString ReadAsString();                         // 0x009BA6F0
    long ReadAsInteger();                               // 0x009BA540
    float ReadAsFloat();                                // 0x009BA650
    bool ReadNextItem(CParsedItem& item);               // 0x009BA080
};
namespace NGameText {
class CTextEntry { public:
    virtual void v0();
    virtual void DeleteThis();                          // slot 1
    long RefCount;                                      // +4
    CWideString GetText() const;                        // 0x009C7A40
};
class CDataBankTextEntry;
}
template <class T> class CIVCountedPointer { public:
    T* Object;
    CIVCountedPointer() : Object(0) {}
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        if (Object) {
            if (--((NGameText::CTextEntry*)Object)->RefCount == 0)
                ((NGameText::CTextEntry*)Object)->DeleteThis();
            Object = 0;
        }
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++((NGameText::CTextEntry*)Object)->RefCount;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
namespace NGameText {
class CDataBank { public:
    unsigned long LookupValue(unsigned long key);       // 0x009CC410
    const CDataBankTextEntry* GetTextInfo(unsigned long index) const;   // 0x009C7D00
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long index, bool load) const;   // 0x009C9070
};
}
class CSubTitle { public:
    CIVCountedPointer<const NGameText::CTextEntry> TextEntry;
    CWideString FormattedText;
    CCharString Sound;
    long FrameStart;
    long FrameEnd;
    float StartTime;
    float EndTime;
    CSubTitle() {}
    CSubTitle(const CSubTitle& other);                  // 0x009BDE00
};
inline void* operator new(unsigned int, void* p) { return p; }
struct CSubTitleNode {
    CSubTitleNode* _M_next;
    CSubTitleNode* _M_prev;
    CSubTitle _M_data;
};
class CSubTitleList { public:
    CSubTitleNode* _M_node;
    void clear();                                       // 0x009BE060
    void push_back(const CSubTitle& x)
    {
        CSubTitleNode* tmp = (CSubTitleNode*)malloc(sizeof(CSubTitleNode));
        new (&tmp->_M_data) CSubTitle(x);
        CSubTitleNode* n = _M_node;
        CSubTitleNode* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
    }
};
class CFontBank;
class CFontManager { public:
    CCountedPointer<CFontBank> GetFont(const CCharString& name);   // 0x009E2C80
};
class CASoundManager;
class CSystemManager { public:
    unsigned char _pad[0x7c];
    CASoundManager* SoundManager;                       // +0x7c
    unsigned char _pad80[4];
    CFontManager* FontManager;                          // +0x84
};
CSystemManager* __fastcall GFGetSystemManager();        // 0x009A4EC0
extern CCountedPointer<CFontBank> g_SubtitleFont;       // 0x013CA7F8 (name unproven)
void __fastcall WordWrap_SetOption(unsigned int codePage, unsigned long options);   // 0x00A0FDB0
void __fastcall WordWrap_SetCallback(unsigned int (__fastcall* callback)(wchar_t), unsigned long context);   // 0x00A0FD90
class CMovieSubtitles { public:
    CSubTitleList SubTitles;                            // +0x00 (STLport list: 4 bytes)
    NGameText::CDataBank* pTextBank;                    // +0x04
    long Fps;                                           // +0x08
    CASoundManager* PSoundManager;                      // +0x0c
    bool IsSubtitleTime;                                // +0x10
    CWideString FormattedText;                          // +0x14
    float VSubtitleWindow[4];                           // +0x18
    long FrameCounter;                                  // +0x28
    static unsigned int __fastcall GetCharWidthW(wchar_t c);   // 0x009BCEF0
    void ParseSubtitles(const CWideString& filename);
};
void CMovieSubtitles::ParseSubtitles(const CWideString& filename)
{
    WordWrap_SetOption(0, 1);
    WordWrap_SetCallback(GetCharWidthW, 0);
    FrameCounter = 0;
    IsSubtitleTime = false;
    CWideString drive;
    CWideString dir;
    CWideString name;
    CAFile::SplitPath(filename, &drive, &dir, &name, 0);
    name = CAFile::MakePath(drive, dir, name, CWideString(L"txt"));
    SubTitles.clear();
    if (pTextBank && CAFile::PathExists(name)) {
        CDiskFileWin32 file(name, 1, 2);
        CCharString text;
        text.LoadFromFile(file, 0, -1);
        file.Close();
        CStringParser parser(text, 0, CCharString(""), 0);
        CParsedItem item;
        while (parser.NextItemExists()) {
            CCharString key = parser.ReadAsString();
            long startMinutes = parser.ReadAsInteger();
            parser.ReadNextItem(item);
            float startSeconds = parser.ReadAsFloat();
            long endMinutes = parser.ReadAsInteger();
            parser.ReadNextItem(item);
            float endSeconds = parser.ReadAsFloat();
            unsigned long index = pTextBank->LookupValue(GetCRC32(key));
            if (index && pTextBank->GetTextInfo(index)) {
                CSubTitle subtitle;
                subtitle.TextEntry = pTextBank->GetPTextEntry(index, true);
                float startTime = (float)(startMinutes * 60) + startSeconds;
                float fps = (float)Fps;
                subtitle.FrameStart = (long)(fps * startTime);
                float endTime = (float)(endMinutes * 60) + endSeconds;
                subtitle.FrameEnd = (long)(fps * endTime);
                subtitle.StartTime = startTime;
                subtitle.EndTime = endTime;
                subtitle.TextEntry->GetText().GetLength();
                SubTitles.push_back(subtitle);
            }
        }
        CCharString fontName("ENG_ARIAL_18");
        g_SubtitleFont = GFGetSystemManager()->FontManager->GetFont(fontName);
        PSoundManager = GFGetSystemManager()->SoundManager;
    }
}
