# Chunk 520 lessons

## GOTCHAS one-liners
- A forwarded float argument tail-calls `jmp [edx+N]` without re-storing the stack slot. An int or pointer argument gets `mov [esp+4],eax` first, and a `void` forward of a `void*` gives `call`, not `jmp` (0x00C38850 SetScale proved float).
- `shr ecx,8 ; and ecx,0FFFFFF01h ; push ecx` is an `unsigned char` argument built from `(flags >> 8) & 1` (0x00C59720 / 0x00C59BC0).
- A named local `CHeader* h = Handles[i]` in a loop adds a load through eax. Index `Handles[i]` three times to keep retail's dead `lea` (0x00C28640, 72v75 then MATCH).
- In a ??_G of a predicate pair, an implicit Right-member dtor schedules `lea ecx,[esi+4]` before the vtable store. A user-written inline `virtual ~CIsVariableFalse() { }` restores retail order (0x00E7DFE0).
- Script `return` guarded by the thread-terminating check (`if (cond) { if (IsActiveThreadTerminating()) return false; return true; } if (IsActiveThreadTerminating()) return false; return false;`) gives the two tails, `neg/sbb/inc` and `xor al,al`. A single `bool r = cond; if (term) return false; return r;` merges into `not/and` (82v76) (0x00EB4710).
- `IsPlayerThreateningEntity` returns `int` (YIH): `test eax,eax`. Declaring it bool gives `test al,al`, which was the 2026-10-14 park cause.
- libpk (0xC08AE0..) carries its own CHandleBasedMemoryPool (vtable 0x012B4098, no dtor slot, 0x10-byte Dinkumware vector). Do not model it with engine/CHandleBasedMemoryPool.h.
- 0x00CBE512 is GetDistanceBetweenThings (`fsqrt`) and 0x00CBE583 is GetSquaredDistanceBetweenThings (no sqrt). The landed bindings were swapped (FableWin.pdb order: Dist, DistThingAndPosition, Squared, SquaredThingAndPosition).
- `__finally` funclets of the CRT eh vector ctor/dtor iterators (`cmp [ebp-20h],0 ; jne ; push [ebp+..] x4 ; call`) appear as separate catalog rows. SKIP them.
- 2026-10-14 QFE-lane parks: retry under RTM 13.10.3077 before re-deriving. Most rows of this chunk matched on the first grade.

## Review at landing
- libpk pool rows 00c28380 / 00c28640 / 00c28690 use the PDB engine spellings that the engine build already owns (0xA026B0 GetPointerFromHandle, 0xA02AD0 ValidateResources, 0xA02B30 Free; Lock 0xA026F0 vs libpk 0xC283C0). This follows the landed libpk siblings (00c283a0, 00c28610, 00c28ae0). The coordinator must decide whether the libpk copy gets its own class name, because one retail address = one name breaks once both builds are landed.
- 00eb4710: RELOC_CONFLICT on GetSquaredDistanceBetweenThings (0xCBE583 here vs 0xCBE512 in landed 0x00DDC6C0). This row is right. Landed 0x00DDC6C0 and 0x00DA0D90 swap their declarations (NAME_FIXES.tsv).
- 00c20cb0: CheckState 0x00C20720 is a same-TU noinline stand-in body under the bound spelling `?CheckState@CSoundEvent@@QAEXJ@Z`. 0xC20720 has 8 spellings in reloc_bindings.tsv (ChangeState / CheckState / Init / Dispatch / Clear ...). Unify to one.
- 00c59720 / 00c59bc0: SetBuffer (slot 18, 0x00C59880) is declared `(void*, unsigned char)` here. The landed binding spells it `?SetBuffer@CCodaVoice@@UAEXPAVCSoundBuffer@@_N@Z`. The call is virtual, so there is no relocation clash, but the declared parameter types differ.
- 00c2d260: CInstance::Setup 0x00C38CD0 is declared here as `?Setup@CInstance@@QAEXJPBDJJJPAVCInstanceManager@@JJ@Z`. A landed row binds it as `CSlotRequest_00c38cd0::Setup`.
- 00c437c0: GetInfo 0x00C442F0 returns `SQueueInfo&` here. Landed CMarkedBuffer_00c43810 binds it as `GetMark` returning void.
- 00c52c40 parked DIFFER(75v75), register choice only, 4 grades spent (natural switch, reordered cases, inner switch, Model local).
