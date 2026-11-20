# NF205 notes (round 10, 2026-11-20; entries q4860 .. q12295, handed back at the q11879 / q11886 entry boundary)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 39 entries: 4 done (q11815, q11816, q11862, q11864), 10 blocked (q5052, q5064,
q5138, q6196, q6309, q11692, q11793, q11879, q12191, q12295), 4 deferred (q4860, q11760, q11873, q11772), 21 open (q11886 onward, except
q12191 and q12295). Rows in rows/: 7 ported (0x00D2D060, 0x009FD160, 0x00994670, 0x00A01650, 0x0066CA80, 0x0122A600, 0x009F1F90), all
RELOCATION_MATCH PASS in recheck_final.log. Two RELOCATION_MATCH PASS drafts held outside rows/ for a pick-any decision (work/held/:
0x008E9850, 0x009A0AE0). No made-up names: every new spelling is a PDB name (or the compiler-generated ??_G / _$E2 / STLport form).

## Coordinator decisions needed
1. **0x008E9850 (q12295 / q11692).** The new draft fixes the nf201 hold (its SetCurrentGroupName copy now equals owner 0x0073DFF0, bytes and
   relocs True). It also newly defines three fable_string.h inline copies: ?GetCRC@CCharString@@QBEKXZ, ?GetAsCharArray@CCharString@@QBEPBDXZ,
   ?GetLength@CCharString@@QBEJXZ. They bind ?NullChar@CCharString@@1DB at 0x0129AAF4, while the owner rows use stand-ins for that same address:
   0x00404310 binds ?g_EmptyString@@3QBDB (same Leaf name, relocs False); 0x00403A10 / 0x00403A00 define free-function stand-ins (different
   symbols). The three copies are byte- and reloc-identical to the ones landed 0x0073DFF0 already emits (and the 24 GETCRC_INLINE users).
   Question: does a header-inline copy count as "different" when its only relocation difference is the owner's stand-in spelling for the same
   retail address? If not, the draft lands as is; if yes, respell 0x00404310 first (NAME_FIXES).
2. **0x009A0AE0 (q11879).** The draft is the address's own body (??_GCMemoryAllocator@@IAEPAXI@Z, FableWin.pdb; calls ??3 0x009F1F90), as
   nf86 scaffolding through the inline Destroy. Landed 0x0122A9A0 (DEALLOCATOR opt-in, operator delete inline) emits unbound byproduct COMDATs
   of ??_GCMemoryAllocator (35 bytes, VirtualFree expanded) and ?Destroy@CMemoryAllocator@@KIXXZ (44) that differ from this row's (28 / 37).
   Question: may the owner row land while another landed row's unbound byproduct copy differs (queue 0x0122A9A0 to stop emitting them), or
   hold until 0x0122A9A0 changes?

## Self renames (RENAMES section 1, 6 lines; each address ends on one spelling)
- 0x009FD160 ?Load@CGraphicHeader@CGraphicDataBank@@QAEXAAVCDataInputStream@@@Z (Ego_r + FableWin); caller 0x009FF590 (NF200) already bound it.
- 0x00994670 ?ReadPixelFormat@CDataInputStream@@QAE?AVCPixelFormat@@XZ (both PDBs); claimed outside rows.txt (lock was free) because
  0x009FD160 is its only binder: moving the binder alone would have created a split.
- 0x00A01650 ?push_back@?$CArray@VCGraphicInfo@@@@QAEXXZ (FableWin + body); caller 0x009FF590 already bound it. Its callee 0x00A01120 moves
  from QAE to the self row's own IAE STLport spelling (split narrowed, RENAMES s2).
- 0x0066CA80 ?LoadRegionMinimapTGAFile@CRegionMinimap@@QAEXABVCWideString@@@Z (param PAV -> ABV: Ego_r + FableWin); caller 0x00829D90 bound it.
- 0x0122A600 _$E2 (NF142 draft, unchanged; lifted by the nf192 per-TU-hash ruling).
- 0x009F1F90 ??3CMemoryAllocator@@KIXPAX@Z (both PDBs); no landed row bound the old free stand-in.

## Pick-any checks (every new defined symbol with a table line or a landed owner; tools/cmpbody.py, tools/cmp2.py)
- 0x00D2D060: ??0/??1CScriptGameResourceObjectScriptedThingBase vs 0x007E72A0 / 0x007E74D0, ??0CScriptGameResourceObjectBase vs 0x006E7A60:
  bytes True, relocs True. ??_G of STB: owner 0x007E7520 defines a ...Retail stand-in symbol (no clash); same copies as landed 0x00D2ED20.
- 0x00A01650: ??0CGraphicHeader@CGraphicDataBank@@QAE@XZ vs 0x009FD6C0: True/True. Form 1 newly defined an STLport _M_insert_overflow copy
  (278 v owner 337 bytes): fixed with FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY (no copy).
- 0x009F1F90: its ??3 copy vs 0x0122A9A0's: True/True.
- 0x009FD160 / 0x00994670 / 0x0066CA80 / 0x0122A600: new symbols are header inlines with no table line and no landed owner (listed in STATUS).
- Held drafts: see the coordinator decisions above.

## Binding census of the changed rows (binds.py unfiltered, HEAD object v new object; RENAMES section 2 has every changed pair, 9 lines)
- 0x00D2D060: 5 changed targets: 0x007E74D0, 0x0099A380, 0x0099A430, 0x0126008C, 0x0127094C; each new spelling is already in reloc_bindings
  (witnesses 0x00CD3DA4, 0x00658301, 0x0065ED59, 0x006E70A0, 0x007E7640); the old stand-in spellings had 0x00D2D060 as their table witness.
- 0x009FD160 (0x00994670), 0x00A01650 (0x00A01120), 0x0122A600 (0x009F9F70, 0x013C5A80): as in RENAMES. 0x00994670, 0x0066CA80,
  0x009F1F90: no callee spelling changed.
- New inline copies with no table line, checked against catalog names as well as Leaf lines: ??_GCScriptGameResourceObjectScriptedThingBase
  vs 0x007E7520's object (stand-in name there; cmp2 bytes True and the same four relocation symbols); CTexture::CTexture 0x009F9D00 is the copy
  constructor (??0CTexture@@QAE@ABV0@@Z), not the default one 0x0122A600 emits. No catalog row for ~CScriptGameResourceObjectBase,
  ??_GCScriptGameResourceObjectBase or CCountedPointer<STB> ctor / dtor.
- EH funclets: no landed row includes any of the seven changed files (grep of rebuild/src/compiled). The self renames do not move a file.

## Descriptive / fidelity
- Scaffolding rows (`// Fidelity: scaffolding, not retail`): 0x009F1F90 (and the held 0x009A0AE0); helper symbols are in the TU-hashed
  anonymous namespace (objdump).
- No `// Flags:` lines added; no row needed flags beyond its catalog entry.

## Follow-ups seen (not done)
- 0x01223EE0 (GFontTextureCache initializer) binds 0x013C5A80 as the CStatic_009f9f70_013c5a80 stand-in: port onto the same anonymous-namespace static.
- 0x00730F90 (q5138): next try an assignment-body CENTRE ctor variant or ?Initialise@C2DBoxF@@QAEXABVC2DVector@@M@Z (both PDBs).
- 0x009BDE70 (q6309): retail is a constructor (long, CWideString, CCharString); catalog ~CInputProcessManager is wrong; class unproven.
- 0x00AD5C30 (q6196): a scalar-deleting shape folding onto ~CUniqueVertex, whose dtor is non-virtual: owning class unproven.
- 0x0129AAF4 has two spellings (?NullChar@CCharString@@1DB, ?g_EmptyString@@3QBDB; plus ?g_EmptyCharString_0129AAF4 in 0x00403A10).

## Rule slips
- None known. One typo in an APPLIED note (0x0046D3A30) was corrected to 0x006D3A30 at once.
- Queue order: q11793 was recorded as a family pass (coordinator steer) before q11815..q11864 were worked; q12191 recorded per the steer
  before q11886..q12190 (open).
