// Behaviour test for CTCSkeletalMorph::Copy (0x00786650): Changed is set and the Morphs array is copied from the other component.
#include <cstdio>
struct CSkeletalMorph { long ID; int Name; float Strength; };
struct CMorphArray {                       // stands in for CArray<CSkeletalMorph> (STLport vector assignment)
    CSkeletalMorph items[4];
    int count;
    CMorphArray() : count(0) { }
    CMorphArray& operator=(const CMorphArray& o)
    {
        if (&o != this) { count = o.count; for (int i = 0; i < count; ++i) items[i] = o.items[i]; }
        return *this;
    }
};
class CTCBase { public: virtual ~CTCBase() { } void* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    virtual void Copy(const CTCBase&) { } };
class CTCSkeletalMorph : public CTCBase { public:
    bool Changed; CMorphArray Morphs;
    CTCSkeletalMorph() : Changed(false) { }
    virtual void Copy(const CTCBase& other)
    {
        const CTCSkeletalMorph& o = (const CTCSkeletalMorph&)other;
        Changed = true;
        Morphs = o.Morphs;
    }
};
int main()
{
    CTCSkeletalMorph a, b;
    b.Morphs.count = 2; b.Morphs.items[0].ID = 7; b.Morphs.items[1].ID = 9; b.Morphs.items[1].Strength = 0.5f;
    CTCBase& base = a;
    base.Copy(b);
    if (!a.Changed || a.Morphs.count != 2 || a.Morphs.items[0].ID != 7 || a.Morphs.items[1].ID != 9 || b.Changed) {
        std::printf("00786650_TEST FAIL\n"); return 1;
    }
    a.Changed = false; base.Copy(a);
    if (!a.Changed || a.Morphs.count != 2) { std::printf("00786650_TEST FAIL\n"); return 1; }
    std::printf("00786650_TEST PASS\n");
    return 0;
}