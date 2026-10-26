// Behaviour test for CPersistContext::TransferVectorSaveText (0x009BAC80): one `name[i] value;\r\n` line per element.
#include <cstdio>
#include <cstring>
class CCharString { public:
    char Text[256];
    CCharString(const char* text, long = -1) { std::strcpy(Text, text); }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); }
    ~CCharString() {}
    CCharString& operator+=(const char* text) { std::strcat(Text, text); return *this; }
    CCharString& operator+=(const CCharString& other) { std::strcat(Text, other.Text); return *this; }
};
CCharString GFIntToCharString(long value) { char b[16]; std::sprintf(b, "%ld", value); return CCharString(b); }
CCharString operator+(const CCharString& left, const char* right) { CCharString r(left); r += right; return r; }
CCharString operator+(const CCharString& left, const CCharString& right) { CCharString r(left); r += right; return r; }
class CPersistContext;
class CStringParser;
class CPersistContext_TransferableVectorGenericType { public:
    virtual long GetCount() = 0;
    virtual void Resize(long) {}
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index) = 0;
    virtual void TransferIn(CPersistContext*, CStringParser*, long) {}
    virtual ~CPersistContext_TransferableVectorGenericType() {}
};
class CPersistContext { public:
    CCharString* PStringToSaveTo;
    void TransferVectorSaveText(const char* name, CPersistContext_TransferableVectorGenericType& vec)
    {
        long count = vec.GetCount();
        for (long i = 0; i < count; ++i) {
            *PStringToSaveTo += name;
            *PStringToSaveTo += CCharString("") + "[" + GFIntToCharString(i) + "] ";
            vec.TransferOut(this, PStringToSaveTo, i);
            *PStringToSaveTo += ";\r\n";
        }
    }
};
struct Values : CPersistContext_TransferableVectorGenericType {
    long n; CPersistContext* seen;
    long GetCount() { return n; }
    void TransferOut(CPersistContext* context, CCharString* out, long index) { seen = context; char b[8]; std::sprintf(b, "%ld", index * 10); *out += b; }
};
int main()
{
    CCharString text("");
    CPersistContext c; c.PStringToSaveTo = &text;
    Values v; v.n = 3; v.seen = 0;
    c.TransferVectorSaveText("Tags", v);
    if (std::strcmp(text.Text, "Tags[0] 0;\r\nTags[1] 10;\r\nTags[2] 20;\r\n") != 0 || v.seen != &c) { std::printf("009bac80_TEST FAIL\n"); return 1; }
    CCharString empty(""); c.PStringToSaveTo = &empty; v.n = 0;
    c.TransferVectorSaveText("Tags", v);
    if (empty.Text[0] != 0) { std::printf("009bac80_TEST FAIL\n"); return 1; }
    std::printf("009bac80_TEST PASS\n");
    return 0;
}