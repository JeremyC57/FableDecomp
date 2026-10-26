// CDrawEnvironmentUser_00747030::SetWorld(CWorld*) -- caches the world's draw environment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00747030, length 19 bytes.
// Callees: CWorld::DrawGetEnvironment @ 0x00692620 (__fastcall on the argument, returns
// CIDrawEnvironment*); the result is stored at +0x2c and the single argument is popped (`ret 4`).
// The manifest label (CAIStateGroup_BuyFromSeller::BoughtSomething) is propagated from an
// unrelated body (BuyFromSeller is 0x18 bytes); the class is named by what the body does.
class CIDrawEnvironment;

class CWorld { public:
    CIDrawEnvironment* DrawGetEnvironment();    // 0x00692620
};

struct CDrawEnvironmentUser_00747030 {
    unsigned char _pad[0x2c];
    CIDrawEnvironment* m_pDrawEnvironment;      // +0x2c
    void SetWorld(CWorld* world);
};

void CDrawEnvironmentUser_00747030::SetWorld(CWorld* world)
{
    m_pDrawEnvironment = world->DrawGetEnvironment();
}