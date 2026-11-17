// CTCBerserk::CreateStartingEffect() -- look up the berserk def's "STARTUP_EFFECT" particle, create it at the thing's position, keep it as StartUpEffect and attach its emitter to the thing
// Leaf: CreateStartingEffect
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail 0x0078C4C0, 336 bytes, `ret`.
// Callees: CDefClassBase::GetSubDef<T>(CDefPointer<T>*) 0x00669580, the sub-def's named-effect lookup (const CCharString&,
// long*) 0x00669D30, CCharString(const char*, long) 0x0099EBF0 ("STARTUP_EFFECT" 0x0126BC64, "" 0x0122D70E), ~CCharString
// 0x0099EAE0, CThing::GetPos 0x004C73D0, CreateParticleEffect(long, const C3DVector&) 0x0072AB70 (`__fastcall`),
// CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10 (vtable 0x01238C6C),
// CVectorMap::LowerBound 0x0040F020 (interface 0x4B), CTCDParticleEmitter::AttachToThing 0x006E0BE0; the def's slot 1 release.
// Layout: engine/CTCBerserk.h is the PDB layout with a 16-byte DependentThings vector; retail sits 4 lower after it
// (StartUpEffect +0x58, PDef +0x68), so a local model is used; ParentThing +4.
// Advisor fix (Fable 5.1): the GetEffect stand-in. Shape: same-TU auto_inline(off) stand-ins for GetSubDef AND GetEffect (with GetEffect only declared, the &effect out-param
// counts as escaped and its slot stays live, pushing the key/""/emitter homes into the dead def slot); the out CDefPointer and
// the effect id are zeroed together at entry; the sub-def is a CDefPointer with an inline releasing dtor that VC7.1 keeps in
// ebp thanks to the GetSubDef stand-in (`dec [ebp+4] ; jne`); found and effect lookups are nested ifs; the effect name is a temporary in the dead out
// slot; StartUpEffect is assigned from an intelligent-pointer temporary; the emitter is the out-parameter GetInterface.
#include "fable_compiler.h"
// NF49 (round 7): the def classes come from the shared headers (were local CDefHolder / CBerserkEffectsDef / CCharString / CDefPointeeBase / CDefPointer / CDefClassBase models with same-TU stand-in bodies over made-up FindSubDef / FindEffect callees): the call at
// 0x00669580 is the member template ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z
// (Ego_r.pdb / FableWin.pdb), its body visible and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10
// idiom), and CSpecialEffectsDef::GetEffectFromString 0x00669D30 is the header's same-TU noinline body
// (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY, the 0x007941D0 idiom), so the caller's handles stay uncaptured as with the stand-ins.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
class CTCBase;
class CBaseObject;
#include "engine/C3DVector.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    const C3DVector& GetPos() const;               // 0x004C73D0
    bool IsDead() const { return (m_flags & 1) != 0; }
    template <class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
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
};
// NF49 (round 7): CreateParticleEffect 0x0072AB70 is the public static __fastcall
// ?CreateParticleEffect@CTCSpecialAbilities@@SIPAVCThing@@JABVC3DVector@@@Z (Ego_r.pdb / FableWin.pdb), declared by
// engine/CTCSpecialAbilities.h; was a free stand-in.
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#include "engine/CTCSpecialAbilities.h"
class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = ((CThing*)other.GetPItem());
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}
class CTCBerserk {
public:
    void*                        __vftable;             // +0x00
    CThing*                      ParentThing;           // +0x04
    unsigned char                _pad08[0x58 - 0x08];
    CIntelligentPointer<CThing>  StartUpEffect;         // +0x58
    CIntelligentPointer<CThing>  BerserkBodyEffect;     // +0x60
    CDefClassBase*               PDef;                  // +0x68 (CDefPointer Object)
    void CreateStartingEffect();
};

void CTCBerserk::CreateStartingEffect()
{
    CDefPointer<const CSpecialEffectsDef> def;
    long effect = 0;
    if (PDef->GetSubDef(&def)) {
        if (def->GetEffectFromString("STARTUP_EFFECT", effect)) {
            StartUpEffect = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(effect, ParentThing->GetPos()));
            if (IS_THING_ALIVE(StartUpEffect)) {
                CTCDParticleEmitter* emitter;
                ((CThing*)StartUpEffect.GetPItem())->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(*ParentThing, 0x28, "", 0, 0.0f);
            }
        }
    }
}