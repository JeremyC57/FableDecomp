// CAFile::IsFilePathSyntaxValid(const CWideString& filePath, bool absolute) -- check a path's syntax: at most 260 characters, then a small state machine over drive labels, separators and components (an absolute path must open with a drive label, a relative one with a component) that must end where an end is allowed
// Leaf: ?IsFilePathSyntaxValid@CAFile@@
// Flags: /Oa
// Retail: Fable.exe (TLC, VC7.1) @ 0x00998A30, 512 bytes (incl. int3 padding). FableWin.pdb
//   `?IsFilePathSyntaxValid@CAFile@@SI_NABVCWideString@@_N@Z`: a public static `__fastcall` (path in ecx, the bool in dl),
//   plain `ret`. The catalog name is the PDB name.
// Callees: CWideString::ConvertToCharString 0x0099B2C0, ~CCharString 0x0099EAE0, CCharString(const char*, long = -1)
//   0x0099EBF0, CStringParser(const CCharString&, callback, const CCharString&, long) 0x00404720, CCharString()
//   0x0099E4B0, CAFile::ParserIsSeparatorNext 0x00997DA0, CAFile::ParserReadPastSeparator 0x00997E90 (the catalog's
//   second "ParserIsSeparatorNext" label), CAFile::ParserReadPastComponent 0x00998010 (this chunk),
//   CStringParser::SetStringPos 0x009B9940, CAFile::ParserReadPastDriveLabel 0x00998170, CStringParser::NextItemExists
//   0x009BA4F0, the comment list's destructor 0x004046B0, CBaseClass::~CBaseClass 0x0099A2E0. Literal "" 0x0122D70E.
// Layout: CAFile::CExpectedTextType {ComponentOK, SeparatorOK, DriveLabelOK, FinishedOK} (Ego_r.pdb, 4 bools);
//   CParsedItem {Type +0, union +4, CCharString String +8}; retail CStringParser (0x28 bytes): vptr, String +4,
//   BaseLineOffset +8, Filename +0xC, TempCharString +0x10, the 4-byte STLport comment list +0x14, ErrorCallback +0x18,
//   CursorPos +0x1C (12 bytes; StringPos +0x20).
// Shape: FableWin.pdb's ParserIsComponentNext / ParserIsDriveLabelNext / ParserIsFinished are inline here (save the
//   string position, try the read into a scratch component or item, restore the position); the loop runs while ok and
//   not finished and returns `ok && finished`; /Oa is the CAFile parser TU's flag (as ParserReadPastComponent 0x00998010).
#include <stddef.h>
extern const char g_emptyLiteral_0129aaf4[];            // 0x0129AAF4: ""

class CCharStringData;
class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00 {PData +0, StringLength +4}
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    long GetLength() const { return PStringData == 0 ? 0 : ((const long*)PStringData)[1]; }
};

class CWideString
{
public:
    void* PStringData;
    CCharString ConvertToCharString() const;            // 0x0099B2C0
};

class CBaseClass {
public:
    CBaseClass();                                            // 0x0099A2D0
    virtual ~CBaseClass();                                   // 0x0099A2E0
};

class CStringParserCommentBoundaries;
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class list
{
public:
    void* _M_node;
    ~list();                                            // 0x004046B0 (CStringParserCommentBoundaries)
};
} // namespace std

class CStringParser : public CBaseClass                      // vtable 0x0122D874, 0x28 bytes
{
public:
    const char*   String;                               // +0x04
    unsigned long BaseLineOffset;                       // +0x08
    CCharString   Filename;                             // +0x0C
    CCharString   TempCharString;                       // +0x10
    std::list<CStringParserCommentBoundaries> Comments; // +0x14
    void (*ErrorCallback)(CCharString*);                // +0x18
    unsigned long CursorLine;                           // +0x1C CursorPos (12 bytes)
    unsigned long StringPos;                            // +0x20
    unsigned long CursorColumn;                         // +0x24
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename, long baseLineOffset);   // 0x00404720
    bool NextItemExists();                              // 0x009BA4F0
    unsigned long GetStringPos() const { return StringPos; }
    void SetStringPos(unsigned long pos);               // 0x009B9940
};

class CParsedItem                                       // 12 bytes
{
public:
    long        Type;                                   // +0x00 EParsedItemType
    long        Value;                                  // +0x04 (union)
    CCharString String;                                 // +0x08
};

class CAFile
{
public:
    class CExpectedTextType
    {
    public:
        bool ComponentOK;                               // +0
        bool SeparatorOK;                               // +1
        bool DriveLabelOK;                              // +2
        bool FinishedOK;                                // +3
        void Set(bool componentOK, bool separatorOK, bool driveLabelOK, bool finishedOK)
        {
            ComponentOK = componentOK;
            SeparatorOK = separatorOK;
            DriveLabelOK = driveLabelOK;
            FinishedOK = finishedOK;
        }
    };
    static bool __fastcall IsFilePathSyntaxValid(const CWideString& filePath, bool absolute);
private:
    static bool __fastcall ParserIsSeparatorNext(CStringParser& parser);                         // 0x00997DA0
    static bool __fastcall ParserReadPastSeparator(CStringParser& parser);                       // 0x00997E90
    static bool __fastcall ParserReadPastComponent(CStringParser& parser, CCharString& component);   // 0x00998010
    static bool __fastcall ParserReadPastDriveLabel(CStringParser& parser, CParsedItem& item);   // 0x00998170
    static bool __fastcall ParserIsComponentNext(CStringParser& parser);
    static bool __fastcall ParserIsDriveLabelNext(CStringParser& parser);
    static bool __fastcall ParserIsFinished(CStringParser& parser);
};

inline bool __fastcall CAFile::ParserIsComponentNext(CStringParser& parser)
{
    unsigned long pos = parser.GetStringPos();
    CCharString component("");
    bool result = ParserReadPastComponent(parser, component);
    parser.SetStringPos(pos);
    return result;
}

inline bool __fastcall CAFile::ParserIsDriveLabelNext(CStringParser& parser)
{
    unsigned long pos = parser.GetStringPos();
    CParsedItem item;
    bool result = ParserReadPastDriveLabel(parser, item);
    parser.SetStringPos(pos);
    return result;
}

inline bool __fastcall CAFile::ParserIsFinished(CStringParser& parser)
{
    return !parser.NextItemExists();
}

bool __fastcall CAFile::IsFilePathSyntaxValid(const CWideString& filePath, bool absolute)
{
    CCharString path = filePath.ConvertToCharString();
    if (path.GetLength() > 260)
        return false;
    CStringParser parser(path, 0, CCharString(""), 0);
    CParsedItem item;
    CExpectedTextType expected;
    if (absolute)
        expected.Set(false, false, true, false);
    else
        expected.Set(true, false, false, false);
    bool ok = true;
    bool finished = false;
    while (ok && !finished) {
        if (ParserIsSeparatorNext(parser)) {
            ok = expected.SeparatorOK;
            expected.Set(true, false, false, false);
            ParserReadPastSeparator(parser);
        } else if (ParserIsComponentNext(parser)) {
            ok = expected.ComponentOK;
            expected.Set(false, true, false, true);
            CCharString component;
            ParserReadPastComponent(parser, component);
        } else if (ParserIsDriveLabelNext(parser)) {
            ok = expected.DriveLabelOK;
            expected.Set(false, true, false, false);
            ParserReadPastDriveLabel(parser, item);
        } else if (ParserIsFinished(parser)) {
            ok = expected.FinishedOK;
            finished = true;
        } else
            ok = false;
    }
    return ok && finished;
}