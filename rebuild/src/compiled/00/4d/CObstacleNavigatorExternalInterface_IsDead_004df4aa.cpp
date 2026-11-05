// Leaf: ?CObstacleNavigatorExternalInterface_IsDead@@YI_NPAVCObstacleNavigatorExternalInterface@@@Z
#pragma optimize("s",on)
struct Inner {
    char pad[0x91];
    unsigned char flag; // +0x91
};

class CObstacleNavigatorExternalInterface { public:
    void* vtbl;      // +0
    Inner* inner;    // +4
};

bool __fastcall CObstacleNavigatorExternalInterface_IsDead(CObstacleNavigatorExternalInterface* self)
{
    Inner* p = self->inner;
    bool alive = (p != 0) && ((p->flag & 1) == 0);
    return !alive;
}