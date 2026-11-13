#ifndef FABLE_ENGINE_CTHINGCOMPARE_NEARESTTOPOS_H
#define FABLE_ENGINE_CTHINGCOMPARE_NEARESTTOPOS_H
// CThingCompare_NearestToPos (new, chunk 2015, row 0x007A0D90 CTCWillLightning::GetTargets). Layout: Ego_r.pdb struct_layouts_egor.tsv
// (12 bytes, `C3DVector Pos` +0). Names: FableWin.pdb ??0CThingCompare_NearestToPos@@QAE@ABVC3DVector@@@Z (inline in retail: the caller
// builds the comparator straight in std::sort's by-value argument slot, 0x007A0FBF..0x007A0FCF) and the member template
// ??$?RVCThing@@@CThingCompare_NearestToPos@@QBE_NPAVCThing@@0@Z (out of line, landed 0x007A2F90; called by the sort helpers of the TU).
#include "engine/C3DVector.h"
class CThingCompare_NearestToPos {
    C3DVector Pos;
public:
    CThingCompare_NearestToPos(const C3DVector& pos) : Pos(pos) {}
    template<class T> bool operator()(T* a, T* b) const;
};
#endif
