# Chunk 102 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A destructor with NO vtable store that releases a def pointer with `and dword ptr [esi+N],0` and ends in `jmp ~CTCBase`
  is a `__declspec(novtable)` class with a non-virtual out-of-line `~T() {}` in a `#pragma optimize("s",on)` TU, the def
  pointer's release `__forceinline` (pointer read into a local, clear inside the non-null arm); the same source shape
  covered three different classes here (CTCTargetingBase 0x004DA8B7, CTCDExplosiveTrail 0x004E4045, CTCTrapBase 0x007891F8).
- A bool `&&` chain whose false exits share one `xor al,al ; pop esi ; ret` tail with esi pushed at ENTRY is ONE
  `if (a && b && c && d) return true; return false;`: sequential early `return false`s duplicate the exits and push esi
  late, a nested `if` with a vector reference CSEs `first` (CanSplit 0x007DA1A0).
- The out-parameter `GetInterface(T** out, long type)` template must be `__forceinline` when its bool result is unused
  (`hero->GetInterface(out, 4);` as a statement): VC7.1 otherwise calls the template out of line (GetHeroStats 0x005EB4B0, 50v92).
- `bool __fastcall operator==(const C2DVector&, const C2DVector&)` written `return a.X == b.X && a.Y == b.Y;` gives the
  retail `fld [edx] ; fld [ecx] ; fucompp ; test ah,44h ; jp` pairs and full-register `mov eax,1` / `xor eax,eax` exits;
  grade it with `// Leaf: ??8` (0x00C46DC0).
- A by-value intelligent-pointer result built from a vector element (`return Jugs[index];`) needs the retail vector's
  iterator OBJECT (`*(begin() + n)`) so the element address is re-derived after the base constructor
  (`lea ecx,[edi+ebx*8]`) and `size()` loads `last` first (GetJug 0x007B9F60).
- The 24i `mov ecx,edi` placement between the 2nd and 3rd zero store of a vector member in a component constructor is the
  STLport `_Vector_base` + `_STLP_alloc_proxy` model (landed 0x00547600) -- confirmed again on CTCTavernBar 0x007B9FC0.
- A def-pointer local released with NO null test and load/dec/store of the count (`mov eax,[ecx+4] ; dec eax ; mov
  [ecx+4],eax ; jne`) is closest with a local pointer AND a local count in the destructor (`T* o = Object; long c =
  o->count - 1; o->count = c; if (c == 0) o->Destroy();`): the member form re-reads the slot after the store, a
  local-pointer `--o->count` materialises the count address (`add eax,4`) in the lane (0x008F0B80, parked 75v73).
- An argument forwarded unchanged by a member with the same argument count tail-jumps by overwriting dead argument slots
  (`mov [esp+0Ch],1 ; mov [esp+8],eax ; jmp`): write `return PlaySpeech(id, DialogueVoice, 1, b1, b2);` (0x006E3B60).
- In a speed TU an intelligent-pointer temporary constructed from a raw pointer (`CIntelligentPointer p(thing);`) is
  fully inlined (base ctor call, vtable store, SetPItem) while the same source in a size TU calls the converting
  constructor (compare landed 0x004DDAB4 with 0x007F5890).
- DirectShow base-class bodies live in Fable.exe (CTextureRenderer : CBaseVideoRenderer at 0x00CA5xxx): match them against
  the SDK's renbase.cpp text before trusting the catalog label (0x00CA5380 = OnStopStreaming).
- Retail CTCTavernGame offsets: the members below +0x20 are 8 lower than Ego_r.pdb (UIComponents vector), the members after
  the five handle vectors 0x1c lower; the card base starts at retail 0xf0 (8-byte alignment of the double) and its own
  three vectors shrink by 4 each; the blackjack class starts at 0x318.

Review at landing:
- Local base models next to an existing engine header: CTCTargeted (004da8f7, 007ea510 -- engine/CTCTargeted.h is a
  flattened POD, the bodies need the polymorphic CTCBase chain / a typed def pointer), CTCTargetingPlayer (00803b20 --
  engine/CTCTargetingPlayer.h is 8 bytes high after its two PDB vectors; 00802590 parked uses a local model too),
  CTCTrapBase (007891f8 -- engine/CTCTrapBase.h is 0xc high), CTCTavernGame (005e9b70, 005eb4b0 and parked 005e9b20 --
  engine/CTCTavernGame.h is 8 / 0x1c high). The generated headers for these classes keep the PDB's 16-byte vectors.
- Rows that needed 4 grades: 007da1a0 CanSplit matched on the 4th; 006e2cf0, 00802590, 005e9b20, 007bd2a7, 008f0b80 parked
  after 4; 008f0b20 parked after 2 (lane-gap candidate).
- 008f0960 is NewRound, not the catalogued NewGame: land under `CTCTavernGameCardBase::NewRound`.
- Catalog row 004debf7 (CTCTargetingSpirit::vfunc_10) spans six functions; it should be split before any landing.
- Already-landed CTCTavernGame_NewGame_005e9a70.cpp models `field_40 -= field_34` as ints on an anonymous struct: they are
  CurrentMoney -= BaseBet (retail +0x40/+0x34) and NewBestScore/GameStarted (+0xa0/+0xa2); worth retyping with the retail
  offsets above.
- Already-landed CTCTargeted_vfunc1_007ea550.cpp names slot 1 `vfunc_1` and the member `m_bTargetable` as `signed char`:
  it is CTCTargeted::Persist on the bool Targetable.
