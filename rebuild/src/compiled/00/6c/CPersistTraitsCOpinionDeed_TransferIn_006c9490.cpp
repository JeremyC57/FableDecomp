// CPersistTraits<COpinionDeed*>::TransferIn(CPersistContext& context, CStringParser& parser, COpinionDeed*& value) -- text load of an opinion deed: allocate a default deed, then "(type,handle,position,frame,stillHappening,target,reported,crime,extra)"
// Leaf: ?TransferIn@?$CPersistTraits@PAVCOpinionDeed@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAPAVCOpinionDeed@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C9490, 1128 bytes, `ret 0Ch` (traits `this` unused).
// Identity: Ego_r.pdb / FableWin.pdb ?TransferIn@?$CPersistTraits@PAVCOpinionDeed@@@@QAEXAAVCPersistContext@@AAVCStringParser@@
//   AAPAVCOpinionDeed@@@Z (the CPersistTraits<COpinionDeed*> declaration of engine/CPersistTraits.h); its binary twin is landed
//   0x006C88D0 (same deed construction, same field order). The catalog's `_global::COpinionDeed_TransferIn` is an autoname.
// Layout: Ego_r.pdb COpinionDeed (0x28): DeedType +0, DeedHandle +4, Position +8, FrameHappened +0x14, PTarget
//   (CIntelligentPointer<const CThing>) +0x18, WasSearchingForWitnesses +0x20 (not saved), IsStillHappening +0x21 (read, then
//   cleared), IsCrimeReportedToVillage +0x22, IsCrime +0x23, ExtraData +0x24.
// Callees: operator new 0x00BFEA1A, COpinionDeed::COpinionDeed 0x006C54C0, CBaseIntelligentPointer() 0x00A01B10 /
//   ~CBaseIntelligentPointer 0x00A01C10 (the pointer traits' DefaultVal, vtable 0x01238C6C), CStringParser::ReadAsSymbol
//   0x009BA790, CBasicString<char>::operator==(const char*) 0x004115A0, ~CCharString 0x0099EAE0, CPersistTraitsSLONG::
//   TransferIn 0x009E2590, CPersistTraits<C3DVector>::TransferIn 0x009E84D0, CPersistTraits<bool>::TransferIn 0x00403FF0,
//   CPersistTraits<CIntelligentPointer<CThing> >::TransferIn 0x006C9900 (landed spellings).
// Shape: the C3DVector and pointer traits are function-scope locals (frame +0x14 / +0); the bool reader's body is in the TU
//   (retail's copy is the early header COMDAT 0x00403FF0, called out of line): VC7.1 then sees the traits `this` unused, so the
//   bool traits object takes no frame slot and is not zero-initialised. The chain is `if (!(sym == ",")) return;` with every
//   exit through the pointer traits' destructor. The first three integer reads are the generic inline traits reader; VC7.1
//   calls the generic reader out of line for the last one in every spelling of the chain tried (nested, early returns,
//   named traits, int/long instance), where retail expands it: ExtraData is written as a reference bound before the
//   SLONG reader (the same bytes as the expansion) -- the original spelling at that site is unproven.
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CIntelligentPointer.h"
#include "engine/EOpinionDeedType.h"

template<class T> inline long __fastcall CBasicString<T>::Compare(const T* a,const T* b)
{
    while(*a || *b) {
        if(*a<*b)return -1;
        if(*a>*b)return 1;
        ++a; ++b;
    }
    return 0;
}
template<class T> inline bool CBasicString<T>::operator==(const T* text) const
{ return Compare(PData,text)==0; }

// The bool reader's body is header-inline in retail (an early COMDAT, 0x00403FF0, called out of line by every TU): visible
// here, VC7.1 sees that the traits `this` is unused and drops the value-initialisation of the temporary traits object.
template<> inline void CPersistTraits<bool>::TransferIn(CPersistContext& context, CStringParser& parser, bool& value)
{
    CCharString token = parser.ReadAsString();
    if (token.EqualsCaseInsensitive("TRUE") || token.EqualsCaseInsensitive("BTRUE"))
        value = true;
    else if (token.EqualsCaseInsensitive("FALSE") || token.EqualsCaseInsensitive("BFALSE"))
        value = false;
    else
        context.PersistError(parser, "Unknown boolean value entry " + token);
}

void* __cdecl operator new(unsigned int size);        // 0x00BFEA1A
class CThing;
class COpinionDeed {                                   // Ego_r.pdb, 0x28 bytes
public:
    EOpinionDeedType DeedType;                         // +0x00
    long             DeedHandle;                       // +0x04
    C3DVector        Position;                         // +0x08
    long             FrameHappened;                    // +0x14
    CIntelligentPointer<const CThing> PTarget;         // +0x18
    bool             WasSearchingForWitnesses;         // +0x20
    bool             IsStillHappening;                 // +0x21
    bool             IsCrimeReportedToVillage;         // +0x22
    bool             IsCrime;                          // +0x23
    long             ExtraData;                        // +0x24
    COpinionDeed(EOpinionDeedType type, long handle, const C3DVector& position, const CThing* target,
                 bool stillHappening, bool isCrime, long extraData);   // 0x006C54C0
};
template<> class CPersistTraits<CIntelligentPointer<CThing> > {
public:
    CIntelligentPointer<CThing> DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, CIntelligentPointer<CThing>& value);   // 0x006C9900
};

void CPersistTraits<COpinionDeed*>::TransferIn(CPersistContext& context, CStringParser& parser, COpinionDeed*& value)
{
    CPersistTraits<C3DVector> vector_traits;
    CPersistTraits<CIntelligentPointer<CThing> > pointer_traits;
    CPersistTraits<bool> bool_traits;
    value = new COpinionDeed(OPINION_DEED_TYPE_NONE, 0, C3DVector(0.0f, 0.0f, 0.0f), 0, false, false, 0);
    if (!(parser.ReadAsSymbol() == "("))
        return;
    CPersistTraits<EOpinionDeedType>().TransferIn(context, parser, value->DeedType);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    CPersistTraits<long>().TransferIn(context, parser, value->DeedHandle);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    vector_traits.TransferIn(context, parser, value->Position);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    CPersistTraits<long>().TransferIn(context, parser, value->FrameHappened);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    bool_traits.TransferIn(context, parser, value->IsStillHappening);
    value->IsStillHappening = false;
    if (!(parser.ReadAsSymbol() == ","))
        return;
    pointer_traits.TransferIn(context, parser, (CIntelligentPointer<CThing>&)value->PTarget);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    bool_traits.TransferIn(context, parser, value->IsCrimeReportedToVillage);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    bool_traits.TransferIn(context, parser, value->IsCrime);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    long& extra = value->ExtraData;
    extra = CPersistTraitsSLONG::TransferIn(context, parser);
    if (parser.ReadAsSymbol() == ")")
        return;
}