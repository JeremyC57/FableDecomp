// Leaf: ??1CModeSitBase@
// NCreatureMode::CModeSitBase::~CModeSitBase() -- this class's vtable (0x01285698), the counted action pointer at +0x24/+0x28 released (the info block's count decremented, its deleter called on its data and the block freed @ 0x00BFE9BC when the count reaches zero, both words cleared), then the inline CModeBase destructor (vtable 0x012673CC, the name string at +0xc @ 0x0099EAE0, the CAnimFilterBase member at +8 with its vtable 0x0122FFCC)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D71D0, length 81 bytes.
// Callees: the info block's deleter through its +4 slot (`__fastcall`, the data at +8 in ecx), operator delete @ 0x00BFE9BC, CCharString::~CCharString @ 0x0099EAE0. The counted release re-reads the member for every access (`dec [eax] ; mov eax,[esi+28h] ; cmp [eax],0`), the landed CCountedPointer shape. Shape: `this` in esi, `ret`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
struct CAnimFilterBase {                           // vtable 0x0122FFCC
    virtual void v00();
    ~CAnimFilterBase() { }                         // inline: the vtable store
};
struct CCreatureActionBase;
struct CCPPointerInfo {                            // the counted pointer's info block
    long                 m_count;                  // +0x00
    void (__fastcall* m_pfnDelete)(void* data);    // +0x04
    void*                m_pData;                  // +0x08
};
struct CCountedActionPointer {                     // CCountedPointer<CCreatureActionBase>: Data, Info
    CCreatureActionBase* m_pData;
    CCPPointerInfo*      m_pInfo;
    ~CCountedActionPointer()
    {
        if (m_pInfo) {
            --m_pInfo->m_count;
            if (m_pInfo->m_count == 0) {
                m_pInfo->m_pfnDelete(m_pInfo->m_pData);
                delete m_pInfo;
            }
        }
        m_pData = 0;
        m_pInfo = 0;
    }
};
struct CThing;

namespace NCreatureMode {
struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    __forceinline virtual ~CModeBase() { }
    CThing*         m_pThing;                      // +0x04
    CAnimFilterBase m_filter;                      // +0x08
    CCharString     m_name;                        // +0x0c
};

struct CModeSitBase : CModeBase {                  // vtable 0x01285698
    unsigned char         _pad10[0x14];            // +0x10 .. +0x24
    CCountedActionPointer m_pendingAction;         // +0x24
    virtual ~CModeSitBase();
};

CModeSitBase::~CModeSitBase()
{
}
}
