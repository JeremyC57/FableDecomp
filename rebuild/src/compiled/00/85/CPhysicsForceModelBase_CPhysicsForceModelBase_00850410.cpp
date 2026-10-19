// CPhysicsForceModelBase::CPhysicsForceModelBase(CBase*, CGameCameraManager*, CPhysicsState*) -- initialize linked state and three 0x24-byte force submodels; retail @ 0x00850410, 199 bytes.
// The landed GetVelocity model confirms the vptr +0 and LinkedState +4 base layout. Each submodel constructor call is the external helper at 0x008FB170, with six scalar parameters stored at subobject offsets +0xc..+0x20. The trailing state, float, flags, owner and camera-manager fields follow the retail stores at +0x74..+0xa8.
struct CBase;
struct CGameCameraManager;
struct CPhysicsState;
struct CPhysicsForceSubmodel {
    unsigned char _storage[0x24];
    CPhysicsForceSubmodel(long kind, float a, float b, long c, long d, long e); // 0x008FB170
};
struct CPhysicsForceModelBase {
    virtual void Slot00();
    CPhysicsState* LinkedState;
    CPhysicsForceSubmodel first;
    CPhysicsForceSubmodel second;
    CPhysicsForceSubmodel third;
    long value74, value78, value7c, value80, value84, value88, value8c, value90, value94;
    float scale98;
    unsigned long flags9c;
    unsigned char enabledA0;
    unsigned char padA1[3];
    CBase* owner;
    CGameCameraManager* cameraManager;
    CPhysicsForceModelBase(CBase* owner, CGameCameraManager* camera, CPhysicsState* state);
};
CPhysicsForceModelBase::CPhysicsForceModelBase(CBase* owner, CGameCameraManager* camera, CPhysicsState* state)
    : LinkedState(state),
      first(10, 0.0f, 0.05f, 255, 0, 0),
      second(20, 0.25f, 0.26f, 255, 180, 60),
      third(10, 0.25f, 0.05f, 0, 0, 255),
      value74(0), value78(0), value7c(0), value80(0), value84(0), value88(0), value8c(0), value90(0), value94(0),
      scale98(1.0f), flags9c(0xffffffff), enabledA0(0), owner(owner), cameraManager(camera)
{
}

void CPhysicsForceModelBase::Slot00() {}