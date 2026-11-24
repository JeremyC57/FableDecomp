// Leaf: ?IsUsingScriptedCameraMode@CGameCameraManager@@QBE_NXZ
struct Sub {
    struct Vtbl {
        void* s0;
        void* s1;
        bool (__fastcall* s2)(Sub*);
    };
    Vtbl* vptr;
};

class CGameCameraManager { public:
    char pad[0xd4];
    Sub* obj_d4;
    char pad2[4];
    int  flag_dc;
    public: bool IsUsingScriptedCameraMode() const;
};

bool CGameCameraManager::IsUsingScriptedCameraMode() const
{
    const CGameCameraManager* manager = this;
    if (manager->flag_dc == 0) {
        Sub* s = manager->obj_d4;
        if (s == 0)
            return false;
        if (s->vptr->s2(s))
            return false;
    }
    return true;
}