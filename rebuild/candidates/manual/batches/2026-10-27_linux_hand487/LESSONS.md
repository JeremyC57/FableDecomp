# Chunk 487 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `if (dead) return false; if (!a) return false; if (!b) return false; return true;` is the retail shape whose tail is
  `test al,al ; setne al` with the shared false block placed after the first test (0x00666EA0
  CThingCreatureBase::IsThingInLOSAndFOV); `return a && b`, `return b`, `b ? true : false` and `bool r = ...` all lay the
  blocks out differently (59..95 bytes against 66).
- A binary search whose step-past branch is the `if` body (`if (!comp(key, *mid)) { first = mid + 1; count -= half + 1; }
  else count = half;`) gives retail's `jl` to the `count = half` block; writing the `count = half` case first flips it to
  `jge` (0x00680500 CVectorMap::UpperBound).
- STLport `vector<Enum>::resize` erases through `__copy_ptrs(..., __false_type)` = the counted `__copy` loop, because an
  enum element is not trivially assignable in STLport's type traits: no memmove (0x00643890).
- A CPlayer `Init/UninitMode*` handler matches first time on the landed screen-mode model (0x00635080: donor include of
  engine/CPlayer.h, bitset-reference-proxy `GetTC(&tc)`, `CIntelligentPointer` GetPItem); `T* tc = 0; GetTC(&tc);`
  keeps the `xor esi,esi`, an uninitialised `T* tc;` inside `if (GetTC(&tc))` drops it (0x00634A30 vs 0x00636270).
- `CGameCameraManager* camera = 0; if (GetCameraManager(&camera)) camera->X();` keeps the zero store and the test; the
  uninitialised, unchecked form is a different handler shape (0x00633530 vs 0x00634560).
- Retail inlines `CTBaseSingleton<CConsole>::Get()` and `CConsole::SetActive(bool)` into the console mode handlers: the
  singleton word is tested before `this` is saved (0x00634510).
- A `.text` "abs" xref that lands inside a `call rel32` is not a reference: check the bytes before trusting it
  (0x00643970, 0x0066C6D0 had none real).

Review at landing:
- 0x00666EA0 needed 17 compiles (variant probes) before the three-guard form matched.
- RELOC_CONFLICT: this chunk binds `?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ` to 0x00487DC0 (rows
  006292a0, 0062c6e0, 0062d380); the landed 0x00896EE0 binds it to 0x00487DD0, which by Ego_r.pdb order is
  PeekPControlledCreature. Fix 0x00896EE0, not these rows.
- Landed 0x006804C0 is misnamed (ETCInterfaceType LowerBound; really the EReactionSpeechType map's const LowerBound) and
  landed 0x00661FD0 "CThingPlayerCreature::vfunc_75" is CThingCreatureBase::IsWillable.
- Local base models next to an existing engine header: 0063dd70 (CWorldMap, PDB container sizes + flattened vptr),
  00661fc0 (CThingCreatureBase via donor include but `QBE` because the header flattens the vptr), the CMinimapData rows
  (engine/CSurface.h, engine/CTexture.h exist; CSurface needs its vptr), the CObserver / CGameEvent / CPlayer /
  CGameCameraManager / CWorld partial views flagged SHARED_HEADER_AVAILABLE by vland (all are callee or member types).
- The CPlayer mode-handler names are probable (handler order + mirror bodies); 0x00634A30 and its byte-twin 0x006349B0
  can only be told apart by donor order (FreezeControls before SpecialAbilityFreezeControls).
- 0x00661FC0 is declared non-virtual (`QBE`); the PDB symbol is `?IsIndestructible@CThingCreatureBase@@UBE_NXZ`.
