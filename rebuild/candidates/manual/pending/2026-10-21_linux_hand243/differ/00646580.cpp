// NPlayerGui::CDrawMeleeActionButtonEffects::Draw(CDrawContext* context) -- unless the gui def disables them: when a flourish is pending (counter >= 0 or the icon timer >= 0) and the effect count is positive, create the 2D flourish emitter once (anchored at the def's offset) and keep it in the intelligent pointer, then draw the flourish sprite at the def's position plus offset; when the def disables them the count is reset; with no effects left the emitter list is cleared
// Grade: DIFFER(425v393) -- best effort (size TU): the emitter creation and the sprite constructor argument lists are inferred; intelligent-pointer temporary handling differs
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00646580, length 393 bytes (frame pointer), `ret 4`.
// Callees: PeekGuiDef @ 0x00438570, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, GFAnchorToBottomRight @ 0x00643D0C (`__fastcall` out, in),
// CRightHandedSet::CRightHandedSet @ 0x00487050 (hidden-pointer temporary), CParticleEmitterManager::Get @ 0x00AEAAF0 (`__fastcall`),
// ::Create2DEmitter @ 0x00AEB790, CIntelligentPointer<const CThing>::CIntelligentPointer(CThing*) @ 0x00655318, operator= @ 0x0065533C,
// ~CBaseIntelligentPointer @ 0x00A01C10, CDrawBase::AnchorPoint @ 0x00643D5D, CEnginePrimitive2DSprite ctor @ 0x00493AC2, the drawer's
// virtual slot 0x5c, ClearEffects @ 0x006445BA.
// Layout: CDrawMeleeActionButtonEffects Def +4 (flag +0x250, emitter offset +0x788, sprite offset +0x794, texture +0x790), Drawn +0x18,
// Sprite +0x1c, PEmitter +0x28 (intelligent pointer), FlashTimer +0x34, ComboLoadingIconTimer +0x38, EffectCount +0x40.
// Shape: best-effort (size TU).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; };
struct CPlayerGuiDef {
    unsigned char _pad[0x250];
    bool DisableEffects;                   // +0x250
    unsigned char _pad2[0x788 - 0x251];
    C2DVector EmitterOffset;               // +0x788
    long Texture;                          // +0x790
    C2DVector SpriteOffset;                // +0x794
};
CPlayerGuiDef* __fastcall PeekGuiDef();   // 0x00438570
void __fastcall GFAnchorToBottomRight(C2DVector* out, const C2DVector* in);   // 0x00643D0C
struct CRightHandedSet { unsigned char _body[0x18]; CRightHandedSet(); };   // 0x00487050
struct CThing;
struct CParticleEmitterManager {
    static CParticleEmitterManager* __fastcall Get();   // 0x00AEAAF0
    CThing* Create2DEmitter(long count, const C2DVector* position, long a, long b, CRightHandedSet set, C3DVector* scale, C3DVector* colour);   // 0x00AEB790
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;              // 0x00A01B50
    ~CBaseIntelligentPointer();            // 0x00A01C10
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(CThing* thing);    // 0x00655318
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x0065533C
};
struct SAnchor { long A, B, C; };
struct CEnginePrimitive2DSprite {
    unsigned char _body[0x88];
    CEnginePrimitive2DSprite(long a, long b, const C2DVector* position, long texture, long c, long d, long mode, long e, float u, float v, long anchor);   // 0x00493AC2
};
struct CDrawer {
    virtual void v00();
    unsigned char _slots[0x5c - 4];
    virtual void DrawPrimitive(void* request, const CEnginePrimitive2DSprite* sprite, long order);
};
struct CDrawContext { unsigned char _pad[0x34]; CDrawer* Drawer; };
struct CEffects { void ClearEffects(); };   // 0x006445BA
struct CDrawMeleeActionButtonEffects {
    unsigned char _pad0[4];
    CPlayerGuiDef* Def;                    // +0x04
    unsigned char _pad1[0x18 - 8];
    bool Drawn;                            // +0x18
    long _pad1c;
    void* Sprite;                          // +0x1c
    unsigned char _pad2[0x28 - 0x20];
    CIntelligentPointer PEmitter;          // +0x28
    unsigned char _pad3[0x34 - 0x30];
    long FlashTimer;                       // +0x34
    long ComboLoadingIconTimer;            // +0x38
    long _pad3c;
    long EffectCount;                      // +0x40
    long AnchorPoint(long tier, SAnchor anchor, long flags, C2DVector* out, const C2DVector* in);   // 0x00643D5D
    void Draw(CDrawContext* context);
};

void CDrawMeleeActionButtonEffects::Draw(CDrawContext* context)
{
    if (!Def->DisableEffects) {
        if ((FlashTimer >= 0 || ComboLoadingIconTimer >= 0) && EffectCount > 0) {
            Drawn = true;
            CPlayerGuiDef* def = PeekGuiDef();
            if (FlashTimer >= 0 && !PEmitter.GetPItem()) {
                C2DVector position;
                GFAnchorToBottomRight(&position, &def->EmitterOffset);
                C3DVector scale = { 0.0f, 1.0f, 0.0f };
                C3DVector colour = { 0.0f, 0.0f, 1.0f };
                CIntelligentPointer emitter(CParticleEmitterManager::Get()->Create2DEmitter(EffectCount, &position, 0x44, 1, CRightHandedSet(), &scale, &colour));
                PEmitter = emitter;
            }
            if (PEmitter.GetPItem()) {
                C2DVector position;
                GFAnchorToBottomRight(&position, &def->EmitterOffset);
                C2DVector pos = { position.X + def->SpriteOffset.X, position.Y + def->SpriteOffset.Y };
                C2DVector size = { 0.0f, 0.0f };
                C2DVector zero = { 0.0f, 0.0f };
                SAnchor anchor;
                long tier = AnchorPoint(255, anchor, 0, &size, &zero);
                CEnginePrimitive2DSprite sprite(0, 0, &pos, 0x43, def->Texture, 0, 2, 0, 0.0f, 0.0f, tier);
                context->Drawer->DrawPrimitive(Sprite, &sprite, 0xc0);
            }
        }
    }
    else
        EffectCount = 0;
    if (EffectCount == 0)
        ((CEffects*)&PEmitter)->ClearEffects();
}
