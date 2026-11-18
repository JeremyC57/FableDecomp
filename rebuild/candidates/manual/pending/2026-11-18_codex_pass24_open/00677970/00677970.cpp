// CPersistTraits<CReactionFrequencyTraitsArray>::TransferIn reads the three reaction-frequency formats.
// Grade: DIFFER(2304v2272). Initializing the numeric trait locals improves instruction correspondence. Context allocation and numeric token string lifetimes still differ.
// Leaf: ?TransferIn@?$CPersistTraits@VCReactionFrequencyTraitsArray@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCReactionFrequencyTraitsArray@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00677970, 2272 bytes, `ret 0Ch` (traits `this` unused), frame 0x18.

#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_REACTIONFREQUENCYTRAITS_CLASS_CHAIN_PASS24
#define FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"

#include "engine/CReactionFrequencyTraitsArray.h"
#include "engine/CReactionFrequencyTraits_LinearTime.h"
#include "engine/CReactionFrequencyTraits_PeriodicFrequency.h"
#include "engine/CReactionFrequencyTraits_ControlledCount.h"
#include "engine/CMainGameComponent.h"

enum EOpinionReactionType { OPINION_REACTION_TYPE_NONE = 0 };
template<> void CPersistTraits<unsigned long>::TransferIn(CPersistContext& persist_context, CStringParser& parser, unsigned long& val);
template<> void CPersistTraits<bool>::TransferIn(CPersistContext& persist_context, CStringParser& parser, bool& val);

void CPersistTraits<CReactionFrequencyTraitsArray>::TransferIn(CPersistContext& persist_context, CStringParser& parser, CReactionFrequencyTraitsArray& val)
{
    if (parser.SkipPastNextItemIfIs(".Set")) {
        if (!(parser.ReadAsSymbol() == "("))
            return;
        EOpinionReactionType reaction;
        CPersistTraits<EOpinionReactionType> reaction_traits;
        reaction_traits.TransferIn(persist_context, parser, reaction);
        if (!(parser.ReadAsSymbol() == ","))
            return;
        parser.SkipWhitespace();
        CCharString matchtype = parser.ReadAsStringUntilString("(");
        if (!(parser.ReadAsSymbol() == "("))
            return;
        if (matchtype == "Linear") {
            CPersistTraits<float> float_traits = CPersistTraits<float>();
            float min_wait;
            float max_wait;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, min_wait);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, max_wait);
            unsigned long constant_fps = CMainGameComponent::GetConstantFPS();
            val.LowHigh[0][reaction] = new CReactionFrequencyTraits_LinearTime(min_wait * constant_fps, max_wait * constant_fps);
            val.LowHigh[1][reaction] = new CReactionFrequencyTraits_LinearTime(min_wait * constant_fps * 0.2f, max_wait * constant_fps * 0.2f);
            if (!(parser.ReadAsSymbol() == ")"))
                return;
        } else if (matchtype == "PeriodicFrequency") {
            CPersistTraits<float> float_traits = CPersistTraits<float>();
            float ramp_up_time;
            float stay_high_time;
            float low_freq_per_min;
            float high_freq_per_min;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, ramp_up_time);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, stay_high_time);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, low_freq_per_min);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, high_freq_per_min);
            val.LowHigh[0][reaction] = new CReactionFrequencyTraits_PeriodicFrequency(ramp_up_time, stay_high_time, low_freq_per_min, high_freq_per_min);
            val.LowHigh[1][reaction] = new CReactionFrequencyTraits_PeriodicFrequency(ramp_up_time * 0.2f, stay_high_time * 0.2f, low_freq_per_min * 5.0f, high_freq_per_min * 2.0f);
            if (!(parser.ReadAsSymbol() == ")"))
                return;
        } else if (matchtype == "ControlledCount") {
            CPersistTraits<float> float_traits = CPersistTraits<float>();
            CPersistTraits<unsigned long> ulong_traits;
            unsigned long max_count;
            float min_gap_secs;
            float recharge_time_secs;
            parser.SkipWhitespace();
            ulong_traits.TransferIn(persist_context, parser, max_count);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, min_gap_secs);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            parser.SkipWhitespace();
            float_traits.TransferIn(persist_context, parser, recharge_time_secs);
            CCharString next_str = parser.ReadAsSymbol();
            if (next_str == ")") {
                val.LowHigh[0][reaction] = new CReactionFrequencyTraits_ControlledCount(max_count, min_gap_secs, recharge_time_secs, true);
                val.LowHigh[1][reaction] = new CReactionFrequencyTraits_ControlledCount(max_count * 5, min_gap_secs * 0.2f, recharge_time_secs * 0.2f, true);
            } else if (next_str == ",") {
                CPersistTraits<bool> bool_traits;
                bool allow_individual_repeats;
                parser.SkipWhitespace();
                bool_traits.TransferIn(persist_context, parser, allow_individual_repeats);
                val.LowHigh[0][reaction] = new CReactionFrequencyTraits_ControlledCount(max_count, min_gap_secs, recharge_time_secs, allow_individual_repeats);
                val.LowHigh[1][reaction] = new CReactionFrequencyTraits_ControlledCount(max_count * 5, min_gap_secs * 0.2f, recharge_time_secs * 0.2f, allow_individual_repeats);
                if (!(parser.ReadAsSymbol() == ")"))
                    return;
            }
        }
        parser.SkipWhitespace();
        parser.ReadAsSymbol() == ")";
    } else {
        persist_context.PersistError(parser, "Unknown command for TOpinionTransientOffsetList");
    }
}
