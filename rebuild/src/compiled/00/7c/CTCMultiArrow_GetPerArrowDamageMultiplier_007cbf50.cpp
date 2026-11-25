// CTCMultiArrow::GetPerArrowDamageMultiplier() const -- the multi-arrow def's per-arrow damage multiplier for the current spell level
// Leaf: ?GetPerArrowDamageMultiplier@CTCMultiArrow@@QBEMXZ
// Retail: Fable.exe @ 0x007CBF50, 13 bytes: mov eax,[ecx+0Ch] ; mov ecx,[ecx+1Ch] ; mov edx,[eax+74h] ; fld dword [edx+ecx*4] ; ret
// Naming (NF443, 2026-11-25; coordinator round 3): self respelt from the stand-in ?GetDamageMultiplier@@YIMPAUS@@@Z. Evidence: both PDB
//   name sets (?GetPerArrowDamageMultiplier@CTCMultiArrow@@QBEMXZ, Ego_r 0x007CA5A0 / FableWin 0x0272F250); Ego_r donor order SetAICast ->
//   CancelMove -> GetPerArrowDamageMultiplier = retail SetAICast 0x007CBF20 -> 0x007CBF50 -> ~CTCMultiArrow 0x007CBF60 (CTCMultiArrow TU);
//   the body reads PDef (+0x0C) and SpellLevel (+0x1C), the CTCMultiArrow layout engine/CTCMultiArrow.h asserts, and the def's
//   MultiArrowPerArrowDamageMultiplier vector<float>; its caller 0x007BDCFC reaches it through interface 0x88 = TCI_MULTI_ARROW.
// Layout: local models of the retail offsets (engine/CTCMultiArrow.h lays SpellLevel out only under an opt-in; header debt as at HEAD,
//   which modelled the same fields with a local struct).
class CSpecialAbilitiesMultiArrowDef {
public:
    unsigned char _pad00[0x74];
    float*        MultiArrowPerArrowDamageMultiplier;   // +0x74 vector<float> begin
};
class CTCMultiArrow {
public:
    unsigned char                         _pad00[0x0C];
    const CSpecialAbilitiesMultiArrowDef* PDef;         // +0x0C
    unsigned char                         _pad10[0x1C - 0x10];
    long                                  SpellLevel;   // +0x1C
    float GetPerArrowDamageMultiplier() const;
};
float CTCMultiArrow::GetPerArrowDamageMultiplier() const
{
    return PDef->MultiArrowPerArrowDamageMultiplier[SpellLevel];
}
