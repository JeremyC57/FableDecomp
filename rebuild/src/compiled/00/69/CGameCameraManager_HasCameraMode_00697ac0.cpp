typedef int EMode;

class CCamera { public:
    virtual void slot0();
    virtual void slot1();
    virtual EMode GetCurrentMode();
};

class CGameCameraManager { public:
    char pad[0x98];
    CCamera* camera;
    bool HasCameraMode(EMode mode);
};

bool CGameCameraManager::HasCameraMode(EMode mode)
{
    return this->camera->GetCurrentMode() == mode;
}