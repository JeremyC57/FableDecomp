// Behaviour test for 0x005DF790 CAnimComponentCombatFlourishTargetOffset::Alloc() const: the factory returns a fresh object
// of the same dynamic type with the IV reference count 1 (the inlined CIVCountedPointeeBase ctor), independent of `this`.
#include <cstdio>

class CIVCountedPointeeBase {
public:
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const { delete this; }
    mutable unsigned int IVRefCount;
};

class C2DVector { public: float X; float Y; };

#pragma pack(push, 1)
class CAnimComponentBase : public CIVCountedPointeeBase {
public:
    virtual int GetType() const = 0;
    virtual CAnimComponentBase* Alloc() const = 0;
};

class CAnimComponentCombatFlourishTargetOffset : public CAnimComponentBase {
public:
    virtual int GetType() const { return 7; }
    virtual CAnimComponentBase* Alloc() const;
    bool TargetOffset;
    C2DVector TargetOffsetVector;
};
#pragma pack(pop)

CAnimComponentBase* CAnimComponentCombatFlourishTargetOffset::Alloc() const
{
    return new CAnimComponentCombatFlourishTargetOffset;
}

int main()
{
    CAnimComponentCombatFlourishTargetOffset proto;
    proto.IVRefCount = 9;
    proto.TargetOffset = true;
    CAnimComponentBase* a = proto.Alloc();
    CAnimComponentBase* b = proto.Alloc();
    if (!a || !b || a == b || a == &proto) { std::printf("005df790_TEST FAIL\n"); return 1; }
    if (a->IVRefCount != 1 || b->IVRefCount != 1 || a->GetType() != 7) { std::printf("005df790_TEST FAIL\n"); return 1; }
    if (proto.IVRefCount != 9) { std::printf("005df790_TEST FAIL\n"); return 1; }
    a->Delete(); b->Delete();
    std::printf("005df790_TEST PASS\n");
    return 0;
}