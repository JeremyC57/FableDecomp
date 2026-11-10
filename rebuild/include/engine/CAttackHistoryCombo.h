#ifndef FABLE_ENGINE_CATTACKHISTORYCOMBO_H
#define FABLE_ENGINE_CATTACKHISTORYCOMBO_H
// CAttackHistoryCombo (chunk 1020, row 0x006DC6A0 CPersistTraits<CAttackHistoryCombo>::TransferBinaryIn). Retail model, all under
// FABLE_RETAIL_CATTACKHISTORYCOMBO. Ego_r.pdb (struct_layouts_egor.tsv, 20 bytes there): AttackCombo
// vector<pair<EAttackHistoryType,EDamageAttribute> > +0, float Multiplier +0x10 -- +0xC in retail with the 12-byte STLport vector
// (0x006DC85x stores the float at +0xC; CHeroExperienceDef::AttackHistoryCombos is the vector<CAttackHistoryCombo> at +0xA8, stride
// 0x10 in the element loop 0x006DD580). EAttackHistoryType's enumerators are in no name set (an MS forward enum, as CDegradableDef.h).
#ifdef FABLE_RETAIL_CATTACKHISTORYCOMBO
#include "rebuild_abi.h"
#include "engine/EDamageAttribute.h"
#include "stlport/fable_containers.h"
#ifdef _MSC_VER
enum EAttackHistoryType;
#else
enum EAttackHistoryType { };
#endif
class CAttackHistoryCombo {
public:
    std::vector<std::pair<EAttackHistoryType,EDamageAttribute> > AttackCombo;   // +0x00
    float Multiplier;                                                             // +0x0C
};
FABLE_STATIC_ASSERT(sizeof(CAttackHistoryCombo)==0x10);
#endif
#endif
