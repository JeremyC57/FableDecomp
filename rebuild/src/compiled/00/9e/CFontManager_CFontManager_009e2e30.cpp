// CFontManager::CFontManager(const CCharString& fontBankName, const CCharString& streamingFontBankName) -- create the font and streaming-font data banks and their symbol maps, open each named bank and build its symbol map, then allocate the eight font vertex buffers
// Leaf: ??0CFontManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E2E30, 449 bytes, `ret 8`, vtable 0x0129BA38.
// Callees: CBaseObject::CBaseObject 0x0099A2F0 (the catalog's NHeroInformationScreens::CBase label is a shared body),
//   operator new 0x00BFEA1A, CFontDataBank() 0x00A63630, CSymbolMap() 0x009E5200, CStreamingFontDataBank() 0x00A5F290,
//   CCountedPointer<T>::Reset 0x009E3090 / 0x009B1F50 / 0x009E3100, CBankFile::CreateSymbolMap 0x009CC530,
//   CStreamingFontDataBank::InitStreamingEntry 0x00A5EF50, CArray<CVertexBufferWin32*>::resize 0x009E3580,
//   CVertexBufferWin32::Initialise 0x00A63150; the data banks' virtual slot 1 (open by name, flags 0x48).
// Layout: retail 0x3c bytes -- the PDB's members in order with the 12-byte CArray at +0x24, CurrentVB +0x30,
//   NumberAsyncGlyphsPending +0x34 and LoadFontGlyphsAsynchronously +0x38; CVertexBufferWin32 is 0x34 bytes with vtable
//   0x0129BA20 (its inline constructor calls Initialise).
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
};
struct CCharStringData { CBasicStringChar Data; long NoRefs; };
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    bool IsEmpty() const { return PStringData == 0 || PStringData->Data.StringLength == 0; }
};
namespace N_0099a2f0 { struct CBaseObject {
    CBaseObject();                                       // 0x0099A2F0
    virtual ~CBaseObject();
}; } using N_0099a2f0::CBaseObject;
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    void Reset(T* p);                                    // 0x009E3090 / 0x009B1F50 / 0x009E3100
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
};
struct CSymbolMap {
    unsigned char Data[0x24];
    CSymbolMap();                                        // 0x009E5200
};
struct CBankFile {
    virtual void v0();
    virtual void Open(const CCharString& name, long flags);   // slot 1
    void CreateSymbolMap(CSymbolMap& map, bool b) const;      // 0x009CC530
};
struct CFontDataBank : CBankFile {
    unsigned char Data[0x12c];
    CFontDataBank();                                     // 0x00A63630
};
struct CStreamingFontDataBank : CBankFile {
    unsigned char Data[0x18c];
    CStreamingFontDataBank();                            // 0x00A5F290
    void InitStreamingEntry(CSymbolMap& map);            // 0x00A5EF50
};
struct IDirect3DVertexBuffer9;
struct CVertexCompressionParams {                        // 32 bytes
    float Scale[4];
    float Offset[4];
    CVertexCompressionParams()
    {
        Scale[0] = 1.0f; Scale[1] = 1.0f; Scale[2] = 1.0f; Scale[3] = 1.0f;
        Offset[0] = 0.0f; Offset[1] = 0.0f; Offset[2] = 0.0f; Offset[3] = 0.0f;
    }
};
struct CVertexBufferBase : CBaseObject {                 // PDB, 0x34 bytes
    IDirect3DVertexBuffer9*  PVertexBuffer;              // +0x04
    unsigned long            Length;                     // +0x08
    long                     Type;                       // +0x0c EVertexType
    unsigned char            Stride;                     // +0x10
    CVertexCompressionParams CompressionParams;          // +0x14
    CVertexBufferBase() : Length(0), Type(0) { }
    virtual ~CVertexBufferBase();
};
struct CVertexBufferWin32 : CVertexBufferBase {          // vtable 0x0129BA20
    CVertexBufferWin32(long vertexType, long count, long stride, unsigned long usage, long pool)
    {
        PVertexBuffer = 0;
        Stride = 0;
        Initialise(vertexType, count, stride, usage, pool);
    }
    virtual ~CVertexBufferWin32();
    bool Initialise(long vertexType, long count, long stride, unsigned long usage, long pool);   // 0x00A63150
};
template <class T> struct CArray {
    T*   Data;
    long Size;
    long MaxSize;
    CArray() : Data(0), Size(0), MaxSize(0) { }
    void resize(long size, const T& value);             // 0x009E3580
    T& operator[](long i) { return Data[i]; }
};
struct CFontManager : CBaseObject {                      // vtable 0x0129BA38
    CCountedPointer<CFontDataBank>          PFontDataBank;            // +0x04
    CCountedPointer<CStreamingFontDataBank> PStreamingFontDataBank;   // +0x0c
    CCountedPointer<CSymbolMap>             PSymbols;                 // +0x14
    CCountedPointer<CSymbolMap>             PStreamingSymbols;        // +0x1c
    CArray<CVertexBufferWin32*>             VertexBuffers;            // +0x24
    long                                    CurrentVB;                // +0x30
    unsigned long                           NumberAsyncGlyphsPending; // +0x34
    bool                                    LoadFontGlyphsAsynchronously;   // +0x38
    CFontManager(const CCharString& fontBankName, const CCharString& streamingFontBankName);
    virtual ~CFontManager();
};

CFontManager::CFontManager(const CCharString& fontBankName, const CCharString& streamingFontBankName)
    : NumberAsyncGlyphsPending(0), LoadFontGlyphsAsynchronously(true)
{
    PFontDataBank.Reset(new CFontDataBank());
    PSymbols.Reset(new CSymbolMap());
    if (!fontBankName.IsEmpty()) {
        PFontDataBank->Open(fontBankName, 0x48);
        PFontDataBank->CreateSymbolMap(*PSymbols, false);
    }
    PStreamingFontDataBank.Reset(new CStreamingFontDataBank());
    PStreamingSymbols.Reset(new CSymbolMap());
    if (!streamingFontBankName.IsEmpty()) {
        PStreamingFontDataBank->Open(streamingFontBankName, 0x48);
        PStreamingFontDataBank->CreateSymbolMap(*PStreamingSymbols, false);
        PStreamingFontDataBank->InitStreamingEntry(*PStreamingSymbols);
    }
    CurrentVB = 0;
    VertexBuffers.resize(8, 0);
    for (long i = 0; i < 8; ++i)
        VertexBuffers[i] = new CVertexBufferWin32(0x144, 0x12c, 0x1c, 8, 1);
}