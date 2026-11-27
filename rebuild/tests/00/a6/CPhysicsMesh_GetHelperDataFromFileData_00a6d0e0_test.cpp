#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
static void require(bool condition) { if (!condition) std::exit(1); }
template<class T> class CCountedPointer {
    T* value;
public:
    CCountedPointer(T* p = 0) : value(p) {}
    T* Get() const { return value; }
    T* operator->() const { require(value != 0); return value; }
    T& operator*() const { require(value != 0); return *value; }
};
class CCharString {
public:
    std::string value;
    CCharString(const char* s="") : value(s) {}
    bool StartsWith(const CCharString& s) const { return value.compare(0,s.value.size(),s.value)==0; }
    unsigned long GetLength() const { return (unsigned long)value.size(); }
    CCharString GetSubString(unsigned long a,unsigned long b) const {
        require(a==11 && b==value.size()); return CCharString(value.substr(a,b).c_str());
    }
    bool operator<(const CCharString& b) const { return value<b.value; }
    bool operator==(const CCharString& b) const { return value==b.value; }
};
template<class K,class T> class TestMap {
    typedef std::pair<K,T> Entry;
    static bool ByKey(const Entry& a,const Entry& b) { return a.first < b.first; }
public:
    std::vector<Entry> entries; unsigned int sorts; bool dirty;
    TestMap() : sorts(0),dirty(false) {}
    void Insert(const K& k,const T& v) { entries.push_back(Entry(k,v));dirty=true; }
    void Sort() { std::stable_sort(entries.begin(),entries.end(),ByKey);++sorts;dirty=false; }
    void Update() { if(dirty) Sort(); }
};
template<class T> struct TestChunks {
    std::vector<T> nodes; mutable unsigned int first,next;
    TestChunks(): first(0),next(0) {}
    CCountedPointer<const T> First() const { ++first;return nodes.empty()?0:&nodes[0]; }
    CCountedPointer<const T> Next(const CCountedPointer<const T>& v) const {
        ++next;require(!nodes.empty() && v.Get()>=&nodes[0] && v.Get()<=&nodes.back());
        return v.Get()==&nodes.back()?0:v.Get()+1;
    }
};
struct C3DVector { float x,y,z; };
struct CMatrix3x4 { float e[12]; };
struct C3DMeshFileHelperPointChunk {
    C3DVector position; long bone; CCharString name;
    C3DVector GetPosition() const {return position;}
    long GetBoneIndex() const {return bone;}
    const CCharString& GetName() const {return name;}
};
struct C3DMeshFileDummyObjectChunk {
    CMatrix3x4 transform; long bone; CCharString name;
    CMatrix3x4 GetTransform() const {return transform;}
    long GetBoneIndex() const {return bone;}
    const CCharString& GetName() const {return name;}
};
struct C3DHelperPoint {
    C3DVector Position;unsigned long BoneIndex;
    C3DHelperPoint(const C3DVector& p,unsigned long b):Position(p),BoneIndex(b){}
};
struct C3DDummyObject {
    CMatrix3x4 Transform;unsigned long BoneIndex;
    C3DDummyObject(const CMatrix3x4& m,unsigned long b):Transform(m),BoneIndex(b){}
};
struct C3DMeshFileHelpersChunk {
    TestChunks<C3DMeshFileHelperPointChunk> points;
    TestChunks<C3DMeshFileDummyObjectChunk> objects;
    CCountedPointer<const C3DMeshFileHelperPointChunk> FindFirstPoint()const{return points.First();}
    CCountedPointer<const C3DMeshFileHelperPointChunk> FindNextPoint(const CCountedPointer<const C3DMeshFileHelperPointChunk>& v)const{return points.Next(v);}
    CCountedPointer<const C3DMeshFileDummyObjectChunk> FindFirstDummy()const{return objects.First();}
    CCountedPointer<const C3DMeshFileDummyObjectChunk> FindNextDummy(const CCountedPointer<const C3DMeshFileDummyObjectChunk>& v)const{return objects.Next(v);}
};
class C3DMeshFileRootChunk {
public:
    C3DMeshFileHelpersChunk data; bool present; mutable unsigned int checks,gets;
    C3DMeshFileRootChunk():present(false),checks(0),gets(0){}
    bool HasHelpers() const {++checks;return present;}
    CCountedPointer<const C3DMeshFileHelpersChunk> GetHelpers() const {require(present);++gets;return &data;}
};
class C3DMeshFile {
public:
    C3DMeshFileRootChunk root;mutable unsigned int reads;
    C3DMeshFile():reads(0){}
    CCountedPointer<const C3DMeshFileRootChunk> GetRoot() const {++reads;return &root;}
};
class CPhysicsMesh {
public:
    TestMap<CCharString,C3DHelperPoint> HelperPoints;
    TestMap<CCharString,C3DDummyObject> DummyObjects;
    void GetHelperDataFromFileData(const C3DMeshFile&);
};
void CPhysicsMesh::GetHelperDataFromFileData(const C3DMeshFile& file_data)
{
    CCountedPointer<const C3DMeshFileRootChunk> root = file_data.GetRoot();
    if (root->HasHelpers()) {
        CCountedPointer<const C3DMeshFileHelpersChunk> helpers = root->GetHelpers();
        CCountedPointer<const C3DMeshFileHelperPointChunk> point_data = helpers->FindFirstPoint();
        while (point_data.Get()) {
            C3DHelperPoint helper_point(point_data->GetPosition(), point_data->GetBoneIndex());
            HelperPoints.Insert(point_data->GetName(), helper_point);
            point_data = helpers->FindNextPoint(point_data);
        }
        CCountedPointer<const C3DMeshFileDummyObjectChunk> dummy_data = helpers->FindFirstDummy();
        while (dummy_data.Get()) {
            C3DDummyObject dummy_object(dummy_data->GetTransform(), dummy_data->GetBoneIndex());
            DummyObjects.Insert(dummy_data->GetName(), dummy_object);
            dummy_data = helpers->FindNextDummy(dummy_data);
        }
    }
    HelperPoints.Sort();
    DummyObjects.Sort();
}
int main() {
    for(unsigned int key=0;key<8192;++key) {
        unsigned int np=(key>>1)&7,nd=(key>>4)&7;C3DMeshFile file;CPhysicsMesh physics;
        file.root.present=(key&1)!=0;
        C3DMeshFileHelpersChunk& entries=file.root.data;
        for(unsigned int i=0;i<np;++i) {
            C3DMeshFileHelperPointChunk node;char name[8];std::sprintf(name,"P%u",np-i);
            node.name=CCharString(name);node.position.x=float(key+i);node.position.y=-float(i);node.position.z=float(2*i+1);
            node.bone=(key&128)?-1:long(key+i*3);entries.points.nodes.push_back(node);
        }
        for(unsigned int i=0;i<nd;++i) {
            C3DMeshFileDummyObjectChunk node;char name[8];std::sprintf(name,"D%u",nd-i);
            node.name=CCharString(name);node.bone=(key&256)?-1:long(i+4);
            for(int k=0;k<12;++k)node.transform.e[k]=float(31*i+k);entries.objects.nodes.push_back(node);
        }
        physics.GetHelperDataFromFileData(file);
        require(file.reads==1&&file.root.checks==1&&file.root.gets==unsigned(file.root.present));
        require(entries.points.first==file.root.gets&&entries.objects.first==file.root.gets);
        if(!file.root.present){np=0;nd=0;}
        require(entries.points.next==np&&entries.objects.next==nd);
        require(physics.HelperPoints.sorts==1&&physics.DummyObjects.sorts==1);
        require(physics.HelperPoints.entries.size()==np&&physics.DummyObjects.entries.size()==nd);
        for(unsigned int i=0;i<np;++i) {
            const C3DMeshFileHelperPointChunk& n=entries.points.nodes[np-i-1];
            const C3DHelperPoint& q=physics.HelperPoints.entries[i].second;
            require(physics.HelperPoints.entries[i].first==n.name&&q.BoneIndex==(unsigned long)n.bone);
            require(q.Position.x==n.position.x&&q.Position.y==n.position.y&&q.Position.z==n.position.z);
        }
        for(unsigned int i=0;i<nd;++i) {
            const C3DMeshFileDummyObjectChunk& n=entries.objects.nodes[nd-i-1];
            const C3DDummyObject& q=physics.DummyObjects.entries[i].second;
            require(physics.DummyObjects.entries[i].first==n.name&&q.BoneIndex==(unsigned long)n.bone);
            for(int k=0;k<12;++k)require(q.Transform.e[k]==n.transform.e[k]);
        }
    }
    std::puts("00a6d0e0_TEST PASS");return 0;
}