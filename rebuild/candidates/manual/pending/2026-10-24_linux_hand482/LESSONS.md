# Chunk 482 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Vector loop with a pointer and a down-counter (0x0098E7D0).** Retail's `sub; sar 2; test; jbe` entry, then
  `add ecx,4 ; dec eax ; jne` over a local vector whose address escapes (push_back -> `_M_insert_overflow(this)`), needs
  the data pointer and the count in named scalars: `unsigned long* ids = v.begin(); unsigned count = v.size();
  for (i < count) ids[i]`. With `v[i]` / `i < v.size()`, VC7.1 reloads `_M_start` every pass. A `p != end` pointer
  loop is never turned into a counted loop.
- **Pending-map erase without distance (0x0098E7D0).** A retail body that calls `_M_upper_bound`, then `_M_lower_bound`,
  then `erase(first, last)` with no `distance` call is `m.erase(m.lower_bound(k), m.upper_bound(k))` (arguments are
  evaluated right to left). STLport's `erase(const key&)` keeps an out-of-line distance call (see 0x00990CD0 ->
  0x0098FF50).
- **x87 vector expression order (0x008FC430).** With inline member `operator+(const C3DVector&)` and
  `operator*(float)`, VC7.1 evaluates the right operand of `+` first and `this` first for `*`.
  - `centre + Y*r*cos(b)*sin(a) + X*r*sin(b)*sin(a) + Z*r*cos(a)` therefore computes the Z term, then X, then Y, and sums
    `(Y + centre) + X + Z`.
  - Writing the angles as `sin(theta * 6.2831854820251465)` inline, rather than as named double/float locals, keeps
    retail's order. VC CSEs the repeated sin, storing it twice (`fst` + `fstp`).
- **Unfolded float sum (0x008FC430).** An unfolded `fld [eps] ; fadd [0.5]` means the epsilon is not a compile-time
  constant: model it as `extern const float`. The literal `0.5f + 0.0001f` folds and flips the loop test to
  `fld x ; fcomp c ; test ah,41h ; jp`.
- **Postfix list iterator (0x008FC430).** `(*it++).first` on an STLport list iterator gives
  `mov eax,ecx ; fld [eax+8] ; mov ecx,[ecx]`.
- **Remainder through a named local (0x008FC430).** `long step = frame % density; ... (float)step / density` with
  `long density = points.ScanDensity` named gives `div ecx ; mov [esp+14h],ecx ; ... fild ; fidiv [esp+14h]`. The later
  `points.ScanDensity` is re-read after the `this->TriggerFrame` store.
- **Registry locals (0x0099E260).** HKEY / type / langID / size declared at function scope give retail's local order.
  `g = (WORD)(((v & 0xFC00) == 0) ? (v | 0x400) : v)` avoids a store-back into the address-taken local.
- **String compare stand-in (0x009AF1E0).**
  - A same-TU `__declspec(noinline)` body of `CBasicStringChar::operator==` (0x004115A0) under `#pragma auto_inline(off)`
    keeps the caller's string block in ebx across the call; /Oa does too, but breaks the rest.
  - `SkipPastWholeString("#definition")` written as an implicit conversion, not an explicit CCharString temporary, homes
    the temporary in the dead `text` argument slot.
- **inline exp() (0x008FA800).** VC7.1 /O2 inlines exp() (`fldl2e ; f2xm1 ; fscale`). A C3DVector user copy ctor is
  required to keep an accumulator vector on the x87 stack; an implicit copy forces a memory home and an int copy.
- **Inlined vector(n) constructor (0x00996610).** Retail's inlined `vector<unsigned char>(0x70000)` recomputes finish
  from a saved p (`mov edx,edi ; rep stosd ; lea eax,[edx+70000h]`). `memset; finish = p + n` CSEs the end-of-storage
  sum (443v449); memset's return + n gets 451v449.
- **Retail CThreadedFileManager layout.** The maps are STLport `_Rb_tree` (header +0, count +4, compare +8). DeviceQueue
  entries are 16 bytes: the map, then NextObject at +0xC. Objects is at +0x13C, CriticalSection +0x14C, Event +0x164,
  BlockPendingObjectProcessingRefCount +0x16C. `_Rb_global<bool>::_Rebalance_for_erase` (0x0042A34E) is the inline
  erase(iterator) helper.
- **CAFile TU (0x00997BB0, 0x00998010, 0x00998250, 0x00998A30).**
  - The CAFile path-parser TU is /Oa, and the retail CStringParser is 0x28 bytes: vptr, String +4, BaseLineOffset +8,
    Filename +0xC, TempCharString +0x10, list +0x14, ErrorCallback +0x18, CursorPos +0x1C.
  - An implicit `const char*` -> CCharString conversion builds the temporary before any argument push; an explicit
    temporary is built in argument order.
- **CAFile buffers (0x00998F60, 0x00999AB0).** With two `vector<unsigned char>(n)` buffers VC7.1 stops inlining the
  constructors: mark `vector(n)` / `_Vector_base(n)` `__forceinline`. Named `L""` locals in a forced-inline MakePath
  helper are built extension-first.
- **Fill char and last-index locals (0x0090B160, 0x00998C90).** A named fill-char local hoists its zero store out of the
  loop; a named last-index local inside the length test frees ebp.

Review at landing:
- **No row used more than 4 grades.** 009919c0 / 00991c10 are parked uncacheable-flag (volatile) candidates after
  1 grade.
- **009af1e0 mangled name.** It is landed through a `CDefinitionManager_Methods : CDefinitionManager` wrapper (the
  generated engine header has no methods), so its symbol is `?CompileString@CDefinitionManager_Methods@@...`, not the
  PDB `?CompileString@CDefinitionManager@@IAEXPBDABVCCharString@@@Z`. The same applies to the CompileDefinition callee.
  Check that the lander's relocation identity accepts the wrapper (landed rows use the same convention).
- **Local base models next to engine headers.**
  - 008f6c90 uses a local CHeightMapCell (the engine header is pack(1) with no constructor).
  - The CAFile rows use a local 0x28-byte CStringParser.
  - The parked UpdateScan rows use a local CPhysicsForceModelBase / CPhysicsCollisionPointsObject with retail offsets
    because the PDB offsets run high (Themes +0x44 v retail +0x2C, Ground +0x80 v +0x50, TriggerFrame +0xE4 v +0x9C;
    the three STLport lists are 4 bytes each in retail).
- **Already-landed CThreadedFileManager_RemoveObjectFromPendingMap_0098e770.cpp.** It models the pending map as a
  "VC6-era Dinkumware" {allocator byte, head, size} (head at +4). Retail is STLport with the header at +0 (the landed
  0x0098FE70 `_M_find`, 0x009905D0 erase and this chunk's 0x0098E7D0 all read the header at +0). The model compiles only
  because RemoveObjectFromPendingMap never touches the map's fields; fix the comment and model.
- **Already-landed 0x00998170 CAFile::ParserReadPastDriveLabel.** Its comment swaps CStringParser::GetString 0x009B9880
  and GetSymbol 0x009B9870.
- **Generated stand-in class names for STLport helpers.** Landed 0x0098FE70 (`CPendingObjectTree_0098fe70::_M_find`),
  0x0098FCA0 / 0x009901D0 (`CPendingObjectTree...`) and 0x009905D0 (`CRetailTree_009905d0::EraseRange`) are
  `std::_Rb_tree<CPendingObjectSortKey, ...>` members. Their mangled names do not match retail's `@std@@` instantiations.
- **Catalog row 0x0098E7D0** ("ProcessPendingRequests") is CThreadedFileManager::CloseAllStreamsForFile (one function,
  840 bytes). The UpdateScan rows 0x008FC430 / 0x008FC050 are one template body in two instantiations; land both from
  one source shape.
