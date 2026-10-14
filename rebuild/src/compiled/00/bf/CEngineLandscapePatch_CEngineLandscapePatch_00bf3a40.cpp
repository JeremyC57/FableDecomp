// CEngineLandscapePatch::~CEngineLandscapePatch() -- clears the patch (@ 0x00BF38F0), releases the counted resource pointer at +0x40/+0x44 (object, count block), then the inlined empty CMovableResource destructor leaves its vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF3A40, length 71 bytes (80 with padding).
// Callees: CEngineLandscapePatch::Clear @ 0x00BF38F0 (on `this`, right after the vtable store), the count block's destroy function at +4 (`__fastcall` with the
// object at +8) when the count at +0 reaches zero, operator delete @ 0x00BFE9BC for the block. Vtables: 0x012A8200 = CEngineLandscapePatch, 0x0129B860 =
// CMovableResource (rtti-vtables.tsv). Shape: as CParticleSimulation::~CParticleSimulation @ 0x00AE0630 with the pointer at +0x40: the release re-reads the
// block pointer after the decrement (`dec [eax] ; mov eax,[esi+44h] ; cmp [eax],0`) and again for the delete; both words are zeroed before the base vtable store.
extern "C" void __cdecl operator_delete_00bfe9bc(void* block);   // 0x00BFE9BC

struct SCountBlock {                               // the counted pointer's shared block
    long         m_count;                          // +0x00
    void (__fastcall* m_pfnDestroy)(void*);        // +0x04
    void*        m_pObject;                        // +0x08
};

struct CCountedResourcePointer {
    void*        m_pObject;                        // +0x00
    SCountBlock* m_pBlock;                         // +0x04
    ~CCountedResourcePointer()
    {
        if (m_pBlock) {
            --m_pBlock->m_count;
            if (m_pBlock->m_count == 0) {
                m_pBlock->m_pfnDestroy(m_pBlock->m_pObject);
                operator_delete_00bfe9bc(m_pBlock);
            }
        }
        m_pObject = 0;
        m_pBlock = 0;
    }
};

struct CMovableResource {                          // vtable 0x0129B860
    virtual ~CMovableResource() { }
};

struct CEngineLandscapePatch : CMovableResource {  // vtable 0x012A8200
    unsigned char           _pad04[0x3c];          // +0x04 .. +0x40
    CCountedResourcePointer m_resource;            // +0x40
    void Clear();                                  // 0x00BF38F0
    virtual ~CEngineLandscapePatch();
};

CEngineLandscapePatch::~CEngineLandscapePatch()
{
    Clear();
}