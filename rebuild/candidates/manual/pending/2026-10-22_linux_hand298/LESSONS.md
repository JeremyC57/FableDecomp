# Chunk 298 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- An inlined body that copies a member into a named local (`frame = Bank->CurrentFrame; ... = frame;`) gets a temp with its own register, and that one extra live range rotates the colouring of the whole function. Reading the member in place at each use (`Data->TouchedFrame = GraphicInfo->Bank->CurrentFrame;`) gives retail's colouring: 13 register pairs fixed at 0041ac20 (CSprite::GetDataFromDef, inlined CGraphicInfoFrame::GetFrame).
- When a def temporary dies before a store in retail, an inline index getter fixes it. At 0041ac20 an inline `GetFirstGraphicIndex()` makes the second def temporary die before the store; an open-coded index keeps it alive past the store.
- At 0041ac20, bank `GetGraphicFrames(i)` must be `__forceinline` and written `GetGraphicInfo(i) ? GetGraphicInfo(i)->Frames : 0` (two calls, which VC CSEs) over an STLport-shaped vector with size()/operator[]. A plain `inline` version stayed out of line.
- For a bitfield clear, `x.loading = 0` gives `and [mem],imm`. Retail's `mov reg,[mem]; and reg,imm; mov [mem],reg` comes from clearing through a reference or a small inline setter (009a2000, CStreamingFontBank::Preload drain loop).
- `MemOffset * 0x400` keeps retail's `and 7FFFFFh` bitfield mask; `MemOffset << 10` lets VC drop it (009a2000).
- Pointer-ternary cache selection (`Cache = big ? &BigCache : &SmallCache`) with an `unsigned short` index and an inline `GetCacheMemoryOffset()` gives retail's registers for the first half of 009a2000.
- In implicit conversions, the order the temporaries are built follows the argument conversion. At 0053f810 (CScrollingViewport::Initialise), passing a `const char*` where a CCharString is expected builds the string temporary BEFORE the later `false` argument is pushed; an explicit `CCharString("x")` local moves it. The same row needed a forceinline STLport push_back, a `__false_type` with a user ctor, `Children[Children.size()-1]`, and a CCountedPointer dtor that clears both words.
- Name the operands before a long vector expression. At 00872630 (CStringAndCageBase::OnInit), the physics pointer and `C3DVector up(0,0,h)` are named BEFORE `pos - facing*(cage+string)*0.5f + up`, and the focal's thing is a function-scope named pointer. 16 other forms differed only in the order of the sum.
- A temporary `C2DBoxF` built in place as a call argument (rather than a named local) fixed the frame layout of 004900f5 (CSubtitleRenderer::DrawTextPrimitives). The row is still parked on its slot order.
- Persisted fixed arrays at 00b21e90/00b22100 match with `CCharString(prefix) + GFIntToCharString(i)` through a forceinline SaveValue wrapper, `*(begin() + i)` element access, and the zero defaults inside a guarded do-while. CShape::OnSerialise (00761c00) probably needs the same idiom: retail hoists its 0.0f defaults into the loop preheader.
- Compiler-generated `operator=` (0045fac1, CSpecialAbilitiesBulletTimeDef): don't write one. Let VC generate the memberwise copy (fld/fstp loops for float arrays, movsd x3 for C3DVector) and force its emission from a helper that is never landed.
- Ability-def `Transfer` bodies (00464db5, 00465390, 0046a2fc) call in the def's TRANSFER list order, not member declaration order, and /Os never unrolls the fixed arrays.
- `while (!used)` on a bool from a call emits `neg/sbb/inc` (00dc8030). A `&&` loop condition whose operands are temporaries gives them VC's destruction-flag word, which is zeroed in the prologue.
- In these TUs, VC7.1 does not treat `wchar_t` as a keyword (C2143 on `const wchar_t*`). Use `unsigned short` unless a header typedefs it.
- A forceinline helper is required wherever retail inlines a small math helper under `#pragma optimize("s")`. A plain `inline` SquaredDistance (00b4ced0) or GetTextVWindow (004900f5) stayed out of line.
- Retail inline-asm rows are a lane gap; don't try to recover them with intrinsics. The SSE 4x4 matrix product under the 0x13D2880 CPU flag (00988e80, 00989230), the movss/maxps/minps box distance (00b4ced0) and the fistp rounding block (00646762, which also forces the ebp frame) all compile to different code from intrinsic or C++ forms.

Review at landing:
- These rows needed more than 4 grades:
  - 0041ac20: 16 grades, matched with the advisor.
  - 00872630: 17 forms before it matched.
  - 009a2000: 13 probes (a-m), still parked at 1004v982.
  - 004900f5, 00b206b0, 0047042a: 6-8 each, all parked.
- Local base models sit next to existing engine headers. No src file includes `engine/<Class>.h`; each declares a local model of these classes:
  - CSprite, CGraphicFrame, CResource, CResourceList (0041ac20)
  - CParentDefClassBase (0045fac1)
  - CSpecialAbilitiesPhysicalShieldDef (00465390)
  - CFrontEndManager, CManager, CObserver (0053f810, 0054a6f0, 0054a990)
  - CGameCameraManager, CThingGameObject (00872630)
  - CVertexShaderConstantLayout (0098a540)
  - CMemoryDataInputStream, CDataInputStream (00a49240, 00a62120, 00b22100)
  - CBankFile, CShaderDataBank (00a62120)
  - CSBAgents (00c094a0)
  - CGameScriptInterface, CScriptThing (00dc8030, 00dc8890, 00dde940, 00eff930)

  Retype them with retype_landed.py where the header layout agrees. Several are STLport- or member-shaped on purpose (e.g. the 0041ac20 GraphicList), so re-grade after retyping.
- Three catalog rows hold several functions. Re-split them in the catalog:
  - 00a3e520 CSoundManagerLH::vfunc_18: a 24-byte body, int3 padding, then the next function.
  - 00a3e830 CSoundManagerLH::vfunc_35: the same.
  - 00ed14e0 CSingingStone::Init: int3 padding at +0x291, then the next body.
- Label corrections, all listed in NOTES.md:
  - 0x0053F810 is NUISystem::CScrollingViewport::Initialise, not CSprite::Initialize.
  - 0x00C094A0 is the CSteeringGroup ctor, not Initialize.
  - 0x0049D870 is GetWorldFrame.
  - The sound-lib addresses near 0xC0A790 and 0xAAB850 are not engine code.
- 00c094a0 needs `/EHsc`; its `// Flags:` line carries it.
- A stray empty directory `D:\Documents\FableTLC/rebuild/build/landverify` exists inside this chunk dir. A grading tool created it from a Windows path; ignore it, don't land it.
