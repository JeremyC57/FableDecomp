// CTCMultiStrike::RemoveEffectOnWeapon(CThing* weapon) -- when the weapon is alive, kill every live particle effect this multi-strike attached
// Leaf: ?RemoveEffectOnWeapon@CTCMultiStrike@@QAEXPAVCThing@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CC9E0, length 128 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three per element), CThing::Kill(bool) @ 0x004C9B80 (true).
// Layout: PDB CTCMultiStrike (0x5C): ParticleEffects +0x4C (vector<CIntelligentPointer<CThing> >, 8-byte elements; PDB offset).
// Shape: `for (long i = 0; i < (long)size(); ++i)` signed loop; IS_THING_ALIVE on the element reference, then the element re-indexed
// through STLport `*(begin() + i)` for the Kill (retail re-reads `_M_start`).
class CTCBase;
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

template<class T> struct CRetailVector {        // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}
class CTCMultiStrike { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x4c - 0x08];
    CRetailVector<CIntelligentPointer<CThing> > ParticleEffects;   // +0x4c
    void RemoveEffectOnWeapon(CThing* weapon);
};

void CTCMultiStrike::RemoveEffectOnWeapon(CThing* weapon)
{
    if (weapon && !weapon->IsDead()) {
        for (long i = 0; i < (long)ParticleEffects.size(); ++i) {
            if (IS_THING_ALIVE(ParticleEffects[i]))
                ParticleEffects[i].Get()->Kill(true);
        }
    }
}