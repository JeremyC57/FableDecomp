// CAIStateGroup_JackOfBladesStage3::OnActivate() -- clear Jack's will-power flag, switch his combat component to the HERO_JACK_OF_BLADES_ATTACK_STYLE_LAST_STAND attack style, and swap his hit locations: the big and forcefield ones off, the plain one on
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00972670, length 437 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xce / 0x30 / 0x7f: bit 14 of +0x38, bit 16 of +0x24,
// bit 31 of +0x2c), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0
// ("HERO_JACK_OF_BLADES_ATTACK_STYLE_LAST_STAND" @ 0x01297668, "HIT_LOCATION_JACK_OF_BLADES_BIG" @ 0x01297604,
// "HIT_LOCATION_JACK_OF_BLADES_FORCEFIELD" @ 0x01297624, "HIT_LOCATION_JACK_OF_BLADES" @ 0x0129764C),
// CAIStateGroupBase::GetPDefinitionManager @ 0x008FCFB0, CDefinitionManager::GetDef<CCombatTypeDef> @ 0x00669770,
// CTCCombat::SetCombatType @ 0x007355F0, CTCHitLocations::GetPHitLocationFromName @ 0x00815820,
// CHitLocation::SetEnabled @ 0x00816390, the def's virtual slot 1 (release).
// Shape: the will flag is cleared inside the checked out-parameter lookup; the combat and hit-location components come from
// UNCHECKED inline accessors (the absent path reads the lookup key's frame word), the def pointer and its release as the
// landed 0x008CB620 model; each hit location is fetched into a named local (its name temporary dies first) and then set.
// Layout: CTCWillPower (interface 0xce) flag byte +0x18 (descriptive, as landed 0x009098E0's component).
#include "fable_compiler.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* GetTCIfPresent(long interfaceType)
    {
        T* component;
        GetInterface(&component, interfaceType);
        return component;
    }
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CRefCounted {                            // second base of the def: the count at +4
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
class CCombatTypeDef : public CDefVirtual, public CRefCounted { public:   // Ego_r.pdb (was the row-local CAttackStyleDef)
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template <class T> class CDefPointer { public:
    typename CDefNonConst<T>::Type* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};
class CDefinitionManager { public:
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* def) const;   // 0x00669770 GetDef<CCombatTypeDef> (Ego_r.pdb)
};
struct CTCWillPower {                           // interface 0xce
    unsigned char _pad0[0x18];
    bool Active;                                // +0x18 (descriptive)
};
class CTCCombat { public:                              // interface 0x30
    void SetCombatType(const CDefPointer<const CCombatTypeDef>& def);   // 0x007355F0 (Ego_r.pdb)
};
struct CHitLocation {
    void SetEnabled(bool enabled);              // 0x00816390
};
class CTCHitLocations { public:                        // interface 0x7f
    CHitLocation* GetPHitLocationFromName(const CCharString& name);   // 0x00815820
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    CThing* GetCreature() const { return PBrain->PCreature; }
    CDefinitionManager* GetPDefinitionManager();   // 0x008FCFB0
};
struct CAIStateGroup_JackOfBladesStage3 : CAIStateGroupBase {
    virtual void OnActivate();
};

void CAIStateGroup_JackOfBladesStage3::OnActivate()
{
    CTCWillPower* will;
    if (GetCreature()->GetInterface(&will, 0xce))
        will->Active = false;
    CTCCombat* combat = GetCreature()->GetTCIfPresent<CTCCombat>(0x30);
    CDefPointer<const CCombatTypeDef> def;
    {
        CCharString name("HERO_JACK_OF_BLADES_ATTACK_STYLE_LAST_STAND");
        GetPDefinitionManager()->GetDef(name, &def);
    }
    combat->SetCombatType(def);
    CTCHitLocations* hitLocations = GetCreature()->GetTCIfPresent<CTCHitLocations>(0x7f);
    CHitLocation* big = hitLocations->GetPHitLocationFromName("HIT_LOCATION_JACK_OF_BLADES_BIG");
    big->SetEnabled(false);
    CHitLocation* forcefield = hitLocations->GetPHitLocationFromName("HIT_LOCATION_JACK_OF_BLADES_FORCEFIELD");
    forcefield->SetEnabled(false);
    CHitLocation* body = hitLocations->GetPHitLocationFromName("HIT_LOCATION_JACK_OF_BLADES");
    body->SetEnabled(true);
}