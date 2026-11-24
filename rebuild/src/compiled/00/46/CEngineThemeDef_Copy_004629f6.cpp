// Leaf: ?Copy@CEngineThemeDef@@UAEXPBVCDefClassBase@@@Z
// CEngineThemeDef::Copy(const CDefClassBase* other) -- copy every member of another CEngineThemeDef: `*this = *(const CEngineThemeDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x004629F6, 5 bytes: a thiscall tail call, `jmp` to CEngineThemeDef::operator= 0x004629FB (emitted right after it).
// Identity: the RTTI vftable placement of 0x004629F6 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CEngineThemeDef::operator=(const CEngineThemeDef&) 0x004629FB, ??4CEngineThemeDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class CEngineThemeDef {                                // declarations only (no engine/CEngineThemeDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x004629F6
    CEngineThemeDef& operator=(const CEngineThemeDef& other);   // out of line: 0x004629FB
};

void CEngineThemeDef::Copy(const CDefClassBase* other)
{
    *this = *(const CEngineThemeDef*)other;
}
