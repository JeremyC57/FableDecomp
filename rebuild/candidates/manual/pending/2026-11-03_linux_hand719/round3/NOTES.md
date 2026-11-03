# Chunk 719, second pass (agent719b, 2026-11-03)

Builds on the previous agent's `2026-11-03_linux_hand719` records and its `hdr/` overlay (copied here and extended).

| Row | Result | Attempts |
|---|---|---|
| 0x00409730 CUserProfileManager::VerifySignature (1078, /GS) | DIFFER(1154v1078), parked `differ/00409730.cpp` | 14 |
| 0x00509A20 CWorldMap::GetMapNavigationAreaUpdateInfo (3120) | not attempted (budget); map below | 0 |
| 0x0040CEC0 CUserProfileManager::CUserProfileManager (977) | DIFFER(1015v977) unchanged; declaration-only growth specialisation tried | 2 |

## 0x00409730 VerifySignature
Flow: PathExists && IsPathFile; `CDiskFileWin32 file(path, 2, 2)`; Length < 16 fails (the one inline ~CDiskFileWin32);
`file.Read(header, 8, 0)`; isProfile = memcmp "FablePro", isSave = memcmp "FableSav"; `CFileDataInputStream stream(file, 0x4000)`;
version (save must be 0x2165), dataSize (`dataSize + 4 > Length` fails); `CArray<char> buffer(dataSize)`; SetPosition(0);
Read the block; crc; `CCRC::Calc(0, block, dataSize) == crc`; total = max(isProfile ? 0x4000 : 0x4B000, StreamPos) must equal
Length; pad byte ' ' (profile) or 0 (save) over the rest, read in 0x4000 chunks through `buffer.resize(0x4000)`.

Levers (each verified by grading):
1. **ReadType/ReadULONG/Read inlining.** VC7.1 expands the plain-inline `CDataInputStream::Read` / `ReadType<T>` /
   `ReadULONG` only inside a loop (LoadHeroStateBinary 0x00520670) or into a tiny caller; in a straight-line function it calls
   the COMDATs. Retail holds no out-of-line copy of any of them and expands them everywhere (also the traits reader
   0x0040FE60). New opt-in `FABLE_RETAIL_CDATAINPUTSTREAM_READTYPE` (hdr/engine/CDataInputStream.h) makes the three
   `__forceinline`; default and the existing FABLE_RETAIL_CDATAINPUTSTREAM branch are unchanged (hdrcheck2 default=same).
   An explicit instantiation does not help; an explicit specialisation fails C2908 (ReadULONG already instantiated it).
2. **Same-TU CCRC::Calc body.** 0x004014A0 is a header COMDAT (Ego_r 0x00401920). With its body in the TU, VC7.1 knows it
   writes no memory and keeps the buffer's `_M_start` in esi across the call (retail), which also lets the CRC and size
   failures share one epilogue.
3. **Nested CRC / size ifs with one trailing `return false`.** Gives retail's single false block 0x00409B34.

Residue: the in-loop pad mismatch `return false` gets ~CDiskFileWin32 expanded (a cleanup site nested in a loop), so that
exit keeps its own epilogue. A return in the outer loop is not expanded (probe), but every way of leaving the inner loop
without a return (break + test, flag, remaining>0 test) changes the frame or the layout.

## 0x00509A20 GetMapNavigationAreaUpdateInfo -- map for the next agent
`void CWorldMap::GetMapNavigationAreaUpdateInfo(long map, const C2DBoxI& area, const CCharString& section, CNavigationAreaUpdate& update) const`,
frame 0x128, no EH, `ret 0x10`. Retail CNavigationAreaUpdate (STLport): NavigationLayers +0x00 (vector of 24-byte
CNavigationLayerUpdate {vector<C2DLineF> LineList; vector<CSwitchableBlockage> SwitchableBlockages}), TransitionPoints +0x0C
(vector of 16-byte CNavigationTransitionPoint), ActionPoints +0x18 (vector of 12-byte CNavigationPosition), TopologyWeights
+0x24 (vector<unsigned char>). CSwitchableBlockage is 16 bytes in retail (UniqueID 8, list<C2DLineF> 4, pad).
1. `list<CThing*> things` (sentinel malloc 0xC); `CThingSearchTools::GetAllThings` 0x00516200 on World(+0x08)->+0x20 with a
   filter built from `CCharString(section)` (0x0099EC30) and `area` -- probably CThingFilter_HasNavigationInformation bound
   with an area filter (Ego_r ??RCThingFilter_HasNavigationInformation@@ 0x004E0840).
2. NavigationLayers cleared via 0x0051CAE0 / 0x0051CD80 (copy + destroy = erase(begin, end)).
3. Per thing: `layer = GetNavigationLayerAt(thing->GetPosition(), thing)` 0x0063E590 (this); `count = thing` 0x006A4F10
   (reads +0x60); grow NavigationLayers to layer+count (0x0051D870); if `(thing->+0x2C >> 25) & 1` (switchable):
   for each i, a CSwitchableBlockage {UniqueID = thing +0x18 / +0x1C & 0xFF, lines from 0x006A4F60(&vec, i) inserted into
   the list via 0x00517480 / 0x005152C0} push_back'd into layers[layer+i].SwitchableBlockages (inline STLport growth with
   0x0051A810 uninitialized_copy, 0x005199E0 copy, 0x00519EF0 destroy); else 0x006A4F60(&layers[layer+i], i).
4. Dummy objects: `GetAllDummyObjectNames` 0x004BDB40 on thing +0x64 into vector<CCharString>; names with length >= 12 whose
   GetSubString(0, 12) (0x0099EC70) equals the 13-byte literal 0x01244DB8 (the "" compare 0x0122D70E is the empty-string fast
   path of CCharString ==, 0x004115A0 / Compare 0x00411570; global 0x013BAC9C is a CCharString compared with the name);
   `GetDummyObject` 0x004C08C0(name, idx-or-0, 0, &pos, &rhs, 1.0f); ftol X/Y inside `area` -> ActionPoints.push_back
   {pos.X, pos.Y, GetNavigationLayerAt(pos, 0)} (growth 0x00517BA0). Temporaries guarded by a flag word (`or ebx,1` / `and ~1`).
5. Helper points: if thing+0x60 vfunc +0x174 and !(flags & 0x2000000): `GetPhysicsMeshObject` 0x006B0850 (dtor
   ~CPhysicsMeshObject 0x00A127D0), helper names 0x00A11820, global CCharString 0x013BAC94, transform 0x00A11760; inside `area`:
   name equal to the global -> TransitionPoints.push_back {X, Y, 0, 1}; else CStringParser 0x00404720 over the name,
   `FindKeyword` 0x009B9BA0 with 0x01244DB4 (and operator+ 0x0099F600), `ReadAsInteger` 0x009BA540 twice ->
   {X, Y, a-1, b-1} (growth 0x00510F60).
6. Topology: `TopologyWeights.resize(width*height)` inline (memmove 0x00BFEAE6 / insert 0x00485F3A),
   `ClipAndConvertWorldMapAreaToLocalArea` 0x004FC4D0, `GetMap(map)` (vfunc +0x4C), then per cell `CMap::IsPassableAt`
   0x0081F3D0 / `IsPrefNavAt` 0x0081F4B0 -> 0xFF (blocked), 0x00 (preferred), 0x80.

## 0x0040CEC0 constructor
The declaration-only explicit specialisation of `vector<CDisplayModeDesc>::_M_insert_overflow` keeps BOTH growths out of
line (814v977). Retail expands the first growth (inside the mode loop, which is why VC7.1 expands it -- the loop rule above)
and calls the second (after the loop); the open question is only why retail's in-loop expansion still calls
`__uninitialized_copy` 0x0040DDC0 out of line.

## Header overlay (hdr/)
- `engine/CDataInputStream.h`: FABLE_RETAIL_CDATAINPUTSTREAM_READTYPE (see lever 1).
- `engine/CDiskFileWin32.h`: FABLE_RETAIL_CDISKFILEWIN32 retail class (CAFile base, Ego_r 0x1C members, ctor 0x0099AD80,
  inline `virtual ~CDiskFileWin32() { Close(); }`), same text as the Codex pass 12 overlay's non-GFMAIN branch.
- `engine/CFileDataInputStream.h`: FABLE_RETAIL_CFILEDATAINPUTSTREAM (Codex pass 12 text): ctor (CAFile&, ulong) 0x00994700,
  virtual dtor 0x00994780.
- `fable_user_profile.h`: `static bool FABLE_FASTCALL VerifySignature(const CWideString&)` in the FABLE_RETAIL_CUSERPROFILEMANAGER class.
hdrcheck2: default=same for every file.
