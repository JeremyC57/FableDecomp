// StoreSoundTempAllocator_00c166d0(LHA::CAllocator* allocator) -- install the temporary-sound allocator into the libpk library global 0x0139C968; a null allocator selects the library's default malloc/free allocator object 0x0143BB34
// Leaf: ?StoreSoundTempAllocator_00c166d0@@YIXPAVCAllocator@LHA@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C166D0, 21 bytes (`__fastcall`: `test ecx,ecx ; mov [g],ecx ; jne ; mov dword ptr [g],offset default ; ret`).
// Naming (naming wave 21, wave 20 DEFERRED): the spelling the landed 0x00C08D00 (SetSound*Allocator, chunk 687 / naming wave 20) binds; descriptive (libpk, no PDB).
//   Was the propagated CHoverable_ProcessEvent over hover stand-ins. The default object is the libpk malloc/free allocator (vtable 0x012B2AB8:
//   ctor 0x00C166F0, Allocate 0x00C16700, Free 0x00C16720; dynamic initializer 0x01224B40), CDefaultAllocator_00c166f0 (descriptive) here.
namespace LHA { class CAllocator; }
class CDefaultAllocator_00c166f0;                               // vtable 0x012B2AB8, derives from LHA::CAllocator
extern LHA::CAllocator* g_pSoundTempAllocator_0139c968;                   // 0x0139C968
extern CDefaultAllocator_00c166f0 g_DefaultAllocator_0143bb34;   // 0x0143BB34

void __fastcall StoreSoundTempAllocator_00c166d0(LHA::CAllocator* allocator);

void __fastcall StoreSoundTempAllocator_00c166d0(LHA::CAllocator* allocator)
{
    g_pSoundTempAllocator_0139c968 = allocator;
    if (!allocator)
        g_pSoundTempAllocator_0139c968 = (LHA::CAllocator*)&g_DefaultAllocator_0143bb34;
}
