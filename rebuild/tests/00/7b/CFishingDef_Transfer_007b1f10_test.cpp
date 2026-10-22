// Behaviour test for CFishingDef::Transfer (0x007B1F10): a save writes the 19 members in order with the right types,
// a load reads them back, and each member is preceded by one CRC check.
#include <cstdio>
struct Log { int crc; int n; char kind[64]; double val[64]; };
static Log g;
struct CMemoryDataInputStream { double next; };
struct CDataOutputStream {
    void WriteLong(int value) { g.kind[g.n] = 'l'; g.val[g.n++] = value; }
    void WriteFloat(float value) { g.kind[g.n] = 'f'; g.val[g.n++] = value; }
};
struct CLongVector { int* _First; int* _Last; int* _End; };
void GFSerialiseVectorBinaryOutLong(CDataOutputStream*, CLongVector* v) { g.kind[g.n] = 'v'; g.val[g.n++] = (double)(v->_Last - v->_First); }
void GFSerialiseVectorBinaryInLong(CMemoryDataInputStream*, CLongVector*) { g.kind[g.n] = 'V'; g.val[g.n++] = 0; }
template<class T> struct CPersistTraits;
template<> struct CPersistTraits<int> {
    void TransferBinaryIn(CMemoryDataInputStream* s, int* value) { *value = (int)s->next; s->next += 1; g.kind[g.n] = 'L'; g.val[g.n++] = *value; }
    static void TransferBinaryOut(CDataOutputStream* s, const int& value) { s->WriteLong(value); }
};
template<> struct CPersistTraits<float> {
    void TransferBinaryIn(CMemoryDataInputStream* s, float* value) { *value = (float)s->next; s->next += 1; g.kind[g.n] = 'F'; g.val[g.n++] = *value; }
    static void TransferBinaryOut(CDataOutputStream* s, const float& value) { s->WriteFloat(value); }
};
struct CPersistContext {
    int m_mode; CMemoryDataInputStream* m_pLoadStream; CDataOutputStream* m_pSaveStream;
    void CheckCRC(const char*) { ++g.crc; }
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2: CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value); break;
        case 3: CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value); break;
        }
    }
    void Transfer(const char* name, CLongVector& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2: GFSerialiseVectorBinaryInLong(m_pLoadStream, &value); break;
        default: GFSerialiseVectorBinaryOutLong(m_pSaveStream, &value); break;
        }
    }
};
struct CFishingDef {
    CLongVector FishTypes;
    float FishingSpotProximity, FishingSpotMinDepth, FishingSpotMinHeightDiff, FishingSpotMaxHeightDiff;
    int PointsPerFishingLevel;
    float SlowFishingSpeed, NormalFishingSpeed, FastFishingSpeed, FastestFishingSpeed;
    float FishWeightRainMult, FishWeightSnowMult, FishWeightMistMult;
    float BaseWaitPeriodSeconds, MaxRandomWaitPeriodSeconds, HookWindowSeconds;
    int BaseHookPresses, MaxRandomHookPresses;
    float StrainIncreasePerPress;
    void Transfer(CPersistContext& context)
    {
        context.Transfer("", FishTypes);
        context.Transfer("", FishingSpotProximity);
        context.Transfer("", FishingSpotMinDepth);
        context.Transfer("", FishingSpotMinHeightDiff);
        context.Transfer("", FishingSpotMaxHeightDiff);
        context.Transfer("", PointsPerFishingLevel);
        context.Transfer("", SlowFishingSpeed);
        context.Transfer("", NormalFishingSpeed);
        context.Transfer("", FastFishingSpeed);
        context.Transfer("", FastestFishingSpeed);
        context.Transfer("", FishWeightRainMult);
        context.Transfer("", FishWeightSnowMult);
        context.Transfer("", FishWeightMistMult);
        context.Transfer("", BaseWaitPeriodSeconds);
        context.Transfer("", MaxRandomWaitPeriodSeconds);
        context.Transfer("", HookWindowSeconds);
        context.Transfer("", BaseHookPresses);
        context.Transfer("", MaxRandomHookPresses);
        context.Transfer("", StrainIncreasePerPress);
    }
};
static int Same(const char* a, const char* b, int n) { for (int i = 0; i < n; ++i) if (a[i] != b[i]) return 0; return 1; }
int main()
{
    int store[3] = { 1, 2, 3 };
    CFishingDef d;
    d.FishTypes._First = store; d.FishTypes._Last = store + 3; d.FishTypes._End = store + 3;
    d.FishingSpotProximity = 1.5f; d.PointsPerFishingLevel = 7; d.BaseHookPresses = 4; d.MaxRandomHookPresses = 9; d.StrainIncreasePerPress = 0.25f;
    d.FishingSpotMinDepth = d.FishingSpotMinHeightDiff = d.FishingSpotMaxHeightDiff = 0; d.SlowFishingSpeed = d.NormalFishingSpeed = 0;
    d.FastFishingSpeed = d.FastestFishingSpeed = d.FishWeightRainMult = d.FishWeightSnowMult = d.FishWeightMistMult = 0;
    d.BaseWaitPeriodSeconds = d.MaxRandomWaitPeriodSeconds = d.HookWindowSeconds = 0;
    CDataOutputStream out; CMemoryDataInputStream in; in.next = 10;
    CPersistContext save; save.m_mode = 3; save.m_pSaveStream = &out; save.m_pLoadStream = &in;
    g.crc = 0; g.n = 0;
    d.Transfer(save);
    int ok = g.crc == 19 && g.n == 19 && Same(g.kind, "vfffflffffffffffllf", 19)
        && g.val[0] == 3 && g.val[1] == 1.5 && g.val[5] == 7 && g.val[16] == 4 && g.val[17] == 9 && g.val[18] == 0.25;
    CPersistContext load = save; load.m_mode = 2;
    g.crc = 0; g.n = 0;
    d.Transfer(load);
    ok = ok && g.crc == 19 && Same(g.kind, "VFFFFLFFFFFFFFFFLLF", 19) && d.FishingSpotProximity == 10 && d.PointsPerFishingLevel == 14
        && d.StrainIncreasePerPress == 27;
    CPersistContext other = save; other.m_mode = 1;
    g.crc = 0; g.n = 0;
    d.Transfer(other);
    ok = ok && g.crc == 19 && g.n == 1 && g.kind[0] == 'v';
    if (!ok) { std::printf("007b1f10_TEST FAIL\n"); return 1; }
    std::printf("007b1f10_TEST PASS\n");
    return 0;
}