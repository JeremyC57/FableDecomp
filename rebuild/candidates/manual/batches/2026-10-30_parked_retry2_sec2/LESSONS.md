# Retry wave 2, section 2 -- lessons (new rules)

1. **Same-TU mod-ref callee body.** When a parked row calls an out-of-line helper with the address of a local (a TC key,
   a def handle, an out-pointer) and the residue is frame-slot packing, a reload, or which register holds the value
   after the call, define the helper's real retail body in the TU as `__declspec(noinline)`. VC7.1 then knows what
   the callee reads and writes (the key does not escape; the handle is only written through `out`). The call stays
   out of line and byte-identical, while the caller's allocation changes to match retail. Wins: 0064a060 and 0076e2f0
   (const `CVectorMap::LowerBound` 0x004365B0), 00698a30 (real `GetSubDef<CEntitySoundDef>` 0x0065F604; pending a
   header ctor), 006ca380 (improved: the tail-jump shape came back). It does not help when the body is a stand-in
   (keep it the real body) or when the residue is pure colouring (0065cb6b: a `bitset::test` body made it worse;
   00739360: an STLport `lower_bound` body left it unchanged).
2. **PDB inline getters, written with retail parenthesisation, beat named locals.** CTCLook's private getters
   (Ego_r + FableWin: `GetHeadAngularVelocityMaxXY@AAEMXZ`, `GetHeadTurnRangeXY@ABEMXZ`) as inline bodies matched
   00752120. One getter body, `GetHeadTurnRangeXY() const { return 0.5f * (PDef->HeadTurnRangeXY * (1/360)); }`,
   matches three retail callers: 00752120, 00752510 (one getter call instead of the earlier named-local fix) and the
   arithmetic of 00752e00 (`Sign * GetHeadTurnRangeXY() * 0.5f` becomes `fmul 1/360; fimul; fmul 0.25`). The
   constant on the LEFT and the product in brackets is what stops VC7.1 from folding `* k * 0.5` into one constant.
   `x * k * 0.5f`, `x / 360 * 0.5f`, `0.5f * getter()` and a local inside the getter all fold or change shape. If
   several callers of one class show the same fold, look for one getter body that fits all of them.
3. **Flag sweep first for close rows.** The lander's own sweep (EXTRA_FLAGSETS x optimize pragmas) matched two rows
   that were parked as finished: 00717950 under `/Oa` and 0075a530 under `/EHsc` (the latter also needed the PDB
   class and base spellings). It is cheap and runs unattended, so start it in the background at the beginning.
4. **A parked `// Leaf:` that matches no symbol makes the grade meaningless.** q.py then grades the largest function
   (the WARN line only shows up in the sweep log). The sweep logged it for 15 rows: 00687540, 00687730, 006a64d0, 006af2d0,
   006af3f0, 006af7d0, 006b08f0, 006fe7c0, 0070e710, 0071d940, 00726160, 00756d00, 00766dd0, 00773790, 0077c960. The
   causes are struct/class spellings (U vs V), short leaves (`operator`, `Ctor`) that never matched a mangled name, and
   template or anonymous-namespace spellings. Check the leaf before
   trusting a parked grade.
5. **Keep counted-pointer models const-correct rather than swapping in a header whose shapes differ.** For 0075a530,
   `CIVCountedPointer<const CPhysicsMesh>` with a const ReleaseTemporary and a mutable count matched. Using
   engine/CIVCountedPointeeBase.h's Release/Delete shapes changed the bytes. That header also needs rebuild_abi.h
   included first (FABLE_STATIC_ASSERT).
6. **Respelling a CInterfaceMap model as `CVectorMap<ETCInterfaceType, CTCBase*>`:** the const LowerBound returns
   `const value_type*`, so the caller's iterator must be `const CInterfaceMap::value_type*` (otherwise C2440).
   Field names become `first`/`second` and `_M_start`/`_M_finish`.
7. Unchanged by any form: the RMW bitfield clear (`and byte ptr [esi+15ch],7Dh`, 00666310), the 12-byte struct copy
   interleave (007534b0), and the "load into eax then `mov ecx,eax`" before a thiscall on a null-tested member
   (00751da0, 00752300).
8. **Prefer a literal over an invented global for a merged `__real@` constant.** 00717950's double 0.0 inits read
   0x0122ED70 from memory. `0.0` in a member initializer list compiles to that same load and relocation
   (RELOCATION_MATCH), so it needs no name. Try the literal before binding a descriptive name to a constant address.
9. **Read the vland RELOC_NAME / RELOC_CONFLICT warnings before handing back.** They caught a callee bound under its
   byte-identical twin's name (GetWorld 0x00686A70 vs PeekWorld 0x00686A80), a class the PDBs do not have (CMeshBank)
   and a missing namespace (NGraphicAppearance).
