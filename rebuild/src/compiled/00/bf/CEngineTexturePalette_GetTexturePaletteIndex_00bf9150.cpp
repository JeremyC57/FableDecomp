// CEngineTexturePalette::GetTexturePaletteIndex(unsigned long index) -- map a graphic-bank entry index to its palette
// slot, appending a new {index, symbol} entry when it is not in the palette yet (retail builds use the index as is)
// Leaf: GetTexturePaletteIndex
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF9150, length 205 bytes, `ret 4`.
// Callees: CCharString::CCharString() @ 0x0099E4B0, CBankFile::GetEntrySymbol(unsigned long) (CCharString by hidden
// pointer) @ 0x009CC1D0, CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0, the CCharString copy ctor
// @ 0x0099EC30, the STLport vector<CPaletteEntry>::_M_insert_overflow @ 0x00BF94D0. Graphics layer (landscape
// texture palettes).
// Layout (Ego_r.pdb, retail: the 16-byte CArray is a 12-byte STLport vector): Bank +0 (CGraphicDataBank*), Palette
// +4 (8-byte CPaletteEntry {Index, CCharString Name}), DefaultIndex +0x10, RetailVersion +0x14.
// Shape: an unsigned index loop re-reading size() each pass; the new entry is a default-constructed local whose name
// is assigned from the symbol temporary (built in the dead `index` slot) and whose index is assigned after;
// push_back is the inline STLport form (placement-new memberwise copy, the overflow helper with the tag in the dead
// slot); the result is `size() - 1`.

class CCharString {
public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CBankFile {
public:
    CCharString GetEntrySymbol(unsigned long index) const;  // 0x009CC1D0
};
class CGraphicDataBank : public CBankFile {};

struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return _M_start[n]; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fillLength, bool atEnd);   // 0x00BF94D0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

class CEngineTexturePalette {
public:
    struct CPaletteEntry {
        unsigned long Index;
        CCharString Name;
    };
    CGraphicDataBank* Bank;                         // +0x00
    CRetailVector<CPaletteEntry> Palette;           // +0x04
    unsigned long DefaultIndex;                     // +0x10
    bool RetailVersion;                             // +0x14
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
    CPaletteEntry entry;
    entry.Name = Bank->GetEntrySymbol(index);
    entry.Index = index;
    Palette.push_back(entry);
    return Palette.size() - 1;
}