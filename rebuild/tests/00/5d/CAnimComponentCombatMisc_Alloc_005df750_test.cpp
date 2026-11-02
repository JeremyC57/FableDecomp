// Behaviour test for CAnimComponentCombatMisc::Alloc (0x005DF750): a new object of the same dynamic type with reference
// count 1 (and 10 bytes on the 32-bit 1-packed build).
#include <cstdio>
#pragma pack(push, 1)
class CIVCountedPointeeBase { public:
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
    unsigned int IVRefCount;
};
class CAnimComponentBase : public CIVCountedPointeeBase { public:
    virtual long GetType() const { return -1; }
    virtual CAnimComponentBase* Alloc() const { return 0; }
};
class CAnimComponentCombatMisc : public CAnimComponentBase { public:
    virtual long GetType() const { return 6; }
    virtual CAnimComponentBase* Alloc() const { return new CAnimComponentCombatMisc; }
    bool MeleeFlourish; bool MeleeKnockdown;
};
#pragma pack(pop)
int main()
{
    CAnimComponentCombatMisc proto; proto.IVRefCount = 5;
    const CAnimComponentBase& b = proto;
    CAnimComponentBase* made = b.Alloc();
    if (!made || made == &proto || made->GetType() != 6 || made->IVRefCount != 1) { std::printf("005df750_TEST FAIL\n"); return 1; }
    if (sizeof(void*) == 4 && sizeof(CAnimComponentCombatMisc) != 10) { std::printf("005df750_TEST FAIL\n"); return 1; }
    delete made;
    std::printf("005df750_TEST PASS\n");
    return 0;
}