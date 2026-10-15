# Chunk 97 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A float scale written inline, `(max - min) * 0.001f * (float)r + min`, is reassociated by VC7.1 (r multiplied first, the
  constant last) whether the constant is a literal or an extern and in either operand order; retail's
  `fld ; fsub ; fmul [c] ; fild r ; fmulp ; fadd` needs the scale in a NAMED float local (`float step = (max - min) * 0.001f;
  return step * (float)r + min;`) -- 0x0080F130.
- A global loaded into a callee-saved register BEFORE a call and multiplied after it (`mov edi,[g] ; call ; sub ; imul eax,edi`)
  is a named local read first; written as an operand (`g * (Frame() - t)`) VC folds it into `imul eax,[g]` after the call -- 0x0080F210.
- A C3DVector member copied word by word through ecx/edx/eax in a constructor init list is a MEMBERWISE user copy constructor
  (`X(o.X), Y(o.Y), Z(o.Z)`: plain float copies stay integer moves); the implicit copy of a float POD goes through `lea eax,[esi+4]`
  -- 0x006A06E0.
- The wave-25 STLport-vector lesson holds beyond the `lea ecx` shape: zero words stored AFTER a vtable store (an inlined `new T(x)`,
  0x004DF365), a `mov ecx,edi` scheduled after a vector's zero stores (0x007CC7D0) and a malloc's `add esp,4` scheduled before a
  CArray's last zero store (0x00BAA250) all reproduce only with `_Vector_base(const allocator&) : start(0), finish(0),
  end(allocator, 0)` over an `allocator_type()` temporary; three plain zeroing words schedule differently.
- An init-list float stored LAST as an immediate after all other stores is a body assignment (FOV = 72.0f in 0x0075C180; in the
  initialiser list it is stored right after the vtable).
- A `__fastcall` SGI `for_each` whose functor is an EMPTY class with a user constructor (hidden-pointer return) homes the functor
  temporary and the returned copy in the caller's dead argument slot, so retail appears to "reload the argument"
  (`mov ecx,[esp+8] ; push ecx`) -- it is copying the empty temporary; a functor that really holds the argument keeps the argument
  in ebx instead -- 0x007F8B60.
- `bool CBasicString<char>::operator==(const CBasicString&) const` (0x004A93C0, PDB `??8?$CBasicString@D@@QBE_NABV0@@Z`) is
  `if (StringLength != other.StringLength) return false; return Compare(PData, other.PData) == 0;` -- the early false is `xor al,al`,
  the compare result `neg ; sbb eax,eax ; inc eax`; the 71 callers inline CCharString's null/same-block tests around it.
- DirectShow base classes are linked into Fable: 0x00CB1E50 CMediaType::Set (FreeMediaType inlined: CoTaskMemFree via the import,
  `pUnk->Release()` as a `__stdcall` slot 2; CopyMediaType @ 0x00CB1BD0 is WINAPI), the forwarders @ 0x00CB1FE0/0x00CB2000 are
  `operator=`; AM_MEDIA_TYPE pUnk +0x3c, cbFormat +0x40, pbFormat +0x44.
- A `long` predicate `if (flag && Call(x)) return 1; return 0;` gives `mov eax,1` + a shared `xor eax,eax` tail (0x008640C0); a
  bool `if (p && !(p->m_flags91 & 1)) return true; return false;` with ONE GetPItem in a local gives `mov al,1` / `xor al,al`
  (0x007E8420); a long equality helper negated into a bool (`return !IsSame();`) duplicates `xor ecx,ecx ; test eax,eax ; sete cl ;
  mov al,cl` into both arms (0x00C384E0).
- A zero-initialised result local assigned in an if/else-if chain (`long r = 0; if (A) r = x; else if (B) r = y; ... return r;`)
  gives `xor edi,edi` up front and a separate `pop ; pop ; ret` epilogue per arm (0x00766480).
- A function-local static from a global (`static long s = g * 5;`) reproduces the VC7.1 guard (`test al,dl` on the guard bit,
  `or` + store, `lea eax,[eax+eax*4]`) -- 0x007BBA70.
- The CTC component lookup with an UNTESTED out-parameter (`GetInterface(&c, 0x13); Use(c->...)`) reproduces retail's read of the
  uninitialised shared frame word on the absent path (0x007CC410) -- do not null-initialise or test it.

Review at landing:
- Line-1 names that differ from the catalogue (Leaf lines carried): 00a3c330 `ClearCombinationBlends`, 00788d68 `pair`
  (member-template ctor with an explicit instantiation; objdump shows the single `??$?0...pair` symbol), 004a93c0 `??8`,
  0075c180 `??0CTCCameraPointDefinitionBase`, 00baa250 `??0CEngineInternalPrimitiveMorphInfo`, 007e1ac0 `??0CTCInfoDisplay`,
  00cb1e50 `Set`, 008640f0 `PeekWitnessWillOverrideFactionEnemy`.
- Local base models next to an existing engine header: `rebuild/include/engine/CTCBase.h` exists, but it models the vptr as a
  `void*` member and cannot be a polymorphic base; every CTC row uses a local `CTCBase_<addr>` (virtual + ParentThing + 4 bytes).
  No engine headers exist for the other classes in this chunk.
- `#pragma pack(push, 1)` is used for real unaligned PDB layouts (CTCOwnedEntity::POwner +0x0d, CTCCameraPointDefinitionBase
  CoordBase +0x16, the type-0x13 component's pointer +0x141), not to hide layout.
- ALREADY-LANDED sources to fix: `rebuild/src/compiled/00/cb/CComHolder00cb1cc0_Release_00cb1cc0.cpp` is DirectShow
  `FreeMediaType(AM_MEDIA_TYPE&)`; `rebuild/src/compiled/00/44/CTCNoiseDef_CTCNoiseDef_0044c1bb.cpp` names its base 0x0044C0C0
  "CShoveHaPennyRegionDef" -- it is CDefClassBase's constructor (bit-field clear +0x24, PDefManager +0x1c, vtable 0x01231D54);
  `rebuild/src/compiled/00/78/global_AddChildPrimitive_007898f0.cpp` (returns 50) is CTCMoneyReceiver's default amount getter.
- No row needed more than 4 grades; two parked (00749810 split byte RMW, 0071a850 esi/edi colouring). No catalogue row holds
  two functions.
