# Chunk 2008 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x004CA390) The load-side CThing::ConstructFromBinarySave is the landed SaveBinary 0x004C84B0's TU recipe verbatim: inline
  Transfer<short / bool> specialisations + visible noinline traits readers; a TU-defined (non-inline) CThing::PeekComponent is expanded
  by /O2 as retail expands its same-TU out-of-line copy. Matched on the first grade.
- (0x00B665E0) A constant that feeds both a store and an x87 expression across a function-local static's guard: write it as a local
  `const float` and assign the member after the static; assigning the member first schedules its store before the guard (0x00B66844).
- (0x00B665E0) `f(C2DBoxF(0, 0, F((float)s.GetWidth()), G((float)s.GetHeight())))` evaluates both by-value GetDimensions temporaries
  first (height, then width), each in its own frame slot, then G, then F; a named box local costs a slot the temporary shares.
- (0x00B54890) A named `float radius = Settings.RainFadeEnd;` before an `if` whose arm passes it to a call reproduces retail's
  apparently dead frame store (`mov [esp+28h],edx` with `mov eax,edx ; push eax` in the arm) and the 4 extra frame bytes.
- (0x00B54890) A long& out-index stepped by a same-TU callee stays in ebx after the call only when that callee's body is visible
  (`__declspec(noinline)`), confirming the 2003/2004 lesson for a `long&` parameter.
- (0x00B54890) STLport push_back's inline `_Construct` copy of a local C3DVector is scheduled load-ahead (three loads, three stores) only
  when every same-TU callee that receives the local's address has a visible body (ClipBoxToCamera + ClipBoxToLandscape +
  ClipBoxToWeatherMaskSphere); one missing body keeps the pairwise load/store order. Instantiating _M_insert_overflow does not matter.
- (0x00BB1F50) A C4DVector `+= C4DVector(v3, 0.0f)` folds `W += 0.0f` into a W self-copy (`mov edx,[eax+0Ch] ; mov [eax+0Ch],edx`), as
  retail; where the scheduler places that copy is a region effect: C3DVector's CROSS_X_GROUP + SUBTRACT_XY_GROUP opt-ins (PAREN tuples)
  fixed the quad loop's three adds but moved the triangle loop's.
- (0x00BB1F50) Retail cloth quads are triangle-strip ordered (faces 0 1 2 and 1 3 2): n1 = (p0 - p1) x (p2 - p1), n2 = (p2 - p1) x (p3 - p1).

Review at landing:
- 0x00B54890 is a pair-landing case: author ClipBoxToLandscape 0x00B52DD0 (~4.5 KB) and respell the landed ClipBoxToCamera 0x00B51F60
  (stand-in CWeatherRendererMethods) and 0x00B52090 (IsPointInPrecipitation) under their PDB names, then retry RenderRain with the
  three bodies visible (differ/00b54890.cpp is otherwise exact).
- 0x00BB1F50 used all 30 grades; its SSE block uses the new helper engine/CEngineClothRendererSSE.h whose per-site ASM_RULES attempts
  table (30 C++ + 10 intrinsic spellings) is NOT done -- do not land the helper without it.
- 0x013BAB0C has two landed spellings (g_DefaultSerialisationSectionName in 0x004CA010, g_CCharString_013bab0c in 0x0121E160).
- 0x004C7F40's landed row declares InitFromThingInfo(const CThingInfo&) public; the PDB form is private and takes CThing::CSerialiseInfo
  (NAME_FIXES.tsv) -- respelling it changes the landed row's symbol.
- The landed CEngineSkyRenderer::Render 0x00B662F0 binds RenderSky through a `CEngineSkyRenderer_Methods` stand-in with a pointer
  parameter (wrong mangling; NAME_FIXES.tsv).
