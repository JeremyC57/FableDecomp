// CGenericVarRef::ReadFromParserAsFloat(CStringParser& parser) -- reads the parser's next item into the referenced float (pointer at +4): an integer item (type 3) converted, a float item (type 4) as is; false for anything else
// Leaf: ?ReadFromParserAsFloat@CGenericVarRef@@AAE_NAAVCStringParser@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A63D20, length 135 bytes (padded to 144).
// Callees: CCharString::CCharString @ 0x0099E4B0 / ~CCharString @ 0x0099EAE0 (the item's text), CStringParser::ReadNextItem
//          @ 0x009BA080, the item's long accessor @ 0x009B9890 (manifest "CWorld::DrawGetEnvironment", propagated) and float
//          accessor @ 0x009B98A0 (manifest "CMap::DrawGetSeaLevel", propagated).
// Shape: twin of CGenericVar::ReadFromParserAsFloat (0x00A641A0) through the value pointer; the landed
// CGenericVarRef::ReadFromParserAsSLONG (0x00A63CC0) item model -- a 12-byte frame item that owns its string; each
// successful arm runs its own item destructor before `mov al,1`; the long is converted through the dead parser argument
// slot (`mov [esp+14h],eax ; fild`).
// Fix (sweep 3): the item is the PDB's CParsedItem (was the invented SParsedItem stand-in) and its accessors are bound under
//   their PDB names, CParsedItem::GetInteger() const 0x009B9890 / GetFloat() const 0x009B98A0 (retail body order = the PDB member run
//   GetSymbol, GetString, GetInteger, GetFloat at 0x009B9870..0x009B98A0); class keys, members and access (private) are
//   Ego_r.pdb's (CGenericVarRef, the FableWin/Ego_r symbol ?ReadFromParserAsFloat@CGenericVarRef@@AAE_NAAVCStringParser@@@Z). Same bytes.
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
    float GetFloat() const;                        // 0x009B98A0
};
class CStringParser {
public:
    bool ReadNextItem(CParsedItem& item);          // 0x009BA080
};
enum EGenericVarType { };
class CGenericVarRef {                             // Ego_r.pdb, 8 bytes
public:
    EGenericVarType Type;                          // +0x00
    union {                                        // +0x04 (CGenericVarRef_u_4: the referenced value; names descriptive)
        long*          PValSLONG;
        unsigned long* PValULONG;
        float*         PValFloat;
        bool*          PValBool;
        CCharString*   PValString;
    };
private:
    bool ReadFromParserAsFloat(CStringParser& parser);
};

bool CGenericVarRef::ReadFromParserAsFloat(CStringParser& parser)
{
    CParsedItem item;
    if (parser.ReadNextItem(item)) {
        if (item.Type == 3) {
            *PValFloat = (float)item.GetInteger();
            return true;
        }
        if (item.Type == 4) {
            *PValFloat = item.GetFloat();
            return true;
        }
    }
    return false;
}
