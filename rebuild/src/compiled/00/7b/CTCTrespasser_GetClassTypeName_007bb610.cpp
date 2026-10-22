// CTCTrespasser::GetClassTypeName @ 0x007bb610
// __fastcall member returning a CCharString by value, constructed from a
// literal C-string with len=-1. The this pointer is unused.

struct CCharString {
    char* m_data;
    int   m_len;
    int   m_ref;
    CCharString(const char* s, int len);
};

static const char* const kActionNameLit = (const char*)0x0123cb18;

struct CTCTrespasser {
    CCharString GetClassTypeName();
};

CCharString CTCTrespasser::GetClassTypeName()
{
    return CCharString(kActionNameLit, -1);
}