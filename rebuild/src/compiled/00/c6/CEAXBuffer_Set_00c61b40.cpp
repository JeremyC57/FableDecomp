// CEAXBuffer::Set(unsigned long property, const void* value, _CHANGES** ppEvents) -- set one EAX source property, then hand back the context's event queue
// Leaf: ?Set@CEAXBuffer@@QAEJKPBXPAPAU_CHANGES@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C61B40, 70 bytes (thiscall, `ret 0Ch`).
// Callees: the context's ClearEvents 0x00C60410, the buffer's private CEAXBuffer::Set3DBuffer 0x00C60FF0 (GameCODA 2.10
//   EAXBuffer.obj name and signature, owner decision 2026-11-03 D3; `cmp [esp+20h],1` tests its third, apply-mode argument).
// Identity: the manifest's "CDrawGuildSeal_Render" is a propagated label. `this` is an EAX source (vtable 0x012B9344)
//   whose +0x14 is the owning EAX 4.0 context; bit 0x100000 of the property id selects the deferred path and is
//   stripped. No direct or absolute references in the image (an exported/unused entry). Descriptive name.
struct _CHANGES;

class CEAXListener {
public:
    unsigned char _pad000[0x3c4];
    _CHANGES* m_pEvents;                          // +0x3c4 queued notifications
    void ClearEvents();                            // 0x00C60410
};

class CEAXBuffer {
public:
    unsigned char _pad000[0x14];
    CEAXListener* m_pContext;                       // +0x14
    long Set(unsigned long property, const void* value, _CHANGES** ppEvents);
private:
    int Set3DBuffer(unsigned long property, void* value, unsigned long apply);   // 0x00C60FF0 (GameCODA EAXBuffer.obj, private)
};

long CEAXBuffer::Set(unsigned long property, const void* value, _CHANGES** ppEvents)
{
    long result;
    m_pContext->ClearEvents();
    if (property & 0x100000)
        result = Set3DBuffer(property & ~0x100000, (void*)value, 1);
    else
        result = Set3DBuffer(property, (void*)value, 0);
    *ppEvents = m_pContext->m_pEvents;
    return result;
}