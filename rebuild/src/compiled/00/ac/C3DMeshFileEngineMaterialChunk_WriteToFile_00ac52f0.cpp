// C3DMeshFileEngineMaterialChunk::WriteToFile(CDataOutputStream&, CChunkedFileSaver&) -- retail @ 0x00AC52F0, 256 bytes; stream type, material values and optional map filenames, then inherit the subchunk writer. Retained CChunkedFileChunk -> CChunkedFileSuperChunk -> C3DMeshFileChunk hierarchy; its retail base is 0x2c bytes though the donor's 0x10-byte counted-vector layout grows it to 0x34. Calls stream writes at 0x00993EA0/0x00993EB0/0x00993E30/0x00995AE0 and SuperChunk::WriteToFile 0x00AA78C0.
class CCharString { public: void* m_pStringData; };
#include "engine/CDataOutputStream.h"
struct CDataOutputStreamMethods : CDataOutputStream {
    void WriteULong(unsigned long value); // 0x00993EA0
    void WriteLong(long value);           // 0x00993EB0
    void WriteNullTerminatedString(const CCharString& value); // 0x00995AE0
    void WriteEBOOL(bool value); // 0x00993E30
};
class CChunkedFileSaver { public:};
class CChunkedFileLoader { public:};
class CDataInputStream;
#if defined(_MSC_VER)
#define RETAIL_FASTCALL __fastcall
#else
#define RETAIL_FASTCALL
#endif
struct SCountBlock { long references; void (RETAIL_FASTCALL* destroy)(void*); void* object; };
template<class T> class CCountedPointer { public: T* object; SCountBlock* count; };
template<class T> class CArray { public: T* begin; T* end; T* capacity; };
class CChunkedFileSuperChunk;
// The shared CChunkedFileChunk header contains an explicit vptr data member, not virtual methods.
// This equivalent 0x10-byte virtual shell is required for WriteToFile slot8 and the qualified base call.
class CChunkedFileChunk { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual void ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&);
    virtual void WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const;
    virtual CChunkedFileChunk* CreateNewInstance() const;
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk*);
    char ChunkType[5]; unsigned char _pad_09[3]; CChunkedFileSuperChunk* Parent;
};
class CChunkedFileSuperChunk : public CChunkedFileChunk { public:
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
class C3DMeshFileEngineMaterialChunk : public C3DMeshFileChunk { public:
    long MaterialIndex; CCharString Name; CMapFilenames* MapFilenames;
    unsigned long DiffuseMapBankIndex, BumpMapBankIndex, ReflectionMapBankIndex, IlluminationMapBankIndex;
    unsigned long MapFlags, SelfIllumination;
    bool TwoSided, Transparent, BooleanAlpha, DegenerateTriangles;
    virtual void WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const;
};
void C3DMeshFileEngineMaterialChunk::WriteToFile(CDataOutputStream& destination, CChunkedFileSaver& saver) const
{
    CDataOutputStreamMethods& stream=(CDataOutputStreamMethods&)destination;
    stream.WriteULong(4);
    stream.WriteLong(MaterialIndex);
    stream.WriteNullTerminatedString(Name);
    stream.WriteULong(MapFlags);
    stream.WriteULong(DiffuseMapBankIndex);
    stream.WriteULong(BumpMapBankIndex);
    stream.WriteULong(ReflectionMapBankIndex);
    stream.WriteULong(IlluminationMapBankIndex);
    if (MapFilenames) {
        stream.WriteEBOOL(true);
        stream.WriteNullTerminatedString(MapFilenames->Diffuse);
        stream.WriteNullTerminatedString(MapFilenames->Bump);
        stream.WriteNullTerminatedString(MapFilenames->Reflection);
        stream.WriteNullTerminatedString(MapFilenames->Illumination);
    } else {
        stream.WriteEBOOL(false);
    }
    stream.WriteULong(SelfIllumination);
    stream.WriteEBOOL(TwoSided);
    stream.WriteEBOOL(Transparent);
    stream.WriteEBOOL(BooleanAlpha);
    stream.WriteEBOOL(DegenerateTriangles);
    CChunkedFileSuperChunk::WriteToFile(destination, saver);
}