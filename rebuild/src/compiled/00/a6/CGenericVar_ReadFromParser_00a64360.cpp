// CGenericVar::ReadFromParser(CStringParser& parser) -- read the variable's value from the parser according to its type (1 long, 2 unsigned long, 3 float, 4 bool, 5 quoted string); false for an unknown type
// Leaf: ?ReadFromParser@CGenericVar
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A64360, 80 bytes incl. the 5-entry jump table, `ret 4`.
// Callees (all tail jumps but the string arm): CGenericVar::ReadFromParserAsSLONG 0x00A640D0, ReadFromParserAsULONG
//   0x00A64130 (landed as "CGenericVar_00a64130::ReadFromParserAsSLONG": the same read into the word at +0x10, ValULONG),
//   ReadFromParserAsFloat 0x00A641A0, ReadFromParserAsBool 0x00A64230, and for the string arm the inlined
//   ReadFromParserAsString = CStringParser::ReadNextItemAsQuotedString(&ValString) 0x009BA1D0.
// Shape: the landed CGenericVarRef::ReadFromParser 0x00A63FC0 switch (cases written 3,5,4,1 then 2, so the arms are laid out
//   float, string, bool, long, ulong); `dec eax ; cmp eax,4 ; ja` + table, the default `xor al,al`.
// Layout: CGenericVar (Ego_r.pdb): Padding +0, Type +4, ValString +8, ValSLONG +0xC, ValULONG +0x10, ValBool +0x14,
//   ValFloat +0x18 (as modelled in the landed ReadFromParserAsBool 0x00A64230); no generated header exists.
struct CCharString {
    void* m_pData;
};
struct CStringParser {
    bool ReadNextItemAsQuotedString(CCharString& text);   // 0x009BA1D0
};

struct CGenericVar {
    long          Padding;                         // +0x00
    long          Type;                            // +0x04
    CCharString   ValString;                       // +0x08
    long          ValSLONG;                        // +0x0C
    unsigned long ValULONG;                        // +0x10
    bool          ValBool;                         // +0x14
    float         ValFloat;                        // +0x18
    bool ReadFromParserAsSLONG(CStringParser& parser);    // 0x00A640D0
    bool ReadFromParserAsULONG(CStringParser& parser);    // 0x00A64130
    bool ReadFromParserAsFloat(CStringParser& parser);    // 0x00A641A0
    bool ReadFromParserAsBool(CStringParser& parser);     // 0x00A64230
    bool ReadFromParserAsString(CStringParser& parser) { return parser.ReadNextItemAsQuotedString(ValString); }
    bool ReadFromParser(CStringParser& parser);
};

bool CGenericVar::ReadFromParser(CStringParser& parser)
{
    switch (Type) {
    case 3:
        return ReadFromParserAsFloat(parser);
    case 5:
        return ReadFromParserAsString(parser);
    case 4:
        return ReadFromParserAsBool(parser);
    case 1:
        return ReadFromParserAsSLONG(parser);
    case 2:
        return ReadFromParserAsULONG(parser);
    default:
        return false;
    }
}