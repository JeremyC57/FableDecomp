// 00e7fd50 NScript::CV_FishingCompetitionScript::GenerateSignText
// Retail-asm-helper: GFFloatToLongNear via GFRoundFloatNear.
// Retail-asm: 0x00e7fe5d rounds the scaled fish weight with a dword x87 store.
#define FABLE_RETAIL_STLP_FALSE_TAG_POD
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#define FABLE_RETAIL_BEARDY_ORIGINAL_TEXT_COMPARISON_PASS27
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY

// Leaf: ?GenerateSignText@CV_FishingCompetitionScript@NScript@@QAEXXZ
// Flags: /Gr
#define FABLE_RETAIL_FISHING_SIGN_PASS29
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#include <string.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/GFPackFloat.h"
inline CScriptThing::~CScriptThing() {}

CWideString __fastcall GFFloatToWideString(double value);   // 0x00486C40
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/NScript_CV_FishingCompetitionScript.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
namespace std {
template<> inline void _Destroy(CWideString* p) {
#ifdef _MSC_VER
// Inline-budget: preserve the ordinary element cleanup in the out-of-line vector destructor.
#pragma inline_depth(5)
#endif
 p->~CWideString();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
}
}
namespace std {
// Inline-budget: the retail vector base constructor is called as its 15-byte physical owner.
template<>
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline _Vector_base<CWideString,allocator<CWideString> >::_Vector_base(const allocator<CWideString>& a): _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {} }
namespace std {
// Inline-budget: the sign expands each push, with a call to the original overflow instantiation on the slow path.
template<> FABLE_FORCEINLINE void vector<CWideString>::push_back(const CWideString& x)
{
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
 if(this->_M_finish!=this->_M_end_of_storage._M_data){
  _Construct(this->_M_finish,x);
  ++this->_M_finish;
 } else {
     const _IsPODType tag = _IsPODType();
     _M_insert_overflow(this->_M_finish, x, tag, 1, true);
 }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
}
}

namespace std {
// Inline-budget: the original 49-byte destructor expands range destruction before releasing its base.
template<> inline vector<CWideString>::~vector() {
 _Destroy(this->_M_start,this->_M_finish);
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
}
namespace NScript {
inline float CV_FishingCompetitionScript::GetNextPrizeCategoryWeight()
{
    long next = PrizeNumber + 1;
    if (PrizeNumber == -1)
        ++next;
    return pGlobalScriptDef->FishPrizeWeights[next];
}
void CV_FishingCompetitionScript::GenerateSignText()
{
    CScriptThing Sign = Game.GetThingWithScriptName("FishCompSign");
    CWideString WinnerTitle = Game.GetTextString(Winner);
    if (Winner == "HERO")
        WinnerTitle = Game.GetTextString(GetHeroTitleTextTag());
    float NextPrizeCategory = GetNextPrizeCategoryWeight();
    std::vector<CWideString> values;
    values.push_back(GFFloatToWideString(GFRoundFloatNear(BiggestCompetitorFish, 2)));
    values.push_back(WinnerTitle);
    CWideString CompetitionText;
    if (CompetitionWon)
        CompetitionText = Game.GetFormattedString("TEXT_QST_B15_FISH_SIGN_INFO_HEROWON", values);
    else {
        values.push_back(GFFloatToWideString(NextPrizeCategory));
        CompetitionText = Game.GetFormattedString("TEXT_QST_B15_FISH_SIGN_INFO_NORMAL", values);
    }
    Game.SetReadableObjectText(Sign, CompetitionText);
}
}