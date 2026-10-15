// CGameCameraManager::PreparePendingMode(CDefPointer<const CCameraModeDef> def) -- look up the registered initialiser for the def's camera mode, let it allocate the mode for this manager, and make that the pending mode; the by-value def pointer is released on the way out
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00698840, length 105 bytes (+ int3 padding). `ret 4`.
// Callees: map<EMode, CCountedPointer<CBaseInit> >::find @ 0x0069CF90 (catalog "TreeNode_FindLowerBound"; iterator returned through a
// hidden pointer, key by reference), CBaseInit::AllocMain(CGameCameraManager*, CDefPointer<const CCameraModeDef>&) @ 0x0084D270 (landed),
// CCountedPointer<NCameraModes::CBase>::Reset(CBase*) @ 0x0069C2F0 (landed; on PendingMode), the def's virtual slot 1 (Destroy).
// Layout: retail CGameCameraManager as the landed SetCameraManagerDef 0x0069A850 (PendingMode +0xa0) with RegisteredModes at +0x138
// (PDB +0x150; 12-byte retail containers); the def's mode at +0x3c; a map node holds the key at +0x10 and the value's object at +0x14.
// Shape: the key is a frame local (bound to find's reference); the iterator is a named local (re-read from its frame slot; chaining off the call
// uses the returned eax); the by-value CDefPointer destructor is the landed GetDef family's ReleaseTemporary (`add eax,4` adjusted base,
// stored count forwarded to the test, def kept in ecx for the virtual call).
struct CGameCameraManager;

struct CRefCounted {                       // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CCameraModeDef : CDefBase {
    unsigned char _pad08[0x34];
    long          Mode;                    // +0x3c NCameraModes::EMode
};
struct CDefPointer_CCameraModeDef {
    CCameraModeDef* m_p;
    CCameraModeDef* operator->() const { return m_p; }
    ~CDefPointer_CCameraModeDef()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
};

namespace NCameraModes {
struct CBase;
struct CBaseInit {
    CBase* AllocMain(CGameCameraManager* manager, CDefPointer_CCameraModeDef& def);   // 0x0084D270
};
}

struct SCountBlock;
template <class T>
struct CCountedPointer {
    T* m_pObject;                          // +0
    SCountBlock* m_pCount;                 // +4
    T* operator->() const { return m_pObject; }
    void Reset(T* object);                 // 0x0069C2F0 (CBase instantiation)
};

struct SModeMapNode {                      // map<EMode, CCountedPointer<CBaseInit> > tree node
    SModeMapNode* m_links[3];              // +0x00
    unsigned char m_color[4];              // +0x0c
    long          first;                   // +0x10
    CCountedPointer<NCameraModes::CBaseInit> second;   // +0x14
};
struct CModeMapIterator {
    SModeMapNode* m_node;
    CModeMapIterator() { }
    SModeMapNode* operator->() const { return m_node; }
};
struct CModeMap {
    unsigned char _body[0x0c];
    CModeMapIterator find(const long& key);   // 0x0069CF90
};

struct CGameCameraManager {
    void*         __vftable;                                  // +0x00
    unsigned char _pad04[0x9c];                               // +0x04
    CCountedPointer<NCameraModes::CBase> PendingMode;         // +0xa0
    unsigned char _pada8[0x90];                               // +0xa8
    CModeMap      RegisteredModes;                            // +0x138
    void PreparePendingMode(CDefPointer_CCameraModeDef def);
};

void CGameCameraManager::PreparePendingMode(CDefPointer_CCameraModeDef def)
{
    long mode = def->Mode;
    CModeMapIterator it = RegisteredModes.find(mode);
    PendingMode.Reset(it->second->AllocMain(this, def));
}
