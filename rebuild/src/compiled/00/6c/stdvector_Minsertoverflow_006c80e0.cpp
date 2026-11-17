// std::vector<std::pair<long, COpinionDeed*> >::_M_insert_overflow(iterator, const value_type&, const __false_type&, size_type, bool) --
//   STLport 4.6 stl/_vector.c: allocate max(2 * size, size + n), copy the head, construct the n new values, copy the tail unless at
//   the end, free the old block
// Flags: /Gr
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@JPAVCOpinionDeed@@@std@@V?$allocator@U?$pair@JPAVCOpinionDeed@@@std@@@2@@std@@IAEXPAU?$pair@JPAVCOpinionDeed@@@2@ABU32@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C80E0, protected thiscall member, `ret 10h`.
// NF75 (round 7, land1117fu q10921): an explicit instantiation of stlport/fable_containers.h's _M_insert_overflow for the
//   vector<pair<long, COpinionDeed*> > that CTCHeroOpinionDeedLog::OnSerialise fills from its map<long, COpinionDeed*> (the landed
//   0x006C8610 / 0x006C8730 bind this protected IAE allocator spelling; the 8-byte element is the pair). Was the address-suffixed
//   stand-in FableVector_006c80e0::vector<pair<unsigned int, unsigned int> >. Descriptive-by-library (STLport; no PDB has it).
#include "stlport/fable_containers.h"
class COpinionDeed;
template void std::vector<std::pair<long, COpinionDeed*> >::_M_insert_overflow(std::pair<long, COpinionDeed*>*,
    const std::pair<long, COpinionDeed*>&, const std::__false_type&, unsigned int, bool);
