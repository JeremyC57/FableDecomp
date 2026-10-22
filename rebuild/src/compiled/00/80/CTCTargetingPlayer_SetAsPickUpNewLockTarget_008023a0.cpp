// SetAsPickUpNewLockTarget @ 008023a0
struct CTCTargetingPlayer {
    char pad[0x3c];
    unsigned char f3c;   // +0x3c
    char pad2[3];        // align to +0x40
    unsigned int f40;    // +0x40
    unsigned int f44;    // +0x44
};

extern "C" unsigned int __fastcall helper_49d870(CTCTargetingPlayer* player);

void __fastcall SetAsPickUpNewLockTarget(CTCTargetingPlayer* player)
{
    player->f3c = 1;
    player->f40 = 0;
    player->f44 = helper_49d870(player);
}