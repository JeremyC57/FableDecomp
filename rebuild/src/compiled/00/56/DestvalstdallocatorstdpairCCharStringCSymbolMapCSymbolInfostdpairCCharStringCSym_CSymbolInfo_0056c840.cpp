// Faithful VC7.1 reconstruction of _Dest_val<...> at 0x0056c840 (stand-in self, descriptive, see T3)
// Leaf: ?Dest_val_CSymbolInfo___@@YIXPAUSymbolPair@@H@Z
//
// Retail disasm:
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x18]; call 0x99b510   (CWideString::~CWideString, member@0x18)
//   lea ecx,[esi+0x14]; call 0x99b510   (CWideString::~CWideString, member@0x14)
//   lea ecx,[esi+0x10]; pop esi
//   jmp 0x99eae0                         (CCharString::~CCharString, member@0x10: tail)
//
// reloc_bindings binds this address as ??1?$pair@QAVCMenuEntry@NUISystem@@VCBoast@CQuestManager@@@std@@QAE@XZ (caller
// 0x0056AD40): pair<CMenuEntry* const, CQuestManager::CBoast>, CBoast at +4 with the Ego_r layout ScriptName CCharString +0x0C,
// Text CWideString +0x10, Title CWideString +0x14, i.e. +0x10 / +0x14 / +0x18 of the pair; only those three sub-objects have
// non-trivial destructors, the last one is tail-called.
// NF404 (T3 nf401 0x004AF220): the two 0x0099B510 calls are ??1CWideString@@QAE@XZ (reloc_bindings, landed 0x00406E30 /
// 0x009979E0; NoRefs at +0x0C as in Ego_r.pdb ~CWideString 0x00B02740), not ~CCharString; the tail 0x0099EAE0 is
// ??1CCharString@@QAE@XZ (landed self), not a separate "CInfoHead" destructor. Both from the shared fable_string.h.
#include "fable_string.h"

struct SymbolPair {
    int         m_pad[4];       // +0x00 .. +0x0F  (first pointer, ID, Cost, Reward: trivial scalars)
    CCharString ScriptName;     // +0x10  (dtor 0x99eae0, tail-called last)
    CWideString Text;           // +0x14  (dtor 0x99b510)
    CWideString Title;          // +0x18  (dtor 0x99b510)
};

// _Dest_val: __fastcall(allocator*, pair*).  The allocator arrives in ecx
// but the emitted body destroys the object *at ecx* (the pair pointer is the
// operand actually used).  Model it as a free __fastcall whose first arg is
// the object being destroyed; the second (edx) allocator arg is unused.
void __fastcall Dest_val_CSymbolInfo___(SymbolPair* self, int /*edx*/)
{
    self->Title.~CWideString();        // +0x18
    self->Text.~CWideString();         // +0x14
    self->ScriptName.~CCharString();   // +0x10  (tail)
}
