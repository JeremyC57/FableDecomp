// CSoundEmitterLH::`scalar deleting destructor'(unsigned int) -- stores the CSoundEmitterLH vtable, runs the destructor body call, clears the array member at +0x14 and frees its block, runs CBaseClass::~CBaseClass (0x0099A2E0) and frees on bit 0
// Leaf: ??_GCSoundEmitterLH
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AACEA0, length 70 bytes (vtable 0x0129E6EC slot 0).
// Callees: the emitter's shutdown member @ 0x00AAC2E0 (manifest "CDrawQuestInfo::GetMiniGameType",
// propagated), the array's element teardown @ 0x00AACC60 (manifest "remove_if<...>", propagated),
// free @ 0x00BFEA14, CBaseClass::~CBaseClass @ 0x0099A2E0, operator delete @ 0x00BFE9BC.
// The catalog label "CThingTrackNode::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the CSoundEmitterLH vtable 0x0129E6EC.
// Layout: no PDB layout under this name; a polymorphic CBase-derived class with an array member at
// +0x14 (data pointer first).
// Shape: the destructor body calls one member function, then the array member's inline destructor
// (`DestroyElements(); if (data) free(data);`) -- retail keeps the member address in edi and reloads
// the data word through it -- then the base destructor. The array destructor must be `__forceinline`:
// a plain inline one (two calls + a free) is emitted out of line and called once.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CBaseClass {
public:                                      // root polymorphic base
    CBaseClass();
    virtual ~CBaseClass();                              // 0x0099A2E0
};

struct CEmitterArray {                             // array whose destructor tears the elements down, then frees
    void* m_pData;
    long  m_size;
    long  m_capacity;
    void DestroyElements();                        // 0x00AACC60
    CEmitterArray() : m_pData(0), m_size(0), m_capacity(0) { }
    __forceinline ~CEmitterArray()
    {
        DestroyElements();
        if (m_pData) free(m_pData);
    }
};

struct CSoundEmitterLH : CBaseClass {                   // vtable 0x0129E6EC
    unsigned char _pad_0x04[0x10];                 // +0x04
    CEmitterArray m_soundEmitters;                      // +0x14
    void Shutdown();                               // 0x00AAC2E0
    CSoundEmitterLH() { }
    __forceinline virtual ~CSoundEmitterLH() { Shutdown(); }
};

CSoundEmitterLH* Create_CSoundEmitterLH()
{
    return new CSoundEmitterLH;
}