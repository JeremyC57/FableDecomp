#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FABLE_PORTABLE
#ifndef _MSC_VER
#include <strings.h>
#define _stricmp strcasecmp
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __int64 long long
#endif
#define FABLE_RETAIL_C3DANIMATIONBUILDER_BASE
#define FABLE_RETAIL_ANIMATION_BONE_TREE_PASS30
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "engine/C3DAnimationBuilder.h"
#include "engine/C3DAnimationSequence.h"
#include "fable_assert.h"

static void check(bool condition, const char* why) {
    if (!condition) { fprintf(stderr, "%s\n", why); exit(1); }
}

class CSimpleMemoryPool {};
class C3DMesh2 {};
namespace N3DAnimationBuilder { class COutputAnimation {}; }
class C3DAnimFileSequenceChunk {
public:
    char* name;
    long parent;
    const CCharString GetBoneName() const { return CCharString(name ? name : ""); }
    long GetParentIndex() const { return parent; }
};

bool C3DAnimationSequenceData::InitialiseFromFileData(const C3DAnimFileSequenceChunk& chunk, CSimpleMemoryPool&) {
    BoneName = chunk.name; return true;
}
bool C3DAnimationSequence::InitialiseFromFileData(const C3DAnimFileSequenceChunk& chunk, C3DAnimationSequenceData* data, CSimpleMemoryPool&) {
    Data = data; ParentIndex = (short)chunk.parent; Shared = false; return true;
}

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
C3DAnimationBuilder::C3DAnimationBuilder() {}
typedef C3DAnimationBuilder::CBoneInfo Bone;
static int destroyed;
C3DAnimationBuilder::CBoneInfo::~CBoneInfo() { ++destroyed; }

struct StringRecord { const CCharString* object; const char* text; };
static StringRecord strings[256];
static int stringCount;
static int findString(const CCharString* value) {
    for (int i=0; i<stringCount; ++i) if (strings[i].object == value) return i;
    check(false, "unregistered string"); return 0;
}
static void newString(const CCharString* value, const char* text) {
    check(stringCount < 256, "string fixture capacity");
    strings[stringCount].object = value; strings[stringCount++].text = text;
}
CCharString::CCharString() : PStringData(0) { newString(this, ""); }
CCharString::CCharString(const char* text, long) : PStringData(0) { newString(this,text); }
CCharString::CCharString(const CCharString& value) : PStringData(0) { newString(this, strings[findString(&value)].text); }
const CCharString& CCharString::operator=(const CCharString& value) {
    strings[findString(this)].text = strings[findString(&value)].text; return *this;
}
CCharString::~CCharString() { int i=findString(this); strings[i]=strings[--stringCount]; }

CCountedPointer<C3DAnimationBuilder::CBoneInfo> C3DAnimationBuilder::BuildBoneInfoTree(
    const C3DAnimationSequence* anim, unsigned long size)
{
    CArray<CCountedPointer<CBoneInfo> > root_bones;
    CArray<CCountedPointer<CBoneInfo> > bone_list;
    for (unsigned long c_bone = 0; c_bone < size; ++c_bone) {
        CCountedPointer<CBoneInfo> bone_info(new CBoneInfo);
        const C3DAnimationSequence& bone_anim = anim[c_bone];
        bone_info->Name = bone_anim.GetBoneName();
        bone_info->Index = c_bone;
        bone_info->OriginalNoChildren = 0;
        long parent_index = bone_anim.GetParentIndex();
        if (parent_index != -1) {
            FABLE_RETAIL_ASSERT(parent_index < bone_list.size());
            CBoneInfo& parent = *bone_list[parent_index];
            parent.Children.push_back(bone_info);
            ++parent.OriginalNoChildren;
        } else {
            root_bones.push_back(bone_info);
        }
        bone_list.push_back(bone_info);
    }
    FABLE_RETAIL_ASSERT(root_bones.size() == 1);
    return root_bones[0];
}

static char names[42][12];
static long parents[42];
static const char* expectedNames[42];
static bool seen[42];
static unsigned long expectedSize;
static unsigned long visits;
static void verify(const Bone& b, unsigned long index) {
    check(index < expectedSize && !seen[index], "duplicate bone or invalid index");
    seen[index] = true; ++visits;
    check(b.Index == index, "bone index");
    check(!b.ExtensionChild && !b.ExtensionParent && b.AnimSource.IsNull(), "new bone state");
    check(strcmp(strings[findString(&b.Name)].text, expectedNames[index]) == 0, "bone name");
    unsigned long child = 0;
    for (unsigned long i=index+1; i<expectedSize; ++i) if (parents[i] == (long)index) {
        check(child < b.Children.size(), "missing child");
        verify(*b.Children[child++], i);
    }
    check(b.OriginalNoChildren == child && b.Children.size() == child, "child count");
}
unsigned long C3DAnimationBuilder::BuildMeshAnimation(N3DAnimationBuilder::COutputAnimation&,
        const C3DAnimationSequence* anim, unsigned long size, const C3DMesh2&, bool, CSimpleMemoryPool&) {
    expectedSize = size; visits = 0;
    memset(seen, 0, sizeof(seen));
    {
        CCountedPointer<CBoneInfo> root = BuildBoneInfoTree(anim, size);
        check(!root.IsNull(), "missing root");
        verify(*root, 0);
        check(visits == expectedSize, "unreachable descendant");
    }
    return visits;
}
int main() {
    C3DAnimationBuilder builder; N3DAnimationBuilder::COutputAnimation out;
    C3DMesh2 mesh; CSimpleMemoryPool pool;
    for (unsigned long size=1; size<=42; ++size)
        for (unsigned shape=0; shape<5; ++shape) {
            C3DAnimationSequence animation[42];
            C3DAnimationSequenceData data[42];
            for (unsigned long i=0; i<size; ++i) {
                sprintf(names[i], "bone%lu", i);
                parents[i] = !i ? -1 : shape==0 ? 0 : shape==1 ? (long)i-1 :
                    shape==2 ? (long)(i-1)/2 : shape==3 ? (long)(i-1)/4 : (long)((i*19+7)%i);
                C3DAnimFileSequenceChunk chunk;
                chunk.name = (i+shape)%3 == 0 ? 0 : ((i+shape)%3 == 1 ? (char*)"" : names[i]);
                expectedNames[i] = chunk.name ? chunk.name : "";
                chunk.parent = parents[i];
                data[i].InitialiseFromFileData(chunk, pool);
                animation[i].InitialiseFromFileData(chunk, &data[i], pool);
            }
            int before = destroyed;
            check(builder.BuildMeshAnimation(out, animation, size, mesh, false, pool) == size, "returned count");
            check(destroyed-before == (int)size && stringCount == 0, "bone/string lifetime");
        }
    puts("00ab1990_TEST PASS"); return 0;
}