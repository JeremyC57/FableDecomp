// CCombatAction_RecoverFromDazed::Initialise() -- resumes a player creature's suspended animation, removes mode 67 from its mode manager, then runs the base Initialise
// Leaf: ?Initialise@CCombatAction_RecoverFromDazed
// Retail: Fable.exe (TLC, VC7.1) @ 0x008ED8C0, 144-byte slot (139 bytes of code). Initialise is vtable slot 12.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times through one `lea esi,[edi+8]`),
//          CThingAnimatable::SetSuspendAnimation(bool, long) @ 0x00833A10, CVectorMap::LowerBound @ 0x0040F020 (inlined
//          GetInterface for type 0x31), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, CCombatActionBase::Initialise @ 0x00857520.
// Shape: the thing-type byte at +0x10 (1 = the player creature, as in the landed CModeKnockedDown::OnDeactivate)
// gates SetSuspendAnimation(false, 0); the GetInterface result is not tested, so RemoveMode + the base call are
// duplicated on both paths (the absent path reads the uninitialised out pointer from the `push ecx` frame word).
class CTCBase;
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
    unsigned char _pad0[0x10];
    unsigned char m_type;                   // +0x10 thing type (1 = the player creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10 (CThingAnimatable)
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
class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // slot 12: 0x00857520
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCombatAction_RecoverFromDazed : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCombatAction_RecoverFromDazed::Initialise()
{
    if (PCreature.GetPItem()->m_type == 1) {
        CThing* pCreature = PCreature.GetPItem();   // named: retail fetches it before the argument pushes
        pCreature->SetSuspendAnimation(false, 0);
    }
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetInterface(&pModes, 0x31);
    pModes->RemoveMode(67);
    CCombatActionBase::Initialise();
}