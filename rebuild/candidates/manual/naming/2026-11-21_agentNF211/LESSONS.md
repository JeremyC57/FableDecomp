# NF211 lessons (one line each)
- A pick-any COMDAT may match a landed row that sits under a stand-in name. Before reporting "no owner", search Fable.exe for the COMDAT's masked bytes. ??0CCombatDebugInfo@@QAE@XZ hit only 0x006F2BD0, landed as `SNamedZeroRecord`.
- The binding table keeps one witness per symbol. An exe scan (refs.py) of the callee found 0x006ED9E0 as a second IsCombatant binder that the brief did not list. Run it before claiming a split is closed.
- A declaration-only opt-in can be an `#elif` branch of an existing OUTOFLINE block. The out-of-line body stays under its own macro, so the default path is untouched and the new macro emits nothing.
- When a copy and its owner use different symbol names (a stand-in owner), cmpbody cannot pair them. Use body() with two names, then compare the relocation lists by hand.
- Copied helper scripts can carry another agent's paths (pre.sh pointed at agentNF130's overlay). Grep them for `agentNF` before use.
