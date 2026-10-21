// CThreadedFileManager::CThreadedFileManager() -- build the 2x7 pending-object queues and the object map (STLport trees with malloc'd headers), the critical section and the wake event, reset the per-device state and time slices, then start the loader thread
// Leaf: ??0CThreadedFileManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098F3A0, length 350 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (base ctor; manifest "CBase"), malloc @ 0x00BFEA0E
//   (tree header nodes, 0x18 bytes), KERNEL32 InitializeCriticalSection (IAT 0x0143FEE4), CreateEventW (IAT 0x0143FDC8),
//   _beginthread @ 0x00BFED3A with the static thread procedure @ 0x0098F380. Vtable 0x0129A164.
// Layout: Ego_r.pdb CThreadedFileManager with retail 12-byte STLport maps: DeviceQueue (CPendingObjectMap[2][7], 16 bytes
//   each: map +0, CurrentPosition iterator +0xc) +0x04, CurrentDeviceAtPriorityLevel[7] +0xe4,
//   CurrentDeviceEndOfTimeSlice double[7] +0x100, CurrentlyReadingObject +0x138, Objects map +0x13c, Initialised +0x148,
//   ExitThread +0x149, ThreadFinished +0x14a, CriticalSection +0x14c (0x18 bytes), Event +0x164, NextObjectUID +0x168,
//   BlockPendingObjectProcessingRefCount +0x16c, DeviceTimeSlice float[2] +0x170. No generated engine header.
// Shape: the STLport tree ctor is the alloc-proxy base (header = 0, then malloc) plus the derived node count and
//   `_M_empty_initialize` (red header, null root, leftmost/rightmost = header, re-read after each store); the 14 queues
//   are an inline array construction loop (ebp = 14). The reset loop indexes the per-device arrays by the inner index
//   only (as retail does: both priority passes write the same seven device slots) and points every queue's
//   CurrentPosition at its map's end(). The zero end-of-slice time is a `const double&` bound to a 0.0 temporary:
//   the 8-byte temporary gives retail's `and esp,-8` aligned frame (VC7.1 still loads the value from the constant
//   pool); a plain double local or literal leaves the frame unaligned. In the inner loop the device id is cleared
//   before the time is stored (that order gives retail's edx = time array, ecx = id array).
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
extern "C" unsigned long __cdecl _beginthread(void (__cdecl* start)(void*), unsigned int stackSize, void* arg);   // 0x00BFED3A
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);   // IAT 0x0143FEE4
extern "C" __declspec(dllimport) void* __stdcall CreateEventW(void* attributes, int manualReset, int initialState, const unsigned short* name);   // IAT 0x0143FDC8

struct SRbTreeNode {                                        // 0x18 bytes: header + (key, value)
    unsigned char m_color;                                  // +0x00 (0 = red)
    SRbTreeNode*  m_pParent;                                // +0x04
    SRbTreeNode*  m_pLeft;                                  // +0x08
    SRbTreeNode*  m_pRight;                                 // +0x0c
    unsigned long m_key;                                    // +0x10
    void*         m_value;                                  // +0x14
};

struct CRbTreeBase {                                        // _Rb_tree_base: the alloc proxy holding the header
    SRbTreeNode* m_pHeader;
    CRbTreeBase() : m_pHeader(0)
    {
        m_pHeader = (SRbTreeNode*)malloc(sizeof(SRbTreeNode));
    }
};

struct CRbTree : CRbTreeBase {                              // map<K, CFileObject*>, 12 bytes in retail
    unsigned long m_nodeCount;
    unsigned char m_keyCompare;
    CRbTree() : m_nodeCount(0)
    {
        m_pHeader->m_color = 0;
        m_pHeader->m_pParent = 0;
        m_pHeader->m_pLeft = m_pHeader;
        m_pHeader->m_pRight = m_pHeader;
    }
    SRbTreeNode* end() const { return m_pHeader; }
};

struct CTreeIterator {
    SRbTreeNode* m_pNode;
    CTreeIterator() : m_pNode(0) {}
};

struct CPendingObjectMap {                                  // 16 bytes in retail
    CRbTree       Objects;                                  // +0x00
    CTreeIterator CurrentPosition;                          // +0x0c
};

struct CRITICAL_SECTION_MODEL {                             // RTL_CRITICAL_SECTION, 0x18 bytes, dword-aligned
    void*         DebugInfo;
    long          LockCount;
    long          RecursionCount;
    void*         OwningThread;
    void*         LockSemaphore;
    unsigned long SpinCount;
};

struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                                // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

struct CThreadedFileManager : CBaseClassNonCopyable {      // vtable 0x0129A164
    CPendingObjectMap      DeviceQueue[2][7];              // +0x04
    long                   CurrentDeviceAtPriorityLevel[7]; // +0xe4 EThreadedFileDeviceID
    double                 CurrentDeviceEndOfTimeSlice[7];  // +0x100
    void*                  CurrentlyReadingObject;          // +0x138
    CRbTree                Objects;                         // +0x13c
    bool                   Initialised;                     // +0x148
    bool                   ExitThread;                      // +0x149
    bool                   ThreadFinished;                  // +0x14a
    CRITICAL_SECTION_MODEL CriticalSection;                 // +0x14c
    void*                  Event;                           // +0x164
    unsigned long          NextObjectUID;                   // +0x168
    long                   BlockPendingObjectProcessingRefCount; // +0x16c
    float                  DeviceTimeSlice[2];              // +0x170
    CThreadedFileManager();
    virtual ~CThreadedFileManager();
    static void __cdecl ThreadProc(void* manager);          // 0x0098F380
};

CThreadedFileManager::CThreadedFileManager()
{
    InitializeCriticalSection(&CriticalSection);
    BlockPendingObjectProcessingRefCount = 0;
    Initialised = true;
    ThreadFinished = false;
    ExitThread = false;
    Event = CreateEventW(0, 0, 0, 0);
    NextObjectUID = 1;
    CurrentlyReadingObject = 0;
    const double& zero = 0.0;
    for (long priority = 0; priority < 2; ++priority) {
        DeviceTimeSlice[priority] = 0.2f;
        for (long device = 0; device < 7; ++device) {
            DeviceQueue[priority][device].CurrentPosition.m_pNode = DeviceQueue[priority][device].Objects.end();
            CurrentDeviceAtPriorityLevel[device] = 0;
            CurrentDeviceEndOfTimeSlice[device] = zero;
        }
    }
    _beginthread(ThreadProc, 0x3000, this);
}
