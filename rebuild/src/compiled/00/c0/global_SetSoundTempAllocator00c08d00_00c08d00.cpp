// SetSoundTempAllocator_00c08d00(LHA::CAllocator* allocator) -- install the game's allocator into the libpk sound library (the global 0x0139C968) through the __fastcall store 0x00C166D0
// Leaf: ?SetSoundTempAllocator_00c08d00@@YGXPAVCAllocator@LHA@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C08D00, 10 bytes (`__stdcall`, `ret 4`: `mov ecx,[esp+4] ; call store ; ret 4`).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- the spelling the landed CSoundManagerLH ctor 0x00A3EF00 binds (it passes a new
//   CBBBSoundAllocator / CBBBSoundPhysicalAllocator, RTTI base LHA::CAllocator). Was a _Cons_val_pair_morph_* STLport stand-in. The store callee's
//   own row carries a propagated name (CHoverable_ProcessEvent): named descriptively here, DEFERRED for its own respell.
namespace LHA { class CAllocator; }
void __fastcall StoreSoundTempAllocator_00c166d0(LHA::CAllocator* allocator);   // 0x00C166D0 (stores the global 0x0139C968)
void __stdcall SetSoundTempAllocator_00c08d00(LHA::CAllocator* allocator)
{
    StoreSoundTempAllocator_00c166d0(allocator);
}
