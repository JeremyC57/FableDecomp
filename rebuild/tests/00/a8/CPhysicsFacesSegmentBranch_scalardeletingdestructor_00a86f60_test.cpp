#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#endif
// Behaviour test for 00a86f60: deleting a branch through the interface pointer runs the scalar deleting destructor: children deleted, object freed.
static int g_childrenDeleted = 0;
static int g_branchesFreed = 0;
struct CPlane { float m_normal[3]; float m_distance; };
struct CIPhysicsFacesSegmentBase {
    virtual unsigned char HasChildren() = 0;
    virtual long Sizeof() = 0;
    virtual long GetNoFaces() = 0;
    virtual ~CIPhysicsFacesSegmentBase() {}
};
struct CLeaf : CIPhysicsFacesSegmentBase {
    virtual unsigned char HasChildren() { return 0; }
    virtual long Sizeof() { return sizeof(CLeaf); }
    virtual long GetNoFaces() { return 1; }
    virtual ~CLeaf() { ++g_childrenDeleted; }
};
struct CPhysicsFacesSegmentBranch : CIPhysicsFacesSegmentBase {
    CPlane ChildDividingPlane;
    CIPhysicsFacesSegmentBase* PositiveChild;
    CIPhysicsFacesSegmentBase* NegativeChild;
    virtual unsigned char HasChildren() { return 1; }
    virtual long Sizeof() { return sizeof(CPhysicsFacesSegmentBranch); }
    virtual long GetNoFaces() { return PositiveChild->GetNoFaces() + NegativeChild->GetNoFaces(); }
    virtual ~CPhysicsFacesSegmentBranch();
    static void operator delete(void* p) { ++g_branchesFreed; ::operator delete(p); }
};
CPhysicsFacesSegmentBranch::~CPhysicsFacesSegmentBranch()
{
    delete PositiveChild;
    PositiveChild = 0;
    delete NegativeChild;
    NegativeChild = 0;
}
int main()
{
    CPhysicsFacesSegmentBranch* inner = new CPhysicsFacesSegmentBranch;
    inner->PositiveChild = new CLeaf;
    inner->NegativeChild = new CLeaf;
    CPhysicsFacesSegmentBranch* outer = new CPhysicsFacesSegmentBranch;
    outer->PositiveChild = inner;
    outer->NegativeChild = new CLeaf;
    if (outer->GetNoFaces() != 3) { std::printf("00a86f60_TEST FAIL\n"); return 1; }
    CIPhysicsFacesSegmentBase* root = outer;
    delete root;
    if (g_childrenDeleted != 3 || g_branchesFreed != 2) { std::printf("00a86f60_TEST FAIL\n"); return 1; }
    std::printf("00a86f60_TEST PASS\n");
    return 0;
}