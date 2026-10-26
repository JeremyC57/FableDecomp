#include <cstdio>
#include <stddef.h>
#include <new>
static int g_base = 0;
class CSubDefClassBase { public: virtual void v00() { } unsigned char _body[0x24]; CSubDefClassBase() { ++g_base; } };
class CDefString { public: long TablePos; CDefString() : TablePos(-1) { } };
class CPrizeScoreDef;
template<class T> struct CRetailVector { T* first; T* last; T* end_of_storage; CRetailVector() : first(0), last(0), end_of_storage(0) { } };
class CTavernGameDef : public CSubDefClassBase { public:
    unsigned long Banter; unsigned char _ids_0x2c[0x64]; CDefString CameraName; unsigned char _ids_0x94[0x88];
    CDefString ScoreFont; CDefString TargetFont; CDefString StatsFont; unsigned char _layout_0x128[0x4c];
    CRetailVector<CPrizeScoreDef> PrizeScores; long Prize; unsigned char _tail_0x184[0x24]; virtual void v00() { }
    CTavernGameDef() : CSubDefClassBase(), CameraName(), ScoreFont(), TargetFont(), StatsFont(), PrizeScores() { }
};
int main()
{
    unsigned char raw[sizeof(CTavernGameDef)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAA;
    CTavernGameDef* d = new (raw) CTavernGameDef;
    if (g_base != 1 || d->CameraName.TablePos != -1 || d->ScoreFont.TablePos != -1 || d->TargetFont.TablePos != -1 || d->StatsFont.TablePos != -1
        || d->PrizeScores.first || d->PrizeScores.last || d->PrizeScores.end_of_storage || d->Banter != 0xAAAAAAAA
        || offsetof(CTavernGameDef, CameraName) != 0x90 || offsetof(CTavernGameDef, ScoreFont) != 0x11c || offsetof(CTavernGameDef, StatsFont) != 0x124
        || offsetof(CTavernGameDef, PrizeScores) != 0x174 || sizeof(*d) != 0x1a8) { std::printf("004e1049_TEST FAIL\n"); return 1; }
    std::printf("004e1049_TEST PASS\n"); return 0;
}