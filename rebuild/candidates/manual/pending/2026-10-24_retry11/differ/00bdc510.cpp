// Grade: DIFFER(1118v1134) on the real span -- retail_asm PENDING OWNER DECISION (19-instruction SSE AABB block > RETAIL_ASM_MAX 8). Fresh byte-level reconstruction (the landed source was a paraphrase): prologue, camera/box homes, the SSE block, the x87 clamp fallback, the range compare, the BeginReadDataAutoAlloc arm, IsFinished/PeekData/CMemoryDataInputStream, the count range check, both UpdateForeground loops, the not-near arm and delete[] now match; residues: (1) the patch count multiply loads PatchColumns first where retail loads PatchRows (`mov edi,[esi+10h] ; imul edi,[esi+0Ch]`), either source order gives the same; (2) the zero-count allocation: retail keeps `count*0x48+4` inside the non-zero arm and pushes a constant 4 in the zero arm with ONE shared placement/vector-ctor expansion -- a ternary over the allocation (this file) hoists the lea pair, a ternary over two new[] expressions duplicates the expansion, an inline operator new[] testing size does not fold to `test edi,edi`; (3) in the Initialise/LoadHeader loop retail loads ForegroundPatches before the argument pushes and the index after, ours computes the index first (indexing, pointer-arithmetic and accessor forms tried). 7 grades (A..G in $S/w22/retry11/var/bdc).
// 0x00BF36C0 is modelled as CEngineLandscapePatch::AllocateArray (catalogued operator new[]); 0x00993B10 is ~CMemoryDataInputStream (catalogued CChunkedFileChunk ctor); 0x009D68A0 DiscardAsyncData takes the counted pointer.
// Leaf: ?UpdateForeground@CEngineLandscapeMap@@
void* __cdecl operator new[](unsigned int size);     // 0x00BFEB22 (MSVCR71 ??_U@YAPAXI@Z)
void __cdecl operator delete[](void* p);              // 0x00BFEB1C (MSVCR71 ??_V@YAXPAX@Z)
void __cdecl operator delete(void* p);                // 0x00BFE9BC (MSVCR71 ??3@YAXPAX@Z)

enum LS_LOADING_STATUS { LS_LOADING_READY = 1, LS_LOADING_PENDING = 2, LS_LOADING_FAILED = 3 };
enum EThreadedFilePriority { THREADED_FILE_PRIORITY_DEFAULT = 3 };
extern EThreadedFilePriority g_LandscapeReadPriority;          // 0x0139A728
extern unsigned char g_SSEAvailable;                            // 0x013D2880

struct C3DVector { float X, Y, Z; };
struct CBox { C3DVector Min; C3DVector Max; };

struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> struct CCountedPointer {
    T* Data;                                       // +0
    SCountBlock* Info;                             // +4
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->m_references; }
    ~CCountedPointer();                            // 0x009A4400
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x009A4440
    T* operator->() const { return Data; }
    __forceinline void Release()
    {
        if (Info) {
            if (--Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Info = 0;
        Data = 0;
    }
};

class CMovableResource { public: virtual void v00(); };
class CBankFileAsyncData {
public:
    unsigned char _pad[0x10];
    unsigned long DataSize;                        // +0x10
    bool IsFinished();                             // 0x009D54E0
    void* PeekData();                              // 0x009D5530
};
void __fastcall DiscardAsyncData(CCountedPointer<CBankFileAsyncData>& async);   // 0x009D68A0
class CBankFileAsyncEntry {
public:
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long start, unsigned long size, EThreadedFilePriority priority);   // 0x009D6100
};
class CMemoryDataInputStream {
public:
    unsigned char _body[0x1c];
    CMemoryDataInputStream(const void* data, long size);   // 0x00994930
    ~CMemoryDataInputStream();                             // 0x00993B10
};
class CEngineLandscapeMap;
class CEngineLandscapePatch {                              // 0x48 bytes, polymorphic
public:
    CEngineLandscapePatch();                               // 0x00BF3A10
    virtual ~CEngineLandscapePatch();
    static void* __fastcall AllocateArray(unsigned int size, CMovableResource* owner);   // 0x00BF36C0 (catalogued as operator new[])
    static __forceinline void* operator new[](unsigned int, void* memory) { return memory; }
    static __forceinline void operator delete[](void* p, CMovableResource*) { ::operator delete[](p); }
    static __forceinline void operator delete[](void* p) { ::operator delete[](p); }
    unsigned char _body[0x48 - 4];
    void Initialise(CEngineLandscapeMap* map, long x, long y);                 // 0x00BF3700
    void LoadHeader(CMemoryDataInputStream& input);                            // 0x00BF3A90
    LS_LOADING_STATUS UpdateForeground(CCountedPointer<CBankFileAsyncEntry> bankEntry);   // 0x00BF4220
};
struct CBackgroundTree { unsigned char _pad[0xa8]; CBox Box; };
struct CCamera { unsigned char _pad[0x54]; C3DVector Position; };
extern CCamera* g_pCamera;                                 // 0x01436EA0
struct CLandscapeSettings { unsigned char _pad[0x618]; float ForegroundRange; };
extern CLandscapeSettings* g_pLandscapeSettings;           // 0x01436EA8

static __forceinline float Square(float value) { return value * value; }
static __forceinline bool IsValidCount(__int64 count) { return count >= 0 && count <= 0x7fffffff; }

static __forceinline float GetSquaredDistanceToBox(const CBox& box, const C3DVector& point)
{
    if (g_SSEAvailable) {
        float result;
        // Retail-asm: 0x00BDC54A..0x00BDC592 (and 0x00BF47A6 in CEngineLandscapePatch::RenderForeground): the box and point
        // pointers are stored to [esp+14h]/[esp+10h] and reloaded into ecx/edx, then movss/movhps loads, maxps/minps clamp,
        // subps/mulps, two shufps/add steps and the result through a stack float. The x87 arm below is the C++ fallback.
        __asm {
            mov ecx, box
            mov edx, point
            movss xmm0, dword ptr [ecx+8]
            movhps xmm0, qword ptr [ecx]
            movss xmm1, dword ptr [ecx+20]
            movhps xmm1, qword ptr [ecx+12]
            movss xmm2, dword ptr [edx+8]
            movhps xmm2, qword ptr [edx]
            movaps xmm3, xmm2
            maxps xmm2, xmm0
            minps xmm2, xmm1
            subps xmm2, xmm3
            mulps xmm2, xmm2
            movaps xmm3, xmm2
            shufps xmm2, xmm2, 0xB0
            addps xmm2, xmm3
            shufps xmm2, xmm2, 3
            addss xmm2, xmm3
            movss result, xmm2
        }
        return result;
    }
    C3DVector closest;
    closest.X = point.X;
    closest.Y = point.Y;
    closest.Z = point.Z;
    if (closest.X < box.Min.X) closest.X = box.Min.X; else if (closest.X > box.Max.X) closest.X = box.Max.X;
    if (closest.Y < box.Min.Y) closest.Y = box.Min.Y; else if (closest.Y > box.Max.Y) closest.Y = box.Max.Y;
    if (closest.Z < box.Min.Z) closest.Z = box.Min.Z; else if (closest.Z > box.Max.Z) closest.Z = box.Max.Z;
    C3DVector delta;
    delta.X = point.X - closest.X;
    delta.Y = point.Y - closest.Y;
    delta.Z = point.Z - closest.Z;
    return delta.X * delta.X + delta.Y * delta.Y + delta.Z * delta.Z;
}

class CEngineLandscapeMap : public CMovableResource {
public:
    CBackgroundTree*                     BackgroundTree;          // +0x04
    CEngineLandscapePatch*               ForegroundPatches;       // +0x08
    long                                 PatchColumns;            // +0x0c
    long                                 PatchRows;               // +0x10
    unsigned char                        _pad14[4];
    CCountedPointer<CBankFileAsyncEntry> BankEntry;               // +0x18
    unsigned char                        _pad20[0x0c];
    unsigned long                        ForegroundBlockOffset;   // +0x2c
    unsigned char                        _pad30[8];
    CCountedPointer<CBankFileAsyncData>  ForegroundAsync;         // +0x38
    unsigned char                        _pad40[0x1c];
    bool                                 UpdateForegroundActive;  // +0x5c
    bool                                 KeepForegroundLoaded;    // +0x5d
    LS_LOADING_STATUS UpdateForeground();
    __forceinline LS_LOADING_STATUS UpdateForegroundInternal();
    __forceinline CEngineLandscapePatch& GetPatch(long column, long row) { return ForegroundPatches[PatchColumns * row + column]; }
};

__forceinline LS_LOADING_STATUS CEngineLandscapeMap::UpdateForegroundInternal()
{
    const C3DVector& cameraPosition = g_pCamera->Position;
    if (BackgroundTree != 0 &&
        GetSquaredDistanceToBox(BackgroundTree->Box, cameraPosition) < Square(g_pLandscapeSettings->ForegroundRange))
    {
        if (ForegroundPatches == 0 && BankEntry.Data != 0) {
            if (ForegroundAsync.Data == 0) {
                if (ForegroundBlockOffset == 0)
                    return LS_LOADING_READY;
                unsigned long size = PatchRows * PatchColumns * 0x24;
                ForegroundAsync = BankEntry->BeginReadDataAutoAlloc(ForegroundBlockOffset, size, g_LandscapeReadPriority);
                return LS_LOADING_PENDING;
            }
            if (!ForegroundAsync->IsFinished())
                return LS_LOADING_PENDING;
            CMemoryDataInputStream input(ForegroundAsync->PeekData(), ForegroundAsync->DataSize);
            unsigned long count = PatchRows * PatchColumns;
            void* memory = count != 0 ? CEngineLandscapePatch::AllocateArray(count * sizeof(CEngineLandscapePatch) + sizeof(long), this)
                                      : ::operator new[](count * sizeof(CEngineLandscapePatch) + sizeof(long));
            ForegroundPatches = new (memory) CEngineLandscapePatch[count];
            if (ForegroundPatches == 0)
                return LS_LOADING_FAILED;
            __int64 total = PatchColumns * PatchRows;
            if (total < 0 || total > 0x7fffffff)
                return LS_LOADING_FAILED;
            for (long row = 0; row < PatchRows; ++row) {
                for (long column = 0; column < PatchColumns; ++column) {
                    ForegroundPatches[row * PatchColumns + column].Initialise(this, column << 4, row << 4);
                    ForegroundPatches[row * PatchColumns + column].LoadHeader(input);
                }
            }
            ForegroundAsync.Release();
        }
        if (ForegroundPatches != 0) {
            LS_LOADING_STATUS result = LS_LOADING_READY;
            for (long row = 0; row < PatchRows; ++row) {
                for (long column = 0; column < PatchColumns; ++column) {
                    LS_LOADING_STATUS status = ForegroundPatches[row * PatchColumns + column].UpdateForeground(BankEntry);
                    if (status == LS_LOADING_FAILED)
                        return LS_LOADING_FAILED;
                    if (status == LS_LOADING_PENDING)
                        result = status;
                }
            }
            return result;
        }
        return LS_LOADING_READY;
    }
    if (ForegroundAsync.Data != 0)
        DiscardAsyncData(ForegroundAsync);
    if (ForegroundPatches != 0) {
        if (KeepForegroundLoaded) {
            for (long row = 0; row < PatchRows; ++row)
                for (long column = 0; column < PatchColumns; ++column)
                    ForegroundPatches[row * PatchColumns + column].UpdateForeground(BankEntry);
            return LS_LOADING_READY;
        }
        delete[] ForegroundPatches;
        ForegroundPatches = 0;
    }
    return LS_LOADING_READY;
}

LS_LOADING_STATUS CEngineLandscapeMap::UpdateForeground()
{
    UpdateForegroundActive = true;
    LS_LOADING_STATUS status = UpdateForegroundInternal();
    UpdateForegroundActive = false;
    return status;
}
