// CTCCreatureOpinionOfHero::OnSerialise(CPersistContext& context, bool fullSave) -- persist the hero-opinion state: the flags and hit counters, then (full save only) the opinion values, the pending deeds as a "count,deed,..." string, LastFrameAppliedDeedEffect[0..91], the known-deed lists as "(index:count:,deed...)" groups, the transient offsets as a counted list, and the attitude / reaction / deed masks
// Grade: DIFFER(2826v2823) -- 122 difflines, same instruction sequence: frame-slot assignment (retail homes the known-deeds / item-name strings in the dead argument slots +0x80/+0x84 and the conditional-temporary flag at +0x14 set at entry), the LastFrame / COpinionTransientOffset default temporaries hoisted out of the loops in retail but rebuilt per iteration here, and the per-call-site inline split of Transfer<CCharString> 2-arg 0x00411010 / list ~ 0x004046B0
// Next idea: build the two GetDefaultVal temporaries once before the loops through a named-default Transfer overload in CPersistContext.h (3-arg with a hoisted const local was tried and is worse: 443), and try the load path's PendingDeedsList parser in its own scope with the save path's strings declared at function scope to move the slots
// Leaf: ?OnSerialise@CTCCreatureOpinionOfHero@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00684610, 2823 bytes, thiscall `ret 8`, virtual: CTCCreatureOpinionOfHero vtable 0x0125BC3C slot 1.
// Identity: the catalog's COpinionReactionManager::TransferOpinionData is a propagated label: the only reference to 0x00684610 is
//   .rdata slot 1 of vtable 0x0125BC3C (RTTI .?AVCTCCreatureOpinionOfHero@@, stored by the ctor 0x00684420); Ego_r.pdb
//   ?OnSerialise@CTCCreatureOpinionOfHero@@UAEXAAVCPersistContext@@_N@Z (donor 0x00592120); the members it transfers are the Ego_r
//   CTCCreatureOpinionOfHero members at their retail offsets (engine/CTCCreatureOpinionOfHero.h).
// Callees: CPersistContext::Transfer<bool> 0x004045C0 / <long> 0x00410BE0 / <float> 0x00410620 / <EOpinionAttitudeType> 0x00685CC0 /
//   <CCharString> 0x004109A0 (3-arg) and 0x00411010 (2-arg) / <COpinionTransientOffset> 0x00685F80, TransferVector<float> 0x00569FE0,
//   GFIntToCharString 0x0099F830, operator+ 0x0099F600 / 0x0099F690 / 0x0099F570, CCharString += 0x0099F0A0 / 0x0099F100,
//   operator const char* 0x0099E4C0, CStringParser(const CCharString&, ...) 0x00404720 / ReadAsInteger 0x009BA540 / ReadAsSymbol
//   0x009BA790, list<CStringParserCommentBoundaries>::~list 0x004046B0, ~CBaseClass 0x0099A2E0, CBasicString<char>::operator==
//   0x004115A0, malloc 0x00BFEA0E / free 0x00BFEA14, CTCCreatureOpinionOfHero::SerialiseFlagMask 0x00681BA0.
// Data: "," 0x0122E024, "(" 0x0122E028, ")" 0x0122E020, ":" 0x0124C528, "]" 0x01235430, "" 0x0122D70E.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SERIALISE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CPersistContext.h"
#include "engine/CStringParser.h"

CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830

template <> inline CCharString __fastcall CPersistContext::GetDefaultVal<CCharString>() { CCharString value; return value; }
template <> inline long __fastcall CPersistContext::GetDefaultVal<long>() { return 0; }
template <> inline COpinionTransientOffset __fastcall CPersistContext::GetDefaultVal<COpinionTransientOffset>() { return COpinionTransientOffset(); }

void CTCCreatureOpinionOfHero::OnSerialise(CPersistContext& context, bool fullSave)
{
    context.Transfer("InteractedFlag", InteractedFlag, false);
    context.Transfer("GreetedFlag", GreetedFlag, false);
    context.Transfer("LastOpinionReactionFrame", LastOpinionReactionFrame, 0L);
    context.Transfer("NumberOfTimesHit", NumberOfTimesHit, 0.0f);
    context.Transfer("ToleranceToBeingHitOverride", ToleranceToBeingHitOverride, -1.0f);
    context.Transfer("FrameToDecayNumberOfTimesHit", FrameToDecayNumberOfTimesHit, 0x7fffffffL);
    context.Transfer("ForcedAttitude", ForcedAttitude, (EOpinionAttitudeType)0x12);
    context.Transfer("HeroOpinionEnemy", HeroOpinionEnemy, false);
    if (fullSave)
        context.TransferVector("OpinionValues", RelativeOpinion.OpinionValues);
    if (context.IsSaving()) {
        if (!fullSave)
            return;
        CCharString pending = GFIntToCharString(PendingDeeds.size());
        for (std::list<long>::iterator it = PendingDeeds.begin(); it != PendingDeeds.end(); ++it)
            pending += "," + GFIntToCharString(*it);
        context.Transfer("PendingDeedsList", pending);
        for (long i = 0; i < 92; ++i) {
            CCharString name = "LastFrameAppliedDeedEffect[" + GFIntToCharString(i) + "]";
            context.Transfer(name.GetAsCharArray(), LastFrameAppliedDeedEffect[i]);
        }
        CCharString known("");
        for (long i = 0; i < 92; ++i) {
            if (!KnownDeeds[i].empty()) {
                known += "(" + GFIntToCharString(i) + ":";
                known += GFIntToCharString(KnownDeeds[i].size()) + ":";
                for (std::list<long>::iterator it = KnownDeeds[i].begin(); it != KnownDeeds[i].end(); ++it)
                    known += "," + GFIntToCharString(*it);
                known += ")";
            }
        }
        context.Transfer("KnownDeedsLists", known);
        CCharString name("TransientOffsets");
        long count = TransientOffsets.size();
        context.Transfer(name, count);
        long i = 0;
        for (std::list<COpinionTransientOffset>::iterator it = TransientOffsets.begin(); it != TransientOffsets.end(); ++it, ++i) {
            CCharString itemName = name + "" + GFIntToCharString(i) + "";
            context.Transfer(itemName.GetAsCharArray(), *it);
        }
    }
    else {
        if (!fullSave)
            return;
        CCharString pending("");
            context.Transfer("PendingDeedsList", pending);
            if (!pending.IsEmpty()) {
                CStringParser parser(pending, 0, CCharString(""), 0);
                long count = parser.ReadAsInteger();
                for (long i = 0; i < count; ++i) {
                    parser.ReadAsSymbol();
                    PendingDeeds.push_back(parser.ReadAsInteger());
                }
            }
        for (long i = 0; i < 92; ++i) {
            CCharString name = "LastFrameAppliedDeedEffect[" + GFIntToCharString(i) + "]";
            context.Transfer(name.GetAsCharArray(), LastFrameAppliedDeedEffect[i]);
        }
        {
            CCharString known("");
            context.Transfer("KnownDeedsLists", known);
            if (!known.IsEmpty()) {
                CStringParser parser(known, 0, CCharString(""), 0);
                while (!parser.AtEnd() && parser.ReadAsSymbol() == "(") {
                    long index = parser.ReadAsInteger();
                    parser.ReadAsSymbol();
                    long count = parser.ReadAsInteger();
                    parser.ReadAsSymbol();
                    for (long i = 0; i < count; ++i) {
                        parser.ReadAsSymbol();
                        KnownDeeds[index].push_back(parser.ReadAsInteger());
                    }
                    parser.ReadAsSymbol();
                }
            }
        }
            CCharString name("TransientOffsets");
            long count = 0;
            context.Transfer(name, count);
            for (long i = 0; i < count; ++i) {
                COpinionTransientOffset offset;
                CCharString itemName = name + "" + GFIntToCharString(i) + "";
                context.Transfer(itemName.GetAsCharArray(), offset);
                TransientOffsets.push_back(offset);
            }
    }
    SerialiseFlagMask(context, "AttitudeMask", AttitudeMask);
    SerialiseFlagMask(context, "ReactionMask", ReactionMask);
    SerialiseFlagMask(context, "DeedMask", DeedMask);
}
