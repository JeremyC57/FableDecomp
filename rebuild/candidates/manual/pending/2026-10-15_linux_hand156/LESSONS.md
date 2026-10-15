# Lessons from chunk 156
- An overload that takes `const CTexture**` and loops `AttachTextureToStage(i, tex[i])` reproduces the four Draw*(textures, n, ...)
  overloads once the landed AttachTextureToStage body is an inline member.
- `SetD3DMatrix(D3DMATRIX&, 16 floats)` (private PDB member) inlined gives retail's transform setters: 12 dword copies, the
  0,0,0,1 column stored after the pushes.
- Store order puzzles: a flag assignment written before a group of copies (BeginRender), two enum stores through an inline
  two-argument setter (Reset), and a chained `a = b = c = 0` all move stores to where retail has them.
- A raw-pointer CCountedPointer ctor written `Data(p) { if (Data) Info = new Block(Data); else Info = 0; }` gives retail's
  tail-duplicated three-exit shape when built straight in the argument area.
- A list `front()` reached through an iterator `*begin()` gives `mov eax,[eax] ; lea ecx,[eax+8]`; the direct `Head->Next->Data`
  form folds into `mov ecx,[eax] ; add ecx,8`.
- An inline push_back that captures the head node in a local BEFORE `_Buynode` keeps it in a callee-saved register across the call.
- A pointer returned and also passed by reference (`set.insert(key)`) sits in a dead argument slot when the insert is wrapped in a
  `__forceinline` helper taking the key by value; the return then uses the register copy.
- 64-bit key compares: `it != end() && key >= it->first` gives `cmp hi ; jb ; ja ; cmp lo ; jae` (retail order);
  `key < it->first` gives ja before jb.
- STLport `__copy(..., random_access_iterator_tag(), (int*)0)` in vector::erase placed the empty tag in the dead `position` slot
  without any special source form (so the chunk-149 "tag at an existing slot" gap is not universal).
- `std::strncmp(text.GetPString(), marker.GetPString(), marker.GetLength())`: a temporary in the call is converted after the
  marker accessors (wrong order); a named local/const& gets the frame slots swapped -- open.
- Quest/entity-script slot names: `grep -P "^CGameScriptInterface\t\S+\t<slot>\t" rebuild/manifest/rtti-vtables.tsv` then the
  manifest row of that address gives the PDB-mangled method; argument values follow the push order.
