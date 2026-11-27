#ifndef FABLE_ENGINE_CCREATUREGENERATIONFAMILYDEF_H
#define FABLE_ENGINE_CCREATUREGENERATIONFAMILYDEF_H
// New header (chunk 2017, row 0x00974110 CAIStateGroup_JackOfBladesBase::Summon). RTTI .?AVCCreatureGenerationFamilyDef@@ :
// CParentDefClassBase (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb TPI names and order (Creatures vector<long>, DifficultyLevel,
// GeneratorObject, GenerationTypes vector<ECreatureGeneratorGenerateType>), at the retail offsets: CParentDefClassBase is 0x39 bytes
// (packed), so Creatures sits at +0x3C (Summon reads its start / finish at +0x3C / +0x40, 0x009741BB) where Ego_r has +0x40.
// Needs FABLE_RETAIL_DEF_CLASS_BASE (engine/CParentDefClassBase.h).
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_GENERATOR_CREATE_PASS29
enum ECreatureGeneratorGenerateType { GENERATE_NORMAL,GENERATE_AMBUSH_DROP_IN,GENERATE_AMBUSH_JUMP_OUT,GENERATE_GENERATOR_ANIMATION,GENERATE_UNDEAD };
#else
enum ECreatureGeneratorGenerateType;                                     // MS forward enum
#endif
class CCreatureGenerationFamilyDef : public CParentDefClassBase {
public:
#ifdef FABLE_RETAIL_CCREATUREGENERATIONFAMILYDEF_TRANSFER
// Opt-in (NF40, 2026-11-16, row 0x004696C1 CCreatureGenerationFamilyDef::Transfer): Ego_r.pdb / FableWin.pdb ?Transfer@CCreatureGenerationFamilyDef@@UAEXAAVCPersistContext@@@Z,
// the def's override of CDefClassBase::Transfer (virtual slot 18). Declaration only (the elaborated class needs no include); no layout change.
public: virtual void Transfer(class CPersistContext&);
public:
#endif
    std::vector<long> Creatures;                                         // +0x3c
    long DifficultyLevel;                                                // +0x48
    long GeneratorObject;                                                // +0x4c
    std::vector<ECreatureGeneratorGenerateType> GenerationTypes;         // +0x50
#ifdef FABLE_RETAIL_CCREATUREGENERATIONFAMILYDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CCreatureGenerationFamilyDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
};
#endif
