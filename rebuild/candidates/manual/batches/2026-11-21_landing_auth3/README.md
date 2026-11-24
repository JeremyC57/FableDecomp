# Landing auth3 (Account A, 2026-11-21): 0x00C73050 CAudioSource::UpdateStream (1 row)

The first match among the ten rows released from Account B (owner: "keep working until they match"). AUTH1 / AUTH2 / AUTH3 worked it
(`pending/2026-11-21_auth1`, `_auth2`, `_auth3`); AUTH3 found the text.

- RELOCATION_MATCH 672 = 672 at `/d2inlT145`, test PASS. Catalog `CompilerFlags = '/O2 /Oy /W3 /d2inlT145'` (the lander's full form).
  `// Inline-budget:` line: T145 is the smallest that matches (T144 calls IsPlaying out of line, 600v672; T270+ expands the last
  SourcePlay, 720v672; T145..T250 match).
- The residue (the tail item in edi, retail edx) was c2 colouring order: the queue pointer (prio 23) was coloured before the prio-20 temps.
  Forcing the order with the c2 hook gave a RELOCATION_MATCH object (AUTH2). The natural text: SourceUnqueueBuffers' lazy AL resolver
  keeps the resolved address in a named `void* proc` before the member store, and still calls through the member. Queue's priority
  drops to 19.
  - `// Inferred: named resolved address --` on the 0x00E9E420 / 0x00A1E5C0 terms (advisor, owner delegated). It is plain C++ with no flag,
    type or helper invented. It is the smallest matching spelling: the local in SourceUnqueueBuffers only. The uniform variant (all three
    resolvers) also matches, but the bytes do not support the other two locals, and the landed Sourcei / SourceQueueBuffers keep the
    ternary. Without it: DIFFER(672v672), 3 lines. No Ego_d / Ego_r record exists for these classes (retail-only).
  - Validated on the helper's users: the same resolver text keeps the retail bytes of the out-of-line owners 0x00C575A0 / 0x00C576E0 /
    0x00C575F0 and of every landed inliner (0x00C57460 0x00C57AF0 0x00C57C90 0x00C57D80 0x00C72620 0x00C72800 0x00C72A40;
    `pending/2026-11-21_auth3/00c73050/cons/`). A local function pointer called directly also matched the row, but the owners contradict
    it (they reload the member: 69v70, 74v75), so it is rejected.
- Pick-any copies the object defines: SourcePlay 75, GetSourcei 70, SourceUnqueueBuffers 79 equal owners 0x00C575F0 / 0x00C575A0 /
  0x00C576E0 in bytes (relocations masked), and reloc_identity finds their targets consistent (GetProcAddress 0x00C4D740, IAT
  GetProcAddress 0x0143FE44). IsPlaying 91 / IsEmpty 23 / IsLooping 10 have no landed owner rows (queued as copy debt).
- Name: `CAudioSource::UpdateStream` (descriptive, as every earlier draft). It is vtable slot 9 of 0x012B8CEC / 0x012B8D6C / 0x012BEE64,
  where slot 8 is the landed CAudioSource::Update 0x00C72FE0; the retail tail is `ret 0Ch` (thiscall). The catalogue autoname
  `_global::Audio_PlaySourceAndProcessQueue` is replaced. GameCODA CodeView libs not checked here (queued).
- Lander warning: RELOC_NAME `?ReportQueueInfo@CAudioObject@@QAEXXZ` -> 0x00C432F0, whose catalog name is the placeholder
  `CMarkedBufferOwner_00c43810::ReportMark`. The row uses the spelling that the reloc_bindings witness 0x00C43A10 and landed 0x00C5B300
  already bind, so this is a pre-existing self-name split, queued. LANDVERIFY_STRICT_RELOCS=1 rejects on this label warning alone.
- Queued (tag land1121auth3): the 0x00C432F0 name, a re-port of SourceUnqueueBuffers' owner and inliners onto the new text, and the
  GameCODA name check plus the ownerless copies.
