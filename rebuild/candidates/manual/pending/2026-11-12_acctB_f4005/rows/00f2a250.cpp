// CScriptDef::operator=(const CScriptDef& other) -- the implicit member-wise assignment of the script def: the parent-def base chain, then all 943 script-tuning members in declaration order, each with its own type's assignment; returns *this
// Leaf: ??4CScriptDef@@QAEAAV0@ABV0@@Z
// Inline-budget: no inline control (no pragma, flag or forced inline) -- the row is the compiler-written operator= and this file is the piece of retail's translation unit that decides its two inline decisions: CRegionScriptDef's Copy / Alloc ahead of CScriptDef's Copy / constructor. c2 trace: ??4CParentDefClassBase r=1 sz=49 c1=44 and ??4CDefClassBase r=1 sz=77 c1=72 by the cost rule (both already compiled); without the CRegionScriptDef block they are budget-rule candidates and fail c2's statement cap for a not-yet-compiled callee (51 and 38 counted against (2+2)*3+3 = 15) -> `call 0x00431ED0`, 11625 bytes.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F2A250, 11,711 bytes, thiscall, `push ebx` .. `ret 4` at 0x00F2D00C; `this` in esi, `other` in edi,
//   the result is `this` (`mov eax,esi`). No EH frame, no frame words.
// Identity: Ego_r.pdb / FableWin.pdb ??4CScriptDef@@QAEAAV0@ABV0@@Z. The only reference is the 5-byte CScriptDef::Copy 0x00F2A240
//   (`jmp 0x00F2A250`; vtable 0x012FB5C8 slot 19, both PDBs ?Copy@CScriptDef@@UAEXPBVCDefClassBase@@@Z). The class declares no
//   operator=: every one of the 943 members is copied at its own offset in declaration (= offset) order with exactly its type's
//   assignment (tools/map2.py: 943 copy events v 943 members of engine/CScriptDef.h, no member without an event, no event without a
//   member), and the parent-def chain is the implicit one. The landed twins are CCutsceneDef::operator= 0x00F2A000 and
//   NUISystem::CUIDef::operator= 0x00631720.
// What the compiler writes (nothing below is source text):
//   - base chain, expanded: CResource::operator= 0x00431F60 (the one call), PDefManager +0x1c, GlobalIndex +0x20, the Setup /
//     Template / DefaultValsApplied bits of +0x24 as three xor / and / xor merges, InstantiationName +0x25, then SubDefInfoMap +0x29
//     through ebx / ebp: the pair vector's operator= 0x00431F90 and the Dirty byte +0x36;
//   - 548 long, 285 float and 45 CDefString members as single dword moves through eax / ecx / edx in rotation;
//   - 15 CCharString members by CCharString::operator= 0x0099EFB0;
//   - 50 STLport vectors by vector<T>::operator=: CCharString 0x00432EE9 (14), float 0x0045EF2D (14), CDefString 0x0046078C (12),
//     long 0x00454886 (5), CConversation 0x00E54AA0 (BookReactions), CFireHeartPatternDef 0x00D34D40 (FHPatterns), CFeatDef
//     0x00F2D010 (Feats), CArenaRoundDef 0x00F25980 (ArenaRounds), CHeroSoulsRoundDef 0x00F26C30 (HeroSoulsRounds).
// Why the file holds more than one function (NOTES.md "What decides the base chain"; every function of this file equals its retail
//   counterpart, tools/sib.py):
//   - The implicit operator= exists only where something uses it: CScriptDef::Copy. Retail lays Copy out BEFORE operator= (0x00F2A240
//     then 0x00F2A250; the same for CRegionScriptDef 0x00F29F60 / 0x00F29F70 and CCutsceneDef 0x00F29FF0 / 0x00F2A000), which is the
//     order VC7.1 gives an INLINE virtual that only the vtable needs (the user, then what it uses). A non-inline Copy gets the
//     assignments first, bottom-up: ??4CParentDefClassBase then swallows ??4CDefClassBase (105 bytes, retail's 0x00431ED0 has 49)
//     and the row calls it. So Copy is inline and the constructor (retail 0x00F28F90, same translation unit) brings in the vtable.
//   - c2 expands a callee it has NOT yet compiled only when its body counts at most (arguments + 2) * 3 + 3 statements. The parent
//     def assignment counts 51 and CDefClassBase's 38, so the first function of the unit that uses them calls them: retail's
//     CRegionScriptDef::operator= 0x00F29F70 does (`call 0x00431ED0`). After that both are compiled (0x00431ED0 with its own call
//     to 0x00431F10) and later users get the cost rule, which expands both: CCutsceneDef::operator= 0x00F2A000 and this row.
//     The CVectorMap assignment is never compiled on its own here, stays a budget-rule candidate (7 statements, size word 54) and is
//     expanded under this row's large budget (2 x 9679).
//   - Hence CRegionScriptDef::Copy (0x00F29F60) and Alloc (0x00F29FA0, the user of its vtable) stand in front, as in retail.
// Header opt-ins: FABLE_RETAIL_CSCRIPTDEF_IMPLICIT_ASSIGN / FABLE_RETAIL_CREGIONSCRIPTDEF_IMPLICIT_ASSIGN (overlay: no declared
//   operator=), FABLE_RETAIL_PDCB_IMPLICIT_ASSIGN, FABLE_RETAIL_CDEFCLASSBASE_IMPLICIT_ASSIGN, FABLE_RETAIL_CRESOURCE_ASSIGN (the
//   implicit chain down to the declared CResource copy), FABLE_RETAIL_STLP_VECTOR_ASSIGN (vector operator= out of line),
//   FABLE_RETAIL_DEF_CLASS_BASE_CTORS + FABLE_RETAIL_CVECTORMAP_CTOR (retail's inline base constructors: with them the unit is
//   emitted in source order and the constructors here equal retail's 0x00F28F90 / 0x00F29C10 too).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_PDCB_TYPED_MEMBERS
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_PDCB_IMPLICIT_ASSIGN
#define FABLE_RETAIL_CDEFCLASSBASE_IMPLICIT_ASSIGN
#define FABLE_RETAIL_CRESOURCE_ASSIGN
#define FABLE_RETAIL_CSCRIPTDEF_IMPLICIT_ASSIGN
#define FABLE_RETAIL_CREGIONSCRIPTDEF_IMPLICIT_ASSIGN
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CRegionScriptDef.h"
#include "engine/CScriptDef.h"

// Ahead of the row, as in retail (0x00F29F60 Copy, 0x00F29F70 the implicit CRegionScriptDef::operator=, 0x00F29FA0 Alloc): the
// first user of the parent-def assignment in the unit.
inline void CRegionScriptDef::Copy(const CDefClassBase* def)
{
    *this = *static_cast<const CRegionScriptDef*>(def);
}

CDefClassBase* FABLE_FASTCALL CRegionScriptDef::Alloc()
{
    return new CRegionScriptDef;
}

// Slot 19 (0x00F2A240, `jmp 0x00F2A250`): the one user of the row, CScriptDef's implicit operator=.
inline void CScriptDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CScriptDef*>(other);
}

// Retail's constructor 0x00F28F90: the vtable it stores is what makes the compiler emit the inline Copy above, and with it the row.
CScriptDef::CScriptDef() {}
