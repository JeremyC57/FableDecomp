// CAIStateGroup_HoverHornetQueen::KillBossHelpers() -- gets the creature's boss component (interface type 0xAD) and calls its KillHelpers
// Leaf: KillBossHelpers
// Retail: Fable.exe @ 0x0095AA00, 81 bytes. Only caller: CAIStateGroup_HoverHornetQueen::OnReplace
//         (0x0095AB30, vtable 0x0128D484 slot 10) -- a direct `call` with `this` first thing.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface),
//          CTCBoss::KillHelpers 0x007AF490 (manifest label, also called at 0x0097561B).
// Label: the manifest's "CTCTavernTable::FreeServingPoint" is propagated over the byte-identical
// GetInterface shape (as the landed CTCTavernTable::FreePoint 0x007B9090); here `this`+4 is the
// state group's PBrain and brain+0x18 its Creature (engine/CAIBrain.h), so it is a hornet-queen
// state-group helper; the method name is descriptive.
// Shape: the landed out-parameter GetInterface template (0x007B9090): type 0xAD tests bit 13 of the
// presence word at +0x34 (`test ch,20h`), the key lives in the `push ecx` slot, the map pointer is a
// named local (esi); on a miss the never-written component local is read from that same slot
// (`mov eax,[esp]`) -- the callee is called on it anyway, so both arms end in the call.
// `this` is modelled through engine/CAIStateGroupBase.h (PBrain +0x04): the flat
// engine/CAIStateGroup_HoverHornetQueen.h hides PBrain in `_pad_0x04`.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);      // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
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

class CThingAICreature : public CThing { public:
};

class CTCBoss { public:
    void KillHelpers();                         // 0x007AF490
};

struct CAIStateGroup_HoverHornetQueen : CAIStateGroupBase {
    void KillBossHelpers();
};

void CAIStateGroup_HoverHornetQueen::KillBossHelpers()
{
    CTCBoss* boss;
    PBrain->Creature->GetInterface(&boss, 0xAD);
    boss->KillHelpers();
}