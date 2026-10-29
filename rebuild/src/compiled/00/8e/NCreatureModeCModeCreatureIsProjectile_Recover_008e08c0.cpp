// NCreatureMode::CModeCreatureIsProjectile::Recover() -- the thrown creature lands: resume its animation and physics if it was in state 1, make it play the knockdown-recover combat action, then remove this projectile mode (0x39)
// Leaf: Recover
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E08C0, 203 bytes (`ret`).
// Callees: CThingAnimatable::SetSuspendAnimation(bool, long) 0x00833A10; the creature's physics object (+0x60) virtual slots 40
// (+0xa0, enable) and 44 (+0xb0, wake); CCombatAction_GenericStrikeResponseKnockdownRecoverBase(CThingCreatureBase*, bool)
// 0x00867AF0 (then the derived CCombatAction_GenericStrikeResponseKnockdownRecover vtable 0x0127191C from its inline ctor);
// CThingCreatureBase::SetCurrentAction(const CCombatActionBase&) 0x006644F0; ~CCombatActionBase 0x0062E900;
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface, CTCCreatureModeManager = type 0x31,
// bit 17 of +0x24); CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0.
// Layout (PDB CModeCreatureIsProjectile 0x88, matches retail): CModeStandBase 0x68, StartThrow +0x68, Filter +0x74,
// OldTargetFlags +0x80, PCreature +0x84. Creature: state byte +0x10, physics object +0x60. The recover action is a 0x118-byte
// frame temporary (retail CCombatActionBase is 0x114 bytes; CanBeHit/Flip follow).
// Shape: the action is an unnamed temporary passed by reference (built, used, destroyed by the implicit derived dtor = the
// out-of-line base dtor); VC duplicates the RemoveMode tail for the GetInterface miss arm.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                     // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);  // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CPhysicsObject {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void SetEnabled(bool enabled);   // slot 40 (+0xa0)
    virtual void v041(); virtual void v042(); virtual void v043();
    virtual void Wake();   // slot 44 (+0xb0)
};

class CCombatActionBase;

struct CThingAnimatable {                  // empty here: only its member is called (thing base at offset 0)
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};

class CThingCreatureBase : public CThingAnimatable { public:
    unsigned char   _pad00[0x10];
    unsigned char   m_state;               // +0x10 creature state byte (compared with 1)
    unsigned char   _pad11[0x0f];
    unsigned long   m_presentBits[9];      // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;          // +0x44
    unsigned char   _pad4c[0x14];
    CPhysicsObject* PPhysics;              // +0x60
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
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

class CTCCreatureModeManager { public:            // interface type 0x31
    void RemoveMode(unsigned long mode);   // 0x0073ABA0
};

class CCombatActionBase { public:                 // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x110];
    virtual ~CCombatActionBase();                  // 0x0062E900
};

class CCombatAction_GenericStrikeResponseKnockdownRecoverBase : public CCombatActionBase { public:   // vtable 0x01271BAC
    bool CanBeHit;                         // +0x114
    bool Flip;                             // +0x115
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase(CThingCreatureBase* owner, bool flip);   // 0x00867AF0
};

class CCombatAction_GenericStrikeResponseKnockdownRecover : public CCombatAction_GenericStrikeResponseKnockdownRecoverBase { public:   // vtable 0x0127191C
    virtual void v00();
    CCombatAction_GenericStrikeResponseKnockdownRecover(CThingCreatureBase* owner, bool flip)
        : CCombatAction_GenericStrikeResponseKnockdownRecoverBase(owner, flip)
    {
    }
};

class C3DVector { public: float X, Y, Z; };

class CAnimFilterOfGroup { public:                // vtable 0x01267568
    virtual void v00();
    void*         Group;                   // +4 (CCharString)
    unsigned long GroupCRC;                // +8
};

struct CModeStandBase {                    // retail sizeof 0x68
    virtual void v00();
    unsigned char _body[0x64];
};

namespace NCreatureMode {

struct CModeCreatureIsProjectile : CModeStandBase {
    C3DVector           StartThrow;        // +0x68
    CAnimFilterOfGroup  Filter;            // +0x74
    unsigned long       OldTargetFlags;    // +0x80
    CThingCreatureBase* PCreature;         // +0x84
    void Recover();
};

void CModeCreatureIsProjectile::Recover()
{
    CThingCreatureBase* creature = PCreature;
    if (creature->m_state == 1) {
        creature->SetSuspendAnimation(false, 0);
        creature->PPhysics->SetEnabled(true);
        creature->PPhysics->Wake();
    }
    PCreature->SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdownRecover(PCreature, false));
    CTCCreatureModeManager* modes;
    PCreature->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x39);
}

}