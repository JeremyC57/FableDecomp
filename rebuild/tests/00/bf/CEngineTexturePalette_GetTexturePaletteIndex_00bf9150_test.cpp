// Behaviour test for 0x00BF9150 CEngineTexturePalette::GetTexturePaletteIndex: retail passthrough, lookup of known
// indices, and appends with the bank's entry symbol (through the overflow path too).
#include <cstdio>
#include <cstring>
#include <cstdlib>
class CCharString {
public:
    char text[24];
    CCharString() { text[0] = 0; }
    CCharString(const char* t) { std::strcpy(text, t); }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); }
    ~CCharString() {}
    CCharString& operator=(const CCharString& o) { std::strcpy(text, o.text); return *this; }
};
class CGraphicDataBank {
public:
    CCharString GetEntrySymbol(unsigned long index) const { char b[24]; std::sprintf(b, "TEX_%lu", index); return CCharString(b); }
};
struct __false_type { __false_type() {} };
inline void* operator new(size_t, void* where, int) { return where; }
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return _M_start[n]; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long, bool)
    {
        unsigned long n = size(), cap = n ? n * 2 : 1;
        T* s = (T*)std::malloc(cap * sizeof(T));
        for (unsigned long i = 0; i < n; ++i) { new (s + i, 0) T(_M_start[i]); _M_start[i].~T(); }
        new (s + n, 0) T(x);
        std::free(_M_start); _M_start = s; _M_finish = s + n + 1; _M_end_of_storage = s + cap;
        (void)position;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish, 0) T(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
class CEngineTexturePalette {
public:
    struct CPaletteEntry { unsigned long Index; CCharString Name; };
    CGraphicDataBank* Bank; CRetailVector<CPaletteEntry> Palette; unsigned long DefaultIndex; bool RetailVersion;
    long GetTexturePaletteIndex(unsigned long index);
};
long CEngineTexturePalette::GetTexturePaletteIndex(unsigned long index)
{
    if (RetailVersion)
        return index;
    for (unsigned long i = 0; i < Palette.size(); ++i) {
        if (Palette[i].Index == index)
            return i;
    }
    CEngineTexturePalette::CPaletteEntry entry;
    entry.Name = Bank->GetEntrySymbol(index);
    entry.Index = index;
    Palette.push_back(entry);
    return Palette.size() - 1;
}
int main()
{
    CGraphicDataBank bank;
    CEngineTexturePalette p; p.Bank = &bank; p.Palette._M_start = p.Palette._M_finish = p.Palette._M_end_of_storage = 0; p.RetailVersion = true;
    int fail = 0;
    if (p.GetTexturePaletteIndex(77) != 77 || p.Palette.size()) fail |= 1;
    p.RetailVersion = false;
    if (p.GetTexturePaletteIndex(40) != 0 || p.GetTexturePaletteIndex(12) != 1 || p.GetTexturePaletteIndex(99) != 2) fail |= 2;
    if (p.GetTexturePaletteIndex(12) != 1 || p.GetTexturePaletteIndex(40) != 0 || p.Palette.size() != 3) fail |= 4;
    if (std::strcmp(p.Palette[2].Name.text, "TEX_99") || p.Palette[1].Index != 12) fail |= 8;
    if (fail) { std::printf("00bf9150_TEST FAIL\n"); return 1; }
    std::printf("00bf9150_TEST PASS\n");
    return 0;
}