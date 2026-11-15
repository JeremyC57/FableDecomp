// Complete production loader below. Runtime strings/stream boundaries are portable semantic models; native ABI audited separately.
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
static bool bad;static int liveStrings,liveData;static unsigned int weightCoverage[256],idCoverage[256];
class CCharString {struct Data{std::string text;int refs;Data(const std::string&s):text(s),refs(1){++liveData;}~Data(){--liveData;}};Data*p;void Release(){if(p&&--p->refs==0)delete p;p=0;}public:CCharString():p(0){++liveStrings;}explicit CCharString(const std::string&s):p(s.empty()?0:new Data(s)){++liveStrings;}CCharString(const CCharString&s):p(s.p){if(p)++p->refs;++liveStrings;}~CCharString(){Release();--liveStrings;}const CCharString&operator=(const CCharString&s){if(this!=&s){Release();p=s.p;if(p)++p->refs;}return*this;}std::string Text()const{return p?p->text:std::string();}};
class CDataInputStream {const std::vector<unsigned char>&bytes;unsigned int chunk;public:unsigned long position;unsigned int ulongs,ubytes,strings,crossings;CDataInputStream(const std::vector<unsigned char>&a,unsigned int n):bytes(a),chunk(n),position(0),ulongs(0),ubytes(0),strings(0),crossings(0){}void Read(void*p,unsigned int n){if(position+n>bytes.size()){bad=true;memset(p,0,n);return;}unsigned int done=0;while(done<n){unsigned int take=n-done;if(chunk){unsigned int room=chunk-((unsigned int)position%chunk);if(take>room){take=room;++crossings;}}memcpy((unsigned char*)p+done,&bytes[(unsigned int)position],take);done+=take;position+=take;}}unsigned long ReadULONG(){++ulongs;unsigned char x[4];Read(x,4);return(unsigned long)((unsigned int)x[0]|((unsigned int)x[1]<<8)|((unsigned int)x[2]<<16)|((unsigned int)x[3]<<24));}unsigned char ReadUBYTE(){++ubytes;unsigned char x;Read(&x,1);return x;}virtual CCharString ReadNullTerminatedString(){++strings;std::string s;for(;;){unsigned char c;Read(&c,1);if(!c)break;s+=(char)c;}return CCharString(s);}virtual~CDataInputStream(){}};
class CLipSyncPhoneme{public:CCharString Symbol;unsigned long ID;float Weight;CLipSyncPhoneme():ID(0),Weight(0){}};
template<class T>class CArray:public std::vector<T>{};
class CLipSyncFrameData{public:CArray<CLipSyncPhoneme>Phonemes;};
class CLipSyncSampleData{public:CArray<CLipSyncFrameData>FrameData;float FramesPerSecond;void ReadFromFile(CDataInputStream&);};

void CLipSyncSampleData::ReadFromFile(CDataInputStream& is)
{
    std::map<unsigned long,CCharString> phoneme_map;
    unsigned long count = is.ReadULONG();
    for (unsigned long i = 0; i < count; ++i) {
        unsigned long id = is.ReadUBYTE();
        CCharString symbol = is.ReadNullTerminatedString();
        phoneme_map[id] = symbol;
    }
    FramesPerSecond = (float)is.ReadULONG();
    FrameData.resize(is.ReadULONG());
    for (unsigned long c_frame = 0; c_frame < FrameData.size(); ++c_frame) {
        CLipSyncFrameData& frame = FrameData[c_frame];
        unsigned long n = is.ReadUBYTE();
        for (unsigned long k = 0; k < n; ++k) {
            CLipSyncPhoneme phoneme;
            phoneme.ID = is.ReadUBYTE();
            phoneme.Weight = (float)is.ReadUBYTE() * (1.0f / 255.0f);
            if (phoneme.Weight < 0.0f)
                phoneme.Weight = 0.0f;
            else if (phoneme.Weight > 1.0f)
                phoneme.Weight = 1.0f;
            phoneme.Symbol = phoneme_map[phoneme.ID];
            frame.Phonemes.push_back(phoneme);
        }
    }
}


struct Expected {std::string text;unsigned long id;float weight;Expected(const std::string&s,unsigned long i,float w):text(s),id(i),weight(w){}};
static int Fail(int id,const char*msg){printf("00a495f0_TEST FAIL %d %s\n",id,msg);return 1;}
static void Put32(std::vector<unsigned char>&b,unsigned int n){for(int i=0;i<4;++i)b.push_back((unsigned char)(n>>(8*i)));}
static bool Bits(float a,float b){return memcmp(&a,&b,4)==0;}
static unsigned int calls,decoded,legacyChecked;
int main(){
 const unsigned int fpsValues[]={0u,1u,24u,60u,0x80000001u,0xffffff00u,0xffffffffu};const float fpsFloat[]={0.0f,1.0f,24.0f,60.0f,2147483648.0f,4294967040.0f,4294967296.0f};const unsigned int chunks[]={0,1,3,17};const unsigned int dictCounts[]={0,1,4,19};const unsigned int counts[]={0,1,4,17,255};int id=0;
 for(unsigned int old=0;old<5;++old)for(unsigned int frames=0;frames<6;++frames)for(unsigned int dict=0;dict<4;++dict)for(unsigned int ch=0;ch<4;++ch,++id){
  if(liveStrings||liveData)return Fail(id,"prior lifetime leak");
  bad=false;
  {
   CLipSyncSampleData sample;sample.FramesPerSecond=-1;sample.FrameData.resize(old);std::vector<std::vector<Expected> > expected(old);
   for(unsigned int f=0;f<old;++f)for(unsigned int j=0;j<=f;++j){char text[32];sprintf(text,"old_%u_%u",f,j);CLipSyncPhoneme p;p.Symbol=CCharString(std::string(text));p.ID=900+f*8+j;p.Weight=-1.0f-(float)j;sample.FrameData[f].Phonemes.push_back(p);expected[f].push_back(Expected(text,p.ID,p.Weight));}
   for(unsigned int round=0;round<2;++round){
    std::vector<unsigned char>input;std::string names[256];unsigned int nameCount=dictCounts[(dict+round)%4];Put32(input,nameCount);
    for(unsigned int k=0;k<nameCount;++k){unsigned int key=(k%3==2?(unsigned int)id:(unsigned int)id+k*31u)&255u;char text[64];sprintf(text,"name_%d_%u_%u",id,round,k);input.push_back((unsigned char)key);for(const char*p=text;;++p){input.push_back((unsigned char)*p);if(!*p)break;}names[key]=text;}
    unsigned int rate=((unsigned int)id+round)%7;Put32(input,fpsValues[rate]);unsigned int nf=round?(frames+2)%6:frames;Put32(input,nf);expected.resize(nf);unsigned int total=0;unsigned int before=0;for(unsigned int f=0;f<nf;++f)before+=(unsigned int)expected[f].size();
    for(unsigned int f=0;f<nf;++f){unsigned int count=counts[((unsigned int)id+f+round)%5];input.push_back((unsigned char)count);for(unsigned int j=0;j<count;++j){unsigned int key=(j+f*37u+(unsigned int)id*11u+round*5u)&255u;unsigned int w=(j+f*71u+(unsigned int)id*19u+round*13u)&255u;input.push_back((unsigned char)key);input.push_back((unsigned char)w);float weight=(float)w*(1.0f/255.0f);if(weight>1.0f)weight=1.0f;expected[f].push_back(Expected(names[key],key,weight));++total;++weightCoverage[w];++idCoverage[key];}}
    CDataInputStream is(input,chunks[ch]);sample.ReadFromFile(is);++calls;decoded+=total;legacyChecked+=before;
    if(bad||is.position!=input.size()||is.ulongs!=3||is.ubytes!=nameCount+nf+2*total||is.strings!=nameCount||!Bits(sample.FramesPerSecond,fpsFloat[rate])||sample.FrameData.size()!=nf)return Fail(id,"wire protocol, frame rate and resized length");
    if(chunks[ch]==1&&is.crossings==0)return Fail(id,"chunk path");
    for(unsigned int f=0;f<nf;++f){if(sample.FrameData[f].Phonemes.size()!=expected[f].size())return Fail(id,"preserve prefix then append");for(unsigned int j=0;j<expected[f].size();++j){const CLipSyncPhoneme&p=sample.FrameData[f].Phonemes[j];const Expected&e=expected[f][j];if(p.Symbol.Text()!=e.text||p.ID!=e.id||!Bits(p.Weight,e.weight))return Fail(id,"phoneme symbol,id or weight");}}
   }
  }
  if(liveStrings||liveData)return Fail(id,"string/map/frame cleanup");
 }
 if(id!=480||calls!=960||!decoded||!legacyChecked)return Fail(id,"case coverage");
 for(unsigned int i=0;i<256;++i)if(!weightCoverage[i]||!idCoverage[i])return Fail(id,"all byte IDs and weights");
 puts("00a495f0_TEST PASS");return 0;
}