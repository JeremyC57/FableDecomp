// CStringParser::ReadAsStringUntilString  (0x009ba110)
//
// Scans the parser's buffer (starting at m_pos within m_buffer) for a
// matching terminator/substring using SkipUntilWholeString 0x009B9AA0 (updates m_pos
// on success). If found, builds a temporary CCharString slice spanning the
// consumed bytes [oldPos, newPos), assigns it into item->value, marks
// item->type = 2 (string), and returns true. If not found, returns false.
// Name (2026-10-31, hand621 NAME_FIXES follow-up): 0x009B9AA0 is ?SkipUntilWholeString@CStringParser@@QAE_NABVCCharString@@@Z
// (Ego_r.pdb donor order; the landed 0x009B9AA0 body), not FindMatchingSubstring_009b9aa0(CCharString*). Bytes unchanged.

class CCharString
{
public:
    CCharString() : data(0) {}
    CCharString(const char* text, long length);
    ~CCharString();
    const CCharString& operator=(const CCharString& rhs);

    void* data;
};

class CParsedItem
{ public:
    long type;         // +0x00
    long m_pad0;        // +0x04
    CCharString value; // +0x08 (embedded)
};

class CStringParser
{
public:
    bool SkipUntilWholeString(const CCharString& word);   // 0x009B9AA0
    bool ReadAsStringUntilString(CCharString* terminator, CParsedItem* item);

    long m_unk0;    // +0x00
    long m_pos;     // +0x04
    // ... unrelated fields between +0x08 and +0x20 not needed here.
    long m_pad[6];  // +0x08 .. +0x1f
    char* m_buffer; // +0x20
};

bool CStringParser::ReadAsStringUntilString(CCharString* terminator, CParsedItem* item)
{
    char* scanStart = m_buffer + m_pos;

    if (SkipUntilWholeString(*terminator))
    {
        long matchLen = (m_pos + (long)m_buffer) - (long)scanStart;

        CCharString slice(scanStart, matchLen);
        item->value = slice;
        item->type = 2;
        return true;
    }
    return false;
}