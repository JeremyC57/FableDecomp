// CPersistTraits<std::pair<long, COpinionDeed*> >::TransferIn(CPersistContext& context, CStringParser& parser, std::pair<long, COpinionDeed*>& value) -- parse one "(key, deed)" pair of a text persist: the opening bracket, the long key, the comma, the opinion-deed reference, then the closing bracket
// Grade: DIFFER(314v312) -- inliner-budget split: retail expands CCharString::operator==(const char*) at all three symbol checks but calls the inner CBasicString<char>::operator==(const char*) 0x004115A0 out of line at the "," check only (0x006C93F8, result kept in a frame byte), so context stays in ebx; VC7.1 expands it at every site. No /d2inlS (3..20) or /d2inlT (20..100) value gives the outer-only split (T<=55 calls the outer operator instead)
// Leaf: ?TransferIn@?$CPersistTraits@U?$pair@JPAVCOpinionDeed@@@std@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAU?$pair@JPAVCOpinionDeed@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C9350, 312 bytes (+8 int3), thiscall `ret 0Ch` (traits `this` unused); speed TU.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?TransferIn@?$CPersistTraits@U?$pair@JPAVCOpinionDeed@@@std@@@@QAEXAAVCPersistContext@@AAVCStringParser@@...;
//   the body reads a long through CPersistTraitsSLONG::TransferIn 0x009E2590 into value.first (+0) and calls CPersistTraits<COpinionDeed*>::TransferIn
//   0x006C9490 on value.second (+4); its neighbour 0x006C9310 is CPersistContext_TransferableVector<pair<long,COpinionDeed*> >::TransferIn. The catalog
//   label CPersistTraits<CAtmosBankEntry>::TransferIn is propagated (that instance is a different body).
// Callees: CStringParser::ReadAsSymbol 0x009BA790 (x3), CBasicString<char>::Compare 0x00411570, CBasicString<char>::operator==(const char*) 0x004115A0,
//   ~CCharString 0x0099EAE0, CPersistTraitsSLONG::TransferIn 0x009E2590, CPersistTraits<COpinionDeed*>::TransferIn 0x006C9490.
// Literals: "(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020, "" 0x0122D70E.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CPERSISTTRAITS_PAIR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CStringParser.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"

template class CPersistTraits<std::pair<long, COpinionDeed*> >;
