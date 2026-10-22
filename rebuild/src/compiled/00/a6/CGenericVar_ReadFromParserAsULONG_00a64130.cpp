// CGenericVar_00a64130::ReadFromParserAsULONG(CStringParser& parser) -- read the parser's next item (0x009BA080) and, when it is a number (type 3), store it in the long at +0x10 (a twin of 0x00A640D0 with the value one word further); a CCharString local is built and destroyed around the read
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A64130, length 88 bytes.
// Callees: CCharString::CCharString @ 0x0099E4B0 and ~CCharString @ 0x0099EAE0 (the unused local), CStringParser::ReadNextItem @ 0x009BA080 (the 8-byte item by
// address, a bool in al), the item's long accessor @ 0x009B9890 (labelled "CWorld::DrawGetEnvironment", propagated). Shape: a 12-byte frame (the item with its string),
// the two destructor tails, `ret 4`.
struct CCharString {
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};
struct SParsedItem_00a64130 {                          // 12 bytes: the item type, its value and the text it was read from (constructed / destroyed with the item)
    long        m_type;                            // +0x00 (3 = a number)
    long        m_value;                           // +0x04
    CCharString m_text;                            // +0x08
    long GetAsLong();                              // 0x009B9890
};
struct CStringParser {
    bool ReadNextItem(SParsedItem_00a64130& item);      // 0x009BA080
};

struct CGenericVar_00a64130 {
    unsigned char _pad[0x10];
    long          m_value;                         // +0x10
    bool ReadFromParserAsULONG(CStringParser& parser);
};

bool CGenericVar_00a64130::ReadFromParserAsULONG(CStringParser& parser)
{
    SParsedItem_00a64130 item;                          // +4 (its string at +0xc)
    if (parser.ReadNextItem(item) && item.m_type == 3) {
        m_value = item.GetAsLong();
        return true;
    }
    return false;
}