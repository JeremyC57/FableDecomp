// CParsedItem::GetSymbol() const -- the symbol text (the String member at +8)
// Leaf: ?GetSymbol@CParsedItem@@QBEABVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B9870, 4 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb place CParsedItem's members InitAsSymbol, InitAsInteger, InitAsFloat, InitAsString
//   x2, GetSymbol, GetString, GetInteger, GetFloat, ConvertToString; retail has the same run: the InitAs bodies storing
//   types 3 (0x009B9745 / 0x009B975B), 4 (0x009B97FD), 2 (0x009B9845 / 0x009B9860), then 0x009B9870 / 0x009B9880 (`lea
//   eax,[ecx+8]`), 0x009B9890 (`mov eax,[ecx+4]`), 0x009B98A0 (`fld [ecx+4]`) and the landed CParsedItem::ConvertToString
//   0x009B98B0. Callers include CAFile::ParserReadPastDriveLabel 0x00998170, which reads a type-1 item (a symbol) and
//   compares it with ":". Earlier landed as a junk `C009b9870::M` stand-in (manifest CModeBlock::GetAnimFilter,
//   propagated) and catalogued as a second CParsedItem::GetString (FINDINGS chunk 482: 0x009B9870 is GetSymbol, 0x009B9880
//   GetString).
// Layout: Ego_r.pdb CParsedItem (12 bytes): EParsedItemType Type +0, an anonymous union +4 (CParsedItem_u_4: the integer /
//   float value; the member names Integer / Float are descriptive, the PDB keeps none), CCharString String +8.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                       // +0x00
};
enum EParsedItemType { };
class CParsedItem {
public:
    EParsedItemType Type;                               // +0x00
    union {
        long  Integer;                                  // +0x04 (descriptive)
        float Float;
    };
    CCharString     String;                             // +0x08
    const CCharString& GetSymbol() const;
};
const CCharString& CParsedItem::GetSymbol() const
{
    return String;
}
