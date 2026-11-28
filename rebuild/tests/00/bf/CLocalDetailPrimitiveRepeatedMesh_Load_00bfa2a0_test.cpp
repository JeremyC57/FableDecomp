#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FABLE_PORTABLE
#ifndef _MSC_VER
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
#define __int64 long long
enum EIndexBufferFormat { TestIndexFormat };
enum EPrimitiveType { TestPrimitiveType };
#endif
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_REPEATED_MESH_ALLOCATIONS_PASS30
#define FABLE_RETAIL_PASS22_REPEATED_MESH_CLIP_SIMD
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/RepeatedStaticMeshSubsection.h"
#include "engine/C4DVector.h"

static void require(bool condition, const char* message)
{
    if (!condition) { fprintf(stderr, "%s\n", message); exit(1); }
}

CFailedAllocationHandler::~CFailedAllocationHandler() {}
bool CMovableResource::CanRelocateData(long) const { return true; }
unsigned long CMovableResource::Verify() const { return 123; }
class TestManager : public CEngineLandscapeMemoryManager {
public:
    bool HandleFailedAllocation(unsigned long) { return false; }
    void RelocateData(long, void*, void*, long) {}
};

struct Allocation {
    long size;
    long alignment;
    CMovableResource* owner;
    unsigned char bytes[1024];
};
static Allocation allocations[5];
static int callCount, failAt;
static void* record(long size, long alignment, CMovableResource* owner)
{
    require(callCount < 5, "unexpected extra allocation");
    Allocation& a = allocations[callCount++];
    a.size = size; a.alignment = alignment; a.owner = owner;
    memset(a.bytes, 0xcd, sizeof(a.bytes));
    return callCount == failAt ? 0 : a.bytes;
}
void* CEngineLandscapeMemoryManager::AllocVirtual(long size, CMovableResource* owner)
{
    return record(size, 0, owner);
}
void* CEngineLandscapeMemoryManager::AllocVirtualAligned(long size, long alignment, CMovableResource* owner)
{
    return record(size, alignment, owner);
}
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_LOCAL_DETAIL_RENDER_PASS29
#define FABLE_RETAIL_REPEATED_MESH_LOAD_PASS30
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#ifndef _MSC_VER
enum LOCAL_DETAIL_PRIMITIVE_TYPE { TestDetailType };
#endif
#include "engine/CLocalDetailPrimitiveRepeatedMesh.h"
#include "engine/CDataInputStream.h"
#include "fable_assert.h"

static unsigned char wire[4096];
static long length, chunkSize;
static unsigned slowReads;
static CEngineLandscapeMemoryManager* activeManager;
class CEngineLandscapeRenderer {
public:
    CEngineLandscapeMemoryManager* GetMemoryManager() { return activeManager; }
};
namespace NEngineInternal {
    static CEngineLandscapeRenderer renderer;
    CEngineLandscapeRenderer* GPLandscapeRenderer = &renderer;
}

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CDataStream::~CDataStream() {}
CDataInputStream::CDataInputStream()
    : StreamPos(0), StreamSize(length), CurrentSourcePtr(wire), SourceChunkStreamPos(0),
      SourceChunkBytesRemaining(chunkSize < length ? chunkSize : length) {}
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* out, long size)
{
    require(size > 0 && StreamPos + size <= (unsigned long)length, "stream bounds");
    ++slowReads;
    memcpy(out, wire + StreamPos, size);
    StreamPos += size;
    CurrentSourcePtr = wire + StreamPos;
    SourceChunkStreamPos = StreamPos;
    SourceChunkBytesRemaining = (long)(length - StreamPos);
    if (SourceChunkBytesRemaining > chunkSize) SourceChunkBytesRemaining = chunkSize;
}
void CDataInputStream::Close() {}
void CDataInputStream::SetPosition(unsigned long) { require(false, "unexpected seek"); }
void CDataInputStream::ReadCompressed(void*,long) { require(false, "compressed read"); }
void CDataInputStream::ReadZlibCompressed(void*,long) { require(false, "zlib read"); }
class CCharString {};
class CWideString {};
CCharString CDataInputStream::ReadNullTerminatedString() { require(false, "string read"); return CCharString(); }
CWideString CDataInputStream::ReadNullTerminatedWString() { require(false, "wide string read"); return CWideString(); }
void CDataInputStream::InternalReadNextSourceChunk(const unsigned char*&,long&) { require(false, "unexpected refill"); }
bool CDataInputStream::NeedsBufferedRead(long) { return false; }
void CDataInputStream::InternalReadUnbuffered(unsigned char*,long) { require(false, "unexpected unbuffered read"); }

void CLocalDetailPrimitiveBase::RenderUpdate(const CEngineCamera&,bool,unsigned long,const CLocalDetailObjectCollectionType*,float) {}
bool CLocalDetailPrimitiveBase::NeedsRenderUpdate() { return false; }
void CLocalDetailPrimitiveBase::OnPostRelocation() {}
void CLocalDetailPrimitiveRepeatedMesh::AddObjectsToPrimitiveRenderer() {}
LOCAL_DETAIL_PRIMITIVE_TYPE CLocalDetailPrimitiveRepeatedMesh::GetType() const { return (LOCAL_DETAIL_PRIMITIVE_TYPE)0; }
void CLocalDetailPrimitiveRepeatedMesh::RelocateData(long,void*,void*,long) {}
void CLocalDetailPrimitiveRepeatedMesh::OnPostRelocation() {}
CLocalDetailPrimitiveRepeatedMesh::CLocalDetailPrimitiveRepeatedMesh(const CLocalDetailObjectCollectionType*, CLocalDetailCacheMap*)
    : MaxObjectScale(-1), ObjectCount(-1), SubSectionsCount(-1), SubSections(0),
      ObjectMatricies(0), ObjectOffsets(0), WindDelayArray(0), LandscapeNormalArray(0) {}

struct Request {
    char member;
    long bytes, alignment, position, data, copied;
};
static Request expected[5];
static int requestCount;
static long expectedCount, expectedSubSections, sectionCountReadAt;
static long lastPosition;
static const float maxScale = 1.375f;
static const void* assigned(char field)
{
    for (int i = 0; i < callCount; ++i)
        if (expected[i].member == field && i+1 != failAt) return allocations[i].bytes;
    return 0;
}
CLocalDetailPrimitiveRepeatedMesh::~CLocalDetailPrimitiveRepeatedMesh()
{
    require(memcmp(&BoundingBox, wire, sizeof(BoundingBox)) == 0, "bounding box");
    require(memcmp(&BoundingSphere, wire+sizeof(BoundingBox), sizeof(BoundingSphere)) == 0, "bounding sphere");
    require(ObjectCount == expectedCount && MaxObjectScale == maxScale, "scale or object count");
    require(SubSectionsCount == (lastPosition >= sectionCountReadAt ? expectedSubSections : -1), "subsection count");
    require(ObjectMatricies == assigned('m') && ObjectOffsets == assigned('o'), "matrix or offset destination");
    require(WindDelayArray == assigned('w') && LandscapeNormalArray == assigned('n') && SubSections == assigned('s'),
            "optional destination");
}

bool CLocalDetailPrimitiveRepeatedMesh::Load(CDataInputStream& stream)
{
    FABLE_RETAIL_ASSERT(!ObjectOffsets && !ObjectMatricies && !LandscapeNormalArray && !SubSections && !WindDelayArray);
    stream.Read(&BoundingBox, sizeof(BoundingBox));
    stream.Read(&BoundingSphere, sizeof(BoundingSphere));
    ObjectCount = stream.ReadSLONG();
    FABLE_RETAIL_ASSERT(ObjectCount > 0);
    MaxObjectScale = stream.ReadFloat();
    long aligned_object_count = (ObjectCount + 3) & ~3L;
    CEngineLandscapeMemoryManager* manager = NEngineInternal::GPLandscapeRenderer->GetMemoryManager();
    ObjectMatricies = manager->AllocArrayOfObjectsNoConstuctors<C4DVector>(ObjectCount, this);
    if (!ObjectMatricies)
        return false;
    ObjectOffsets = manager->AllocAlignedArrayOfObjectsNoConstuctors<C4DVector>(aligned_object_count, 16, this);
    if (!ObjectOffsets)
        return false;
    stream.Read(ObjectMatricies, ObjectCount * sizeof(C4DVector));
    stream.Read(ObjectOffsets, ObjectCount * sizeof(C4DVector));
    if (stream.ReadEBOOL()) {
        long long overflow_check = static_cast<long long>(aligned_object_count) * 3;
        if (overflow_check < 0 || overflow_check > 0x7fffffff) {
            FABLE_RETAIL_ASSERT(false);
            return false;
        }
        LandscapeNormalArray = manager->AllocAlignedArrayOfObjectsNoConstuctors<float>(aligned_object_count * 3, 16, this);
        if (!LandscapeNormalArray)
            return false;
        stream.Read(LandscapeNormalArray, aligned_object_count * 3 * sizeof(float));
    }
    if (stream.ReadEBOOL()) {
        WindDelayArray = manager->AllocArrayOfObjectsNoConstuctors<unsigned char>(ObjectCount, this);
        if (!WindDelayArray)
            return false;
        stream.Read(WindDelayArray, ObjectCount);
    }
    if (stream.ReadEBOOL()) {
        SubSectionsCount = stream.ReadSLONG();
        FABLE_RETAIL_ASSERT(SubSectionsCount > 0);
        long subsection_size = SubSectionsCount * sizeof(NEnginePrimitiveManagerRepeatedStaticMeshes::CSubsectionElement);
        SubSections = manager->AllocAlignedArrayOfObjectsNoConstuctors<NEnginePrimitiveManagerRepeatedStaticMeshes::CSubsectionElement>(SubSectionsCount, 16, this);
        if (!SubSections)
            return false;
        stream.Read(SubSections, subsection_size);
    }
    return true;
}
static void append(const void* bytes, long n)
{
    require(n >= 0 && length+n <= (long)sizeof(wire), "fixture capacity");
    memcpy(wire+length, bytes, n); length += n;
}
static void pattern(long size, unsigned seed)
{
    for (long i = 0; i < size; ++i) {
        unsigned char b = (unsigned char)(seed + 7*i);
        append(&b, 1);
    }
}
static void flag(bool active)
{
    unsigned char b = active ? 0xf3 : 0;
    append(&b, 1);
}
static Request& expect(char member, long size, long align)
{
    Request& r = expected[requestCount++];
    r.member = member; r.bytes = size; r.alignment = align;
    r.position = length; r.data = 0; r.copied = 0;
    return r;
}
static void payload(Request& r, long size, unsigned seed)
{
    r.data = length; r.copied = size; pattern(size, seed);
}
static void prepare(long count, unsigned mask)
{
    requestCount = 0; length = 0; expectedCount = count;
    expectedSubSections = 2; sectionCountReadAt = 0x7fffffffL;
    pattern(24, 3); pattern(16, 29); append(&count, sizeof(count)); append(&maxScale, sizeof(maxScale));
    if (count < 0 || count > 0x7fffffffL/16) return;
    const long aligned = (count+3)&~3L;
    Request& m = expect('m', count*16, 0);
    Request& o = expect('o', aligned*16, 16);
    payload(m, count*16, 45); payload(o, count*16, 73);
    flag((mask&1)!=0);
    if (mask&1) { Request& n = expect('n', aligned*12, 16); payload(n, aligned*12, 97); }
    flag((mask&2)!=0);
    if (mask&2) { Request& w = expect('w', count, 0); payload(w, count, 131); }
    flag((mask&4)!=0);
    if (mask&4) {
        append(&expectedSubSections, sizeof(expectedSubSections));
        sectionCountReadAt = length;
        Request& s = expect('s', expectedSubSections*80, 16); payload(s, expectedSubSections*80, 187);
    }
}
static void runFixture(int failure)
{
    callCount = 0; failAt = failure; slowReads = 0;
    TestManager manager; activeManager = &manager;
    CDataInputStream stream;
    CLocalDetailPrimitiveRepeatedMesh mesh(0, 0);
    bool result = mesh.Load(stream);
    lastPosition = (long)stream.GetPosition();
    require(result == (requestCount != 0 && !failure), "return value");
    require(callCount == (failure ? failure : requestCount), "allocation count");
    require(lastPosition == (failure ? expected[failure-1].position : length), "consumed bytes");
    if (chunkSize < 16) require(slowReads != 0, "chunk-overflow path");
    if (chunkSize >= length) require(slowReads == 0, "contiguous path");
    for (int j = 0; j < callCount; ++j) {
        const Request& r = expected[j]; const Allocation& a = allocations[j];
        require(a.owner == &mesh && a.size == r.bytes && a.alignment == r.alignment, "allocator contract");
        long used = ((j+1 != failure && r.data+r.copied <= lastPosition) ? r.copied : 0);
        require(memcmp(a.bytes, wire+r.data, used) == 0, "allocated payload");
        for (long i = used; i < (long)sizeof(a.bytes); ++i) require(a.bytes[i] == 0xcd, "unexpected copy into padding");
    }
}
int main()
{
    const long counts[] = { 0L, 1L, 3L, 4L, 5L, 8L, -1L, 0x08000000L };
    const long chunks[] = { 1L, 15L, 24L, 4096L };
    for (unsigned n = 0; n < sizeof(counts)/sizeof(counts[0]); ++n)
        for (unsigned mask = 0; mask < 8; ++mask) {
            prepare(counts[n], mask);
            for (unsigned c = 0; c < sizeof(chunks)/sizeof(chunks[0]); ++c) {
                chunkSize = chunks[c];
                for (int failure = 0; failure <= requestCount; ++failure) runFixture(failure);
            }
        }
    puts("00bfa2a0_TEST PASS");
    return 0;
}