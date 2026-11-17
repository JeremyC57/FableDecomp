// std::pair<CCharString,C3DMeshVolume>::~pair() -- destroy a named mesh volume entry: release the volume's counted convex volume, destroy its name, then the key string
// Leaf: ??1?$pair@VCCharString@@VC3DMeshVolume@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6F390, length 72 bytes, `jmp` tail. Called from the introsort partition
//   __unguarded_partition 0x00A71910 (the by-value pivot of a vector<pair<CCharString,C3DMeshVolume>> sort; NF89 binds this spelling);
//   0x00A70130 is the same destructor emitted in another TU (no references).
// Callees: CCPPointerInfo::DeleteFunc through the block, operator delete @ 0x00BFE9BC (the block), CCharString::~CCharString @ 0x0099EAE0
//   (x2, the key is the tail jmp).
// Name fix (agentNF91, q11196): the leaf was already the implicit STLport ~pair COMDAT; the catalog name _global::CCountedPointer_Dtor_00a6f390
//   was a stand-in (now std::pair<CCharString,C3DMeshVolume>::~pair, the spelling of the sibling 0x00A326D0). The row moved off its local
//   CCharString / C3DMeshVolume / std::pair models onto fable_string.h, engine/C3DMeshVolume.h and stlport/fable_containers.h. The implicit
//   destructor is a compiler-emitted COMDAT: the anonymous-namespace helper below only makes VC7.1 emit it (nf86 pattern).
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
namespace {
// Fidelity: scaffolding, not retail -- the qualified explicit destructor call makes VC7.1 emit the implicit ~pair COMDAT.
void EmitPairDestructor(std::pair<CCharString, C3DMeshVolume>* entry)
{
    entry->std::pair<CCharString, C3DMeshVolume>::~pair();
}
}
