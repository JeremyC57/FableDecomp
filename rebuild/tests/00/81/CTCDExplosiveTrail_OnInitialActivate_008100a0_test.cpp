// Test for 008100a0 CTCDExplosiveTrail::OnInitialActivate: a counted copy of the thing's def fetches the trail sub-def;
// the copy's add-ref is undone afterwards (destroy only when the copy held the last count).
#include <cstdio>
class CExplosiveTrailDef;
template <class T> class CDefPointer;
class CDefClassBase { public:
    long m_refCount; int Destroyed; CExplosiveTrailDef* SubDef; long CountDuringCall;
    virtual ~CDefClassBase() {}
    virtual void Destroy() { ++Destroyed; }
    bool GetSubDef(CDefPointer<CExplosiveTrailDef>* out);
};
class CExplosiveTrailDef : public CDefClassBase { public:};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer()
    {
        T* object = Object;
        if (object != 0) {
            if (--object->m_refCount == 0)
                object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};
bool CDefClassBase::GetSubDef(CDefPointer<CExplosiveTrailDef>* out) { CountDuringCall = m_refCount; out->Object = SubDef; return true; }
struct CThingDef : CDefClassBase {};
class CThing { public: CDefPointer<CThingDef> PDef; };
class CTCDExplosiveTrail { public:
    CThing* ParentThing;
    CDefPointer<CExplosiveTrailDef> PDef;
    void OnInitialActivate()
    {
        CDefPointer<CThingDef> thingDef(ParentThing->PDef);
        thingDef->GetSubDef(&PDef);
    }
};
int main()
{
    CExplosiveTrailDef sub; sub.m_refCount = 100; sub.Destroyed = 0;
    CThingDef def; def.m_refCount = 1; def.Destroyed = 0; def.SubDef = &sub;
    CThing thing; thing.PDef.Object = &def;
    CTCDExplosiveTrail trail; trail.ParentThing = &thing;
    trail.OnInitialActivate();
    if (trail.PDef.Object != &sub || def.CountDuringCall != 2 || def.m_refCount != 1 || def.Destroyed != 0) {
        std::printf("008100a0_TEST FAIL\n"); return 1;
    }
    thing.PDef.Object = 0; trail.PDef.Object = 0;  // detach before the stack objects unwind
    std::printf("008100a0_TEST PASS\n");
    return 0;
}