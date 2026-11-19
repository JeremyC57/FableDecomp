# NF94 lessons (round w22)

- Observed once (0x005E1B60, batch b8): a `// Flags: /Gr` line I added to a row whose catalog entry has no /Gr was not applied by
  vland --recheck-landed (it graded "the largest function", DIFFER 128v126), while my helper honoured the line. Landed rows whose
  catalog flags already carry the option (0x005E1210, 0x0077EDA0) are unaffected. Spell the calling convention in the text
  (FABLE_STLP_FASTCALL bodies such as FABLE_RETAIL_STLP_HEAP) instead of adding a Flags line, and grep vland logs for
  "matches no symbol".
- A family whose members bind each other by local, non-template spellings (0x005E1C30 / 0x005E2860 / 0x005E2F40 / 0x005E37D0;
  the particle heap 0x00769470 / 0x00769D50 / 0x0076A480 / 0x0076AF10; CSalSwFilter) cannot be moved one row at a time: the
  first port changes its own leaf and breaks the next row's binding. Block it as one family port and leave a graded draft.
- `return a > b;` with a bool result gives the full-register `mov eax,1` / `xor eax,eax` exits; `if (...) return true; return false;`
  gives `mov al,1` / `xor al,al` (0x00875120).
- `find(x) != end()` in one expression evaluates end() first (VC7.1 right-to-left) and loads it before the call; name the
  iterator first (0x0051F010).
- A shared-header class can replace a local model even when a header inline body does not match: keep the row's own
  out-of-line text and turn a former member helper into a file-local inline function, passing members by reference
  (0x0063EF50: by-value radius DIFFER 103v95, by reference RELOCATION_MATCH).
- Some lane TUs need both fable_compiler.h (FABLE_FORCEINLINE) and rebuild_abi.h (FABLE_FASTCALL) before stlport headers.
- CODAApi.lib (sdk/gamecoda/tree/CODA/lib/Win32) carries the whole GameCODA member name set: `strings` on it settles names
  that neither PDB has (CSalSwFilter, CSalIIRFilter, sal_levelToGain).
- Shell: st.py with a last argument `-` reads stdin, and `grep pattern $f` with an empty $f waits on stdin; give every helper
  call `</dev/null` (or a heredoc) so a Bash call cannot hang.
