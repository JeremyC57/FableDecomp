// C3DMesh2::GetUsedBones(CBoolArray&) -- mark root, primitive, helper-point and dummy-object bones.
// Retail 0x00A8B5F0, 194 bytes. Primitive GetUsedBones 0x00A8FCF0.
// PDB helper/dummy names and record sizes; retail base C3DMeshStats is 0x7c, so PrimitiveCount/Primitives are +0x90/+0x94.
// No shared headers exist for these classes. Array values are byte bools, not packed vector<bool> bits.
struct C3DMeshStats;
struct CMovableResourceMemoryPool {
    bool Allocate(long*, C3DMeshStats*, unsigned long, unsigned long);
    char* GetBuffer(long);
};
struct CCharString { void* PStringData; };
struct C3DBoundingSphere { float values[4]; };
struct C3DBoundingBox { float values[6]; };
template<class T> struct CPreAllocVectorMap {
    unsigned long Compare;
    T* Elements;
    unsigned long Size;
    bool Dirty;
    struct iterator {
        T* Pointer;
        iterator(T* p) : Pointer(p) {}
        T* operator->() const { return Pointer; }
        iterator& operator++() { ++Pointer; return *this; }
        bool operator!=(const iterator& p) const { return Pointer != p.Pointer; }
    };
    iterator begin() { return iterator(Elements); }
    iterator end() { return iterator(Elements + Size); }
    void SetBuffer(T* p, unsigned long count) { Elements = p; Size = count; }
};
struct CBoolArray {
    unsigned char* First;
    unsigned char* Last;
    unsigned char* End;
    unsigned long size() const { return (unsigned long)(Last - First); }
    unsigned char& operator[](unsigned long n) { return First[n]; }
};
struct C3DHelperPoint {
 float Position[3]; long BoneIndex;
 void GetUsedBones(CBoolArray& used) const { if (BoneIndex != -1) used[BoneIndex] = true; }
};
struct C3DHelperPointEntry { unsigned long Key; C3DHelperPoint Value; };
struct C3DDummyObject {
 float Transform[12]; long BoneIndex;
 void GetUsedBones(CBoolArray& used) const { if (BoneIndex != -1) used[BoneIndex] = true; }
};
struct C3DDummyObjectEntry { unsigned long Key; C3DDummyObject Value; };
struct C3DMeshStats {
    void* __vftable;
    CMovableResourceMemoryPool* MemoryPool;
    long StatsMemoryHandle;
    CCharString Name;
    C3DBoundingSphere BoundingSphere;
    C3DBoundingBox BoundingBox;
    CPreAllocVectorMap<C3DHelperPointEntry> HelperPoints;
    CPreAllocVectorMap<C3DDummyObjectEntry> DummyObjects;
    unsigned char MeshVolumes[16];
    void* MeshGenerators[3];
    char* PackedNames;
    unsigned short PackedNamesSize;
    bool AnimatedFlag;
    bool AllocateBuffers(unsigned long helpers, unsigned long dummies, unsigned long names);
};

struct C3DPrimitive2 {
    unsigned char _retailPrimitive[0xa0]; // PDB 0xac minus 3 retail vector shrinkages
    void GetUsedBones(CBoolArray&); // 0x00A8FCF0
};
struct C3DMesh2 : C3DMeshStats {
    long MemoryHandle;
    long PrimMemoryHandle;
    long BoneNameMemoryHandle;
    unsigned long MaterialCount;
    void* Materials;
    unsigned long PrimitiveCount;
    C3DPrimitive2* Primitives;
    void GetUsedBones(CBoolArray& used);
};
void C3DMesh2::GetUsedBones(CBoolArray& used)
{
    if (used.size()) used[0] = true;
    for (unsigned long n = 0; n < PrimitiveCount; ++n) Primitives[n].GetUsedBones(used);
    for (CPreAllocVectorMap<C3DHelperPointEntry>::iterator p = HelperPoints.begin(); p != HelperPoints.end(); ++p)
        p->Value.GetUsedBones(used);
    for (CPreAllocVectorMap<C3DDummyObjectEntry>::iterator p = DummyObjects.begin(); p != DummyObjects.end(); ++p)
        p->Value.GetUsedBones(used);
}


#include <cstdio>
#include <cstring>
static int primitiveCalls;
void C3DPrimitive2::GetUsedBones(CBoolArray& used) { ++primitiveCalls;used[_retailPrimitive[0]]=1; }
int main() {
 C3DMesh2 mesh;std::memset(&mesh,0,sizeof mesh);
 C3DPrimitive2 primitives[2];std::memset(primitives,0,sizeof primitives);
 primitives[0]._retailPrimitive[0]=1;primitives[1]._retailPrimitive[0]=6;
 C3DHelperPointEntry helpers[2];C3DDummyObjectEntry dummies[2];
 std::memset(helpers,0,sizeof helpers);std::memset(dummies,0,sizeof dummies);
 helpers[0].Value.BoneIndex=3;helpers[1].Value.BoneIndex=-1;
 dummies[0].Value.BoneIndex=-1;dummies[1].Value.BoneIndex=5;
 mesh.PrimitiveCount=2;mesh.Primitives=primitives;
 mesh.HelperPoints.Elements=helpers;mesh.HelperPoints.Size=2;
 mesh.DummyObjects.Elements=dummies;mesh.DummyObjects.Size=2;
 unsigned char flags[10]={0,0,0,0,0,0,0,0,0xa5,0x5a};CBoolArray used={flags,flags+8,flags+8};
 mesh.GetUsedBones(used);
 const unsigned char expected[10]={1,1,0,1,0,1,1,0,0xa5,0x5a};
 if(std::memcmp(flags,expected,sizeof flags) || primitiveCalls!=2) return 1;
 mesh.PrimitiveCount=0;mesh.HelperPoints.Size=0;mesh.DummyObjects.Size=0;
 std::memset(flags,0,8);used.Last=flags;mesh.GetUsedBones(used);
 for(int i=0;i<8;++i)if(flags[i])return 2;
 used.Last=flags+8;mesh.GetUsedBones(used);
 if(flags[0]!=1 || primitiveCalls!=2)return 3;
 for(int j=1;j<8;++j)if(flags[j])return 4;
 std::printf("00a8b5f0_TEST PASS\n");return 0;
}
