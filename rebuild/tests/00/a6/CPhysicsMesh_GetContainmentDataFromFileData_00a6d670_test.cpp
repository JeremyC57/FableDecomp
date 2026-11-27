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
struct C3DMeshFileConvexVolumeChunk {CCharString name; mutable unsigned int checks;C3DMeshFileConvexVolumeChunk(): checks(0) {} void CheckDebug() const {++checks;}};
struct CMovableResourceMemoryPool {};
static CMovableResourceMemoryPool* expected_pool;
struct C3DMeshVolume {
    const C3DMeshFileConvexVolumeChunk* data;
    C3DMeshVolume(const C3DMeshFileConvexVolumeChunk& d,CMovableResourceMemoryPool* pool):data(&d){require(pool==expected_pool);}
    const C3DMeshFileConvexVolumeChunk* GetConvexVolume()const{return data;}
    CCharString GetName()const{return data->name;}
};
struct C3DMeshFileHelpersChunk {
    TestChunks<C3DMeshFileConvexVolumeChunk> volumes;
    CCountedPointer<const C3DMeshFileConvexVolumeChunk> FindFirstVolumeW() const {return volumes.First();}
    CCountedPointer<const C3DMeshFileConvexVolumeChunk> FindNextVolumeW(const CCountedPointer<const C3DMeshFileConvexVolumeChunk>& n) const {return volumes.Next(n);}
};
static unsigned int checks,bad_levels;
static void CheckLevel(bool value){++checks;if(!value)++bad_levels;}
#define FABLE_RETAIL_ASSERT(expression) CheckLevel(expression)
long GFCharStringToInt(const CCharString& x) {return std::strtol(x.value.c_str(),0,10);}
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
    TestMap<long,C3DMeshVolume> NavigationContainmentVolumes;
    TestMap<CCharString,C3DMeshVolume> ContainmentVolumes;
    CMovableResourceMemoryPool* MemoryPool;
    void GetContainmentDataFromFileData(const C3DMeshFile&);
};
void CPhysicsMesh::GetContainmentDataFromFileData(const C3DMeshFile& file_data)
{
    CCharString starts_with_text("NAV_VOLUME_");
    CCountedPointer<const C3DMeshFileRootChunk> root = file_data.GetRoot();
    if (root->HasHelpers()) {
        CCountedPointer<const C3DMeshFileHelpersChunk> helpers = root->GetHelpers();
        CCountedPointer<const C3DMeshFileConvexVolumeChunk> volume_data = helpers->FindFirstVolumeW();
        while (volume_data.Get()) {
            C3DMeshVolume mesh_volume(*volume_data, MemoryPool);
            mesh_volume.GetConvexVolume()->CheckDebug();
            CCharString volume_name = mesh_volume.GetName();
            if (volume_name.StartsWith(starts_with_text)) {
                CCharString level_number_string = volume_name.GetSubString(starts_with_text.GetLength(),volume_name.GetLength());
                long level_number = GFCharStringToInt(level_number_string);
                FABLE_RETAIL_ASSERT(level_number > 0 && level_number <= 10);
                NavigationContainmentVolumes.Insert(level_number - 1,mesh_volume);
            } else {
                ContainmentVolumes.Insert(volume_name,mesh_volume);
            }
            volume_data = helpers->FindNextVolumeW(volume_data);
        }
        NavigationContainmentVolumes.Update();
        ContainmentVolumes.Update();
    }
}
int main() {
    const char* names[]={"NAV_VOLUME_1","NAV_VOLUME_10","NAV_VOLUME_11","NAV_VOLUME_0","NAV_VOLUME_-2","NAV_VOLUME_","nav_volume_3","","box","NAV_VOLUME_5","NAV_VOLUME_2","NAV_VOLUME_09"};
    const int levels[]={1,10,11,0,-2,0,999,999,999,5,2,9};
    for(unsigned int k=0;k<8192;++k) {
        CPhysicsMesh physics;C3DMeshFile file;CMovableResourceMemoryPool pool;expected_pool=physics.MemoryPool=(k&8)?&pool:0;
        file.root.present=(k&1)!=0;unsigned int n=(k>>4)%8,nav=0,named=0,invalid=0;
        physics.NavigationContainmentVolumes.dirty=(k&2)!=0;physics.ContainmentVolumes.dirty=(k&4)!=0;
        bool need_nav=physics.NavigationContainmentVolumes.dirty,need_named=physics.ContainmentVolumes.dirty;
        TestChunks<C3DMeshFileConvexVolumeChunk>& v=file.root.data.volumes;
        for(unsigned int i=0;i<n;++i){C3DMeshFileConvexVolumeChunk item;item.name=CCharString(names[(k/11+i*5)%12]);v.nodes.push_back(item);}
        checks=bad_levels=0;physics.GetContainmentDataFromFileData(file);
        for(unsigned int i=0;i<n;++i){
            unsigned int id=(k/11+i*5)%12;require(v.nodes[i].checks==unsigned(file.root.present));
            if(!file.root.present)continue;unsigned int found=0;
            if(levels[id]!=999){
                ++nav;if(levels[id]<1||levels[id]>10)++invalid;
                for(unsigned int j=0;j<physics.NavigationContainmentVolumes.entries.size();++j)
                    if(physics.NavigationContainmentVolumes.entries[j].second.data==&v.nodes[i]){++found;require(physics.NavigationContainmentVolumes.entries[j].first==levels[id]-1);}
            } else {
                ++named;
                for(unsigned int j=0;j<physics.ContainmentVolumes.entries.size();++j)
                    if(physics.ContainmentVolumes.entries[j].second.data==&v.nodes[i]){++found;require(physics.ContainmentVolumes.entries[j].first==v.nodes[i].name);}
            }
            require(found==1);
        }
        require(file.reads==1&&file.root.checks==1&&file.root.gets==unsigned(file.root.present));
        require(v.first==file.root.gets&&v.next==(file.root.present?n:0));
        require(checks==nav&&bad_levels==invalid);
        require(physics.NavigationContainmentVolumes.entries.size()==nav&&physics.ContainmentVolumes.entries.size()==named);
        require(physics.NavigationContainmentVolumes.sorts==unsigned(file.root.present&&(need_nav||nav)));
        require(physics.ContainmentVolumes.sorts==unsigned(file.root.present&&(need_named||named)));
    }
    std::puts("00a6d670_TEST PASS");return 0;
}