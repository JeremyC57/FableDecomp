#include <cstdio>

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void UpdateTick(long questId, bool enabled);
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
    virtual void UpdateQuestInfoTick(
        long questId,
        bool enabled) const;
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
static bool g_ObservedEnabled;
static unsigned long g_CallCount;

void NPlayerGui::CDrawQuestInfo::UpdateTick(
    long questId,
    bool enabled)
{
    g_ObservedQuestInfo = this;
    g_ObservedQuestId = questId;
    g_ObservedEnabled = enabled;
    ++g_CallCount;
}

int main()
{
    NPlayerGui::CDrawQuestInfo firstQuestInfo;
    NPlayerGui::CDrawQuestInfo secondQuestInfo;
    CPlayerGui guiOwner;
    CGameScriptInterface interfaceObject;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(&guiOwner);

    guiOwner.m_QuestInfo = &firstQuestInfo;
    interfaceObject.UpdateQuestInfoTick(0, false);
    if (g_CallCount != 1 ||
        g_ObservedQuestInfo != &firstQuestInfo ||
        g_ObservedQuestId != 0 ||
        g_ObservedEnabled)
        return 1;

    guiOwner.m_QuestInfo = &secondQuestInfo;
    interfaceObject.UpdateQuestInfoTick(
        static_cast<long>(0x87654321UL),
        true);
    if (g_CallCount != 2 ||
        g_ObservedQuestInfo != &secondQuestInfo ||
        g_ObservedQuestId != static_cast<long>(0x87654321UL) ||
        !g_ObservedEnabled)
        return 2;

    std::printf("FSE2_00891bf0_TEST PASS\n");
    return 0;
}
