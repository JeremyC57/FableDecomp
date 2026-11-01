// StoreSoundPhysicalAllocator_00c166b0(LHA::CAllocator* allocator) -- install the physical-sound allocator into the libpk library global 0x0139C964; a null allocator selects the library's default malloc/free allocator object 0x0143BB34
// Leaf: ?StoreSoundPhysicalAllocator_00c166b0@@YIXPAVCAllocator@LHA@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C166B0, 21 bytes (`__fastcall`: `test ecx,ecx ; mov [g],ecx ; jne ; mov dword ptr [g],offset default ; ret`).
// Naming (naming wave 21, wave 20 DEFERRED): the spelling the landed 0x00C08D10 (SetSound*Allocator, chunk 687 / naming wave 20) binds; descriptive (libpk, no PDB).
//   Was the propagated CHoverable_ProcessEvent over hover stand-ins. The default object is the libpk malloc/free allocator (vtable 0x012B2AB8:
//   ctor 0x00C166F0, Allocate 0x00C16700, Free 0x00C16720; dynamic initializer 0x01224B40), CDefaultAllocator_00c166f0 (descriptive) here.
namespace LHA { class CAllocator; }
class CDefaultAllocator_00c166f0;                               // vtable 0x012B2AB8, derives from LHA::CAllocator
extern LHA::CAllocator* g_pSoundPhysicalAllocator_0139c964;                   // 0x0139C964
extern CDefaultAllocator_00c166f0 g_DefaultAllocator_0143bb34;   // 0x0143BB34

void __fastcall StoreSoundPhysicalAllocator_00c166b0(LHA::CAllocator* allocator);

void __fastcall StoreSoundPhysicalAllocator_00c166b0(LHA::CAllocator* allocator)
{
    g_pSoundPhysicalAllocator_0139c964 = allocator;
    if (!allocator)
        g_pSoundPhysicalAllocator_0139c964 = (LHA::CAllocator*)&g_DefaultAllocator_0143bb34;
}
