// CUserProfileManager::GetManualSaveFileName @ 0x00406610
// Leaf: ?GetManualSaveFileName@CUserProfileManager@@QBE?AVCWideString@@J@Z
// Name-fix pass (2026-11-12): the return type is the shared CWideString (fable_string.h; Ego_r.pdb 0x00405270 / FableWin.pdb 0x0187B5B0
//   ?GetManualSaveFileName@CUserProfileManager@@QBE?AVCWideString@@J@Z) in place of the local 4-byte CWS stand-in.
#include "fable_string.h"

struct CWSraw {
    void* storage;
    void combine(const CWSraw& a);           // 0x99b720 (thiscall member)
    void fromLit(const unsigned short* lit);  // 0x99b6b0 (thiscall member)
};

// 4-byte wide-string returned via hidden pointer (memory return -> ret 8): CWideString.

// helper seams (relocations masked by harness)
extern "C" void __fastcall WSctor_0099aed0(CWSraw* self);                                  // 0x99aed0
extern "C" void __fastcall WSdtor_0099b510(CWSraw* self);                                  // 0x99b510
extern "C" void __cdecl   FmtWide_0099ba70(CWSraw* out, const unsigned short* fmt, long slot); // 0x99ba70
extern      CWideString __fastcall AddWide_0099be70(CWSraw* left, CWSraw* right);                       // 0x99be70

extern const unsigned short kFmtSave_0122de44[];    // 0x122de44
extern const unsigned short kSavePrefix_0122de90[]; // 0x122de90

// scoped raw wrapper whose destructor issues the retail dtor call
struct ScopedWS {
    CWSraw r;
    ~ScopedWS() { WSdtor_0099b510(&r); }
};

// CUserProfileManager: the shared engine/CUserProfileManager.h (SAVENAMES opt-in declares GetManualSaveFileName) in place of the local model.
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#include "engine/CUserProfileManager.h"

// member (thiscall): this in ECX, hidden return pointer as first stack arg.
CWideString CUserProfileManager::GetManualSaveFileName(long slot) const
{
    CWSraw a;
    WSctor_0099aed0(&a);
    FmtWide_0099ba70(&a, kFmtSave_0122de44, slot);

    ScopedWS combine;                       // localC (abs 0xc), right operand
    combine.r.combine(a);
    WSdtor_0099b510(&a);

    ScopedWS lit;                           // localB (abs 8), left operand
    lit.r.fromLit(kSavePrefix_0122de90);

    return AddWide_0099be70(&lit.r, &combine.r);
}