// _$E2() -- `dynamic atexit destructor for 'CTCTavernGame::GetTextWithNarrator::no_text_entry'': forward through the global object
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x01229170, 10 bytes: `mov ecx, offset 0x013B8C58 ; jmp 0x0099B510`.
// Shape (family K004, representative 0x0048573D): a forwarder whose only work is a member call on one global object, which
//   VC7.1 emits as the `this` load and a tail jump.
// Identity (probable): its global 0x013B8C58 is bound ?no_text_entry@?1??GetTextWithNarrator@CTCTavernGame@@QAE?AVCWideString@@K@Z@4V3@A (the function-local static CWideString read by GetTextWithNarrator 0x005EB423) and the jump target is ~CWideString 0x0099B510; FableWin.pdb lists the GetTextWithNarrator `dynamic atexit destructor for 'no_text_entry'' (its debug scope number is 19, the binding spells ?1); VC7.1 emits the stub as the TU-local `_$E2` in .text$yd, where retail's sits (0x01229170, past the code).
class CWideStringData;
class CWideString {                                      // 4 bytes: PStringData
public:
    CWideString();                                       // 0x0099AED0
    CWideString(const CWideString& other);               // 0x0099B720
    ~CWideString();                                      // 0x0099B510
    CWideStringData* PStringData;
};
class CTCTavernGame {
public:
    CWideString GetTextWithNarrator(unsigned long id);
};
// Only the local static's atexit destructor is graded; the body here just owns the static (the retail one does more).
CWideString CTCTavernGame::GetTextWithNarrator(unsigned long)
{
    static CWideString no_text_entry;
    return no_text_entry;
}