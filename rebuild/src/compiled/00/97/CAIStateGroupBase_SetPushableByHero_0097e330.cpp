// CAIStateGroupBase::SetPushableByHero(bool pushable) -- writes the flag (+0x79) on the creature's interface-170 component when the creature has that interface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097E330, length 69 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (landed callers).
// engine/CAIStateGroupBase.h: PBrain +0x04; engine/CAIBrain.h: Creature +0x18. The body is the
// creature's inline GetInterface(type, &out) of the landed 0x007BEA08 family: the presence
// bitset at +0x20 is tested on the raw masked word (type 0xaa -> word 5 at +0x34, bit 10:
// `test ch,4`), the sorted interface map at +0x44 is searched with LowerBound (end or a larger
// key -> the end entry), the component goes to the out pointer (eax) and `true` folds the
// caller's `if`; the component's byte at +0x79 then receives the argument. The type key lives
// in the `push ecx` frame slot and the map pointer is a named local (esi). The leaf keeps the
// manifest name (the flag matches CAIScratchpadDef::IsPushableByHero's purpose); the component
// class is not identified.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCBase { public:
    unsigned char _pad[0x79];
    bool          m_bPushableByHero;        // +0x79
};

class CThingAICreature { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    bool GetInterface(long type, CTCBase** out)   // inline: the landed GetInterface_0x.. family shape
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = it->second;
            return true;
        }
        return false;
    }
};

class CAIBrain { public:
    void*             __vftable;            // +0x00
    unsigned char     _pad[0x14];           // +0x04..+0x18
    CThingAICreature* Creature;             // +0x18
};

class CAIStateGroupBase { public:
    void*     __vftable;                    // +0x00
    CAIBrain* PBrain;                       // +0x04
    void SetPushableByHero(bool pushable);
};

void CAIStateGroupBase::SetPushableByHero(bool pushable)
{
    CTCBase* component;
    if (PBrain->Creature->GetInterface(0xaa, &component))
        component->m_bPushableByHero = pushable;
}