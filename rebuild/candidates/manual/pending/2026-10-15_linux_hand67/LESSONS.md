# Chunk 67 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- x87 matrix-vector sums written directly in the caller are reordered to start with the X term whatever the source
  order; retail's Z-first order comes out only from an inline `CMatrix3x4` member taking `const C3DVector&` and
  returning a named local (00a11ec0).
- A 12-byte struct copy: the compiler-generated assignment gives retail's `mov edx,ecx` plus an esi temporary; a
  hand-written memberwise `operator=` copies through edx straight off ecx (32 v 36).
- `Member == call()` with the member loaded before the call and re-read after = an inline helper that copies the member
  into a local first (as `IsMainPlayer`).
- A loop reading the argument's data once, after the entry test = a rotated loop `if (it != last) { key = name; do ... while }`.
- Retail layouts: `CPhysicsMesh` keeps the PDB member order with 12-byte containers; in `CPlayer` the mode list at +0x210
  is a 4-byte head, so every later member sits 8 bytes below the PDB offset (AggressiveMode +0x217, JoystickDeviceNumber +0x228).
- Matching neighbour order between `ghidra_out/egor_pdb_names.tsv` (Ego_r.pdb) and retail recovered 10 propagated labels.

Review at landing: parked 00a18760 and 00c21330 keep `this` in edx/ecx across a callee (LTCG register-preserving lane
gap, as the 00c20cb0 family); 00c21330's identity is doubtful (`ret 4` while the PDB version takes no argument).
00487aa0: try `number != mainDevice ? number : 0` in the else branch. Label corrections with evidence in NOTES.md
(e.g. the three "~CPhysicsMeshInfo" rows are ~CPixelShader 00987e20, ~CBaseObject 0099a430, ~CVertexShader 009be1e0).
