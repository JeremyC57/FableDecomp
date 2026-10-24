# Chunk 426 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `delete (T*)data` inside a static `CCountedPointer<T>::DeleteData`, for a class with a user-declared inline
  destructor and no vtable, makes VC7.1 emit the class's non-virtual `??_G...QAEPAXI@Z` as its own COMDAT. One source
  then lands both rows by changing only the `// Leaf:` (0x00AECA50 DeleteData and 0x00AEC9F0
  `??_GCParticleEmitterManager`).
- Return order: `if (it != last) return value; return 0;` keeps the value return on the fall-through and merges every
  `return 0` (including one inside a loop) into one block at the function end. `if (it == last) return 0; return value;`
  splits them (0x00AF4360 79v79; 0x00AF28C0).
- The in-class inline `CVectorMap::Add` (push_back + `Dirty = true`), called from a member, stays out of line under /O2
  (24v137). `__forceinline` on Add gives the retail inlined body (0x00AE2D90).
- vector::push_back: when the `_M_insert_overflow` body is in the TU, VC7.1 auto-inlines it (0x00AE83D0). When retail
  calls the overflow, declare it without a body (0x00AF39C0 -> 0x00AF37F0).
- STLport `__uninitialized_copy` instantiations are reached from vector internals with plain `T*` (non-const)
  iterators. Read the callers (`_M_insert_overflow` copies out of `_M_start`) before choosing const: the bytes are the
  same, only the mangled name differs (0x00AF1C10).
- The ret count shows the real signature. Retail 0x00AE2510 is `ret 0Ch` = ApplyConstraints(sim&, long, float), so the
  float the landed caller pushes earlier is that function's third argument, not a value for a 2-argument
  ExecuteLooped.
- GF maths `a * (1.0f - t) + b * t` over inline C3DVector `operator*(float)` / `operator+` matched on the first grade.
  The right operand of `+` is evaluated first (b*t, then 1-t) (0x00AF3CF0, NParticleEngine::GFInterpolate). Not every
  GF row is the register-copy lane gap.
- CDynamicObjectHeap<N,M> constructors: model them as the real template (`template <long ObjectSize, long BlockCount>`,
  nested CObjectEntry / CObjectAllocatorBlockEntry). The landed non-template shape is reproduced exactly and the leaf
  becomes the PDB `??0?$CDynamicObjectHeap@$0BI@$0DFF@@@QAE@_N@Z` (0x00AF8700).
- A ReleaseData that ends in `Block = 0; Data = 0;` and a by-value CCountedPointer parameter (the `dec ; jne` destructor
  form): a probe that leaves out the zeroing differs. The zeroing version is the one retail runs (0x00AEAB20).
- The landed fixed-heap twin rule applies here too: in a constructor, the bit-field stores and body assignments are
  scheduled before the vtable store / after the base constructor in BODY order (0x00AEBA90, 0x00AF8700).
- When a PDB name exists, use the exact mangled name as the `// Leaf:` and make the model mangle to it. `struct` vs
  `class` (`U` vs `V`) and the namespace of a parameter or return type change the symbol without changing a byte: a
  prefix leaf graded MATCH while the symbol was wrong. 0x00AE1210 needed `class CArray` / `class C3DGroup2`
  (`AAV?$CArray@_N@@`); 0x00AF28C0 needed `NParticleEngine::CPSCBase` (`PAVCPSCBase@2@`).

Review at landing:
- No row needed more than 4 grades (the most was 3: 0x00AF28C0).
- Local models next to an existing engine header:
  - CParticleProgram (0x00AE0680 / 0x00AE1210 / 0x00AE2510 / 0x00AE2D90) and CDataOutputStream (0x00AE0680): the
    generated headers are opaque Ego_r structs with no member functions.
  - CParticleEmitter (0x00AECD70): engine/CParticleEmitter.h has the Ego_r offsets, 0x0c high (MaxDrawDistance +0x168
    vs retail +0x15c), and is not in NParticleEngine.
  - CIVCountedPointeeBase (0x00AF1AA0 / 0x00AF1C10).
  - All of these use `class`, so the verifier's `struct X {` header check does not fire. Decide whether to regenerate
    the headers with the retail offsets.
- Rule conflict: EXTRA_RULES line 17 says STLport `pair` is `class`. STLport 4.6's own `_pair.h` declares
  `struct pair`, and 155 landed sources use `struct pair` against 76 using `class`. These sources use `struct pair`
  (mangled `U?$pair`). There is no retail symbol to settle it.
- Errors in ALREADY-LANDED sources:
  - 0x00AE2D20 / 0x00AE2CB0 CParticleProgram::SimulationStep model `ExecuteLooped(sim, GetStepCount((float)dt))` with
    2 arguments and an ExecuteLooped that takes a float in slot 4. The callee 0x00AE2510 is `ret 0Ch`
    ApplyConstraints(CParticleSimulation&, long iterations, float tolerance): the earlier-pushed float is its third
    argument. The bytes coincide; the model is wrong.
  - 0x00AE5FB0 / 0x00AE71A0 (and 0x00AF6120 UpperBound, 0x00AF6980 UninitFillN) describe the code as Dinkumware
    (`_Uninit_fill_n`, `upper_bound`). Retail is STLport (`__uninitialized_fill_n`, CVectorMap::UpperBound).
  - 0x00AF6980 is `_STL::__uninitialized_fill_n<pair<EPSCType, CComponentInstance>*>`, the fill twin of 0x00AF6920.
    It is landed as a non-template `UninitFillN_SPairEPSCTypeCComponentInstance_00af6980` with a by-value
    "CRetailAllocator" (in fact the `const __false_type&`).
  - 0x00AF2C00 is landed as a non-template `std::__uninitialized_fill_n`. Its "Caller: the combat-wheel ring resize at
    0x00AF37F0" is vector<pair<CCharString, CRegisteredPSCInfo> >::_M_insert_overflow.
  - 0x00AF8B40 is landed as a non-template `CDynamicObjectHeap_116_529` (leaf `??0CDynamicObjectHeap_116_529`). It is
    the template `CDynamicObjectHeap<116,529>` (RTTI 0x0129F650 `.?AV?$CDynamicObjectHeap@$0HE@$0CBB@@@`), so its
    mangled name is wrong.
  - 0x00AED2F0 CParticleEmitter::IsVisible calls its callee "CalcFadeAlpha". The PDB name is
    `?GetFadeAlpha@CParticleEmitter@NParticleEngine@@QBEMM@Z` (0x00AECD70, this chunk).
- Catalog rows holding several functions: none in this chunk.
