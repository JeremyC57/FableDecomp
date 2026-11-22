# NF253 lessons (one line each)
- g3.sh-style graders that read only the row's `// Flags:` line grade a catalog-flagged row (/GS) without its flags: fall back to the catalog CompilerFlags, and stop the catalog lookup at the entry's closing `}` (a `grep -A8` can read the next entry's flags).
- A second `// Leaf:` line used as prose (a "Leaf (NFxxx): ..." note copied from a sibling) is read as the leaf by tools that take the first match: write it `// Name:`.
- A ??_G TU-emitter row whose class has a protected inline ctor takes the sibling ctor row's anonymous-namespace scaffolding verbatim: the hashed scaffolding names differ per TU, so the two objects link with no duplicate.
- Before porting a stand-in-free row "onto the header", map every UND symbol of its HEAD object onto reloc_bindings: when all already bind their address's table spelling, the port is local-model debt only (0x006648D0 / 0x00664A50).
- An engine header whose comment calls its offsets Ego_r may already carry retail layout (CMaterialDef.h: 12-byte STLport vectors, sizeof 0x98): read the asserts before keeping a local model for "header debt".
- A CWorld.h-before-CMainGameComponent.h include order is needed for CMainGameComponent::GetWorld() (CWorld.h includes CMainGameComponent.h at its end; the other order leaves CWorld undeclared).
- For a ctor row that will emit a class vtable for the first time, compare three things per slot: the class's retail target, the base's retail target (by RTTI), and the landed self Leaf at the target. A slot equal to the base's is the header branch's job; one that differs is the class's override; a landed stand-in self Leaf there is a split the row creates.
- vtmap-style RTTI lookups need the `@@` suffix on the class name (`.?AVName@@`).
