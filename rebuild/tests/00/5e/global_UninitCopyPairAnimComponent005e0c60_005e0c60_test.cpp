// Behaviour test for 005e0c60: placement-copy pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>> entries, bumping each non-null component's intrusive count
#include <cstdio>
#include <new>

enum EAnimComponent { ANIM_COMPONENT_NONE = 0, ANIM_COMPONENT_A = 3, ANIM_COMPONENT_B = 7 };

struct CAnimComponentBase {
    virtual void Destroy(int flags) { (void)flags; }
    virtual void Delete() { }
    unsigned int IVRefCount;
    CAnimComponentBase() : IVRefCount(0) { }
    virtual ~CAnimComponentBase() { }
};

struct CIVCountedPointer {
    CAnimComponentBase* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

struct PairAnimComponent {
    EAnimComponent    first;
    CIVCountedPointer second;
    PairAnimComponent() : first(ANIM_COMPONENT_NONE) { }
    PairAnimComponent(const PairAnimComponent& other) : first(other.first), second(other.second) { }
};

struct FalseType { FalseType() { } };

PairAnimComponent* UninitCopy(const PairAnimComponent* first, const PairAnimComponent* last, PairAnimComponent* result, const FalseType&)
{
    for (; first != last; ++first, ++result)
        new (result) PairAnimComponent(*first);
    return result;
}

int main()
{
    CAnimComponentBase comp;
    comp.IVRefCount = 2;
    PairAnimComponent src[3];
    src[0].first = ANIM_COMPONENT_A; src[0].second.Object = &comp;
    src[1].first = ANIM_COMPONENT_B; src[1].second.Object = 0;
    src[2].first = ANIM_COMPONENT_NONE; src[2].second.Object = &comp;
    void* raw = ::operator new(3 * sizeof(PairAnimComponent));
    PairAnimComponent* dst = static_cast<PairAnimComponent*>(raw);
    PairAnimComponent* end = UninitCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3;
    for (int i = 0; i < 3; ++i)
        ok = ok && dst[i].first == src[i].first && dst[i].second.Object == src[i].second.Object;
    ok = ok && comp.IVRefCount == 4;
    ::operator delete(raw);
    if (!ok) { std::printf("005e0c60_TEST FAIL\n"); return 1; }
    std::printf("005e0c60_TEST PASS\n");
    return 0;
}