// Behaviour test for CBankFileEntryUpdateData::SetFilenames (0x009CC840): the filename list is resized
// to the source count (filled with empty strings) and every name copied in order.
#include <cstdio>
#include <cstring>

class CCharString { public: char text[16]; CCharString() { text[0] = 0; } };
struct CNames { const CCharString* m_pFirst; const CCharString* m_pLast; unsigned int size() const { return (unsigned int)(m_pLast - m_pFirst); } const CCharString& operator[](unsigned int n) const { return *(m_pFirst + n); } };
struct CSmallVector {
    CCharString data[8]; unsigned int count;
    void resize(unsigned int n, const CCharString& fill) { for (unsigned int i = count; i < n; ++i) data[i] = fill; count = n; }
    CCharString& operator[](unsigned int n) { return *(data + n); }
};
class CBankFileEntryUpdateData { public:
    CSmallVector Filenames;
    void SetFilenames(const CNames& names)
    {
        Filenames.resize(names.size(), CCharString());
        for (unsigned int i = 0; i < names.size(); ++i) Filenames[i] = names[i];
    }
};

int main()
{
    CCharString src[3];
    std::strcpy(src[0].text, "a.tga"); std::strcpy(src[1].text, "b.tga"); std::strcpy(src[2].text, "c.tga");
    CNames names = { src, src + 3 };
    CBankFileEntryUpdateData d; d.Filenames.count = 1; std::strcpy(d.Filenames.data[0].text, "old");
    d.SetFilenames(names);
    if (d.Filenames.count != 3 || std::strcmp(d.Filenames.data[0].text, "a.tga") || std::strcmp(d.Filenames.data[2].text, "c.tga")) {
        std::printf("009cc840_TEST FAIL\n"); return 1;
    }
    std::printf("009cc840_TEST PASS\n");
    return 0;
}