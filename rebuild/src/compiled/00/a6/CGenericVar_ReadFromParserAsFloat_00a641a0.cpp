// CGenericVar::ReadFromParserAsFloat(CStringParser& parser) -- reads the parser's next item into the float at +0x18: an integer item (type 3) converted, a float item (type 4) as is; false for anything else
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A641A0, length 131 bytes (padded to 144).
// Callees: CCharString::CCharString @ 0x0099E4B0 / ~CCharString @ 0x0099EAE0 (the item's text), CStringParser::ReadNextItem
//          @ 0x009BA080, the item's long accessor @ 0x009B9890 (manifest "CWorld::DrawGetEnvironment", propagated) and float
//          accessor @ 0x009B98A0 (manifest "CMap::DrawGetSeaLevel", propagated).
// Shape: the landed ReadFromParserAsSLONG (0x00A640D0) item model -- a 12-byte frame item that owns its string; each
// successful arm runs its own item destructor before `mov al,1`; the long is converted through the dead parser argument
// slot (`mov [esp+14h],eax ; fild`).
class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};
struct SParsedItem {                               // 12 bytes: the item type, its value and the text it was read from
    long        m_type;                            // +0x00 (3 = an integer, 4 = a float)
    long        m_value;                           // +0x04
    CCharString m_text;                            // +0x08
    long  GetAsLong();                             // 0x009B9890
    float GetAsFloat();                            // 0x009B98A0
};
class CStringParser { public:
    bool ReadNextItem(SParsedItem& item);          // 0x009BA080
};
class CGenericVar { public:
    unsigned char _pad[0x18];
    float         m_floatValue;                    // +0x18
    bool ReadFromParserAsFloat(CStringParser& parser);
};

bool CGenericVar::ReadFromParserAsFloat(CStringParser& parser)
{
    SParsedItem item;
    if (parser.ReadNextItem(item)) {
        if (item.m_type == 3) {
            m_floatValue = (float)item.GetAsLong();
            return true;
        }
        if (item.m_type == 4) {
            m_floatValue = item.GetAsFloat();
            return true;
        }
    }
    return false;
}