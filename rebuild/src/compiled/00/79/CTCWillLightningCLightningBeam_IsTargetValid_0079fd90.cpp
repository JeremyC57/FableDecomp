// CTCWillLightning::CLightningBeam::IsTargetValid() -- beam target check through the home thing's interface-8 component
// Leaf: ?IsTargetValid@CLightningBeam@@QAE_NXZ
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Grade: see STATUS.tsv
class CTCBase;
class CThing;
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}
struct CTCOwned {                               // the interface-8 component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32();
    virtual CThing* GetOwner();                 // slot 33 (+0x84)
    unsigned char _pad04[0x40 - 0x04];
    CIntelligentPointer<CThing> Owner;              // +0x40
};
struct CLightningBeam {
    long                    m_lsState;          // +0x00 ELightningState
    CIntelligentPointer<CThing> m_tpParticleHandle; // +0x04
    CIntelligentPointer<CThing> m_tcpHome;          // +0x0c
    CIntelligentPointer<CThing> m_tcpTarget;        // +0x14
    bool IsTargetValid();
};

bool CLightningBeam::IsTargetValid()
{
    if (IS_THING_ALIVE(m_tcpHome)) {
        CTCOwned* owned;
        if (m_tcpHome.Get()->GetTC(TCI_TARGETING, &owned)) {
            CThing* owner;
            if (m_tcpHome.Get()->GetType() == 1) {
                CTCOwned* creatureOwned;
                m_tcpHome.Get()->GetTC(TCI_TARGETING, &creatureOwned);
                owner = creatureOwned->Owner.Get();
                if (owner == 0)
                    owner = owned->GetOwner();
            } else {
                owner = owned->GetOwner();
            }
            if (m_tcpTarget.Get() != owner)
                return false;
        }
    }
    return true;
}