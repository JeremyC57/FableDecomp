#include <stddef.h>

class CCharString
{
public:
    void* m_Value;
    CCharString(const CCharString& other);
};

class CTCBase;
class CTCCreditsUI;

enum ETCInterfaceType
{
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>

typedef std::pair<ETCInterfaceType, CTCBase*> KickOffCreditsScreenPair;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    std::pair<TKey, TValue>* m_pBegin;
    std::pair<TKey, TValue>* m_pEnd;

    std::pair<TKey, TValue>* LowerBound(const TKey& key);
};

class CTCCreditsUI
{
public:
    void __fastcall Activate(CCharString screenName);
};

class CGameScriptInterface
{
public:
    void KickOffCreditsScreen(CCharString& screenName);
};

typedef CVectorMap<
    ETCInterfaceType,
    CTCBase*,
    CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >
    KickOffCreditsScreenInterfaceMap;

struct CGameScriptInterfaceVTableOverlay
{
    unsigned char m_Pad00[0x1C];
    void (__fastcall* m_RawSlot1C)(CGameScriptInterface* pThis);
};

struct CGameScriptInterfaceOverlay
{
    const CGameScriptInterfaceVTableOverlay* m_pVTable;
    unsigned char m_Pad04[0x10];
    void* m_Field14;
};

struct KickOffCreditsScreenResolvedOverlay
{
    unsigned char m_Pad00[0x40];
    unsigned char m_Flags40;
    unsigned char m_Pad41[0x03];
    KickOffCreditsScreenInterfaceMap m_InterfaceMap44;
};
class CThingPlayerCreature : public KickOffCreditsScreenResolvedOverlay { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

extern "C" CPlayer* __fastcall Helper00449970(void* value);

void CGameScriptInterface::KickOffCreditsScreen(
    CCharString& screenName)
{
    const CGameScriptInterfaceOverlay* self =
        (const CGameScriptInterfaceOverlay*)this;
    CTCCreditsUI* creditsUI = 0;
    KickOffCreditsScreenResolvedOverlay* resolved =
        Helper00449970(self->m_Field14)->GetPControlledCreature();

    if ((resolved->m_Flags40 & 0x02) != 0)
    {
        ETCInterfaceType interfaceType =
            (ETCInterfaceType)0x101;
        KickOffCreditsScreenInterfaceMap* map =
            (KickOffCreditsScreenInterfaceMap*)&resolved->m_InterfaceMap44;
        KickOffCreditsScreenPair* entry =
            map->LowerBound(interfaceType);
        if (entry == map->m_pEnd ||
            (ETCInterfaceType)0x101 < entry->first)
            entry = map->m_pEnd;
        creditsUI = (CTCCreditsUI*)entry->second;
    }

    creditsUI->Activate(CCharString(screenName));
    self->m_pVTable->m_RawSlot1C((CGameScriptInterface*)this);
}
