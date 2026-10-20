// Behaviour test for CProgressDisplay::~CProgressDisplay 0x00499FB0 (model copy; counts element destructions)
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_strings = 0, g_wide = 0, g_textures = 0, g_frees = 0, g_stops = 0;
static int g_order[16]; static int g_n = 0;

struct CCharString { int tag; ~CCharString() { ++g_strings; g_order[g_n++] = tag; } };
struct CWideString { ~CWideString() { ++g_wide; g_order[g_n++] = 100; } };
struct CTexture { void* a; void* b; ~CTexture() { ++g_textures; } };

template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}

template <class T> struct CVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    void Fill(int n) { _M_start = (T*)std::malloc(sizeof(T) * n); for (int i = 0; i < n; ++i) new (&_M_start[i]) T(); _M_finish = _M_end_of_storage = _M_start + n; }
    ~CVector() { _Destroy(_M_start, _M_finish); if (_M_start) { std::free(_M_start); ++g_frees; } }
};

struct CAProgressDisplay {
    virtual ~CAProgressDisplay() {}
    virtual void StopProgress(bool a, bool b) = 0;
};

struct CProgressDisplay : CAProgressDisplay {
    CVector<CTexture>    ProgressTextures;
    CVector<CTexture>    GuildSealTextures;
    CVector<CTexture>    LoadingScreenTextures;
    long                 LoadingScreenIndex;
    CVector<CCharString> TextTags;
    CVector<CCharString> TextTagsPC;
    long                 TextTagIndex;
    CWideString          Text;
    CCharString          TextTag;
    CCharString          LastTextDisplayed;
    virtual ~CProgressDisplay();
    virtual void StopProgress(bool a, bool b) { if (!a && !b) ++g_stops; }
};

CProgressDisplay::~CProgressDisplay()
{
    StopProgress(false, false);
}

int main()
{
    {
        CProgressDisplay* d = new CProgressDisplay;
        d->TextTag.tag = 1; d->LastTextDisplayed.tag = 2;
        d->ProgressTextures.Fill(3); d->LoadingScreenTextures.Fill(2); d->TextTags.Fill(4);
        for (int i = 0; i < 4; ++i) d->TextTags._M_start[i].tag = 10;
        delete d;
    }
    if (g_stops != 1 || g_textures != 5 || g_strings != 6 || g_wide != 1 || g_frees != 3 ||
        g_order[0] != 2 || g_order[1] != 1 || g_order[2] != 100) {
        std::printf("00499fb0_TEST FAIL\n"); return 1;
    }
    std::printf("00499fb0_TEST PASS\n");
    return 0;
}
