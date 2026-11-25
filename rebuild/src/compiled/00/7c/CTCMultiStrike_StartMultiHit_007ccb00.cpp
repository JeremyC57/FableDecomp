// CTCMultiStrike::StartMultiHit(const CHitParameters& hit, CThing* target) -- remember the target, the hit location, weapon, point and direction of the hit, start the strike (first hit, delay from the def's per-level DelayTimeSecs), spawn the def's HIT_EFFECT at the target, clear the weapon's strike effects, rumble the main player's joystick and feed the experience multi-hit combo
// Leaf: StartMultiHit
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CCB00, 619 bytes, `ret 8`.
// Callees: CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), __ftol2 @ 0x00BFEA70, GetSubDef<CSpecialEffectsDef> @ 0x00669580,
//   CalcAndLookup @ 0x00669D30, CCharString @ 0x0099EBF0/0x0099EAE0, CThing::GetPos @ 0x004C73D0, CreateParticleEffect
//   @ 0x0072AB70 (__fastcall), const LowerBound @ 0x004365B0 (0x13) / LowerBound @ 0x0040F020 (0x68), CTCMultiStrike::
//   RemoveEffectOnWeapon @ 0x007CC9E0, CThing::GetPlayer @ 0x004C7A30, CPlayer::IsMainPlayer @ 0x00487520,
//   GFGetSystemManager @ 0x009A4EC0, CThing::GetJoystickDeviceNumber @ 0x004C7CA0, the joystick's Rumble (slot 17),
//   the interface-0x68 component's ProcessMultiHitCombo @ 0x006D8830; the effects def's slot 1.
// Data: "HIT_EFFECT" 0x0126EE7C, frames per second 0x01375550.
// Layout: CTCMultiStrike as Ego_r.pdb (landed FrameUpdate 0x007CCE20); retail CHitParameters is 8 bytes below Ego_r after
//   its 12-byte map/vector: Position +0x02, Direction +0x0E, PHitLocation +0x48, PWeapon +0x58; the multi-strike def's
//   DelayTimeSecs vector at +0x6C (retail).
#include "fable_compiler.h"
// NF49 (round 7): the def classes come from the shared headers (were local CCharString / CDefPointeeBase / CDefPointer / CSpecialEffectsDef / CDefClassBase models with same-TU stand-in bodies over made-up Lookup_CSpecialEffectsDef_CalcAndLookup / FindDef_CDefClassBase_GetSubDef_0 callees): the call at
// 0x00669580 is the member template ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z
// (Ego_r.pdb / FableWin.pdb), its body visible and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10
// idiom), and CSpecialEffectsDef::GetEffectFromString 0x00669D30 is the header's same-TU noinline body
// (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY, the 0x007941D0 idiom), so the caller's handles stay uncaptured as with the stand-ins.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
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
#include "engine/CSpecialAbilitiesMultiStrikeDef.h"   // NF49: the shared def class (was a local model over a local CStdVector)
class CTCBase;
class CThing;
class CBaseObject;

#include "engine/C3DVector.h"

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);              // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;        // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                   // +0x04
    CBaseIntelligentPointer();                         // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                        // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                  // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem((CBaseObject*)other.GetPItem());
        return *this;
    }
};
#pragma pack(pop)

class CPlayer {
public:
    bool IsMainPlayer() const;                         // 0x00487520
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    FABLE_FORCEINLINE bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    FABLE_FORCEINLINE CTCBase* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
    FABLE_FORCEINLINE CTCBase* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;              // 0x004C73D0
    CPlayer& GetPlayer();                        // 0x004C7A30
    long GetJoystickDeviceNumber() const;              // 0x004C7CA0
};
// NF49 (round 7): CreateParticleEffect 0x0072AB70 is the public static __fastcall
// ?CreateParticleEffect@CTCSpecialAbilities@@SIPAVCThing@@JABVC3DVector@@@Z (Ego_r.pdb / FableWin.pdb), declared by
// engine/CTCSpecialAbilities.h; was a free stand-in.
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#include "engine/CTCSpecialAbilities.h"

namespace NHitLocations { class CHitLocation; }
#pragma pack(push, 1)
class CHitParameters {                                 // retail layout (Ego_r minus 8 after the 12-byte containers)
public:
    bool                         Localised;            // +0x00
    bool                         Directional;          // +0x01
    C3DVector                    Position;             // +0x02
    C3DVector                    Direction;            // +0x0e
    unsigned char                _pad1a[0x48 - 0x1a];
    NHitLocations::CHitLocation* PHitLocation;         // +0x48
    unsigned char                _pad4c[0x58 - 0x4c];
    CIntelligentPointer<CThing>  PWeapon;              // +0x58
};
#pragma pack(pop)

struct CRumbleParams {                                 // 20 bytes
    float Strength;
    float Duration;
    float Delay;
    float Falloff;
    bool  Looping;
    CRumbleParams(float strength, float duration, float delay, float falloff, bool looping)
        : Strength(strength), Duration(duration), Delay(delay), Falloff(falloff), Looping(looping) { }
};
struct CJoystickDevice {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual void Rumble(long type, const CRumbleParams& params, long flag);   // slot 17 (+0x44)
};
struct CJoystickEntry {                                // 8 bytes
    CJoystickDevice* Device;
    long             Id;
};
class CInputManager { public:
    unsigned char   _pad00[0x2c];
    CJoystickEntry* Joysticks;                         // +0x2c (vector begin)
    CJoystickDevice* GetJoystick(long n) const { return Joysticks[n].Device; }
};
class CSystemManager { public:
    unsigned char  _pad00[0x58];
    CInputManager* InputManager;                       // +0x58
};
CSystemManager* __fastcall GFGetSystemManager();       // 0x009A4EC0

#pragma pack(push, 1)
struct CTCWeaponHolder {                               // interface 0x13 (descriptive)
    unsigned char _pad00[0x134];
    CBaseIntelligentPointer PWeapon;                   // +0x134
};
#pragma pack(pop)
struct CTCExperienceCombo {                            // interface 0x68 (descriptive)
};

class CTCMultiStrike {
public:
    void*                        __vftable;            // +0x00
    CThing*                      ParentThing;          // +0x04 (CTCBase)
    unsigned char                _pad08[0x0c - 0x08];
    CDefPointer<const CSpecialAbilitiesMultiStrikeDef> PDef;   // +0x0c
    long                         SpellLevel;           // +0x10
    bool                         StrikeStarted;        // +0x14
    long                         DelayTimer;           // +0x18
    long                         NumHits;              // +0x1c
    NHitLocations::CHitLocation* PHitLocation;         // +0x20
    CIntelligentPointer<CThing>  PWeapon;              // +0x24
    C3DVector                    PointOfHit;           // +0x2c
    C3DVector                    HitDirection;         // +0x38
    CIntelligentPointer<CThing>  PTarget;              // +0x44
    void RemoveEffectOnWeapon(CThing* weapon);         // 0x007CC9E0
    void StartMultiHit(const CHitParameters& hit, CThing* target);
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x006D8830 ?AddSuccessfulMultistrikeHit@CTCHeroExperience@@QAEXABVCHitParameters@@@Z (was ?ProcessMultiHitCombo@CTCExperienceCombo@@QAEXABVCHitParameters@@@Z)
class CTCHeroExperience { public: void AddSuccessfulMultistrikeHit(CHitParameters const&); };
void CTCMultiStrike::StartMultiHit(const CHitParameters& hit, CThing* target)
{
    PTarget = CIntelligentPointer<CThing>(target);
    PHitLocation = hit.PHitLocation;
    PWeapon = CIntelligentPointer<CThing>(((CThing*)hit.PWeapon.GetPItem()));
    PointOfHit = hit.Position;
    HitDirection = hit.Direction;
    StrikeStarted = true;
    NumHits = 1;
    DelayTimer = (long)(CMainGameComponent::GetConstantFPS() * PDef->DelayTimeSecs[SpellLevel]);
    CDefPointer<const CSpecialEffectsDef> effects;
    long effect = 0;
    bool found = PDef->GetSubDef(&effects) && effects->GetEffectFromString("HIT_EFFECT", effect);
    if (found)
        CTCSpecialAbilities::CreateParticleEffect(effect, target->GetPos());
    const CThing* parentThing = ParentThing;
    if (parentThing->HasTC(0x13)) {
        CThing* weapon =((CThing*) ((CTCWeaponHolder*)parentThing->GetTC(0x13))->PWeapon.GetPItem());
        if (weapon)
            RemoveEffectOnWeapon(weapon);
    }
    if (ParentThing->GetPlayer().IsMainPlayer()) {
        CThing* parent = ParentThing;
        CInputManager* input = GFGetSystemManager()->InputManager;
        input->GetJoystick(parent->GetJoystickDeviceNumber())->Rumble(4, CRumbleParams(1.0f, 0.5f, 0.0f, 0.4f, true), 0);
    }
    CTCExperienceCombo* combo;
    if (ParentThing->GetInterface(&combo, 0x68))
        reinterpret_cast<CTCHeroExperience*>(combo)->AddSuccessfulMultistrikeHit(hit);
}