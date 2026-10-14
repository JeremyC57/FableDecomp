// CDisplayManager::PostDeviceReset() -- re-initialise the render manager on the recreated device when one is in use, give every registered device-reset callback a chance to veto, then tell the system manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BF550, length 78 bytes.
// Callees: CRenderManagerCore::Init(IDirect3DDevice9*) @ 0x00A0A940 (on PRenderManager), virtual slot 1 of each CDeviceResetCallback
// (bool OnDeviceReset), the system manager's device-reset hook @ 0x009A52C0 (manifest label "CThingFilter_IsUnconscious::operator()" is propagated).
// PDB CDisplayManager (748 bytes): SystemManager +4, PRenderManager +8, D3DDevice +0x58, UsingRenderManager +0x1b0,
// DeviceResetCallbacks (VC6 Dinkumware list<CDeviceResetCallback*>, one sentinel pointer) +0x264.
// Shape: the list walk keeps the node in esi and re-reads the sentinel from the member after every virtual call (`cmp esi,[edi+264h]`;
// only the first compare uses the register copy); a veto returns `xor al,al` from inside the loop, the success path falls
// out to the system-manager call and `mov al,1`.
struct IDirect3DDevice9;

struct CRenderManagerCore {
    void Init(IDirect3DDevice9* device);               // 0x00A0A940
};

struct CSystemManager {
    void OnDisplayDeviceReset();                       // 0x009A52C0
};

struct CDeviceResetCallback {
    virtual void v00();
    virtual bool OnDeviceReset();                      // slot 1
};

struct CRetailCallbackList {                           // VC6 Dinkumware list<CDeviceResetCallback*>: one sentinel node pointer
    struct node { node* next; node* prev; CDeviceResetCallback* value; };
    node* head;
};

struct CDisplayManager {
    void*               __vftable;                     // +0x000
    CSystemManager*     SystemManager;                 // +0x004
    CRenderManagerCore* PRenderManager;                // +0x008 scoped_ptr<CRenderManager>
    unsigned char       _pad00c[0x58 - 0x0c];
    IDirect3DDevice9*   D3DDevice;                     // +0x058
    unsigned char       _pad05c[0x1b0 - 0x5c];
    bool                UsingRenderManager;            // +0x1b0
    unsigned char       _pad1b1[0x264 - 0x1b1];
    CRetailCallbackList DeviceResetCallbacks;          // +0x264
    bool PostDeviceReset();
};

bool CDisplayManager::PostDeviceReset()
{
    if (UsingRenderManager)
        PRenderManager->Init(D3DDevice);
    for (CRetailCallbackList::node* n = DeviceResetCallbacks.head->next; n != DeviceResetCallbacks.head; n = n->next) {
        if (!n->value->OnDeviceReset())
            return false;
    }
    SystemManager->OnDisplayDeviceReset();
    return true;
}
