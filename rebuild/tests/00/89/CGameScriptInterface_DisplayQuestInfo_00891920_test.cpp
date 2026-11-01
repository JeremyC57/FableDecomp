#include <cstdio>

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void DisplayQuestInfo(bool display);
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
    virtual void DisplayQuestInfo(bool display) const;
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
static bool g_ObservedDisplay;
static unsigned long g_CallCount;

void NPlayerGui::CDrawQuestInfo::DisplayQuestInfo(
    bool display)
{
    g_ObservedQuestInfo = this;
    g_ObservedDisplay = display;
    ++g_CallCount;
}

static int CheckCall(
    CGameScriptInterface& interfaceObject,
    CPlayerGui& guiOwner,
    NPlayerGui::CDrawQuestInfo& questInfo,
    bool requestedDisplay)
{
    guiOwner.m_QuestInfo = &questInfo;
    g_ObservedDisplay = false;
    interfaceObject.DisplayQuestInfo(requestedDisplay);
    if (g_ObservedQuestInfo != &questInfo ||
        !g_ObservedDisplay)
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
            false) != 0)
        return 1;
    if (g_CallCount != 1)
        return 2;

    if (CheckCall(
            interfaceObject,
            guiOwner,
            secondQuestInfo,
            true) != 0)
        return 3;
    if (g_CallCount != 2)
        return 4;

    std::printf("FSE2_00891920_TEST PASS\n");
    return 0;
}
