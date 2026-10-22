// CZoomingComponent::Initialise()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00546690, oracle length 19 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CChangingStateComponent::Initialise ; mov eax,[esi+30h] ; mov [esi+15Ch],eax ; pop esi ; ret
// Runs the base-class initialiser (0x0052C730, manifest: CChangingStateComponent::Initialise,
// __fastcall no args) then seeds the runtime field at +0x15C from the configured value at +0x30.
// PDB has CZoomingComponent at 452 bytes with both offsets unnamed padding; names below are inferred.

struct CZoomingComponent {
    unsigned char _pad_0x000[0x30];
    long          ConfiguredClickState;    // +0x030 (name inferred)
    unsigned char _pad_0x034[0x15C - 0x34];
    long          CurrentClickState;       // +0x15C (name inferred)
    void InitialiseBase();                 // CChangingStateComponent::Initialise @ 0x0052C730
    void Initialise();
};

void CZoomingComponent::Initialise()
{
    InitialiseBase();
    this->CurrentClickState = this->ConfiguredClickState;
}