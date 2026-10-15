// NUISystem::CManager::RenderErrorMessage(CRenderContext* context) -- advance the error-message animation by the real time since the last call, then render the error-message component one meta layer above the current one and restore the layer
// Grade: DIFFER(108v108) -- scheduling only: for the float argument of UpdateErrorMessage retail emits `push ecx ; fld dword ptr [ebp-4] ; mov ecx,esi ; fstp [esp]`, ours `fld dword ptr [ebp-4] ; push ecx ; ...`
// Leaf: RenderErrorMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041D03C, length 108 bytes. Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Callees: GFGetTime @ 0x009E1BC0 (`__fastcall`, double; twice), CManager::UpdateErrorMessage(float) @ 0x0041CE9D, own virtual slots
//   35 GetMetaLayer (+0x8c) and 34 SetMetaLayer (+0x88) (vtable 0x01230134), the error component's virtual slot 2 (+0x08) with four zeros.
// Layout (retail NUISystem::CManager): error-message component +0xa0, last update time (double) +0xa8 (`#pragma pack(4)` keeps the
//   vptr in a 4-byte slot).
// Tried: the second time read assigned directly, and through a double local (identical).
#pragma optimize("y", off)
#pragma optimize("s", on)
double __fastcall GFGetTime();                     // 0x009E1BC0
struct CRenderContext;
namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01();
    virtual void Render(CRenderContext* context, long x, long y, long flags, long alpha);   // slot 2 (+0x08)
};
#pragma pack(push, 4)
struct CManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33();
    virtual void SetMetaLayer(long layer);         // slot 34 (+0x88)
    virtual long GetMetaLayer();                   // slot 35 (+0x8c)
    unsigned char _pad04[0xa0 - 4];
    CComponent*   m_pErrorMessage;                 // +0xa0
    unsigned char _pada4[4];
    double        m_lastErrorUpdateTime;           // +0xa8
    void UpdateErrorMessage(float deltaTime);      // 0x0041CE9D
    void RenderErrorMessage(CRenderContext* context);
};
#pragma pack(pop)
void CManager::RenderErrorMessage(CRenderContext* context)
{
    float deltaTime = (float)(GFGetTime() - m_lastErrorUpdateTime);
    m_lastErrorUpdateTime = GFGetTime();
    UpdateErrorMessage(deltaTime);
    long layer = GetMetaLayer();
    SetMetaLayer(layer + 1);
    m_pErrorMessage->Render(context, 0, 0, 0, 0);
    SetMetaLayer(layer);
}
}
