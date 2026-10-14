// CBlockCacheOwner_00bdf0d0::Reset() -- release the cached element's block through the manager
// and drop every entry of the 16-byte-element vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDF0D0, length 33 bytes (+ int3 padding).
// Callees: CQuadTreeElement::ClearCache(CEngineStaticMapFileBlockManager*) @ 0x00BDDB90 on the
// element at +0x0c with the manager pointer at +0x04 (the manifest names the callee; this body's
// own label "CContainmentVolume::SaveBinary" is propagated and rejected), and the vector's
// erase(first, last) @ 0x00515BC0 (copies 16-byte elements down and resets `last`; the manifest's
// "Vector_RemoveAt" label) on the vector at +0x58 with its own begin/end -- `clear()`.
// Layout: +0x04 manager pointer, +0x0c the quad-tree element (0x4c bytes), +0x58 { first, last }.
// Shape: no vtable touched; the vector's `last` is read into edx before `first` into eax and
// both are pushed, the second call's `this` is the member address.
struct CEngineStaticMapFileBlockManager;

struct CQuadTreeElement_Retail {               // 0x4c bytes at +0x0c
    unsigned char _body[0x4c];
    void ClearCache(CEngineStaticMapFileBlockManager* manager);   // 0x00BDDB90
};

struct SBlockEntry16 {                         // 16-byte element
    unsigned char _body[0x10];
};

struct CRetailVector_SBlockEntry16 {           // first / last (the end-of-storage word follows, unused here)
    SBlockEntry16* first;
    SBlockEntry16* last;
    SBlockEntry16* erase(SBlockEntry16* eraseFirst, SBlockEntry16* eraseLast);   // 0x00515BC0
    void clear() { erase(first, last); }
};

struct CBlockCacheOwner_00bdf0d0 {
    virtual void v00();
    CEngineStaticMapFileBlockManager* m_pManager;      // +0x04
    unsigned char                     _pad08[4];       // +0x08
    CQuadTreeElement_Retail           m_element;       // +0x0c
    CRetailVector_SBlockEntry16       m_entries;       // +0x58
    void Reset();
};

void CBlockCacheOwner_00bdf0d0::Reset()
{
    m_element.ClearCache(m_pManager);
    m_entries.clear();
}