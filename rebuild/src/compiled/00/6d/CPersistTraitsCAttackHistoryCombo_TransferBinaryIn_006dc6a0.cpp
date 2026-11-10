// CPersistTraits<CAttackHistoryCombo>::TransferBinaryIn(CMemoryDataInputStream& stream, CAttackHistoryCombo& value) -- binary read of one attack-history combo: the entry count, every (attack history type, damage attribute) pair pushed onto AttackCombo, then the Multiplier
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCAttackHistoryCombo@@@@QAEXAAVCMemoryDataInputStream@@AAVCAttackHistoryCombo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DC6A0, 447 bytes (the catalog row's 720 runs on into 0x006DC860), thiscall `ret 8` (the
//   traits `this` is unused); speed TU, no frame pointer.
// Identity: Ego_r.pdb / FableWin.pdb ?TransferBinaryIn@?$CPersistTraits@VCAttackHistoryCombo@@@@QAEXAAVCMemoryDataInputStream@@AAVCAttackHistoryCombo@@@Z;
//   the only caller is the CAttackHistoryCombo vector loop 0x006DD580 (stride 0x10, call 0x006DD5F2), run on CHeroExperienceDef's
//   AttackHistoryCombos right after StatIncreasePerHitType. The catalog label CPersistTraits<CHeroStatIncreaseData> is propagated.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of each inline read),
//   vector<pair<EAttackHistoryType,EDamageAttribute> >::_M_insert_overflow(pos, x, __true_type, 1, true) 0x006DB8C0.
// Shape: the inline CDataInputStream::ReadULONG / ReadType<T> / ReadFloat reads, a down-counting loop over the count, and
//   push_back(std::make_pair(type, attribute)) (FableWin.pdb ??$make_pair@AAW4EAttackHistoryType@@AAW4EDamageAttribute@@@std@@:
//   make_pair of the two lvalue locals). STLport's inline push_back with the _M_insert_overflow body visible (no capture).
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CATTACKHISTORYCOMBO
#include "stlport/fable_containers.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CAttackHistoryCombo.h"

template<> void CPersistTraits<CAttackHistoryCombo>::TransferBinaryIn(CMemoryDataInputStream& stream, CAttackHistoryCombo& value)
{
    for (unsigned long count = stream.ReadULONG(); count != 0; --count) {
        EAttackHistoryType type = stream.ReadType<EAttackHistoryType>();
        EDamageAttribute attribute = stream.ReadType<EDamageAttribute>();
        value.AttackCombo.push_back(std::make_pair(type, attribute));
    }
    value.Multiplier = stream.ReadFloat();
}