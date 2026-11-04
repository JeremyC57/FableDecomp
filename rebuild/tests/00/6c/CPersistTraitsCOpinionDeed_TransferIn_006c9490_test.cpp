// behaviour test for 0x006C9490 CPersistTraits<COpinionDeed*>::TransferIn (body copied from the row source)
// CPersistTraits<COpinionDeed*>::TransferIn(CPersistContext& context, CStringParser& parser, COpinionDeed*& value) -- text load of an opinion deed
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

// ---- test stubs: a scripted token stream ----
#include <stdio.h>
#include <stdlib.h>
struct FakeStringData { char* PData; unsigned long StringLength; unsigned long DataLength : 31; bool UseFastExtend : 1; long NoRefs; };
const char CCharString::NullChar = 0;
static const char* g_tokens[40]; static int g_pos; static int g_errors; static int g_targetReads; static C3DVector g_pos3;
static void Script(const char* const* t) { int k = 0; for (; t[k]; ++k) g_tokens[k] = t[k]; g_tokens[k] = 0; g_pos = 0; }
CCharString::CCharString(const char* text, long)
{
    FakeStringData* d = new FakeStringData;
    d->StringLength = (unsigned long)strlen(text); d->PData = new char[d->StringLength + 1]; strcpy(d->PData, text);
    d->DataLength = d->StringLength; d->UseFastExtend = false; d->NoRefs = 1;
    PStringData = (CCharStringData*)d;
}
CCharString::~CCharString() { }
static CCharString Next() { const char* t = g_tokens[g_pos]; if (t) ++g_pos; return CCharString(t ? t : ""); }
CCharString CStringParser::ReadAsSymbol() { return Next(); }
CCharString CStringParser::ReadAsString() { return Next(); }
void CPersistContext::PersistError(const CStringParser&, const CCharString&) const { ++g_errors; }
CCharString __fastcall operator+(const char* a, const CCharString& b) { return CCharString(a); }
void __fastcall CPersistTraitsSLONG::TransferIn(CPersistContext&, CStringParser&, long& value)
{ const char* t = g_tokens[g_pos]; if (t) ++g_pos; value = t ? atol(t) : 0; }
void CPersistTraits<C3DVector>::TransferIn(CPersistContext&, CStringParser&, C3DVector& v)
{ const char* t = g_tokens[g_pos]; if (t) ++g_pos; v = g_pos3; }
void CPersistTraits<CIntelligentPointer<CThing> >::TransferIn(CPersistContext&, CStringParser&, CIntelligentPointer<CThing>&)
{ const char* t = g_tokens[g_pos]; if (t) ++g_pos; ++g_targetReads; }
CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
COpinionDeed::COpinionDeed(EOpinionDeedType type, long handle, const C3DVector& position, const CThing*, bool stillHappening,
                           bool isCrime, long extraData)
    : DeedType(type), DeedHandle(handle), Position(position), FrameHappened(0), WasSearchingForWitnesses(false),
      IsStillHappening(stillHappening), IsCrimeReportedToVillage(false), IsCrime(isCrime), ExtraData(extraData) { }
int main()
{
    CPersistTraits<COpinionDeed*> traits;
    CPersistContext* context = (CPersistContext*)malloc(0x400); CStringParser* parser = (CStringParser*)malloc(0x100);
    COpinionDeed* deed = 0;
    bool ok = true;
    g_pos3.X = 1.0f; g_pos3.Y = 2.0f; g_pos3.Z = 3.0f;
    static const char* full[] = { "(", "9", ",", "1234", ",", "V", ",", "77", ",", "TRUE", ",", "T", ",", "btrue", ",",
                                  "FALSE", ",", "-5", ")", 0 };
    Script(full); traits.TransferIn(*context, *parser, deed);
    ok = ok && deed && deed->DeedType == (EOpinionDeedType)9 && deed->DeedHandle == 1234 && deed->Position.Y == 2.0f &&
         deed->FrameHappened == 77 && !deed->IsStillHappening && deed->IsCrimeReportedToVillage && !deed->IsCrime &&
         deed->ExtraData == -5 && g_targetReads == 1 && g_pos == 19 && g_errors == 0;
    static const char* part[] = { "(", "3", ";", "7", 0 };
    COpinionDeed* first = deed;
    Script(part); traits.TransferIn(*context, *parser, deed);
    ok = ok && deed != first && deed->DeedType == (EOpinionDeedType)3 && deed->DeedHandle == 0 && g_pos == 3;
    static const char* bad[] = { "(", "1", ",", "2", ",", "V", ",", "3", ",", "maybe", 0 };
    Script(bad); traits.TransferIn(*context, *parser, deed);
    ok = ok && g_errors == 1 && deed->FrameHappened == 3;
    printf(ok ? "006c9490_TEST PASS\n" : "006c9490_TEST FAIL\n");
    return ok ? 0 : 1;
}