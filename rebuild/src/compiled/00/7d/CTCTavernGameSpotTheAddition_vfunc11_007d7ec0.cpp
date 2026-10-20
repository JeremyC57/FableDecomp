// CTCTavernGameSpotTheAddition::vfunc_11(CRenderContext* context) -- DrawPreparePrimitivesForRendering override: base draw-prepare, then while the game is played fetches (and immediately drops) the spot-the-addition definition
// Retail: Fable.exe @ 0x007D7EC0, 66 bytes (vtable 0x0126F814 slot 11, `ret 4`).
// Callees: CTCTavernGame::DrawPreparePrimitivesForRendering 0x005ECA50 (non-virtual base call),
//          CDefClassBase::GetSubDef<T>(CDefPointer<T>*) 0x007D9A80 (bool; result unused).
// The definition comes from the parent thing (+4) -> +0x70; whatever used it was compiled out, so
// only the CDefPointer's construction and destruction remain.
// Shape: the def pointer is a one-word class (null constructor, destructor `if (p && --p->ref == 0)
//        p->Destroy()`, virtual slot 1); VC7.1 homes it in the dead `context` argument slot
//        (`lea ecx,[esp+8]`, `mov [esp+0Ch],0` after the push). IsBeingPlayed is +0x20 (PDB +0x28).
struct CRenderContext;

struct CDefObject {
    virtual void v00();
    virtual void Destroy();                         // slot 1 (+0x04)
    long m_refCount;                                // +0x04
};

struct CDefPointer {                                // CDefPointer<const CSpotTheAdditionDef>
    CDefObject* m_p;
    CDefPointer() : m_p(0) {}
    ~CDefPointer()
    {
        CDefObject* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
        }
    }
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer* out);               // 0x007D9A80
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char _pad[0x70];
    CDefClassBase* m_pDefinition;                   // +0x70
};

struct CTCTavernGame {
    void* __vftable;
    CThing* m_pParentThing;                         // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x18];
    bool IsBeingPlayed;                             // +0x20
    void DrawPreparePrimitivesForRendering(CRenderContext* context);    // 0x005ECA50
};

struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    void vfunc_11(CRenderContext* context);
};

void CTCTavernGameSpotTheAddition::vfunc_11(CRenderContext* context)
{
    CTCTavernGame::DrawPreparePrimitivesForRendering(context);
    if (IsBeingPlayed) {
        CDefPointer def;
        m_pParentThing->m_pDefinition->GetSubDef(&def);
    }
}