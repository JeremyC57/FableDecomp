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
// 0088b610 std::vector<C3DSkeletalBoneMorph>::push_back
// Flags: /Gr
// Leaf: ?push_back@?$vector@VC3DSkeletalBoneMorph@@V?$allocator@VC3DSkeletalBoneMorph@@@std@@@std@@QAEXABVC3DSkeletalBoneMorph@@@Z
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_SKELETAL_MORPH_FILE_PASS30
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "fable_string.h"
#include "engine/C3DSkeletalMorph.h"

template void std::vector<C3DSkeletalBoneMorph>::push_back(const C3DSkeletalBoneMorph&);

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

int main()
{
    {
        std::vector<C3DSkeletalBoneMorph> records;
        for (int n = 0; n < 32; ++n) {
            char text[10];
            sprintf(text,"%d",n);
            CCharString name(text);
            C3DVector scale;
            scale.X = (float)n; scale.Y = -2.0f; scale.Z = 0.5f;
            if (n && n % 3 == 1) records.push_back(records[0]);
            else records.push_back(C3DSkeletalBoneMorph(name,scale));
            require(records.size() == (unsigned int)n + 1);
            for (int i = 0; i <= n; ++i) {
                int expected = i && i % 3 == 1 ? 0 : i;
                require(atoi(get(&records[i].GetName())) == expected);
                require(records[i].GetScaling().X == (float)expected &&
                        records[i].GetScaling().Y == -2.0f && records[i].GetScaling().Z == 0.5f);
            }
        }
    }
    for (int i=0; i<128; ++i) require(texts[i].owner==0);
    if (failed) return 1;
    puts("0088b610_TEST PASS");
    return 0;
}
