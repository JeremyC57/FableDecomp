// CCombatAction_RecoverFromStunned::Initialise() -- resumes a player creature's suspended animation, resets its physics component (slot 44), removes mode 29 from its mode manager, then runs the knockdown-recover base Initialise
// Leaf: ?Initialise@CCombatAction_RecoverFromStunned
// Retail: Fable.exe (TLC, VC7.1) @ 0x00866CE0, 160-byte slot (157 bytes of code). Initialise is vtable slot 12.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, four times through one `lea esi,[edi+8]`),
//          CThingAnimatable::SetSuspendAnimation(bool, long) @ 0x00833A10, the physics component's virtual slot 44
//          (`call [eax+0B0h]`, no arguments; CTCPhysicsBase @ 0x006B1120 is empty, CTCPhysicsStandard @ 0x00723A50
//          forwards to its slot 43), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x31),
//          CTCCreatureModeManager::RemoveMode @ 0x0073ABA0,
//          CCombatAction_GenericStrikeResponseKnockdownRecoverBase::Initialise @ 0x00865EB0 (direct base call).
// Shape: as RecoverFromDazed (0x008ED8C0): the creature fetched for SetSuspendAnimation is a named local (retail
// calls GetPItem before the pushes); the physics component pointer is read through an inline accessor
// (`mov ecx,[eax+60h]` straight into ecx; the bare member goes through eax) of the thing's member at +0x60 (PhysicsTC,
// PDB +0x70; the landed JackFloatUp::Initialise reads its position through the same member); the GetInterface
// result is not tested, so RemoveMode + the base call are duplicated on both paths.
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
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43();
    virtual void ResetMovement();           // slot 44 (unnamed in the manifest: CTCPhysicsBase::vfunc_44)
};
class CThing { public:
    unsigned char _pad0[0x10];
    unsigned char m_type;                   // +0x10 thing type (1 = the player creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;              // +0x60 (PDB +0x70)
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }   // inline accessor: loads straight into ecx
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

class CCombatAction_GenericStrikeResponseKnockdownRecoverBase : public CCombatActionBase { public:
    virtual void Initialise();              // 0x00865EB0
};

class CCombatAction_RecoverFromStunned : public CCombatAction_GenericStrikeResponseKnockdownRecoverBase { public:
    virtual void Initialise();
};

void CCombatAction_RecoverFromStunned::Initialise()
{
    if (PCreature.GetPItem()->m_type == 1) {
        CThing* pCreature = PCreature.GetPItem();   // named: retail fetches it before the argument pushes
        pCreature->SetSuspendAnimation(false, 0);
    }
    PCreature.GetPItem()->GetPhysicsTC()->ResetMovement();
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetInterface(&pModes, 0x31);
    pModes->RemoveMode(29);
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase::Initialise();
}