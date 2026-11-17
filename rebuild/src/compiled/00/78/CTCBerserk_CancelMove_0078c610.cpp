// CTCBerserk::CancelMove() -- cancel the berserk move: when the creature's berserk state component is in state 8 and the ability is not initialised, reset it, kill the start-up effect, spawn the def's "BERSERK_EXIT_EFFECT" and remove TC 0x84; otherwise rescale or clamp the timer
// Leaf: CancelMove
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail 0x0078C610, 368 bytes, `ret`. Same TU as CreateStartingEffect 0x0078C4C0 (shared model).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (interface 0xA7), CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CThing::Kill(bool) 0x004C9B80, CDefClassBase::GetSubDef<T> 0x00669580, the effects def's named lookup
// 0x00669D30, CCharString(const char*, long) 0x0099EBF0 ("BERSERK_EXIT_EFFECT" 0x0126BC74), ~CCharString 0x0099EAE0,
// CThing::GetPos 0x004C73D0, CreateParticleEffect 0x0072AB70 (`__fastcall`), CThing::RemoveTC(long) 0x004C9840,
// __ftol2 0x00BFEA70; the def's slot 1 release.
// Layout: retail CTCBerserk (local model, see CreateStartingEffect): Timer +0x0C, ReleasedAnimationTimer +0x1C (float),
// AnimTimer +0x20, AbilityInitialised +0x49, StartUpEffect +0x58, PDef +0x68 (max timer +0x40); the 0xA7 component's
// active flag +0x0C and state +0x10.
// Shared-TU model with CreateStartingEffect 0x0078C4C0 (same GetSubDef/GetEffect stand-ins). Shape: HasTC(0xA7) is a sign test of the bitset byte; the exit-effect test is `GetSubDef(&def) && def->GetEffect("...",
// &effect)` with a conditional CCharString temporary (flag zeroed at entry); the def is a CDefPointer released by its inline
// dtor (register-held thanks to the same-TU GetSubDef stand-in) and the berserk def is a named local read before it (so
// `mov ecx,[esi+68h]` precedes the out address); IS_THING_ALIVE must be `__forceinline` here; the anim timer is converted
// once into a float local and the rescale is written `max * (t / released)` (VC7.1 keeps `fdiv ; fimul`; `t / released *
// max` is reassociated into `fild max ; fdiv ; fmul`).
#include "fable_compiler.h"
// NF49 (round 7): the def classes come from the shared headers (were local CDefHolder / CBerserkEffectsDef / CDefClassBase / CCharString / CDefPointeeBase / CDefPointer models with same-TU stand-in bodies over made-up FindSubDef / FindEffect callees): the call at
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
    void Kill(bool immediately);                        // 0x004C9B80
    void RemoveTC(ETCInterfaceType type);                           // 0x004C9840
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> FABLE_FORCEINLINE T* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
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
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
FABLE_FORCEINLINE bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CBerserkDef : CDefClassBase {                    // NF49: now derives from the shared CDefClassBase (was its local base)
    unsigned char _pad25[0x40 - 0x25];
    long          MaxTimer;                             // +0x40
};
struct CTCBerserkState {
    unsigned char _pad00[0x0c];
    bool          Active;                               // +0x0c
    long          State;                                // +0x10
};
class CTCBerserk {
public:
    void*                        __vftable;             // +0x00
    CThing*                      ParentThing;           // +0x04
    unsigned char                _pad08[0x0c - 0x08];
    long                         Timer;                 // +0x0c
    long                         DarkenEnvTimer;        // +0x10
    float                        StrengthPercentage;    // +0x14
    float                        MaxIncreaseInStrength; // +0x18
    float                        ReleasedAnimationTimer; // +0x1c
    long                         AnimTimer;             // +0x20
    unsigned char                _pad24[0x48 - 0x24];
    bool                         JustReleased;          // +0x48
    bool                         AbilityInitialised;    // +0x49
    unsigned char                _pad4a[0x58 - 0x4a];
    CIntelligentPointer<CThing>  StartUpEffect;         // +0x58
    CIntelligentPointer<CThing>  BerserkBodyEffect;     // +0x60
    CBerserkDef*                 PDef;                  // +0x68 (CDefPointer Object)
    void CancelMove();
};

void CTCBerserk::CancelMove()
{
    CThing* thing = ParentThing;
    if (!thing->HasTC(0xa7))
        return;
    CTCBerserkState* state = thing->GetTC<CTCBerserkState>(0xa7);
    if (!AbilityInitialised && state->State == 8) {
        state->Active = false;
        state->State = 0;
        if (IS_THING_ALIVE(StartUpEffect))
            StartUpEffect.GetPItem()->Kill(true);
        CDefClassBase* berserkDef = PDef;
        CDefPointer<const CSpecialEffectsDef> def;
        long effect = 0;
        if (berserkDef->GetSubDef(&def) && def->GetEffectFromString("BERSERK_EXIT_EFFECT", effect))
            CTCSpecialAbilities::CreateParticleEffect(effect, thing->GetPos());
        thing->RemoveTC((ETCInterfaceType)(0x84));
    } else {
        float animTimer = (float)AnimTimer;
        if (animTimer < ReleasedAnimationTimer)
            Timer = (long)(PDef->MaxTimer * (animTimer / ReleasedAnimationTimer));
        else if (Timer > PDef->MaxTimer)
            Timer = PDef->MaxTimer;
    }
}