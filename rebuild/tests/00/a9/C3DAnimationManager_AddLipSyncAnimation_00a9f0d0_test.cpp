// Behaviour test for 0x00A9F0D0 C3DAnimationManager::AddLipSyncAnimation (model copy + recording stubs).
#include <cstdio>
#include <string.h>

static int g_strings = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { ++g_strings; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_strings; }
    ~CCharString() { --g_strings; }
};

class CLipSyncPhoneme { public:
    CCharString Symbol;
    unsigned long ID;
    float Weight;
};

template <class T> class CArray { public:
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    unsigned long size() const { return m_pEnd - m_pBegin; }
    const T& operator[](unsigned long i) const { return m_pBegin[i]; }
};

class CLipSyncFrameData { public:
    CArray<CLipSyncPhoneme> Phonemes;
};

class CIVCountedPointeeBase { public:
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(1) {}
    virtual ~CIVCountedPointeeBase() {}
};
class C3DAnimationStateBase : public CIVCountedPointeeBase { public:
    virtual long GetType() const { return 0; }
};
class C3DAnimationState : public C3DAnimationStateBase { public:
    long AnimationIndex;
    float AnimationTime;
    C3DAnimationState(long index, float time) : AnimationIndex(index), AnimationTime(time) {}
};
class C3DAnimationStateAlpha : public C3DAnimationState { public:
    float AlphaWeight;
    C3DAnimationStateAlpha(long index, float time, float alpha) : C3DAnimationState(index, time), AlphaWeight(alpha) {}
    virtual long GetType() const { return 2; }
};

static long g_addedIndex[8]; static float g_addedTime[8]; static float g_addedAlpha[8]; static unsigned long g_addedRef[8]; static int g_added = 0;
class C3DAnimationBlendState { public:
    void AddCombinationBlend(C3DAnimationStateAlpha* state)
    {
        g_addedIndex[g_added] = state->AnimationIndex; g_addedTime[g_added] = state->AnimationTime;
        g_addedAlpha[g_added] = state->AlphaWeight; g_addedRef[g_added] = state->IVRefCount; ++g_added;
    }
};

static const char* g_lastSet = 0;
struct CAnimationBank {
    long GetLipSyncAnimationIndex(const CCharString& set, const CCharString& phoneme)
    {
        g_lastSet = set.m_pData;
        if (strcmp(phoneme.m_pData, "AA") == 0) return 11;
        if (strcmp(phoneme.m_pData, "EE") == 0) return 22;
        return 0;
    }
};

class C3DAnimationManager { public:
    virtual ~C3DAnimationManager() {}
    CAnimationBank* Bank;
    void AddLipSyncAnimation(C3DAnimationBlendState& blend, const CLipSyncFrameData& frame, const CCharString& set);
};

void C3DAnimationManager::AddLipSyncAnimation(C3DAnimationBlendState& blend, const CLipSyncFrameData& frame, const CCharString& set)
{
    for (unsigned long i = 0; i < frame.Phonemes.size(); ++i) {
        CLipSyncPhoneme phoneme = frame.Phonemes[i];
        long index;
        {
            CCharString symbol(phoneme.Symbol);
            index = Bank->GetLipSyncAnimationIndex(set, symbol);
        }
        if (index) {
            C3DAnimationStateAlpha state(index, 0.0f, phoneme.Weight * 0.5);
            blend.AddCombinationBlend(&state);
        }
    }
}

int main()
{
    CLipSyncPhoneme ph[3] = { { CCharString("AA"), 1, 0.8f }, { CCharString("XX"), 2, 0.6f }, { CCharString("EE"), 3, 0.4f } };
    CLipSyncFrameData frame; frame.Phonemes.m_pBegin = ph; frame.Phonemes.m_pEnd = ph + 3; frame.Phonemes.m_pCapacity = ph + 3;
    CAnimationBank bank; C3DAnimationManager manager; manager.Bank = &bank;
    C3DAnimationBlendState blend; CCharString set("Hero");
    int before = g_strings;
    manager.AddLipSyncAnimation(blend, frame, set);
    float a0 = 0.4f, a1 = 0.2f;
    if (g_added != 2 || g_addedIndex[0] != 11 || g_addedIndex[1] != 22 || g_addedTime[0] != 0.0f || g_addedTime[1] != 0.0f ||
        g_addedAlpha[0] != a0 || g_addedAlpha[1] != a1 || g_addedRef[0] != 1 || g_addedRef[1] != 1 ||
        g_lastSet != set.m_pData || g_strings != before) {
        std::printf("00a9f0d0_TEST FAIL\n"); return 1;
    }
    frame.Phonemes.m_pEnd = ph;   // empty frame: nothing added
    manager.AddLipSyncAnimation(blend, frame, set);
    if (g_added != 2) { std::printf("00a9f0d0_TEST FAIL\n"); return 1; }
    std::printf("00a9f0d0_TEST PASS\n");
    return 0;
}