// CEAXManager::SetEAXProperty(const GUID* propertySet, unsigned long id, CEAXVoice* voice, void* data, unsigned long size) -- the EAX 3.0 property-set entry point: classify the property-set GUID, store a listener property into the manager's cached EAX30LISTENERPROPERTIES (+0x74, environment presets from a 26-entry local reverb table) or a buffer property into the voice's EAX30BUFFERPROPERTIES (+0x1A8), and push changed state down (ApplyListenerProperties 0x00C4E3C0 / ApplyVoiceBufferProperties 0x00C4EA30)
// Residue: NOT AUTHORED (0 grades) -- analysis only; budget went to the five rows that share the ivec/CSoftwareVoice/EAX-source work. No probe quantity yet.
// Next: author from the structure below: (1) the 26-entry preset table as a local aggregate (27 dwords each, stride 0x6C, esp+0x14..; VC caches -1000/5000/250/-5/0 in eax/esi/edx/ecx/edi and -6000/-602/-4000 in ebp); (2) the GUID ladder; (3) the two switches.
//
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4F600, true extent 9204 bytes = 0x00C4F600..0x00C519F4: code to `ret 14h` at 0x00C5192A,
//   `lea ecx,[ecx]` pad, jump table A (23 entries, buffer properties) at 0x00C51930, jump table B (26 entries, listener
//   properties) at 0x00C5198C, int3 pad. The catalogued 9248 is a next-start gap: EXTRA START 0x00C51A00 (25 bytes, thiscall
//   `ret 14h`: pushes (id, data, size) to GetListenerProperty 0x00C4E730 and returns 0 -- the matching Get entry point).
// Frame: `sub esp,0AFCh`, this kept in ebx and spilled to [esp+10h]; args at [esp+0B10h] propertySet, [esp+0B14h] id,
//   [esp+0B18h] voice, [esp+0B1Ch] data, [esp+0B20h] size.
// GUID ladder (inline IsEqualGUID, `mov ecx,4 ; repe cmpsd` against .rdata GUIDs): 0x012B9468 -> 0x20000, 0x012B9478 ->
//   0x40000, 0x012B9488 -> 0x80000, 0x012B9498 -> 0x100000, 0x012B9448 -> 0x200000, 0x012B94A8 -> 4, 0x012B9418 -> 0x10000,
//   0x012B9428 -> 2 (`setne al ; dec eax ; and eax,2`), else 0.
// kind & 6 and kind == 2 (DSPROPSETID_EAX30_BufferProperties): id & 0x7fffffff <= 0x16 -> table A on voice (+0x1A8 block:
//   ALLPARAMETERS = 0x12-dword memcmp/rep movsd, single fields compared through `fucompp`/`test ah,44h` for floats), then
//   if changed and (long)id >= 0: ApplyVoiceBufferProperties(voice, 2, id, data, size) via 0x00C4EA30.
// kind & 0x3F0000 and kind == 0x10000 (DSPROPSETID_EAX30_ListenerProperties): id & 0x7fffffff <= 0x19 -> table B on this
//   (+0x74 block, ALLPARAMETERS = 0x1C dwords; ENVIRONMENT copies a preset), then if changed and (long)id >= 0:
//   ApplyListenerProperties(&this->+0x74) via 0x00C4E3C0. Always returns 0 (`xor eax,eax` before `ret 14h`).
// Names: SetEAXProperty is the spelling landed in QueueCommit 0x00C51A20 (its declaration names this address) and in the
//   ApplyVoiceBufferProperties 0x00C4EA30 header ("its only caller is the EAX property setter 0x00C4F600 (SetEAXProperty)");
//   the catalog's "PropertySerializer_GetPropertyTypeCode" is a propagated guess.
