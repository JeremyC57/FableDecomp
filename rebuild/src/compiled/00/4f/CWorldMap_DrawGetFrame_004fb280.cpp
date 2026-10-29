class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
#include "engine/CWorldMap.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorldMap_Methods : CWorldMap {
    long DrawGetFrame() const;
};
long CWorldMap_Methods::DrawGetFrame() const { return CWorld::GetFrame(); }