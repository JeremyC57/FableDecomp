#ifndef FABLE_ENGINE_CCREATUREGENERATIONFAMILYDEF_H
#define FABLE_ENGINE_CCREATUREGENERATIONFAMILYDEF_H
// New header (chunk 2017, row 0x00974110 CAIStateGroup_JackOfBladesBase::Summon). RTTI .?AVCCreatureGenerationFamilyDef@@ :
// CParentDefClassBase (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb TPI names and order (Creatures vector<long>, DifficultyLevel,
// GeneratorObject, GenerationTypes vector<ECreatureGeneratorGenerateType>), at the retail offsets: CParentDefClassBase is 0x39 bytes
// (packed), so Creatures sits at +0x3C (Summon reads its start / finish at +0x3C / +0x40, 0x009741BB) where Ego_r has +0x40.
// Needs FABLE_RETAIL_DEF_CLASS_BASE (engine/CParentDefClassBase.h).
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
enum ECreatureGeneratorGenerateType;                                     // MS forward enum
class CCreatureGenerationFamilyDef : public CParentDefClassBase {
public:
    std::vector<long> Creatures;                                         // +0x3c
    long DifficultyLevel;                                                // +0x48
    long GeneratorObject;                                                // +0x4c
    std::vector<ECreatureGeneratorGenerateType> GenerationTypes;         // +0x50
};
#endif
