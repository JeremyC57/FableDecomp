// CPersistTraits<COpinionDeed *>::TransferIn(CPersistContext &,CStringParser &,COpinionDeed * &) -- parse an opinion deed and transfer its persistence fields.
// Leaf: ?TransferIn@?$CPersistTraits@PAVCOpinionDeed@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAPAVCOpinionDeed@@@Z
// Grade: DIFFER(890v1128) -- inline budget: retail inlines CCharString::operator==(const char*) at every symbol test and the final CPersistTraits<long>::TransferIn; this build inlines the first three only
// Context: grades against the Codex row context $S/codex11_dl/x/rows/006c9490/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = retry11a probe p02; 006c9490 probes in probe/006c9490.log
#define FABLE_RETAIL_PERSIST_OPINION
#define FABLE_RETAIL_CSTRINGPARSER
#include <string.h>
#pragma intrinsic(strcmp)
#include "engine/COpinionDeed.h"
#include "engine/CBaseObjectPointer.h"
#include "engine/CStringParser.h"
#include "engine/CPersistTraits.h"
CCharString __fastcall operator+(const char*,const CCharString&);
#pragma auto_inline(off)
template<> void CPersistTraits<bool>::TransferIn(CPersistContext& persist_context,CStringParser& parser,bool& val)
{
    CCharString token=parser.ReadAsString();
    if (token.EqualsCaseInsensitive("TRUE") || token.EqualsCaseInsensitive("BTRUE")) val=true;
    else if (token.EqualsCaseInsensitive("FALSE") || token.EqualsCaseInsensitive("BFALSE")) val=false;
    else persist_context.PersistError(parser,"Unknown boolean value entry " + token);
}
#pragma auto_inline(on)
inline bool CCharString::operator==(const char* lpsz) const
{
    return PStringData ? PStringData->Data==lpsz : strcmp(lpsz,"")==0;
}
void CPersistTraits<COpinionDeed*>::TransferIn(CPersistContext& persist_context,CStringParser& parser,COpinionDeed*& val)
{
    CPersistTraits<long> slong_traits;
    CPersistTraits<bool> bool_traits;
    CPersistTraits<C3DVector> coord_traits;
    CPersistTraits<CIntelligentPointer<CThing> > ptr_traits;
    CPersistTraits<EOpinionDeedType> opinion_deed_type_traits;
    val=new COpinionDeed(OPINION_DEED_TYPE_NONE,0,C3DVector(0,0,0),0,false,false,0);
    if (parser.ReadAsSymbol()=="(") {
    opinion_deed_type_traits.TransferIn(persist_context,parser,val->DeedType);
    if (parser.ReadAsSymbol()==",") {
    slong_traits.TransferIn(persist_context,parser,val->DeedHandle);
    if (parser.ReadAsSymbol()==",") {
    coord_traits.TransferIn(persist_context,parser,val->Position);
    if (parser.ReadAsSymbol()==",") {
    slong_traits.TransferIn(persist_context,parser,val->FrameHappened);
    if (parser.ReadAsSymbol()==",") {
    bool_traits.TransferIn(persist_context,parser,val->IsStillHappening);
    val->IsStillHappening=false;
    if (parser.ReadAsSymbol()==",") {
    ptr_traits.TransferIn(persist_context,parser,reinterpret_cast<CIntelligentPointer<CThing>&>(val->PTarget));
    if (parser.ReadAsSymbol()==",") {
    bool_traits.TransferIn(persist_context,parser,val->IsCrimeReportedToVillage);
    if (parser.ReadAsSymbol()==",") {
    bool_traits.TransferIn(persist_context,parser,val->IsCrime);
    if (parser.ReadAsSymbol()==",") {
    slong_traits.TransferIn(persist_context,parser,val->ExtraData);
    if (parser.ReadAsSymbol()==")") { }
    }
    }
    }
    }
    }
    }
    }
    }
    }
}
