// CParsedItem::GetFloat() const -- the float value (the +4 union)
// Leaf: ?GetFloat@CParsedItem@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B98A0, 4 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb place CParsedItem's members InitAsSymbol, InitAsInteger, InitAsFloat, InitAsString
//   x2, GetSymbol, GetString, GetInteger, GetFloat, ConvertToString; retail has the same run: the InitAs bodies storing
//   types 3 (0x009B9745 / 0x009B975B), 4 (0x009B97FD), 2 (0x009B9845 / 0x009B9860), then 0x009B9870 / 0x009B9880 (`lea
//   eax,[ecx+8]`), 0x009B9890 (`mov eax,[ecx+4]`), 0x009B98A0 (`fld [ecx+4]`) and the landed CParsedItem::ConvertToString
//   0x009B98B0. Catalog "_global::GetMember_float0x4" is descriptive and the manifest's CMap::DrawGetSeaLevel propagated.
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
    float GetFloat() const;
};
float CParsedItem::GetFloat() const
{
    return Float;
}
