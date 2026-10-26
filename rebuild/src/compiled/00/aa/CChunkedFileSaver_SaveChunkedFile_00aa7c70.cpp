// CChunkedFileSaver::SaveChunkedFile(CChunkedFile&, CDataOutputStream&) -- serialize header, root chunk, patched body length and terminator
// Retail 0x00AA7C70, 305 bytes. Layouts match PDB CChunkedFile (20 bytes), saver (12 bytes).
// Genuine stream slots: dtor0, SetPosition1, GetPosition2, Close3, Write4 (CMemoryDataOutputStream RTTI 0x0129A788).
// Genuine chunk slots from retail vtable0x0129E474: dtor,description,info,isSuper,isUnknown,const/nonconst GetAsSuper,read,write,create,init.
#include "fable_string.h"
#define CDataOutputStream CDataOutputStream_PDB
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream
#define CChunkedFileChunk CChunkedFileChunk_PDB
#include "engine/CChunkedFileChunk.h"
#undef CChunkedFileChunk
struct CRetailString : CCharString { CRetailString(const CCharString& s); }; // native copy0x0099EC30
class CChunkedFileSaver; class CDataInputStream; class CChunkedFileLoader;
class CDataOutputStream { public:
    virtual ~CDataOutputStream();
    virtual void SetPosition(unsigned long position)=0;
    virtual unsigned long GetPosition() const=0;
    virtual void Close()=0;
    virtual void Write(const void* data,long count)=0;
    CCharString NewLine;
    void WriteNullTerminatedString(const CCharString& value); // 0x00995AE0
public:
    void WriteULONG(unsigned long);   // 0x00993EA0
};
class CChunkedFileChunk { public:
    virtual ~CChunkedFileChunk();
    virtual CCharString GetChunkDescription() const;
    virtual CCharString GetChunkInfo() const;
    virtual int IsSuperChunk() const;
    virtual int IsUnknownType() const;
    virtual const CChunkedFileSuperChunk* GetAsSuperChunk() const;
    virtual CChunkedFileSuperChunk* GetAsSuperChunk();
    virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&)=0;
    virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const=0;
    virtual CChunkedFileChunk* CreateNewInstance() const=0;
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk*) const;
    char ChunkType[5]; CChunkedFileSuperChunk* Parent;
};
struct CountBlock { long References; void (FABLE_FASTCALL* Destroy)(void*); void* Object; };
struct ChunkPointer {
    CChunkedFileChunk* Data; CountBlock* Info;
    ChunkPointer() : Data(0), Info(0) {}
    ChunkPointer(const ChunkPointer& p) : Data(p.Data), Info(p.Info) { if(Info) ++Info->References; }
    ~ChunkPointer() { if(Info && --Info->References==0) { Info->Destroy(Info->Object); operator delete(Info); } }
    CChunkedFileChunk* operator->() const { return Data; }
};
class CChunkedFile { public:
    virtual ~CChunkedFile();
    ChunkPointer RootChunk; unsigned long Version; CCharString FileComment;
    ChunkPointer GetRootChunk() const { return RootChunk; }
};
class CChunkedFileSaver { public:
    virtual ~CChunkedFileSaver();
    char SupportedFileType[5];
    unsigned long SaveChunkedFile(CChunkedFile& file,CDataOutputStream& out);
};
unsigned long CChunkedFileSaver::SaveChunkedFile(CChunkedFile& file,CDataOutputStream& out) {
    unsigned long start=out.GetPosition();
    out.Write(">>>>",4); out.Write(SupportedFileType,4); out.WriteULONG(file.Version);
    { CRetailString comment(file.FileComment); out.WriteNullTerminatedString(comment); }
    out.Write(file.GetRootChunk()->ChunkType,4);
    unsigned long lengthPosition=out.GetPosition(); out.WriteULONG(0);
    unsigned long contentPosition=out.GetPosition();
    file.GetRootChunk()->WriteToFile(out,*this);
    unsigned long end=out.GetPosition();
    out.SetPosition(lengthPosition); out.WriteULONG(end-contentPosition); out.SetPosition(end);
    unsigned long zero=0; out.Write(&zero,4);
    return out.GetPosition()-start;
}