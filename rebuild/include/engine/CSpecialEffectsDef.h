#ifndef FABLE_ENGINE_CSPECIALEFFECTSDEF_H
#define FABLE_ENGINE_CSPECIALEFFECTSDEF_H
#include "engine/CSubDefClassBase.h"
class CCharString;
#include "stlport/fable_map.h"
class CSpecialEffectsStringMap {
public: std::map<unsigned long,long> ContainmentMap;
};
// Original TPI0x17a69; original SpecialEffectsMap28 retained at native sizec.
class CSpecialEffectsDef : public CSubDefClassBase {
public:
    unsigned char _pad_0x25[3];
    CSpecialEffectsStringMap SpecialEffectsMap;
    bool GetEffectFromString(const CCharString&,long&) const;
#ifdef FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
    // Retail opt-in (chunk 782, row 0x007941D0): FableWin.pdb ?StatGetTypeName@CSpecialEffectsDef@@SI?AVCCharString@@XZ, inline (the
    // class-name literal GetSubDef<CSpecialEffectsDef> 0x00669580 builds for its lookup; the landed 0x00669580 body). Needs fable_string.h.
    static CCharString __fastcall StatGetTypeName() { return CCharString("CSpecialEffectsDef", -1); }
#endif
};
FABLE_STATIC_ASSERT(sizeof(CSpecialEffectsDef)==0x34);
#ifdef FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
// Retail opt-in (chunk 782, row 0x007941D0 CTCUnholyPower ctor): the body of Ego_r.pdb
// ?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z (retail 0x00669D30, called out of line) visible in the TU, as a
// same-TU noinline body (EXTRA_RULES chunk 422): VC7.1 then treats the caller's effect-index local as uncaptured and keeps it in a
// register across the CreateParticleEffect calls, as retail does (0x0079476B `mov edi,[esp+28h]` used for both hand effects).
// Body (the landed 0x00669D30 shape): the CRC of the name over its text and length, then the map lookup.
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
inline __declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& effect) const
{
    std::map<unsigned long,long>::const_iterator it = SpecialEffectsMap.ContainmentMap.find(CCRC::Calc(0, name.GetAsCharArray(), name.GetLength()));
    if (it != SpecialEffectsMap.ContainmentMap.end()) {
        effect = (*it).second;
        return true;
    }
    return false;
}
#endif
#endif
