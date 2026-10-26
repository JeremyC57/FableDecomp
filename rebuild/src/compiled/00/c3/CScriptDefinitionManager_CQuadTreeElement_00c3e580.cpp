// CScriptDefinitionManager::Shutdown() -- release owned services and two counted buffers after the helper reset; retail @ 0x00C3E580, 191 bytes.
// The manifest's CQuadTreeElement destructor name is propagated: landed CScriptDefinitionManager::~CScriptDefinitionManager @ 0x00C3F380 calls this routine. The preliminary helper at 0x00C4D1F0 performs the shared four-subobject cleanup; fields at +0x58, +0x174/+0x178, +0x180/+0x184/+0x188 and +0x1d4 are inferred directly from the retail accesses.
struct IDeletingService { virtual void Destroy(unsigned long flags) = 0; };
struct ICountedBuffer {
    virtual void Release(unsigned long flags) = 0;
    virtual void v1() = 0; virtual void v2() = 0; virtual void v3() = 0;
};
struct IServiceHub { virtual void v0() = 0; virtual void v1() = 0; virtual void Shutdown() = 0; };
extern void __cdecl operator delete(void*); // 0x00BFE9BC
class CScriptDefinitionManager { public:
    void* vftable;
    unsigned char pad04[0x54];
    IDeletingService* service58;
    unsigned char pad5c[0x118];
    ICountedBuffer* buffer174;
    ICountedBuffer* buffer178;
    unsigned char pad17c[4];
    IDeletingService* service180;
    IDeletingService* service184;
    IDeletingService* service188;
    unsigned char pad18c[0x48];
    IServiceHub* hub1d4;
    void Cleanup(); // 0x00C4D1F0
    void Shutdown();
};
void CScriptDefinitionManager::Shutdown() {
    Cleanup();
    if (service188) service188->Destroy(1);
    if (service58) { service58->Destroy(1); service58 = 0; }
    if (service180) { service180->Destroy(1); service180 = 0; }
    if (service184) service184->Destroy(1);
    if (buffer174) {
        if (*((unsigned int*)buffer174 - 1)) buffer174->Release(3);
        else operator delete((char*)buffer174 - 4);
        buffer174 = 0;
    }
    if (buffer178) {
        if (*((unsigned int*)buffer178 - 1)) buffer178->Release(3);
        else operator delete((char*)buffer178 - 4);
        buffer178 = 0;
    }
    if (hub1d4) { hub1d4->Shutdown(); hub1d4 = 0; }
}