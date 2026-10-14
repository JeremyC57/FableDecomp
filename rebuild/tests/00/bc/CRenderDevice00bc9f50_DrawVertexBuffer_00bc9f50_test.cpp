#include <stdio.h>
#define COM_SLOT(n) virtual void __stdcall slot##n() {}
struct IDirect3DVertexBuffer9;
static unsigned long g_ss[4]; static unsigned long g_dp[3]; static int g_order[3]; static int g_n;
struct IDirect3DDevice9 {
    COM_SLOT(0) COM_SLOT(1) COM_SLOT(2) COM_SLOT(3) COM_SLOT(4) COM_SLOT(5) COM_SLOT(6) COM_SLOT(7) COM_SLOT(8) COM_SLOT(9)
    COM_SLOT(10) COM_SLOT(11) COM_SLOT(12) COM_SLOT(13) COM_SLOT(14) COM_SLOT(15) COM_SLOT(16) COM_SLOT(17) COM_SLOT(18) COM_SLOT(19)
    COM_SLOT(20) COM_SLOT(21) COM_SLOT(22) COM_SLOT(23) COM_SLOT(24) COM_SLOT(25) COM_SLOT(26) COM_SLOT(27) COM_SLOT(28) COM_SLOT(29)
    COM_SLOT(30) COM_SLOT(31) COM_SLOT(32) COM_SLOT(33) COM_SLOT(34) COM_SLOT(35) COM_SLOT(36) COM_SLOT(37) COM_SLOT(38) COM_SLOT(39)
    COM_SLOT(40) COM_SLOT(41) COM_SLOT(42) COM_SLOT(43) COM_SLOT(44) COM_SLOT(45) COM_SLOT(46) COM_SLOT(47) COM_SLOT(48) COM_SLOT(49)
    COM_SLOT(50) COM_SLOT(51) COM_SLOT(52) COM_SLOT(53) COM_SLOT(54) COM_SLOT(55) COM_SLOT(56) COM_SLOT(57) COM_SLOT(58) COM_SLOT(59)
    COM_SLOT(60) COM_SLOT(61) COM_SLOT(62) COM_SLOT(63) COM_SLOT(64) COM_SLOT(65) COM_SLOT(66) COM_SLOT(67) COM_SLOT(68) COM_SLOT(69)
    COM_SLOT(70) COM_SLOT(71) COM_SLOT(72) COM_SLOT(73) COM_SLOT(74) COM_SLOT(75) COM_SLOT(76) COM_SLOT(77) COM_SLOT(78) COM_SLOT(79)
    COM_SLOT(80)
    virtual long __stdcall DrawPrimitive(unsigned long t, unsigned long s, unsigned long c) { g_dp[0] = t; g_dp[1] = s; g_dp[2] = c; g_order[g_n++] = 3; return 0; }
    COM_SLOT(82) COM_SLOT(83) COM_SLOT(84) COM_SLOT(85) COM_SLOT(86) COM_SLOT(87) COM_SLOT(88) COM_SLOT(89)
    COM_SLOT(90) COM_SLOT(91) COM_SLOT(92) COM_SLOT(93) COM_SLOT(94) COM_SLOT(95) COM_SLOT(96) COM_SLOT(97) COM_SLOT(98) COM_SLOT(99)
    virtual long __stdcall SetStreamSource(unsigned long n, IDirect3DVertexBuffer9* d, unsigned long o, unsigned long st) { g_ss[0] = n; g_ss[1] = (unsigned long)d; g_ss[2] = o; g_ss[3] = st; g_order[g_n++] = 1; return 0; }
};
struct CRenderStateManager { void RealiseRenderState() { g_order[g_n++] = 2; } };
struct CVertexBufferInfo { unsigned char _pad0[4]; IDirect3DVertexBuffer9* Buffer; unsigned char _pad1[8]; unsigned char Stride; };
struct CRenderDevice_00bc9f50 {
    unsigned char _pad[0x1d0]; CRenderStateManager* m_pRenderStateManager; IDirect3DDevice9* m_pDevice;
    void DrawVertexBuffer(const CVertexBufferInfo* vb, unsigned long startVertex, unsigned long primitiveCount, unsigned long primitiveType);
};
void CRenderDevice_00bc9f50::DrawVertexBuffer(const CVertexBufferInfo* vb, unsigned long startVertex, unsigned long primitiveCount, unsigned long primitiveType)
{
    m_pDevice->SetStreamSource(0, vb->Buffer, 0, vb->Stride);
    m_pRenderStateManager->RealiseRenderState();
    m_pDevice->DrawPrimitive(primitiveType, startVertex, primitiveCount);
}
int main()
{
    IDirect3DDevice9 dev; CRenderStateManager rsm; CRenderDevice_00bc9f50* r = new CRenderDevice_00bc9f50; r->m_pDevice = &dev; r->m_pRenderStateManager = &rsm;
    CVertexBufferInfo vb; vb.Buffer = (IDirect3DVertexBuffer9*)0x1000; vb.Stride = 0xF0;
    r->DrawVertexBuffer(&vb, 5, 12, 4);
    bool ok = g_n == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3 && g_ss[0] == 0 && g_ss[1] == 0x1000 && g_ss[2] == 0 && g_ss[3] == 0xF0 && g_dp[0] == 4 && g_dp[1] == 5 && g_dp[2] == 12;
    delete r;
    if (!ok) { printf("00bc9f50_TEST FAIL\n"); return 1; }
    printf("00bc9f50_TEST PASS\n"); return 0;
}