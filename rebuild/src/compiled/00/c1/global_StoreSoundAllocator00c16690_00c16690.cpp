// StoreSoundAllocator_00c16690(LHA::CAllocator* allocator) -- install the sound allocator into the libpk library global 0x0139C960; a null allocator selects the library's default malloc/free allocator object 0x0143BB34
// Leaf: ?StoreSoundAllocator_00c16690@@YIXPAVCAllocator@LHA@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C16690, 21 bytes (`__fastcall`: `test ecx,ecx ; mov [g],ecx ; jne ; mov dword ptr [g],offset default ; ret`).
// Naming (naming wave 21, wave 20 DEFERRED): the spelling the landed 0x00C08CF0 (SetSound*Allocator, chunk 687 / naming wave 20) binds; descriptive (libpk, no PDB).
//   Was the propagated CHoverable_ProcessEvent over hover stand-ins. The default object is the libpk malloc/free allocator (vtable 0x012B2AB8:
//   ctor 0x00C166F0, Allocate 0x00C16700, Free 0x00C16720; dynamic initializer 0x01224B40), CDefaultAllocator_00c166f0 (descriptive) here.
namespace LHA { class CAllocator; }
class CDefaultAllocator_00c166f0;                               // vtable 0x012B2AB8, derives from LHA::CAllocator
extern LHA::CAllocator* g_pSoundAllocator_0139c960;                   // 0x0139C960
extern CDefaultAllocator_00c166f0 g_DefaultAllocator_0143bb34;   // 0x0143BB34

void __fastcall StoreSoundAllocator_00c16690(LHA::CAllocator* allocator);

void __fastcall StoreSoundAllocator_00c16690(LHA::CAllocator* allocator)
{
    g_pSoundAllocator_0139c960 = allocator;
    if (!allocator)
        g_pSoundAllocator_0139c960 = (LHA::CAllocator*)&g_DefaultAllocator_0143bb34;
}
