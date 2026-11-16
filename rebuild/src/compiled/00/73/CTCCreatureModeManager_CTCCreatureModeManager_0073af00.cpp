// CTCCreatureModeManager::CTCCreatureModeManager(CThing* parent) -- CTCBase(parent), empty PModes list and AvailableIdleModes vector, TimeForNextIdleMode = 30 s of frames, then AddMode(0)
// Leaf: ??0CTCCreatureModeManager@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCCreatureModeManager@@QAE@AAVCThing@@@Z).
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073AF00, 87 bytes (+9 int3).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, malloc @ 0x00BFEA0E (the STLport node allocator), __ftol2 @ 0x00BFEA70,
//          CTCCreatureModeManager::AddMode(long) @ 0x0073AE10 (landed).
// Globals: CMainGameComponent::GetConstantFPS() @ 0x01375550 (long), 30.0f @ 0x012673C4.
// Label: the manifest's "CEngineSubPrimitive2DClockSprite_ctor2" is a propagation -- the vtable stored, 0x01268854, is
// CTCCreatureModeManager in rtti-vtables.tsv (slot 9 = 0x0073A880), and the callee 0x0073AE10 is its landed AddMode.
// Layout (retail, STLport containers): PModes list +0x0C (one sentinel pointer; 0x10-byte nodes = links + an 8-byte
// CCountedPointer<CModeBase>), TimeLeftForCurrentMode +0x10 (not initialised here), TimeForNextIdleMode +0x14,
// AvailableIdleModes vector<NCreatureMode::EMode> +0x18 (12 bytes). PDB offsets run high (Dinkumware list 12 / vector 16).
// Shape: the list head is nulled by the alloc-proxy initialiser, then the sentinel node is malloc'd and linked to itself
// before it is stored; the vector's three zero words share the zero register (edi) with the AddMode(0) argument; the
// frame count is converted AFTER the vector zero stores, so it is a body assignment, not an initialiser.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

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

class CTCCreatureModeManager : public CTCBase { public:         // vtable 0x01268854
    CRetailList<CModePointer> PModes;                   // +0x0C
    long TimeLeftForCurrentMode;                        // +0x10
    long TimeForNextIdleMode;                           // +0x14
    CRetailVector<long> AvailableIdleModes;             // +0x18
    CTCCreatureModeManager(CThing& parent);
public:
    bool AddMode(unsigned long);   // 0x0073AE10
};

CTCCreatureModeManager::CTCCreatureModeManager(CThing& parent)
    : CTCBase(parent)
{
    TimeForNextIdleMode = (long)(CMainGameComponent::GetConstantFPS() * 30.0f);
    AddMode(0);
}