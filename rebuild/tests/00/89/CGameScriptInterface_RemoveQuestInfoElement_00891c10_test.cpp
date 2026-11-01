#include <cstdio>

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void RemoveElement(long elementId);
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
    virtual void RemoveQuestInfoElement(long elementId) const;
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
static long g_ObservedElementId;
static unsigned long g_CallCount;

void NPlayerGui::CDrawQuestInfo::RemoveElement(long elementId)
{
    g_ObservedQuestInfo = this;
    g_ObservedElementId = elementId;
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
    interfaceObject.RemoveQuestInfoElement(0);
    if (g_CallCount != 1 ||
        g_ObservedQuestInfo != &firstQuestInfo ||
        g_ObservedElementId != 0)
        return 1;

    guiOwner.m_QuestInfo = &secondQuestInfo;
    interfaceObject.RemoveQuestInfoElement(
        static_cast<long>(0x87654321UL));
    if (g_CallCount != 2 ||
        g_ObservedQuestInfo != &secondQuestInfo ||
        g_ObservedElementId != static_cast<long>(0x87654321UL))
        return 2;

    std::printf("FSE2_00891c10_TEST PASS\n");
    return 0;
}
