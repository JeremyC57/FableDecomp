# Chunk 439 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `new T[n]` in VC7.1 calls the SCALAR `operator new` (`??2`, 0x00BFEA1A) unless `void* __cdecl operator new[](unsigned int);` is declared in the TU; retail's 0x00BFEB22 is `??_U`. The scratch grader masks this (RELOCATION_MATCH either way): declare new[] whenever retail calls 0x00BFEB22 (0x00C46400, 0x00C49E00; 0x00C48F80 already did).
- Out-of-line template container dtor "remove the last until empty": `for (count = m_count; count; count = m_count) Remove(m_data[count-1]);` -- `while (m_count)` re-reads the count and swaps edx/edi (0x00C42AD0; agent 437 saw the same).
- bool predicates: `if (x) return false; return true;` gives the byte `neg ; sbb al,al ; inc al`; `return x == 0` computes in eax (`sbb eax,eax`, one byte longer) and `return x != 0` adds `xor eax,eax` (0x00C43530, 0x00C43390).
- A flag bit read as `(m_state >> 2) & 1` gives `shr edx,2 ; test dl,1`; a bit-field gives `test byte ptr [mem],4` -- libpk uses shift accessors (0x00C43390, 0x00C43570).
- `m_flags &= 0xFFFF0000` with the low half known zero in a register compiles to a `mov word ptr [esi+10h],di` store (0x00C436C0).
- if/else vs ternary picks registers: `level = caps & 0x80 ? 0 : -10000` put caps in edi; the if/else form keeps retail's ebx (0x00C449C0).
- `/Oa` is a per-TU flag in the libpk ambient emitter TU: without it ProcessEvents/AcquireSounds rotate registers after the first virtual call (0x00C44BD0, 0x00C44CE0); the cube and source TUs match without it.
- A by-value inline `T GetAt(i) const { return m_data[i]; }` (or a named local) puts the element load + push before the following `this` chain loads; a direct `ids.m_data[i]` argument schedules the chain first or loads into edx (0x00C44CE0, 0x00C47050, 0x00C470E0).
- A `__fastcall` hidden-result vector op followed by a member call: name the temporary (`C3DVector d = a - b; d.GetMagnitude()` -> `lea ecx,[esp+n]`); `(a - b).GetMagnitude()` passes `mov ecx,eax` (0x00C45810).
- MSVCR71 math through the IAT (`ff 15`): declare `extern "C" __declspec(dllimport) double __cdecl ceil(double);` -- `<math.h>` gives a direct call (0x00C46900).
- A list-drain dtor loop stays unrotated (GetHead at the loop top, `jmp` back) only with an inline `PopFirst()` (GetHead + Remove-if-nonnull) in `while ((n = PopFirst()) != 0) delete n;` (0x00C46860).
- A node allocator modelled as a BASE class makes VC7.1 emit a vptr store for the derived dtor; retail's CRegionMap holds it as the first MEMBER (0x00C46860).
- An inline constructor inside a `new T(x)` gets inlined; retail calls it -- declare the constructor out of line (0x00C47050).
- Two-slot vtables followed directly by the next vtable's `_purecall` slots are separate abstract bases, not longer vtables (region visitor 0x012B7D50 / copier 0x012B7D58).
- An out-of-line member that another member in the TU calls inline (CanUseHardwareVoice 0x00C43530 inside CanUseBuffer 0x00C43D30): define it in-class inline; the COMDAT copy is the out-of-line row.
- Retail inline-asm float-to-long (`fstp [esp+8] ; fld [esp+8] ; fistp dword [esp]`) in libpk SetGain 0x00C42F50: retail_asm, park after 1 grade.

Review at landing:
- No row needed more than 4 grades (max 2). 0x00C42F50 is parked as a retail_asm candidate (C++ reference body in differ/ with a passing test).
- All models are local (libpk has no engine headers). Shared local models across rows: CAudioObject (0x00C43000..0x00C43E20, 0x00C449C0), CAmbientEmitter (0x00C44B80..0x00C44CE0), CAmbientCube (0x00C45810..0x00C45940), CRegionMap family (0x00C46400..0x00C46900), CAmbientSource (0x00C46F90..0x00C47300). Keep the member offsets identical if they get a shared header.
- Already-landed sources whose names look wrong (all libpk, descriptive):
  - 0x00C4A0E0 "CEntranceStack" is CLockedRingBuffer::Pop (the locked ring of 0x00C49D50/0x00C49DD0).
  - 0x00C167B0 CBaseIntelligentPointer is an owned heap block, not an intelligent pointer.
  - 0x00C48DB0 is the PKCMessageQueue destructor; 0x00C49AF0 / 0x00C4BB00 are the PKCFrameQueue / PKCReadRequestQueue destructors.
  - 0x00C43F80 is CAudioObject::PlaySource.
  - 0x00C43370 landed as naked asm (purity: should be re-authored as C++).
  - CMarkedBufferOwner, CDrawAutoPickups and CBankFile models are all CAudioObject: "Resource" +0x50 is the voice, the "marked buffer" +0x54 is the CircularQueue, 0x00C43810 "Shutdown" is ReleaseQueue, 0x00C436C0 "Cleanup" is Reset.
  - 0x00C467D0 "CGuiVarTransferFloat" is the CRegionMap constructor (its wrapper.Bind is the CGridCircle temporary); 0x00C465E0 "DrawGetSeaLevel" is CRegionMap::GetCellSize; 0x00C465B0 "CIsThingAlive" is CRegionMap::ForEach; 0x00C46CE0 / 0x00C46D30 "CVtable012b7df8" are PKCTRList<SRegionNode>; 0x00C466F0's owner is the region collector visitor (vtable 0x012B7D64).
  - 0x00C349C0 "CVtable012b4fbc" is a CItemTable table with a CPtrArray member.
- Catalog rows holding several functions: none found.
