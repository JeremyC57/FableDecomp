// C3DMeshFileDummyObjectChunk::GetChunkInfo() const -- append six-decimal Direction XYZ to the inherited helper-point description
// Retail @ 0x00AA9910, 263 bytes. The later donor header puts chunk-derived fields +8; retail uses a 0x2c-byte C3DMeshFileChunk base, Position +0x2c, SubMeshIndex +0x38, BoneIndex +0x3c, Name +0x40 and Direction +0x44. FormatDouble is 0x009A08B0; inherited helper-point GetChunkInfo is 0x00AA96B0.
#if !defined(_MSC_VER)
#define __fastcall
#endif
class CCharString { public:
    void* m_pStringData;
    CCharString();
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString(); // 0x0099EAE0
    static CCharString __fastcall FormatDouble(long digits, double value); // 0x009A08B0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right); // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right); // 0x0099F600
// Local virtual shell matches shared CChunkedFileChunk storage while declaring its virtual slots;
// donor 16-byte superchunk vectors conflict with the 12-byte retail vectors used here.
class CDataInputStream; class CDataOutputStream; class CChunkedFileLoader; class CChunkedFileSaver;
class CChunkedFileChunk { public:
    virtual void v00(); virtual void v01(); virtual CCharString GetChunkInfo() const;
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&);
    virtual void WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const;
    virtual CChunkedFileChunk* CreateNewInstance() const;
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk*);
    char ChunkType[5]; unsigned char _pad[3]; void* Parent;
};
class CChunkedFileSuperChunk : public CChunkedFileChunk { public:
    struct CArrayPair { void* begin; void* end; void* capacity; };
    CArrayPair SubChunks; CArrayPair RegisteredSubChunkTypes; unsigned long CurrentChunkIndex;
};
struct C3DMeshFileChunk : CChunkedFileSuperChunk {};
class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public: float row[3][4]; };
class C3DMeshFileHelperPointChunk : public C3DMeshFileChunk { public:
    C3DVector Position; long SubMeshIndex; long BoneIndex; CCharString Name;
    CCharString GetChunkInfo() const; // 0x00AA96B0
};
class C3DMeshFileDummyObjectChunk : public C3DMeshFileHelperPointChunk { public:
    C3DVector Direction; CMatrix3x4 Transform; bool UseLocalOrigin;
    CCharString GetChunkInfo() const;
};
CCharString C3DMeshFileDummyObjectChunk::GetChunkInfo() const
{
    return C3DMeshFileHelperPointChunk::GetChunkInfo() + ", Direction: (" +
        CCharString::FormatDouble(6, Direction.X) + ", " +
        CCharString::FormatDouble(6, Direction.Y) + ", " +
        CCharString::FormatDouble(6, Direction.Z);
}

#include <cstdio>
#include <cstring>
#include <cstdlib>
static int formats, baseCalls; static bool wrongPrecision;
static char buffers[64][512]; static int next_buffer; static const void* base_receiver;
static char* fresh() { char* p=buffers[next_buffer++%64]; p[0]=0; return p; }
static void append(char* p,const char* s) { std::strcat(p,s?s:""); }
CCharString::CCharString() : m_pStringData(fresh()) {}
CCharString::CCharString(const char* s,long n) : m_pStringData(fresh()) { char* p=(char*)m_pStringData; if(s&&n>0){std::strncpy(p,s,(size_t)n);p[n]=0;} }
CCharString::~CCharString() {}
CCharString CCharString::FormatDouble(long digits,double value) { ++formats; if(digits!=6) wrongPrecision=true; char temp[128]; std::sprintf(temp,"%.*f",(int)digits,value); return CCharString(temp,(long)std::strlen(temp)); }
CCharString __fastcall operator+(const CCharString& a,const CCharString& b) { char* p=fresh(); append(p,(const char*)a.m_pStringData);append(p,(const char*)b.m_pStringData); CCharString r; r.m_pStringData=p; return r; }
CCharString __fastcall operator+(const CCharString& a,const char* b) { char* p=fresh(); append(p,(const char*)a.m_pStringData);append(p,b); CCharString r; r.m_pStringData=p; return r; }
CCharString CChunkedFileChunk::GetChunkInfo() const { return CCharString("CHUNK",5); }
void CChunkedFileChunk::v00(){} void CChunkedFileChunk::v01(){} void CChunkedFileChunk::v03(){} void CChunkedFileChunk::v04(){} void CChunkedFileChunk::v05(){} void CChunkedFileChunk::v06(){}
void CChunkedFileChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){}
void CChunkedFileChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{}
CChunkedFileChunk* CChunkedFileChunk::CreateNewInstance() const { return 0; }
CChunkedFileChunk* CChunkedFileChunk::InitNewInstance(CChunkedFileChunk* p) { return p; }
CCharString C3DMeshFileHelperPointChunk::GetChunkInfo() const { ++baseCalls;base_receiver=this; return CCharString("BASE",4); }
int main() { C3DMeshFileDummyObjectChunk x; const float vals[2][3]={{-1.25f,2.5f,-3.0f},{0.0f,0.0f,0.0f}}; const char* expected[2]={"BASE, Direction: (-1.250000, 2.500000, -3.000000","BASE, Direction: (0.000000, 0.000000, 0.000000"}; for(int i=0;i<2;i++){formats=baseCalls=0;wrongPrecision=false;base_receiver=0;x.Direction.X=vals[i][0];x.Direction.Y=vals[i][1];x.Direction.Z=vals[i][2];CCharString s=x.GetChunkInfo();const char* got=(const char*)s.m_pStringData;if(formats!=3||baseCalls!=1||wrongPrecision||std::strcmp(got,expected[i])!=0||base_receiver!=(const C3DMeshFileHelperPointChunk*)&x){std::printf("00aa9910_TEST FAIL: %s\n",got);return 1;}} std::puts("00aa9910_TEST PASS");return 0; }