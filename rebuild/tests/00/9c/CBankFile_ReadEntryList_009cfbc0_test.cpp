#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <vector>
class CCharString {
public:
    std::string value;
    CCharString(const char* text="") : value(text) {}
};
template<class T> class CArray : public std::vector<T> {
public:
    CArray() {}
    explicit CArray(unsigned n) : std::vector<T>(n) {}
    T* GetAsCArray() {return this->empty()?0:&(*this)[0];}
    const T* GetAsCArray() const {return this->empty()?0:&(*this)[0];}
};
template<class T,int N> class CSmallVector {
public:
    T* Start;
    signed char Size;
    CSmallVector() : Start(0),Size(0) {}
    explicit CSmallVector(unsigned n) : Start(0),Size(0) { Allocate(n); }
    ~CSmallVector() {Clear();}
    bool empty()const{return Size==0;}
    T& operator[](unsigned long i){assert(i<(unsigned long)Size);return Start[i];}
    CSmallVector& operator=(const CSmallVector& other){if(&other!=this)Copy(other);return *this;}
    void Allocate(unsigned long);
    void Copy(const CSmallVector&);
    void Clear(){delete[] Start;Start=0;Size=0;}
private:
    CSmallVector(const CSmallVector&);
};
class CFileDataInputStream{
public:
    std::vector<unsigned char> bytes;
    unsigned long pos;
    CFileDataInputStream():pos(0){}
    void WriteWord(unsigned long x){for(int n=0;n<4;++n) bytes.push_back((unsigned char)(x>>(n*8)));}
    void WriteString(const char* x){WriteWord((unsigned long)strlen(x));while(*x)bytes.push_back((unsigned char)*x++);}
    unsigned long ReadULONG(){unsigned long n=0;for(int j=0;j<4;++j){assert(pos<bytes.size());n|=((unsigned long)bytes[pos++])<<(j*8);}return n;}
    CCharString ReadPresizedString(){CCharString s;unsigned long n=ReadULONG();while(n--){assert(pos<bytes.size());s.value+=(char)bytes[pos++];}return s;}
    void Read(void* out,long size){assert(size>=0&&pos+(unsigned long)size<=bytes.size());if(size)memcpy(out,&bytes[pos],size);pos+=size;}
    unsigned long GetPosition()const{return pos;}
    void SetPosition(unsigned long p){assert(p<=bytes.size());pos=p;}
};
class CBankFileDataTypeCount {public: unsigned long DataType,Count;};
class CBankFileEntryUpdateData {
public:
    unsigned long crc;
    std::vector<char> info;
    CSmallVector<CCharString,8> Filenames;
    void SetStateBlockCRC(unsigned long n){crc=n;}
    void SetInfo(const CArray<char>& a){info.assign(a.begin(),a.end());}
};
struct IndexFixture { int updates, trims; IndexFixture():updates(0),trims(0){} void Update(){++updates;} void DeallocateUnusedMemory(){++trims;} };
class CBankFile{
public:
    struct CRuntimeData{
        unsigned long DataSize,DataOffset; unsigned char DataType; bool Valid;
        CRuntimeData():DataSize(0),DataOffset(0),DataType(0),Valid(false){}
    };
    unsigned long Size,OpenFlags;bool FileValid,ReadOnly;
    int packed,size_events,finished;
    CArray<CRuntimeData> RuntimeData;
    CArray<CCharString> Symbols; CArray<unsigned long> Checksums;
    CArray<CBankFileEntryUpdateData*> UpdateData;
    IndexFixture SymbolCRCIndices;
    std::vector<CBankFileDataTypeCount> types;
    std::vector<unsigned long> added,notified,notificationType;
    std::vector<std::string> names,firstFile,payloads;
    CFileDataInputStream* stream;
    CBankFile():Size(0),OpenFlags(0),FileValid(false),ReadOnly(false),packed(0),size_events(0),finished(0),stream(0){}
    ~CBankFile(){for(unsigned i=0;i<UpdateData.size();++i)delete UpdateData[i];}
    void AllocateBuffers(unsigned long n){Size=n;RuntimeData.resize(n);Symbols.resize(n);Checksums.resize(n);UpdateData.resize(n);}
    long GetSize()const{return Size;}
    void NotifyBankSize(unsigned long n,const CArray<CBankFileDataTypeCount>& t){assert(n==Size&&added.empty());++size_events;types.assign(t.begin(),t.end());}
    void InitEntry(unsigned long i,const CCharString& s,const CCharString& f){added.push_back(i);names.push_back(s.value);firstFile.push_back(f.value);}
    void NotifyEntryAdded(unsigned long t,unsigned long i,const CArray<char>& b){assert(added.back()==i);notified.push_back(i);notificationType.push_back(t);payloads.push_back(std::string(b.begin(),b.end()));stream->SetPosition(0);}
    void PackRuntimeData(){assert(SymbolCRCIndices.trims==1);++packed;}
    void NotifyPostEntriesLoaded(){assert(!FileValid&&SymbolCRCIndices.trims==1);++finished;}
    void PostInitEntries();
    bool ReadEntryList(CFileDataInputStream&,unsigned long,unsigned long);
};
// Original donor 02FA2EA0 clears the previous array, allocates count strings and narrows the stored size.
template<class T,int N> void CSmallVector<T,N>::Allocate(unsigned long count)
{
    Clear();
    Start = new T[count];
    Size = (signed char)count;
}

// Original donor 02FA2FA0 first resizes, then assigns elements in ascending index order.
template<class T,int N> void CSmallVector<T,N>::Copy(const CSmallVector& other)
{
    Allocate(other.Size);
    for(unsigned long i=0; i<(unsigned long)other.Size; ++i)
        Start[i]=other.Start[i];
}

// FableWin 02F9C690 calls Update, the storage trim and optional runtime packing in order.
inline void CBankFile::PostInitEntries()
{
    SymbolCRCIndices.Update();
    SymbolCRCIndices.DeallocateUnusedMemory();
    if(ReadOnly) PackRuntimeData();
}

bool CBankFile::ReadEntryList(CFileDataInputStream& is,unsigned long max_index,unsigned long entry_count)
{
    AllocateBuffers(max_index+1);
    unsigned long no_types=is.ReadULONG();
    CArray<CBankFileDataTypeCount> datatype_counts(no_types);
    for(unsigned long c_type=0;c_type<no_types;++c_type) {
        datatype_counts[c_type].DataType=is.ReadULONG();
        datatype_counts[c_type].Count=is.ReadULONG();
    }
    NotifyBankSize(GetSize(),datatype_counts);
    for(unsigned long c_entry=0;c_entry<entry_count;++c_entry) {
        unsigned long magic=is.ReadULONG();
        unsigned long index=is.ReadULONG();
        unsigned long data_type=is.ReadULONG();
        unsigned long data_size=is.ReadULONG();
        unsigned long data_offset=is.ReadULONG();
        unsigned long state_block_crc=is.ReadULONG();
        CCharString symbol=is.ReadPresizedString();
        unsigned long checksum=is.ReadULONG();
        unsigned long file_count=is.ReadULONG();
        CSmallVector<CCharString,8> filenames(file_count);
        for(unsigned long c_file=0;c_file<file_count;++c_file) filenames[c_file]=is.ReadPresizedString();
        unsigned long info_size=is.ReadULONG();
        CArray<char> info_buffer(info_size);
        is.Read(info_buffer.GetAsCArray(),info_size);
        CRuntimeData& runtime_data=RuntimeData[index];
        runtime_data.DataType=(unsigned char)data_type;
        runtime_data.DataSize=data_size;
        runtime_data.DataOffset=data_offset;
        runtime_data.Valid=true;
        if(OpenFlags&8) Symbols[index]=symbol;
        if(OpenFlags&16) Checksums[index]=checksum;
        if(OpenFlags&32) {
            CBankFileEntryUpdateData* update_data=new CBankFileEntryUpdateData;
            update_data->SetStateBlockCRC(state_block_crc);
            update_data->SetInfo(info_buffer);
            update_data->Filenames=filenames;
            UpdateData[index]=update_data;
        }
        CCharString filename=filenames.empty() ? CCharString("") : filenames[0];
        InitEntry(index,symbol,filename);
        unsigned long current_offset=is.GetPosition();
        NotifyEntryAdded(data_type,index,info_buffer);
        is.SetPosition(current_offset);
    }
    PostInitEntries();
    NotifyPostEntriesLoaded();
    FileValid=true;
    return true;
}

int main(){
    const char* filenames[]={"alpha","beta"};
    for(unsigned flags=0;flags<8;++flags) for(int readonly=0;readonly!=2;++readonly)
    for(unsigned n=0;n<3;++n) for(unsigned types=0;types<3;++types) for(unsigned fileCount=0;fileCount<3;++fileCount){
        CFileDataInputStream stream;stream.WriteWord(types);
        for(unsigned i=0;i<types;++i){stream.WriteWord(9+i);stream.WriteWord(41+i);}
        for(unsigned e=0;e<n;++e){
            stream.WriteWord(0x19283746UL);stream.WriteWord(e+1);stream.WriteWord(0x109+e);stream.WriteWord(71+e);
            stream.WriteWord(109+e);stream.WriteWord(0xDEAD0000UL+e);stream.WriteString(e?"two":"one");
            stream.WriteWord(0x1B2C0030UL+e);stream.WriteWord(fileCount);
            for(unsigned f=0;f<fileCount;++f)stream.WriteString(filenames[f]);
            stream.WriteWord(2*e);if(e){stream.bytes.push_back('x');stream.bytes.push_back('y');}
        }
        CBankFile bank;bank.OpenFlags=flags*8;bank.ReadOnly=readonly!=0;bank.stream=&stream;
        if(!bank.ReadEntryList(stream,4,n))return 1;
        if(stream.pos!=stream.bytes.size()||bank.Size!=5||!bank.FileValid||bank.packed!=readonly||bank.size_events!=1||bank.finished!=1||bank.SymbolCRCIndices.updates!=1)return 2;
        if(bank.types.size()!=types||bank.added.size()!=n||bank.notified.size()!=n||bank.RuntimeData[0].Valid||bank.RuntimeData[4].Valid)return 3;
        for(unsigned t=0;t<types;++t)if(bank.types[t].DataType!=9+t||bank.types[t].Count!=41+t)return 4;
        for(unsigned e=0;e<n;++e){
            unsigned ix=e+1;std::string name=e?"two":"one", payload=e?"xy":"";
            const CBankFile::CRuntimeData& data=bank.RuntimeData[ix];
            if(!data.Valid||data.DataType!=9+e||data.DataOffset!=109+e||data.DataSize!=71+e)return 5;
            if(bank.added[e]!=ix||bank.notified[e]!=ix||bank.notificationType[e]!=0x109+e||bank.names[e]!=name||bank.firstFile[e]!=(fileCount?"alpha":"")||bank.payloads[e]!=payload)return 6;
            if(bank.Symbols[ix].value!=((flags&1)?name:"")||bank.Checksums[ix]!=((flags&2)?0x1B2C0030UL+e:0))return 7;
            CBankFileEntryUpdateData* info=bank.UpdateData[ix];
            if((info!=0)!=((flags&4)!=0))return 8;
            if(info){
                if(info->crc!=0xDEAD0000UL+e||std::string(info->info.begin(),info->info.end())!=payload||info->Filenames.Size!=(int)fileCount)return 9;
                for(unsigned k=0;k<fileCount;++k)if(info->Filenames[k].value!=filenames[k])return 10;
            }
        }
    }
    puts("009cfbc0_TEST PASS");return 0;
}