#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
typedef long long NativeSigned64;
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 NativeSigned64
#endif
extern "C" void __cdecl tracked_free(void*);
#define free tracked_free
// Host width adapter: the exact implementation below uses native 32-bit long.
#ifndef _MSC_VER
#define long int
#endif
// C3DPrimitive2::ConvertToVBFormatStatic(void*, unsigned long, unsigned long, CVertexCompressionParams*)
// Retail 0x00A92740, 295 bytes (ret 0x10). Field-free C3DPrimitive2 adapter: RepeatingMeshReps +8,
// Data +0x2c, VertexCount +0x58. CVertexShaderInput is reconstructed from the landed adjacent
// destructor: CBaseClass vptr then DataStreams CArray +4 and D3DDeclaration CArray +0x10; true
// virtual destructor chain and static vtable 0x0129DFB8. Native getter @0x00AB37B0 receives
// DataStreams.first +4 (the stream's Format subobject), as shown by A927A7's mov [local+4]; add 4.
// Empty staging uses the vector object as its native sentinel.
// Invalid wrapped 32-bit byte count skips both transform paths and conversion while preserving cleanup.
#include <string.h>

extern "C" void __cdecl free(void*); // 0x00BFEA14
class CVertexCompressionParams;
extern "C" void __fastcall ConvertStaticVertices(const void*, unsigned long, void*, unsigned long, unsigned long, CVertexCompressionParams&); // 0x00A91390

class CVertexCompressionParams { public: float PosScale[4]; float PosBias[4]; };
struct CVertexFormat { unsigned long GetVertexSize() const; }; // 0x00AB37B0
struct CStreamInfo { long Unnamed00; CVertexFormat Format; };
struct D3DVERTEXELEMENT9;
template<class T> class CArray { public:
    T* first;
    T* last;
    T* end;
};
class CBaseClass { public:
    virtual ~CBaseClass(); // vtable 0x01231710, deleting destructor 0x004371F0
    CBaseClass();           // 0x0099A2D0
};
class CVertexShaderInput : public CBaseClass { public: // vtable 0x0129EA08, size 0x1c
    CArray<CStreamInfo> DataStreams;               // +4
    CArray<D3DVERTEXELEMENT9> D3DDeclaration;      // +0x10
    virtual ~CVertexShaderInput();                 // 0x00A5EB70 (AB4690 is deleting wrapper)
    CVertexShaderInput();                          // 0x00AB3B10
};
namespace N3DPrimitive {
struct CVertexShaderInputStatic : CVertexShaderInput { // vtable 0x0129DFB8
    CVertexShaderInputStatic() { }
    virtual ~CVertexShaderInputStatic();          // 0x00A98640
    void Initialise(unsigned long, bool);          // 0x00A90560
};
}

struct CByteVector {
    unsigned char* first;
    unsigned char* last;
    unsigned char* end;
    void insert(unsigned char*, unsigned long, const unsigned char&); // 0x00485F3A
    unsigned long size() const { return (unsigned long)(last - first); }
    __forceinline void resize(unsigned long count, const unsigned char& value)
    {
        if (count < size())
            last = first + count;
        else
            insert(last, count - size(), value);
    }
    ~CByteVector() { if (first) free(first); }
};

struct CStaticVertex { unsigned char bytes[32]; };
struct CArrayStaticVertex { CStaticVertex* first; CStaticVertex* last; CStaticVertex* end; };
struct CDeviceIndependent { long pad; CArrayStaticVertex Vertices; };
class C3DPrimitive2 { public:
    unsigned char pad0[8];
    long RepeatingMeshReps;
    unsigned char pad0c[0x20];
    CDeviceIndependent* Data;
    unsigned char pad30[0x28];
    unsigned long VertexCount;
    void ConvertToVBFormatRepeatingMesh(void*); // 0x00A8F770
    void TransformVertexBufferWithNormals(void*); // 0x00A8F1B0
    void TransformVertexBuffer(void*);            // 0x00A8EFA0
    void ConvertToVBFormatStatic(void*, unsigned long, unsigned long, CVertexCompressionParams*);
};

void C3DPrimitive2::ConvertToVBFormatStatic(void* destination, unsigned long sourceFormat,
                                            unsigned long flags, CVertexCompressionParams* params)
{
    if (RepeatingMeshReps > 0) {
        ConvertToVBFormatRepeatingMesh(destination);
        return;
    }
    unsigned long convertedFormat = flags & ~4;
    CByteVector bytes;
    bytes.first = 0;
    bytes.last = 0;
    bytes.end = 0;
    void* transformed = destination;
    if (flags != convertedFormat) {
        unsigned long stride;
        {
            N3DPrimitive::CVertexShaderInputStatic input;
            input.Initialise(convertedFormat, true);
            stride = input.DataStreams.first->Format.GetVertexSize();
        }
        unsigned long count = VertexCount * stride;
        __int64 checkedCount = (__int64)count;
        if (!(checkedCount >= 0 && checkedCount <= 0x7fffffffLL))
            return;
        unsigned char zero = 0;
        bytes.resize(count, zero);
        transformed = (unsigned long)(bytes.last - bytes.first) != 0 ? (void*)bytes.first : (void*)&bytes;
    }
    {
        if (flags & 2)
            TransformVertexBufferWithNormals(transformed);
        else
            TransformVertexBuffer(transformed);
        if (flags != convertedFormat)
            ConvertStaticVertices(transformed, convertedFormat, destination, flags, VertexCount, *params);
    }
}
#undef free

static char events[64];
static int eventCount, failures;
static unsigned long strideValue;
static unsigned long expectedInitFlags;
static int initCalls, getSizeCalls, staticDtorCalls, inputDtorCalls, baseDtorCalls;
static int insertCalls, basicTransformCalls, normalTransformCalls, convertCalls, repeatCalls, freeCalls;
static unsigned char* allocated;
static C3DPrimitive2* mutatePrimitive;
static void* transformArg;
static const void* convertSource;
static void* convertDestination;
static unsigned long convertSourceFormat, convertFlags, convertVertexCount;
static CVertexCompressionParams* convertParams;
static void event(char c) { if(eventCount < 63) events[eventCount++]=c; events[eventCount]=0; }
static void reset() {
    eventCount=0; events[0]=0; initCalls=getSizeCalls=staticDtorCalls=inputDtorCalls=baseDtorCalls=0;
    insertCalls=basicTransformCalls=normalTransformCalls=convertCalls=repeatCalls=freeCalls=0;
    allocated=0; mutatePrimitive=0; transformArg=0; convertSource=0; convertDestination=0;
    convertSourceFormat=convertFlags=convertVertexCount=0; convertParams=0;
}
static void expect(bool ok) { if(!ok) ++failures; }

extern "C" void __cdecl tracked_free(void* p) {
    if(p) { ++freeCalls; event('f'); }
    std::free(p);
}

CBaseClass::CBaseClass() { event('b'); }
CBaseClass::~CBaseClass() { ++baseDtorCalls; event('z'); }
CVertexShaderInput::CVertexShaderInput() {
    DataStreams.first=DataStreams.last=DataStreams.end=0;
    D3DDeclaration.first=D3DDeclaration.last=D3DDeclaration.end=0;
    event('i');
}
CVertexShaderInput::~CVertexShaderInput() { ++inputDtorCalls; event('x'); }
N3DPrimitive::CVertexShaderInputStatic::~CVertexShaderInputStatic() { ++staticDtorCalls; event('d'); }
static CStreamInfo streamInfo;
void N3DPrimitive::CVertexShaderInputStatic::Initialise(unsigned long flags, bool useDeclaration) {
    ++initCalls; event('n');
    if(flags!=expectedInitFlags || !useDeclaration) ++failures;
    DataStreams.first=&streamInfo;
    DataStreams.last=&streamInfo+1;
    DataStreams.end=&streamInfo+1;
}
unsigned long CVertexFormat::GetVertexSize() const { ++getSizeCalls; event('g'); expect(this==&streamInfo.Format); return strideValue; }
void CByteVector::insert(unsigned char* pos, unsigned long count, const unsigned char& value) {
    ++insertCalls; event('a');
    if(pos!=last) ++failures;
    unsigned long oldSize=first ? (unsigned long)(last-first) : 0;
    if(count==0) return;
    unsigned char* p=(unsigned char*)std::realloc(first,oldSize+count);
    if(!p) { ++failures; return; }
    first=p; last=p+oldSize+count; end=last; allocated=p;
    std::memset(p+oldSize,(int)value,count);
}
void C3DPrimitive2::ConvertToVBFormatRepeatingMesh(void*) { ++repeatCalls; event('r'); }
void C3DPrimitive2::TransformVertexBuffer(void* p) { ++basicTransformCalls; transformArg=p; event('u'); }
void C3DPrimitive2::TransformVertexBufferWithNormals(void* p) { ++normalTransformCalls; transformArg=p; event('t'); if(mutatePrimitive) mutatePrimitive->VertexCount=9; }
extern "C" void __fastcall ConvertStaticVertices(const void* source, unsigned long sourceFormat,
        void* destination, unsigned long flags, unsigned long vertices, CVertexCompressionParams& params) {
    ++convertCalls; event('c'); convertSource=source; convertDestination=destination;
    convertSourceFormat=sourceFormat; convertFlags=flags; convertVertexCount=vertices; convertParams=&params;
}

static void initPrimitive(C3DPrimitive2& p, long reps, unsigned long vertices) {
    p.RepeatingMeshReps=reps; p.VertexCount=vertices;
}
static bool eventsAre(const char* expected) { return std::strcmp(events,expected)==0; }

int main() {
    CVertexCompressionParams params;
    unsigned char destination[128];
    C3DPrimitive2 primitive;

    reset(); initPrimitive(primitive,1,3); strideValue=8;
    primitive.ConvertToVBFormatStatic(destination,0,4,&params);
    expect(eventsAre("r") && repeatCalls==1 && initCalls==0 && insertCalls==0 &&
           basicTransformCalls==0 && normalTransformCalls==0 && convertCalls==0 && freeCalls==0);

    reset(); initPrimitive(primitive,0,0x30000000UL); strideValue=8; expectedInitFlags=0;
    primitive.ConvertToVBFormatStatic(destination,0,4,&params);
    expect(eventsAre("bingdxz") && initCalls==1 && getSizeCalls==1 && staticDtorCalls==1 &&
           inputDtorCalls==1 && baseDtorCalls==1 && insertCalls==0 && basicTransformCalls==0 &&
           normalTransformCalls==0 && convertCalls==0 && freeCalls==0);

    reset(); initPrimitive(primitive,0,3); strideValue=8; expectedInitFlags=2;
    primitive.ConvertToVBFormatStatic(destination,0,6,&params);
    expect(eventsAre("bingdxzatcf") && initCalls==1 && getSizeCalls==1 && insertCalls==1 &&
           normalTransformCalls==1 && basicTransformCalls==0 && transformArg==allocated &&
           convertCalls==1 && convertSource==allocated && convertDestination==destination &&
           convertSourceFormat==2 && convertFlags==6 && convertVertexCount==3 &&
           convertParams==&params && freeCalls==1);

    reset(); initPrimitive(primitive,0,2); strideValue=8; expectedInitFlags=0;
    primitive.ConvertToVBFormatStatic(destination,0,4,&params);
    expect(eventsAre("bingdxzaucf") && initCalls==1 && getSizeCalls==1 && insertCalls==1 &&
           basicTransformCalls==1 && normalTransformCalls==0 && transformArg==allocated &&
           convertCalls==1 && convertSource==allocated && convertDestination==destination &&
           convertSourceFormat==0 && convertFlags==4 && convertVertexCount==2 &&
           convertParams==&params && freeCalls==1);

    reset(); initPrimitive(primitive,0,2); strideValue=8; expectedInitFlags=0;
    primitive.ConvertToVBFormatStatic(destination,0,2,&params);
    expect(eventsAre("t") && initCalls==0 && insertCalls==0 && normalTransformCalls==1 &&
           transformArg==destination && convertCalls==0 && freeCalls==0);

    reset(); initPrimitive(primitive,0,0); strideValue=8; expectedInitFlags=0;
    primitive.ConvertToVBFormatStatic(destination,0,4,&params);
    expect(eventsAre("bingdxzauc") && transformArg!=destination && transformArg!=0 &&
           convertSource==transformArg && insertCalls==1 && freeCalls==0);
    reset(); initPrimitive(primitive,0,0x20000000UL); strideValue=8; expectedInitFlags=0;
    primitive.ConvertToVBFormatStatic(destination,0,4,&params);
    expect(eventsAre("bingdxzauc") && convertCalls==1 && freeCalls==0 && transformArg!=destination);
    reset(); initPrimitive(primitive,0,3); strideValue=8; expectedInitFlags=2;
    mutatePrimitive=&primitive;
    primitive.ConvertToVBFormatStatic(destination,0,6,&params);
    expect(convertVertexCount==9 && normalTransformCalls==1 && freeCalls==1);
    if(failures) { std::printf("00a92740_TEST FAIL %d\n",failures); return 1; }
    std::printf("00a92740_TEST PASS\n"); return 0;
}