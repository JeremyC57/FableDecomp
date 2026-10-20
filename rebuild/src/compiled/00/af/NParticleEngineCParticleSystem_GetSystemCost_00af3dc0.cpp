// NParticleEngine::CParticleSystem::GetSystemCost(CParticleSystemCost& cost) -- sum two cost weights over the enabled component instances, turn them into per-component shares (only when the first total is positive; the second share is dropped when its total is not), then let every enabled component add its cost
// Leaf: GetSystemCost
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF3DC0, 273 bytes, `ret 4`. PDB: ?GetSystemCost@CParticleSystem@NParticleEngine@@QAEXAAVCParticleSystemCost@2@@Z.
// Callees: virtual CPSCBase slots on each component: 20 (+0x50) GetComponentCost(CParticleSystemCost&, const CParticleSystem&,
//   float, float) (PDB name/signature from the CPSCRender* overrides), 21/23 (+0x54/+0x5C) and 22/24 (+0x58/+0x60) take a
//   float accumulator by reference (the latter two also the matching total); their names are not in the PDB (descriptive).
// Layout: the component list at +0x0C is a 12-byte STLport vector of pair<unsigned long type, CComponentInstance> (0x18 bytes;
//   AddComponent 0x00AF4530 builds one): CComponentInstance is {vptr 0x0129F478, CCountedPointer<CPSCBase> Component
//   (Object +4, Info +8), bool Enabled +0x0C, unsigned long +0x10}, so the component is at pair+8 and the flag at pair+0x10.
// Shape: three loops over [first, last) with `last` re-read after every pass (virtual calls); a per-iteration
//   `CComponentInstance& instance = it->second;` makes VC carry a second induction pointer (`lea edi,[ebx+8]`, both
//   stepped by 0x18; reading `it->second` directly folds the offsets into ebx). `x > 0.0f` gives `test ah,41h ; jne`, `x <= 0.0f` gives
//   `test ah,41h ; jp`. The two share locals are declared after the first loop in frame order
//   (shareA first: its zero store lands before the fcomp, shareB's after it).
namespace NParticleEngine {

class CParticleSystem;
class CParticleSystemCost;

class CPSCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float shareA, float shareB);   // slot 20
    virtual void AddCostWeightA(float& total);                    // slot 21
    virtual void AddCostShareA(float& share, float totalA);       // slot 22
    virtual void AddCostWeightB(float& total);                    // slot 23
    virtual void AddCostShareB(float& share, float totalB);       // slot 24
};

template <class T> struct CCountedPointer {
    T*    Object;
    void* Info;
    T* operator->() const { return Object; }
};

class CComponentInstance {                         // 0x14 bytes, vtable 0x0129F478
public:
    virtual ~CComponentInstance();
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
    virtual ~CParticleSystem();
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