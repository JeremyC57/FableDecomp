# Lessons from chunk 168
- An implicit-copy `return box;` of a 16-byte POD copies through `mov ecx,eax`; retail's eax-based dword copy-out needs a `__forceinline` memberwise copy ctor (C2DBoxI, 0x00759870).
- `(unsigned char)~flags & 1` keeps the 8-bit `not al ; test al,1`; without the cast VC widens to `xor eax,eax ; not eax` (0x00723AC0).
- A call retail makes before an argument's load (GetWorld before `movzx` of the index) wants the object in a named local first (`CMeshBank* bank = GetWorld()->PMeshBank;`, 0x006B0850).
- Euler-angle vectors filled one member at a time follow retail's store order exactly, even when the argument store sits between two virtual calls (0x00723760).
- Inlined bit-field setters with a constant false (ResetPerceptionVariables) reproduce retail's surviving "was set" arms; the setter must be defined in the class, the out-of-line sibling only declared.
- A CTC member forming `add esi,N` / `lea esi,[edi+N]` for an intelligent-pointer member right after a temporary's GetPItem is a scheduling gap (0x007446B0, 0x00729BC0); `lea ecx,[esi+N]` without reuse matches (0x00800460).
- Several "CancelForCutscene" labels on CTCPhysicalShieldSpell are byte-shape propagations onto other spell/mode classes: check the member offsets against the neighbouring functions' classes.
