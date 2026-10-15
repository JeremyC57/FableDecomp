// CScriptGameResourceObjectScriptedThingBase::func_009cbd30(long a0) -- forward to the +0xec object's virtual slot 1 when it is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CBD30, length 20 bytes (32 with padding). Caller 0x009CF27B.
// One of the three AskHeroQuestion-style forwarders of the catalogue row 009cbd30 (0x009CBD30 slot 1, 0x009CBD50 slot 0,
// 0x009CBD70 slot 2; skipped ledger, chunk 901). The manifest class is propagated (engine/CScriptGameResourceObjectScriptedThingBase.h
// is 0x10 bytes), so the owner with the polymorphic pointer at +0xec is modelled locally.
// Shape: `mov eax,[ecx+0ECh] ; test eax,eax ; je R ; mov ecx,eax ; mov eax,[ecx] ; jmp [eax+0x4] ; R: ret 0x4`.
struct CQuestionTarget_Retail {
    virtual void v00();
    virtual void vfunc_1(long a0);     // slot 1
};

struct CScriptedThingOwner_Retail {
    unsigned char          _pad_0x00[0xec];
    CQuestionTarget_Retail* m_pTarget;         // +0xec
    void func_009cbd30(long a0);
};

void CScriptedThingOwner_Retail::func_009cbd30(long a0)
{
    if (m_pTarget)
        m_pTarget->vfunc_1(a0);
}