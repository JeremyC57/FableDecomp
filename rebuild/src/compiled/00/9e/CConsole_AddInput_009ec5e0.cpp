// CConsole::AddInput(CConsoleInputBase*) -- file the input under a lowercase copy of its name, shared through a counted pointer
// Leaf: ?AddInput@CConsole@@QAEXPAVCConsoleInputBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EC5E0, 296 bytes. Ego_r.pdb 0x00B3FB60 / FableWin.pdb 0x030AC1C0 ?AddInput@CConsole@@QAEXPAVCConsoleInputBase@@@Z.
// Callees: CCharString::GetAsLowerCase 0x0099ED10, CCharString(const CCharString&) 0x0099EC30, ~CCharString 0x0099EAE0, operator new,
//   CCountedPointer<CConsoleInputBase>::DeleteData 0x009EDC50 (stored in the count block), map<CCharString, CCountedPointer<CConsoleInputBase> >
//   insert = _Rb_tree::insert_unique 0x009EFB80 (out of line), operator delete.
// Layout: Ego_r.pdb CConsole::RegisteredConsoleInputs (map<CCharString, CCountedPointer<CConsoleInputBase> >) at retail +0x40 (12-byte STLport
//   maps; Ego_r +0x48), CConsoleInputBase::Name +4 after the vptr. Neither class has a shared header; both are modelled here with only the
//   members this body touches (CConsole opaque before RegisteredConsoleInputs).
// Port (NF78, round 7; q10762 binder, NF70 blocked/port): the map, CCountedPointer and CCharString come from stlport/fable_map.h,
//   engine/CCountedPointer.h and fable_string.h (FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE); the local CInputMap / CInputPointer /
//   SInputCountBlock / CRetailString stand-ins are gone.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"

class CConsoleInputBase {
public:
    virtual ~CConsoleInputBase();
    CCharString Name;                                                   // +0x04
};
class CConsole {
public:
    void AddInput(CConsoleInputBase* input);
private:
    unsigned char _pad00[0x40];
    std::map<CCharString, CCountedPointer<CConsoleInputBase> > RegisteredConsoleInputs;   // +0x40
};

void CConsole::AddInput(CConsoleInputBase* input)
{
    CCountedPointer<CConsoleInputBase> pointer(input);
    RegisteredConsoleInputs.insert(std::make_pair(input->Name.GetAsLowerCase(), pointer));
}
