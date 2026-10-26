// CTCShop::OnInitialActivate()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00775B20, oracle length 19 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CTCShop::CalcIsStallLike ; mov [esi+50h],al ; mov ecx,esi ; pop esi ; jmp <OnInitialActivate body>
// Caches CalcIsStallLike() (0x007729A0, returns bool in al) into the byte at +0x50, then
// tail-jumps to the remaining activation body at 0x00775350 (manifest: CTCShop_OnInitialActivate).
// Ego_r.pdb places `long DaysSinceSpecialItemPlaced` at +0x50; retail writes a single byte
// there, so the shipped layout differs from the editor PDB at this offset (field name inferred).

class CTCShop { public:
    unsigned char _pad_0x00[0x50];
    bool          IsStallLike;          // +0x50 (retail byte; PDB disagrees, see above)
    bool CalcIsStallLike();             // 0x007729A0
    void OnInitialActivateBody();       // 0x00775350
    void OnInitialActivate();
};

void CTCShop::OnInitialActivate()
{
    this->IsStallLike = CalcIsStallLike();
    OnInitialActivateBody();
}