// CTCCreatureOpinionOfHero::OnSerialise(CPersistContext& context, bool fullSave) -- persist the hero-opinion state: the flags and hit counters, then (full save only) the opinion values, the pending deeds as a "count,deed,..." string, LastFrameAppliedDeedEffect[0..91], the known-deed lists as "(index:count:,deed...)" groups, the transient offsets as a counted list, and the attitude / reaction / deed masks
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
// Same-TU callee bodies (chunk 824): Transfer<long> 0x00410BE0 and Transfer<COpinionTransientOffset> 0x00685F80 are written out above
//   OnSerialise under `#pragma auto_inline(off)` (the landed rows' switch bodies, as explicit specializations). With their bodies visible
//   VC7.1 knows the default argument is only read, hoists the GetDefaultVal temporaries' stores out of the LastFrame / TransientOffsets
//   loops into the preheaders and homes the strings in the dead argument slots as retail does (122 -> 4 lines).
//   Header overlay: engine/CTCCreatureOpinionOfHero.h FABLE_RETAIL_COPINIONTRANSIENTOFFSET_BINARY (opt-in, TransferBinaryIn).
// Default temporaries (chunk 827): Ego_r.pdb names a user copy constructor ??0COpinionTransientOffset@@QAE@ABV0@@Z (Ego_r 0x0045DA29),
//   so the header opt-in FABLE_RETAIL_COPINIONTRANSIENTOFFSET_COPY declares it (member-wise init list). GetDefaultVal<COpinionTransientOffset>
//   builds a named local and returns it (no NRVO, as GetDefaultVal<CCharString>): the hoisted default is the copy of a body-order
//   constructed local, constant-propagated, i.e. one `xor eax,eax` and six memory-order stores (0x00684A46, 0x00684F37), while the load
//   loop's own `offset` local keeps the body-order default constructor (0x00684F5E..0x00684F7A).
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
#define FABLE_RETAIL_COPINIONTRANSIENTOFFSET_BINARY
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_COPINIONTRANSIENTOFFSET_COPY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CPersistContext.h"
#include "engine/CStringParser.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"

CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830

template <> inline CCharString __fastcall CPersistContext::GetDefaultVal<CCharString>() { CCharString value; return value; }
template <> inline long __fastcall CPersistContext::GetDefaultVal<long>() { return 0; }
template <> inline COpinionTransientOffset __fastcall CPersistContext::GetDefaultVal<COpinionTransientOffset>() { COpinionTransientOffset value; return value; }

#pragma auto_inline(off)
template <> void CPersistContext::Transfer<long>(const char* name, long& value, const long& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_CLEAR:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY:
        PSaveStream->WriteSLONG(value);
        break;
    case MODE_LOAD_BINARY: {
        CPersistTraits<long> traits;
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    }
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<long> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<long> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}
template <> void CPersistContext::Transfer<COpinionTransientOffset>(const char* name, COpinionTransientOffset& value, const COpinionTransientOffset& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_CLEAR:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY:
        PSaveStream->Write(&value, sizeof(COpinionTransientOffset));
        break;
    case MODE_LOAD_BINARY:
        value.TransferBinaryIn(*PLoadStream);
        break;
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<COpinionTransientOffset> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<COpinionTransientOffset> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}
#pragma auto_inline(on)

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