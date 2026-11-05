// Leaf: ?CanRelocateData@CLandscapeLayerMesh@@QAE_NH@Z
#include "rebuild_abi.h"
struct S { char p[0x5c]; unsigned char f5c; };
struct CLandscapeLayerMesh { char p[0x40]; S* f40; bool CanRelocateData(int a); };
bool CLandscapeLayerMesh::CanRelocateData(int a){ return this->f40->f5c==0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?CanRelocateData@CLandscapeLayerMesh@@EBE_NJ@Z); evidence: RTTI vtable slot + PDB name set.
