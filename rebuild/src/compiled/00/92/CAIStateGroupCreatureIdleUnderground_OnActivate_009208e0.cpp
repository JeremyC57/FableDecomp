// CAIStateGroup_CreatureIdleUnderground::OnActivate() -- unless the creature is already in the underground mode (type 0x1e), make it go underground
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009208E0, length 124 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x31 =
// CTCCreatureModeManager: bitset word 1 bit 17 -> `test dword ptr [eax+24h],20000h`), the mode manager's current-mode
// getter @ 0x0073A6E0 (the manifest's CPixelShader::CountReferences label is propagated), the mode's virtual slot 1
// (+0x04, its type), CCreatureAction_GoUnderground::CCreatureAction_GoUnderground(CThingCreatureBase*) @ 0x008B8110,
// CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0, the action base destructor @ 0x0062E900
// (the manifest's ~CTCLightningOrb label is propagated).
// Shape: the landed GetInterface out-parameter template inside the `if`; the action is a TEMPORARY inside the
// call (the landed 0x00933BB0 shape): the creature read into a named local (esi) is both the constructor
// argument and the `this` of SetCurrentAction, the constructor's returned `this` is pushed, the base destructor
// runs after the call. Frame 0x118 = the lookup key (+0) and the 0x114-byte retail action (+4; the PDB's
// 300-byte layout is wider).
// Model: local (no engine header for this class).
class CTCBase;
class CThingCreatureBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

namespace N_0062e900 { struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x110];                 // +0x04..+0x113
    ~CCreatureActionBase();                     // 0x0062E900
}; } using N_0062e900::CCreatureActionBase;

class CCreatureAction_GoUnderground : public CCreatureActionBase { public:
    CCreatureAction_GoUnderground(CThingCreatureBase* creature);   // 0x008B8110
};

class CThingAICreature { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    bool SetCurrentAction(CCreatureActionBase* action);            // 0x006644F0 (CThingCreatureBase)
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

struct CCreatureModeBase {
    virtual void v00();
    virtual long GetType();                     // slot 1 (+0x04)
};

class CTCCreatureModeManager { public:                 // interface 0x31
    CCreatureModeBase* GetCurrentMode();        // 0x0073A6E0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_CreatureIdleUnderground : CAIStateGroupBase {
    virtual void OnActivate();
};

void CAIStateGroup_CreatureIdleUnderground::OnActivate()
{
    CTCCreatureModeManager* modes;
    if (PBrain->Creature->GetInterface(&modes, 0x31)) {
        if (modes->GetCurrentMode()->GetType() != 0x1e) {
            CThingAICreature* creature = PBrain->Creature;
            creature->SetCurrentAction(&CCreatureAction_GoUnderground((CThingCreatureBase*)creature));
        }
    }
}