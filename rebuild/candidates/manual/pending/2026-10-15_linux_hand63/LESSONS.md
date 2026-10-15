# Chunk 63 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- No vtable store comes from an implicit destructor: VC7.1 drops the vtable reset in a compiler-generated destructor, so a
  `??_G` with no vtable store over a polymorphic base = the class declares NO destructor (0x00953A40); `~T() {}` stores it.
- `push edi ; lea edi,[esi+4]` hoisted above the null test in a deleting destructor = the member is an object with its
  own user destructor (its `if (m_data) free(m_data)` body, then its first member's destructor through edi; 0x009B6C50,
  0x00A5E7B0); flattening those members gives a late `lea ecx`.
- Flag tests needing an inline bool accessor: `shr eax,1Fh ; not al ; test al,1` = `bool IsRunning() const { return !m_bit; }`
  (0x00C3F3D0); `xor edx,edx ; mov dx,[..] ; shr edx,N ; test dl,1` = a bool accessor over a `ushort` bit-field
  (0x00AED290); testing the bit-field directly folds to `test byte ptr [..],imm`.
- Name lookups with the argument's data pointer hoisted before the loop and `last` kept in a register across calls need
  `/Oa`; to keep `add eax,6Ch ; mov eax,[eax]` the CCharString `==` must be a FREE inline
  `operator==(const CCharString&, const CCharString&)` re-reading both `m_pData` at every use (0x00AEE180 / 0x00AEE1E0).
- A virtual call returning a small class by hidden pointer, then compared: store it in a NAMED local (the call precedes
  the argument push and `this` of `==` is a `lea` of the dead argument slot; 0x00815820).
- Retail layouts (the generated `engine/CParticleEmitter.h` is wrong from +0x14 on): CParticleEmitter UpdateParameters
  +0x14 (0x24 bytes), PrimitiveGroupHandle +0x38, bit-field bytes +0x40/+0x42/+0x44/+0x45, Transform +0xb0, sizeof 0x16c;
  CParticleSystem Particles/Decals lists +0x1c/+0x20, counts +0x48/+0x4c, bool bit-fields +0x50, Name +0x6c, sizeof 0x70.
- Flags: 0x00C3F3D0 `/EHsc`; 0x00AEE180 / 0x00AEE1E0 `/Oa` (both in the lander's sweep, recorded as `// Flags:`); the six
  def-module rows use `#pragma optimize("s", on)`.

Review at landing: 0x00AEE180 matched on a 5th grade (the shape already proven on its byte-identical twin 0x00AEE1E0).
