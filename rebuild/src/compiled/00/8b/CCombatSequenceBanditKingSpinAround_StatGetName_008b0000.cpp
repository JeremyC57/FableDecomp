// CCombatSequence_BanditKingSpinAround::StatGetName() -- the sequence's registry name "COMBAT_SEQUENCE_BANDIT_KING_SPIN_AROUND"
// Leaf: ?StatGetName@CCombatSequence_BanditKingSpinAround@@SI?AVCCharString@@XZ
// Retail: Fable.exe @ 0x008B0000, 19 bytes, static __fastcall with the hidden CCharString result in ecx, plain `ret`.
// Identity (proven): the body builds a CCharString from the literal 0x012788D8 "COMBAT_SEQUENCE_BANDIT_KING_SPIN_AROUND"
// (the name CCombatManager::RegisterCombatSequences 0x008B0D10 registers for CCombatSequence_BanditKingSpinAround) into the
// hidden result and returns it -- the static `SI ?AVCCharString@@XZ` getter shape (no `this`, no stack arguments). Ego_r.pdb
// has ?StatGetName@CCombatSequence_BanditKingSpinAround@@SI?AVCCharString@@XZ (the virtual GetName is a separate UBE member).
// No caller is visible in Fable.exe (the 359 xref.py hits are `0x008B0000` immediates, not references).
// Label correction: catalog "_gapscan::sub_008b0000".
// Callees: 0x0099EBF0 ??0CCharString@@QAE@PBDJ@Z (Ego_r.pdb spelling; reloc_bindings holds only stand-ins for it).
// Shape: `push -1 ; push offset literal ; mov esi,ecx ; call ctor ; mov eax,esi`.
class CCharString {
public:
    CCharString(const char* text, long length);             // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
private:
    void* PStringData;
};

class CCombatSequence_BanditKingSpinAround {
public:
    static CCharString __fastcall StatGetName();
};

CCharString __fastcall CCombatSequence_BanditKingSpinAround::StatGetName()
{
    return CCharString("COMBAT_SEQUENCE_BANDIT_KING_SPIN_AROUND", -1);
}