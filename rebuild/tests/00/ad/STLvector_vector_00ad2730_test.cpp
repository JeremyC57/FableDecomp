// Behaviour test for _STL::vector<QuadricOptimiserInternals::COptimisedBone>::vector(const vector&) @ 0x00AD2730
// Allocates exactly x.size() elements and copy-constructs x's elements (the name string is deep-copied).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int gStringCopies = 0;

struct CNameModel {
    char* Text;
    CNameModel(const char* text) : Text((char*)std::malloc(std::strlen(text) + 1)) { std::strcpy(Text, text); }
    CNameModel(const CNameModel& other) : Text((char*)std::malloc(std::strlen(other.Text) + 1))
    {
        std::strcpy(Text, other.Text);
        ++gStringCopies;
    }
    ~CNameModel() { std::free(Text); }
};

struct COptimisedBoneModel {
    CNameModel BoneName;
    int Parent;
    bool Remapped;
    int NewIndex;
    COptimisedBoneModel(const char* name, int parent) : BoneName(name), Parent(parent), Remapped(false), NewIndex(-1) { }
};

struct CBoneVector {
    COptimisedBoneModel* start;
    COptimisedBoneModel* finish;
    COptimisedBoneModel* end;
    unsigned int size() const { return (unsigned int)(finish - start); }
    CBoneVector() : start(0), finish(0), end(0) { }
    CBoneVector(const CBoneVector& x)
    {
        unsigned int n = x.size();
        start = n != 0 ? (COptimisedBoneModel*)std::malloc(n * sizeof(COptimisedBoneModel)) : 0;
        finish = start;
        end = start + n;
        for (const COptimisedBoneModel* p = x.start; p != x.finish; ++p, ++finish)
            new (finish) COptimisedBoneModel(*p);
    }
    void destroy()
    {
        for (COptimisedBoneModel* p = start; p != finish; ++p) p->~COptimisedBoneModel();
        std::free(start);
    }
};

int main()
{
    CBoneVector src;
    src.start = (COptimisedBoneModel*)std::malloc(4 * sizeof(COptimisedBoneModel));
    src.finish = src.start; src.end = src.start + 4;
    new (src.finish++) COptimisedBoneModel("root", -1);
    new (src.finish++) COptimisedBoneModel("spine", 0);
    new (src.finish++) COptimisedBoneModel("head", 1);
    int before = gStringCopies;
    CBoneVector copy(src);
    int ok = copy.size() == 3 && copy.end == copy.start + 3 && gStringCopies - before == 3;
    if (ok && (std::strcmp(copy.start[1].BoneName.Text, "spine") != 0 || copy.start[2].Parent != 1
               || copy.start[0].BoneName.Text == src.start[0].BoneName.Text)) ok = 0;
    CBoneVector empty;
    CBoneVector emptyCopy(empty);
    if (emptyCopy.start != 0 || emptyCopy.size() != 0) ok = 0;
    copy.destroy();
    src.destroy();
    if (!ok) { std::printf("00ad2730_TEST FAIL\n"); return 1; }
    std::printf("00ad2730_TEST PASS\n");
    return 0;
}