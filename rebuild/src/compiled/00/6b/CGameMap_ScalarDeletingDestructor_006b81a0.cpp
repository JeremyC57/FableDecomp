// CGameMap::`scalar deleting destructor'(unsigned int) -- free the map buffer, ~CBaseClass, optional delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B81A0, length 54 bytes.
// Callees: free @ 0x00BFEA14 (__cdecl, result dropped: `add esp,4`), CBaseClass::~CBaseClass
// @ 0x0099A300 (the second copy of the body storing vtable 0x01231710), operator delete
// @ 0x00BFE9BC when bit 0 of the flag is set. Vtable 0x0125D558 CGameMap (rtti-vtables.tsv);
// the manifest's "CAInputProcess" label is propagated.
// Shape: this class's vtable FIRST, then the buffer member's inline destructor `if (m_pData)
// free(m_pData)` (a member destructor: a statement in this class's own body would be scheduled
// above the vtable store), the base destructor, then the deleting tail. The vtable (and
// this `??_G`) is emitted because the TU constructs the object (MakeOne).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

namespace N_0099a300 { struct CBaseClass {
    virtual void v00();
    ~CBaseClass();                              // 0x0099A300
}; } using N_0099a300::CBaseClass;

struct CMapBuffer {
    void* m_pData;                              // +0x00 the map cells (malloc'd)
    ~CMapBuffer()
    {
        if (m_pData != 0)
            free(m_pData);
    }
};

struct CGameMap : CBaseClass {
    CMapBuffer m_cells;                         // +0x04
    virtual ~CGameMap() {}
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne()
{
    return new CGameMap;
}