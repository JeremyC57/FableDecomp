// CParsedItem::ConvertToString() const -- the item as text: its string for the two string kinds, the formatted integer / float (6 digits), else "NULL"
// Grade: SKIP (row holds two functions) -- the body is byte-identical through the jump table (128 bytes, 0x9B98B0..0x9B992F); the row also covers the separate 3-byte `ret 4` function at 0x9B9930 (16-aligned, no padding between)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B98B0, length 114 bytes (jump table to 0x009B9930; padded to 144).
// Callees: GFIntToCharString(long) @ 0x0099F830 (`__fastcall`, hidden CCharString result in ecx, value in edx),
// CCharString::FormatDouble(long digits, double) @ 0x009A08B0 (static `__fastcall`, result in ecx, digits in edx,
// the double on the stack), CCharString(const CCharString&) @ 0x0099EC30, CCharString(const char*, long) @ 0x0099EBF0
// ("NULL" at 0x012393E4).
// Layout: Ego_r.pdb CParsedItem (12 bytes): Type +0, Integer/Float union +4, String +8.
// Shape: a dense switch over Type 1..4 (four-entry jump table, the two string kinds share one arm; arms laid out in source order: integer, float, string, default) returning by
// hidden pointer; every arm constructs straight into the result.
enum EParsedItemType { PIT_NONE, PIT_STRING, PIT_QUOTED_STRING, PIT_INTEGER, PIT_FLOAT };

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);                      // 0x0099EC30
    CCharString(const char* text, long length);                 // 0x0099EBF0
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};
CCharString __fastcall GFIntToCharString(long value);            // 0x0099F830

struct CParsedItem {
    EParsedItemType Type;                   // +0
    union {
        long  Integer;                      // +4
        float Float;
    };
    CCharString String;                     // +8
    CCharString ConvertToString() const;
};

CCharString CParsedItem::ConvertToString() const
{
    switch (Type) {
    case PIT_INTEGER:
        return GFIntToCharString(Integer);
    case PIT_FLOAT:
        return CCharString::FormatDouble(6, Float);
    case PIT_STRING:
    case PIT_QUOTED_STRING:
        return String;
    default:
        return CCharString("NULL", -1);
    }
}
