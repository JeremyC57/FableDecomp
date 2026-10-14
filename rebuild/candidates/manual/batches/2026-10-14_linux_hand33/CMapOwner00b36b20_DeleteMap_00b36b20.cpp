// CMapOwner_00b36b20::DeleteMap() -- deletes the polymorphic object at +0 (virtual slot 0 with flag 1) and clears the slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B36B20, length 23 bytes.
// Callees: the object's virtual slot 0 (scalar deleting destructor, flag 1). Shape: `delete` of a polymorphic pointer carries its own null test; the zero store
// is unconditional. The chunk label (CEngineLandscapeRenderer::DeleteMap) is propagated: the renderer has a vtable at +0, this body's +0 is the owned
// pointer, so the owner is modelled locally. Byte-identical to 0x00B282B0.
struct CMap_00b36b20 {
    virtual ~CMap_00b36b20();
};

struct CMapOwner_00b36b20 {
    CMap_00b36b20* m_pMap;                         // +0x00
    void DeleteMap();
};

void CMapOwner_00b36b20::DeleteMap()
{
    delete m_pMap;
    m_pMap = 0;
}
