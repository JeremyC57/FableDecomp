// CPersistTraits<CReactionFrequencyTraitsArray>::TransferIn(CPersistContext& context, CStringParser& parser, CReactionFrequencyTraitsArray& value) -- text load of one frequency-traits slot pair: `.Set(<reaction>, <Type>(<args>))`, where Type is Linear (min wait, wait range in seconds), PeriodicFrequency (ramp-up, cut-off interval, low and high frequency) or ControlledCount (min gap, recharge, max count[, allow individual repeats]); LowHigh[0][reaction] gets the traits as read, LowHigh[1][reaction] the high-frequency copy (times 0.2, frequency x5 / x2, gap x5); anything but `.Set` is a persist error
// Grade: DIFFER(2273v2272) -- 484 lines, same instruction stream and size class: frame 0x20 v 0x18 -- retail keeps the context in ebx (pushed to the 11 traits calls) and homes the compare bools and CCharString temporaries in the dead parser / value argument slots, ours keeps the bool in bl and reloads the context, so every stack offset shifts
// Next idea: find the source spelling that makes VC7.1 give ebx to `context` (e.g. one `CPersistContext& ctx` use per branch as retail, or the `== ")"` results consumed differently) -- the uninitialised CPersistTraits<float> temporary (empty user ctor, a3) was the step that fixed the size
// Leaf: ?TransferIn@?$CPersistTraits@VCReactionFrequencyTraitsArray@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCReactionFrequencyTraitsArray@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00677970, 2272 bytes, `ret 0Ch` (traits `this` unused), frame 0x18.
// Identity: Ego_r.pdb / FableWin.pdb ?TransferIn@?$CPersistTraits@VCReactionFrequencyTraitsArray@@@@QAEXAAVCPersistContext@@
//   AAVCStringParser@@AAVCReactionFrequencyTraitsArray@@@Z: three arguments (`ret 0Ch`), the body news the three
//   CReactionFrequencyTraits subclasses by their type names and stores them at LowHigh[0][i] (+0) and LowHigh[1][i] (+0x13C =
//   79 * 4: PDB CReactionFrequencyTraitsArray { CReactionFrequencyTraits* LowHigh[2][79] }); it sits after the landed
//   TransferBinaryOut 0x00677840 / TransferBinaryIn 0x006778A0 of the same CPersistTraits. The catalog placeholder
//   TOpinionTransientOffsetList_TransferParsing came from the error literal "Unknown command for TOpinionTransientOffsetList",
//   which Lionhead copied from the COpinionTransientOffset list reader.
// Callees: CStringParser SkipPastNextItemIfIs 0x009B9E00 / ReadAsSymbol 0x009BA790 / SkipWhitespace 0x009B9C60 /
//   ReadAsStringUntilString 0x009BA330, CPersistContext::PersistError 0x009BAA90, CBasicString<char>::operator==(const char*)
//   0x004115A0, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CPersistTraitsSLONG::TransferIn 0x009E2590, CPersistTraits<float>::TransferIn
//   0x0040F7E0, CPersistTraits<long>::TransferIn 0x005D8167, CPersistTraits<bool>::TransferIn 0x00403FF0, operator new 0x00BFEA1A,
//   CReactionFrequencyTraits_LinearTime(float, float) 0x006FF9A0, _PeriodicFrequency(float, float, float, float) 0x006FED30,
//   _ControlledCount(unsigned long, float, float, bool) 0x006FEE20; CMainGameComponent::ConstantFPS 0x01375550.
// Layout: CReactionFrequencyTraits family as the landed CreateFromType 0x006FF930 (engine/CReactionFrequencyTraits_*.h are flattened
//   structs without the polymorphic base: header debt).
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"

void* __cdecl operator new(unsigned int size);            // 0x00BFEA1A
template<> void CPersistTraits<long>::TransferIn(CPersistContext& context, CStringParser& parser, long& value);    // 0x005D8167
template<> class CPersistTraits<float> {
public:
    CPersistTraits() { }
    float DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, float& value);   // 0x0040F7E0
};
enum EOpinionReactionType { OPINION_REACTION_TYPE_NONE = 0 };

class CMainGameComponent {
protected:
    static long ConstantFPS;                              // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }
};

class CThingAICreature;
class CMemoryDataInputStream;
enum EReactionFrequencyType { REACTION_FREQUENCY_LINEAR_TIME = 1, REACTION_FREQUENCY_PERIODIC = 2, REACTION_FREQUENCY_CONTROLLED_COUNT = 3 };
class CReactionFrequencyTraits {                          // polymorphic base (vptr only)
public:
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream) = 0;
    virtual void TransferBinaryOut(CDataOutputStream& stream) const = 0;
    virtual EReactionFrequencyType GetType() const = 0;
    virtual float CalculateWeight(const CThingAICreature* creature) = 0;
    virtual void LogReaction(const CThingAICreature& creature, bool happened) = 0;
};
class CReactionFrequencyTraits_LinearTime : public CReactionFrequencyTraits {   // vtable 0x01264B64, 0x10 bytes
public:
    float MinWait;                                        // +0x04
    float WaitRange;                                      // +0x08
    unsigned long FrameHappened;                          // +0x0c
    CReactionFrequencyTraits_LinearTime(float minWait, float waitRange);                                   // 0x006FF9A0
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;
    virtual EReactionFrequencyType GetType() const;
    virtual float CalculateWeight(const CThingAICreature* creature);
    virtual void LogReaction(const CThingAICreature& creature, bool happened);
};
class CReactionFrequencyTraits_PeriodicFrequency : public CReactionFrequencyTraits {   // vtable 0x01264B7C, 0x20 bytes
public:
    float LowFreqPerMin;                                  // +0x04
    float HighLowFreqInterval;                            // +0x08
    unsigned long FramesPerMinute;                        // +0x0c
    unsigned long LastCutoffFrame;                        // +0x10
    unsigned long RampUpTimeFrames;                       // +0x14
    unsigned long CutoffIntervalFrames;                   // +0x18
    unsigned long LastHappenedFrame;                      // +0x1c
    CReactionFrequencyTraits_PeriodicFrequency(float rampUpTime, float cutoffInterval, float lowFreqPerMin, float highFreqPerMin);   // 0x006FED30
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;
    virtual EReactionFrequencyType GetType() const;
    virtual float CalculateWeight(const CThingAICreature* creature);
    virtual void LogReaction(const CThingAICreature& creature, bool happened);
};
class CReactionFrequencyTraits_ControlledCount : public CReactionFrequencyTraits {   // vtable 0x01264B94, 0x2c bytes
public:
    bool AllowIndividualRepeats;                          // +0x04
    CThingAICreature* PLastReactionCreature;              // +0x08
    bool InRisingPhase;                                   // +0x0c
    unsigned long MinGapFrames;                           // +0x10
    float CountRechargePerFrame;                          // +0x14
    float CurrentAvailableCount;                          // +0x18
    float MaxCount;                                       // +0x1c
    float InvMaxCount;                                    // +0x20
    unsigned long FrameLastStarted;                       // +0x24
    unsigned long FrameLastRecharged;                     // +0x28
    CReactionFrequencyTraits_ControlledCount(unsigned long minGapFrames, float countRecharge, float maxCount, bool allowIndividualRepeats);   // 0x006FEE20
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;
    virtual EReactionFrequencyType GetType() const;
    virtual float CalculateWeight(const CThingAICreature* creature);
    virtual void LogReaction(const CThingAICreature& creature, bool happened);
};
class CReactionFrequencyTraitsArray {
public:
    CReactionFrequencyTraits* LowHigh[2][79];             // +0x000 (PDB, 632 bytes)
};
template<> class CPersistTraits<CReactionFrequencyTraitsArray> {
public:
    CReactionFrequencyTraitsArray DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, CReactionFrequencyTraitsArray& value);
};

void CPersistTraits<CReactionFrequencyTraitsArray>::TransferIn(CPersistContext& context, CStringParser& parser, CReactionFrequencyTraitsArray& value)
{
    if (parser.SkipPastNextItemIfIs(".Set")) {
        if (!(parser.ReadAsSymbol() == "("))
            return;
        EOpinionReactionType reaction;
        CPersistTraits<EOpinionReactionType>().TransferIn(context, parser, reaction);
        if (!(parser.ReadAsSymbol() == ","))
            return;
        parser.SkipWhitespace();
        CCharString type = parser.ReadAsStringUntilString("(");
        if (!(parser.ReadAsSymbol() == "("))
            return;
        if (type == "Linear") {
            float minWait;
            float waitRange;
            parser.SkipWhitespace();
            CPersistTraits<float>().TransferIn(context, parser, minWait);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            CPersistTraits<float>().TransferIn(context, parser, waitRange);
            unsigned long fps = CMainGameComponent::GetConstantFPS();
            value.LowHigh[0][reaction] = new CReactionFrequencyTraits_LinearTime(minWait * fps, waitRange * fps);
            value.LowHigh[1][reaction] = new CReactionFrequencyTraits_LinearTime(minWait * fps * 0.2f, waitRange * fps * 0.2f);
            if (!(parser.ReadAsSymbol() == ")"))
                return;
        } else if (type == "PeriodicFrequency") {
            float rampUpTime;
            float cutoffInterval;
            float lowFreqPerMin;
            float highFreqPerMin;
            parser.SkipWhitespace();
            CPersistTraits<float>().TransferIn(context, parser, rampUpTime);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            CPersistTraits<float>().TransferIn(context, parser, cutoffInterval);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            CPersistTraits<float>().TransferIn(context, parser, lowFreqPerMin);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            CPersistTraits<float>().TransferIn(context, parser, highFreqPerMin);
            value.LowHigh[0][reaction] = new CReactionFrequencyTraits_PeriodicFrequency(rampUpTime, cutoffInterval, lowFreqPerMin, highFreqPerMin);
            value.LowHigh[1][reaction] = new CReactionFrequencyTraits_PeriodicFrequency(rampUpTime * 0.2f, cutoffInterval * 0.2f, lowFreqPerMin * 5.0f, highFreqPerMin * 2.0f);
            if (!(parser.ReadAsSymbol() == ")"))
                return;
        } else if (type == "ControlledCount") {
            long minGap;
            float countRecharge;
            float maxCount;
            parser.SkipWhitespace();
            CPersistTraits<long>().TransferIn(context, parser, minGap);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            CPersistTraits<float>().TransferIn(context, parser, countRecharge);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            CPersistTraits<float>().TransferIn(context, parser, maxCount);
            CCharString symbol = parser.ReadAsSymbol();
            if (symbol == ")") {
                value.LowHigh[0][reaction] = new CReactionFrequencyTraits_ControlledCount(minGap, countRecharge, maxCount, true);
                value.LowHigh[1][reaction] = new CReactionFrequencyTraits_ControlledCount(minGap * 5, countRecharge * 0.2f, maxCount * 0.2f, true);
            } else if (symbol == ",") {
                bool allowRepeats;
                parser.SkipWhitespace();
                CPersistTraits<bool>().TransferIn(context, parser, allowRepeats);
                value.LowHigh[0][reaction] = new CReactionFrequencyTraits_ControlledCount(minGap, countRecharge, maxCount, allowRepeats);
                value.LowHigh[1][reaction] = new CReactionFrequencyTraits_ControlledCount(minGap * 5, countRecharge * 0.2f, maxCount * 0.2f, allowRepeats);
                if (!(parser.ReadAsSymbol() == ")"))
                    return;
            }
        }
        parser.SkipWhitespace();
        parser.ReadAsSymbol() == ")";
    } else {
        context.PersistError(parser, "Unknown command for TOpinionTransientOffsetList");
    }
}
