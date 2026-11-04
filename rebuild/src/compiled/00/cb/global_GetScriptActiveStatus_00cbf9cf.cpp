// GetScriptActiveStatus(const CCharString& name) -- the recorded active status of a named script (the ScriptActiveStatuses entry)
// Leaf: ?GetScriptActiveStatus@@YI?AW4EScriptActiveStatus@@ABVCCharString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBF9CF, 15 bytes, __fastcall (name in ecx): `push ecx ; mov ecx,offset ScriptActiveStatuses ;
//   call 0x00CD2B1C ; mov eax,[eax+14h] ; ret` -- the map's out-of-line _M_find returns the node, the status is its value +0x14
//   (node: colour/parent/left/right, then pair<const CCharString, EScriptActiveStatus> at +0x10).
// Identity (two sources): (1) Ego_r.pdb ?GetScriptActiveStatus@@YI?AW4EScriptActiveStatus@@ABVCCharString@@@Z, paired in the same
//   TU with the landed SetScriptActiveStatus 0x00CBFAB8 (ScriptActiveStatuses[name] = status); (2) the global 0x0143E910 is the
//   landed ?ScriptActiveStatuses@@3V?$map@VCCharString@@W4EScriptActiveStatus@@..@std@@A and 0x00CD2B1C is that map's STLport
//   _M_find (lower bound over std::less<CCharString> 0x0042AAB3, then the `key < found` test that returns the header). Label from
//   labels_chunk674 (the row split out of catalog row 0x00CBF7FE).
// Callees: ??$_M_find@VCCharString@@@?$_Rb_tree@..EScriptActiveStatus..@std@@ABE..@Z 0x00CD2B1C (out of line in this size TU).
// Header: stlport/fable_map.h, fable_string.h (FABLE_RETAIL_CCHARSTRING). EScriptActiveStatus: the MS forward enum (no shared header
//   names its enumerators).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "stlport/fable_map.h"
enum EScriptActiveStatus;
extern std::map<CCharString, EScriptActiveStatus> ScriptActiveStatuses;   // 0x0143E910
EScriptActiveStatus __fastcall GetScriptActiveStatus(const CCharString& name)
{
    return ScriptActiveStatuses.find(name)->second;
}