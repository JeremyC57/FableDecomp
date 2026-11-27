#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <map>
static void ck(bool b){if(!b)exit(2);}
static int diagnostic,shown,sorted,cleared;
#define FABLE_RETAIL_ASSERT(e) do{if(!(e))++diagnostic;}while(0)
struct CDataInputStream{
 std::vector<unsigned char> bytes;unsigned at;
 CDataInputStream():at(0){}
 bool IsEOF()const{return at==bytes.size();}
 bool ReadEBOOL(){ck(at<bytes.size());return bytes[at++]!=0;}
 unsigned long ReadULONG(){ck(at+4<=bytes.size());unsigned long v=0;for(int i=0;i<4;++i)v|=(unsigned long)bytes[at++]<<(8*i);return v;}
 void Read(void*p,unsigned long n){ck(at+n<=bytes.size());if(n){memcpy(p,&bytes[at],n);at+=n;}}
 void put(unsigned long u){for(int i=0;i<4;++i)bytes.push_back((unsigned char)(u>>(8*i)));}
};
namespace NProgressDisplay{static void DisplayProgress(const char*s,float f,bool a,bool b){ck(!strcmp(s,"Definition Manager: Load Binary Def")&&f==-1&&!a&&!b);++shown;}}
struct DefStrings{unsigned long GetTableRandomID()const{return 107;}}GDefStringTable;
struct CDefString{unsigned long t;void TransferBinaryIn(CDataInputStream& is){t=is.ReadULONG();}unsigned long GetCRC()const{return t*10;}};
template<class T>struct Arr:std::vector<T>{void* GetAsCArray(){return empty()?0:&front();}unsigned long GetByteSize()const{return (unsigned long)(size()*sizeof(T));}};
struct NameIndex:std::map<unsigned long,long>{void Insert(unsigned long key,long v){(*this)[key]=v;}void Update(){++sorted;}};
struct CDefinitionManager{
 struct CInstantiatedDefInfo{CDefString ClassName,InstantiationName;unsigned long ClassIndex;};
 struct CDefClassInfo{std::vector<unsigned long> GlobalIndices;};
 std::map<unsigned long,CDefClassInfo> classes;NameIndex NameCRCToIndexMap;
 std::vector<CInstantiatedDefInfo*> InstantiatedDefs;Arr<std::pair<unsigned long,unsigned long> > CompressedOffsets;
 Arr<unsigned char> CompressedDefData;unsigned long DependencyCheckCRC;bool UseSafeBinaryFormat;
 CDefinitionManager():DependencyCheckCRC(0),UseSafeBinaryFormat(false){}
 void ClearInstantiatedDefs(){++cleared;for(unsigned i=0;i<InstantiatedDefs.size();++i)delete InstantiatedDefs[i];InstantiatedDefs.clear();NameCRCToIndexMap.clear();}
 CDefClassInfo*GetPDefClassInfo(unsigned long key){std::map<unsigned long,CDefClassInfo>::iterator i=classes.find(key);return i==classes.end()?0:&i->second;}
 bool LoadBinaryDefinitions(CDataInputStream&);
 ~CDefinitionManager(){ClearInstantiatedDefs();}
};
bool CDefinitionManager::LoadBinaryDefinitions(CDataInputStream& is)
{
    if (is.IsEOF())
        return false;
    bool safe_format = is.ReadEBOOL();
    unsigned long saved_crc = is.ReadULONG();
    if (DependencyCheckCRC && saved_crc != DependencyCheckCRC)
        return false;
    unsigned long string_table_id = is.ReadULONG();
    if (string_table_id != GDefStringTable.GetTableRandomID())
        return false;
    UseSafeBinaryFormat = safe_format;
    ClearInstantiatedDefs();
    unsigned long no_defs = is.ReadULONG();
    InstantiatedDefs.resize(no_defs);
    for (long c_def = 0; c_def < no_defs; ++c_def) {
        NProgressDisplay::DisplayProgress("Definition Manager: Load Binary Def", -1.0f, false, false);
        InstantiatedDefs[c_def] = new CInstantiatedDefInfo;
        CInstantiatedDefInfo& def_info = *InstantiatedDefs[c_def];
        def_info.ClassName.TransferBinaryIn(is);
        def_info.InstantiationName.TransferBinaryIn(is);
        def_info.ClassIndex = is.ReadULONG();
        CDefClassInfo* pdef_type_info = GetPDefClassInfo(def_info.ClassName.GetCRC());
        if (!pdef_type_info) {
            FABLE_RETAIL_ASSERT(false);
            continue;
        }
        long class_index = pdef_type_info->GlobalIndices.size();
        FABLE_RETAIL_ASSERT(class_index == def_info.ClassIndex);
        pdef_type_info->GlobalIndices.push_back(c_def);
        NameCRCToIndexMap.Insert(def_info.InstantiationName.GetCRC(), c_def);
    }
    unsigned long offsets_size = is.ReadULONG();
    CompressedOffsets.resize(offsets_size);
    is.Read(CompressedOffsets.GetAsCArray(), CompressedOffsets.GetByteSize());
    unsigned long data_size = is.ReadULONG();
    CompressedDefData.resize(data_size);
    is.Read(CompressedDefData.GetAsCArray(), CompressedDefData.GetByteSize());
    NameCRCToIndexMap.Update();
    return true;
}
int main(){
 for(int flag=0;flag<4;++flag)for(int crc=0;crc<3;++crc)for(int n=0;n<5;++n)
 for(int missing=0;missing<16;++missing)for(int blob=0;blob<5;++blob)for(int table=0;table<2;++table){
  CDefinitionManager m;m.classes[10];m.classes[20];m.DependencyCheckCRC=crc?98:0;m.UseSafeBinaryFormat=true;
  CDataInputStream in;
  if(flag){in.bytes.push_back(flag==1?0:flag==2?1:255);in.put(crc==2?55:98);in.put(table?107:99);in.put(n);}
  int classCount[2]={0,0},errors=0;
  for(int j=0;j<n&&flag;++j){bool absent=(missing&(1<<j))!=0;unsigned idx=absent?0:classCount[j%2]++;
   in.put(absent?3:j%2+1);in.put(31+j);in.put(idx+(j==3));if(absent||j==3)++errors;}
  int pairs=blob%3;
  if(flag){in.put(pairs);for(int j=0;j<pairs;++j){in.put(100+j);in.put(210+j);}in.put(blob);for(int j=0;j<blob;++j)in.bytes.push_back((unsigned char)(0xa1+j));}
  shown=sorted=cleared=diagnostic=0;bool expected=flag && crc!=2 && table;ck(m.LoadBinaryDefinitions(in)==expected);
  if(!expected){ck(!cleared&&!sorted&&!shown&&!diagnostic&&m.UseSafeBinaryFormat);ck(in.at==(flag?(crc==2?5:9):0));continue;}
  ck(in.IsEOF()&&sorted==1&&cleared==1&&shown==n&&diagnostic==errors&&m.UseSafeBinaryFormat==(flag>1));
  ck(m.InstantiatedDefs.size()==n&&m.CompressedOffsets.size()==pairs&&m.CompressedDefData.size()==blob);
  unsigned seen[2]={0,0};
  for(int j=0;j<n;++j){CDefinitionManager::CInstantiatedDefInfo* inf=m.InstantiatedDefs[j];bool absent=(missing&(1<<j))!=0;
   ck(inf&&inf->ClassName.t==(unsigned)(absent?3:j%2+1)&&inf->InstantiationName.t==31+j);
   unsigned key=(31+j)*10;
   if(absent)ck(m.NameCRCToIndexMap.find(key)==m.NameCRCToIndexMap.end());
   else{ck(m.NameCRCToIndexMap[key]==j);ck(m.classes[10*(j%2+1)].GlobalIndices[seen[j%2]++]==j);}}
  for(int k=0;k<pairs;++k)ck(m.CompressedOffsets[k].first==100+k&&m.CompressedOffsets[k].second==210+k);
  for(int k=0;k<blob;++k)ck(m.CompressedDefData[k]==0xa1+k);
 }
 puts("009afb90_TEST PASS");return 0;
}