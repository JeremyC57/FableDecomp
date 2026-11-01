# Chunk 690 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A catalog row that "ends at the catch funclet" is short: VC7.1 emits the catch funclet inside the function and the body continues after it (CBankFile::Update 0x009CEC60 runs to 0x009CF399, 1850 bytes, not 1684); grade such rows against the bytes up to the final `ret`.
- VC7.1 inline CArray dtor that re-reads m_last each element iteration does not match retail; cache `T* last = m_last` before the destroy loop (0x00B3B8E0).
- Member-object virtual calls that retail makes through the vtable (`mov eax,[esi+X]; call [eax+0x14]`) need a pointer (`CShaderDataBank* bank = &ShaderBankPixel; bank->Close();`); `ShaderBankPixel.Close()` devirtualises.
- A throw of an empty exception class copied from an uninitialised byte (`mov cl,[ebp-x]; mov [ebp-y],cl`) needs a user-declared empty ctor (or a named local thrown); `throw E()` on an aggregate zeroes it (`xor al,al`).
- A named CCharString local for a one-off status string (not a temporary) gives retail's `mov eax,[esi+0xEC]; ...; lea eax,[local]` reload at the inlined callback call.
- GetEntrySymbol inlines in the main body but stays a call in the catch funclet; reach the element through the CArray iterator (`*(Symbols.begin() + index)`) as the landed 0x009CC1D0 does; a ternary body stops it inlining.
- OpenAL lazy resolvers: `__forceinline` on the resolver wrapper gets all five inline sites (VC's per-function inline budget otherwise drops one); a shared `value` local for processed/state plus a `__forceinline IsPlaying()` helper with its own `state` local reproduce retail's push-ecx frame and dead-argument-slot homing (0x00C73050).
- `(unsigned char)(flags >> 7) & 1` on an unsigned long member gives `mov eax,[m]; shr eax,7; test al,1`; a 1-bit bitfield gives `mov al,[m]; test al,al; jns`.

Review at landing:
- 0x009CEC60: re-cut the oracle to 0x73A bytes and drop the separate Catch@009cf2f4 row before any later attempt; the parked source compiles to 1840 bytes against 1850 true bytes.
- 0x00CB0700 is a RELOCATION_MATCH under retail_asm/ (purity grade retail_asm); payload.json from finish.py does not carry retail_asm rows.
- 0x00B3B8E0 uses a local class model (header CShaderDataBank size wrong); the verifier will warn about the local struct -- regenerate engine/CEngineResourceManager.h first if strict mode is on.
- Respell the landed ??_G 0x00B3ECA0 callee and the 0x009CBD30 / 0x009CBD50 forwarders per NAME_FIXES.tsv.
