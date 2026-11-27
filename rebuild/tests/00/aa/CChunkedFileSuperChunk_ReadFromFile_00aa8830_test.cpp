#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void check(bool b){if(!b)exit(2);}
static int live, clones, read_count, getter_count;
class CDataInputStream{
public:
 std::vector<unsigned char> data; unsigned long pos; std::vector<unsigned long> seeks;
 CDataInputStream():pos(0){}
 void Read(void* dst,long n){check(n>=0&&pos+n<=data.size());memcpy(dst,&data[pos],n);pos+=n;}
 unsigned long ReadULONG(){unsigned long n;Read(&n,4);return n;}
 unsigned long GetPosition()const{++getter_count;return pos;}
 void SetPosition(unsigned long p){check(p<=data.size());seeks.push_back(p);pos=p;}
 void add(const char* type,unsigned long n){for(int i=0;i<4;++i)data.push_back(type[i]);for(int j=0;j<4;++j)data.push_back((unsigned char)(n>>(8*j)));for(unsigned long k=0;k<n;++k)data.push_back((unsigned char)(k+10));}
};
class CChunkedFileLoader{public:bool enabled;int GetReadUnknownChunkTypes()const{return enabled;}};
class CChunkedFileSuperChunk;
class CChunkedFileChunk{
public:
 char type[5];CChunkedFileSuperChunk* parent;unsigned long requested;bool raw;int refs;
 explicit CChunkedFileChunk(const char* t):parent(0),requested(0),raw(false),refs(0){++live;memcpy(type,t,5);}
 virtual ~CChunkedFileChunk(){--live;}
 virtual CChunkedFileChunk* CreateNewInstance()const{++clones;return new CChunkedFileChunk(type);}
 virtual void ReadFromFile(CDataInputStream& is,unsigned long bytes,CChunkedFileLoader&){++read_count;requested=bytes;if(bytes){unsigned char first;is.Read(&first,1);check(first==10);}}
};
class CChunkedFileRawDataChunk:public CChunkedFileChunk{
public:
 explicit CChunkedFileRawDataChunk(const char* type):CChunkedFileChunk(type){raw=true;}
};
template<class T>class CCountedPointer{
 T* data;
 void release(){if(data&&!--data->refs)delete data;}
public:
 explicit CCountedPointer(T* p):data(p){if(data)++data->refs;}
 CCountedPointer(const CCountedPointer& rhs):data(rhs.data){if(data)++data->refs;}
 CCountedPointer& operator=(const CCountedPointer& rhs){if(data!=rhs.data){release();data=rhs.data;if(data)++data->refs;}return *this;}
 ~CCountedPointer(){release();}
 T* operator->()const{return data;}
};
class CChunkedFileSuperChunk{
public:
 CChunkedFileChunk registered;std::vector<CCountedPointer<CChunkedFileChunk> > children;
 CChunkedFileSuperChunk():registered("KNWN"){}
 const CChunkedFileChunk* FindRegisteredSubChunkType(const char* type)const{check(type[4]==0);return strcmp(type,"KNWN")==0?&registered:0;}
 void AddSubChunk(const CCountedPointer<CChunkedFileChunk>& p){p->parent=this;children.push_back(p);}
 void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
};
void CChunkedFileSuperChunk::ReadFromFile(CDataInputStream& is, unsigned long bytes, CChunkedFileLoader& loader)
{
    unsigned long bytes_read = 0;
    while (bytes_read < bytes) {
        char chunktype[5] = {0};
        is.Read(chunktype, 4);
        chunktype[4] = 0;
        unsigned long chunksize = is.ReadULONG();
        bytes_read += 8;
        long oldpos = is.GetPosition();
        const CChunkedFileChunk* clonable_chunk = FindRegisteredSubChunkType(chunktype);
        if (clonable_chunk) {
            CCountedPointer<CChunkedFileChunk> new_chunk(clonable_chunk->CreateNewInstance());
            new_chunk->ReadFromFile(is, chunksize, loader);
            long chunksize_read = is.GetPosition() - oldpos;
            AddSubChunk(new_chunk);
        } else if (loader.GetReadUnknownChunkTypes()) {
            CCountedPointer<CChunkedFileChunk> new_chunk(new CChunkedFileRawDataChunk(chunktype));
            new_chunk->ReadFromFile(is, chunksize, loader);
            AddSubChunk(new_chunk);
        }
        is.SetPosition(oldpos + chunksize);
        bytes_read += chunksize;
    }
}
int main(){
 for(int length=0;length<=4;++length)for(int enabled=0;enabled<2;++enabled)for(int mask=0;mask<(1<<length);++mask){
  CDataInputStream stream;CChunkedFileLoader loader;loader.enabled=enabled!=0;
  clones=read_count=getter_count=0;int known=0,expected_children=0;
  std::vector<unsigned long> ends;
  for(int i=0;i<length;++i){bool registered=(mask&(1<<i))!=0;stream.add(registered?"KNWN":"UNKN",i);ends.push_back(stream.data.size());if(registered)++known;if(registered||enabled)++expected_children;}
  {
   CChunkedFileSuperChunk parent;unsigned long read_limit=stream.data.size();stream.add("STOP",2);
   parent.ReadFromFile(stream,read_limit,loader);
   check(stream.pos==read_limit&&stream.seeks==ends);
   check(clones==known&&read_count==expected_children&&getter_count==length+known);
   check((int)parent.children.size()==expected_children&&live==expected_children+1);
   unsigned child=0;
   for(int j=0;j<length;++j){bool registered=(mask&(1<<j))!=0;if(!registered&&!enabled)continue;
    const CCountedPointer<CChunkedFileChunk>& p=parent.children[child++];check(p->parent==&parent&&p->requested==(unsigned long)j&&p->raw==!registered&&p->refs==1);
    check(strcmp(p->type,registered?"KNWN":"UNKN")==0);
   }
  }
  check(live==0);
 }
 puts("00aa8830_TEST PASS");
}