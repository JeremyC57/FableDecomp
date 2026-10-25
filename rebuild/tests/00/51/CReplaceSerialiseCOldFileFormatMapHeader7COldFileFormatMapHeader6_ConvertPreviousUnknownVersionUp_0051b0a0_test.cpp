// Behaviour test for CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::ConvertPreviousUnknownVersionUp
// (0x0051B0A0): a version-6 record is converted directly, an older one goes through the previous link first; the
// fields come forward, NoSoundThemeEntries is zeroed and the counted pointers balance -- through a copy of the model.
#include <cstdio>
#include <cstring>

static int g_infos = 0, g_upgrades = 0, g_deletes = 0;

struct Info { unsigned int refs; void* object; };
template <class T> struct Counted {
    T* object; Info* control;
    Counted() : object(0), control(0) {}
    explicit Counted(T* p) : object(p), control(0) { if (p) { control = new Info; control->refs = 1; control->object = p; ++g_infos; } }
    Counted(const Counted& x) : object(x.object), control(x.control) { if (control) ++control->refs; }
    Counted& operator=(const Counted& x)
    {
        Info* c = x.control; T* o = x.object;
        if (control != c) { Release(); object = o; control = c; if (control) ++control->refs; }
        return *this;
    }
    void Release() { if (control && --control->refs == 0) { delete (T*)control->object; delete control; --g_infos; ++g_deletes; } }
    ~Counted() { Release(); }
};

struct Header6 { unsigned char version; unsigned int uidLo, uidHi, sizeX, sizeY; bool extra; unsigned char palette[64]; unsigned int versionNumber; };
struct Header7 { unsigned char version; unsigned int uidLo, uidHi, sizeX, sizeY; bool extra; unsigned char palette[64]; unsigned int versionNumber, noSoundThemeEntries;
    void ConvertFromOld(const Header6* old)
    {
        uidLo = old->uidLo; uidHi = old->uidHi; sizeX = old->sizeX; sizeY = old->sizeY; extra = old->extra;
        std::memcpy(palette, old->palette, sizeof(palette)); versionNumber = old->versionNumber; noSoundThemeEntries = 0;
    }
};

static Counted<Header6> ConvertUpTo6(void* data)           // stands in for the previous link 0x0051A5A0
{
    ++g_upgrades;
    Header6* h = new Header6;
    std::memcpy(h, data, sizeof(Header6));
    h->version = 6; h->sizeX += 1000;                       // mark the upgraded copy
    return Counted<Header6>(h);
}

static Counted<Header7> ConvertUpTo7(void* data)
{
    Counted<Header7> result(new Header7);
    Counted<Header6> held;
    Header6* old = (Header6*)data;
    if (((Header6*)data)->version != 6) {
        held = ConvertUpTo6(data);
        old = held.object;
    }
    result.object->ConvertFromOld(old);
    return result;
}

int main()
{
    Header6 h6; std::memset(&h6, 0, sizeof(h6));
    h6.version = 6; h6.uidLo = 0x11; h6.uidHi = 0x22; h6.sizeX = 64; h6.sizeY = 32; h6.extra = true; h6.palette[63] = 9; h6.versionNumber = 5;
    {
        Counted<Header7> r = ConvertUpTo7(&h6);
        if (g_upgrades != 0 || r.object->sizeX != 64 || r.object->uidHi != 0x22 || !r.object->extra || r.object->palette[63] != 9) { std::printf("0051b0a0_TEST FAIL\n"); return 1; }
        if (r.object->versionNumber != 5 || r.object->noSoundThemeEntries != 0 || r.control->refs != 1 || g_infos != 1) { std::printf("0051b0a0_TEST FAIL\n"); return 1; }
    }
    if (g_infos != 0) { std::printf("0051b0a0_TEST FAIL\n"); return 1; }
    h6.version = 5;                                         // older record: upgraded through version 6 first
    {
        Counted<Header7> r = ConvertUpTo7(&h6);
        if (g_upgrades != 1 || r.object->sizeX != 1064 || r.control->refs != 1 || g_infos != 1 || g_deletes != 2) { std::printf("0051b0a0_TEST FAIL\n"); return 1; }
    }
    if (g_infos != 0 || g_deletes != 3) { std::printf("0051b0a0_TEST FAIL\n"); return 1; }
    std::printf("0051b0a0_TEST PASS\n");
    return 0;
}