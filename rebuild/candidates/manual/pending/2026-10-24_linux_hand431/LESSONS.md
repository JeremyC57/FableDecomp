# Chunk 431 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- The 0x00C0xxxx library's CString is MFC text over a 12-byte CStringData. `ASSERT` is `if (!(c)) { if (g_pfnAssertHook /*0x0143BB2C, __fastcall*/) hook(#c "\n"); CDebugBreak(); }` with CDebugBreak @ 0x00C16750 as an empty temporary. The temporary is homed in the dead argument slot of a register-held argument, or in the `push ecx` frame word (`lea ecx,[esp+7]`) when there is none (0x00C0B8C0, 0x00C0B910, 0x00C0BDA0).
- MFC CString bodies that call `_tcsrchr`/`_tcschr`/`_tcsinc` must go through tchar.h's `_MBCS` static inline wrappers, not the `_mbs*` imports directly. Only the wrappers give retail's register choice and call-through-memory (0x00C0BC20 ReverseFind, 0x00C0D740 TrimRight).
- `a != b` on two float members loads the RIGHT operand first (`fld [b] ; fld [a] ; fucompp`). Write the compare as retail's operand order reads (0x00C0AF20: `m_time != m_duration`).
- POD `new T` plus three member stores: VC7.1 stores the first source assignment first, holds the second in edx and stores it last. Retail's store order +4, +0, +8 (edx loaded first) is therefore the source order m_time, m_data, m_type (0x00C0E9E0).
- A top-tested `while` loop that retail does not rotate (two loads of the entry feeding one shared `test eax,eax`) needs a `__forceinline` early-return condition helper (`if (!e) return false; return true;`). Plain `while (p)`, `for (;;)` and `for` with a `return` all rotate it (0x00C10CD0).
- `for (;;) { T* found = 0; for (i = 0; !found && i < n; ++i) ...; if (!found) break; Destroy(found); }` reproduces retail's duplicated destroy arms. The `do { ... if (found) Destroy(found); } while (found)` form merges the arms and swaps registers (0x00C0EC90).
- `bool r = false; if (a < b) r = true;` gives retail's `xor dl,dl ; ... mov dl,1`; `bool r = (a < b)` branches both ways (0x00C0B7B0).
- Early-return float range tests (`if (x >= hi) return x - w; if (x < lo) return x + w; return x;`) reload the argument for each return, as retail does; the if/else-assign form keeps it on the x87 stack (0x00C0B4C0 vs the inline register form in 0x00C0B510).
- In x87 code a CSE'd member (`m_time` read twice) instead of a named local changes the operand order (`fld st1 ; fmul st1`). Try both forms before parking (0x00C0B7B0).
- VC7.1 in this lane canonicalises x87 sum order whatever the source term order is. Retail's accumulate-into-a-middle-slot (`fxch st3 ; faddp st3`) cannot be reached: park as a lane gap after 2 grades (0x00C0B7B0).
- MFC `operator!=(const CString&, const CString&)` + an inline `operator LPCTSTR` + an explicit `!= CString("")` gives retail's conditional temporary, whose text is read through the ctor's eax (`mov edi,eax ; mov edi,[edi]`). Declare all three MFC `operator!=` overloads or VC7.1 reports C2666 (0x00C101D0).
- A local 8-byte {dispatcher, item} struct constructed inside a loop, with its address taken, reproduces retail's `and esp,-8` frame under 3077 (0x00C10400). This is consistent with the chunk-232 note: it only looks like a lane gap when retail has no such frame.
- Calling a virtual through a member OBJECT (`m_items.Enumerate(&v)`, `m_index.Find(h)`) stays a virtual call in VC7.1, so a member typed list or container can be modelled as an object member at its offset (0x00C10400, 0x00C101D0).
- In a local typed list (vtable 0x012B1B78) the dtor's base-vtable store after the impl dtor call is dead and dropped. The derived visitor's own vtable store before its base store is dropped too, so write the inline dtors naturally and the retail tail falls out (0x00C10400).

Review at landing:
- No row needed more than 4 grades. 0x00C10CD0 took exactly 4 (3 DIFFER, then MATCH).
- Local models: CString, CStringData and CScopedLock are modelled locally in every source. There is no engine header for this library; if one is generated for the 0x00C0xxxx CString / CScopedLock, retype these sources onto it. CScriptHost (vtable 0x012B25A8) is modelled locally, next to the landed CScriptHost_00c0e600.
- Landed 0x00C0DEA0, named "operator&&", is really `CString::operator+=(const CString&)`: it calls Append (0x00C0DCF0) and returns `*this`.
- Landed 0x00C0DBB0, "CNamedRecord_00c0dbb0::JenkinsHashOfName", is `CString::GetHash() const`. The hash may be Jenkins, but the class is the library CString.
- The audio owner class is named three ways in landed sources: CCreatureActionHeroJump_00c09380, CPixelShader_00c091f0 and COwner_00c0b380. The parameter block is landed as CAbilitySoundComponent_00c0b200. Unify them (this chunk uses CAudioOwner / CAudioParamBlock / CRampedParam).
- The landed CBitMaskPair_00c0a8c0 comment has the mask roles swapped. The FIRST stack mask is the one set or cleared; the SECOND is OR-only (touched), as in 0x00C0AE70.
- Landed 0x00C0BCC0 is named SafeStringLength in one source and CString::SafeStrlen in another.
- Landed CVtable012b1b78 ctor/dtor (0x00C0A610 / 0x00C09B70) model the typed list with address-named classes. This chunk's 0x00C10400 uses CTypedListBase / CItemList / CListImpl for the same layout; pick one name set.
