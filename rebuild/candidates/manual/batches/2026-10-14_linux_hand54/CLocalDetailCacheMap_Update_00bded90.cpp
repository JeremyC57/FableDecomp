// CLocalDetailCacheMap::Update(double time) -- with the busy byte (+0) raised, the quad-tree element at +0xc is updated (0x00BDEBF0 with (the list at +4, the record at +0x64, time)); when the list exists its finished entries are removed (0x00BE04F0); the busy byte is dropped and the update's result returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDED90, length 64 bytes.
// Callees: CQuadTreeElement::Update @ 0x00BDEBF0 (bool), 0x00BE04F0 (thiscall on the list). Shape: the double re-pushed through the FPU, the result kept in bl across the second call, `ret 8`.
struct CBankFileList_00bded90 {
    void RemoveFinished();                         // 0x00BE04F0
};
struct SCacheRecord_00bded90 { unsigned char m_body[4]; };
struct CQuadTreeElement_00bded90 {
    unsigned char _body[0x58];
    bool Update(CBankFileList_00bded90* list, SCacheRecord_00bded90* record, double time);   // 0x00BDEBF0
};

struct CLocalDetailCacheMap {
    bool                      m_busy;              // +0x00
    CBankFileList_00bded90*   m_pFiles;            // +0x04
    long                      m_8;                 // +0x08
    CQuadTreeElement_00bded90 m_element;           // +0x0c
    SCacheRecord_00bded90     m_record;            // +0x64
    bool Update(double time);
};

bool CLocalDetailCacheMap::Update(double time)
{
    m_busy = true;
    bool updated = m_element.Update(m_pFiles, &m_record, time);
    if (m_pFiles)
        m_pFiles->RemoveFinished();
    m_busy = false;
    return updated;
}
