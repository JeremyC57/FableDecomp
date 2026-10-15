#include <cstdio>
#include <cstring>
#include <new>
static int g_baseCalls = 0;
struct CParentDefClassBase {
    virtual void v00() { }
    unsigned char _base[0x38];
    CParentDefClassBase() { ++g_baseCalls; std::memset(_base, 0x11, sizeof(_base)); }
};
template <class T> struct CRetailVector { T* first; T* last; T* end; CRetailVector() : first(0), last(0), end(0) { } };
struct CSoundBankEntry; struct CAtmosBankEntry; struct CMusicEntry; struct CMusicSetEntry; struct CReverbEnvironmentEntry;
struct CSoundDef : CParentDefClassBase {
    CRetailVector<CSoundBankEntry> SoundBankEntries;
    CRetailVector<CAtmosBankEntry> AtmosBankEntries;
    CRetailVector<CMusicEntry> MusicEntries;
    CRetailVector<CMusicSetEntry> MusicSetEntries;
    CRetailVector<CReverbEnvironmentEntry> ReverbEnvironmentEntries;
    float AtmosListenerDampingMin, AtmosListenerDampingRange, DistanceModelCurve;
    virtual void v00() { }
    CSoundDef() : CParentDefClassBase() { }
};
int main()
{
    static unsigned char buf[sizeof(CSoundDef)];
    std::memset(buf, 0xCD, sizeof(buf));
    CSoundDef* d = new (buf) CSoundDef();
    bool ok = g_baseCalls == 1 && d->_base[0] == 0x11
        && !d->SoundBankEntries.first && !d->SoundBankEntries.last && !d->SoundBankEntries.end
        && !d->AtmosBankEntries.first && !d->AtmosBankEntries.end && !d->MusicEntries.last
        && !d->MusicSetEntries.first && !d->ReverbEnvironmentEntries.end;
    if (!ok) { std::printf("00458413_TEST FAIL\n"); return 1; }
    std::printf("00458413_TEST PASS\n");
    return 0;
}
