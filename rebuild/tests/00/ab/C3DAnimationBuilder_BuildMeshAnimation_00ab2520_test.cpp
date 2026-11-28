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
#include <math.h>
// C3DAnimationBuilder::BuildMeshAnimation, retail 00AB2520.
// Leaf: ?BuildMeshAnimation@C3DAnimationBuilder@@QAEKAAVCOutputAnimation@N3DAnimationBuilder@@PBVC3DAnimationSequence@@KABVC3DMesh2@@_NAAVCSimpleMemoryPool@@@Z
// Flags: /Gr
#include "fable_compiler.h"
#define FABLE_RETAIL_C3DANIMATIONBUILDER_BASE
#define FABLE_RETAIL_C3DBONE2_PEEK
#define FABLE_RETAIL_C3DMESH2_BONES
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_MESH_ANIMATION_TREE_PASS30
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_ANIMATION_BUILDER_LONG_DISTANCE_PASS30
#define FABLE_RETAIL_CMATRIX3X4_SETINVERSE_DONOR_PASS26
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_PASS23
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CARRAY_INDEX_ASSERT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_CMATRIX3X4_SETINVERSE_DETERMINANT
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "engine/C3DAnimationBuilder.h"
#include "engine/N3DAnimationBuilder_COutputAnimation.h"

static void check(bool condition, const char* why) {
    if (!condition) { fprintf(stderr, "%s\n", why); exit(1); }
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

class CSimpleMemoryPool {};
class C3DAnimationSequence {};
typedef C3DAnimationBuilder::CBoneInfo Bone;
typedef N3DAnimationBuilder::CSequenceInfo Sequence;
typedef N3DAnimationBuilder::COutputAnimation Output;

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
C3DAnimationBuilder::C3DAnimationBuilder() {}
static int released;
C3DAnimationBuilder::CBoneInfo::~CBoneInfo() { ++released; }

static const C3DAnimationSequence* expectedSource;
static unsigned long expectedSize;
static unsigned long sequenceCalls, assignCalls, visited, meshCount, matchMask, extensionMask;
static char names[5][6] = {"one","two","three","four","five"};
CCountedPointer<Bone> C3DAnimationBuilder::BuildBoneInfoTree(const C3DAnimationSequence* source, unsigned long size)
{
    ++sequenceCalls;
    check(source == expectedSource && size == expectedSize, "source arguments");
    if (size == 0) return 0;
    CCountedPointer<Bone> value(new Bone);
    value->Index = 987;
    return value;
}
static unsigned long assign(Bone& dest, const CCountedPointer<Bone>& source)
{
    check(dest.Index < meshCount, "mesh index in range");
    check(!(visited & (1ul << dest.Index)), "one visit per bone");
    visited |= 1ul << dest.Index;
    check(strcmp(strings[findString(&dest.Name)].text,names[dest.Index]) == 0, "packed bone names");
    check(dest.OriginalNoChildren == dest.Children.size(), "child count copied from mesh");
    unsigned long match = (matchMask >> dest.Index) & 1ul;
    if (match) dest.AnimSource = source;
    dest.ExtensionChild = (extensionMask & (1ul << dest.Index)) != 0;
    for (unsigned int i = 0; i < dest.Children.size(); ++i)
        match += assign(*dest.Children[i], source);
    return match;
}
unsigned long C3DAnimationBuilder::AssignAnimation(Bone& dest, const CCountedPointer<Bone>& source, bool root)
{
    ++assignCalls;
    check(root && source.Get() && dest.Index == 0, "root assignment");
    return 700 + assign(dest,source);
}

unsigned long C3DAnimationBuilder::BuildMeshAnimation(N3DAnimationBuilder::COutputAnimation& dest_anim,
    const C3DAnimationSequence* src_anim, unsigned long src_size, const C3DMesh2& mesh, bool delta, CSimpleMemoryPool& alloc)
{
    dest_anim.GeneratedBoneCount = 0;
    CArray<CCountedPointer<CBoneInfo> > mesh_bone_list;
    CCountedPointer<CBoneInfo> mesh_tree = BuildBoneInfoTree(mesh, mesh_bone_list);
    CCountedPointer<CBoneInfo> anim_tree = BuildBoneInfoTree(src_anim, src_size);
    if (mesh_tree.IsNull() || anim_tree.IsNull())
        return 0;
    unsigned long size = mesh_bone_list.size();
    unsigned long matches = AssignAnimation(*mesh_tree, anim_tree, true);
    dest_anim.Sequences.resize(size);
    for (unsigned long c_bone = 0; c_bone < size; ++c_bone) {
        const CBoneInfo& mesh_bone = *mesh_bone_list[c_bone];
        const C3DBone2& bone = *mesh.GetBone(c_bone);
        N3DAnimationBuilder::CSequenceInfo& output_seq = dest_anim.Sequences[c_bone];
        if (mesh_bone.AnimSource.Get()) {
            output_seq.OriginalIndex = mesh_bone.AnimSource->Index;
        } else {
            if (mesh_bone.ExtensionChild || delta) {
                output_seq.GeneratedTransform.SetIdentity();
            } else {
                output_seq.GeneratedTransform.SetInverse(bone.PeekLocalizationMatrix());
                long parent_index = bone.GetParent();
                if (parent_index != -1)
                    output_seq.GeneratedTransform.PostMultiply(mesh.GetBone(parent_index)->PeekLocalizationMatrix());
            }
            output_seq.OriginalIndex = -1;
            ++dest_anim.GeneratedBoneCount;
        }
    }
    return matches;
}

typedef float CMatrix3x4::*Member;
static Member field[4][3] = {
    {&CMatrix3x4::E11,&CMatrix3x4::E12,&CMatrix3x4::E13},
    {&CMatrix3x4::E21,&CMatrix3x4::E22,&CMatrix3x4::E23},
    {&CMatrix3x4::E31,&CMatrix3x4::E32,&CMatrix3x4::E33},
    {&CMatrix3x4::E41,&CMatrix3x4::E42,&CMatrix3x4::E43}
};
struct Matrix { double m[4][4]; };
static Matrix identity() {
    Matrix a = {0};
    for (int i=0; i<4; ++i) a.m[i][i]=1;
    return a;
}
static Matrix read(const CMatrix3x4& value) {
    Matrix a=identity();
    for(int r=0;r<4;++r)for(int c=0;c<3;++c) a.m[r][c]=value.*field[r][c];
    return a;
}
static Matrix inverse(Matrix a) {
    Matrix b=identity();
    for(int col=0;col<4;++col) {
        int pivot=col;
        for(int r=col+1;r<4;++r) if(fabs(a.m[r][col])>fabs(a.m[pivot][col])) pivot=r;
        check(fabs(a.m[pivot][col])>0.001, "fixture matrix invertible");
        for(int j=0;j<4;++j) {
            double t=a.m[pivot][j]; a.m[pivot][j]=a.m[col][j]; a.m[col][j]=t;
            t=b.m[pivot][j]; b.m[pivot][j]=b.m[col][j]; b.m[col][j]=t;
        }
        double scale=a.m[col][col];
        for(int j=0;j<4;++j) { a.m[col][j]/=scale; b.m[col][j]/=scale; }
        for(int r=0;r<4;++r) if(r!=col) {
            double k=a.m[r][col];
            for(int j=0;j<4;++j){ a.m[r][j]-=k*a.m[col][j]; b.m[r][j]-=k*b.m[col][j];}
        }
    }
    return b;
}
static Matrix times(const Matrix& a, const Matrix& b) {
    Matrix c={0};
    for(int r=0;r<4;++r) for(int col=0;col<4;++col) for(int k=0;k<4;++k) c.m[r][col]+=a.m[r][k]*b.m[k][col];
    return c;
}
static void same(const CMatrix3x4& output,const Matrix& expected) {
    for(int r=0;r<4;++r) for(int c=0;c<3;++c)
        check(fabs((output.*field[r][c])-expected.m[r][c])<0.0001,"affine matrix result");
}
static void scenario(unsigned long count, unsigned long sourceCount, unsigned long matched,
                     unsigned long extensions, bool delta, bool shrink, int tree)
{
    meshCount=count;matchMask=matched;extensionMask=extensions;
    sequenceCalls=assignCalls=visited=0;
    C3DMesh2 mesh; C3DBone2 bones[5]; unsigned short starts[5];
    mesh.Bones=bones; mesh.BoneCount=count; mesh.BoneNameIndices=starts; mesh.BoneNameData=&names[0][0];
    for(unsigned long i=0;i<count;++i) {
        starts[i]=(unsigned short)(i*6);
        bones[i].Parent=i ? (tree ? (long)i-1 : 0) : -1;
        bones[i].OriginalNoChildren=0;
        if(i) ++bones[bones[i].Parent].OriginalNoChildren;
        CMatrix3x4& m=bones[i].LocalizationMatrix;
        m.E11=2.0f+i; m.E12=0.75f; m.E13=-0.25f;
        m.E21=1.0f; m.E22=-2.0f; m.E23=0.5f;
        m.E31=1.0f; m.E32=0.25f; m.E33=1.5f+i;
        m.E41=1.25f*i; m.E42=-2.5f*i; m.E43=3.0f+i;
    }
    Output output; output.GeneratedBoneCount=321;
    Sequence initial; initial.OriginalIndex=1234;
    for(int r=0;r<4;++r) for(int c=0;c<3;++c) initial.GeneratedTransform.*field[r][c]=float(10*r+c+11);
    // A matched entry retains its old transform. Keep populated input whenever matches are requested.
    unsigned long prior=shrink?count+2:(matched?count:0);
    output.Sequences.resize((unsigned int)prior,initial);
    C3DAnimationSequence anim; CSimpleMemoryPool pool; C3DAnimationBuilder builder;
    expectedSize=sourceCount;expectedSource=sourceCount?&anim:0;int previousReleases=released;
    unsigned long value=builder.BuildMeshAnimation(output,expectedSource,expectedSize,mesh,delta,pool);
    check(sequenceCalls==1,"both trees built even for empty mesh");
    check(released-previousReleases==(int)(count+(sourceCount?1:0)),"all temporary trees released");
    if(!count || !sourceCount) {
        check(value==0 && assignCalls==0 && output.GeneratedBoneCount==0,"early return");
        check(output.Sequences.size()==prior,"early return preserves sequence list");
        for(unsigned long i=0;i<prior;++i) {check(output.Sequences[i].OriginalIndex==1234,"early index");same(output.Sequences[i].GeneratedTransform,read(initial.GeneratedTransform));}
        return;
    }
    check(assignCalls==1 && visited==((1ul<<count)-1) && output.Sequences.size()==count,"all mesh sequences assigned");
    unsigned long generated=0;
    for(unsigned long i=0;i<count;++i) {
        const Sequence& seq=output.Sequences[i];
        if(matched&(1ul<<i)) {
            check(seq.OriginalIndex==987,"source index reused");same(seq.GeneratedTransform,read(initial.GeneratedTransform));
        } else {
            ++generated;check(seq.OriginalIndex==-1,"generated marker");
            Matrix target=identity();
            if(!delta && !(extensions&(1ul<<i))) {
                target=inverse(read(bones[i].LocalizationMatrix));
                if(bones[i].Parent!=-1) target=times(target,read(bones[bones[i].Parent].LocalizationMatrix));
            }
            same(seq.GeneratedTransform,target);
        }
    }
    check(output.GeneratedBoneCount==generated && value==700+count-generated,"return and generated count");
}
int main() {
    for(int tree=0;tree<2;++tree) for(int delta=0;delta<2;++delta) for(int shrink=0;shrink<2;++shrink) {
        scenario(0,0,0,0,delta!=0,shrink!=0,tree);
        scenario(0,3,0,0,delta!=0,shrink!=0,tree);
        scenario(5,0,0,0,delta!=0,shrink!=0,tree);
        for(unsigned long mask=0;mask<32;++mask) {
            scenario(5,3,mask,0,delta!=0,shrink!=0,tree);
            scenario(5,3,mask,21,delta!=0,shrink!=0,tree);
        }
    }
    check(stringCount==0,"all string temporaries released");
    puts("00ab2520_TEST PASS");return 0;
}