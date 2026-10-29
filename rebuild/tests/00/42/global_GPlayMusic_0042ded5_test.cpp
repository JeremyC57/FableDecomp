// Behaviour test for GPlayMusic (0x0042DED5): with a sound manager present the stream played is the music directory +
// the misc-things def's FrontEndMusic with ".wma" replaced by ".ogg", gain 1, the given fade, looping (-1); the def
// pointer temporary releases its reference; without a sound manager nothing happens.
#include <cstdio>
#include <stddef.h>
static int wlen(const wchar_t* s) { int n = 0; while (s[n]) ++n; return n; }
static bool weq(const wchar_t* a, const wchar_t* b) { int i = 0; for (; a[i] && a[i] == b[i]; ++i) {} return a[i] == b[i]; }
class CWideString {
public:
    CWideString(const wchar_t* text) { int i = 0; for (; text[i]; ++i) buf[i] = text[i]; buf[i] = 0; }
    CWideString(const CWideString& o) { int i = 0; for (; o.buf[i]; ++i) buf[i] = o.buf[i]; buf[i] = 0; }
    ~CWideString() {}
    void Replace(const CWideString& from, const CWideString& to)
    {
        wchar_t out[128]; int o = 0; int fl = wlen(from.buf);
        for (int i = 0; buf[i];) {
            int k = 0; while (k < fl && buf[i + k] == from.buf[k]) ++k;
            if (fl && k == fl) { for (int t = 0; to.buf[t]; ++t) out[o++] = to.buf[t]; i += fl; }
            else out[o++] = buf[i++];
        }
        out[o] = 0; for (int j = 0; j <= o; ++j) buf[j] = out[j];
    }
    wchar_t buf[128];
};
CWideString operator+(const CWideString& l, const CWideString& r)
{
    CWideString s(l); int n = wlen(s.buf); int i = 0; for (; r.buf[i]; ++i) s.buf[n + i] = r.buf[i]; s.buf[n + i] = 0; return s;
}
namespace NResourceDirectoryNames { CWideString GetDVDMusicDir() { return CWideString(L"Data\\Sound\\"); } }
class CIVCountedPointeeBase {
public:
    CIVCountedPointeeBase() : IVRefCount(1), deleted(0) {}
    virtual ~CIVCountedPointeeBase() {}
    virtual void Delete() const { ++const_cast<CIVCountedPointeeBase*>(this)->deleted; }
    void AddReference() { ++IVRefCount; }
    void RemoveReference() { if (--IVRefCount == 0) Delete(); }
    unsigned long IVRefCount; int deleted;
};
template <class T> class CDefPointer {
public:
    CDefPointer(T* p) : PObject(p) { if (PObject) PObject->AddReference(); }
    CDefPointer(const CDefPointer& o) : PObject(o.PObject) { if (PObject) PObject->AddReference(); }
    ~CDefPointer() { if (PObject) PObject->RemoveReference(); }
    T* operator->() const { return PObject; }
    T* PObject;
};
namespace NUISystem {
class CUIMiscThingsDef : public CIVCountedPointeeBase {
public:
    CUIMiscThingsDef() : FrontEndMusic(L"Music\\Title.wma") {}
    CWideString FrontEndMusic;
};
CUIMiscThingsDef g_def;
class CManager {
public:
    CDefPointer<CUIMiscThingsDef> GetMiscThingsDef() { return CDefPointer<CUIMiscThingsDef>(&g_def); }
    static CManager* __fastcall GetInstance() { static CManager m; return &m; }
};
}
class CSoundManagerLH {
public:
    CSoundManagerLH() : calls(0), name(L"") {}
    virtual void PlayStream(const CWideString& fileName, long channel, long flags, float gain, float fade, long loops)
    { ++calls; name = fileName; c = channel; f = flags; g = gain; fd = fade; l = loops; }
    int calls; CWideString name; long c, f, l; float g, fd;
};
CSoundManagerLH* g_pSoundSystem = 0;
void GPlayMusic(float fade)
{
    if (g_pSoundSystem) {
        CWideString fileName = NResourceDirectoryNames::GetDVDMusicDir()
                             + NUISystem::CManager::GetInstance()->GetMiscThingsDef()->FrontEndMusic;
        fileName.Replace(L".wma", L".ogg");
        g_pSoundSystem->PlayStream(fileName, 0, 0, 1.0f, fade, -1);
    }
}
int main()
{
    int bad = 0;
    GPlayMusic(2.0f);                                    // no sound manager: nothing
    if (NUISystem::g_def.IVRefCount != 1) bad = 1;
    CSoundManagerLH sm; g_pSoundSystem = &sm;
    GPlayMusic(0.5f);
    if (sm.calls != 1 || !weq(sm.name.buf, L"Data\\Sound\\Music\\Title.ogg")) bad = 1;
    if (sm.c != 0 || sm.f != 0 || sm.g != 1.0f || sm.fd != 0.5f || sm.l != -1) bad = 1;
    if (NUISystem::g_def.IVRefCount != 1 || NUISystem::g_def.deleted != 0) bad = 1;
    if (bad) { std::printf("0042ded5_TEST FAIL\n"); return 1; }
    std::printf("0042ded5_TEST PASS\n");
    return 0;
}