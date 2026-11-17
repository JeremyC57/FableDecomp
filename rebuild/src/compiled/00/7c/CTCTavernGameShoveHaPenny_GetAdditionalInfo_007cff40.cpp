// CTCTavernGameShoveHaPenny::GetAdditionalInfo() -- the def's "additional info" text followed by "<round>/<rounds>": the current round (rounds played + 1) and the shove-ha'penny def's round count
// Leaf: GetAdditionalInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CFF40, 290 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameShoveHaPenny vtable 0x0126F2F4 slot 46 (Ego_r.pdb `?GetAdditionalInfo@CTCTavernGameShoveHaPenny@@UAE?AVCWideString@@XZ`).
// The manifest row says "CTCTavernGameCoinGolf::GetAdditionalInfo" (propagated; the CoinGolf override is 0x007D3700).
// Callees: CTCTavernGame::GetDef(CDefPointer*) 0x005EA000; CDefClassBase::GetSubDef<CTavernGameShoveHaPennyDef>
//          (CDefPointer<const CTavernGameShoveHaPennyDef>*) 0x007D1260 (on the owner's def class, owner +0x70; manifest
//          label "<CAICreatureWillPowerIndicatorDef>" is propagated, the landed body names the ShoveHaPenny def string);
//          GFIntToCharString(long) 0x0099F830 (`__fastcall`, hidden result in ecx); CCharString::ToWideString() const
//          0x0099E480; CTCTavernGame::GetText(unsigned long) 0x005E9E70; operator+(const CWideString&, const CWideString&)
//          0x0099BE70 and operator+(const CWideString&, const wchar_t*) 0x0099BF30 (both `__fastcall`); ~CWideString
//          0x0099B510; ~CCharString 0x0099EAE0; the defs' virtual slot 1 on release.
// Layout (retail): CTCBase::Owner +0x04, CThing def class +0x70; CTavernGameDef::AdditionalInfo +0xc0;
//          CTavernGameShoveHaPennyDef::Rounds +0x1fc (PDB +0x204, the Regions vector is 12 bytes in retail);
//          CTCTavernGameShoveHaPenny::RoundsPlayed +0x148 (as the landed EnterGame 0x007CF530).
// Shape: VC evaluates the `+` chain right to left (the rounds total is formatted first); the typed sub-def pointer is
//        dereferenced before its dtor, so its release (`dec [edi+4] ; jne`) has no null test; the tavern def's dtor
//        clears the field after the release (`mov edx,edi ; test edx,edx`, the dead store elided).
// NF49 (round 7): the string and def classes come from the shared headers (were local CWideString / CCharString / CRefCounted / CDefVirtual / CDefBase / CTavernGameDef / CTavernGameShoveHaPennyDef / CDefPointer / CShoveHaPennyDefPointer / CDefClassBase models
// and a same-TU stand-in GetSubDef body): the call at 0x007D1260 is the member template
// ??$GetSubDef@VCTavernGameShoveHaPennyDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameShoveHaPennyDef@@@@@Z (Ego_r.pdb / FableWin.pdb), its body visible
// and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10 idiom); GFIntToCharString / the CWideString
// operator+ overloads / CCharString::ToWideString are the fable_string.h / engine/fable_script_text.h declarations.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/fable_script_text.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameShoveHaPennyDef.h"
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
    unsigned char _pad_0x08[0x140];
    long          RoundsPlayed;                 // +0x148
};

struct CTCTavernGameShoveHaPenny : CTCTavernGame {
    CWideString GetAdditionalInfo();
};

CWideString CTCTavernGameShoveHaPenny::GetAdditionalInfo()
{
    CDefPointer<const CTavernGameDef> def;
    GetDef(&def);
    CDefPointer<const CTavernGameShoveHaPennyDef> shoveDef;
    Owner->DefClass->GetSubDef(&shoveDef);
    return GetText(def->AdditionalInfo) + GFIntToCharString(RoundsPlayed + 1).ToWideString() + L"/"
         + GFIntToCharString(shoveDef->Rounds).ToWideString();
}