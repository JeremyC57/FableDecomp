// CCreatureAction_FinishFireballSpell::SetAnims() -- if the caster has a fireball-spell component (interface 0x9d), use its release animation name as the action's animation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BAB70, length 98 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020,
//          CTCFireballSpell::GetReleaseAnimName() (CCharString by value, hidden result pointer) @ 0x007C86F0,
//          CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0 (manifest label
//          "CTCInGameMenu::SetPDef" is propagated), CCharString::~CCharString @ 0x0099EAE0.
// Shape: the bool-returning out-parameter GetInterface template (landed AlertToPlayer 0x00849320 shape) in the `if`:
//   bit 0x9d is tested in memory (`test dword ptr [eax+30h],20000000h`); the returned string temporary lands in the
//   same `push ecx` frame word as the lookup key, is passed by reference and destroyed after the setter.
// Layout: CCombatActionBase (retail 0x114 bytes) with PCreature at +8; FinishFireballSpell adds nothing used here.
class CTCBase;

class CCharString { public:
    void* m_pData;
    ~CCharString();                         // 0x0099EAE0
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCFireballSpell { public:                   // interface type 0x9d component
    CCharString GetReleaseAnimName();       // 0x007C86F0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_FinishFireballSpell : public CCombatActionBase { public:
    void SetAnims();
};

void CCreatureAction_FinishFireballSpell::SetAnims()
{
    CTCFireballSpell* fireball;
    if (PCreature.GetPItem()->GetInterface(&fireball, 0x9d))
        SetCombatAnimation(fireball->GetReleaseAnimName());
}