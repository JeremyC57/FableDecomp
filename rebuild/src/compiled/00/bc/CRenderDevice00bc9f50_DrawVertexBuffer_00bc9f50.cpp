// CRenderDevice_00bc9f50::DrawVertexBuffer(const CVertexBufferInfo* vb, unsigned long startVertex, unsigned long primitiveCount, unsigned long primitiveType) -- bind the stream, realise the render state, draw
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC9F50, length 82 bytes.
// Callees: IDirect3DDevice9::SetStreamSource (COM slot 100, +0x190) and IDirect3DDevice9::DrawPrimitive
// (COM slot 81, +0x144) on the device at +0x1d4 (`__stdcall` virtuals: `this` goes on the stack),
// CRenderStateManager::RealiseRenderState @ 0x00A058C0 on the manager at +0x1d0.
// Shape: SetStreamSource(0, vb->Buffer (+0x04), 0, vb->Stride (+0x10, a byte zero-extended into
// edi)); the device is re-read from `this` for the second call (the realise call may change it);
// DrawPrimitive(type, start, count) with the arguments pulled from their slots right to left.
// The manifest label (CBowerstoneIntroGuard::Init) is propagated garbage for this 0x00BCxxxx
// engine body; the class is named by what it does.
#define COM_SLOT(n) virtual void __stdcall slot##n()

struct IDirect3DVertexBuffer9;

struct IDirect3DDevice9 {                       // only the two slots used are typed
    COM_SLOT(0); COM_SLOT(1); COM_SLOT(2); COM_SLOT(3); COM_SLOT(4); COM_SLOT(5); COM_SLOT(6); COM_SLOT(7); COM_SLOT(8); COM_SLOT(9);
    COM_SLOT(10); COM_SLOT(11); COM_SLOT(12); COM_SLOT(13); COM_SLOT(14); COM_SLOT(15); COM_SLOT(16); COM_SLOT(17); COM_SLOT(18); COM_SLOT(19);
    COM_SLOT(20); COM_SLOT(21); COM_SLOT(22); COM_SLOT(23); COM_SLOT(24); COM_SLOT(25); COM_SLOT(26); COM_SLOT(27); COM_SLOT(28); COM_SLOT(29);
    COM_SLOT(30); COM_SLOT(31); COM_SLOT(32); COM_SLOT(33); COM_SLOT(34); COM_SLOT(35); COM_SLOT(36); COM_SLOT(37); COM_SLOT(38); COM_SLOT(39);
    COM_SLOT(40); COM_SLOT(41); COM_SLOT(42); COM_SLOT(43); COM_SLOT(44); COM_SLOT(45); COM_SLOT(46); COM_SLOT(47); COM_SLOT(48); COM_SLOT(49);
    COM_SLOT(50); COM_SLOT(51); COM_SLOT(52); COM_SLOT(53); COM_SLOT(54); COM_SLOT(55); COM_SLOT(56); COM_SLOT(57); COM_SLOT(58); COM_SLOT(59);
    COM_SLOT(60); COM_SLOT(61); COM_SLOT(62); COM_SLOT(63); COM_SLOT(64); COM_SLOT(65); COM_SLOT(66); COM_SLOT(67); COM_SLOT(68); COM_SLOT(69);
    COM_SLOT(70); COM_SLOT(71); COM_SLOT(72); COM_SLOT(73); COM_SLOT(74); COM_SLOT(75); COM_SLOT(76); COM_SLOT(77); COM_SLOT(78); COM_SLOT(79);
    COM_SLOT(80);
    virtual long __stdcall DrawPrimitive(unsigned long primitiveType, unsigned long startVertex, unsigned long primitiveCount);   // slot 81 (+0x144)
    COM_SLOT(82); COM_SLOT(83); COM_SLOT(84); COM_SLOT(85); COM_SLOT(86); COM_SLOT(87); COM_SLOT(88); COM_SLOT(89);
    COM_SLOT(90); COM_SLOT(91); COM_SLOT(92); COM_SLOT(93); COM_SLOT(94); COM_SLOT(95); COM_SLOT(96); COM_SLOT(97); COM_SLOT(98); COM_SLOT(99);
    virtual long __stdcall SetStreamSource(unsigned long streamNumber, IDirect3DVertexBuffer9* streamData, unsigned long offsetInBytes, unsigned long stride);   // slot 100 (+0x190)
};

struct CRenderStateManager {
    void RealiseRenderState();                  // 0x00A058C0
};

struct CVertexBufferInfo {
    unsigned char          _pad0[4];            // +0x00
    IDirect3DVertexBuffer9* Buffer;             // +0x04
    unsigned char          _pad1[8];            // +0x08..+0x0f
    unsigned char          Stride;              // +0x10
};

struct CRenderDevice_00bc9f50 {
    unsigned char        _pad[0x1d0];           // +0x000..+0x1cf
    CRenderStateManager* m_pRenderStateManager; // +0x1d0
    IDirect3DDevice9*    m_pDevice;             // +0x1d4
    void DrawVertexBuffer(const CVertexBufferInfo* vb, unsigned long startVertex, unsigned long primitiveCount, unsigned long primitiveType);
};

void CRenderDevice_00bc9f50::DrawVertexBuffer(const CVertexBufferInfo* vb, unsigned long startVertex, unsigned long primitiveCount, unsigned long primitiveType)
{
    m_pDevice->SetStreamSource(0, vb->Buffer, 0, vb->Stride);
    m_pRenderStateManager->RealiseRenderState();
    m_pDevice->DrawPrimitive(primitiveType, startVertex, primitiveCount);
}