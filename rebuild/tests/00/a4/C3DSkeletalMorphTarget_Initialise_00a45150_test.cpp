#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 00a45150 C3DSkeletalMorphTarget::Initialise
// Flags: /Gr
// Leaf: ?Initialise@C3DSkeletalMorphTarget@@QAEXABVCCharString@@ABV?$CArray@VC3DSkeletalBoneMorph@@@@@Z
#define FABLE_RETAIL_STLP_SORT_INLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_SKELETAL_MORPH_FILE_PASS30
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "fable_string.h"
#include "engine/C3DSkeletalMorph.h"

void C3DSkeletalMorphTarget::Initialise(const CCharString& name, const CArray<C3DSkeletalBoneMorph>& morph_list)
{
    Name = name;
    for (unsigned long c_morph = 0; c_morph < morph_list.size(); ++c_morph) {
        const C3DSkeletalBoneMorph& morph = morph_list[c_morph];
        if (morph.GetScaling().X != 1.0f || morph.GetScaling().Y != 1.0f || morph.GetScaling().Z != 1.0f)
            BoneMorphs.push_back(morph);
    }
    std::sort(BoneMorphs.begin(), BoneMorphs.end(), C3DSkeletalBoneMorph::CompareCRCs);
}

namespace {
int failed;
struct TextEntry { const void* owner; char value[256]; };
TextEntry texts[128];
void require(bool condition) { if (!condition) ++failed; }
const char* get(const void* p)
{
    for (int i = 0; i < 128; ++i) if (texts[i].owner == p) return texts[i].value;
    require(false);
    return "";
}
void set(const void* p, const char* s, long length = -1)
{
    int i;
    for (i = 0; i < 128 && texts[i].owner != p; ++i) { }
    if (i == 128) for (i = 0; i < 128 && texts[i].owner; ++i) { }
    if (i == 128) { require(false); return; }
    if (length < 0) length = (long)strlen(s);
    if (length > 255) { require(false); return; }
    texts[i].owner = p;
    memmove(texts[i].value, s, length);
    texts[i].value[length] = 0;
}
void remove_text(const void* p)
{
    for (int i = 0; i < 128; ++i) if (texts[i].owner == p) { texts[i].owner = 0; return; }
    require(false);
}
}
CCharString::CCharString() : PStringData(0) { set(this, ""); }
CCharString::CCharString(const char* s, long n) : PStringData(0) { set(this, s, n); }
CCharString::CCharString(const CCharString& s) : PStringData(0) { set(this, get(&s)); }
CCharString::~CCharString() { remove_text(this); }

int assignments;
const CCharString& CCharString::operator=(const CCharString& other)
{
    ++assignments;
    if (this != &other) set(this, get(&other));
    return *this;
}
C3DSkeletalBoneMorph::C3DSkeletalBoneMorph(const CCharString& n, const C3DVector& scale)
    : BoneName(n), BoneNameCRC((unsigned long)atoi(get(&n))), Scaling(scale) { }
bool FABLE_FASTCALL C3DSkeletalBoneMorph::CompareCRCs(const C3DSkeletalBoneMorph& lhs, const C3DSkeletalBoneMorph& rhs)
{
    return lhs.BoneNameCRC < rhs.BoneNameCRC;
}
namespace std {
template<> vector<C3DSkeletalBoneMorph>::~vector() { _Destroy(this->begin(),this->end()); }
}

namespace {
int intros, insertions, tails, depth, wanted;
template<class I, class C> void order(I first, I last, C comp)
{
    for (I i = first; i != last; ++i) {
        I j = i;
        while (j != first && comp(*j, *(j-1))) {
            C3DSkeletalBoneMorph value(*j);
            *j = *(j-1); *(j-1) = value;
            --j;
        }
    }
}
}
namespace std {
template<class I, class T, class N, class C>
void FABLE_FASTCALL __introsort_loop(I first, I last, T* tag, N limit, C comp)
{
    ++intros;
    require(last-first == wanted && limit == depth && tag == 0 && assignments > 0 && comp == &C3DSkeletalBoneMorph::CompareCRCs);
    order(first,last,comp);
}
template<class I, class C>
void FABLE_FASTCALL __insertion_sort(I first, I last, C comp)
{
    ++insertions;
    require(last-first == (wanted > 16 ? 16 : wanted));
    order(first,last,comp);
}
template<class I, class T, class C>
void FABLE_FASTCALL __unguarded_insertion_sort_aux(I first, I last, T* tag, C comp)
{
    ++tails;
    require(last-first == wanted-16 && tag == 0);
    order(first,last,comp);
}
}
int main()
{
    for (int n = 0; n <= 24; ++n) {
        C3DSkeletalMorphTarget target;
        CArray<C3DSkeletalBoneMorph> input;
        bool retained[25] = {false};
        wanted = 0;
        for (int i = n; i >= 1; --i) {
            char label[10]; sprintf(label,"%d",i);
            C3DVector s;
            s.X = i%4 == 0 ? 0.5f : 1.0f;
            s.Y = i%4 == 1 ? 2.0f : 1.0f;
            s.Z = i%4 == 2 ? -2.0f : 1.0f;
            retained[i] = i%4 != 3;
            wanted += retained[i] ? 1 : 0;
            input.push_back(C3DSkeletalBoneMorph(CCharString(label),s));
        }
        for (int pass=0; pass<2; ++pass) {
            CArray<C3DSkeletalBoneMorph> empty;
            depth = 0; for (int k=wanted; k>1; k/=2) depth+=2;
            intros = insertions = tails = assignments = 0;
            CCharString name(pass ? "second" : "first");
            target.Initialise(name,pass ? empty : input);
            require(assignments > 0 && intros == (wanted>0) && insertions == (wanted>0) && tails == (wanted>16));
            const CArray<C3DSkeletalBoneMorph>& result = target.GetBoneMorphs();
            require((int)result.size() == wanted);
            int j=0;
            for (int index=1; index<=n; ++index) if (retained[index]) {
                require(j < (int)result.size() && atoi(get(&result[j].GetName())) == index);
                ++j;
            }
        }
    }
    for (int i=0; i<128; ++i) require(texts[i].owner==0);
    if (failed) {printf("failures %d\n",failed);return 1;}
    puts("00a45150_TEST PASS");
    return 0;
}
