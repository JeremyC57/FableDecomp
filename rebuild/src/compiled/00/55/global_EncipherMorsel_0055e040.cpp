// _global::EncipherMorsel(unsigned long* v, unsigned long* w, unsigned long* k) -- XTEA-encrypts one 64-bit block (32 rounds, 128-bit key)
// Flags: /Gr
// Leaf: ?EncipherMorsel@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055E040, 312 bytes, __fastcall (ecx = v, edx = w, [esp+4] = k), `ret 4`. Grade: MATCH.
// Identity: Ego_r.pdb / FableWin.pdb ?EncipherMorsel@?A0xf010c0d0@@YIXPAK00@Z (anonymous namespace). Ego_d.pdb locals v, w, k, v0, delta, sum, v1, i.
// TU: the scoreboard crypto TU (Ego_d.pdb line-table file id 2880; retail order EncipherMorsel 0x0055E040, DecipherMorsel 0x0055E180,
//   CryptoMatic 0x0055E2C0, Explode 0x00560920, GetDecryptedDocument 0x00560BB0). All five rows share this define block and /Gr: the
//   retail TU is a __fastcall-default one (the anonymous-namespace helpers and the STLport instances it calls -- search 0x00567120,
//   __uninitialized_copy 0x00568E70 -- are __fastcall without a spelling; Explode grades DIFFER(472v653) without /Gr).
// Text: the landed 0x00563480 file's same-TU text with Ego_d.pdb's names; the morsels' declarations split over two lines as Ego_d's
//   line table has them (lines 83 / 84, 95 / 96). See the batch README.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_STRING_FIND
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_SEARCH
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_EXPLODE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_SCOREBOARD
#define FABLE_RETAIL_LHHTTP2_DOCUMENT
#include "engine/CTCHeroOnlineScoreboard.h"
#include "stlport/fable_string_bodies.h"

void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*) throw();

namespace {
void FABLE_FASTCALL EncipherMorsel(unsigned long* v,unsigned long* w,unsigned long* k)
{
    unsigned long v0=v[0],v1=v[1];
    unsigned long sum=0,delta=0x9e3779b9;
    for(unsigned long i=0;i<32;++i) {
        v0+=(((v1<<4)^(v1>>5))+v1)^(sum+k[sum&3]);
        sum+=delta;
        v1+=(((v0<<4)^(v0>>5))+v0)^(sum+k[(sum>>11)&3]);
    }
    w[0]=v0;w[1]=v1;
}
}
