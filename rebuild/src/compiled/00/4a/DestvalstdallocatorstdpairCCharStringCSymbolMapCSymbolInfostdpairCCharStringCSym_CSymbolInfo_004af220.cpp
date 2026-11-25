// _Dest_val<allocator<pair<...>>, pair<...>> stand-in self (descriptive, see T3): the inlined member destruction of a
// CQuestManager::CBoast (Ego_r layout: ScriptName CCharString +0x0c, Text CWideString +0x10, Title CWideString +0x14)
// Leaf: ?_Dest_val_CSymbolInfo___@SymPair@@QAEXXZ
// Retail 0x004af220 (28 bytes):
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x14]; call 0x99b510   (CWideString::~CWideString, Title)
//   lea ecx,[esi+0x10]; call 0x99b510   (CWideString::~CWideString, Text)
//   lea ecx,[esi+0x0c]; pop esi; jmp 0x99eae0  (CCharString::~CCharString, ScriptName: tail)
//
// NF404 (T3 nf401 0x004AF220): the two 0x0099B510 calls are ??1CWideString@@QAE@XZ, not ~CCharString (reloc_bindings binds
// ??1CWideString@@QAE@XZ there from landed 0x00406E30 / 0x009979E0; its body decrements the data's NoRefs at +0x0C, as the
// Ego_r.pdb ~CWideString 0x00B02740 does at its +0x1C wide layout, and ends `dec [0x013BCA20]`). The tail 0x0099EAE0 is
// ??1CCharString@@QAE@XZ (landed self, RELOCATION_MATCH; ReleaseData 0x0099E9B0 reads NoRefs at +0x0D like Ego_r ~CCharString
// 0x00AFA3F0). The members come from the shared fable_string.h declarations (out-of-line destructors).
#include "fable_string.h"

// Members destroyed in REVERSE declaration order: +0x14, +0x10, +0x0c.
struct SymPair {
    char        head[0x0c];   // +0x00..+0x0b  ID, Cost, Reward (trivial)
    CCharString ScriptName;   // +0x0c  (declared first -> destroyed LAST / tail jmp 0x99eae0)
    CWideString Text;         // +0x10  (destroyed 2nd, call 0x99b510)
    CWideString Title;        // +0x14  (destroyed 1st, call 0x99b510)
    void _Dest_val_CSymbolInfo___();
};

// The body: reverse-order member destruction, inlined -> exactly retail.
void SymPair::_Dest_val_CSymbolInfo___() {
    this->Title.~CWideString();       // +0x14  call 0x99b510
    this->Text.~CWideString();        // +0x10  call 0x99b510
    this->ScriptName.~CCharString();  // +0x0c  tail jmp 0x99eae0
}
