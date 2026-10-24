# Hand chunk 367 (NLocalisation::GetFontBankName .. std::__destroy_aux<_Deque_iterator<CSubtitleRenderer::CSetTextInfo> >): 38 of 40 byte-exact

All 38 sources grade MATCH or RELOCATION_MATCH in the final `gall.sh` run (`grades.txt`). Every test prints `<addr>_TEST PASS`
under both g++ (`tall.sh`) and VC7.1 (`tall.py`). Two rows are parked in `differ/`, and none were skipped.

## What matched (38)

- **NLocalisation** (0x00415350 GetFontBankName, 0x00415440 GetStreamingFontBankName, 0x00415550 GetSoundDefName): each builds
  a `"<prefix>_<LANGUAGE>_<suffix>"` CCharString for the current language. The body is `switch (GetLanguage())` with the arms
  in retail's source order (French, Japanese, German, Korean, Chinese, Spanish, Italian, then the English default). VC7.1
  biases by the lowest case (`add eax,-4`), then goes through a byte index table into an 8-entry jump table. The hidden result
  comes in ecx (`__fastcall`).
- **CMainGameComponent** (0x0041641F SetDisplayMode, 0x00417568 ConsoleSetResolution): these switch the display mode, then
  re-run the post-resolution setup. Both are frame-pointer size TUs (`optimize("s", on)` + `("y", off)`). In SetDisplayMode
  the C2DExtentsI temporary's slot is reused for the GetDimensions result. In ConsoleSetResolution the component reference is
  homed at [ebp-4] and the three integer arguments stay in esi/edi/ebx across every call.
- **NGlobalConsole::Initialise** (0x00419D90): registers the "ActivateQuest" console command. The name must be an IMPLICIT
  `const char*` -> CCharString conversion. VC7.1 builds implicit-conversion temporaries before the explicit argument
  temporaries, which gives retail's string-first order. The conditional-destruction flags live in ebx.
- **NResourceDirectoryNames::GetWriteableEngineCacheDir** (0x0041A220): returns `<AppData>\Microsoft\Fable\` via
  SHGetFolderPathW. It needs `// Flags: /GS`. The sum is built straight into the hidden result: with no named local there is
  no copy.
- **C2DBoxF::C2DBoxF(const C2DVector&, const C2DExtentsF&)** (0x0041A7E0, MATCH): the box from its corner and its size. The
  second sum is written extents-first (`fld ext.Y ; fadd tl.Y`).
- **CGraphicFrameData::Release** (0x0041C090): decrements the reference count and destroys on zero. The destructor chain must
  be `__forceinline` so that `delete this` expands in place instead of calling the out-of-line ??_G.
- **CPixelFormat::CPixelFormat(long x4)** (0x0041C850): forwards to Initialise(r, g, b, a, false) under /O2.
- **NUISystem::CManager** (0x0041CEB3 PlaySample slot 41, 0x0041D0A8 PlaySoundW slot 40): play a UI sound by symbol. Both are
  frame-pointer size TUs. The player's PlaySoundEvent overloads are declared string-first so that the index overload lands in
  slot 3 (VC lays overloaded virtuals out in reverse order). In PlaySoundW the sound-system pointer is a named local kept in
  esi.
- **STLport map<unsigned long, NUISystem::CUIState> tree** (0x0042CB90 _M_copy, 0x0042CE7A operator=): STLport is verbatim
  with exceptions off. _M_copy is a frame-pointer size TU, with `top` parked in the dead `x` argument slot. In operator= the
  root/leftmost/rightmost accessors return references, so the header is re-read before each call.
- **map<CCharString, CCountedPointer<NUISystem::CComponent> >::operator[]** (0x0042D45D): lower_bound, then insert of
  (k, null pointer). This is a frame-pointer size TU. The `T()` temporary is the zeroed counted pointer at [ebp-8]
  (`and dword ptr`).
- **NUISystem::CYesNo / COk::ChangeState** (0x0042D604 / 0x0042D65B): add or remove the dialog as a (exclusive) observer of
  the UI manager on show/hide, then call the base ChangeState. These need `optimize("s", on)`: /O2 tail-duplicates the base
  call and threads the second test away. Passing `this` as a CObserver* gives the null-preserving `neg ; sbb ; and`.
- **GPlayMusic(float)** (0x0042DED5): starts the front-end music stream (`FrontEndMusic` under the music directory, .wma
  renamed to .ogg). Pragmas: "y" off + "s" (the CNewFrontendGameComponent TU). One full expression builds the name, and its
  temporaries die in reverse order. It needs `#include <stddef.h>` for wchar_t and `L""`.
- **CNewFrontendGameComponent::InitialiseSounds** (0x0042E17C): `PSampleBank.Reset(new CSoundBankLH(...))`, then Load, then
  the group volume, then SetEnabled. Same TU pragmas.
- **InstallationComplete()** (0x0042E3CE): the PDB says `bool`, but retail returns a full register (`xor eax,eax ; inc eax`).
  An `int` return under "s" matches; the bool form graded 29v32.
- **STLport map<CCharString, long> tree** (0x004316CF _M_insert, 0x00432BAB _M_copy): _M_insert is verbatim, with
  CCharString::operator< inlined as the result form (same block -> false, null left -> true, null right -> false, else
  `Compare < 0`). VC tail-merges the two `_M_rightmost() = z` stores. In _M_copy, _M_clone_node is INLINED (`__forceinline` in
  the size TU) around the out-of-line _M_create_node 0x00431773.
- **CControlsDef::operator= / CEngineDef::operator=** (0x00431E80 / 0x00434370): compiler-generated member-wise assignments
  emitted out of line through a helper `a = b`. The retail callers are the vtable slot-19 stubs (0x00431E70 / 0x00434360). The
  bools are copied byte by byte with a cl/dl/al rotation. The float members are copied through integer registers.
- **vector::_M_allocate_and_copy** (0x0043212B pair<unsigned long, CSubDefInfo>, 0x00432FF0 CCharString): allocate, then the
  non-POD `__uninitialized_copy` with the placement-new null test. allocate, _Construct and __uninitialized_copy must be
  `__forceinline` in the size TU. The pair needs an explicit memberwise copy constructor: without one VC copies the 12 bytes
  with `movsd`.
- **GFSerialiseVectorBinaryIn<CCharString>** (0x00433273): the landed 0x005C2AD8 template verbatim. The traits constructor,
  TransferBinaryIn and the one-argument `resize(n) { resize(n, T()); }` are `__forceinline`; the two-argument resize stays out
  of line (0x0043332C).
- **vector<CCharString>::erase(first, last)** (0x0043336A): `copy` (the counted random-access loop, forceinline), then
  `_Destroy` (the out-of-line __fastcall 0x00414E00), then `_M_finish = i`.
- **CDisplayEngine** (0x00434B50 GetMainWindow, 0x004350D0 Init): GetMainWindow returns the C2DBoxF by value through C2DBoxF's
  USER copy constructor: the first float goes through the FPU, the rest through integer registers. The implicit copy graded
  52v50. In Init the class has a double member, so its own vptr pads to 8. A vptr-only root base (CBaseClass) keeps the
  members at +4.
- **CCountedPointer<CDisplayViewManager>::DeleteData** (0x00436BA0): `delete static_cast<T*>(data)` with the inlined
  ~CCountedPointer of PCurrentView.
- **CTBaseSingleton<CEngineManager>::Get** (0x00436FB0): the inlined CEngineManager constructor. It needs the real STLport
  `_List_base` / `_Vector_base` structures with `_STLP_alloc_proxy` and `allocator_type()` temporaries (the "24i" lesson). The
  flat model graded 105v105 with scheduling differences.
- **CPlayerGui::AddScriptScreenText** (0x0043A2A0): converts the position to virtual coordinates snapped to the pixel grid, then
  `push_back(CScriptScreenText(text, vpos, colour, font, CWorld::GetFrame()))`. The five-argument constructor is inline;
  CRGBColour's copy goes through Set(R, G, B, A); the element copy constructor is out of line (0x0043F490).
- **std::fill_n<unsigned long*, unsigned int, unsigned long>** (0x0043E060, MATCH): STLport, returns `first`. `__fastcall` is
  explicit.
- **CEnginePrimitiveHandle algorithms** (0x00440100 fill, 0x00440260 __copy, 0x00440390 __copy_backward; all MATCH): the
  element assignment is `Clear(); Data = other.Data;`. Clear() must use the POINTEE member
  `if (Data.Object) Data.Object->NotifyIfLastHandle(this);`, which keeps the pointer in ecx. The `GetRefCount() == 1` and
  `IsUnique()` forms load it into eax and add `mov ecx,eax` (115v113, graded).
- **std::__destroy_aux<_Deque_iterator<CSubtitleRenderer::CSetTextInfo> >** (0x00440B40): the landed sibling 0x00440C00 with a
  28-byte element (four CWideStrings destroyed in reverse order) and a 4-element block (`lea ebx,[esi+70h]`). It is
  `__fastcall`, with the `__false_type&` in ecx and the two by-value iterators as `ret 20h`.

## Parked for Fable 5.1 (2)

- 0041cfb2 JustGetTheFrikkingText2 DIFFER(139v138): retail takes the hidden CWideString result in EDI (callers do `lea edi,[ebp+8] ; push symbol ; call`, 0x0041DC33) and the symbol on the stack (`ret 4`); ours is standard `__fastcall` (result in ecx, symbol in edx) | tried: `__fastcall` free function returning CWideString by value (frame-pointer size TU) | next: "lane gap: LTCG custom calling convention (ltcg_grade.py exports the target, which forces the standard convention)"
- 00431606 std::_Rb_tree<CCharString,pair<CCharString,long>>::insert_unique DIFFER(201v201): in the descent loop retail jumps (`je`) from the b==a test forward to ONE shared `xor al,al ; jmp` block placed after the `!b` test; ours falls through into a first `xor al,al` and the `!b` test jumps back to it (same instructions, same length) | tried: result-variable comparator (forceinline, 201v201), return-form comparator (199v201: the false threaded into the branch, `cmp ecx,eax` operand order), nested-if form (205v201) | next: re-model with the CCharString key and the result-form CCharString::operator< that matched in 0x004316CF (the graded stand-in was the landed 0x007AD060 set<CBasicString<char>*> model), then try the return form with `b == a` operand order or `result = false` as the default

## Skipped

None. All 40 extents look right: each row ends at its `ret`/`int3` padding and no row spans two functions.

## Label corrections

Catalog name -> real function. Evidence is in each source header: PDB name sets used as names only, vtable slots, xrefs and
string literals.

- 0x00415550 `GetFontBankName` (propagated) -> `NLocalisation::GetSoundDefName()` ("<LANGUAGE>_SOUND_SETUP").
- 0x0041641F `CMainGameComponent_ChangeDisplayMode` -> `CMainGameComponent::SetDisplayMode(unsigned long, unsigned long, unsigned char)`.
- 0x00417568 `ConsoleSetResolution` -> `CMainGameComponent::ConsoleSetResolution(CMainGameComponent&, const vector<CGenericVar>&)` (static).
- 0x00419D90 `StatRegisterClass` (propagated) -> `NGlobalConsole::Initialise()`.
- 0x0041A7E0 `operator+` (propagated) -> `C2DBoxF::C2DBoxF(const C2DVector&, const C2DExtentsF&)`.
- 0x0041C090 `CGraphicFrameData_ReleaseReference` -> `CGraphicFrameData::Release()`.
- 0x0041C850 `ConstructString` (propagated) -> `CPixelFormat::CPixelFormat(long, long, long, long)`.
- 0x0041CEB3 `CASound_PlayFromSymbol` -> `NUISystem::CManager::PlaySample(CCharString&, long)` (vtable 0x01230134 slot 41).
- 0x0041CFB2 `GetTextBySymbol` -> `JustGetTheFrikkingText2(const CCharString&)` (GetTextBySymbol is its callee).
- 0x0041D0A8 `CASound_PlayMessage` -> `NUISystem::CManager::PlaySound(CCharString&, long)` (mangled PlaySoundW; slot 40).
- 0x0042CB90 `CSurface_Clone_Recursive` -> `_Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, ...>::_M_copy`.
- 0x0042CE7A `CTCCarryable_CopyAssign` -> the same tree's `operator=`.
- 0x0042D45D `CPersistTraits_Transfer_And_Construct` -> `map<CCharString, CCountedPointer<NUISystem::CComponent> >::operator[]`.
- 0x0042D604 `CChangingStateComponent_StateChangeHandler_1` -> `NUISystem::CYesNo::ChangeState(unsigned long)` (vtable 0x0122F884 slot 48).
- 0x0042D65B `CChangingStateComponent_StateChangeHandler_2` -> `NUISystem::COk::ChangeState(unsigned long)` (vtable 0x0122FAF4).
- 0x0042DED5 `CFrontEnd_PlayAutoSaveSound` -> `GPlayMusic(float)` (the only `YIXM` free function in the PDB name sets).
- 0x0042E17C `Init_FrontendSounds` -> `CNewFrontendGameComponent::InitialiseSounds()`.
- 0x0042E3CE `FrontEnd_CheckQuickAssignInventory` -> `InstallationComplete()`.
- 0x00431606 `Map_InsertStringKeyNode` -> `_Rb_tree<CCharString, pair<const CCharString, long>, ...>::insert_unique`.
- 0x004316CF `BinaryTree_InsertDigScriptNode` -> the same tree's `_M_insert`.
- 0x00431E80 `CParentDefClassBase_CopyAssign_1` -> `CControlsDef::operator=` (vtable 0x01230E84 slot-19 stub 0x00431E70).
- 0x0043212B `Vector_AllocateAndCopy` -> `vector<pair<unsigned long, CDefClassBase::CSubDefInfo> >::_M_allocate_and_copy`.
- 0x00432BAB `CExpression_DigScript_CloneTree` -> `_Rb_tree<CCharString, pair<const CCharString, long>, ...>::_M_copy`.
- 0x00432FF0 `Vector_AllocateAndCopyCCharString` -> `vector<CCharString>::_M_allocate_and_copy`.
- 0x0043336A `Std_Vector_Erase_Range` -> `vector<CCharString>::erase(iterator, iterator)` (34 callers share this folded body).
- 0x00434370 `CParentDefClassBase_CopyAssign_2` -> `CEngineDef::operator=` (vtable 0x0123118C slot-19 stub 0x00434360).
- 0x00434B50 `GetDefaultVal<CEngineGraphic>` -> `CDisplayEngine::GetMainWindow() const`.
- 0x004350D0 `InitDisplayEngine_InitGraphics` -> `CDisplayEngine::Init()`.
- 0x00436FB0 `Get` -> `CTBaseSingleton<CEngineManager>::Get()` (vtable 0x01231584).
- 0x0043A2A0 `ShowOnScreenMessage_FormatDisplay` -> `CPlayerGui::AddScriptScreenText(...)`.
- 0x0043E060 `_Fill_n<unsigned long*, unsigned int, unsigned long>` (Dinkumware) -> STLport `std::fill_n` (returns the iterator).
- 0x00440100 / 0x00440260 / 0x00440390 `_Move<CEnginePrimitiveHandle*, CEnginePrimitiveHandle*>` (one label on three
  algorithms) -> `std::fill`, `std::__copy<..., int>`, `std::__copy_backward<..., int>` over CEnginePrimitiveHandle.
- 0x00440B40 `Vector_DestructStringRange` -> `std::__destroy_aux<_Deque_iterator<CSubtitleRenderer::CSetTextInfo> >`.
