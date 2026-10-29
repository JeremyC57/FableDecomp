# Chunk 562 (agent562, 2026-10-29): 12 of 12 never-tried rows land (RELOCATION_MATCH)

CPersistContext::Transfer<signed char> 0x004CC5C0, CThingManager::WorldUpdateThings 0x0051FCC0, the NUISystem destructors
~CEditBox 0x005408F0, ~CMovie 0x005483C0, ~CMesh 0x00550230, ~CMorphingSprite 0x005541E0, ~CScrollBarOutside 0x00559520
and ~CScrollBar 0x00559A10 (catalog had CScrollableList / CScrollableListWithButton VectorDtor), ~CTCBoastUI 0x0056AC80,
~CTCQuestCompletionUI 0x005E41E0, CTCQuestCompletionUI::Copy 0x005E49E0 (catalog vfunc_12), and the compiler-generated
~CTCTavernGame 0x005EDA50 (catalog CChunkedFileChunk::DestructorCleanup). Every leaf is the Ego_r.pdb spelling.

Review (NOTES.md): 0x013B8AB8 is bound as CMovie::PEngine, CMovie::Engine and g_pRenderDevice; 0x013B8AC0 as
CMovie::MovieTexture and two g_pMovieTexture spellings -- one address, one name, for the naming pass. Four globals in
WorldUpdateThings carry descriptive names (no PDB name, no other references).

NAME_FIXES.tsv: 14 rows, deferred (naming pass): RemoveFromUpdateList takes CThing& (AAV), STLport _M_erase / clear real
names at five addresses, CPlayerGui::IsOpen 0x004374F0 / Open 0x004374B0, ~CTCBase 0x00686830.
