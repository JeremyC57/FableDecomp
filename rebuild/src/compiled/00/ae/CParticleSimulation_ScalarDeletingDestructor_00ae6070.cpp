// CParticleSimulation::`scalar deleting destructor'(unsigned int flags) -- the inlined destructor: resets the simulation (@ 0x00ADF1E0), releases the counted resource pointer at +4/+8 (object, count block), then the inlined empty CMovableResource destructor leaves its vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE6070, length 89 bytes (96 with padding): the plain destructor @ 0x00AE0630 inlined, then `test byte ptr [esp+8],1` frees the object (operator delete @ 0x00BFE9BC) and `this` is returned.
// Callees: CParticleSimulation::Reset @ 0x00ADF1E0 (on `this`, right after the vtable store), the count block's destroy function at +4 (`__fastcall` with the
// object at +8) when the count at +0 reaches zero, operator delete @ 0x00BFE9BC for the block. Vtables: 0x0129F0C4 = CParticleSimulation, 0x0129B860 =
// CMovableResource (rtti-vtables.tsv): the chunk label (~CEngineLandscapePatch) is propagated from the byte-identical shape @ 0x00BF3A40 (whose vtable is
// CEngineLandscapePatch's). Shape: the counted pointer's release re-reads the block pointer after the decrement (`dec [eax] ; mov eax,[esi+8] ; cmp [eax],0`:
// the decrement may alias the member) and again for the delete; the object and block words are zeroed after the test, before the base vtable store.
void __cdecl operator delete(void* block);   // MSVCR71 import thunk 0x00BFE9BC

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
                operator delete(m_pBlock);
            }
        }
        m_pObject = 0;
        m_pBlock = 0;
    }
};

class CMovableResource { public:                          // vtable 0x0129B860
    virtual ~CMovableResource() { }
};

class CParticleSimulation : public CMovableResource { public:    // vtable 0x0129F0C4
    CCountedResourcePointer m_resource;            // +0x04
    void Reset();                                  // 0x00ADF1E0
    virtual ~CParticleSimulation();
};

CParticleSimulation::~CParticleSimulation()
{
    Reset();
}

// VC7.1 emits the class's vtable and scalar deleting destructor in a translation unit that constructs it; this helper is that construction, nothing more.
void* MakeOne()
{
    return new CParticleSimulation;
}