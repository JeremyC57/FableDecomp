// CEngine::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCEngine@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2AB10, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c673): slot 0 of vtable 0x012A0F3C (rtti-vtables.tsv CEngine); scalar form; FableWin.pdb ??_GCEngine@@UAEPAXI@Z.
// Callees: CEngine::~CEngine 0x00B261D0 (??1CEngine@@UAE@XZ), operator delete 0x00BFE9BC.
// VC7.1 emits ??_GCEngine as a pick-any COMDAT beside the class's vtables, only in a TU whose code stores the vptr (the CEngine
// constructor or destructor). Both have their own rows, so this function is produced by the constructor's translation unit
// (0x00B260B0, CEngine::CEngine), included below (no code of its own), as the EH funclet rows include their parent. Its ??_GCEngine
// copy grades RELOCATION_MATCH against this address.
// NF251 (q12700, nf249 ruling 6 option (a)): was a TU emitter with a local one-slot CEngine model and a strong scaffolding
//   `CEngine::CEngine() { }`, which defined ??0CEngine@@QAE@XZ a second time once 0x00B260B0 took its real self leaf.
#include "../src/compiled/00/b2/CEngine_CEngine_00b260b0.cpp"
