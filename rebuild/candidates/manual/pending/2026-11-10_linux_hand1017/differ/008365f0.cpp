// std::for_each(const std::pair<unsigned long,CAnimationEntry>*, const std::pair<unsigned long,CAnimationEntry>*, CDeadCreatureAnimPreloader) -- start loading every STANDARD_DEAD animation of a creature's animation set
// Grade: DIFFER(119v119) -- register rotation only (4 difflines): retail keeps the strcmp-intrinsic zero (CSE'd with the `false` argument) in eax and loads BankIndex into ecx, ours puts the zero in edx and BankIndex in eax (c2prio --rotation: the zero is the second rotation temp, ecx busy with the 0Eh count -> edx); the empty-range path copies the functor through edx in retail, ecx here. Spellings probed (c2prio, no change): !strcmp, reversed operands, memcmp(...,14), named name / info locals, an inline Eq/IsDeadAnim helper, early return, bool local. 1 grade.
// Leaf: ??$for_each@PBU?$pair@KVCAnimationEntry@@@std@@VCDeadCreatureAnimPreloader@@@std@@YI?AVCDeadCreatureAnimPreloader@@PBU?$pair@KVCAnimationEntry@@@0@0V1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008365F0, 119 bytes, /Gr __fastcall `ret 8` (hidden return pointer in ecx, first in edx, last and
//   the 4-byte functor on the stack); one caller 0x00835C02.
// Identity: Ego_r.pdb ??$_For_each@PBU?$pair@KVCAnimationEntry@@@std@@VCDeadCreatureAnimPreloader@@@std@@YI... (the Dinkumware
//   worker over the same `const pair*` iterator; retail STLport 4.6 has the plain for_each) + ??RCDeadCreatureAnimPreloader@@QBEXABU?$pair@
//   KVCAnimationEntry@@@std@@@Z + ??0CDeadCreatureAnimPreloader@@QAE@ABVCMeshDataBank@@@Z; struct_layouts_egor.tsv: the functor is 4 bytes,
//   MeshBank (CMeshDataBank*) at +0. The catalog's CTCInventoryAbilities_PreloadDeadAnimation was a descriptive guess.
// Callees: CDefStringTable::GetStringAsCharArray 0x009D49F0 (via the inline CDefString::GetAsCharArray), CMeshDataBank::PeekAnimation
//   0x00A242C0, C3DAnimationInfo::BeginLoading(bool) const 0x00A26730. The literal compare is VC7.1's strcmp intrinsic against a
//   14-byte literal (`repe cmpsb`, ecx = 0Eh).
// Graded with FABLE_HDR_OVERLAY=$A/hdr (CDefString / fable_definition_table / CMeshDataBank / stlport fable_algorithm opt-ins).
// Flags: /Gr
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRINGTABLE_ASCHARARRAY
#define FABLE_RETAIL_CDEFSTRING_GETASCHARARRAY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CRESOURCE_RESOURCELIST
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_C3DANIMATIONINFO
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#include <string.h>
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/CAnimationEntry.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DAnimationInfo.h"

// Functor the dead-creature setup hands to for_each over its animation-set entries: preload the STANDARD_DEAD animations.
class CDeadCreatureAnimPreloader {
public:
    CDeadCreatureAnimPreloader(const CMeshDataBank& mesh_bank) : MeshBank(&mesh_bank) {}
    void operator()(const std::pair<unsigned long, CAnimationEntry>& entry) const
    {
        if (strcmp(entry.second.AnimName.GetAsCharArray(), "STANDARD_DEAD") == 0)
            MeshBank->PeekAnimation(entry.second.BankIndex)->BeginLoading(false);
    }
private:
    const CMeshDataBank* MeshBank;   // +0x00
};

template CDeadCreatureAnimPreloader FABLE_FASTCALL std::for_each(const std::pair<unsigned long, CAnimationEntry>*,
                                                                 const std::pair<unsigned long, CAnimationEntry>*,
                                                                 CDeadCreatureAnimPreloader);
