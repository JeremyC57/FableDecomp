// CBankCreatorException::~CBankCreatorException() -- destroys ErrorText (+4) then EntrySymbol (+0)
// Leaf: ??1CBankCreatorException@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D47C0. No retail call reaches it: its one reference is the pmfnUnwind word (0x013693B8) of
//   the ThrowInfo 0x013693B4 whose catchable type is .?AVCBankCreatorException@@ (NF33 scan of the image: the only dword 0x009D47C0).
// Name: Ego_r.pdb 0x00B19590 + FableWin.pdb 0x02F730A0 ??1CBankCreatorException@@QAE@XZ; Ego_r CBankCreatorException (8 bytes):
//   EntrySymbol +0, ErrorText +4 (both CCharString). No shared header; modelled here. NF33 (2026-11-16): was the made-up Pair::Destroy
//   (manifest label std::allocator<pair<long,CRegisteredMusicEntry> >::_Dest_val, propagated).
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
class CBankCreatorException {
public:
    CCharString EntrySymbol;    // +0x00
    CCharString ErrorText;      // +0x04
    ~CBankCreatorException();
};
CBankCreatorException::~CBankCreatorException()
{
}
