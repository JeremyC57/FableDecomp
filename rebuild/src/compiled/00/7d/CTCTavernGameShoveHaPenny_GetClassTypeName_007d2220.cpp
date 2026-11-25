// CTCTavernGameShoveHaPenny::GetClassTypeName @ 0x007d2220
// __fastcall member returning a CCharString by value, constructed from a
// literal C-string with len=-1. The this pointer is unused.

class CCharString { public:
    char* m_data;
    int   m_len;
    int   m_ref;
    CCharString(const char* s, long len);
};


struct CTCTavernGameShoveHaPenny {
    CCharString GetClassTypeName();
};

CCharString CTCTavernGameShoveHaPenny::GetClassTypeName()
{
    return CCharString("CTCTavernGameShoveHaPenny", -1);
}