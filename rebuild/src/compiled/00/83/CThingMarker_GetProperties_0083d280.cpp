// CThingMarker::GetProperties() -- fetches the marker's def by its global index into a counted def pointer and returns the def's Properties word
// Leaf: GetProperties
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083D280, length 94 bytes (vtable 0x01245524 slot 48).
// Callees: CThing::GetDefinitionManager() @ 0x004C7990, CDefinitionManager::GetDef<CThingMarkerDef>(long, CDefPointer&) @ 0x0083D630,
// CThingMarkerDef virtual slot 1 (destroy at a zero count).
// Shape so far: a `__forceinline` by-value def getter (local CDefPointer filled by GetDef, returned
// by copy) consumed by `->Properties`; the returned temporary's release is emitted without a
// null test as in retail. Properties is CThingMarkerDef +0x84 (PDB +0x88).
struct CThingMarkerDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long          m_count;                         // +0x04
    unsigned char _pad08[0x7c];
    unsigned long Properties;                      // +0x84
    void AddRef() { m_count = m_count + 1; }
    void Release() { m_count = m_count - 1; if (m_count == 0) Destroy(); }
};
struct CDefPointer {
    CThingMarkerDef* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    ~CDefPointer() { if (m_p) m_p->Release(); }
    CThingMarkerDef* Get() const { return m_p; }
    CThingMarkerDef* operator->() const { return m_p; }
};
struct CDefinitionManager {
    bool GetDef_0083d630(long globalIndex, CDefPointer& out);   // 0x0083D630
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef_0083d630(long globalIndex, CDefPointer& out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    virtual void v00();
    unsigned char  _pad04[0x88];
    unsigned short DefGlobalIndex;                 // +0x8c
    CDefinitionManager* GetDefinitionManager();    // 0x004C7990
};
struct CThingMarker : CThing {
    __forceinline CDefPointer GetMarkerDef()
    {
        CDefPointer def;
        GetDefinitionManager()->GetDef_0083d630(DefGlobalIndex, def);
        return def;
    }
    virtual unsigned long GetProperties();
};
unsigned long CThingMarker::GetProperties()
{
    return GetMarkerDef()->Properties;
}