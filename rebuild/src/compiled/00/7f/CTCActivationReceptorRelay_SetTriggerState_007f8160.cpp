// CTCActivationReceptorRelay::SetTriggerState(bool state) -- pass the relay's activation state on to the activation-trigger component of its own thing (OnTriggerActivate)
// Leaf: ?SetTriggerState@CTCActivationReceptorRelay@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F8160, 88 bytes (`ret 4` + int3 padding). FableWin.pdb / Ego_r.pdb
//   `?SetTriggerState@CTCActivationReceptorRelay@@AAEX_N@Z` (private thiscall; modelled public, hence the QAE leaf); callers CTCActivationReceptorRelay::Activate
//   0x007F81C0 (true) and ::Deactivate 0x007F81E0 (false), vtable 0x012414EC slots 28/29. Catalog label
//   `TCInterface_HandleEndCutFade` was a guess (the manifest's "CGameScriptInterface::EndCutFade" on the callers is propagated).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (non-const) @ 0x0040F020 (interface 0x66 = CTCActivationTrigger,
//   presence word 3 bit 6), CTCActivationTrigger::OnTriggerActivate(bool) = slot 28 (+0x70) (vtable 0x0123BBAC slot 28
//   0x0077B060).
// Shape: the PDB's out-parameter `CThing::GetTC<CTCActivationTrigger>(&trigger)` with its result ignored, then the virtual
//   call; a thing without the component calls through the never-written local (homed in the argument slot).
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;

template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const ETCInterfaceType& kind);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (HasTC(T::INTERFACE_TYPE)) {
            CTCMap* map = &m_tcs;
            STCEntry* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
            if (it == map->m_pLast || T::INTERFACE_TYPE < it->first)
                it = map->m_pLast;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
class CTCActivationTrigger {
public:
    enum { INTERFACE_TYPE = 0x66 };
    VF10(0) VF10(1) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27)
    virtual void OnTriggerActivate(bool activate);              // slot 28 (+0x70)
};

class CTCActivationReceptorRelay {
public:
    virtual void v00();
    CThing* ParentThing;                                        // +0x04 (CTCBase)
    void SetTriggerState(bool state);
};

void CTCActivationReceptorRelay::SetTriggerState(bool state)
{
    CTCActivationTrigger* trigger;
    ParentThing->GetTC(&trigger);
    trigger->OnTriggerActivate(state);
}