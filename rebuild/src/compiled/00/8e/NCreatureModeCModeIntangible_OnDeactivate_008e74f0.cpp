// NCreatureMode::CModeIntangible::OnDeactivate() -- the creature becomes tangible again: re-enable physics, mapwho and targeting, make it bloody-willable, and for a player creature resume its animation, restore its Damageable bit and start the queued on-deactivate action
// Leaf: ?OnDeactivate@CModeIntangible
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E74F0, 258 bytes (padded to 272), `ret`; vtable 0x01268514 slot 6.
// Name: FableWin.pdb `?OnDeactivate@CModeIntangible@NCreatureMode@@UAEXXZ` (slot 6; slot 5 OnActivate 0x008E7390, slot 3
//   FrameUpdate 0x008E7600, landed).
// Callees: the physics component's virtual slots 38 (+0x98, CTCPhysicsStandard 0x007235E0), 36 (+0x90, 0x00723630) and
//   44 (+0xB0, 0x00723A50: re-apply the def's physics flags); CThing::SetAsDrawable (slot 28, +0x70, 0x004C8E50);
//   CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, types 0x3a mapwho and 9 targeted);
//   CTCWillResponse::SetThingAsNotBloodyWillableAtAll @ 0x007E4040 (static `__fastcall`, bool in dl);
//   CThingAnimatable::SetSuspendAnimation @ 0x00833A10; CThingCreatureBase::SetCurrentAction(const CCountedPointer&) @ 0x00664670
//   (returns bool; the manifest label `_Cons_val<...>` is propagated); the action's virtual slot 4 (+0x10) when it is refused.
// Layout: Ego_r.pdb CModeIntangible (0x44): CModeBase ParentThing +4, Active +0x18; Timer +0x30, BossPhase +0x34,
//   WasDamageable +0x38, PActionOnDeactivate (CCountedPointer<CCreatureActionBase>) +0x3c. The thing (landed CModeDragonFlying /
//   CModeIntangible::FrameUpdate models): type byte +0x10, presence bitset +0x20, interface map +0x44, PhysicsTC +0x60 (PDB +0x70),
//   CThingPhysical::Damageable bit 0 of +0xBC (PDB +0xD4).
// Shape: CModeBase::OnDeactivate is the inline `Active = false` between the first two calls; the player-creature tail runs on a
//   named thing local (edi survives the calls); the bit store is a one-bit bitfield assignment (xor/and/xor).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetFlag85Bit2(bool on);        // slot 36 (+0x90), 0x00723630
    virtual void v37();
    virtual void SetPhysicsActive(bool on);     // slot 38 (+0x98), 0x007235E0
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void SetPhysicsFlags(unsigned long flags);   // slot 43 (+0xAC), 0x007234C0
    virtual void ResetPhysicsFlagsToDef();      // slot 44 (+0xB0), 0x00723A50
};
struct CModeBase;
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void OnRefused();                   // slot 4 (+0x10)
};
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetAsDrawable(bool drawable);  // slot 28 (+0x70), 0x004C8E50
    unsigned char   _pad04[0x0c];
    unsigned char   m_type;                     // +0x10 (1 = player creature)
    unsigned char   _pad11[0x0f];
    unsigned long   m_presentBits[9];           // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;               // +0x44
    unsigned char   _pad4c[0x14];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char   _pad64[0x58];
    unsigned char   Damageable : 1;             // +0xBC bit 0 (CThingPhysical)
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
    void SetSuspendAnimation(bool suspend, CModeBase* mode);                   // 0x00833A10
    bool SetCurrentAction(const CCountedPointer<CCreatureActionBase>& action); // 0x00664670
};
class CTCMapwho { public:                              // interface 0x3a
    unsigned char _pad00[0x50];
    unsigned char InMapwho : 1;                 // +0x50 bit 0
};
class CTCTargeted { public:                            // interface 9
    unsigned char _pad00[0x10];
    bool          Targetable;                   // +0x10
};
class CTCWillResponse { public:
    static void __fastcall SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable);   // 0x007E4040
};

struct CModeBase {                              // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;                  // +0x04
    unsigned char WeaponBasedFilter[0x0c];      // +0x08 CAnimFilterOfGroup
    long          FramesBeforeRemovingSelf;     // +0x14
    bool          Active;                       // +0x18
    void OnDeactivate() { Active = false; }
};

namespace NCreatureMode {
struct CModeIntangible : CModeBase {            // vtable 0x01268514
    unsigned char _pad1c[0x14];
    long          Timer;                        // +0x30
    long          BossPhase;                    // +0x34
    bool          WasDamageable;                // +0x38
    CCountedPointer<CCreatureActionBase> PActionOnDeactivate;   // +0x3c
    void OnDeactivate();
};

void CModeIntangible::OnDeactivate()
{
    ParentThing->PhysicsTC->SetPhysicsActive(true);
    CModeBase::OnDeactivate();
    ParentThing->SetAsDrawable(false);
    CTCMapwho* mapwho;
    if (ParentThing->GetInterface(&mapwho, 0x3a))
        mapwho->InMapwho = 1;
    ParentThing->PhysicsTC->SetFlag85Bit2(true);
    ParentThing->PhysicsTC->ResetPhysicsFlagsToDef();
    CTCWillResponse::SetThingAsNotBloodyWillableAtAll(ParentThing, false);
    CTCTargeted* targeted;
    if (ParentThing->GetInterface(&targeted, 9))
        targeted->Targetable = true;
    CThing* thing = ParentThing;
    if (thing->m_type == 1) {
        thing->SetSuspendAnimation(false, this);
        thing->Damageable = WasDamageable;
        if (PActionOnDeactivate.Data) {
            if (!thing->SetCurrentAction(PActionOnDeactivate))
                PActionOnDeactivate->OnRefused();
        }
    }
}
}