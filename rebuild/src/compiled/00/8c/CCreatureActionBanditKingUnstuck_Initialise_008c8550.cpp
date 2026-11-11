// CCreatureAction_BanditKingUnstuck::Initialise() -- clear Knockdown, animation BK_UNSTUCK, drop creature mode 0x53, close the twin-blade "stuck" hit location and reopen the twin-blade one, create the UNSTUCK special effect at the creature and attach it through its particle emitter, then the base Initialise
// Leaf: ?Initialise@CCreatureAction_BanditKingUnstuck@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C8550, 586 bytes (592 with padding), virtual Initialise, plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetCombatAnimation
//   0x008574D0, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inline interface lookups
//   0x31 CTCCreatureModeManager, 0x7F CTCHitLocations, 0x4B CTCDParticleEmitter), CTCCreatureModeManager::RemoveMode
//   0x0073ABA0, CTCHitLocations::GetPHitLocationFromName 0x00815820, CHitLocation::SetEnabled 0x00816390,
//   CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CCRC::CalcAndLookup 0x00669D30, CThing::GetPos 0x004C73D0,
//   the special-effect Create(long, const C3DVector&, bool) 0x006E0880 (__fastcall),
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0, CCombatActionBase::Initialise 0x00857520.
// Data: "BK_UNSTUCK" 0x01281AEC, "HIT_LOCATION_TWINBLADE_STUCK" 0x01281ACC, "HIT_LOCATION_TWINBLADE" 0x01281AB4,
//   "UNSTUCK" 0x01281AAC, "" 0x0122D70E.
// Layout (retail, local model): CCombatActionBase is 0x114 bytes (PCreature +0x08); Knockdown +0x114 (PDB +0x12c, the
//   engine header carries PDB offsets); CThing def +0x70, dead flag +0x91 bit 0.
// Shape: the landed BanditKingStuck::Initialise (0x008C8380) model. The mode-manager and hit-location components use the
//   out-parameter GetInterface with no null test, so the miss path reads the never-written out slot (retail reads the
//   spilled `this` word); the emitter is the `T* c = 0; if (HasTC) c = GetTC(ternary)` accessor (`xor esi,esi` before the
//   bit test); the created effect is used directly (no intelligent-pointer member); the "" bone name is the implicit
//   const CCharString& conversion, built before the other AttachToThing arguments.
#include "fable_compiler.h"
#include "engine/C3DVector.h"
class CTCBase;
class CThing;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};

class CSpecialEffectsDef { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool GetEffectFromString(const CCharString& name, long& value) const;   // 0x00669D30 // agentNF16: Ego_r/FableWin ?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail @ 0x00669D30): it writes the out value only, so VC7.1 knows the local's address
// does not escape the call (the 0x008D9740 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& value) const
{
    long found = Lookup_CSpecialEffectsDef_CalcAndLookup(this, name);
    if (found < 0)
        return false;
    value = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x008C4170 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad54[0x1c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
    CDefClassBase* GetDef() const { return m_pDef; }
    FABLE_FORCEINLINE bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        return (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
    }
    template<class T> FABLE_FORCEINLINE T* GetTCIfPresent(long interfaceType)
    {
        T* component = 0;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
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
class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
private: public:
    void AttachToThing(CThing& thing, long boneIndex, const CCharString& boneName, long a, float b);   // 0x006E0BE0
};
class CTCCreatureModeManager { public:                       // interface type 0x31
    void RemoveMode(unsigned long mode);              // 0x0073ABA0
};
struct CHitLocation {
    void SetEnabled(bool enabled);                    // 0x00816390
};
class CTCHitLocations { public:                              // interface type 0x7F
    CHitLocation* GetPHitLocationFromName(const CCharString& name);   // 0x00815820
};
class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
    virtual void Initialise();                                // 0x00857520
};
class CCreatureAction_BanditKingUnstuck : public CCombatActionBase { public:
    bool Knockdown;                                   // +0x114 (PDB +0x12c)
    bool NoTarget;                                    // +0x115
    void Initialise();
};

void CCreatureAction_BanditKingUnstuck::Initialise()
{
    Knockdown = false;
    {
        CCharString name("BK_UNSTUCK");
        SetCombatAnimation(name);
    }
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x53);
    CTCHitLocations* hitLocations;
    PCreature.GetPItem()->GetInterface(&hitLocations, 0x7f);
    CHitLocation* stuck = hitLocations->GetPHitLocationFromName("HIT_LOCATION_TWINBLADE_STUCK");
    CHitLocation* blade = hitLocations->GetPHitLocationFromName("HIT_LOCATION_TWINBLADE");
    if (stuck)
        stuck->SetEnabled(false);
    if (blade)
        blade->SetEnabled(true);
    CDefPointer<CSpecialEffectsDef> effects;
    long effect = 0;
    if (PCreature.GetPItem()->GetDef()->GetSubDef(&effects) && effects->GetEffectFromString("UNSTUCK", effect)) {
        CThing* created = CTCDParticleEmitter::Create(effect, PCreature.GetPItem()->GetPos(), false);
        if (created && !(created->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter = created->GetTCIfPresent<CTCDParticleEmitter>(0x4b);
            emitter->AttachToThing(*PCreature.GetPItem(), 0x2a, "", 0, 0);
        }
    }
    CCombatActionBase::Initialise();
}