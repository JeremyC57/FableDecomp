#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
// Behaviour test for CEngineClothSimulationMeshBase::ClearStaticParticleSims() @ 0x00B81450: every pooled simulation
// whose render data is missing or idle is erased (adjacent ones included, the size re-read each pass); busy ones stay
// in order.
class CParticleSimulationRenderData {
public:
    bool Busy;
    virtual ~CParticleSimulationRenderData() { }
    virtual bool IsBusy() const { return Busy; }
};
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
    T* Get() const { return Data; }
    T* operator->() const { return Data; }
};
class CParticleSimulation {
public:
    int Id;
    virtual ~CParticleSimulation() { }
    CCountedPointer<CParticleSimulationRenderData> RenderData;
    bool IsRenderDataBusy() const { return RenderData.Get() != 0 && RenderData->IsBusy(); }
};
template <class _Tp> class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator begin() { return _M_start; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp& operator[](unsigned int __n) { return *(begin() + __n); }
    iterator erase(iterator __position)                 // 0x00B835D0 stand-in
    {
        for (iterator p = __position; p + 1 != _M_finish; ++p) *p = *(p + 1);
        --_M_finish;
        return __position;
    }
};
class CEngineClothSimulationMeshBase {
public:
    static vector<CCountedPointer<CParticleSimulation> > StaticParticleSims;
    static void __fastcall ClearStaticParticleSims()
    {
        for (unsigned long i = 0; i < StaticParticleSims.size(); ++i) {
            if (!StaticParticleSims[i]->IsRenderDataBusy()) {
                StaticParticleSims.erase(StaticParticleSims.begin() + i);
                --i;
            }
        }
    }
};
vector<CCountedPointer<CParticleSimulation> > CEngineClothSimulationMeshBase::StaticParticleSims;
int main()
{
    // sims 0..6: 0 idle, 1 busy, 2 no render data, 3 idle, 4 busy, 5 busy, 6 idle
    static const int kind[7] = { 0, 1, 2, 0, 1, 1, 0 };
    CParticleSimulation sims[7];
    CParticleSimulationRenderData rd[7];
    CCountedPointer<CParticleSimulation> storage[7];
    for (int i = 0; i < 7; ++i) {
        sims[i].Id = i;
        rd[i].Busy = kind[i] == 1;
        sims[i].RenderData.Data = kind[i] == 2 ? 0 : &rd[i];
        sims[i].RenderData.Info = 0;
        storage[i].Data = &sims[i]; storage[i].Info = 0;
    }
    vector<CCountedPointer<CParticleSimulation> >& v = CEngineClothSimulationMeshBase::StaticParticleSims;
    v._M_start = storage; v._M_finish = storage + 7; v._M_end_of_storage = storage + 7;
    CEngineClothSimulationMeshBase::ClearStaticParticleSims();
    if (v.size() != 3 || v[0]->Id != 1 || v[1]->Id != 4 || v[2]->Id != 5) { std::printf("00b81450_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) rd[i].Busy = false;
    CEngineClothSimulationMeshBase::ClearStaticParticleSims();
    if (v.size() != 0) { std::printf("00b81450_TEST FAIL\n"); return 1; }
    CEngineClothSimulationMeshBase::ClearStaticParticleSims();   // empty pool
    if (v.size() != 0) { std::printf("00b81450_TEST FAIL\n"); return 1; }
    std::printf("00b81450_TEST PASS\n");
    return 0;
}