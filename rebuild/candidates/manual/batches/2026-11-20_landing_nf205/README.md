# Landing nf205 (Account A, 2026-11-20): graphic data bank, texture cache and memory allocator names (7 rows)

Agent NF205 (round 10) handed back early (context) at q11886 with 39 entries: 4 done, 10 blocked, 4 deferred and 21 open. Its files are in
`naming/2026-11-20_agentNF205/`, with two held drafts in `work/held/`. The open and deferred entries go back to the queue (25); the
deferred ones were held a third time by another agent's lock, which one-agent-at-a-time removes. The blocked entries are unclaimed with
their notes, so the next agent can take them by id.

- 7 rows land, all RELOCATION_MATCH, every test PASS.
  - 0x00D2D060 (q5052): a whole-row port of a script entity onto engine/CBaseObject.h and the script headers; five callee pairs
    respelled onto spellings the table already had.
  - Self renames: 0x009FD160 (q11815) and 0x00994670 (CDataInputStream::ReadPixelFormat, which that move needed); 0x00A01650 (q11816; its
    callee 0x00A01120 moves onto the IAE spelling its own self row defines, narrowing a split); 0x0066CA80 (q11862, pointer -> reference
    per both PDBs); 0x009F1F90 `??3CMemoryAllocator@@KIXPAX@Z` as scaffolding (q11879).
  - 0x0122A600 (q11864): NF142's draft, the `dynamic atexit destructor for 'GFontTextureCache'' (`_$E2`), unchanged; the nf192 per-TU
    hash ruling lifts its hold. The table now binds `GFontTextureCache` at 0x013C5A80 under three per-TU hashes (0x009A3640, 0x009A25D0,
    0x0122A600): one spelling by the nf192 rule (the PDB hash is `?A0xfe46a155`). The stand-in g_AutoTinyThunkObject line is gone.
- Self-rename check: the six old leaves remain only as comments in their own row files.
- Headers: add-only opt-ins, `orig` equals HEAD, pinned 0, no landed includer defines them: CQ_EndGameFocalSitesScript.h SCARLETROBE,
  CDataInputStream.h READPIXELFORMAT, CGraphicInfo.h DEFAULT_CTOR, CRegionMinimap.h LOADTGAFILE.
- Pick-any (`logs/pickany_check.txt`): every copy with an owner object matches it in bytes and relocations: the CGraphicHeader ctor
  (0x009FD6C0), the CScriptGameResourceObject* ctors / dtor (0x006E7A60, 0x007E72A0, 0x007E74D0). The NO-OWNER-OBJ lines are vtables and
  the TU's own static.
- Link (`logs/link_landing.log`): 2 LNK2005 against 0 at HEAD. Both are 0x00D2D060's copies of `??0CScriptGameResourceObjectBase@@QAE@XZ`
  and `??0CScriptGameResourceObjectScriptedThingBase@@QAE@XZ`, identical to owners that define them strongly (the nf181 / nf184 case,
  accepted as debt; porting the owners is queued).
- Recheck (`logs/recheck.log`): split gate 0 created; 70 good, 1 bad. The bad row is 0x009E15E0 (TextureCache_MoveAndCleanup), which the
  manifest already grades DIFFER at HEAD; its file is unchanged and includes no changed header.
- Held drafts (advisor rulings, owner delegated; both need RENAMES lines and land in the next agent's slice):
  - 0x008E9850 (q11692 / q12295): its SetCurrentGroupName copy now equals the owner 0x0073DFF0. Its GetCRC / GetAsCharArray / GetLength
    copies bind `?NullChar@CCharString@@1DB` where the owner 0x00404310 binds the stand-in `?g_EmptyString@@3QBDB` for the same retail
    address. Ruling: a stand-in spelling for the same retail address is not a different target (EXTRA_RULES nf205 amendment); it lands
    after `objdump -r` of the copies and q.py at the three owners. 0x00404310's respell is queued.
  - 0x009A0AE0 (q11879): `??_GCMemoryAllocator@@IAEPAXI@Z` (FableWin.pdb, and the body). Ruling: the owner lands. Landed 0x0122A9A0's
    differing by-product copies are HEAD debt, if the LNK2005 set is unchanged and 0x0122A9A0 binds neither symbol at a retail site.
- tablecheck (`logs/tablecheck.txt`): five addresses shrink (0x0099A380, 0x0126008C, 0x0127094C, 0x007E74D0, 0x0099A430).
