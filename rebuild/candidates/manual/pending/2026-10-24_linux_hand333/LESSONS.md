# Chunk 333 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- CReplaceSerialise::GetMaxSize (0x0051CBB0, 0x008275B0, 0x00826A00): write it as the static `__fastcall` template `long maxSize = version ==
  GetClassVersionNumber() ? size : TOld::GetMaxSize(version, size); return MAX(maxSize, sizeof(T));` -- the named local gives one call per
  inlined level (MAX straight over the call evaluates it twice); the unsigned `ja/jbe` come from sizeof; ReadFromFile allocates
  `new unsigned char[GetMaxSize(version, 0)]` and calls the static `__fastcall` ConvertToThisVersion(void*).
- A dead `movzx eax,al ; mov [esp+N],eax` in a ReadFromFile old-version path is the fully folded inline GetMaxSize chain keeping the
  converted version argument's home (0x0051A240, parked by chunk 332, matches with the model above).
- CVectorMap<CCharString> bound with an inline CCharString compare (0x004B9750 et al.): retail reads the key's data pointer once after the
  empty test -> `if (count > 0) { K k = key; do { ... } while (count > 0); }`; compute `count = Last - First` before `first = First` (the
  other order drops retail's `mov ebp,esi`).
- CSmallVector resize (0x006FADE0, 0x009D2660, 0x00A7DB10): `Min(n, GetSize())` with a by-value `long GetSize()` gives retail's spilled
  Size copy in the reference select (binding the member gives `lea esi,[ebp+4]`; `(int)n` with a long n adds a second temporary).
- CSmallVector<CCharString> resize (0x009D2660): keep Clear() declared-only when retail calls it (0x009D2110); a visible body inlines the
  destroy loop at one call site and reshuffles every register.
- `new T[n]` in VC7.1: an inline non-empty ctor (CEntry) becomes a null-checked count loop; an empty inline ctor (CEvent) a bare
  operator new[] in Allocate/erase, yet `vector constructor iterator` with the out-of-line ctor copy inside resize (same TU in retail).
- CSmallVector erase (0x006FAD50): `int size = Size; delete[] Start; Size = size - 1; Start = next;` for retail's Size load before the call.
- A temporary CCharString argument that must share the GetTC key's frame word wants the call result in a named local first
  (`long slot = ...GetDefGlobalIndexFromName("...")`; 0x0073A9F0, frame 8 -> 4).

Review at landing:
- Local polymorphic models next to existing engine headers: CSpawnedFuncBase (rows 0x00CDD450, 0x00E95F10, 0x00CE3D70, 0x00CE1C70) -- the
  generated engine/CSpawnedFuncBase.h flattens the vptr into a data member, so it cannot express the vtable store, base ctor call or
  virtuals; same choice as the landed CAT_BanditContact::Init 0x00E6AAC0.
- 0x0051A240 (chunk 332 parked row): a matching source is in `extra/0051a240.cpp` of this directory (graded straight from the exe with a
  local copy of q.py, `bin/qx.py`; the batch oracle grade is still to do).
- Landed `CSmallVectorEntry9_Resize00870370` uses `// Flags: /O2 /Oy /Gr` and a pragma-pack local model; the shared CSmallVector model in this
  chunk (Start/Size, Min(n, GetSize())) is the one to retype it to.
- No row needed more than 4 grades; no catalog row holds two functions.
