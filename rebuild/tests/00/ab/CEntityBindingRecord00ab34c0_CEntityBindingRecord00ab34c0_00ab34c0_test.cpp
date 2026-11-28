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
C3DAnimationBuilder::CBoneInfo::~CBoneInfo()
{
}


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

int main() {
    for (unsigned n = 0; n <= 32; ++n) {
        CCountedPointer<Bone> kept(new Bone);
        {
            CCountedPointer<Bone> parent(new Bone);
            parent->AnimSource = kept;
            for (unsigned i = 0; i < n; ++i) {
                CCountedPointer<Bone> child(new Bone);
                parent->Children.push_back(child);
            }
            parent->Children.push_back(kept);
            check(kept.GetReferenceCount() == 3, "source and child references");
            check(stringCount == (int)n+2, "live nested objects");
        }
        check(kept.GetReferenceCount() == 1 && stringCount == 1, "release children and animation source");
    }
    check(stringCount == 0, "all names destroyed");
    puts("00ab34c0_TEST PASS"); return 0;
}
