#ifndef FABLE_ENGINE_CTHINGFILTER_HASPHYSICSMESH_H
#define FABLE_ENGINE_CTHINGFILTER_HASPHYSICSMESH_H
// New header (chunk 2009, row 0x00745120 CTCPerceiveThing::UpdateLineOfSight). Ego_r.pdb: no data members (an empty filter; retail
// never writes its frame byte, 0x007454E5 passes `lea eax,[esp+78h]`); ??0CThingFilter_HasPhysicsMesh@@QAE@XZ (FableWin.pdb 0x01D2BD40,
// inline in retail) and ??RCThingFilter_HasPhysicsMesh@@QBE_NABVCThing@@@Z (Ego_r.pdb 0x005A3170 / FableWin.pdb 0x01F92C20; retail
// 0x006A6F50, called by the landed search 0x00746560).
#include "stlport/fable_containers.h"
class CThing;
class CThingFilter_HasPhysicsMesh : public std::unary_function<CThing,bool> {
public:
 CThingFilter_HasPhysicsMesh() {}
 bool operator()(const CThing&) const;                                   // 0x006A6F50
};
#endif
