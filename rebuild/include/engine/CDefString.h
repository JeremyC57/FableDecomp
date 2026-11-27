#ifndef FABLE_ENGINE_CDEFSTRING_H
#define FABLE_ENGINE_CDEFSTRING_H
#include "fable_compiler.h"
// Ego_r TPI CDefString class, size4, private TablePos+0; native parent-def
// constructors initialise this member at +25 to -1.
#ifdef FABLE_RETAIL_CCREATUREDEF_TRANSFER
#include "engine/CDataOutputStream.h"
#endif
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
#ifdef FABLE_RETAIL_CCREATUREDEF_TRANSFER
    void TransferBinaryOut(CDataOutputStream& os) const { os.WriteSLONG(TablePos); }
#endif
#ifdef FABLE_RETAIL_EXPRESSION_FRAME
 bool operator!=(const char*)const;
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_NE_DEFSTRING
 // Opt-in (egod e07, row 0x00681E60): FableWin.pdb ??9CDefString@@QBE_NABV0@@Z (0x01D95D70), inline in retail (0x00681E9D: mov ecx,[eax] then cmp ecx,[ebp] compares the two TablePos words). Body only.
 bool operator!=(const CDefString& other)const{return TablePos!=other.TablePos;}
#endif
#if defined(FABLE_RETAIL_CDEFSTRING_NOTEQUAL) && !defined(FABLE_RETAIL_EXPRESSION_FRAME)
 // Opt-in (chunk 2026, row 0x0092DA20 CAIStateGroup_MinionSleep::Update): Ego_r.pdb / FableWin.pdb ??9CDefString@@QBE_NPBD@Z, out of line
 // in retail at 0x005FA740 (`push offset "" ; mov ecx,<def+34h> ; call`, 0x0092DF45; already bound by that spelling). Declaration only.
 bool operator!=(const char* text)const;                                // 0x005FA740
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
// Binding (owner decision 2026-11-19, "Swap and re-land"; hand1 lane, Fable advisor fable3): retail keeps two identical out-of-line bodies
// (`push [ecx]; call CDefStringTable::GetString 0x009D49B0`): GetString 0x00415D70 and the conversion ??B 0x00415DD0. VC7.1 /O2 emits the
// inline callee first (FableWin.pdb, /Od, keeps the declaration order: ??B 0x018C20E0 before GetString 0x018C2120). The NScript TUs call
// GetString 0x00415D70 behind an expanded conversion; the game TUs call the conversion 0x00415DD0 itself for an implicit conversion
// temporary (CTCVillage::AddThingToVillage 0x0065B951, CTCSoundPlayer::PlayCollisionSound 0x0066AC08). Before 2026-11-19 the two addresses
// were bound the other way round (naming wave 13), which took a per-TU device in several callers.
#if defined(FABLE_RETAIL_CDEFSTRING_FORCEINLINE_FORWARDER)
 // Per-TU inline opt-in (wfE10-2, parked rows 0x00795300 CTCUnholyPower::FrameUpdate / 0x0078FD60 CTCDivineWrath::FrameUpdate): the
 // conversion forwarder is always expanded while GetString keeps the plain inline heuristic. Written under the old binding (it read the
 // second cancel-sound call 0x0079546E as GetString); under the 2026-11-19 binding that call is the conversion ??B 0x00415DD0, so re-check
 // these rows on the plain header before reusing this. Same body.
 FABLE_FORCEINLINE operator CCharString()const{return GetString();}
#elif defined(FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING)
 // Per-TU visibility opt-in (the NScript TUs; replaces FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION, Codex pass 13, which declared the
 // conversion only under the old binding): those TUs expand the conversion forwarder and call GetString 0x00415D70 out of line
 // (0x00E02E50, 0x00EC5DE0, 0x00ECB0E0 ...); GetString is declared only (below).
 operator CCharString()const{return GetString();}
#elif defined(FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION)
 // Per-TU visibility opt-in (re-pointed 2026-11-19; under the old binding this was the NScript device, now OUTOFLINE_GETSTRING): the
 // size-optimised game TUs call the conversion 0x00415DD0 out of line for an implicit conversion temporary (CTCVillage::AddThingToVillage
 // 0x0065B951..0x0065B95C, the temporary at [ebp-4]; CMainGameComponent::InitSound 0x00417A58), where VC7.1's plain inline heuristic
 // under optimize("s") expands the forwarder and calls GetString. Declaration only.
 operator CCharString()const;                                           // 0x00415DD0
#else
 // Retail: the conversion forwards to GetString, both inline. A TU that inlines both expands the table call (CTCShop::OnSerialise
 // 0x00775C60); the /O2 game TUs call the conversion 0x00415DD0 out of line once their inline budget is spent (CTCTrophy::GlobalUpdate
 // 0x006DF0C9, CGameScriptInterface::EntitySetAsSittingOnFloor 0x008A8D59, CTCSoundPlayer::PlayCollisionSound 0x0066AC08).
 operator CCharString()const{return GetString();}
#endif
#if defined(FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING)
 CCharString GetString()const;                                          // 0x00415D70
#elif defined(FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING)
 // Per-TU inline opt-in (chunk 794, row 0x007039A0 CThingObject::OnHit): that TU expands GetString in place inside a conditional
 // full expression (0x00703F4B..0x00703F58: `mov ecx,[eax] ; push ecx ; push <result> ; mov ecx,offset GDefStringTable ; call
 // CDefStringTable::GetString 0x009D49B0`) where VC7.1's plain-inline heuristic calls an out-of-line copy. Same body.
 FABLE_FORCEINLINE CCharString GetString()const{return GDefStringTable.GetString(TablePos);}
#else
 CCharString GetString()const{return GDefStringTable.GetString(TablePos);}   // out of line at 0x00415D70
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
#ifdef FABLE_RETAIL_CDEFSTRING_TOWIDESTRING_INLINE
 // Opt-in (wfA2, row 0x00435530 CDisplayEngine::DoRender): Ego_r.pdb / FableWin.pdb ?ToWideString@CDefString@@QBE?AVCWideString@@XZ with
 // the body of its landed out-of-line copy 0x00415D90 (rebuild/src/compiled/00/41/CDefString_ToWideString_00415d90.cpp), visible because
 // DoRender expands it (0x0043599E..0x004359BE: GetString 0x009D49B0 into a CCharString frame slot, CCharString::ToWideString 0x0099E480
 // into the by-value argument slot, ~CCharString 0x0099EAE0). Exclusive with FABLE_RETAIL_CDEFSTRING (its declaration-only twin). Needs
 // CWideString complete (include fable_string.h first).
#ifdef FABLE_RETAIL_CDEFSTRING
#error FABLE_RETAIL_CDEFSTRING_TOWIDESTRING_INLINE and FABLE_RETAIL_CDEFSTRING both declare CDefString::ToWideString
#endif
 CWideString ToWideString()const{CCharString text=GDefStringTable.GetString(TablePos);return text.ToWideString();}
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_EQUALS
 // Opt-in (chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): FableWin.pdb ??8CDefString@@QBE_NPBD@Z, out of
 // line in retail at 0x005B3440 (the def name returned by CThing::GetDefName compared with a literal). Declaration only.
 bool operator==(const char* text) const;                               // 0x005B3440
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_INLINE_EQ
 // Per-TU visibility opt-in (agentC14, row 0x00603000 CTCInventoryMap::CreateMiniMapMarkers; first found by chunk 729): FableWin.pdb
 // ??8CDefString@@QBE_NPBD@Z with the text of its landed out-of-line copy 0x005B3440. CreateMiniMapMarkers expands it against a
 // literal (GetStringAsCharArray 0x009D49F0 on GDefStringTable at 0x006045F9, then `mov ecx,1Ch ; repe cmpsb`). The includer
 // includes <string.h>.
 bool operator==(const char* text)const{return strcmp(GDefStringTable.GetStringAsCharArray(TablePos),text)==0;}
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_SETDEFAULTSTRING
 // Retail opt-in (chunk 1025, row 0x0062FE60 NUISystem::CUIDef::CUIDef): the PDB symbol in ghidra_out (function table)
 // ?SetDefaultString@CDefString@@QAEXABVCCharString@@@Z, out of line in retail at 0x009D5080. Declaration only.
 void SetDefaultString(const CCharString& text);                        // 0x009D5080
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_TRANSFER
 // Opt-in (chunk 765, row 0x005DBED0): Ego_r.pdb / FableWin.pdb ?TransferBinaryIn@CDefString@@QAEXAAVCDataInputStream@@@Z, out of line
 // in retail at 0x0044FC00 (the only PDB spelling of the four bound to it). Declaration only.
#ifdef FABLE_RETAIL_CDEFINITIONMANAGER_LOADBINARY_PASS29
 unsigned long GetCRC() const { return GDefStringTable.GetStringCRC(TablePos); }
#endif
 void TransferBinaryIn(CDataInputStream& stream);                       // 0x0044FC00
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_EQ_CHARSTRING
 // NF67 opt-in (round 7, rows 0x00921440 / 0x00966290): Ego_r.pdb / FableWin.pdb ??8CDefString@@QBE_NABVCCharString@@@Z, out of line in
 // retail at 0x004B7980 (landed; the expression def's instantiation name compared with the caller's CCharString). Declaration only.
 bool operator==(const CCharString& text) const;                       // 0x004B7980
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_NE_CHARSTRING
 // Opt-in (wf14, row 0x0076C9A0 CTCEnemy::OnSerialise): Ego_r.pdb ??9CDefString@@QBE_NABVCCharString@@@Z (0x0077B970), retail
 // 0x0076D640 (out of line, emitted in the CTCEnemy TU). Declaration only.
 bool operator!=(const CCharString& text) const;                       // 0x0076D640
#endif
#ifdef FABLE_RETAIL_CDEFSTRING_COMPARE_FRIEND
 // NF58 opt-in (round 7, rows 0x004E5BA7 / 0x004E5B21, map<CDefString, long, CCompareDefString<long> >): both PDBs name the
 // comparator ??R?$CCompareDefString@J@@QBE_NABVCDefString@@0@Z; retail inlines it as a signed compare of the two TablePos words
 // (0x004E5BA7 `cmp ecx,[ebx+10h] ; jl`). The comparator reads the private TablePos, so it is a friend here.
 template<class T> friend class CCompareDefString;
#endif
private:
 long TablePos;
};
#endif
