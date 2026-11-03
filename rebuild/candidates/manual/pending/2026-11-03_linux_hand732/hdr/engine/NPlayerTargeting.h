#ifndef FABLE_ENGINE_NPLAYERTARGETING_H
#define FABLE_ENGINE_NPLAYERTARGETING_H
// New shared header (chunk 732, row 0x006AA720): Ego_r.pdb / FableWin.pdb
// ?IsThingInTargetingRangeForTargetType@NPlayerTargeting@@YI_NABVCThing@@0K@Z (retail 0x00802D10: `mov edx,target ; mov ecx,hero ;
// push type ; call`).
class CThing;
namespace NPlayerTargeting {
bool __fastcall IsThingInTargetingRangeForTargetType(const CThing& thing, const CThing& target, unsigned long type);   // 0x00802D10
}
#endif
