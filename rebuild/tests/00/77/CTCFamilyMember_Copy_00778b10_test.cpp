// Behaviour test for CTCFamilyMember::Copy (0x00778B10): the other component's FamilyMembers vector is assigned to this one
// (element-wise, capacity grown as needed); the CTCBase part is not copied.
#include <cstdio>
#include <cstdlib>
#include <cstring>
class CThing { public: int id; };
struct ThingVector {                                    // stand-in for STLport vector<const CThing*>
    const CThing** first; const CThing** last; const CThing** end;
    ThingVector() : first(0), last(0), end(0) {}
    ~ThingVector() { std::free(first); }
    unsigned size() const { return (unsigned)(last - first); }
    void push_back(const CThing* t)
    {
        if (last == end) {
            unsigned n = size(), cap = n ? n * 2 : 4;
            const CThing** p = (const CThing**)std::malloc(cap * sizeof(const CThing*));
            if (n) std::memcpy(p, first, n * sizeof(const CThing*));
            std::free(first); first = p; last = p + n; end = p + cap;
        }
        *last++ = t;
    }
    ThingVector& operator=(const ThingVector& o)
    {
        if (&o != this) { last = first; for (unsigned i = 0; i < o.size(); ++i) push_back(o.first[i]); }
        return *this;
    }
};
class CTCBase {
public:
    CTCBase() : ParentThing(0), ToBeRemoved(false) {}
    virtual ~CTCBase() {}
    virtual void Copy(const CTCBase& other) = 0;
    CThing* ParentThing;
    bool ToBeRemoved;
};
class CTCFamilyMember : public CTCBase {
public:
    virtual void Copy(const CTCBase& other)
    {
        const CTCFamilyMember& o = (const CTCFamilyMember&)other;
        FamilyMembers = o.FamilyMembers;
    }
    ThingVector FamilyMembers;
};
int main()
{
    CThing a, b, c, d;
    CTCFamilyMember src, dst;
    src.FamilyMembers.push_back(&a); src.FamilyMembers.push_back(&b); src.FamilyMembers.push_back(&c);
    src.ParentThing = &d; src.ToBeRemoved = true;
    dst.FamilyMembers.push_back(&d);
    CTCBase& base = dst;
    base.Copy(src);
    if (dst.FamilyMembers.size() != 3 || dst.FamilyMembers.first[0] != &a || dst.FamilyMembers.first[2] != &c ||
        dst.ParentThing != 0 || dst.ToBeRemoved || src.FamilyMembers.size() != 3) {
        std::printf("00778b10_TEST FAIL\n"); return 1;
    }
    std::printf("00778b10_TEST PASS\n");
    return 0;
}