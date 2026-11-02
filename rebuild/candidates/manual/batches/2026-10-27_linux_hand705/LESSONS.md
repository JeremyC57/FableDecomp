# Chunk 705 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00692230 / 0x006922C0) A landed row with no `// Leaf:` line can start to DIFFER when a shared-header migration gives its
  class a virtual dtor: the TU then also emits the class's ??_G through the vtable and the verifier grades that larger symbol
  (DIFFER(40v30) here). Check `objdump -t` before blaming codegen; the fix is the Leaf line, not a local base without its dtor.
- (0x004D9861) A def dtor reached only from a ??_G in a vtable is the class's implicit `??1<Def>@@UAE@XZ`; identify the def from
  that vtable's RTTI (CFireheartMinigameDef 0x0123E584). Defining retail's own out-of-line ctor in the TU emits the vtable,
  ??_G and the ??1 -- no emission helper needed. A padding-free region with `push N ; pop eax` getters and `pop ecx` after delete
  is a `#pragma optimize("s", on)` def TU: the pragma reproduces its ??_G and ctor too.
- (0x009E1440) `erase(begin(), end())` callers + a `rep movsd` x N loop + `ret 8` returning the first argument = STLport
  vector<T>::erase(T*, T*) with the random-access __copy inlined; take T from the owning class's Ego_r member type.
- (0x00B23CB0) `g->A(x)` then a tail call on `g->member[i]` with the result: name the member pointer as a const local
  (`const CMeshDataBank* bank = g->MeshBanks[1]; return bank->Peek(index);`) to get retail's ecx loads before the store over the
  argument slot; the inline object expression stores first.
- (0x00913F00) A static member reached through a non-inline accessor's result (`PeekWorld().GetFrame()`) keeps the accessor call.
- (0x008B74A0) On engine/CBaseObject.h + a CClass model, CClass is 0x14 bytes unpacked (Type +0x10): CThing's own members start
  at +0x14 (a pad from 0x11 shifts the bitset by 4).
- (0x00A75E00) Retail calls protected virtuals of CANavQuadTreeNode through child pointers from CNavQuadTreeNode: that needs
  friendship in C++ (the access does not change the bytes or the derived symbol's `MAE`).

Review at landing:
- Replace the four landed repair files by hand: 0x00692230 / 0x006922C0 (same names, add Leaf + real members), 0x004D9861
  (new name ??1CFireheartMinigameDef@@UAE@XZ, retire ??1CDefPointeeBase_Derived_004d9861 and the
  ?CFireheartMinigameDef_Destruct@@YIXPAX@Z binding), 0x009E1440 (new name = STLport vector erase; retire
  Render2DDrawListBlockView::CopyBlock and its July-era test; fable_render2d_vertex_queue.h may lose its only user).
- Landed 0x008C0B40 CCreatureAction_WhisperSommersaultForward::PerformAction declares 0x004C9840 as
  CBaseObject::OnPropertyChanged(long) and the bit test as a raw flag word: respell to CThing::HasTC / RemoveTC (NAME_FIXES).
- Landed 0x00913EB0 SmallBirdWander::Update and 0x0093D030 / 0x00911F80 declare 0x009130D0 as ExecuteFlyingPath and
  0x00912700 with pointer/non-const parameters: respell (NAME_FIXES).
- 0x00A3EB20 / 0x00A3EBF0 / 0x00A3EC30 are CSoundManagerLH vtable slots landed under invented address classes.
- CSoundManagerLH list element type (CSoundObjectLH vs CSoundEmitterLH): coordinator decision (NOTES.md).
