#ifndef FABLE_ENGINE_CINITBASECLASS_H
#define FABLE_ENGINE_CINITBASECLASS_H
#include "engine/CBaseClass.h"
// Layout: original base class4 bytes plus private Valid+4, native size8.
class CInitBaseClass:public CBaseClass {private:bool Valid;public:CInitBaseClass();~CInitBaseClass();
#ifdef FABLE_RETAIL_CINITBASECLASS
// Retail opt-in (chunk 734, row 0x004A5A40 CWorld::UpdateFrame): Ego_r.pdb ?IsValid@CInitBaseClass@@QBE_NXZ, out of line in
// retail at 0x0099A330 (`mov al,[ecx+4] ; ret`: Valid); UpdateFrame calls it on each CPlayer (RTTI CPlayer : CInitBaseClass).
bool IsValid() const;                                                    // 0x0099A330
#endif
};
#endif
