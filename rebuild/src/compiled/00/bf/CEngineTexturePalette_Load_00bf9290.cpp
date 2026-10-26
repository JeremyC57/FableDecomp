// CEngineTexturePalette::Load(CDataInputStream& stream) -- read the palette: the retail flag (a retail palette only clears the entry list), else the entry count, the entry names (entry 0 keeps index 0) and for every later entry the graphic-bank index of its name, or the default index when the bank does not know it
// Leaf: Load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF9290, length 362 bytes, `ret 4`.
// PDB: ?Load@CEngineTexturePalette@@QAEXAAVCDataInputStream@@@Z.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inline Read's slow path), vector<CPaletteEntry>::resize
//   (unsigned) @ 0x00BF97A0 (manifest label propagated), the stream's virtual slot 6 (+0x18, the string read returning
//   CCharString by value), CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0, the bank's entry lookup by name
//   @ 0x009CCDC0 (const CCharString&, as the landed FindEntry callers).
// Layout: as the landed CEngineTexturePalette::GetTexturePaletteIndex 0x00BF9150: Bank +0, Palette (STLport vector of
//   8-byte {Index, CCharString Name}) +4, DefaultIndex +0x10, RetailVersion +0x14.
// Shape: operator[] as `*(begin_() + n)` through an inline accessor (retry-10 STLport rule). The flag byte and the count are inline by-value stream reads homed in the dead `stream` slot; the count is
//   re-read from that slot for each loop test; the name temporary is a frame word.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CDataInputStream {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual CCharString ReadString();               // slot 6 (+0x18)
    unsigned long m_position;                       // +4
    long StreamSize;
    const unsigned char* m_pData;                   // +0xC
    long SourceChunkStreamPos;
    long m_remaining;                               // +0x14
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline char ReadChar() { char v; Read(&v, sizeof(v)); return v; }
    __forceinline long ReadLong() { long v; Read(&v, sizeof(v)); return v; }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};

class CBankFile {
public:
public:
    unsigned long FindIndexBySymbol(const CCharString&) const;   // 0x009CCDC0
};
class CGraphicDataBank : public CBankFile {};

template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin_() const { return _M_start; }
    T& operator[](unsigned long n) { return *(begin_() + n); }
    void resize(unsigned int n);                    // 0x00BF97A0
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
    void Load(CDataInputStream& stream);
};

void CEngineTexturePalette::Load(CDataInputStream& stream)
{
    RetailVersion = stream.ReadChar() != 0;
    if (RetailVersion) {
        Palette.resize(0);
        return;
    }
    long count = stream.ReadLong();
    Palette.resize(count);
    Palette[0].Name = stream.ReadString();
    Palette[0].Index = 0;
    for (long i = 1; i < count; ++i) {
        Palette[i].Name = stream.ReadString();
        unsigned long index = Bank->FindIndexBySymbol(Palette[i].Name);
        if (index > 0)
            Palette[i].Index = index;
        else
            Palette[i].Index = DefaultIndex;
    }
}