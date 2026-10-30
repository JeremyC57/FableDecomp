# Chunk 582 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Spawned-thread statement: `AddSpawnedFunction(new CSpawnedFunc<T>(...), CCharString(""))` through a `__forceinline
  SpawnFunction(func, region = "")` helper keeps retail's order (operator new before the region temporary, both temps to the end
  of the statement); a named `CSpawnedFuncBase* thread` local grows the frame by 4 (0x00EEA020: 0x38 v 0x3c).
- Ego_r places C3DVector members at odd offsets (+0x8A, +0x99 in CQ_NewOakValeIntroScript): a local C3DVector model needs
  `#pragma pack(push, 1)` or every later member shifts (0x00DB4F70).
- Retail `vector<bool>` in script classes is the plain 12-byte byte-per-element vector, not a bit vector (0x00ED7A40;
  class size 0xE4 confirms).
- The PlayCutscene family (0x00D7AA30 model) moves onto the shared NScript headers byte-neutral; the only per-class deltas
  are the map argument slot and an optional `csargs.clear()` (0x00D848D0 / 0x00D872D0 / 0x00D7C470 / 0x00E1D7B0).
- OnPersist key order is not member order: pair each key literal with the Ego_r member at retail offset + 0x14 (0x00E4F7F0).
- A function-local real STLport vector<CScriptThing> reproduces the inline _Destroy loop + free at every early return
  (0x00D6CA90).
- `#pragma inline_depth(1/2)` around an explicit specialization did not stop VC7.1 from inlining CScriptThing's copy ctor in
  _M_fill_insert's raw-end loops (0x00D34B30, parked).

Review at landing:
- Local stand-ins for classes with a retail vtable: CBaseObject 0x0129A7D8, CScriptGameResourceObjectBase 0x0126008C,
  CScriptGameResourceObjectScriptedThingBase 0x0127094C, CScriptGameResourceObjectMovieBase 0x01260EF4 (00d848d0, 00d872d0,
  00d7c470, 00e1d7b0, 00eea020); NScript::CIsThingConscious 0x012C2FE8 and CSpawnedFunc<CExpression_FollowScript> 0x012F55CC
  (00eea020). CScriptThing, CPredicateBase, CSpawnedFuncBase, CScriptBase come from the shared headers.
- 0x009AACE0 bound as map<CCharString,CCharString>::clear in 00d7c470: identity of map::clear vs _Rb_tree::clear unproven
  (probable; inline clear probe gave 584v553, so it is out of line). Landed label CTCCarryable_OnKill is wrong.
- 0x006E7AB0 bound as ??1?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@QAE@XZ (vland RELOC_NAME warning vs the
  CCountedPointer_Release label); landed rows also call it ~CMovieImpPointer / ~CResourcePImp: one name needed.
- SpawnFunction (00eea020) is a source helper with no retail symbol (`__forceinline`), same as landed 0x00ED7830 / 0x00DABAC0.
- Already-landed errors: CV_TalentlessBardScript::OnPersist 0x00ED85A0 bit-vector model of SongThingAvailable;
  CQ_WizardBattleScript::EndMission 0x00E1D640 declares 0x00E1D7B0 as FinishMazeStage; CQ_HeroSoulsThunderScript::Main
  0x00D7AE80 spells PlayCutScene; CQ_NewOakValeIntroScript::Main 0x00DABAC0 still uses a local CScriptBase model.
- litcheck note: strings with `?$AA` in the mangled name (HERO/Hero) show as BAD in my helper but are correct.
