// CActivity::GetScoreAsString(float) const -- format the score to its configured precision and append the localized unit.
// Retail: 0x0055E6B0, 263 bytes. Leaf: GetScoreAsString
// No CActivity header exists. PDB: Name+0, Description+4, UnitOfMeasurement+8,
// NumDecPlacesToShow+c and LowerIsBetter+d. Shared fable_string.h supplies the strings
// (CWideString::Sprintf, FindCharPos and SetLength are its members).
// Calls: string ctor0099AED0/copy0099B720/dtor0099B510, assignment0099B7D0,
// append0099B940/0099B8D0; Sprintf0099BA70; GFFloatToWideString00486C40;
// FindCharPos0099B070, SetLength0099BC50; GetTextBySymbol009C95E0.
#ifndef FABLE_WIDE_STRING_HOST_TYPES
#include "fable_string.h"
#endif
inline CWideString IntToWideString(long value) {
    CWideString text;
    text.Sprintf(L"%d",value); //0099BA70 CWideString::Sprintf, L"%d" 0122DE44
    return text;
}
CWideString FABLE_FASTCALL GFFloatToWideString(double); //00486C40
namespace NGameText { class CDataBank { public: const CWideString GetTextBySymbol(const CCharString&) const; }; }
class CMainGameComponent { public: unsigned char beforeTextBank[0x14]; NGameText::CDataBank* PTextBank; NGameText::CDataBank* GetTextBank() const { return PTextBank; } };
extern CMainGameComponent* g_pMainGameComponent; //013B86A0
struct CActivity {
    CCharString Name,Description,UnitOfMeasurement;
    unsigned char NumDecPlacesToShow;
    bool LowerIsBetter;
    CWideString GetScoreAsString(float) const;
};
CWideString CActivity::GetScoreAsString(float score) const
{
    CWideString result;
    if (!NumDecPlacesToShow) result=IntToWideString((long)score);
    else {
        CWideString value=GFFloatToWideString(score);
        value.SetLength(value.FindCharPos(L'.')+1+NumDecPlacesToShow); //0099BC50, 0099B070
        result=value;
    }
    result+=L" "; //0122D820
    result+=g_pMainGameComponent->GetTextBank()->GetTextBySymbol(UnitOfMeasurement);
    return result;
}