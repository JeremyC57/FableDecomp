// CAFile::ParserIsDriveLabelNext(CStringParser& parser) -- peek: does a drive label ("<name>:") come next? The parser position is restored
// Retail: Fable.exe (TLC, VC7.1) @ 0x00998210, 59 bytes. FableWin.pdb ?ParserIsDriveLabelNext@CAFile@@CI_NAAVCStringParser@@@Z:
// a private static `__fastcall` (parser in ecx), plain `ret`.
// Callees: CCharString() @ 0x0099E4B0 (the item's string), CAFile::ParserReadPastDriveLabel(CStringParser&,
// CParsedItem&) @ 0x00998170 (static `__fastcall`), CStringParser::SetStringPos(unsigned long) @ 0x009B9940,
// ~CCharString @ 0x0099EAE0.
// Layout: retail CStringParser is 0x28 bytes (STLport list of comment boundaries is 4 bytes, not the PDB's 0xC), so
// CursorPos (CCursorPos {bool InQuotes, ulong StringPos, ulong Line}) sits at +0x1C and StringPos at +0x20;
// engine/CStringParser.h carries the Ego_r 0x30 layout (CursorPos +0x24), hence the local model.
// CParsedItem {Type +0, union +4, CCharString String +8} has a user ctor that only builds the string.
// Shape: the position is read (inline GetStringPos) before the item is built; the bool result is kept in bl across
// SetStringPos and the item's destructor.
// Leaf: ?ParserIsDriveLabelNext@CAFile
class CCharStringData;
class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};

class CParsedItem
{
public:
    long        Type;                                   // +0x00 EParsedItemType
    long        Value;                                  // +0x04 numeric union
    CCharString String;                                 // +0x08
    CParsedItem() { }
};

struct CCursorPos {
    bool          InQuotes;                             // +0x00
    unsigned long StringPos;                            // +0x04
    unsigned long Line;                                 // +0x08
};

class CStringParser                                     // retail layout, 0x28 bytes
{
public:
    void*             __vftable;                        // +0x00
    const char*       String;                           // +0x04
    unsigned long     BaseLineOffset;                   // +0x08
    CCharString       Filename;                         // +0x0C
    CCharString       TempCharString;                   // +0x10
    void*             Comments;                         // +0x14 STLport list<CStringParserCommentBoundaries> (4 bytes)
    void*             ErrorCallback;                    // +0x18
    CCursorPos        CursorPos;                        // +0x1C
    unsigned long GetStringPos() const { return CursorPos.StringPos; }
    void SetStringPos(unsigned long pos);               // 0x009B9940
};

class CAFile
{
public:
    static bool __fastcall ParserReadPastDriveLabel(CStringParser& parser, CParsedItem& item);   // 0x00998170
    static bool __fastcall ParserIsDriveLabelNext(CStringParser& parser);
};

bool __fastcall CAFile::ParserIsDriveLabelNext(CStringParser& parser)
{
    unsigned long pos = parser.GetStringPos();
    CParsedItem item;
    bool result = ParserReadPastDriveLabel(parser, item);
    parser.SetStringPos(pos);
    return result;
}