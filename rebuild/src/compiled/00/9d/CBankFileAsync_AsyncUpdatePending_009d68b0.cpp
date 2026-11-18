// CBankFileAsync::AsyncUpdatePending() -- service the asynchronous bank: process the disposal list, then restart any aborted reads (tail call)
// Leaf: ?AsyncUpdatePending@CBankFileAsync@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D68B0, 16 bytes, thiscall: push esi ; mov esi,ecx ; call 0x009D6730 ; mov ecx,esi ; pop esi ; jmp 0x009D5A90.
// Identity (proven): Ego_r.pdb 0x00B2ABD0 + FableWin.pdb 0x02FF6B50 ?AsyncUpdatePending@CBankFileAsync@@QAEXXZ (engine/CBankFileAsync.h); bound by
//   GetPGroupEntry 0x009C8890 (agentNF69, RM PASS) and the landed GetTextEntryFromGroup 0x009C9280 / GetPTextEntry 0x009C9070 under this spelling.
// Callees: CBankFileAsync::ProcessDisposalList 0x009D6730 (Ego_r 0x00B2A6E0 + FableWin 0x02FF6EC0, public; landed self spelling) and the private
//   CBankFileAsync::RestartAbortedReads 0x009D5A90 (Ego_r 0x00B29C60 + FableWin 0x02FF6F90; the landed 0x009D5B60 binds it so), as a tail call.
// NF72 (round 7, q10905): was the CTwoStep_Retail stand-in (catalog _global::Dtor_RestartReadsBase_009d68b0, a propagated label); re-authored on
//   the shared engine/CBankFileAsync.h (FABLE_RETAIL_CBANKFILEASYNC_DATA retail layout, the include set of the landed 0x009D5DF0; ASYNCUPDATEPENDING for the self declaration, and the
//   overlay opt-in UPDATEPENDING_BODY declaring the two callees).
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CBANKFILEASYNC_ASYNCUPDATEPENDING
#define FABLE_RETAIL_CBANKFILEASYNC_UPDATEPENDING_BODY
#include "engine/CBankFileAsyncData.h"
#include "engine/CBankFileAsync.h"

void CBankFileAsync::AsyncUpdatePending()
{
    ProcessDisposalList();
    RestartAbortedReads();
}
