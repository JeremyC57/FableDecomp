# Chunk 223 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- An unaligned call target with `ecx`/`edx` arguments and no stack arguments (e.g. 0x00CBE0C7) is a `__fastcall` free helper; declare it `bool __fastcall F(T* a, T* b)` and the caller's `mov edx,eax ; mov ecx,edi ; call` reproduces (CHitSwitch::Main 0x00EFB8C0; engine_api.tsv already names these).
- `if (IsDone()) return; while (!IsDone()) { ... }` gives retail's back-to-back IsDone calls before the loop's `jmp` (0x00EFB8C0).
- Two else-if arms that each build and queue their own CGameEvent tail-merge after the Type store (`mov [esp+0Ch],21h ; jmp` / `mov [esp+0Ch],22h`); a shared `long type` local keeps the type in eax instead (0x0068E8F0). Same for two identical AddMessage calls in different arms (0x006909A0).
- A CGameEvent declared before a `processed.SetHandled()` call keeps its member stores ahead of the call (0x006909A0); built before a null/Dead test, its stores sit before the test (0x00688A80).
- AddUID(unsigned __int64) homes its argument in a frame pair while AddFloat/AddLong use the dead `event` slot; floats copied through integer registers (0x00688A80).
- An aligned frame (`and esp,-8`) plus `fld [f] ; fstp qword [tmp] ; call ; fsub qword [tmp]` is a block-local `double` copy of a float member taken before the call (`double age = e.StartTime; e.StartTime = (float)(GFGetTime() - age);`, 0x009F5690).
- STLport `__false_type()` temporaries: give the tag struct a user `__false_type() {}` or VC zero-stores the tag byte (0x009F5690).
- STLport vector destructor = `_Destroy(_M_start, _M_finish)` as an inline template taking both by value (the end lives in a register) then `if (_M_start) free(_M_start)` (0x009F53C0).
- A CCountedPointer element reached through `vector::operator[]` + `operator->` each time keeps the element address in eax and the vtable in edx (`mov eax,[esi+2Ch] ; mov ecx,[eax] ; mov edx,[ecx]`); a direct `_M_start->P->` chain reuses ecx (0x009F53C0).
- A match test that reloads `EventStore._M_start` before every accessor call is written in the caller (or a manager-level inline), not as an inline CInputEvent member (the member CSEs `this`) (0x009F4F70).
- STLport list ctor: `_M_node(0)` in the init list, then a local head linked to itself and stored last (`mov [eax],eax ; mov [eax+4],eax ; mov [esi+N],eax`) (0x00B81BC0, 0x0089AC80). STLport 4.6 insert reads `n = pos._M_node; p = n->_M_prev` into locals before linking (one prev load) (0x0089AC80).
- A same-TU stand-in for FindAllMessages 0x006178F0 lets VC7.1 keep the out list's node word in a register and reuse its frame slot (0x0089AC80; register choice still differs).
- Constructor bodies full of constant stores: the lane schedules register-constant stores and zero stores in source order and pushes immediates late; matching needed the retail store order with the immediates written before the C3DVector temporaries, which pins the temporaries' dead X/Y stores after them (CEngineSimulationManager 0x00B81BC0).
- VC7.1 lane has no native wchar_t: a `wchar_t` return type is a compile error; use `unsigned short` (0x009F4F70).
- (earlier rows) `PrepareForRender(5)` must be `__forceinline` to inline into both weather boxes (0x00B54100/0x00B54210); CScriptThing needs a polymorphic CBaseClass root (virtual dtor) or its PImp lands at +4 (cst model); `(float)acos(x) * 0.15915494f` gives the dword multiply; explicit `v.Z = 0` stores fix zero-store order (0x00751610 partial, 0x00805A90).

Review at landing:
- Same-TU stand-ins in matched sources: 0x00811470 / 0x008115A0 (LowerBound 0x40F020 + 0x4365B0, GetDef 0x42AF3C, GetSubDef 0x485815), 0x0089AE30 / 0x008D04E0 (Compare 0x411570). Parked with stand-ins: 0x00698E40 (GetDef 0x56D0C0), 0x008A8890 (simplified GetAllThings 0x8ADBA0 body), 0x0089AC80 (FindAllMessages 0x6178F0).
- Local models next to existing engine headers: CEngineWorldMap (0x00B42ED0), CGameCameraBase (0x006FD8C0), CEngineSimulationManager (0x00B81BC0; the generated header uses the PDB's 12-byte lists, retail lists are 4 bytes -- the landed destructor 0x00B81CF0 already models it locally), CInputProcessGameBase (copied from landed 0x00690C70), CHandleBasedMemoryPool (parked 0x00C28400), CGameCameraManager (parked 0x00698E40).
- CInputManager / CInputEventPackage: Ego_r.pdb's Dinkumware vectors are 16 bytes, retail STLport vectors 12 -- every member after a vector is 4 bytes lower than the PDB says (EventStore +0x10, NoStoredEvents +0x1c, Checksum +0x3c, TimeStamp +0x5c, PackageFile +0x60).
- 0x009F5690 is mislabelled "Save" in engine_api.tsv and the manifest (it is the CAFile& constructor); the real Save is 0x009F52A0.
- 0x00B81BC0 catalog row "CGuiControlNumberInit" is the CEngineSimulationManager constructor.
- Already-landed 0x0089AC10 MsgOnLevelLoaded is an `__asm` naked body (asm_bake), not reconstruction.
- No row needed more than 4 grades; no catalog row held two functions.
