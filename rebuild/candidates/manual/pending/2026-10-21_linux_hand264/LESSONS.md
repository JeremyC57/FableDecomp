# Chunk 264 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A small inline helper that must inline (CResource::Touch-style) needs `__forceinline`; plain `inline` stayed an out-of-line call (009e2e30).
- A component lookup with a constant key takes the key as a template constant (`template<class T, long TYPE> T* GetComponent(CThing*)`). A runtime `type` argument compares against the key slot instead of an immediate (008a0cb0, 0089b5b0).
- The GetComponent result is an UNINITIALISED local (`T* result; if (bit) result = lookup; return result;`). The untaken branch reads the stale key/argument slot exactly as retail does (`mov ecx,[esp+10h]`), and `T* result = 0` gives a `xor` (0089b5b0, 008a8c60).
- `resize(n)` written as the STLport inline overload `{ resize(n, T()); }` evaluates n before the default temporary (00e81d10).
- A string literal passed implicitly to a `const CCharString&` builds its temporary before the other arguments; an explicit `CCharString("")` does not (0089e540, 00697490).
- `CIntelligentPointer<T>` must have NO declared destructor, so retail calls ~CBaseIntelligentPointer (0xA01C10) directly with no vptr store (0089e540, 0089b5b0).
- A derived ctor body that assigns base members places those stores after the derived vptr store (CVertexBufferWin32 sets PVertexBuffer/Stride in its body; base ctor `: Length(0), Type(0) {}`) (009e2e30).
- A by-value return object whose destructor stores a BASE vtable AFTER releasing a counted pointer is `Derived : Base` with the CCountedPointer as the derived member (own inline dtor). `Base` has an empty inline dtor, and the derived vptr store is dead-eliminated (0089b5b0 temp from 0x00903810).
- A counted-pointer count block's deleter is `void (__fastcall*)(void*)` (`mov ecx,[eax+8] ; call [eax+4]`). A cdecl pointer pushes ecx (0089b5b0).
- A copied counted pointer passed by value: `Data = o.Data; CCPPointerInfo* info = o.Info; Info = info; if (info) ++info->Count;` gives retail's `test ecx,ecx ; mov [eax+4],ecx ; je ; inc [ecx]`. The init-list form re-reads the source (008a8c60).
- A STRING temp that retail destroys before a later statement of the same block needs its own `{}` scope. That also lets an earlier key temporary share its slot (0069a990, ResetCamera's renderer message).
- The `double` widening trick for x87: `double along = v.Dot(axis)` plus an inline `InRange(float v, float lo, float hi)` reproduces retail's `fld st(0) ; ... fld st(1) ; fcompp` duplicate-and-compare (00697490).
- A constructor that stores no vtable of its own over a reader base is `__declspec(novtable)` (as the landed CBufferedReader 0x00C3AAC0). Its EH state 0 appears only if the base has a declared destructor (00c2da20, needs `/EHsc /GS`).
- Known lane gaps hit this chunk (per GOTCHAS): callee-saved push sunk inside a guard (00894500, 008a8260), cross-jump/tail duplication (00896b60, 00e1b750), dead-slot reuse (00699e60, 008d1130), temp-slot packing (008a8c60), join reload order (the five 008d19a0 twins, advisor-confirmed).

Review at landing:
- 009c4f20 / 009c5690 (CFileInstaller): retail re-reads PCompletedRead from memory and adds to NoTotalBytesWritten via load/add/store. That points to thread-shared qualified members, which this brief forbade. Re-try with the qualifier at landing (GOTCHAS already allows it for re-read counters).
- Rows that took more than 4 grades: 008a0cb0, 0089b5b0 (9), 008a8c60 (9), 00697490 (14), 00697f80 (7), 00c2da20 (6), 009e2e30, plus the earlier parked rows (008d19a0 twins, 00b66c10, 00e81d10).
- 00697490 is one instruction off. Two half-fixes are known; combine them (see NOTES).
- Local base models next to existing engine headers: CGameCameraBase.h / CGameCameraManager (the header flattens the C3DVector members into scalars, so 006fd6c0 uses a local projection with a Layout note); CGameScriptInterface (the header flattens the vptr).
- The catalog row 00c2da20 is mislabelled `CGuiWindowManager::LoadDrawEnvironment`; it is a segment-reader ctor (see NOTES Label corrections).
