- A shared-header inline accessor is not always byte-neutral. `CMainGameComponent::GetConstantFPS()` (static `__fastcall`,
  inline) makes VC7.1 copy the value to a stack temp before `fild` (7 rows DIFFER). Retail's `fild [0x01375550]` needs a direct
  read of the static. Grade every ConstantFPS respell.
- A bool `return a==b` after an early `return true` compiles to `xor ecx,ecx; sete cl; mov al,cl`. Retail 0x004874D0
  has `sete al`, so a bool spelling may not reach a char-returning stand-in's bytes.
- `CDefPointer<const T>` in a row-local model: the release path calls non-const members on a `const T*`. Make the local
  def model's release chain const (count `mutable`); codegen is unchanged.
- When a row gets both LowerBound overloads (0x0040F020 / 0x004365B0) with the same key type, constness of the map picks
  the overload. Callers that retail sends to 0x004365B0 need a `const CInterfaceMap*`.
- lb.py/lb2.py add `namespace std { struct pair }`. Rows that already define `std::pair` later then fail with C2953;
  keep one definition.
- A VC71 behaviour test links the row object (`sobj`). If the test mocks a callee, a callee respell needs the test's mock
  respelled too (LINK_FAIL on 0089xxxx CKeyPair rows).
- `FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE` needs `FABLE_RETAIL_BASE_OBJECT` or the 0xa8 size assert fires (C2118).
