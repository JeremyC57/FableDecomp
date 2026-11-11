// CEAXManager::GetEAXProperty(const GUID* propertySet, unsigned long id, CEAXVoice* voice, void* data, unsigned long size) -- read an EAX listener property: the property set and the voice are ignored, the listener getter fills the caller's buffer, and the call always reports success
// Leaf: ?GetEAXProperty@CEAXManager@@QAEJPBU_GUID@@KPAUCEAXVoice@@PAXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C51A00, 25 bytes, thiscall `ret 14h`: size, data and id re-pushed, `call 0x00C4E730`
//   with `this` untouched in ecx, `xor eax,eax`.
// Identity (descriptive; libpk/CODA EAX layer, no PDB or RTTI): the getter twin of the manager's property setter 0x00C4F600
//   (landed QueueCommit 0x00C51A20 calls it as SetEAXProperty(propertySet, id, voice, data, size): the same five parameters).
//   The callee 0x00C4E730 is a CEAXManager member (it tests the listener state +4 and copies from the EAX30LISTENERPROPERTIES
//   block +0x74 under a 26-way switch on the id; `ret 0Ch`): GetListenerProperty here. No direct caller in the image.
//   Both names are descriptive (NOTES).
// Extent: the catalog row is exact (25 bytes, then int3 padding to 0x00C51A20).
struct _GUID;
struct CEAXVoice;
struct CEAXManager {
    long GetListenerProperty(unsigned long id, void* data, unsigned long size);   // 0x00C4E730
    long GetEAXProperty(const _GUID* propertySet, unsigned long id, CEAXVoice* voice, void* data, unsigned long size);
};

long CEAXManager::GetEAXProperty(const _GUID* propertySet, unsigned long id, CEAXVoice* voice, void* data, unsigned long size)
{
    GetListenerProperty(id, data, size);
    return 0;
}