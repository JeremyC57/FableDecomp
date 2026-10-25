// CEntryHost_00c37990::CreateEntry(void* key) -- EH-framed: allocate the 0x68-byte registered entry (scalar operator new, null-checked) and construct it with (this, key); its constructor 0x00C47340 registers it with this host, so the pointer is discarded
// Leaf: ?CreateEntry@CEntryHost_00c37990@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C37990, 82 bytes (+ int3 padding), __fastcall `ret 4`. Built under /EHsc (EH
//   state 0 frees the block if the constructor throws; the funclet is graded separately).
// Library: libpk audio (no PDB/RTTI); descriptive names. The entry is the landed 0x00C47340 / 0x00C473F0 class
//   (CPredicateOwner_00c47340: host +0, key +4, two tree containers at +8 / +0x38, added to the host's registry at
//   +0x28 by the constructor, removed by the destructor); DestroyEntry 0x00C37B90 is the inverse. Reached through the
//   forwarders 0x00C379F0 (`mov ecx,[ecx+4] ; jmp`) and 0x00C2BAB0. The manifest's "Alloc" (CScriptDataBase*) is
//   propagated.
// Callees: scalar operator new 0x00BFEA1A, the entry constructor 0x00C47340 (host, key).
// Shape: a discarded `new T(this, key);` in a void function -- `return new T(...)` adds a separate xor-eax
//   epilogue for the null arm (102 v 82); retail's null `je` falls straight into the shared epilogue.
void* __cdecl operator new(unsigned int size);                 // 0x00BFEA1A
void __cdecl operator delete(void* block) throw();             // 0x00BFE9BC

struct CEntryHost_00c37990;

struct CRegisteredEntry_00c37990 {              // 0x68 bytes
    unsigned char _body[0x68];
    CRegisteredEntry_00c37990(CEntryHost_00c37990* host, void* key);   // 0x00C47340
};

struct CEntryHost_00c37990 {
    void CreateEntry(void* key);
};

void CEntryHost_00c37990::CreateEntry(void* key)
{
    new CRegisteredEntry_00c37990(this, key);
}