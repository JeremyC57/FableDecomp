#ifndef FABLE_ENGINE_CDEADCREATUREANIMPRELOADER_H
#define FABLE_ENGINE_CDEADCREATUREANIMPRELOADER_H
// NEW (agentNF11, row 0x00835BD0 CThingDeadCreature::PreloadDeadAnimation): the functor the dead-creature setup hands to
// std::for_each over its appearance animation set (the instantiation is the landed 0x008365F0, which still defines its own copy of the
// class with the operator() body). Ego_r.pdb: ??0CDeadCreatureAnimPreloader@@QAE@ABVCMeshDataBank@@@Z (inline in retail: the caller stores
// the bank pointer into the by-value argument, `push edi`, 0x00835BFC), ??RCDeadCreatureAnimPreloader@@QBEXABU?$pair@KVCAnimationEntry@@@std@@@Z,
// struct_layouts_egor.tsv CDeadCreatureAnimPreloader (4 bytes: MeshBank +0). Declarations only for operator().
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class CAnimationEntry;
class CMeshDataBank;
class CDeadCreatureAnimPreloader {
public:
    CDeadCreatureAnimPreloader(const CMeshDataBank& mesh_bank) : MeshBank(&mesh_bank) {}
    void operator()(const std::pair<unsigned long, CAnimationEntry>& entry) const;
private:
    const CMeshDataBank* MeshBank;                                       // +0x00
};
FABLE_STATIC_ASSERT(sizeof(CDeadCreatureAnimPreloader) == 4);
#endif
