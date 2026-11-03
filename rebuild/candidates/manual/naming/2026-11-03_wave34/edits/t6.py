A="rebuild/src/compiled/00/40/CPersistTraitsfloat_TransferIn_0040f7e0.cpp"
B="rebuild/src/compiled/00/9e/global_TransferIn_009e2590.cpp"
EDITS=[
(A,"""class CStringParser : public CStringParserLayout
{
public:
    CCharString ReadAsString();   // 0x009BA6F0
};""","""enum EParsedItemType { PARSED_ITEM_SYMBOL = 2 };              // Ego_r / FableWin global enum (enumerator name descriptive)
class CStringParser : public CStringParserLayout
{
public:
    CCharString ReadAsString();                                // 0x009BA6F0
    EParsedItemType PeekNextItemType();                        // 0x009BA900 (Ego_r / FableWin ?PeekNextItemType@CStringParser@@QAE?AW4EParsedItemType@@XZ)
    float ReadAsFloat();                                       // 0x009BA650
};"""),
(A,"""struct CStringParserMethods : CStringParser {
    long PeekNextItemType();                                   // 0x009BA900 (EParsedItemType)
    float ReadAsFloat();                                       // 0x009BA650
};
""",""),
(A,"enum { PARSED_ITEM_SYMBOL = 2 };\n",""),
(A,"    CStringParserMethods& p = static_cast<CStringParserMethods&>(parser);\n","    CStringParser& p = parser;\n"),
(B,"""class CStringParser : public CStringParserLayout
{
public:
    long ReadAsInteger();   // 0x009BA540
};""","""enum EParsedItemType { PARSED_ITEM_SYMBOL = 2 };              // Ego_r / FableWin global enum (enumerator name descriptive)
class CStringParser : public CStringParserLayout
{
public:
    long ReadAsInteger();                                      // 0x009BA540
    EParsedItemType PeekNextItemType();                        // 0x009BA900 (Ego_r / FableWin)
    CCharString ReadAsString();                                // 0x009BA6F0 (Ego_r / FableWin)
    bool SkipPastNextItemIfIs(const CCharString& item);        // 0x009B9E00 (Ego_r / FableWin)
};"""),
(B,"""struct CStringParserMethods : CStringParser {
    long PeekNextItemType();                                   // 0x009BA900 (EParsedItemType)
    CCharString ReadAsString();                                // 0x009BA6F0
    bool SkipPastNextItemIfIs(const CCharString& item);        // 0x009B9E00
};
""",""),
(B,"enum { PARSED_ITEM_SYMBOL = 2 };\n",""),
(B,"    CStringParserMethods* p = static_cast<CStringParserMethods*>(parser);\n","    CStringParser* p = parser;\n"),
]
