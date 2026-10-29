// CQuestManager::SetQuestCardRenownReward(const CCharString&, long) -- store the renown reward on the quest-card component of the active quest card for the given script, when the card exists and is not dead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B0EB0, length 93 bytes.
// Callees: CQuestManager::GetActiveQuestCardFromScriptName(const CCharString&) @ 0x004B0C80 (manifest),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the thing's inlined GetInterface for the
// quest-card component, type 0x6c = word 3 / bit 12 of the presence bitset: `test ch,10h` on +0x2c).
// Identity: FableWin.pdb ?SetQuestCardRenownReward@CQuestManager@@QAEXABVCCharString@@J@Z; the twin @ 0x004B0E50 is
// SetQuestCardGoldReward.
// Layout: engine/CTCQuestCard.h (PDB) GoldReward +0x2c, RenownReward +0x30; the thing's dead flag is bit 0 of
// +0x91 (as in the landed CGameScriptInterface::DeactivateBoastUi model), the presence bitset at +0x20, the
// sorted {type, component} map at +0x44.
// Shape: the out-parameter GetInterface template (the landed 0x008CB5C0 / 0x007504E0 shape) with its result
// ignored: the key temporary, the component pointer and the quest argument share ONE dead argument slot, so the
// absent path tests whatever that slot holds (`mov eax,[esp+4]`) -- the out pointer is not initialised.
#include "engine/CTCQuestCard.h"

class CTCBase;
class CCharString { public:
    void* PStringData;
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44 (three words)
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad50[0x41];
    unsigned char m_dead : 1;               // +0x91 bit 0
    unsigned char m_91 : 7;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CQuestManager { public:
    void SetQuestCardRenownReward(const CCharString& scriptName, long reward);
public:
    CThing* GetActiveQuestCardFromScriptName(const CCharString&) const;   // 0x004B0C80
};

void CQuestManager::SetQuestCardRenownReward(const CCharString& scriptName, long reward)
{
    CThing* card = GetActiveQuestCardFromScriptName(scriptName);
    if (card && !card->m_dead) {
        CTCQuestCard* questCard;
        card->GetInterface(&questCard, 0x6c);
        if (questCard)
            questCard->RenownReward = reward;
    }
}