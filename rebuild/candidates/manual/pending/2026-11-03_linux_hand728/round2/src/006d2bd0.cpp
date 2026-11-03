// CTCWife::OnSerialise(CPersistContext& context, bool in_game) -- save or restore the wife's courtship / marriage state: the scalar members with their defaults, the four husband-appearance flags by indexed name, and the two def-index containers as def names
// (ThingsSeenThePlayerWear as vector<CCharString>, GiftsReceived as vector<pair<CCharString,long> >)
// Leaf: ?OnSerialise@CTCWife@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D2BD0, 1770 bytes, `ret 8`; CTCWife vtable 0x0125ED5C slot 1 (the CTCBase OnSerialise slot;
//   every transfer name is the CTCWife member it addresses). The catalog's "CThingNPC::TransferLoveState" is a descriptive label.
// Callees: CPersistContext::Transfer<bool> 0x004045C0 / Transfer<long> 0x00410BE0 / Transfer<float> 0x00410620,
//   TransferVector<CCharString> 0x0049B8D0, TransferVector<pair<CCharString,long> > 0x006D4C00; GFIntToCharString 0x0099F830,
//   operator+(const char*, const CCharString&) 0x0099F690, operator+(const CCharString&, const char*) 0x0099F600,
//   CCharString::operator const char* 0x0099E4C0, CCharString(const CCharString&) 0x0099EC30, ~CCharString 0x0099EAE0;
//   CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDefNameFromGlobalIndex 0x009ACCC0 / GetDefGlobalIndexFromName
//   0x009AD410, CDefStringTable::GetString 0x009D49B0 on GDefStringTable 0x013CA828 (the inline CDefString -> CCharString conversion);
//   vector<CCharString>::reserve 0x004BA7A0 / _M_insert_overflow 0x00433530, vector<pair<CCharString,long> >::reserve 0x006D4680 /
//   _M_insert_overflow 0x006D4750; _Rb_tree<long,long,..>::insert_unique 0x00512DA0, _Rb_tree<long,pair<const long,long>,..>::
//   insert_unique 0x004E539F; free 0x00BFEA14.
// Shape (chunk 728 rounds 1-2): cached end iterators; the two-argument Transfer(name, value) forwarding inline to the three-argument
//   Transfer with GetDefaultVal<T>() (VC7.1 then builds the CCharString name before pushing the default, the value and the name).
//   Round 2: retail stores the BooleanHusbandAppearances loop's `false` default once, before the loop (`mov [esp+0Fh],bl`), because
//   the three-argument CPersistContext::Transfer<T> template body is instantiated in this TU (as in retail, where CTCWife.cpp
//   includes the template definition): with the body visible VC7.1 sees that the callee only reads its `const T&` default, so the
//   loop-invariant temporary is stored once. A declaration-only Transfer re-stores it every iteration (round 1's dl=8 residue).
//   The body is the landed Transfer<bool> 0x004045C0 / Transfer<long> 0x00410BE0 / Transfer<float> 0x00410620 template, with the
//   CPersistTraits<bool> / <float> TransferBinaryOut specialisations (WriteEBOOL 0x00993E30 / WriteFloat 0x00993EE0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "fable_string.h"
#include "engine/CTCWife.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefString.h"
#include "engine/CPersistContext.h"
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CDataOutputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
// The inline two-argument Transfer and the inline GetDefaultVal<T>() body (both declared in engine/CPersistContext.h; FableWin.pdb
// ??$Transfer@_N@CPersistContext@@QAEXPBDAA_N@Z 0x018951A0 / ??$GetDefaultVal@_N@CPersistContext@@KI_NXZ 0x0189C330; no retail copy):
// defined here as the landed NScript::CQ_SunnyvaleMasterData::OnPersist 0x00CDC070 does.
template<class T> inline void CPersistContext::Transfer(const char* name,T& value) { Transfer(name,value,GetDefaultVal<T>()); }
template<class T> inline T FABLE_FASTCALL CPersistContext::GetDefaultVal() { return T(); }
template<> inline void CPersistTraits<bool>::TransferBinaryOut(CDataOutputStream& stream,bool value) { stream.WriteEBOOL(value); }
template<> inline void CPersistTraits<float>::TransferBinaryOut(CDataOutputStream& stream,float value) { stream.WriteFloat(value); }
template<class T> void CPersistContext::Transfer(const char* name,T& value,const T& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_CLEAR:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY: {
        CPersistTraits<T> traits;
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
    case MODE_LOAD_BINARY: {
        CPersistTraits<T> traits;
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    }
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}

CCharString __fastcall GFIntToCharString(long value);                       // 0x0099F830 ?GFIntToCharString@@YI?AVCCharString@@J@Z
CCharString __fastcall operator+(const char* left, const CCharString& right);  // 0x0099F690 ??H@YI?AVCCharString@@PBDABV0@@Z

void CTCWife::OnSerialise(CPersistContext& context, bool in_game)
{
    context.Transfer("CourtingBlocked", CourtingBlocked, false);
    context.Transfer("PermittedToRegionFollow", PermittedToRegionFollow, false);
    context.Transfer("FrameGotMarriedToThePlayer", FrameGotMarriedToThePlayer, 0x7fffffffL);
    context.Transfer("DivorcedHero", DivorcedHero, false);
    context.Transfer("JustMarried", JustMarried, false);
    context.Transfer("NeedsToChangeBrain", NeedsToChangeBrain, false);
    context.Transfer("FrameToCheckAppearanceChanges", FrameToCheckAppearanceChanges, 0L);
    context.Transfer("FrameLastAwareOfHusband", FrameLastAwareOfHusband, 0L);
    context.Transfer("FrameLastReducedOpinion", FrameLastReducedOpinion, 0L);
    context.Transfer("FrameLastEvaluatedGiftOpinion", FrameLastEvaluatedGiftOpinion, 0L);
    context.Transfer("FrameLastConsideredGivingGift", FrameLastConsideredGivingGift, 0L);
    context.Transfer("FrameLastEvaluatedLoveAttitude", FrameLastEvaluatedLoveAttitude, 0L);
    context.Transfer("FrameEnteredAttitudeHate", FrameEnteredAttitudeHate, 0x7fffffffL);
    context.Transfer("FrameLastGaveDivorceWarning", FrameLastGaveDivorceWarning, 0L);
    context.Transfer("FrameEnteredLoveWithHusbandPresentAtHome", FrameEnteredLoveWithHusbandPresentAtHome, 0x7fffffffL);
    context.Transfer("FrameLastGaveSexOffer", FrameLastGaveSexOffer, 0L);
    context.Transfer("GiftGivingOpinionDistanceFromMax", GiftGivingOpinionDistanceFromMax, 0.0f);
    context.Transfer("GiftGivingPriceValue", GiftGivingPriceValue, 500L);
    context.Transfer("GiftToGiveDef", GiftToGiveDef, 0L);
    context.Transfer("LastFatnessChangePoint", LastFatnessChangePoint, 0.0f);
    context.Transfer("HouseDressingLevelLastCommentedOn", HouseDressingLevelLastCommentedOn, 0L);
    for (long i = 0; i < 4; ++i)
        context.Transfer("BooleanHusbandAppearances[" + GFIntToCharString(i) + "]", BooleanHusbandAppearances[i]);
    context.Transfer("FrameLastReceivedNiceGift", FrameLastReceivedNiceGift, 0L);
    context.Transfer("FrameLastCulledGiftsReceived", FrameLastCulledGiftsReceived, 0L);
    context.Transfer("LoveAttitudeValue", LoveAttitudeValue, 0.0f);
    context.Transfer("HasBeenInLoveWithPlayer", HasBeenInLoveWithPlayer, false);
    context.Transfer("ReceivedWeddingRing", ReceivedWeddingRing, false);
    if (context.IsSaving()) {
        std::vector<CCharString> things_seen;
        things_seen.reserve(ThingsSeenThePlayerWear.size());
        std::set<long>::iterator end_it = ThingsSeenThePlayerWear.end();
        for (std::set<long>::iterator it = ThingsSeenThePlayerWear.begin(); it != end_it; ++it)
            things_seen.push_back(GetDefinitionManager().GetDefNameFromGlobalIndex(*it));
        context.TransferVector("ThingsSeenThePlayerWear", things_seen);
        std::vector<std::pair<CCharString,long> > gifts;
        gifts.reserve(GiftsReceived.size());
        std::map<long,long>::iterator gift_end_it = GiftsReceived.end();
        for (std::map<long,long>::iterator it = GiftsReceived.begin(); it != gift_end_it; ++it)
            if ((*it).first > 0)
                gifts.push_back(std::make_pair(GetDefinitionManager().GetDefNameFromGlobalIndex((*it).first), (*it).second));
        context.TransferVector("GiftsReceived", gifts);
    } else {
        std::vector<CCharString> things_seen;
        context.TransferVector("ThingsSeenThePlayerWear", things_seen);
        std::vector<CCharString>::iterator end_it = things_seen.end();
        for (std::vector<CCharString>::iterator it = things_seen.begin(); it != end_it; ++it)
            ThingsSeenThePlayerWear.insert(GetDefinitionManager().GetDefGlobalIndexFromName(*it));
        std::vector<std::pair<CCharString,long> > gifts;
        context.TransferVector("GiftsReceived", gifts);
        std::vector<std::pair<CCharString,long> >::iterator gift_end_it = gifts.end();
        for (std::vector<std::pair<CCharString,long> >::iterator it = gifts.begin(); it != gift_end_it; ++it)
            GiftsReceived.insert(std::make_pair(GetDefinitionManager().GetDefGlobalIndexFromName((*it).first), (*it).second));
    }
}
