#ifndef _MSC_VER
#define FABLE_ENGINE_CDATAOUTPUTSTREAM_H
struct CDataOutputStream { void* __vftable; void* NewLine_PStringData; };
#endif
// C3DMeshFileEngineMaterialChunk::WriteToFile(CDataOutputStream&, CChunkedFileSaver&) -- retail @ 0x00AC52F0, 256 bytes; stream type, material values and optional map filenames, then inherit the subchunk writer. Retained CChunkedFileChunk -> CChunkedFileSuperChunk -> C3DMeshFileChunk hierarchy; its retail base is 0x2c bytes though the donor's 0x10-byte counted-vector layout grows it to 0x34. Calls stream writes at 0x00993EA0/0x00993EB0/0x00993E30/0x00995AE0 and SuperChunk::WriteToFile 0x00AA78C0.
struct CCharString { void* m_pStringData; };
#include "engine/CDataOutputStream.h"
struct CDataOutputStreamMethods : CDataOutputStream {
    void WriteLong(long value); // 0x00993EA0: 4-byte stream write
    void WriteLongSigned(long value); // 0x00993EB0
    void WriteNullTerminatedString(const CCharString& value); // 0x00995AE0
    void WriteEBOOL(bool value); // 0x00993E30
};
struct CChunkedFileSaver {};
struct CChunkedFileLoader {};
struct CDataInputStream;
#if defined(_MSC_VER)
#define RETAIL_FASTCALL __fastcall
#else
#define RETAIL_FASTCALL
#endif
struct SCountBlock { long references; void (RETAIL_FASTCALL* destroy)(void*); void* object; };
template<class T> struct CCountedPointer { T* object; SCountBlock* count; };
template<class T> struct CArray { T* begin; T* end; T* capacity; };
struct CChunkedFileSuperChunk;
// The shared CChunkedFileChunk header contains an explicit vptr data member, not virtual methods.
// This equivalent 0x10-byte virtual shell is required for WriteToFile slot8 and the qualified base call.
struct CChunkedFileChunk {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual void ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&);
    virtual void WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const;
    virtual CChunkedFileChunk* CreateNewInstance() const;
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk*);
    char ChunkType[5]; unsigned char _pad_09[3]; CChunkedFileSuperChunk* Parent;
};
struct CChunkedFileSuperChunk : CChunkedFileChunk {
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes;
    unsigned long CurrentChunkIndex;
    virtual void WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const; // 0x00AA78C0
};
struct C3DMeshFileChunk : CChunkedFileSuperChunk {};
struct CMapFilenames {
    void* __vftable; // +0; derived layout from CFilenames constructor @ 0x00ABEF60
    long references; // +4
    CCharString Diffuse; CCharString Bump; CCharString Reflection; CCharString Illumination; // +8,+c,+10,+14
};
struct C3DMeshFileEngineMaterialChunk : C3DMeshFileChunk {
    long MaterialIndex; CCharString Name; CMapFilenames* MapFilenames;
    unsigned long DiffuseMapBankIndex, BumpMapBankIndex, ReflectionMapBankIndex, IlluminationMapBankIndex;
    unsigned long MapFlags, SelfIllumination;
    bool TwoSided, Transparent, BooleanAlpha, DegenerateTriangles;
    virtual void WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const;
};
void C3DMeshFileEngineMaterialChunk::WriteToFile(CDataOutputStream& destination, CChunkedFileSaver& saver) const
{
    CDataOutputStreamMethods& stream=(CDataOutputStreamMethods&)destination;
    stream.WriteLong(4);
    stream.WriteLongSigned(MaterialIndex);
    stream.WriteNullTerminatedString(Name);
    stream.WriteLong(MapFlags);
    stream.WriteLong(DiffuseMapBankIndex);
    stream.WriteLong(BumpMapBankIndex);
    stream.WriteLong(ReflectionMapBankIndex);
    stream.WriteLong(IlluminationMapBankIndex);
    if (MapFilenames) {
        stream.WriteEBOOL(true);
        stream.WriteNullTerminatedString(MapFilenames->Diffuse);
        stream.WriteNullTerminatedString(MapFilenames->Bump);
        stream.WriteNullTerminatedString(MapFilenames->Reflection);
        stream.WriteNullTerminatedString(MapFilenames->Illumination);
    } else {
        stream.WriteEBOOL(false);
    }
    stream.WriteLong(SelfIllumination);
    stream.WriteEBOOL(TwoSided);
    stream.WriteEBOOL(Transparent);
    stream.WriteEBOOL(BooleanAlpha);
    stream.WriteEBOOL(DegenerateTriangles);
    CChunkedFileSuperChunk::WriteToFile(destination, saver);
}
#include <cstdio>
struct Event { char kind; unsigned long value; const void* stringData; const void* receiver; const void* stream; const void* saver; };
static Event events[32]; static int nevents;
static void record(char k,unsigned long v,const void* str,const void* recv,const void* out,const void* sav)
{ Event& e=events[nevents++]; e.kind=k;e.value=v;e.stringData=str;e.receiver=recv;e.stream=out;e.saver=sav; }
void CDataOutputStreamMethods::WriteLong(long v) { record('U',(unsigned long)v,0,this,this,0); }
void CDataOutputStreamMethods::WriteLongSigned(long v) { record('L',(unsigned long)v,0,this,this,0); }
void CDataOutputStreamMethods::WriteNullTerminatedString(const CCharString& s) { record('S',0,s.m_pStringData,this,this,0); }
void CDataOutputStreamMethods::WriteEBOOL(bool v) { record('B',v?1:0,0,this,this,0); }
void CChunkedFileChunk::v00(){} void CChunkedFileChunk::v01(){} void CChunkedFileChunk::v02(){} void CChunkedFileChunk::v03(){}
void CChunkedFileChunk::v04(){} void CChunkedFileChunk::v05(){} void CChunkedFileChunk::v06(){}
void CChunkedFileChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){}
void CChunkedFileChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{}
CChunkedFileChunk* CChunkedFileChunk::CreateNewInstance() const { return 0; }
CChunkedFileChunk* CChunkedFileChunk::InitNewInstance(CChunkedFileChunk* p) { return p; }
void CChunkedFileSuperChunk::WriteToFile(CDataOutputStream& out,CChunkedFileSaver& saver) const
{ record('T',0,0,this,&out,&saver); }
static int run(bool withFiles)
{
    C3DMeshFileEngineMaterialChunk x; CDataOutputStreamMethods stream; CChunkedFileSaver saver; CMapFilenames filenames;
    int tags[5]={1,2,3,4,5};
    x.MaterialIndex=-2; x.Name.m_pStringData=0;
    filenames.__vftable=0; filenames.references=1; filenames.Diffuse.m_pStringData=&tags[1];
    filenames.Bump.m_pStringData=0; filenames.Reflection.m_pStringData=&tags[3]; filenames.Illumination.m_pStringData=&tags[4];
    x.MapFilenames=withFiles?&filenames:0;
    x.DiffuseMapBankIndex=0x80000001UL; x.BumpMapBankIndex=11; x.ReflectionMapBankIndex=12; x.IlluminationMapBankIndex=13;
    x.MapFlags=0x80000005UL; x.SelfIllumination=0x8000000fUL; x.TwoSided=true; x.Transparent=false; x.BooleanAlpha=true; x.DegenerateTriangles=false;
    nevents=0; x.WriteToFile((CDataOutputStream&)stream,saver);
    if(nevents!=(withFiles?19:15)) return 0;
    int i=0;
    if(events[i].kind!='U'||events[i++].value!=4) return 0;
    if(events[i].kind!='L'||events[i++].value!=(unsigned long)-2) return 0;
    if(events[i].kind!='S'||events[i++].stringData!=0) return 0;
    if(events[i].kind!='U'||events[i++].value!=0x80000005UL) return 0;
    if(events[i].kind!='U'||events[i++].value!=0x80000001UL||events[i].kind!='U'||events[i++].value!=11||events[i].kind!='U'||events[i++].value!=12||events[i].kind!='U'||events[i++].value!=13) return 0;
    if(events[i].kind!='B'||events[i++].value!=(withFiles?1:0)) return 0;
    if(withFiles) { if(events[i].stringData!=&tags[1]||events[i++].kind!='S'||events[i].stringData!=0||events[i++].kind!='S'||events[i].stringData!=&tags[3]||events[i++].kind!='S'||events[i].stringData!=&tags[4]||events[i++].kind!='S') return 0; }
    if(events[i].kind!='U'||events[i++].value!=0x8000000fUL) return 0;
    if(events[i].kind!='B'||events[i++].value!=1||events[i].kind!='B'||events[i++].value!=0||events[i].kind!='B'||events[i++].value!=1||events[i].kind!='B'||events[i++].value!=0) return 0;
    if(events[i].kind!='T'||events[i].receiver!=(const void*)(const CChunkedFileSuperChunk*)&x||events[i].stream!=(const CDataOutputStream*)&stream||events[i].saver!=(const CChunkedFileSaver*)&saver) return 0;
    for(int j=0;j<nevents-1;j++) if(events[j].receiver!=(const void*)&stream||events[j].stream!=(const void*)&stream) return 0;
    return 1;
}
int main(){ if(!run(false)||!run(true)){std::puts("00ac52f0_TEST FAIL");return 1;} std::puts("00ac52f0_TEST PASS");return 0; }