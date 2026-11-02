# Chunk 708 (NA051) -- 10 of 10 byte-exact (RELOCATION_MATCH), six headers given opt-in retail branches (2026-11-02)

Rows (`verify.log`: WINS 10, RELOCATION_MATCH + PASS, no warnings; 27 probes; no asm, volatile or flags):
CCreatureAction_ScorpionTailPlunge::Initialise 0x008C3E90, NCreatureMode::CModeCreatureIsProjectile::OnActivate 0x008D6690,
CCreatureAction_CastStrengthMultiStrikeProper::FinishAction 0x008E63B0, CGraphicDataBank::Close 0x009FECE0 and the private
CGraphicDataBank::CloseGraphics 0x009FEB80 (the catalog called both "Close"; CloseGraphics is inlined into ~CGraphicDataBank and Close),
~CNavigatorBase 0x00A47180, ~CKeyboard 0x00A66990 and ~CMouse 0x00A66F80 (the forge stand-ins `CBase_Destructor_Helper` /
`CGuiControlTreePane_Destructor` were wrong), ~CEngineLandscapeMeshBuilder 0x00BE2B20 and the protected NUISystem::CRedefinerList::ClearList
0x005567B0. Every name is in both PDBs; destructors named from the vtable they store first.

Headers (hdr/README.md): opt-in branches in engine/CMovableResource.h, CIndexBuffer.h, CKeyboard.h, CMouse.h, CClass.h and CNavigatorBase.h.
Coordinator check: `cpp` of each under the default macros gives the same text as before apart from the __LINE__-numbered static-assert
typedef names; all 111 landed includers plus the respelled caller below rechecked: 12 MATCH + 100 RELOCATION_MATCH, all PASS
(includer_recheck_grades.tsv).

Second spellings at landing (owner 2026-11-02 rule): 0x005567B0's landed caller NUISystem::CRedefinerList::Refresh 0x00557000 bound the public
`QAE` spelling; its CRedefinerList model now declares ClearList protected (`IAE`, both PDBs), binding updated, recheck RELOCATION_MATCH + PASS.
Queued as PRIORITY (NAME_FIXES_QUEUE.tsv, c708) because each caller needs a class-model change: 0x006627F0
`?GetAnimDuration@CThingCreatureBase@@QBEMJ@Z` (const; four older spellings), 0x009D58A0 `?Close@CBankFileAsync@@UAEXXZ` (virtual; QAE and a
stand-in), 0x009E4710 the STLport `_List_base<CMaintainedEvent>::clear` (four stand-ins). The agent's other 21 NAME_FIXES rows are queued as
c708 (labels: ghidra_out/labels_chunk708_2026-10-27.tsv). The agent showed the brief's GetPItem spelling was wrong; 0x00A01B50 is
`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` (both PDBs), now in the brief generator.
