#ifndef FABLE_ENGINE_CREACTIONFREQUENCYTRAITS_H
#define FABLE_ENGINE_CREACTIONFREQUENCYTRAITS_H
#ifdef FABLE_RETAIL_REACTIONFREQUENCYTRAITS_CLASS_CHAIN_PASS24
class CDataOutputStream;
class CThingAICreature;
class CMemoryDataInputStream;
enum EReactionFrequencyType { REACTION_FREQUENCY_LINEAR_TIME = 1, REACTION_FREQUENCY_PERIODIC = 2, REACTION_FREQUENCY_CONTROLLED_COUNT = 3 };
class CReactionFrequencyTraits {                          // polymorphic base (vptr only)
public:
#ifdef FABLE_RETAIL_OPINION_STRENGTH_PASS26
    // Original FableWin TPI 1C493 slots 0..4. Native 00701710 calls slot zero for the weight.
    virtual float CalculateWeight(const CThingAICreature* creature) = 0;
    virtual void LogReaction(const CThingAICreature& creature, bool happened) = 0;
    virtual EReactionFrequencyType GetType() const = 0;
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream) = 0;
    virtual void TransferBinaryOut(CDataOutputStream& stream) const = 0;
#else
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream) = 0;
    virtual void TransferBinaryOut(CDataOutputStream& stream) const = 0;
    virtual EReactionFrequencyType GetType() const = 0;
    virtual float CalculateWeight(const CThingAICreature* creature) = 0;
    virtual void LogReaction(const CThingAICreature& creature, bool happened) = 0;
#endif
};
#endif
#endif
