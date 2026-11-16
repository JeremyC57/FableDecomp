// CSampleRef::CSampleRef() -- a null sample reference: Sample and the three keys zeroed
// Leaf: ??0CSampleRef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C25C50, 16 bytes: `mov eax,ecx ; xor ecx,ecx`, four dword stores, `ret`.
// Identity (NF42, round 5): libpk/CSampleRef.h (descriptive libpk names, no PDB/SDK symbol exists) declares this constructor at
// 0x00C25C50; the landed CSoundEventManager::CheckExisting 0x00C11230 binds ??0CSampleRef@@QAE@XZ here. Was the placeholder `T::Init`
// (pack(1) local model); now on the shared header.
#include "libpk/CSampleRef.h"

CSampleRef::CSampleRef() : Sample(0), Key1(0), Key2(0), Key3(0)
{
}
