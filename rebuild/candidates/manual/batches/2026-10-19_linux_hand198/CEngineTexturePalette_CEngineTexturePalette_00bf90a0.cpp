// CEngineTexturePalette::CEngineTexturePalette(CGraphicDataBank*, bool, unsigned long) -- empty palette over the bank; a non-retail palette starts with one default entry (index 0, empty name)
// Leaf: ??0CEngineTexturePalette
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF90A0, length 176 bytes (`ret 0Ch`). The manifest label
// "CParticleMorphs::Initialize" is propagated: the only caller is CEngineLandscapeMap's constructor (0x00BDD2F0), which
// builds its TexturePalette member (+0x40) from the engine's graphic bank, false and a creation value.
// Callees: CCharString::CCharString() @ 0x0099E4B0, the CCharString copy ctor @ 0x0099EC30, ~CCharString @ 0x0099EAE0,
//          the STLport vector<CPaletteEntry>::_M_insert_overflow @ 0x00BF94D0.
// Layout: as the landed CEngineTexturePalette::GetTexturePaletteIndex (0x00BF9150): Bank +0, Palette +4 (12-byte
// STLport vector of 8-byte {Index, CCharString Name}), DefaultIndex +0x10, RetailVersion +0x14.
// Shape: the vector's inline constructor nulls its three pointers first, then the scalars are assigned in the body
// (flag, default index, bank); the default entry is a local whose index is zeroed after its name is constructed,
// pushed back through the inline STLport push_back (the copy's index constant-folds to 0).
class CCharString {
public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};

class CBankFile { };
class CGraphicDataBank : public CBankFile {};

struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
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
    CEngineTexturePalette(CGraphicDataBank* bank, bool retailVersion, unsigned long defaultIndex);
};

CEngineTexturePalette::CEngineTexturePalette(CGraphicDataBank* bank, bool retailVersion, unsigned long defaultIndex)
{
    RetailVersion = retailVersion;
    DefaultIndex = defaultIndex;
    Bank = bank;
    if (!retailVersion) {
        CPaletteEntry entry;
        entry.Index = 0;
        Palette.push_back(entry);
    }
}
