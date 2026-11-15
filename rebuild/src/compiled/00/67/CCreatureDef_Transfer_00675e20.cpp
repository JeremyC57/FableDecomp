// CCreatureDef::Transfer(CPersistContext&) -- persist the creature definition and its pointed-to definitions.
// Scratch registers: the TransferVector switches end in the NODEFAULT `default: { __assume(0); }` block (retail `sub eax,2 ; je` with no
// `dec eax ; jne`, 0x00675EC1); an __assume in the function stops VC7.1 forwarding the inlined traits' stream arguments and the
// CCountedPointer::operator* result, so they are allocator candidates (eax at 0x00675E43 / 0x00675E50 / 0x00675E5B / 0x00675E80 /
// 0x00675E8D) and the vptr load 0x00675EA7 is the first rotating temporary.
// Leaf: ?Transfer@CCreatureDef@@UAEXAAVCPersistContext@@@Z
// Retail: 0x00675e20, 1912 bytes, virtual thiscall, ret 4.
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NODEFAULT
#define FABLE_RETAIL_CPERSIST_ORIGINAL_CLASS_TYPES_PASS22
#define FABLE_RETAIL_CCREATUREDEF_TRANSFER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRING_TRANSFER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERMAP_BINARY_BODY
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
#include "engine/CDataOutputStream.h"
#include "engine/CCreatureDef.h"
#include "engine/CCreatureStatsDef.h"
#include "engine/CRandomAppearanceMorph.h"
#include "engine/CWoundMorphs.h"
#include "engine/CExpressionSet.h"
#include "engine/CPersistContext.h"
#include "engine/CMemoryDataInputStream.h"

template<> void CPersistContext::Transfer<CWoundMorphs>(CWoundMorphs& value) {
    CheckCRC("");
    CPersistTraits<CWoundMorphs> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break;
    }
}
template<> void CPersistContext::Transfer<CExpressionSet>(CExpressionSet& value) {
    CheckCRC("");
    CPersistTraits<CExpressionSet> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break;
    }
}
template<> void CPersistContext::Transfer<long>(long& value) {
    CheckCRC("");
    CPersistTraits<long> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break;
    }
}
template<> void CPersistContext::Transfer<unsigned long>(unsigned long& value) {
    CheckCRC("");
    CPersistTraits<unsigned long> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break;
    }
}
template<> void CPersistContext::Transfer<bool>(bool& value) {
    CheckCRC("");
    CPersistTraits<bool> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break;
    }
}
template<> void CPersistContext::Transfer<CDefString>(CDefString& value) {
    CheckCRC("");
    CPersistTraits<CDefString> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break;
    }
}

void CCreatureDef::Transfer(CPersistContext& persist)
{
    persist.Transfer(*PWoundMorphs);
    persist.Transfer(*PRandomAppearanceMorph);
    persist.Transfer(*PExpressions);
    persist.TransferSubComponent(*PStats);
    persist.TransferVector(InitialActions);
    persist.TransferVector(InitialAppearanceModifiers);
    persist.TransferMap(PhonemeAnim);
    persist.TransferVector(DialogueVoices);
    persist.Transfer(Inventory);
    persist.Transfer(ClothingInventory);
    persist.Transfer(WeaponsInventory);
    persist.Transfer(HeroAbilitiesScreenInventory);
    persist.Transfer(QuestCardScreenInventory);
    persist.Transfer(MapScreenInventory);
    persist.Transfer(MagicScreenInventory);
    persist.Transfer(StatsScreenInventory);
    persist.Transfer(ExperienceScreenInventory);
    persist.Transfer(TradeScreenInventory);
    persist.Transfer(AugmentScreenInventory);
    persist.Transfer(QuestsScreenInventory);
    persist.Transfer(InGameMenuScreen);
    persist.Transfer(CreatureGroup);
    persist.Transfer(BattleCrySound);
    persist.Transfer(OpinionSourceDef);
    persist.Transfer(RespawnWaitInDaysMin);
    persist.Transfer(RespawnWaitInDaysMax);
    persist.Transfer(RespawnWaitInFramesMin);
    persist.Transfer(RespawnWaitInFramesMax);
    persist.Transfer(UseActualHitPosForHitEffects);
    persist.Transfer(Short);
    persist.Transfer(FlashOnHit);
    persist.Transfer(PickPocketable);
    persist.Transfer(ChildThatCanRegionFollow);
    persist.Transfer(AllowedInProtectedTowns);
    persist.TransferMap(RadiusWeight.StatWeights);
    persist.Transfer(RadiusSpread);
    persist.Transfer(AGhostIsWhatIBe);
}