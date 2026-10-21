// CThingFilter_IsInterestingToCreature::operator()(const CThing& thing) const -- any other thing whose type tag is 1 or 2
// Leaf: ??RCThingFilter_IsInterestingToCreature@@QBE_NABVCThing@@@Z
// Retail: Fable.exe @ 0x006A7120, 32 bytes, `ret 4`; Ego_r.pdb names this operator (SourceCreature +0x00 is the only member).
// Body: false for the source creature itself, else true when the thing's type tag byte (+0x10) is 1 or 2.
// Layout: CThing +0x10 is padding in Ego_r.pdb; the same tag is compared with 2 by CPlayerManager::IsThingMainPlayer
// (0x004497C0), so it keeps that placeholder name here. Classes modelled locally (no retail-accurate CThing header).
// Shape: /O2, one bool expression (`mov eax,1` / `xor eax,eax`); an early `return false` gives `xor al,al`.
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingTypeTag;                          // +0x10
};
class CThingCreatureBase : public CThing { };
class CThingFilter_IsInterestingToCreature {
public:
    CThingCreatureBase* SourceCreature;                  // +0x00
    bool operator()(const CThing& thing) const;
};
bool CThingFilter_IsInterestingToCreature::operator()(const CThing& thing) const
{
    return &thing != SourceCreature && (thing.ThingTypeTag == 1 || thing.ThingTypeTag == 2);
}