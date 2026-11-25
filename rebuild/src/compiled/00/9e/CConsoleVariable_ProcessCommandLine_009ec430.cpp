// Naming (NF423, 2026-11-25): the local model of the +0x08 target is now named CGenericVarRef, its retail class (Ego_r.pdb and FableWin.pdb
//   CGenericVarRef::ReadFromParser / PrintToString), so the callees bind ?ReadFromParser@CGenericVarRef@@QAE_NAAVCStringParser@@@Z (0x00A63FC0,
//   the PDB spelling) and the landed self spelling of 0x00A63C00 (QBE; Ego_r.pdb and FableWin.pdb say QAE, NAME_FIXES.tsv). No shared header exists for the class.
// CConsoleVariable::ProcessCommandLine(const CCharString& line, CConsole& console) -- parse the command line after the
// variable name: with no value print "<name> = <value>"; otherwise read the target from the parser and, on failure,
// report the error and the variable's type (bool / float / slong / string)
// Leaf: ?ProcessCommandLine@CConsoleVariable@@EAE_NABVCCharString@@AAVCConsole@@@Z
// Callees: CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CStringParser ctor 0x00404720 and
//   its implicit inline dtor (~_List_base 0x004046B0, two ~CCharString, CBaseClass::~CBaseClass 0x0099A2E0), ReadNextItem
//   0x009BA080, NextItemExists 0x009BA4F0, CGenericVarRef::PrintToString 0x00A63C00 / ReadFromParser 0x00A63FC0,
//   operator+ 0x0099F600 / 0x0099F570 / 0x0099F690 (__fastcall), operator+= 0x0099F100, CConsole::SendMessageToUser 0x009EB260.
// Shape: the 12-byte parser item (type, value, text) is constructed first and filled by ReadNextItem; the printed value
//   and the type message reuse the dead parameter slots; the type names are an if / else-if chain (4, 3, 1, 5).
// Layout: CConsoleVariable vtable +0, Name +4, Target +8 (type enum first); CStringParser 0x28 bytes (landed 0x00CB8690).
// Naming (NF129, 2026-11-20, q4657): the parser item is CParsedItem (Ego_r.pdb / FableWin.pdb ?ReadNextItem@CStringParser@@QAE_NAAVCParsedItem@@@Z,
//   returning bool; engine/CParsedItem.h: Type +0, Integer/Float +4, String +8, compiler-generated ctor / dtor): the nested CItem stand-in
//   and the local CCharString model are replaced by engine/CParsedItem.h and fable_string.h (retail branch).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPARSEDITEM
#include "engine/CParsedItem.h"
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CStringParserCommentBoundaries;
// NF245 (2026-11-22, the 0x004046B0 family; nf243 ruling 2 (a)): the row-local std::allocator / std::list stand-ins replaced by the
//   shared STLport list (declaration-only _List_base ctor / dtor): the comment list's destructor call
//   binds ??1?$_List_base@VCStringParserCommentBoundaries@@..@std@@QAE@XZ (0x004046B0; list's own ~list is the 5-byte jmp 0x00404710).
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#include "stlport/fable_containers.h"
class CStringParser : public CBaseClass { public:                   // retail 0x28 bytes
    char*                    String;                  // +0x04
    unsigned long            BaseLineOffset;          // +0x08
    CCharString              Filename;                // +0x0c
    CCharString              TempCharString;          // +0x10
    std::list<CStringParserCommentBoundaries> Comments;                // +0x14
    void (*ErrorCallback)(CCharString*);              // +0x18
    long                     CursorPos[3];            // +0x1c
    CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename,
                  long baseLineOffset);               // 0x00404720
    bool ReadNextItem(CParsedItem& item);             // 0x009BA080 (result unused here)
    bool NextItemExists();                            // 0x009BA4F0
};
class CConsole { public:
    void SendMessageToUser(const CCharString& message);   // 0x009EB260
};
struct CGenericVarRef {
    long Type;                                        // 1 slong, 3 float, 4 bool, 5 string
    void PrintToString(CCharString& out) const;       // 0x00A63C00
    bool ReadFromParser(CStringParser& parser);       // 0x00A63FC0
};
class CConsoleVariable {
public:
    virtual void v00();
    CCharString    Name;                              // +0x04
    CGenericVarRef Target;                            // +0x08
private:
    virtual bool ProcessCommandLine(const CCharString& line, CConsole& console);
};

bool CConsoleVariable::ProcessCommandLine(const CCharString& line, CConsole& console)
{
    CParsedItem item;
    CStringParser parser(line, 0, CCharString(""), 0);
    parser.ReadNextItem(item);
    bool result;
    if (!parser.NextItemExists()) {
        CCharString value;
        Target.PrintToString(value);
        console.SendMessageToUser(Name + " = " + value);
        result = true;
    } else {
        result = Target.ReadFromParser(parser);
        if (!result) {
            console.SendMessageToUser("error - could not read target for variable");
            CCharString message = "type of " + Name + ": ";
            if (Target.Type == 4)
                message += "bool";
            else if (Target.Type == 3)
                message += "float";
            else if (Target.Type == 1)
                message += "slong";
            else if (Target.Type == 5)
                message += "string";
            console.SendMessageToUser(message);
        }
    }
    return result;
}