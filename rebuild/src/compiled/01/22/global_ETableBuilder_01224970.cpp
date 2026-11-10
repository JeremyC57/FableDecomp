// $E_TableBuilder() -- dynamic initializer of the file-scope TableBuilder: run CEngineInlineTableBuilder's constructor on it, which fills the engine's 256-entry highest-set-bit table (no atexit: trivial destructor)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x01224970, 10 bytes, in the `.text$yc` initializer run, reached from the .CRT$XCU table
//   (data reference 0x01374900): `mov ecx,0x0143BA90 ; jmp 0x00BE2A80` (the constructor's `return this` is the tail call).
// Identity (two sources): Ego_r.pdb (0x01019820) and FableWin.pdb (0x04006820) both list `dynamic initializer for
//   'TableBuilder'' (no class or namespace qualifier); the callee is ??0CEngineInlineTableBuilder@@QAE@XZ 0x00BE2A80 (both PDB
//   name sets; landed), a 1-byte object (Ego_r.pdb CEngineInlineTableBuilder, size 1) whose constructor never touches `this`
//   and fills g_highestSetBitTable 0x0143B990. 0x0143BA90 has no other reference.
// Label: the manifest's "sub_01224970" is a gap-scan placeholder.
// Class key: `struct` -- Ego_r.pdb records CEngineInlineTableBuilder as a struct (ghidra_out/struct_layouts_egor.tsv @STRUCT).
struct CEngineInlineTableBuilder {                   // Ego_r.pdb: 1 byte, no members
public:
    CEngineInlineTableBuilder();                    // 0x00BE2A80
};

CEngineInlineTableBuilder TableBuilder;             // 0x0143BA90