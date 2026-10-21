# Chunk 252 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A float member stored then re-read from memory (`fstp [esi+10h] ; fld [esi+10h]`, `fcom [esi+10h]` instead of `fst` and
  a stack compare) is a clamp through a free inline helper taking `float&` (`ClampLow(v, 0.0f)`, `Raise(v, max)`); direct
  member tests, ternaries, /Op and member helpers all keep the value on the x87 stack (0x007E0750).
- Same-TU setters retail inlines into a sibling (`SetSprinting(bool)` 0x007E0670 / `SetRunning` 0x007E06B0) defined inline
  in the class reproduce the threaded flag tests; when retail skips the setter's availability re-check, the call site was
  `if (!cond) SetX(false);` rather than `SetX(cond)` (0x007E0750).
- STLport list `push_back` = `insert(end(), x)` with the iterator built in its argument slot (`push ecx ; mov eax,esp ;
  mov [eax],edx`) needs a user copy ctor on the list iterator (0x007CA9E0).
- In-place writes of a C2DBoxF / CRightHandedSet member (`lea eax,[esi+N]` then stores through eax) are inline `Set(...)`
  members; assigning a constructed temporary copies through the frame (0x007CA670).
- A def index read twice (`cmp word ptr [eax+8Ch],dx` then `movzx edx,word ptr [eax+8Ch]`) where ours CSEs it into ax:
  name the thing pointer AFTER the def-pointer declarations (`CDefPointer a; CDefPointer b; CThing* thing = ParentThing;
  GetDef(thing->GetDefIndex(), &b)`); with same-TU GetDef/GetSubDef stand-ins the defs then stay in ebp/ebx (0x00801A50).
- A map find whose iterator is read back from its frame slot (`mov eax,[esp+18h] ; cmp eax,[esi]`) is a named
  copy-initialised iterator; to make two finds share one slot put the first in an inline `Contains(map, key)` helper
  (0x00801A50).
- Intelligent-pointer members at odd offsets (+0x134 / +0x141 in CTCInventoryWeapons' holder) need
  `#pragma pack(push,1)` around CBaseIntelligentPointer {vptr; m_pItem} and CIntelligentPointer<T> (0x005C44FA, as the
  landed CTCChest::Open 0x00750540).
- An implicit `const char*` -> CCharString argument (ctor 0x0099EBF0 with the defaulted -1 length pushed) is built
  before the other arguments are pushed; an explicitly constructed CCharString temp pushes in the wrong order
  (0x00731680, 0x007CC2B0, 0x0072E130).
- A same-TU stand-in that must stay out of line (to keep a def object in a register) is a NON-template function under
  `#pragma auto_inline(off)`; a template stand-in was inlined anyway (0x007F0B10).
- `float newDifference = AngleDifference(newAngle, target)` named after `float newAngle = angle + velocity` keeps the
  old difference in its own frame slot (0x00751820).
- `vec[i] = f(temp)` with retail's index store before the call: name the temp and take a start pointer first
  (0x00641AE0; the rest of that row is a register permutation).
- In a size TU (`optimize("s", on)`) small string helpers (GetPString / Length / StartsWith / CCharString operator==)
  are only expanded when `__forceinline` (0x00787F29).
- A member call through a getter whose result retail fetches BEFORE pushing the arguments (`call getter ; lea ; push ;
  push ; mov ecx,eax`) is a named local (`CAnimationManager* m = GetAnimationManager(); ... m->Query(a, b)`); a chained
  `Get()->Query(a, b)` pushes first (0x00787F29, twice: manager and world).
- A CCharString block held in a register across two inlined `==` calls (`mov esi,[edi]` once, reloaded only after
  strncmp) needs CBasicString<char>::operator== (0x004A93C0) AND the Compare it calls (0x00411570) defined in the TU
  under `auto_inline(off)`: VC7.1 then knows the equality writes no memory (0x00787F29; as landed 0x0075D2B0).
- Under 3077 the 8-byte intelligent-pointer temporary built inside a loop gives the `and esp,-8` aligned frame that
  retail really has (0x0072E130 matched first try; GOTCHAS chunk 232 lists the opposite case as a lane gap).
- An out-parameter GetInterface whose result is not tested and whose out pointer is not pre-set reads the uninitialised
  word on the absent path (as landed 0x007CC410); when that word must share with an earlier long (the effect index),
  VC7.1's slot packing order decides the frame -- 0x007CC2B0 could not get the 8-byte pointer onto the def word.
- A propagated GetSubDef<T> COMDAT (0x00669580) serves several def types; label it by the call site's def class.

- Behaviour tests under VC7.1: there is no NRVO, so a stub returned by value whose dtor releases a count needs a
  counting copy ctor (0x0061B3C0 test), and there is no std::nanf (build a quiet NaN from 0x7FC00000; 0x005CC862 test).

Review at landing:
- Same-TU stand-ins: 00801a50, 00583cd4 (GetDef/GetSubDef), 005be119 (IS_THING_ALIVE, LowerBound), 005bf77d
  (GetDef/GetSubDef/GetAllItems), 00787f29 (CBasicString<char>::operator== 0x4A93C0 + Compare 0x411570 -- check these
  symbols do not collide with the landed Compare_00411570 in 0x0075D2B0).
- LowerBound bindings: non-const 0x40F020 in 00583cd4, 005a3436, 005be119 (same-TU stand-in body), 005bf77d,
  005c44fa, 00731680; const 0x4365B0 in
  00789e60 and 0059d820 only (parked 007534b0 also const). No file binds one symbol to both.
- free() (CRT 0x00BFEA14) vector dtors: 005bf77d, 0059d820, 005a3436, 005c69aa -- never operator delete.
- /GS: 005fad40 (matched, `// Flags: /GS`) and parked 005fb4b0. /EHsc: parked 00c3db80 / 00c3f800.
- No jump tables in this chunk.
- Leaf symbols: every src carries `// Leaf:`; 0057832d's leaf is the mangled ctor `??0CTCInventoryExperience@@QAE@PAVCThing@@@Z`.
- pack(1) intelligent pointers in 005c44fa, 00731680, 0072e130 (vtable 0x01238C6C / 0x01237F1C).
