// CTCQuestCompletionUI::SaveInitialValues(const CCharString& questName) -- when the named quest's active card has a def index, remember the hero's renown, gold and experience at the quest start under that index
// Leaf: ?SaveInitialValues@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E4860, 384 bytes (375 without padding), thiscall `ret 4`. Ego_r.pdb
//   `?SaveInitialValues@CTCQuestCompletionUI@@QAEXABVCCharString@@@Z`; called from CGameScriptInterface::
//   KickOffQuestStartScreen 0x00896B60 (on the hero's interface 0xFC).
// Callees: CQuestManager::GetActiveQuestCardFromScriptName 0x004B0C80 (on g_pQuestManager 0x013B89FC),
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline GetInterface 0x6C CTCQuestCard, 4 CTCHeroStats
//   twice, 0x68 the experience component), map<long, unsigned long>::operator[] 0x005E9760 (x3; catalog
//   "BinaryTreeInsert").
// Label: catalog "QuestCard_SetupDialog" -- this is CTCQuestCompletionUI::SaveInitialValues (PDB name).
// Layout: CTCBase Thing +4; CThing interface bitset +0x20, map +0x44, DefGlobalIndex +0x8c (PDB +0x9c); CTCQuestCompletionUI
//   StartQuestExperience +0x84, StartQuestGold +0x90, StartQuestRenown +0x9c (PDB +0x8c/+0x9c/+0xac: retail 12-byte
//   vector / 4-byte list / 12-byte maps, the double LastRenderTime 8-aligned); CTCHeroStats Gold +0x48, Renown +0x7c;
//   the experience component's Experience +0x10.
// Shape: CThing::GetInterface(T**, type) is the forceinline bitset test + LowerBound select; giving LowerBound a same-TU
//   noinline body (it only reads *type) lets VC7.1 treat the key local as uncaptured, so the 0x6C key, `card` and the
//   last map key all share the dead questName slot and the frame is a single `push ecx`. `card` (address taken by
//   GetInterface) stays homed and is re-read; questIndex lives in di for the test and the first key. The renown store
//   loads the value before the key; the gold/experience stores name the key first (`long key = ...`) and then the value,
//   which is retail's key-chain-before-value order ahead of each operator[] call.
class CCharString;
class CTCBase;

class CInterfaceMap {
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                   // +0x20
    CInterfaceMap  m_interfaces;                       // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short DefGlobalIndex;                     // +0x8c
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};

#pragma auto_inline(off)
inline __declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* lo = first;
    int n = last - first;
    while (n > 0) {
        int half = n >> 1;
        if (lo[half].type < *type) { lo += half + 1; n -= half + 1; }
        else n = half;
    }
    return lo;
}
#pragma auto_inline(on)

class CQuestManager {
public:
    CThing* GetActiveQuestCardFromScriptName(const CCharString& name);   // 0x004B0C80
};
extern CQuestManager* g_pQuestManager;                 // 0x013B89FC

class CTCBaseRoot {
public:
    void*   __vftable;                                 // +0x00
    CThing* Thing;                                     // +0x04
};
class CTCQuestCard : public CTCBaseRoot { };           // interface 0x6C
class CTCHeroStats : public CTCBaseRoot {              // interface 0x04
public:
    unsigned char _pad08[0x48 - 0x08];
    unsigned long Gold;                                // +0x48
    unsigned char _pad4c[0x7c - 0x4c];
    unsigned long Renown;                              // +0x7c
};
class CTCHeroExperience : public CTCBaseRoot {         // interface 0x68
public:
    unsigned char _pad08[0x10 - 0x08];
    unsigned long Experience;                          // +0x10
};

namespace std {
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    void*        _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    _Tp& operator[](const _Key& __k);                  // 0x005E9760 for map<long, unsigned long>
};
}

class CTCQuestCompletionUI : public CTCBaseRoot {
public:
    unsigned char _pad08[0x84 - 0x08];
    std::map<long, unsigned long> StartQuestExperience;   // +0x84
    std::map<long, unsigned long> StartQuestGold;         // +0x90
    std::map<long, unsigned long> StartQuestRenown;       // +0x9c
    void SaveInitialValues(const CCharString& questName);
};

void CTCQuestCompletionUI::SaveInitialValues(const CCharString& questName)
{
    CThing* cardThing = g_pQuestManager->GetActiveQuestCardFromScriptName(questName);
    CTCQuestCard* card;
    if (cardThing && cardThing->GetInterface(&card, 0x6c) && card) {
        unsigned short questIndex = card->Thing->DefGlobalIndex;
        if (questIndex) {
            CTCHeroStats* stats;
            if (Thing->GetInterface(&stats, 4)) {
                unsigned long renown = stats->Renown;
                StartQuestRenown[questIndex] = renown;
            }
            if (Thing->GetInterface(&stats, 4)) {
                long key = card->Thing->DefGlobalIndex;
                unsigned long gold = stats->Gold;
                StartQuestGold[key] = gold;
            }
            CTCHeroExperience* exp;
            if (Thing->GetInterface(&exp, 0x68)) {
                long key = card->Thing->DefGlobalIndex;
                unsigned long experience = exp->Experience;
                StartQuestExperience[key] = experience;
            }
        }
    }
}