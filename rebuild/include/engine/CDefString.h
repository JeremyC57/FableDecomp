#ifndef FABLE_ENGINE_CDEFSTRING_H
#define FABLE_ENGINE_CDEFSTRING_H
#include "fable_compiler.h"
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
#ifdef FABLE_RETAIL_EXPRESSION_FRAME
 bool operator!=(const char*)const;
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_DEFAULTCTOR_OUTOFLINE
 // Opt-in (2026-11-05, row 0x007E37C0): the out-of-line COMDAT of the default ctor (Ego_r.pdb / FableWin.pdb ??0CDefString@@QAE@XZ;
 // `mov eax,ecx ; mov [eax],-1 ; ret`), called by the NScript TUs that build CPersistContext::Transfer<CDefString> defaults
 // (0x00F2EA95..0x00F2F55B). Declaration only; the row defines it.
 CDefString();
#else
 CDefString():TablePos(-1){}
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_CTOR_SETSTRING
 // Opt-in (wf2, row 0x009ADC90 CDefinitionManager::InstantiateDef): the CCharString constructor is inline in retail and its body is
 // one call of ?SetString@CDefString@@QAEXABVCCharString@@@Z 0x009D5060 (Ego_r.pdb / FableWin.pdb; landed), with no TablePos
 // store before it (0x009ADCF6: `push ecx ; mov ecx,esp ; push name ; call 0x009D5060` builds the by-value argument in place).
 void SetString(const CCharString& text);                               // 0x009D5060
 CDefString(const CCharString& text){SetString(text);}
#else
 CDefString(const CCharString&);
#endif
#if defined(FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION)
 // Per-TU inline opt-in (chunk 1010b, row 0x008A8C60 CGameScriptInterface::EntitySetAsSittingOnFloor): that TU expands the
 // conversion forwarder and calls GetString 0x00415DD0 out of line for an implicit conversion temporary (0x008A8D51..0x008A8D5E:
 // `lea edx,<tmp> ; push edx ; lea ecx,[ebp+8] ; call 0x00415DD0 ; lea eax,<tmp>`), where VC7.1's plain-inline heuristic
 // calls the forwarder's own COMDAT (??B 0x00415D70). Same body; GetString is declared only under this opt-in (below).
 FABLE_FORCEINLINE operator CCharString()const{return GetString();}
#elif defined(FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION)
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
#if defined(FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION)
#if defined(_MSC_VER)
 __declspec(noinline)
#endif
 CCharString GetString()const{return GDefStringTable.GetString(TablePos);}   // 0x00415DD0, called (not expanded) in that TU
#elif defined(FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING)
 // Per-TU inline opt-in (chunk 794, row 0x007039A0 CThingObject::OnHit): that TU expands GetString in place inside a conditional
 // full expression (0x00703F4B..0x00703F58: `mov ecx,[eax] ; push ecx ; push <result> ; mov ecx,offset GDefStringTable ; call
 // CDefStringTable::GetString 0x009D49B0`) where VC7.1's plain-inline heuristic calls the out-of-line copy 0x00415DD0. Same body.
 FABLE_FORCEINLINE CCharString GetString()const{return GDefStringTable.GetString(TablePos);}
#else
 CCharString GetString()const{return GDefStringTable.GetString(TablePos);}
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_GETASCHARARRAY
 // Opt-in (chunk 1017, row 0x008365F0 for_each<..., CDeadCreatureAnimPreloader>): Ego_r.pdb / FableWin.pdb
 // ?GetAsCharArray@CDefString@@QBEPBDXZ, inline in that TU (0x00836610: `mov eax,[ebx+8] ; push eax ; mov ecx,offset GDefStringTable ;
 // call 0x009D49F0`). Needs FABLE_RETAIL_CDEFSTRINGTABLE_ASCHARARRAY.
 const char* GetAsCharArray()const{return GDefStringTable.GetStringAsCharArray(TablePos);}
#endif
#ifdef FABLE_RETAIL_CDEFSTRING
 // Chunk 732 (row 0x006AA2E0): Ego_r.pdb / FableWin.pdb ?ToWideString@CDefString@@QBE?AVCWideString@@XZ, out of line in retail
 // (0x00415D90: GetString through GDefStringTable, then CCharString::ToWideString 0x0099E480). Declaration only.
 CWideString ToWideString()const;
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_EQUALS
 // Opt-in (chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): FableWin.pdb ??8CDefString@@QBE_NPBD@Z, out of
 // line in retail at 0x005B3440 (the def name returned by CThing::GetDefName compared with a literal). Declaration only.
 bool operator==(const char* text) const;                               // 0x005B3440
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_SETDEFAULTSTRING
 // Retail opt-in (chunk 1025, row 0x0062FE60 NUISystem::CUIDef::CUIDef): the PDB symbol in ghidra_out (function table)
 // ?SetDefaultString@CDefString@@QAEXABVCCharString@@@Z, out of line in retail at 0x009D5080. Declaration only.
 void SetDefaultString(const CCharString& text);                        // 0x009D5080
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_TRANSFER
 // Opt-in (chunk 765, row 0x005DBED0): Ego_r.pdb / FableWin.pdb ?TransferBinaryIn@CDefString@@QAEXAAVCDataInputStream@@@Z, out of line
 // in retail at 0x0044FC00 (the only PDB spelling of the four bound to it). Declaration only.
 void TransferBinaryIn(CDataInputStream& stream);                       // 0x0044FC00
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_NE_CHARSTRING
 // Opt-in (wf14, row 0x0076C9A0 CTCEnemy::OnSerialise): Ego_r.pdb ??9CDefString@@QBE_NABVCCharString@@@Z (0x0077B970), retail
 // 0x0076D640 (out of line, emitted in the CTCEnemy TU). Declaration only.
 bool operator!=(const CCharString& text) const;                       // 0x0076D640
#endif
private:
 long TablePos;
};
#endif
