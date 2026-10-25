// `anonymous namespace'::CEventFilter_HitEventWithWeapon::operator()(NEntityEvents::CEventHitBase* event) const -- true when the hit was dealt with a live weapon whose definition index is the filter's WeaponDefIndex
// Leaf: ??RCEventFilter_HitEventWithWeapon@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D43C0, length 48 bytes, `ret 4`.
// Naming: Ego_d.pdb ??RCEventFilter_HitEventWithWeapon@?A0x70a0c557@@QBE_NPAVCEventHitBase@NEntityEvents@@@Z and
//   ??0CEventFilter_HitEventWithWeapon@?A0x70a0c557@@QAE@J@Z; Ego_r.pdb layout CEventFilter_HitEventWithWeapon { long
//   WeaponDefIndex; } (the long at +0 compared here). The same test is inlined, instruction for instruction, as the
//   second predicate of GetEvent<CEventHitBy, CFilter_Bind4<CIsHitEventHitByThingWithScriptName,
//   CEventFilter_HitEventWithWeapon, CIsEventCreatedBeforeOrOn, CIsEventCreatedAfter> > 0x008D49B0 (0x008D49F8..0x008D4A13).
//   The class lives in an anonymous namespace (per-TU hash not reproducible), so the Leaf pins the prefix. FINDINGS chunk
//   404 ("likely"; confirmed by the inlined copy). Was landed as the stand-in "CIsHitEventHasHitByThing_008d43c0".
// Callee: NEntityEvents::CEventHitBase::GetPWeaponUsed() const @ 0x00810FD0 (PHitParams->PWeapon.GetPItem(); Ego_r.pdb
//   ?GetPWeaponUsed@CEventHitBase@NEntityEvents@@QBEPAVCThing@@XZ; CHitParameters::PWeapon PDB +0x60 = retail +0x58, the
//   twin GetPHitBy 0x00810FC0 reads PThingHitting at +0x50).
// Layout: retail CThing DefGlobalIndex (unsigned short) +0x8c, Dead bit 0 of +0x91 (PDB +0x9c / +0xa1).
// Shape: `movzx eax,word ptr [eax+8Ch] ; cmp eax,[esi]`, separate `mov al,1` / `xor al,al` exits.
class CThing {
public:
    unsigned char  _pad00[0x8c];
    unsigned short DefGlobalIndex;                 // +0x8c
    unsigned char  _pad8e[3];
    unsigned char  Dead : 1;                       // +0x91 bit 0
};

namespace NEntityEvents {
class CEventHitBase {
public:
    CThing* GetPWeaponUsed() const;                // 0x00810FD0
};
}

namespace {
class CEventFilter_HitEventWithWeapon {
public:
    long WeaponDefIndex;                           // +0x00
    bool operator()(NEntityEvents::CEventHitBase* event) const;
};

bool CEventFilter_HitEventWithWeapon::operator()(NEntityEvents::CEventHitBase* event) const
{
    CThing* weapon = event->GetPWeaponUsed();
    if (weapon && !weapon->Dead && weapon->DefGlobalIndex == WeaponDefIndex)
        return true;
    return false;
}
}

bool (CEventFilter_HitEventWithWeapon::*g_keepEventFilterHitEventWithWeapon)(NEntityEvents::CEventHitBase*) const =
    &CEventFilter_HitEventWithWeapon::operator();   // keeps the internal-linkage body emitted; not part of the retail function
