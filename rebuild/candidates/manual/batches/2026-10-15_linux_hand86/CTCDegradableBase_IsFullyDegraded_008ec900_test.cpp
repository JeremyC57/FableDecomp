// Behaviour test for CTCDegradableBase::IsFullyDegraded() const @ 0x008EC900: true only when the current info is the
// definition's last degradation; the temporary def pointer's reference is given back (and the def destroyed when it
// held the last reference).
#include <cstdio>
static int g_destroyed = 0;
struct CDefPointeeBase {
    virtual void v00() { }
    virtual void DestroySelf() { ++g_destroyed; }
    long RefCount;
    void AddRef() { ++RefCount; }
    void Release() { --RefCount; if (RefCount == 0) DestroySelf(); }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object != 0) Object->AddRef(); }
    ~CDefPointer() { if (Object != 0) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CDegradableInfo { float HealthPercentage; unsigned char EngineGraphic[0x10]; long SmashParticleEmitter; bool BlocksNavigation; };
template<class T> struct CRetailVector { T* m_pFirst; T* m_pLast; T* m_pEnd; T& back() const { return *(m_pLast - 1); } };
struct CDegradableDef : CDefPointeeBase {
    unsigned char                  _pad[0x30 - 0x8];
    CRetailVector<CDegradableInfo> Degradations;
};
struct CTCDegradableBase {
    virtual void v00() { }
    unsigned char               _base[0x8];
    long                        LastWorldFrameHit;
    CDefPointer<CDegradableDef> PDefinition;
    CDegradableInfo*            PCurrentDegradableInfo;
    void*                       PParentThingPhysical;
    CTCDegradableBase(CDegradableDef* def) : PDefinition(*reinterpret_cast<CDefPointer<CDegradableDef>*>(&def)) { }
    CDefPointer<CDegradableDef> GetDefinition() const { return PDefinition; }
    bool IsFullyDegraded() const;
};
bool CTCDegradableBase::IsFullyDegraded() const
{
    if (PCurrentDegradableInfo != 0) {
        if (PCurrentDegradableInfo == &GetDefinition()->Degradations.back())
            return true;
    }
    return false;
}
int main()
{
    CDegradableInfo infos[3];
    CDegradableDef def; def.RefCount = 0; def.Degradations.m_pFirst = infos; def.Degradations.m_pLast = infos + 3; def.Degradations.m_pEnd = infos + 3;
    {
        CTCDegradableBase b(&def);                          // holds one reference
        if (def.RefCount != 1) { std::printf("008ec900_TEST FAIL\n"); return 1; }
        b.PCurrentDegradableInfo = 0;
        if (b.IsFullyDegraded()) { std::printf("008ec900_TEST FAIL\n"); return 1; }
        b.PCurrentDegradableInfo = &infos[1];
        if (b.IsFullyDegraded() || def.RefCount != 1) { std::printf("008ec900_TEST FAIL\n"); return 1; }
        b.PCurrentDegradableInfo = &infos[2];
        if (!b.IsFullyDegraded() || def.RefCount != 1 || g_destroyed != 0) { std::printf("008ec900_TEST FAIL\n"); return 1; }
        b.PDefinition.Object = 0;                           // drop the member's reference by hand
        def.RefCount = 0;
        b.PDefinition.Object = &def;                        // a member pointer that owns no reference
        b.IsFullyDegraded();                                // copy add-refs to 1, release to 0 -> destroy
        if (g_destroyed != 1) { std::printf("008ec900_TEST FAIL\n"); return 1; }
        b.PDefinition.Object = 0;
    }
    std::printf("008ec900_TEST PASS\n");
    return 0;
}
