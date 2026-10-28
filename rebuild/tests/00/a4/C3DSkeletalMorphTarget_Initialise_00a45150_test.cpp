#include <stdio.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
#ifdef _MSC_VER
#include "fable_string.h"
#else
class CCharStringData { public:};
class CCharString { CCharStringData* storage_; public:
 CCharString(); CCharString(const char*,long); ~CCharString();
 const CCharString& operator=(const CCharString&);
 operator const char*() const;
};

#endif
// C3DSkeletalMorphTarget::Initialise(CCharString&, CArray<C3DSkeletalBoneMorph>&) -- copy the name and retain nonidentity bone records as this target's sparse morph list.
// Retail: 0x00A45150, 270 bytes plus two padding bytes. CCharString assignment 0x0099EFB0; bone-array push_back 0x0088B610; sort helpers 0x00A46E00/0x00A46680/0x00A45F90.
// PDB: Name at0, BoneMorphs at4; native array is12 bytes versus donor16. Each bone is20 bytes: name0, unsigned CRC4, scale words8/12/16.
// Identity uses exact IEEE words (all0x3f800000). Append preserves existing records and reloads input bounds after each push.
// Helpers use ECX/EDX first/last and a fastcall unsigned-CRC predicate; native comparator0x00A44C90 is independently byte-exact.
// Standard logarithm/algorithm expression retains the original compiler instruction ordering without assembly or artificial storage.
class C3DSkeletalBoneMorph { public:
    CCharString BoneName;
    unsigned long BoneNameCRC;
    unsigned long XBits, YBits, ZBits; // IEEE-754 words for the three scale components
    C3DSkeletalBoneMorph(const C3DSkeletalBoneMorph& o);
    C3DSkeletalBoneMorph& operator=(const C3DSkeletalBoneMorph& o);
};
struct CArray {
    C3DSkeletalBoneMorph* first; C3DSkeletalBoneMorph* last; C3DSkeletalBoneMorph* end;
    unsigned long size() const { return (unsigned long)(last - first); }
    C3DSkeletalBoneMorph& operator[](unsigned long i) const { return first[i]; }
};
struct CVector {
    C3DSkeletalBoneMorph* first; C3DSkeletalBoneMorph* last; C3DSkeletalBoneMorph* end;
    void push_back(const C3DSkeletalBoneMorph&);
};
class C3DSkeletalMorphTarget { public:
    CCharString Name; CVector BoneMorphs;
    void Initialise(CCharString& name, CArray& bones);
};
bool __fastcall BoneMorphLess(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b) // 0x00A44C90
{
    return a.BoneNameCRC < b.BoneNameCRC;
}
void __fastcall IntroSort(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last,
                          long, long depth, bool (__fastcall* less)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)); // 0x00A46E00
void __fastcall InsertionSort(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last,
                              bool (__fastcall* less)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)); // 0x00A46680
void __fastcall InsertionSortTail(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last,
                                  long, bool (__fastcall* less)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)); // 0x00A45F90

template<class Size> __forceinline Size MorphLog(Size n)
{
    Size depth;
    for (depth = 0; n != 1; n >>= 1) ++depth;
    return depth;
}

__forceinline void SortMorphs(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last)
{
    if (first != last) {
        IntroSort(first, last, 0, MorphLog((long)(last - first)) * 2, BoneMorphLess);
        if (last - first > 16) {
            InsertionSort(first, first + 16, BoneMorphLess);
            InsertionSortTail(first + 16, last, 0, BoneMorphLess);
        } else {
            InsertionSort(first, last, BoneMorphLess);
        }
    }
}

void C3DSkeletalMorphTarget::Initialise(CCharString& name, CArray& bones)
{
    Name = name;
    C3DSkeletalMorphTarget* target = this;
    for (unsigned long i = 0; i < bones.size(); ++i) {
        C3DSkeletalBoneMorph* p = &bones[i];
        if (p->XBits != 0x3f800000UL || p->YBits != 0x3f800000UL || p->ZBits != 0x3f800000UL)
            target->BoneMorphs.push_back(*p);
    }
    SortMorphs(BoneMorphs.first, BoneMorphs.last);
}

CCharString::CCharString():storage_(0) {}
CCharString::CCharString(const char* p,long):storage_((CCharStringData*)p) {}
CCharString::~CCharString() {}
CCharString::operator const char*() const {return (const char*)storage_;}
static int nameAssignments;
const CCharString& CCharString::operator=(const CCharString& x) { storage_=x.storage_;++nameAssignments;return *this; }

C3DSkeletalBoneMorph::C3DSkeletalBoneMorph(const C3DSkeletalBoneMorph& x)
 :BoneName(x.BoneName),BoneNameCRC(x.BoneNameCRC),XBits(x.XBits),YBits(x.YBits),ZBits(x.ZBits) {}
C3DSkeletalBoneMorph& C3DSkeletalBoneMorph::operator=(const C3DSkeletalBoneMorph& x) {
 BoneName=x.BoneName;BoneNameCRC=x.BoneNameCRC;XBits=x.XBits;YBits=x.YBits;ZBits=x.ZBits;return *this;
}
static int intro, insertion, tail, bad;
static long expectedDepth;
static C3DSkeletalBoneMorph* expectedFirst;
static C3DSkeletalBoneMorph* expectedLast;
static CArray* mutateSource;
static C3DSkeletalBoneMorph* replacementFirst;
static C3DSkeletalBoneMorph* replacementLast;
static int mutateOnce;
static int assignedName;
typedef bool (__fastcall* Less)(const C3DSkeletalBoneMorph&,const C3DSkeletalBoneMorph&);
static void sort_records(C3DSkeletalBoneMorph* a,C3DSkeletalBoneMorph* b,Less f) {
 for(C3DSkeletalBoneMorph* i=a;i!=b;++i) for(C3DSkeletalBoneMorph* j=i;j!=a&&f(*j,*(j-1));--j) {
  C3DSkeletalBoneMorph x(*j);*j=*(j-1);*(j-1)=x;
 }
}
void CVector::push_back(const C3DSkeletalBoneMorph& x) {
 if (!nameAssignments || last==end) {bad=1;return;} *last++=x;
 if(mutateOnce) {mutateOnce=0;mutateSource->first=replacementFirst;mutateSource->last=replacementLast;}
}
void __fastcall IntroSort(C3DSkeletalBoneMorph* a,C3DSkeletalBoneMorph* b,long z,long d,Less f) {
 ++intro;if(z||d!=expectedDepth||f!=BoneMorphLess||a!=expectedFirst||b!=expectedLast)bad=1;sort_records(a,b,f);
}
void __fastcall InsertionSort(C3DSkeletalBoneMorph* a,C3DSkeletalBoneMorph* b,Less f) {++insertion;if(a!=expectedFirst||b!=(expectedLast-expectedFirst>16?expectedFirst+16:expectedLast)||f!=BoneMorphLess)bad=1;sort_records(a,b,f);}
void __fastcall InsertionSortTail(C3DSkeletalBoneMorph* a,C3DSkeletalBoneMorph* b,long z,Less f) {++tail;if(z||a!=expectedFirst+16||b!=expectedLast||f!=BoneMorphLess)bad=1;sort_records(a,b,f);}
static C3DSkeletalBoneMorph* alloc_records(unsigned char* bytes,int n) {
 C3DSkeletalBoneMorph* a=(C3DSkeletalBoneMorph*)bytes;
 for(int i=0;i<n;++i){new(&a[i].BoneName)CCharString();a[i].BoneNameCRC=0;a[i].XBits=a[i].YBits=a[i].ZBits=0x3f800000UL;}
 return a;
}
int main() {
 union { double alignment;unsigned char raw[sizeof(C3DSkeletalBoneMorph)*40];} in,out;
 C3DSkeletalBoneMorph* a=alloc_records(in.raw,40);C3DSkeletalBoneMorph* b=alloc_records(out.raw,40);
 C3DSkeletalMorphTarget t;CCharString name("target",-1);CArray v;
 t.BoneMorphs.first=t.BoneMorphs.last=b;t.BoneMorphs.end=b+40;v.first=v.last=a;v.end=a+40;
 t.Initialise(name,v);if(intro||insertion||tail||strcmp((const char*)t.Name,"target"))bad=1;
 a[0].BoneNameCRC=0xffffffffUL;a[1].BoneNameCRC=0x80000000UL;a[2].BoneNameCRC=0x7fffffffUL;
 a[0].XBits=0x7fc00001UL;a[1].YBits=0;a[2].ZBits=0x40000000UL;
 v.last=a+4;expectedFirst=b;expectedLast=b+3;expectedDepth=2;t.Initialise(name,v);
 if(t.BoneMorphs.last-b!=3||b[0].BoneNameCRC!=0x7fffffffUL||b[1].BoneNameCRC!=0x80000000UL||b[2].BoneNameCRC!=0xffffffffUL||intro!=1||insertion!=1||tail)bad=1;
 // Existing output is retained, then sorted together with newly appended records.
 for(int i=0;i<20;++i){a[i].BoneNameCRC=20-i;a[i].XBits=0;}
 v.last=a+20;expectedLast=b+23;expectedDepth=8;t.Initialise(name,v);
 if(t.BoneMorphs.last-b!=23||intro!=2||insertion!=2||tail!=1)bad=1;
 for(C3DSkeletalBoneMorph* q=b+1;q!=t.BoneMorphs.last;++q)if(q->BoneNameCRC<(q-1)->BoneNameCRC)bad=1;
 // Exact one/16/17 boundaries, independent invocation and all identity axes.
 for(int count=1;count<=17;++count) {
  t.BoneMorphs.last=b; v.first=a;v.last=a+count;
  for(int j=0;j<count;++j){a[j].BoneNameCRC=(unsigned long)(count-j);a[j].XBits=0x3f800000UL;a[j].YBits=0x3f800001UL;a[j].ZBits=0x3f800000UL;}
  expectedFirst=b;expectedLast=b+count;expectedDepth=0;
  for(int n=count;n>1;n>>=1)expectedDepth+=2;
  intro=insertion=tail=0;t.Initialise(name,v);
  if(intro!=1||insertion!=1||tail!=(count>16)||t.BoneMorphs.last!=b+count)bad=1;
 }
 // Mutation by push_back must refresh both source bounds before the next index.
 t.BoneMorphs.last=b;v.first=a;v.last=a+4;
 a[0].BoneNameCRC=99;a[0].XBits=0;
 a[11].BoneNameCRC=11;a[11].XBits=0;
 mutateSource=&v;replacementFirst=a+10;replacementLast=a+12;mutateOnce=1;
 expectedFirst=b;expectedLast=b+2;expectedDepth=2;intro=insertion=tail=0;
 t.Initialise(name,v);
 if(mutateOnce||t.BoneMorphs.last!=b+2||b[0].BoneNameCRC!=11||b[1].BoneNameCRC!=99)bad=1;
 // Empty input still sorts retained output and assigns the new name first.
 v.first=v.last=a;intro=insertion=tail=0;t.Initialise(name,v);
 if(intro!=1||insertion!=1||tail||t.BoneMorphs.last!=b+2)bad=1;
 #ifdef _MSC_VER
 if(sizeof(C3DSkeletalBoneMorph)!=20||sizeof(CArray)!=12||sizeof(C3DSkeletalMorphTarget)!=16)bad=1;
 #endif
 printf("00a45150_TEST %s\n",bad?"FAIL":"PASS");return bad;
}