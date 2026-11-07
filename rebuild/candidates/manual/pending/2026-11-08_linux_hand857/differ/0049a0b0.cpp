// CProgressDisplay::InitialiseTextures(bool single_background, bool reduced) -- load the loading-screen textures (splash, shuffled backgrounds, guild seals, progress bar parts) and the shuffled loading-tip text tags
// Grade: DIFFER(2753v2920, md=798) -- first draft: the inline CWideString empty test (wcscmp / MemCmp_Unsigned16 0x00403C60) is a call here, the 4-texture block is not tail-merged as retail does, CDiskFileWin32's inline virtual dtor is a call
// Next idea: model the CWideString operator==(const wchar16*) inline body, the CDiskFileWin32 inline dtor (vtable store, Close, ~CBaseClassNonCopyable), and the shared ~CWideString tail retail jumps to (0x0049A8BC)
// Leaf: ?InitialiseTextures@CProgressDisplay@@UAEX_N0@Z
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    CCharString();                                                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);                                   // 0x0099EBF0
    CCharString(const CCharString& other);                                             // 0x0099EC30
    ~CCharString();                                                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);                                  // 0x0099EFB0
    CCharString GetSubString(unsigned long start, unsigned long length) const;         // 0x0099EC70
    void LoadFromFile(class CAFile& file, class CASuspendableProcess* process, long size);   // 0x0099F2E0
    unsigned long GetLength() const
    {
        return PStringData ? ((unsigned long*)PStringData)[1] : 0;
    }
    const char* GetCharPointer() const
    {
        return PStringData ? *(const char**)PStringData : "";
    }
    bool EndsWith(const CCharString& end) const
    {
        long offset = (long)GetLength() - (long)end.GetLength();
        if (offset < 0)
            return false;
        const unsigned char* a = (const unsigned char*)GetCharPointer() + offset;
        const unsigned char* b = (const unsigned char*)end.GetCharPointer();
        for (;;) {
            unsigned char c = *a;
            if (c != *b) return false;
            if (c == 0) return true;
            c = a[1];
            if (c != b[1]) return false;
            a += 2; b += 2;
            if (c == 0) return true;
        }
    }
};
CCharString __fastcall operator+(const CCharString& left, const char* right);         // 0x0099F600

typedef unsigned short wchar16;
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar16* text);                                                  // 0x0099B6B0
    ~CWideString();                                                                    // 0x0099B510
    const CWideString& operator=(const CWideString& other);                            // 0x0099B7D0
    const CWideString& operator+=(const CWideString& other);                           // 0x0099B8D0
    const CWideString& operator+=(const wchar16* text);                                // 0x0099B940
    bool IsEmpty() const;
};
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);      // 0x0099BF30
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);  // 0x0099BE70
CWideString __fastcall GFIntToWideString(long value);                                 // 0x00405F00

namespace NResourceDirectoryNames { CWideString __fastcall GetMiscDir(); }            // 0x0041A0A0
class CAFile { public: static CWideString __fastcall ConvertPathToFullPath(const CWideString& path); };   // 0x00999110

namespace NGameText {
class CDataBank { public: const CWideString GetTextBySymbol(const CCharString& symbol) const; };   // 0x009C95E0
}

double __fastcall GFGetTime();                                                        // 0x009E1BC0

class C2DExtentsI { public: long X, Y; };

class CPixelFormat {
public:
    unsigned long Format;
    long          Index;
    CPixelFormat() : Index(-1) { }
};

class CSurface {
public:
    virtual void v00();
    void*         PD3DSurface;
    unsigned long AllocationSource;
    void*         PAllocatedMemory;
    ~CSurface() { Uninitialise(); }
    void Uninitialise();                                                               // 0x009F2E20
    C2DExtentsI GetDimensions() const;                                                 // 0x009F2F90
    bool LoadFromBitmapFileToPreallocatedSurface(const CWideString& file, long filter);   // 0x009F2DD0
};

class CTexture {
public:
    void*         PD3DTexture;
    unsigned long Flags;
    CTexture(const C2DExtentsI& size, long levels, const CPixelFormat& format, unsigned long usage, long pool)
    {
        PD3DTexture = 0;
        Flags &= 0x0fffffff;
        Initialise(size, levels, format, usage, pool, false, false);
    }
    ~CTexture() { Uninitialise(); }
    bool Initialise(const C2DExtentsI& size, long levels, const CPixelFormat& format, unsigned long usage, long pool,
                    bool a, bool b);                                                   // 0x009FA280
    void Uninitialise();                                                               // 0x009F9F70
    CSurface GetSurfaceLevel(long level) const;                                        // 0x009F9E00
    static C2DExtentsI __fastcall GetNextValidDimensions(const C2DExtentsI& size, const CPixelFormat& format);   // 0x009FA620
};

class CDisplayManager {
public:
    unsigned char _pad_0x00[0x2c];
    CSurface      BackBufferSurface;                                                   // +0x2c
    bool GetBestCompressedOpaqueTextureFormat(unsigned long bits, CPixelFormat& format);   // 0x009BE830
    bool GetBestCompressedAlphaTextureFormat(unsigned long bits, CPixelFormat& format);    // 0x009BE870
};
class CSystemManager { public: unsigned char _pad_0x00[0x60]; CDisplayManager* DisplayManager; };
CSystemManager* __fastcall GFGetSystemManager();                                     // 0x009A4EC0

class CDiskFileWin32 {
public:
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Close();                                                              // 0x0099A920
    CDiskFileWin32(const CWideString& name, long access, unsigned long flags);         // 0x0099AD80
    unsigned char _pad_0x04[0x14 - 0x04];
    bool          Opened;                                                              // +0x14
};

class CStringParser {
public:
    CStringParser(const CCharString& text, void (__fastcall* error)(const CCharString&), const CCharString& filename,
                  long base_line);                                                     // 0x00404720
    ~CStringParser();                                                                  // 0x004047A0
    unsigned char _body[0x34];
};

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    _Tp& operator[](unsigned int n) { return _M_start[n]; }
    void push_back(const _Tp& x);
    void resize(unsigned int n);
    _Tp* erase(_Tp* first, _Tp* last);
    void clear() { erase(_M_start, _M_finish); }
};
template <class _RandomAccessIter, class _RandomNumberGenerator>
void random_shuffle(_RandomAccessIter first, _RandomAccessIter last, _RandomNumberGenerator& rand);
}

class CPersistContext {
public:
    CPersistContext(CStringParser& parser, void (__fastcall* error)(const CCharString&), bool loading, const void* defs,
                    const void* symbols, CCharString* name);                           // 0x009BADD0
    template <class T> void TransferVector(const char* name, std::vector<T>& v);       // 0x0049B8D0
    unsigned char _body[0x40];
};

class CRandomGenerator {
public:
    long* Seed;
    CRandomGenerator(long& seed) : Seed(&seed) { }
    long operator()(long n);
};

extern bool g_ProgressBlackScreenA;                                                   // 0x013B89C1
extern CWideString g_ProgressTipText;                                                 // 0x013B89D8
extern CWideString g_ProgressTipTitle;                                                // 0x013B89DC

class CProgressDisplay {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void SetToDisplayText(bool display);                                       // slot 5 (+0x14)
    std::vector<CTexture>    ProgressTextures;                                         // +0x04
    std::vector<CTexture>    GuildSealTextures;                                        // +0x10
    std::vector<CTexture>    LoadingScreenTextures;                                    // +0x1c
    long                     LoadingScreenIndex;                                       // +0x28
    std::vector<CCharString> TextTags;                                                 // +0x2c
    std::vector<CCharString> TextTagsPC;                                               // +0x38
    long                     TextTagIndex;                                             // +0x44
    CWideString              Text;                                                     // +0x48
    CCharString              TextTag;                                                  // +0x4c
    CCharString              LastTextDisplayed;                                        // +0x50
    unsigned char            _pad_0x54[0x78 - 0x54];
    bool                     DebugDisplay;                                             // +0x78
    bool                     Initialised;                                              // +0x79
    bool                     FullScreen;                                               // +0x7a
    bool                     BlackScreen;                                              // +0x7b
    bool                     DisplayText;                                              // +0x7c
    bool                     DisplayResetProgressTipText;                              // +0x7d
    void CalculateNextTextTag();                                                       // 0x00497B60
    const NGameText::CDataBank* GetPTextBank() const;                                  // 0x00497B30
    virtual void InitialiseTextures(bool single_background, bool reduced);
};

void CProgressDisplay::InitialiseTextures(bool single_background, bool reduced)
{
    if (g_ProgressBlackScreenA)
        return;
    CDisplayManager* display = GFGetSystemManager()->DisplayManager;
    if (display == 0)
        return;
    if (!ProgressTextures.empty())
        return;
    CPixelFormat format;
    display->GetBestCompressedOpaqueTextureFormat(16, format);
    if (DebugDisplay) {
        ProgressTextures.resize(1);
        C2DExtentsI screen;
        screen.Y = display->BackBufferSurface.GetDimensions().Y;
        screen.X = display->BackBufferSurface.GetDimensions().X;
        C2DExtentsI size = CTexture::GetNextValidDimensions(screen, format);
        ProgressTextures[0].Initialise(size, 1, format, 0, 1, false, false);
        ProgressTextures[0].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
            CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + (const wchar16*)L"splash.tga"), 2);
        return;
    }
    LoadingScreenTextures.clear();
    if (reduced && !g_ProgressBlackScreenA) {
        ProgressTextures.clear();
        GuildSealTextures.clear();
    } else {
        ProgressTextures.resize(7);
        GuildSealTextures.resize(16);
    }
    C2DExtentsI size;
    size.X = 0x400;
    size.Y = 0x200;
    if (FullScreen) {
        long seed = (long)GFGetTime();
        std::vector<long> backgrounds;
        for (long i = 2; i <= 9; ++i)
            backgrounds.push_back(i);
        CRandomGenerator random(seed);
        std::random_shuffle(backgrounds.begin(), backgrounds.end(), random);
        long count = reduced ? 1 : 2;
        CTexture texture(size, 1, format, 0, 1);
        for (long i = 0; i < count; ++i) {
            CWideString name = NResourceDirectoryNames::GetMiscDir() + (const wchar16*)L"LoadProgress\\Fable_BG_PC_0";
            name += GFIntToWideString(backgrounds[i]);
            name += (const wchar16*)L".dds";
            texture.GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(name, 2);
            LoadingScreenTextures.push_back(texture);
        }
    }
    display->GetBestCompressedAlphaTextureFormat(16, format);
    if (FullScreen && !reduced) {
        size.X = 0x40;
        size.Y = 0x40;
        long seals = single_background ? 1 : 16;
        for (long i = 1; i <= seals; ++i) {
            GuildSealTextures[i - 1].Initialise(size, 1, format, 0, 1, false, false);
            CWideString name((const wchar16*)L"LoadProgress\\guild_seal_");
            if (i < 10)
                name += (const wchar16*)L"0";
            name += GFIntToWideString(i);
            name += (const wchar16*)L".dds";
            GuildSealTextures[i - 1].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
                CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + name), 2);
        }
    }
    if ((FullScreen && !reduced) || !g_ProgressBlackScreenA) {
        size.X = 0x40; size.Y = 0x20;
        ProgressTextures[1].Initialise(size, 1, format, 0, 1, false, false);
        ProgressTextures[1].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
            CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + (const wchar16*)L"LoadProgress\\slot_left_bar_off.dds"), 2);
        size.X = 8; size.Y = 0x20;
        ProgressTextures[2].Initialise(size, 1, format, 0, 1, false, false);
        ProgressTextures[2].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
            CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + (const wchar16*)L"LoadProgress\\slot_mid_bar_off.dds"), 2);
        size.X = 0x40; size.Y = 0x20;
        ProgressTextures[3].Initialise(size, 1, format, 0, 1, false, false);
        ProgressTextures[3].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
            CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + (const wchar16*)L"LoadProgress\\slot_right_bar_off.dds"), 2);
        size.X = 4; size.Y = 0x10;
        ProgressTextures[4].Initialise(size, 1, format, 0, 1, false, false);
        ProgressTextures[4].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
            CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + (const wchar16*)L"LoadProgress\\Ybar_seg.dds"), 2);
        if (!g_ProgressBlackScreenA) {
            size.X = 0x100; size.Y = 0x80;
            ProgressTextures[5].Initialise(size, 1, format, 0, 1, false, false);
            ProgressTextures[5].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
                CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + (const wchar16*)L"LoadProgress\\fable_BG_bar_border.dds"), 2);
            size.X = 0x80; size.Y = 0x80;
            ProgressTextures[6].Initialise(size, 1, format, 0, 1, false, false);
            ProgressTextures[6].GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(
                CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + (const wchar16*)L"LoadProgress\\rc_seal_border.dds"), 2);
        }
    }
    TextTags.clear();
    TextTagsPC.clear();
    if (FullScreen && !g_ProgressBlackScreenA) {
        SetToDisplayText(true);
        CDiskFileWin32 file(CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetMiscDir() + (const wchar16*)L"LoadProgress\\TextTags.txt"), 1, 2);
        if (file.Opened) {
            CCharString contents;
            contents.LoadFromFile(*(CAFile*)&file, 0, -1);
            CStringParser parser(contents, 0, CCharString(""), 0);
            CPersistContext context(parser, 0, true, 0, 0, 0);
            context.TransferVector("TextTags", TextTags);
            context.TransferVector("TextTagsPC", TextTagsPC);
            file.Close();
        }
        long seed = (long)GFGetTime();
        CRandomGenerator random(seed);
        std::random_shuffle(TextTags.begin(), TextTags.end(), random);
        std::random_shuffle(TextTagsPC.begin(), TextTagsPC.end(), random);
        DisplayResetProgressTipText = true;
        if (g_ProgressTipText.IsEmpty()) {
            CalculateNextTextTag();
            const NGameText::CDataBank* bank = GetPTextBank();
            if (bank != 0) {
                CCharString tag(TextTag);
                if (tag.EndsWith(CCharString("0")))
                    tag = tag.GetSubString(0, tag.GetLength() - 3);
                g_ProgressTipText = bank->GetTextBySymbol(TextTag);
                g_ProgressTipTitle = bank->GetTextBySymbol(tag + "_TITLE");
            }
            DisplayResetProgressTipText = false;
        }
    }
}
