// CCreatureAction_SummonCreatures::Initialise() -- play SUMMON_CREATURES, run the combat-action Initialise, spawn the summon def's effect at the summoner, then have its AI-summon component summon the creatures at the live target (SpawnOnTarget) or at the default spot
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7A20, length 240 bytes (plain `ret`, virtual).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetCombatAnimation(const
// CCharString&) 0x008574D0, CCombatActionBase::Initialise 0x00857520 (direct base call), CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inline GetInterface 0xFA = CTCAISummon, bit 26 of the present-bits dword
// +0x3C), CThing::GetPos 0x004C73D0, __fastcall CreateEffectThing(long, const C3DVector*, bool) 0x006E0880,
// CTCAISummon::SummonCreatures(const C3DVector*) 0x007FE040 (result dropped).
// Layout: CTCAISummon PSummonDef +0x0C (PDB); the CSummonDef effect read is retail +0x44 = PDB SummoneeEffect +0x48 (def
// layouts sit 4 bytes lower in retail; if the def base matches the PDB it is SummonEffect); SpawnOnTarget retail +0x114
// (PDB +0x12C); target CIntelligentPointer +0xA8.
// Shape: the summon component pointer is zero-initialised and filled by the unchecked out-parameter GetInterface (the
// absent path dereferences null, as retail does); the two SummonCreatures calls are not merged.
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
#include "engine/C3DVector.h"
class CTCBase;

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_deleteFlags;                // +0x91 bit 0: being deleted
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->end() || interfaceType < it->first ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};

class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
};

class CSummonDef { public:
    unsigned char _pad0[0x44];
    long          SummoneeEffect;               // +0x44
};

class CTCAISummon { public:
    unsigned char _pad0[0x0c];
    CSummonDef*   PSummonDef;                   // +0x0c CDefPointer<const CSummonDef>
    long SummonCreatures(const C3DVector* position);   // 0x007FE040
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

class CCombatActionBase { public:
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_SummonCreatures : public CCombatActionBase { public:
    bool SpawnOnTarget;                         // +0x114
    virtual void Initialise();
};

void CCreatureAction_SummonCreatures::Initialise()
{
    {
        CCharString name("SUMMON_CREATURES", -1);
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCAISummon* summon = 0;
    PCreature.GetPItem()->GetInterface(&summon, 0xfa);
    long effect = summon->PSummonDef->SummoneeEffect;
    if (effect > 0)
        CTCDParticleEmitter::Create(effect, *(&PCreature.GetPItem()->GetPos()), false);
    if (SpawnOnTarget && PTarget.GetPItem() && !(PTarget.GetPItem()->m_deleteFlags & 1))
        summon->SummonCreatures(&PTarget.GetPItem()->GetPos());
    else
        summon->SummonCreatures(0);
}