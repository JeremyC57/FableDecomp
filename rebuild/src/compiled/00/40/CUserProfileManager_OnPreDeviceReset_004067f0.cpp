// Leaf: ?OnPreDeviceReset@CUserProfileManager@@MAEXXZ
class CUserProfileManager { public:
    char pad[0x130];
    int mField;
    protected: virtual void OnPreDeviceReset();
};

void CUserProfileManager::OnPreDeviceReset()
{
    CUserProfileManager* manager = this;
    manager->mField = 0;
}