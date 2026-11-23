// Leaf: ?SetProjectileWeaponFired@CGameCameraManager@@QAEXXZ
struct Inner;
struct InnerVtbl {
    int (__fastcall *slot0)(Inner*);
    int (__fastcall *slot1)(Inner*);
    int (__fastcall *slot2)(Inner*);
};
struct Inner {
    InnerVtbl* vptr;
};
class CGameCameraManager { public:
    char pad[0x98];
    Inner* inner;
    public: void SetProjectileWeaponFired();
};

extern void __fastcall Handle(Inner* p);

void CGameCameraManager::SetProjectileWeaponFired()
{
    CGameCameraManager* manager = this;
    Inner* p = manager->inner;
    int r = p->vptr->slot2(p);
    if (r == 0xf) {
        Handle(manager->inner);
    }
}