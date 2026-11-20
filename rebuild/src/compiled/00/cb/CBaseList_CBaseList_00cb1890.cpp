// CBaseList::~CBaseList() -- EH-framed destructor (strmbase wxlist.cpp): RemoveAll (@ 0x00CB1340), then the node cache member's inline destructor frees the cached nodes one by one through operator delete
// Leaf: ??1CBaseList@@QAE@XZ
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/wxlist.cpp) ??1CBaseList@@QAE@XZ -- 98 bytes verified against Fable.exe 0x00CB1890
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1890, oracle length 98 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esp+10h],0 ; call RemoveAll ; mov esi,[esi+14h] ; test esi,esi ; mov dword ptr [esp+10h],-1 ; je done ; loop: mov eax,esi ; mov esi,[esi+4] ; push eax ; call operator delete ; add esp,4 ; test esi,esi ; jne loop ; done: mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The body runs under EH state 0 (the cache member is alive); the member's inline destructor then walks and frees the nodes
// (state -1). The class stores no vtable. Retail's unwind funclet 0x012173E0 is `mov ecx,[ebp-10h] ; add ecx,0Ch ; jmp 0x00CB1210`:
// the out-of-line CBaseList::CNodeCache::~CNodeCache (landed row 0x00CB1210) on the cache member at +0xC.
// NF200 (2026-11-20, q6930): the address-named stand-ins CEngineLandscapePatch_00cb1890 / CPatchNodeList_00cb1890 / SPatchNode_00cb1890
//   respelled to the strmbase classes of the vendor proof: CBaseList (self leaf ??1CBaseList@@QAE@XZ, the spelling the landed caller
//   0x00CAF580 binds), its nested CNodeCache (the funclet now binds ??1CNodeCache@CBaseList@@QAE@XZ, the Leaf of the landed 0x00CB1210)
//   and CNode, in the shape of the landed sibling 0x00CB1210 (strmbase wxlist.h: m_pFirst / m_pLast / m_Count / m_Cache; no shared header);
//   the out-of-line release 0x00CB1340 is ?RemoveAll@CBaseList@@QAEXXZ (catalog CBaseList::RemoveAll; bound by 0x00CAF580; was the stand-in
//   ?Release@CEngineLandscapePatch_00cb1890@@QAEXXZ). wxlist.h defines ~CNodeCache in the class (inline here: this TU expands it and
//   the funclet calls the COMDAT, byte-identical to 0x00CB1210's copy). Funclet row 0x012173E0 includes this file by path.
void __cdecl operator delete(void* block);         // 0x00BFE9BC

class CBaseList {
public:
    class CNode {
        CNode* m_pPrev;                             // +0x00
        CNode* m_pNext;                             // +0x04
        void*  m_pObject;                           // +0x08
    public:
        CNode* Next() const { return m_pNext; }
    };

    class CNodeCache {
    public:
        ~CNodeCache()
        {
            CNode* pNode = m_pHead;
            while (pNode) {
                CNode* pCurrent = pNode;
                pNode = pNode->Next();
                delete pCurrent;
            }
        }
    private:
        int    m_iCacheSize;                        // +0x0c
        int    m_iUsed;                             // +0x10
        CNode* m_pHead;                             // +0x14
    };

protected:
    CNode* m_pFirst;                                // +0x00
    CNode* m_pLast;                                 // +0x04
    long   m_Count;                                 // +0x08
private:
    CNodeCache m_Cache;                             // +0x0c
public:
    ~CBaseList();
    void RemoveAll();                               // 0x00CB1340
};

CBaseList::~CBaseList()
{
    RemoveAll();
}
