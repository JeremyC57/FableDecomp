// CTCTavernGameCoinGolf::GetAdditionalInfo() -- the def's "additional info" text followed by "<course>/<courses>": the current course + 1 and the coin-golf def's course count
// Leaf: GetAdditionalInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D3700, 287 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameCoinGolf vtable 0x0126F49C slot 46 (Ego_r.pdb `?GetAdditionalInfo@CTCTavernGameCoinGolf@@UAE?AVCWideString@@XZ`;
// the manifest row says "vfunc_46").
// Callees: CTCTavernGame::GetDef(CDefPointer*) 0x005EA000; CDefClassBase::GetSubDef<CTavernGameCoinGolfDef>
//          (CDefPointer<const CTavernGameCoinGolfDef>*) 0x007D4BD0 (on the owner's def class, owner +0x70; its body names
//          "CTavernGameCoinGolfDef"); GFIntToCharString(long) 0x0099F830 (`__fastcall`, hidden result in ecx);
//          CCharString::ToWideString() const 0x0099E480; CTCTavernGame::GetText(unsigned long) 0x005E9E70;
//          operator+(const CWideString&, const CWideString&) 0x0099BE70 and operator+(const CWideString&, const wchar_t*)
//          0x0099BF30 (both `__fastcall`); ~CWideString 0x0099B510; ~CCharString 0x0099EAE0; the defs' slot 1 on release.
// Layout (retail): CTCBase::Owner +0x04, CThing def class +0x70; CTavernGameDef::AdditionalInfo +0xc0;
//          CTavernGameCoinGolfDef::Courses +0x50 (PDB offset holds); CTCTavernGameCoinGolf::CurrentCourse +0x17c (PDB +0x194).
// Shape: as the ShoveHaPenny override 0x007CFF40: VC evaluates the `+` chain right to left; the dereferenced sub-def's
//        release has no null test; the tavern def's dtor clears the field (`mov edx,edi ; test edx,edx`).
// NF49 (round 7): the string and def classes come from the shared headers (were local CWideString / CCharString / CRefCounted / CDefVirtual / CDefBase / CTavernGameDef / CTavernGameCoinGolfDef / CDefPointer / CCoinGolfDefPointer / CDefClassBase models
// and a same-TU stand-in GetSubDef body): the call at 0x007D4BD0 is the member template
// ??$GetSubDef@VCTavernGameCoinGolfDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameCoinGolfDef@@@@@Z (Ego_r.pdb / FableWin.pdb), its body visible
// and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10 idiom); GFIntToCharString / the CWideString
// operator+ overloads / CCharString::ToWideString are the fable_string.h / engine/fable_script_text.h declarations.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTAVERNGAMECOINGOLFDEF_SUBDEF
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/fable_script_text.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCoinGolfDef.h"
#include "engine/CDefClassBase_GetSubDef.h"

class CThing { public:
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* DefClass;                    // +0x70
};

class CTCTavernGame { public:
    virtual void v00();
    CThing*       Owner;                        // +0x04
    void GetDef(CDefPointer<const CTavernGameDef>* out);    // 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
    unsigned char _pad_0x08[0x174];
    long          CurrentCourse;                // +0x17c
};

class CTCTavernGameCoinGolf : public CTCTavernGame { public:
    CWideString GetAdditionalInfo();
};

CWideString CTCTavernGameCoinGolf::GetAdditionalInfo()
{
    CDefPointer<const CTavernGameDef> def;
    GetDef(&def);
    CDefPointer<const CTavernGameCoinGolfDef> golfDef;
    Owner->DefClass->GetSubDef(&golfDef);
    return GetText(def->AdditionalInfo) + GFIntToCharString(CurrentCourse + 1).ToWideString() + L"/"
         + GFIntToCharString(golfDef->Courses).ToWideString();
}