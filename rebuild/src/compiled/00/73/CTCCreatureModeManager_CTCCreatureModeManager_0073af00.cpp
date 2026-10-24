// CTCCreatureModeManager::CTCCreatureModeManager(CThing* parent) -- CTCBase(parent), empty PModes list and AvailableIdleModes vector, TimeForNextIdleMode = 30 s of frames, then AddMode(0)
// Leaf: ??0CTCCreatureModeManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073AF00, 87 bytes (+9 int3).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (the STLport node allocator), __ftol2 @ 0x00BFEA70,
//          CTCCreatureModeManager::AddMode(long) @ 0x0073AE10 (landed).
// Globals: g_framesPerSecond @ 0x01375550 (long), 30.0f @ 0x012673C4.
// Label: the manifest's "CEngineSubPrimitive2DClockSprite_ctor2" is a propagation -- the vtable stored, 0x01268854, is
// CTCCreatureModeManager in rtti-vtables.tsv (slot 9 = 0x0073A880), and the callee 0x0073AE10 is its landed AddMode.
// Layout (retail, STLport containers): PModes list +0x0C (one sentinel pointer; 0x10-byte nodes = links + an 8-byte
// CCountedPointer<CModeBase>), TimeLeftForCurrentMode +0x10 (not initialised here), TimeForNextIdleMode +0x14,
// AvailableIdleModes vector<NCreatureMode::EMode> +0x18 (12 bytes). PDB offsets run high (Dinkumware list 12 / vector 16).
// Shape: the list head is nulled by the alloc-proxy initialiser, then the sentinel node is malloc'd and linked to itself
// before it is stored; the vector's three zero words share the zero register (edi) with the AddMode(0) argument; the
// frame count is converted AFTER the vector zero stores, so it is a body assignment, not an initialiser.
struct CThing;
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern long g_framesPerSecond;                          // 0x01375550

template <class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;                                         // +0x00
    CRetailList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

template <class T> struct CRetailVector {
    T* m_start;                                         // +0x00
    T* m_finish;                                        // +0x04
    T* m_endOfStorage;                                  // +0x08
    CRetailVector() : m_start(0), m_finish(0), m_endOfStorage(0) {}
};

struct CModeBase;
struct CModePointer {                                   // CCountedPointer<CTCCreatureModeManager::CModeBase>
    CModeBase* m_pObject;
    void* m_pCount;
};

struct CTCBase_Calls {                                  // CTCBase, 0xc bytes
    virtual void v00();
    unsigned char _body[0x8];                           // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                      // 0x00686800
};

struct CTCCreatureModeManager : CTCBase_Calls {         // vtable 0x01268854
    CRetailList<CModePointer> PModes;                   // +0x0C
    long TimeLeftForCurrentMode;                        // +0x10
    long TimeForNextIdleMode;                           // +0x14
    CRetailVector<long> AvailableIdleModes;             // +0x18
    virtual void v00();
    CTCCreatureModeManager(CThing* parent);
    bool AddMode(long modeType);                        // 0x0073AE10
};

CTCCreatureModeManager::CTCCreatureModeManager(CThing* parent)
    : CTCBase_Calls(parent)
{
    TimeForNextIdleMode = (long)(g_framesPerSecond * 30.0f);
    AddMode(0);
}