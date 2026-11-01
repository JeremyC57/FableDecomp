# Chunk 665 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 665) **Quest-script identity from the registration table**: an RTTI-less NScript vtable is stored only by its ctor;
  the ctor's sole caller is `Alloc` (`push size; call new; ... call ctor`), and Alloc's address is stored by the script
  registration routine (0x00CD5xxx..0x00CD6xxx) right after `push "Q_<Name>"` / `push "S_Q.."` -- the class is
  NScript::CQ_<Name>Script (0x00D2F640, 0x00D27290, 0x00D83350, 0x00DD1F40). Catalog "CParticleEmitter_Dtor_N" and even the
  manifest ctor label (0x00D25410 "CQ_NewOakValeIntroScript" = CQ_DragonBossFightScript) are propagated.
- **CLevelScriptBase<T> has an implicit dtor in retail**: a quest's implicit ??1 tail-jumps straight to ~CScriptBase 0x00CBD510;
  the header's out-of-line `virtual ~CLevelScriptBase();` gives a call to a nonexistent COMDAT and an inline `{}` adds a
  vtable store (114v108). Model it without a dtor declaration (header debt noted).
- **Emit an implicit dtor through the real ctor**: defining the class's ctor (as retail's TU does) emits vtable + ??_G + the
  implicit ??1 COMDAT; no EmitDestructor harness is needed (0x00D27290 probe 2, 0x00D2F640).
- **A 2-D global table swept by a pointer inside two counted loops**: retail's `mov edi,100h / mov edx,8 / add ecx,10h /
  dec / jne` nest is `T* e = &tab[0][0]; for (i<256) for (j<8; ++j, ++e)`; `tab[i][j]` indexing folds into one linear loop
  and hoists the compared member (0x009A1090). The two clears keep source order: `e->Glyph = 0; e->FontIndex = 0;` gives
  retail's `mov [ecx+2],bx` before the bitfield's `mov [ecx],ax`.
- **STLport _M_erase inlines one level only where its _tree.c body is visible**: ~CThreadedFileManager (0x0098E690) expands
  the CPendingObjectSortKey tree's `_M_erase` loop once inside the implicit array-element dtor and calls the Objects tree's
  `_M_erase`; the real _tree.c body in the TU reproduces both choices (no noinline/forceinline needed).
- **STLport tree iteration**: `_Rb_global<bool>::_M_increment` defined from _tree.c in the TU is expanded inline by VC7.1 in
  a map for-loop (0x006ED200), matching retail's inline increment.
- **Empty member classes break offsets**: an opaque `class X { void f(); };` member is 1-aligned (DialogueManager at +0x35);
  give it its real members (or a 4-aligned body) before suspecting the layout.
- zlib 1.2.1 `_tr_init` (0x00C051C0) matches from the landed init_block TU text unchanged; tr_static_init folds away.

Review at landing:
- NAME_FIXES.tsv (19 rows): the 10 own rows (wrong caller stand-ins seen), and 9 landed neighbours to respell: 0x00D25410
  (??0CQ_NewOakValeIntroScript -> ??0CQ_DragonBossFightScript), the four quest ??_G rows 0x00D27270 / 0x00D2F620 / 0x00D83330 /
  0x00DD1F20 (landed CLandscapeBackgroundPatch `vector deleting` stand-ins -> ??_GCQ_<quest>Script@NScript@@UAEPAXI@Z, whose
  callees name this chunk's rows), 0x00CBD510 (CRecordSet_00cbd510 -> ??1CScriptBase@@UAE@XZ), 0x007ACC50 (FreeTree_CTreeOwner ->
  map<CCharString,bool> _M_erase), 0x0098FD80 (CUnprovenKey -> map<unsigned long, CFileObject*> _M_erase), 0x00CDB3C0 (global
  ??1CCrateTeamManager -> nested in CQ_OrchardFarmRaidScript), 0x0099A300 (duplicate of chunk 662's row).
- vland WARN RELOC_NAME on 0x0099A300 / 0x00CBD510 / 0x007ACC50 are those catalog labels (fixed by the respells above).
- Header debt: engine/CLevelScriptBase.h (implicit dtor), CTCBase.h / CStreamingFontBank.h / CFontBank.h / CTexture.h flattened
  (local models; WARN SHARED_HEADER_AVAILABLE CTexture).
- Each row's TU defines the class ctor with an empty body (not retail's body: only the graded leaf is retail); 0x00D2F640's
  ctor builds the CCharString arrays through `??_H` (vector ctor iterator) -- not graded.
- The landed ??_G stand-ins (0x007AFF50, 0x007C66B0, 0x009A4E40, 0x00991330) call this chunk's rows under invented spellings
  (CTCBoss_dtor, ?Dtor@CTCGhostSword, ?Dtor@CStreamingFontBank, CThreadedFileManager_dtor): respell with the rows.
