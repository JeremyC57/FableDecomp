// CTCShopKeeper::GetHint() -- returns the shopkeeper's hint text tag "TEXT_AI_SHOP_KEEPER_GIVE_HINT" as a CCharString
// Leaf: ?GetHint@CTCShopKeeper@@QAE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00798D30, 25 bytes: `push esi ; mov esi,[esp+8] ; push -1 ; push 0x0126C12C ; mov ecx,esi ;
//   call 0x0099EBF0 (CCharString(const char*, long)) ; mov eax,esi ; pop esi ; ret 4` (the hidden result pointer; this unused).
// Name fix (NF195, 2026-11-20, q6327 / q6335): was the descriptive _unattributed::GetShopKeeperGiveHintTextTag_00798d30 over a local
//   CCharString model with the literal passed as the cast address 0x0126C12C. Identity: Ego_r.pdb ?GetHint@CTCShopKeeper@@QAE?AVCCharString@@XZ
//   (Ego_r orders GetHint 0x0079E7C0 before GetShopTC 0x0079E900 as retail 0x00798D30 < 0x00798F50, with GetMinDistFromStall /
//   GetMaxDistFromCustomer just before both), the body (one literal CCharString) and the only landed caller 0x0092FAB0, which already
//   binds this spelling (engine/CTCShopKeeper.h FABLE_RETAIL_CTCSHOPKEEPER_SHOP). Now on the shared headers; the literal is the retail text.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCShopKeeper.h"

CCharString CTCShopKeeper::GetHint()
{
    return CCharString("TEXT_AI_SHOP_KEEPER_GIVE_HINT", -1);
}
