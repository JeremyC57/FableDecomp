// CGenericVar::ReadFromParserAsSLONG(CStringParser& parser) -- read the parser's next item (0x009BA080) and, when it is a number (type 3), store it in the long at +0xc; a CCharString local is built and destroyed around the read
// Leaf: ?ReadFromParserAsSLONG@CGenericVar@@IAE_NAAVCStringParser@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A640D0, length 96 bytes.
// Callees: CCharString::CCharString @ 0x0099E4B0 and ~CCharString @ 0x0099EAE0 (the unused local), CStringParser::ReadNextItem @ 0x009BA080 (the 8-byte item by
// address, a bool in al), the item's long accessor @ 0x009B9890 (labelled "CWorld::DrawGetEnvironment", propagated). Shape: a 12-byte frame (the item with its string),
// the two destructor tails, `ret 4`.
// Fix (sweep 3): the item is the PDB's CParsedItem (was the invented SParsedItem stand-in) and its accessors are bound under
//   their PDB names, CParsedItem::GetInteger() const 0x009B9890 (retail body order = the PDB member run
//   GetSymbol, GetString, GetInteger, GetFloat at 0x009B9870..0x009B98A0); class keys, members and access (protected) are
//   Ego_r.pdb's (CGenericVar, the FableWin/Ego_r symbol ?ReadFromParserAsSLONG@CGenericVar@@IAE_NAAVCStringParser@@@Z). Same bytes.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                  // +0x00
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};
enum EParsedItemType { };                          // 1 symbol, 2 string, 3 integer, 4 float (CParsedItem::InitAs* stores)
class CParsedItem {                                // Ego_r.pdb, 12 bytes
public:
    EParsedItemType Type;                          // +0x00
    union {                                        // +0x04 (CParsedItem_u_4; member names descriptive)
        long  Integer;
        float Float;
    };
    CCharString     String;                        // +0x08
    long  GetInteger() const;                      // 0x009B9890
};
class CStringParser {
public:
    bool ReadNextItem(CParsedItem& item);          // 0x009BA080
};
enum EGenericVarType { };
class CGenericVar {                                // Ego_r.pdb, 28 bytes (polymorphic: vtable 0x0129C3E4)
public:
    virtual ~CGenericVar();
    EGenericVarType Type;                          // +0x04
    CCharString     ValString;                     // +0x08
    long            ValSLONG;                      // +0x0C
    unsigned long   ValULONG;                      // +0x10
    bool            ValBool;                       // +0x14
    float           ValFloat;                      // +0x18
protected:
    bool ReadFromParserAsSLONG(CStringParser& parser);
};

bool CGenericVar::ReadFromParserAsSLONG(CStringParser& parser)
{
    CParsedItem item;
    if (parser.ReadNextItem(item) && item.Type == 3) {
        ValSLONG = item.GetInteger();
        return true;
    }
    return false;
}
