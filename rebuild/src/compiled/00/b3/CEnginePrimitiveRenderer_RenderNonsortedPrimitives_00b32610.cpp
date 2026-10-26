// CEnginePrimitiveRenderer::RenderNonsortedPrimitives() -- call virtual slot 11 on the object of every 8-byte entry of the vector at +0x274, re-counting the vector after each call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B32610, length 69 bytes.
// Callees: virtual slot 11 (+0x2c) of each entry's object (the first word). Shape: the count (`sub ; sar 3`) is taken before the loop (`jbe` on an empty vector)
// and again after every call for the unsigned `jb` test; the index runs in edi, the entry's object is loaded with `mov ecx,[ecx+edi*8]`; the loop head is
// aligned with `lea esp,[esp]`.
struct CRenderable_00b32610 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual void Render();                         // slot 11 (+0x2c)
};

struct SRenderEntry { CRenderable_00b32610* m_pObject; long m_key; };   // 8 bytes

struct CRetailEntryVector_00b32610 {
    SRenderEntry* m_pFirst;
    SRenderEntry* m_pLast;
    SRenderEntry* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
};

class CEnginePrimitiveRenderer { public:
    unsigned char               _pad[0x274];
    CRetailEntryVector_00b32610 m_nonsorted;       // +0x274
    void RenderNonsortedPrimitives();
};

void CEnginePrimitiveRenderer::RenderNonsortedPrimitives()
{
    for (unsigned long i = 0; i < m_nonsorted.GetCount(); ++i)
        m_nonsorted.m_pFirst[i].m_pObject->Render();
}