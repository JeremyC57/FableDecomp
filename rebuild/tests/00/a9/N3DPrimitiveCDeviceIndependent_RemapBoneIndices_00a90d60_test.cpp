#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
static void check(bool x){if(!x)exit(2);}
static int living, writes;
class C3DGroup2 {
public:
    long bone,refs;
    C3DGroup2():bone(-1),refs(0){++living;}
    ~C3DGroup2(){--living;}
    void SetBoneIndex(long index){bone=index;++writes;}
};
template<class T>class CCountedPointer {
    T* data;
    void release(){if(data&&--data->refs==0)delete data;}
public:
    CCountedPointer(T* p=0):data(p){if(data)++data->refs;}
    CCountedPointer(const CCountedPointer& p):data(p.data){if(data)++data->refs;}
    ~CCountedPointer(){release();}
    CCountedPointer& operator=(const CCountedPointer& p){if(data!=p.data){release();data=p.data;if(data)++data->refs;}return *this;}
    T* operator->()const{return data;}
    T* Get()const{return data;}
};
template<class T> class CArray:public std::vector<T>{
public:
    explicit CArray(unsigned n=0):std::vector<T>(n){}
    T& At(unsigned index){check(index<this->size());return (*this)[index];}
};
namespace N3DPrimitive {
class CDeviceIndependent {
public:
    CArray<CCountedPointer<C3DGroup2> > Groups;
    void RemapBoneIndices(std::map<unsigned long,unsigned long>&);
};
}
namespace N3DPrimitive {
void CDeviceIndependent::RemapBoneIndices(std::map<unsigned long,unsigned long>& index_map)
{
    if (Groups.empty()) return;
    CArray<CCountedPointer<C3DGroup2> > new_groups(index_map.size());
    unsigned long max_new_index=0;
    for (std::map<unsigned long,unsigned long>::iterator itr=index_map.begin();itr!=index_map.end();++itr) {
        unsigned long old_index=itr->first;
        unsigned long new_index=itr->second;
        if (old_index < Groups.size()) {
            new_groups.At(new_index)=Groups.At(old_index);
            new_groups.At(new_index)->SetBoneIndex(new_index);
            if (max_new_index<=new_index) max_new_index=new_index;
        }
    }
    new_groups.resize(max_new_index+1);
    Groups=new_groups;
}
}
int main(){
    // Retained owning references allow observation of objects removed from Groups.
    for(int g=0;g<=3;++g)for(int shape=0;shape<6;++shape)for(int shared=0;shared<2;++shared){
        {
            N3DPrimitive::CDeviceIndependent obj;
            CCountedPointer<C3DGroup2> held[3];
            for(int k=0;k<g;++k){if(k&&shared)held[k]=held[0];else held[k]=CCountedPointer<C3DGroup2>(new C3DGroup2);obj.Groups.push_back(held[k]);}
            std::map<unsigned long,unsigned long> m;
            if(shape==1)m[15]=0;
            if(shape==2){m[0]=1;m[1]=0;}
            if(shape==3){m[0]=0;m[1]=0;m[2]=0;}
            if(shape==4){m[0]=0;m[2]=1;m[12]=2;}
            if(shape==5){m[0]=2;m[1]=1;m[2]=0;}
            int count=0;unsigned highest=0;C3DGroup2* expected[4]={0,0,0,0};long last[3]={-1,-1,-1};
            for(std::map<unsigned long,unsigned long>::const_iterator it=m.begin();it!=m.end();++it){
                if(it->first<(unsigned)g){expected[it->second]=held[it->first].Get();if(it->second>highest)highest=it->second;++count;
                    for(int j=0;j<g;++j)if(held[j].Get()==held[it->first].Get())last[j]=it->second;}
            }
            writes=0;obj.RemapBoneIndices(m);
            check(writes==count&&obj.Groups.size()==(g?highest+1:0));
            for(unsigned i=0;i<obj.Groups.size();++i)check(obj.Groups[i].Get()==expected[i]);
            for(int j=0;j<g;++j){int refs=0;for(int k=0;k<g;++k)if(held[k].Get()==held[j].Get())++refs;
                for(unsigned k=0;k<obj.Groups.size();++k)if(expected[k]==held[j].Get())++refs;
                check(held[j]->bone==last[j]&&held[j]->refs==refs);}
        }
        check(!living);
    }
    puts("00a90d60_TEST PASS");return 0;
}