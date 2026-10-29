// SetAsPickUpNewLockTarget @ 008023a0
class CTCTargetingPlayer { public:
    char pad[0x3c];
    unsigned char f3c;   // +0x3c
    char pad2[3];        // align to +0x40
    unsigned int f40;    // +0x40
    unsigned int f44;    // +0x44
};

class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ

void __fastcall SetAsPickUpNewLockTarget(CTCTargetingPlayer* player)
{
    player->f3c = 1;
    player->f40 = 0;
    player->f44 = ((unsigned int)CWorld::GetFrame());
}