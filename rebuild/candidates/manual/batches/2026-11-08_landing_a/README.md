# Landing 2026-11-08a: authoring chunk 850 (2 of 6 landed, 3 parked, 1 source failure)

| Row | Grade | Note |
|---|---|---|
| 0x00AC5770 C3DMeshFileEngineMaterialChunk::ReadFromFile | **RELOCATION_MATCH + PASS, landed** | opt-in retail class branch in C3DMeshFileEngineMaterialChunk.h, CIVCountedPointer RESET_ADOPT, CIVCountedPointeeBase CTOR |
| 0x009ED190 CConsole::Initialise | **RELOCATION_MATCH + PASS, landed** | row-local console model like landed 0x00419D90 / 0x009ECD80; the last 2 lines went once CConsoleVariable got its PDB member `CGenericVarRef Var` with its own ctor |
| 0x00AE3C30 CParticleProgram::ReadFromFile | DIFFER(2278v2310), 10 attempts | frame slot / register: retail puts the unused C3DTriangle2 resize temp in the saved-this slot |
| 0x00500540 CWorldMap::LoadRegion | DIFFER(2396v2412), 10 attempts | Codex body on our headers; ebx/ebp swap, end pointer not reloaded, frame 0x94 v 0x98, rotated wait loop (Codex's best was also 2396) |
| 0x00772BB0 CTCShop::AddSaleItem | DIFFER(2585v2585), 13 attempts | x87 scheduling in the matrix block only. The agent's next idea (a row-local `__forceinline` expansion) is ruled out: no row-local forceinline for a compiler residue |
| 0x0050A650 CWorldMap::GetMapNavigationAreaInit | SRC_FAIL, 2 attempts (out of time) | needs typed CThingFilter_* / GFFilter_Bind3 / CThingSearchTools GetAll* / CNavigationAreaInit / C2DBoxF models; best try `probe/0050a650_n2.cpp` |

- **Headers landed:** C3DMeshFileEngineMaterialChunk (FABLE_RETAIL_C3DMESHFILEENGINEMATERIALCHUNK), CIVCountedPointer
  (FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT), CIVCountedPointeeBase (FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR); all additive, no landed
  file defined the macros before. The other 14 opt-ins (CThing, CThingObject, CMatrix3x4, CWADFile, CLevelLoader, CParticleProgram, ...)
  serve only the parked rows and are kept in `pending/2026-11-08_linux_hand850/hdr/`.
- **Names:** labels `labels_landing_2026-11-08a.tsv` (std::_List_base<CConsoleCommandLine>::clear 0x009EE7B0, CDataInputStream::CopyTo
  0x009964E0, CParticleProgram::AllocateArrays 0x00ADF460). Queue `land0811a` (13): the agent's 11 NAME_FIXES, a REVIEW for 0x006BB2F0
  (SetAsUsingVillageTime? the body ignores its argument), and the coordinator's call on 0x009EF900: the retail body copies element by
  element with null-checked stores (no memmove), so it is the `__false_type` _M_insert_overflow that landed 0x009ED190 binds; the landed
  0x009EBB30 row binds the `__true_type` spelling at the same address and needs respelling.
