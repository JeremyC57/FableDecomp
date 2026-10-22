struct CGameCameraManager {
    char pad[0xb1];
    unsigned char changeInControlDirection;
};

void __fastcall CGameCameraManager_SetChangeInControlDirection(CGameCameraManager* manager)
{
    manager->changeInControlDirection = 1;
}