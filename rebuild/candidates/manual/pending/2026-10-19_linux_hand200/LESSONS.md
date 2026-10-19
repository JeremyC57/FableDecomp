# Chunk 200 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A jump-table switch with NO range check (`mov eax,[esi+18h] ; jmp [eax*4+table]`) is `default: __assume(0);` over a closed enum; VC7.1 lays case bodies out in SOURCE order, so write the cases in retail's layout order (CPersistContext::Transfer rows lay out 4,3,2,1,0: 0x412DB0, 0xCDCA70).
- A function-scope local whose constructor only zeroes a member (CPersistTraits<T>{T DefaultVal}) is stored once before the switch jump even though only one arm uses it (0xCDCA70 family `mov [esp+14h],0`).
- STLport inline `clear()` = `erase(begin(), end())` must go through an `erase(T* first, T* last)` with parameters; reading `_M_start/_M_finish` straight inside clear() changes the register choice of unrelated switch arms (0x412DB0). The random_access_iterator_tag temporary needs a user ctor or VC stores a zero byte.
- A two-way persist split `mov eax,[ebx+18h] ; sub eax,2 ; je` is `switch (Mode) { case 2: ...; default: ... }`; an `if (Mode == 2)` gives `cmp dword ptr [ebx+18h],2` (0x67D6B0).
- A stream read whose result is ignored (no load from the chunk, only the pointer/count/position updates) is a by-value inline `ReadULong()` with the result dropped; a named address-taken local keeps the memcpy store and stops the size constant-folding (0x404500).
- A CRC of `CCharString(name)` with two separate block null tests and the ctor's returned this reused is an inline `GetCRC(const CCharString&)` called on the temporary; a named string local merges the tests (0x404500).
- A by-value C3DVector parameter of an inline dot helper (user copy ctor) loads all three components onto the x87 stack before the products (`fld [eax] ; fld [eax+4] ; fld [eax+8] ; fmul ...`) (0xA422C0).
- `for (i = 0; i != 3; ++i)` gives the strength-reduced `lea eax,[esi-2] ; cmp eax,3 ; jne`; `i < 3` gives `jl` (0x815770).
- In an entity-script Main, a returned CScriptThing that lives across the wait loop gets its own frame slot only at function scope (`if (IsDone()) return; CScriptThing effect = ...;`); inside an `if` block it shares the slot of the earlier filter temporary (0xD2D830).
- `if (ok) { ...; return true; } return false;` puts the shared false epilogue last (0x487C20, 0xA422C0).
- A same-TU helper body defined above the caller under `auto_inline(off)` turned a reloading loop into the retail register-held strength-reduced loop even for a float-returning const helper (0xB2B050, not yet matched).
- Wrong vector-size layouts: when a caller passes `this+N` for a vector member, compare N with the PDB offset (PDB -4 per earlier STLport vector/CVectorMap) to recover the element type and the real template argument (0x67D6B0/0x7D2740).

Review at landing:
- 0x412E80 and 0x7B45A0 are catalog rows that hold an extra function (two 9-byte stubs; a 3-byte `ret 4`); their Transfer bodies grade identical -- sources in differ/ ready for re-cut rows.
- 0x13B86A0 is spelled both `g_pMainGameComponent` and `g_pEngine` in landed sources; 0x487CF0 (parked) uses g_pMainGameComponent, which is PDB-correct (it is read as CMainGameComponent::PDisplayEngine +0x28 -> PViewManager +0xE0).
- 0x129BA3C has four landed spellings (g_minLength_0129ba3c, g_epsilon_0129ba3c, g_navigationEpsilon_0129ba3c, g_fZero -- the last is wrong, the value is 1e-4f); parked 0x814EA0 uses g_epsilon_0129ba3c for both the radius test and eps*eps (one symbol, one address).
- 0xF35B10 RegisterFilter: landed callers declare it on each script class with `CIsThingAlive&`; 0xD2D830 declares it `const CIsThingAlive&` on CPermanentBarrier because it passes a temporary.
- Local models next to existing engine headers: CNavQuadTree (0xA7A7F0; header is the debug 16-byte-vector layout), CPlayer +0x216 InitialPlayerModeSet reached through the header pad as landed CPlayer::Update does (0x487CF0 parked).
- 0x00A84110 is reused under its landed name CRetailList_ListOfCountedPointer::clear (it is list<CNavRegion>::clear).
