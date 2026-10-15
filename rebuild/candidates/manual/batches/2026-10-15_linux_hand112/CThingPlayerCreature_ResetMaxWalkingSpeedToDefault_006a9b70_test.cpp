#include <cstdio>
#include <cstring>
struct CCharString {
    const char* m_pData;
    CCharString(const char* text, long length) : m_pData(text) { (void)length; }
    ~CCharString() { m_pData = 0; }
};
struct CThingCreatureDef { float MaxWalkingSpeed; };
template <class T> struct CDefPointer {
    T* m_pDef;
    T* operator->() const { return m_pDef; }
};
struct CThingCreatureBase {
    CDefPointer<CThingCreatureDef>  PDef;
    float* m_lastSpeed; char m_lastName[16]; float m_seen;
    void InitialiseMovementSpeed(float* speed, const CCharString& name)
    { m_lastSpeed = speed; std::strcpy(m_lastName, name.m_pData); m_seen = *speed; }
};
struct CThingPlayerCreature : CThingCreatureBase {
    float MaxWalkingSpeed;
    void ResetMaxWalkingSpeedToDefault();
};
void CThingPlayerCreature::ResetMaxWalkingSpeedToDefault()
{
    MaxWalkingSpeed = PDef->MaxWalkingSpeed;
    CCharString name("ST_WALK", -1);
    InitialiseMovementSpeed(&MaxWalkingSpeed, name);
}
int main()
{
    CThingCreatureDef d; d.MaxWalkingSpeed = 4.5f;
    CThingPlayerCreature c; c.PDef.m_pDef = &d; c.MaxWalkingSpeed = 0.0f; c.m_lastSpeed = 0; c.m_seen = 0.0f;
    c.ResetMaxWalkingSpeedToDefault();
    if (c.MaxWalkingSpeed != 4.5f || c.m_lastSpeed != &c.MaxWalkingSpeed || c.m_seen != 4.5f || std::strcmp(c.m_lastName, "ST_WALK") != 0)
    { std::printf("006a9b70_TEST FAIL\n"); return 1; }
    std::printf("006a9b70_TEST PASS\n");
    return 0;
}
