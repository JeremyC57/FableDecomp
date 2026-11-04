#ifndef FABLE_ENGINE_CDEFSTRING_H
#define FABLE_ENGINE_CDEFSTRING_H
// Ego_r TPI CDefString class, size4, private TablePos+0; native parent-def
// constructors initialise this member at +25 to -1.
class CCharString;
#ifdef FABLE_RETAIL_CDEFSTRING_TRANSFER
class CDataInputStream;
#endif
#ifdef FABLE_RETAIL_CDEFSTRING
class CWideString;
#endif
#ifndef FABLE_RETAIL_CDEFSTRINGTABLE
#error engine/CDefString.h needs the retail CDefStringTable: define FABLE_RETAIL_CDEFSTRINGTABLE (and FABLE_RETAIL_CCHARSTRING) before including it
#endif
#include "fable_definition_table.h"
class CDefString {
public:
 CDefString():TablePos(-1){}
 CDefString(const CCharString&);
#ifdef FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
 // Per-TU visibility opt-in (Codex pass 13, like FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE): the NScript TUs of 0x00EC5DE0 /
 // 0x00ECB0E0 call the out-of-line conversion 0x00415D70 (??BCDefString@@QBE?AVCCharString@@XZ) instead of expanding it.
 operator CCharString() const;
#else
 // Retail (landing 2026-11-03, chunks 718-725): the conversion forwards to GetString, both inline. Retail keeps two identical
 // out-of-line bodies (??B 0x00415D70, GetString 0x00415DD0 -- FableWin donor order ??B 0x018C20E0 before GetString 0x018C2120;
 // both `push [ecx]; call CDefStringTable::GetString 0x009D49B0`), a TU that inlines both expands the table call
 // (CTCShop::OnSerialise 0x00775C60), and the size-optimised CTCVillage TU inlines the forwarder and calls GetString
 // 0x00415DD0 (AddThingToVillage 0x0065B951..0x0065B95C, the conversion temporary at [ebp-4]).
 operator CCharString()const{return GetString();}
#endif
 CCharString GetString()const{return GDefStringTable.GetString(TablePos);}
#ifdef FABLE_RETAIL_CDEFSTRING
 // Chunk 732 (row 0x006AA2E0): Ego_r.pdb / FableWin.pdb ?ToWideString@CDefString@@QBE?AVCWideString@@XZ, out of line in retail
 // (0x00415D90: GetString through GDefStringTable, then CCharString::ToWideString 0x0099E480). Declaration only.
 CWideString ToWideString()const;
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_TRANSFER
 // Opt-in (chunk 765, row 0x005DBED0): Ego_r.pdb / FableWin.pdb ?TransferBinaryIn@CDefString@@QAEXAAVCDataInputStream@@@Z, out of line
 // in retail at 0x0044FC00 (the only PDB spelling of the four bound to it). Declaration only.
 void TransferBinaryIn(CDataInputStream& stream);                       // 0x0044FC00
#endif
private:
 long TablePos;
};
#endif
