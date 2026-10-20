#include <cstdio>
namespace NParticleEngine {

class CParticleSystem;
class CParticleSystemCost { public: int calls; };

class CPSCBase {
public:
    float wa, wb; float sa, sb; float gotA, gotB; const CParticleSystem* sys;
    virtual void GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float shareA, float shareB);
    virtual void AddCostWeightA(float& total) { total += wa; }
    virtual void AddCostShareA(float& share, float totalA) { share += wa / totalA * sa; }
    virtual void AddCostWeightB(float& total) { total += wb; }
    virtual void AddCostShareB(float& share, float totalB) { share += (totalB > 0.0f ? wb / totalB : 99.0f) * sb; }
    virtual ~CPSCBase() {}
};

template <class T> struct CCountedPointer {
    T*    Object;
    void* Info;
    T* operator->() const { return Object; }
};

class CComponentInstance {                         // 0x14 bytes, vtable 0x0129F478
public:
    virtual ~CComponentInstance() {}
    CCountedPointer<CPSCBase> Component;           // +0x04
    bool                      Enabled;             // +0x0c
    unsigned long             Flags;               // +0x10
};

struct SComponentEntry {                           // pair<unsigned long, CComponentInstance>, 0x18 bytes
    unsigned long      first;
    CComponentInstance second;
};

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
};

class CParticleSystem {
public:
    virtual ~CParticleSystem() {}
    unsigned char           _pad04[8];
    vector<SComponentEntry> Components;            // +0x0c
    void GetSystemCost(CParticleSystemCost& cost);
};

void CParticleSystem::GetSystemCost(CParticleSystemCost& cost)
{
    float totalA = 0.0f;
    float totalB = 0.0f;
    SComponentEntry* it;
    for (it = Components.begin(); it != Components.end(); ++it) {
        CComponentInstance& instance = it->second;
        if (instance.Enabled) {
            instance.Component->AddCostWeightA(totalA);
            instance.Component->AddCostWeightB(totalB);
        }
    }
    float shareA = 0.0f;
    float shareB = 0.0f;
    if (totalA > 0.0f) {
        for (it = Components.begin(); it != Components.end(); ++it) {
            CComponentInstance& instance = it->second;
            if (instance.Enabled) {
                instance.Component->AddCostShareA(shareA, totalA);
                instance.Component->AddCostShareB(shareB, totalB);
            }
        }
    }
    if (totalB <= 0.0f)
        shareB = 0.0f;
    for (it = Components.begin(); it != Components.end(); ++it) {
        CComponentInstance& instance = it->second;
        if (instance.Enabled)
            instance.Component->GetComponentCost(cost, *this, shareA, shareB);
    }
}

} // namespace NParticleEngine

void NParticleEngine::CPSCBase::GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float shareA, float shareB)
{ ++cost.calls; gotA = shareA; gotB = shareB; sys = &system; }
using namespace NParticleEngine;
int main()
{
    CPSCBase c[3];
    for (int i = 0; i < 3; ++i) { c[i].wa = (float)(i + 1); c[i].wb = 0.0f; c[i].sa = 1.0f; c[i].sb = 1.0f; c[i].gotA = c[i].gotB = -1.0f; c[i].sys = 0; }
    SComponentEntry e[3];
    for (int i = 0; i < 3; ++i) { e[i].first = i; e[i].second.Component.Object = &c[i]; e[i].second.Component.Info = 0; e[i].second.Enabled = (i != 1); }
    CParticleSystem s; s.Components._M_start = e; s.Components._M_finish = e + 3; s.Components._M_end_of_storage = e + 3;
    CParticleSystemCost cost; cost.calls = 0;
    s.GetSystemCost(cost);
    /* enabled weights: 1 and 3 -> totalA 4; totalB 0 so shareB forced to 0; shareA = 1/4 + 3/4 = 1 */
    if (cost.calls != 2 || c[1].gotA != -1.0f || c[0].gotA != 1.0f || c[2].gotA != 1.0f || c[0].gotB != 0.0f || c[2].sys != &s) { std::printf("00af3dc0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) { c[i].wa = 0.0f; c[i].wb = 2.0f; }
    cost.calls = 0; s.GetSystemCost(cost);
    /* totalA 0: no shares computed at all */
    if (cost.calls != 2 || c[0].gotA != 0.0f || c[0].gotB != 0.0f) { std::printf("00af3dc0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) { c[i].wa = 1.0f; c[i].wb = 2.0f; }
    cost.calls = 0; s.GetSystemCost(cost);
    if (cost.calls != 2 || c[2].gotA != 1.0f || c[2].gotB != 1.0f) { std::printf("00af3dc0_TEST FAIL\n"); return 1; }
    std::printf("00af3dc0_TEST PASS\n");
    return 0;
}