#include <cstdio>

enum EGameAction
{
    EGameAction_None = 0
};

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long AddTick(
        EGameAction action,
        bool completed,
        float fadeTime);
};
}

class CPlayerGui
{
public:
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual long AddQuestInfoTick(
        EGameAction action,
        bool completed,
        float fadeTime) const;
};

union FloatBits
{
    float value;
    unsigned long bits;
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; T& operator*() const { return *Data; } };
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the source reads the private static PClass through Get()
    static CCountedPointer<T> PClass;
public:
    static T& __fastcall Get() { return *PClass; }
    static void __fastcall Set(T* p) { PClass.Data = p; PClass.Info = 0; }   // ?Set@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIXPAVCPlayerGui@@@Z
};
template <class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template class CTBaseSingletonNoDefaultAlloc<CPlayerGui>;
static NPlayerGui::CDrawQuestInfo* g_ObservedQuestInfo;
static EGameAction g_ObservedAction;
static bool g_ObservedCompleted;
static unsigned long g_ObservedFadeTimeBits;
static long g_Result;
static unsigned long g_CallCount;

long NPlayerGui::CDrawQuestInfo::AddTick(
    EGameAction action,
    bool completed,
    float fadeTime)
{
    FloatBits captured;
    captured.value = fadeTime;
    g_ObservedQuestInfo = this;
    g_ObservedAction = action;
    g_ObservedCompleted = completed;
    g_ObservedFadeTimeBits = captured.bits;
    ++g_CallCount;
    return g_Result;
}

static int CheckCall(
    CGameScriptInterface& interfaceObject,
    CPlayerGui& guiOwner,
    NPlayerGui::CDrawQuestInfo& questInfo,
    EGameAction action,
    bool completed,
    unsigned long fadeTimeBits,
    long result)
{
    FloatBits input;
    input.bits = fadeTimeBits;
    guiOwner.m_QuestInfo = &questInfo;
    g_Result = result;

    const long observedResult =
        interfaceObject.AddQuestInfoTick(
            action,
            completed,
            input.value);
    if (observedResult != result ||
        g_ObservedQuestInfo != &questInfo ||
        g_ObservedAction != action ||
        g_ObservedCompleted != completed ||
        g_ObservedFadeTimeBits != fadeTimeBits)
        return 1;
    return 0;
}

int main()
{
    NPlayerGui::CDrawQuestInfo firstQuestInfo;
    NPlayerGui::CDrawQuestInfo secondQuestInfo;
    CPlayerGui guiOwner;
    CGameScriptInterface interfaceObject;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(&guiOwner);

    if (CheckCall(
            interfaceObject,
            guiOwner,
            firstQuestInfo,
            EGameAction_None,
            false,
            0x80000000UL,
            0) != 0)
        return 1;
    if (g_CallCount != 1)
        return 2;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            secondQuestInfo,
            static_cast<EGameAction>(0x76543210),
            true,
            0xFFC12345UL,
            static_cast<long>(0xFEDCBA98UL)) != 0)
        return 3;
    if (g_CallCount != 2)
        return 4;

    std::printf("FSE2_00891b70_TEST PASS\n");
    return 0;
}
