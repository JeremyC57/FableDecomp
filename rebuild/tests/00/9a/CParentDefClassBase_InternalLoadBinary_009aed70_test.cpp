// Dependency doubles for the exact production InternalLoadBinary body.
#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>
#include <utility>
#include <algorithm>
class CDataInputStream {
    std::vector<unsigned char> bytes;
    unsigned int pos,chunk;
public:
    unsigned int fast,slow,base_calls;
    explicit CDataInputStream(unsigned int n):pos(0),chunk(n),fast(0),slow(0),base_calls(0) {}
    void Word(unsigned int v){bytes.push_back((unsigned char)v);bytes.push_back((unsigned char)(v>>8));}
    void Long(unsigned int v){for(unsigned int j=0;j<4;++j)bytes.push_back((unsigned char)(v>>(j*8)));}
    void Read(void* target,unsigned int n){assert(pos+n<=bytes.size());if(chunk&&pos/chunk==(pos+n-1)/chunk)++fast;else ++slow;std::memcpy(target,&bytes[pos],n);pos+=n;}
    unsigned short ReadUWORD(){unsigned short v;Read(&v,2);return v;}
    unsigned long ReadULONG(){unsigned int v;Read(&v,4);return v;}
    long ReadSLONG(){int v;Read(&v,4);return v;}
    bool Finished()const{return pos==bytes.size();}
};
class CDefClassBase {
public:
    class CSubDefInfo {public:long DefIndex,OriginalParentDefIndex;};
    void InternalLoadBinary(CDataInputStream& is){++is.base_calls;}
};
template<class K,class V> class CVectorMap {
public:
    typedef std::pair<K,V> value_type;
    std::vector<value_type> entries;
    bool dirty;
    unsigned int grew,reused;
    CVectorMap():dirty(true),grew(0),reused(0){}
    void reserve(unsigned long n){entries.reserve((unsigned int)n);}
    void Insert(const K& k,const V& v){if(entries.size()==entries.capacity())++grew;else ++reused;entries.push_back(value_type(k,v));dirty=true;}
    void SetDirty(bool v){dirty=v;}
};
class CParentDefClassBase:public CDefClassBase {
public:
    CVectorMap<unsigned long,CSubDefInfo> SubDefInfoMap;
    void InternalLoadBinary(CDataInputStream&);
};

void CParentDefClassBase::InternalLoadBinary(CDataInputStream& is)
{
    CDefClassBase::InternalLoadBinary(is);
    unsigned long no_subs=is.ReadUWORD();
    SubDefInfoMap.reserve(no_subs);
    for (unsigned long c_sub=0;c_sub<no_subs;++c_sub) {
        CSubDefInfo sub_def_info;
        unsigned long name_crc=is.ReadULONG();
        sub_def_info.DefIndex=is.ReadSLONG();
        sub_def_info.OriginalParentDefIndex=is.ReadSLONG();
        SubDefInfoMap.Insert(name_crc,sub_def_info);
    }
    SubDefInfoMap.SetDirty(false);
}
static void Record(CDataInputStream& is,unsigned int key,int index,int parent){is.Long(key);is.Long((unsigned int)index);is.Long((unsigned int)parent);}
static void Check(const CParentDefClassBase& p,unsigned int at,unsigned long key,long index,long parent){assert(p.SubDefInfoMap.entries.at(at).first==key);assert(p.SubDefInfoMap.entries.at(at).second.DefIndex==index);assert(p.SubDefInfoMap.entries.at(at).second.OriginalParentDefIndex==parent);}
int main(){
    {CParentDefClassBase p;CDataInputStream is(1024);is.Word(0);p.InternalLoadBinary(is);assert(is.base_calls==1&&is.Finished());assert(p.SubDefInfoMap.entries.empty()&&!p.SubDefInfoMap.dirty);}
    {CParentDefClassBase p;CDataInputStream is(1024);is.Word(1);Record(is,0xfedcba98U,-1,17);p.InternalLoadBinary(is);Check(p,0,0xfedcba98UL,-1,17);assert(p.SubDefInfoMap.reused==1&&p.SubDefInfoMap.grew==0);assert(!p.SubDefInfoMap.dirty&&is.fast==4&&is.slow==0&&is.Finished());}
    {CParentDefClassBase p;CDataInputStream is(3);is.Word(3);Record(is,7,8,9);Record(is,7,-20,-30);Record(is,0,0,0);p.InternalLoadBinary(is);Check(p,0,7,8,9);Check(p,1,7,-20,-30);Check(p,2,0,0,0);assert(is.fast&&is.slow&&is.Finished());assert(!p.SubDefInfoMap.dirty&&p.SubDefInfoMap.entries.size()==3);}
    {CParentDefClassBase p;CDefClassBase::CSubDefInfo old;old.DefIndex=101;old.OriginalParentDefIndex=102;p.SubDefInfoMap.Insert(100,old);p.SubDefInfoMap.Insert(200,old);p.SubDefInfoMap.grew=p.SubDefInfoMap.reused=0;CDataInputStream is(5);is.Word(3);Record(is,1,2,3);Record(is,4,5,6);Record(is,7,8,9);p.InternalLoadBinary(is);assert(p.SubDefInfoMap.entries.size()==5);Check(p,0,100,101,102);Check(p,1,200,101,102);Check(p,2,1,2,3);Check(p,4,7,8,9);assert(p.SubDefInfoMap.grew&&p.SubDefInfoMap.reused&&!p.SubDefInfoMap.dirty&&is.Finished());}
    std::puts("009aed70_TEST PASS");return 0;
}