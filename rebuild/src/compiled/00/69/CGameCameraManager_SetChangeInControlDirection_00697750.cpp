// Leaf: ?SetChangeInControlDirection@CGameCameraManager@@UAEXXZ
class CGameCameraManager { public:
    char pad[0xad];
    unsigned char changeInControlDirection;
    public: virtual void SetChangeInControlDirection();
};

void CGameCameraManager::SetChangeInControlDirection()
{
    CGameCameraManager* manager = this;
    manager->changeInControlDirection = 1;
}