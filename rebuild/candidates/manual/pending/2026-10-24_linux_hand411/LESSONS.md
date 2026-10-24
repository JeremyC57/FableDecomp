# Chunk 411 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Reusing a bool parameter keeps it in bl.** `if (Get().ExitThread || error) error = true; Get().ExitThread = error;`
  emits `mov bl,1` on both true arms (0x009C3530). `ExitThread = ExitThread || error` materialises 0/1 in ebx instead
  and misses.
- **A dead byte load that survives a constant-true `||` means a volatile member.** When SetDirtyDiscError(true) is
  inlined, retail keeps `mov cl,[eax+7Ah]` (0x009C37A0, 0x009C3820). Every unqualified C++ form drops that load, so
  it is a lane gap for as long as volatile is banned.
- **An `and esp,-8` frame comes from an 8-byte local whose address is taken inside a loop.** In 0x009C4230 that local
  is the CCriticalSectionLock; VC7.1 3077 aligns the frame for it.
- **Tags and temporaries reuse dead argument slots.**
  - `_M_insert_overflow`'s `__false_type` tag goes into the key's dead argument slot (0x009B83B0).
  - map::insert's hidden iterator pointer goes into the key slot (0x009B7BC0).
  - A wrapped-text temporary is built in the dead `text` slot (0x009BC9D0).
- **CCharString compared inside the heap/sort helpers.**
  - Paste the landed CBasicString body with its `__declspec(noinline)` operator< (0x00429950).
  - Write the comparison reading `other` first: `if (other.PStringData == PStringData) return false; if (!PStringData) return true; ...`.
  - This fixed the load order in 0x009B86B0.
- **Whether the STLport vector copy constructor is inlined depends on the function.**
  - __uninitialized_copy 0x009B7320 inlines it as `_Vector_base(n)` plus a `__copy_trivial` memmove.
  - swap 0x009B7650 calls the out-of-line vector<unsigned long>(const vector&) at 0x00535470, and operator= at
    0x0045BC09.
  - Check the retail callees before choosing a model.
- **Explicit instantiation (`template void f(...);`) emits even an `inline` template body as its own COMDAT.** This
  is how the STLport helpers that STLport declares inline are emitted as rows, for example __linear_insert 0x009B8C00.
- **CVectorMap::Insert's push_back is inlined or called depending on the element type.**
  - For the 8-byte pair<CCharString,ulong>, push_back must be `__forceinline`; plain inline stays a call
    (0x009B83B0).
  - For the 24-byte pair<ulong,CDefClassInfo> instantiation, retail calls push_back out of line at 0x009B9170
    (0x009B9290).
- **map<ulong,CDefPointer<CDefClassBase>>::operator[] has a trivially destructible mapped type in retail.** Giving
  CDefPointer a releasing destructor adds a dtor block that retail does not have (0x009B7BC0).
- **STL algorithm TUs here are /Gr.**
  - Add `// Flags: /Gr`: first argument in ecx, second in edx, and the empty comparator in a stack dword.
  - `g.sh` does not apply the Flags line, so pass `--flags /Gr` when diffing. gall.sh honours it.
- **`last - first > 16` over 4-byte elements is `and eax,-4 ; cmp eax,40h`** (0x009B7CF0).
- **D3DSHADER_VERSION_MAJOR folds into a `movzx` byte load; D3DSHADER_VERSION_MINOR stays a dword load plus
  `and 0FFh`** (0x009BEDE0, 0x009BEE00).
- **CDefClassInfo is nested.** Its mangled name is `VCDefClassInfo@CDefinitionManager@@`, and Leaf lines must use
  that nested form.

Review at landing:
- Landed `CFileInstaller_Pause_009c31e0.cpp` names byte 0x013CA814 `g_installerNoPauseWait`. It is the static
  CFileInstaller::DirtyDiscError that SetDirtyDiscError 0x009C3530 writes: Pause skips its wait after a dirty-disc
  error.
- Landed `CTCTrophy_OnShowTrophyInitialise_009bd090.cpp` is really CXMVPlayer::LoadXMVCode (FableWin
  `?LoadXMVCode@CXMVPlayer@@QAEXXZ`).
  - It calls the static StaticLoadXMVCode 0x009BCF70, which the landed source models as `PrepareTrophy()`, then
    `++[this+54h]`.
  - It uses a `char pad[0x54]` struct.
  - 0x009BD0A0 is ReleaseXMVCode.
- Manifest labels to fix:
  - 0x0045BC09 `CActiveFile::operator=` is vector<unsigned long>::operator=.
  - 0x00535470 `Std_Vector_Construct_Copy` is vector<unsigned long>'s copy ctor.
  - 0x00A3A7E0 `C3DMeshInfo::HasPhysicsMesh` is CCompressorZlib::HasStreamInput as called from 0x009C4230. The label
    was propagated across identical bodies.
- Local models next to existing engine headers:
  - CDefinitionManager.h exists, but its CDefClassInfo is the Ego_r layout (24 bytes, CArray 16). Retail is 20 bytes
    (CArray 12), so the sources define the nested class locally.
  - CStringParser.h exists, but the parked 009b99c0 uses the retail 0x28-byte model of the landed siblings.
- No engine header exists for CFileInstaller, CDisplayManager or CXMVPlayer; those sources carry local layout models.
- The parked 009c37a0 and 009c3820 need a lane decision on volatile members: retail ExitThread is evidently volatile.
