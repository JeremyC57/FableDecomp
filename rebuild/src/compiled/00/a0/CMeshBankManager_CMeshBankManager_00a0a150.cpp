// CMeshBankManager::~CMeshBankManager() -- release every bank pointer held in the bank list, then destroy the list and the CBase part
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0A150, 112 bytes (padding included).
// Leaf: ??1CMeshBankManager
// Shape: the class vtables for both subobjects (+0 0x0129C98C, +4 0x0129C980) are stored, then the
// body walks MeshDataBanks (+8, STLport list, sentinel re-read every step) and sets each entry's
// CCountedPointer<CMeshDataBank> to null: inline release (count block: references +0, __fastcall
// deleter +4 run on the object +8, block freed with operator delete 0x00BFE9BC) and the two zero
// stores block-then-object. The list member's destructor follows: out-of-line clear() (0x00A0A720,
// manifest "remove_if" is a byte-twin label) and the sentinel freed when present (0x00BFEA14); the
// CBase destructor (0x0099A300) is the tail jump. Layout per the landed ctor 0x00A0A110.
extern "C" void __cdecl free(void* p);              // 0x00BFEA14

struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
};

template <class T> class CCountedPointer { public:
    T*           m_pObject;                         // +0
    SCountBlock* m_pCount;                          // +4
    CCountedPointer& operator=(T* object)           // only used with null here
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        m_pObject = object;
        return *this;
    }
};

class CMeshDataBank;

struct CMeshDataBankInfo {                          // 12 bytes
    long                           m_id;            // +0
    CCountedPointer<CMeshDataBank> PBank;           // +4
};

struct CBankInfoList {                              // STLport list<CMeshDataBankInfo>
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CMeshDataBankInfo m_value; };
    SNode* m_pHead;                                 // sentinel
    ~CBankInfoList()
    {
        clear();
        if (m_pHead)
            free(m_pHead);
    }
    void clear();                                   // 0x00A0A720
};

namespace N_0099a300 { struct CBase {
    virtual ~CBase();                               // 0x0099A300
}; } using N_0099a300::CBase;

struct CMeshBankListener {                          // second base
    virtual void v00();
};

class CMeshBankManager : public CBase, public CMeshBankListener { public:
    CBankInfoList MeshDataBanks;                    // +0x08
    virtual void v00();
    virtual ~CMeshBankManager();
};

CMeshBankManager::~CMeshBankManager()
{
    for (CBankInfoList::SNode* node = MeshDataBanks.m_pHead->m_pNext; node != MeshDataBanks.m_pHead; node = node->m_pNext)
        node->m_value.PBank = 0;
}