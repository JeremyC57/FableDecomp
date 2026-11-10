// Behaviour test for 0x00AAB910 CSoundEmitterLH::CActiveSound::CActiveSound(long sampleID, CSoundBankLH* atmos, float startTime):
// state 2, the event handle, no bank, the start time, started, not stopping, sample 0, an empty name, the atmos.
#include <cstdio>
class CSoundBankLH { public: long Id; };
class CASoundBank;
class CCharString { public:
    const char* m_pData;
    CCharString() : m_pData(0) { }
};
class CBaseClassNonCopyable { protected:
    CBaseClassNonCopyable() { }
    virtual ~CBaseClassNonCopyable() { }
};
class CActiveSound : public CBaseClassNonCopyable { public:
    long          State;
    long          SampleID;
    CASoundBank*  PBank;
    float         StartTime;
    bool          Started;
    bool          Stopping;
    long          Sample;
    CCharString   Name;
    CSoundBankLH* PAtmos;
    CActiveSound(long sampleID, CSoundBankLH* atmos, float startTime)
        : State(2), SampleID(sampleID), PBank(0), StartTime(startTime), Started(true), Stopping(false), Sample(0), Name(), PAtmos(atmos)
    {
    }
};
int main()
{
    CSoundBankLH atmos = { 4 };
    CActiveSound s(17, &atmos, 0.75f);
    if (s.State != 2 || s.SampleID != 17 || s.PBank != 0 || s.StartTime != 0.75f || !s.Started || s.Stopping
        || s.Sample != 0 || s.Name.m_pData != 0 || s.PAtmos != &atmos) { std::printf("00aab910_TEST FAIL\n"); return 1; }
    std::printf("00aab910_TEST PASS\n");
    return 0;
}
