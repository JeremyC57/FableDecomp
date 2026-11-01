#include <cstdio>

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void UpdateCounterList(
        long questId,
        long value,
        long maximum);
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
    virtual void UpdateQuestInfoCounterList(
        long questId,
        long value,
        long maximum) const;
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
static long g_ObservedQuestId;
static long g_ObservedValue;
static long g_ObservedMaximum;
static unsigned long g_CallCount;

void NPlayerGui::CDrawQuestInfo::UpdateCounterList(
    long questId,
    long value,
    long maximum)
{
    g_ObservedQuestInfo = this;
    g_ObservedQuestId = questId;
    g_ObservedValue = value;
    g_ObservedMaximum = maximum;
    ++g_CallCount;
}

static int CheckCall(
    CGameScriptInterface& interfaceObject,
    CPlayerGui& guiOwner,
    NPlayerGui::CDrawQuestInfo& questInfo,
    long questId,
    long value,
    long maximum)
{
    guiOwner.m_QuestInfo = &questInfo;
    interfaceObject.UpdateQuestInfoCounterList(
        questId,
        value,
        maximum);
    if (g_ObservedQuestInfo != &questInfo ||
        g_ObservedQuestId != questId ||
        g_ObservedValue != value ||
        g_ObservedMaximum != maximum)
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
            0,
            1,
            2) != 0)
        return 1;
    if (g_CallCount != 1)
        return 2;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            secondQuestInfo,
            static_cast<long>(0x87654321UL),
            static_cast<long>(0xFEDCBA98UL),
            static_cast<long>(0x7FFFFFFFUL)) != 0)
        return 3;
    if (g_CallCount != 2)
        return 4;

    std::printf("FSE2_00891be0_TEST PASS\n");
    return 0;
}
