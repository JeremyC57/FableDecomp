// CMemoryAllocatorVariableSize::NewChunk(long minSize) -- reserve and commit a new chunk (halving the request until VirtualAlloc succeeds) and link it at the head of the chunk list
// Grade: DIFFER(269v269) -- one register only: the minSize temporary of the size sum is loaded into ebp before `push esi` (`mov ebp,[esp+28h] ; add esi,ebp`); retail loads it into edi after `push edi` (`mov edi,[esp+30h] ; add esi,edi`). Levers that closed the rest this session: `regionSize = 0;` at the loop top, the protect value as a named local before it (the `xor ebp,ebp ; push ebp` placement), memory/regionSize declared after the size clamp and before allocSize. Respelled to the Ego_r.pdb nested CMemoryAreaChunk and protected NewChunk. Tried: size expression orders, long size, minSize copy, other declaration orders.
// Leaf: ?NewChunk@CMemoryAllocatorVariableSize@@IAEPAVCMemoryAreaChunk@1@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A65720, 269 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb ?NewChunk@CMemoryAllocatorVariableSize@@IAEPAVCMemoryAreaChunk@1@J@Z (protected; CMemoryAreaChunk and
//   CMemoryArea are nested classes of CMemoryAllocatorVariableSize).
// Callees: VirtualAlloc [0x0143FD44] / VirtualQuery [0x0143FE80] (kernel32 imports), CMemoryAreaChunk::Init(void*, long,
//   CMemoryAllocatorVariableSize*) 0x00A65410 (Ego_r QAE), the out-of-memory hook 0x009F1F00 (an empty `ret`; bound as
//   CGlobalMemoryManager::OnOutOfMemory on the global 0x013CAA90 -- both descriptive, no PDB data/function name found).
// Globals: g_CurrentAlignment 0x013D28D4 / g_CurrentAlignmentShift 0x013D28D8 (descriptive, the landed Init 0x00A65410 spelling).
// Layout: Ego_r.pdb CMemoryAllocatorBase (vptr, CPerformanceStats, Alignment +0xC14, AlignmentShift +0xC18) and
//   CMemoryAllocatorVariableSize (PFirstChunk +0xC1C, MinChunkSize +0xC20, Flags +0xC28 ...), CMemoryAreaChunk 0x34.
extern unsigned long g_CurrentAlignment;        // 0x013D28D4
extern unsigned long g_CurrentAlignmentShift;   // 0x013D28D8
// Round a header size up to the current alignment: ((size + align - 1) >> shift) << shift.
#define ALIGNED_SIZE(size) ((((size) + g_CurrentAlignment - 1) >> g_CurrentAlignmentShift) << g_CurrentAlignmentShift)
struct MEMORY_BASIC_INFORMATION {               // winnt.h, 0x1c bytes
    void*         BaseAddress;
    void*         AllocationBase;
    unsigned long AllocationProtect;
    unsigned long RegionSize;                   // +0x0c
    unsigned long State;
    unsigned long Protect;
    unsigned long Type;
};
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void* address, unsigned long size, unsigned long type, unsigned long protect);   // [0x0143FD44]
extern "C" __declspec(dllimport) unsigned long __stdcall VirtualQuery(const void* address, MEMORY_BASIC_INFORMATION* info, unsigned long length);   // [0x0143FE80]
class CGlobalMemoryManager {
public:
    void OnOutOfMemory();                       // 0x009F1F00 (empty in retail)
};
extern CGlobalMemoryManager* g_pGlobalMemoryManager;   // 0x013CAA90

class CMemoryAllocatorBase {                    // Ego_r.pdb: 0xc1c bytes
public:
    virtual ~CMemoryAllocatorBase();
    unsigned char     PerformanceStats[0xc10];  // +0x04 CPerformanceStats
    unsigned long     Alignment;                // +0xc14
    unsigned long     AlignmentShift;           // +0xc18
};

class CMemoryAllocatorVariableSize : public CMemoryAllocatorBase {   // Ego_r.pdb: 0xc38 bytes
public:
    class CMemoryArea;
    class CMemoryAreaChunk {                    // Ego_r.pdb: 52 bytes
    public:
        long              TotalDataSize;        // +0x00
        long              NoFreeAreas;          // +0x04
        long              NoAllocatedAreas;     // +0x08
        long              FreeDataSize;         // +0x0c
        long              LargestFreeAreaSize;  // +0x10
        long              SmallestFreeAreaSize; // +0x14
        CMemoryAllocatorVariableSize* PParentAllocator;   // +0x18
        bool              Permanent;            // +0x1c
        bool              DemandPaged;          // +0x1d
        CMemoryAreaChunk* PNextChunk;           // +0x20
        CMemoryAreaChunk* PPreviousChunk;       // +0x24
        CMemoryArea*      PFirstFreeArea;       // +0x28
        CMemoryArea*      PLastFreeArea;        // +0x2c
        CMemoryArea*      PFirstAllocatedArea;  // +0x30
        void Init(void* memory, long size, CMemoryAllocatorVariableSize* parent);   // 0x00A65410
    };
    class CMemoryArea {                         // Ego_r.pdb: 32 bytes
    public:
        CMemoryArea*      PNextArea;            // +0x00
        CMemoryArea*      PRightChild;          // +0x04
        CMemoryArea*      PPreviousArea;        // +0x08
        CMemoryArea*      PParentArea;          // +0x0c
        CMemoryArea*      PLeftChild;           // +0x10
        CMemoryAreaChunk* PParentChunk;         // +0x14
        long              DataSize;             // +0x18
        unsigned char     Colour;               // +0x1c
        unsigned char     ID;                   // +0x1d
    };
    CMemoryAreaChunk* PFirstChunk;              // +0xc1c
    long              MinChunkSize;             // +0xc20
    long              NoFreesSinceConsolidation;   // +0xc24
    unsigned long     Flags;                    // +0xc28 (bit 1: read-only pages)
    unsigned long     InternalFlags;            // +0xc2c
    unsigned long     ExtraMemUsedByOS;         // +0xc30
    CMemoryArea*      PTreeRoot;                // +0xc34
protected:
    CMemoryAreaChunk* NewChunk(long minSize);
};

CMemoryAllocatorVariableSize::CMemoryAreaChunk* CMemoryAllocatorVariableSize::NewChunk(long minSize)
{
    unsigned long size = ALIGNED_SIZE(sizeof(CMemoryAreaChunk)) + minSize + ALIGNED_SIZE(sizeof(CMemoryArea));
    if (size <= (unsigned long)MinChunkSize)
        size = MinChunkSize;
    void* memory = 0;
    unsigned long regionSize = 0;
    long allocSize = ((Alignment + size - 1) >> AlignmentShift) << AlignmentShift;
    while (memory == 0 && allocSize >= minSize) {
        unsigned long protect = ((unsigned char)Flags & 2) ? 2 : 4;
        regionSize = 0;
        memory = VirtualAlloc(0, allocSize, 0x103000, protect);
        if (memory) {
            MEMORY_BASIC_INFORMATION info;
            VirtualQuery(memory, &info, sizeof(info));
            regionSize = info.RegionSize;
        }
        allocSize /= 2;
    }
    if (!memory) {
        g_pGlobalMemoryManager->OnOutOfMemory();
        return 0;
    }
    CMemoryAreaChunk* chunk = (CMemoryAreaChunk*)memory;
    chunk->Init((char*)memory + ALIGNED_SIZE(sizeof(CMemoryAreaChunk)), regionSize - ALIGNED_SIZE(sizeof(CMemoryAreaChunk)), this);
    chunk->PNextChunk = PFirstChunk;
    if (PFirstChunk)
        PFirstChunk->PPreviousChunk = chunk;
    chunk->PPreviousChunk = 0;
    PFirstChunk = chunk;
    return chunk;
}
