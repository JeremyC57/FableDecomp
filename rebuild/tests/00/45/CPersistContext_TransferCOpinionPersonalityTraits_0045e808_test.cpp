// Behaviour test for 0x0045E808 CPersistContext::Transfer<COpinionPersonalityTraits*>: CRC check, then the five entries of the
// traits array loaded (mode 2) or saved (mode 3) in order; other modes touch nothing
#include <cstdio>
#include <cstring>
static char g_log[64];
class CDataOutputStream { public: int id; };
class CMemoryDataInputStream { public: int id; };
class COpinionPersonalityTraits { public:
    float BaseOffset, HeroStatScaling, VillageOpinionScaling, TimeScalingRunIn, TimeScalingRunOut;
    float PeakScalingIfPos, PeakScalingIfNeg, PersistScalingIfPos, PersistScalingIfNeg;
    void TransferBinaryOut(CDataOutputStream* s) const { if (s->id == 7) { char b[2] = { (char)('0' + (int)BaseOffset), 0 }; std::strcat(g_log, b); } }
    void TransferBinaryIn(CMemoryDataInputStream* s) { if (s->id == 6) { HeroStatScaling = BaseOffset * 2.0f; std::strcat(g_log, "i"); } }
};
const unsigned long OPINION_PERSONALITY_TRAITS_COUNT = 5;
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<COpinionPersonalityTraits*> {
    COpinionPersonalityTraits* DefaultVal;
    void TransferBinaryOut(CDataOutputStream* s, COpinionPersonalityTraits* const* v)
    { for (unsigned long i = 0; i < OPINION_PERSONALITY_TRAITS_COUNT; ++i) (*v)[i].TransferBinaryOut(s); }
    void TransferBinaryIn(CMemoryDataInputStream* s, COpinionPersonalityTraits** v)
    { for (unsigned long i = 0; i < OPINION_PERSONALITY_TRAITS_COUNT; ++i) (*v)[i].TransferBinaryIn(s); }
};
class CPersistContext { public:
    long Mode; CMemoryDataInputStream* PLoadStream; CDataOutputStream* PSaveStream;
    void CheckCRC(const char* name) { if (name[0] == 0) std::strcat(g_log, "K"); }
    template <class T> void Transfer(T& value)
    {
        CheckCRC("");
        CPersistTraits<T> traits;
        switch (Mode) {
        case 2: traits.TransferBinaryIn(PLoadStream, &value); break;
        case 3: traits.TransferBinaryOut(PSaveStream, &value); break;
        }
    }
};
int main()
{
    COpinionPersonalityTraits arr[6];
    std::memset(arr, 0, sizeof arr);
    for (int i = 0; i < 6; ++i) arr[i].BaseOffset = (float)(i + 1);
    COpinionPersonalityTraits* p = arr;
    CMemoryDataInputStream in = { 6 };
    CDataOutputStream out = { 7 };
    CPersistContext ctx; ctx.PLoadStream = &in; ctx.PSaveStream = &out;
    bool ok = true;
    g_log[0] = 0; ctx.Mode = 3; ctx.Transfer(p); ok = ok && std::strcmp(g_log, "K12345") == 0;
    g_log[0] = 0; ctx.Mode = 2; ctx.Transfer(p); ok = ok && std::strcmp(g_log, "Kiiiii") == 0 && arr[4].HeroStatScaling == 10.0f && arr[5].HeroStatScaling == 0.0f;
    g_log[0] = 0; ctx.Mode = 1; ctx.Transfer(p); ok = ok && std::strcmp(g_log, "K") == 0;
    if (!ok) { std::printf("0045e808_TEST FAIL\n"); return 1; }
    std::printf("0045e808_TEST PASS\n");
    return 0;
}