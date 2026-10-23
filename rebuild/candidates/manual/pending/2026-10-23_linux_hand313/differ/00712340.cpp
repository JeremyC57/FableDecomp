// NTCScriptedControl::CActionTalkBase::GetCameraSetupPositions(const CCameraTag&, const CThing*&, C3DVector&, const CThing*&, C3DVector&) -- work out the conversation camera's from/at things and positions for a camera tag (head points, midpoints with sideways offsets, the demon-door special case)
// Grade: DIFFER(1410v1404) -- only the OffsetType switch differs: retail cross-jumps the shared 'fxch; fadd [esi]; fstp [esi]' tail of the scaled cases 1 and 2 into one block placed after case 0 (case 0 jumps over it); VC7.1 here keeps the tails per case (+6 bytes). The global offset loads show as [4]/[8] in the masked view (symbol+offset relocations). Tried: switch case orders, if/else chain, explicit member adds, non-forceinline +=.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    float GetLength() const;                    // 0x00A14480
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetForward() const;       // slot 70 (+0x118)
};
struct CTCGraphicAppearance {
    C3DVector GetHelperPoint(float scale) const;   // 0x004C1060
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PPhysics;                   // +0x60
    CTCGraphicAppearance* PAppearance;          // +0x64
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;           // slot 11 (+0x2c)
    long _a, _b;
};
struct CCreaturePointer { void* __vftable; CThing* PThing; };
extern C3DVector g_CameraHeadOffset;            // 0x01380C44
namespace NTCScriptedControl {
struct CSubtitledMessage {
    struct CCameraTag {
        long FromType;                          // +0x00
        long Who;                               // +0x04 (0xb speaker first, 0xc listener first)
        long AtType;                            // +0x08
        long _0c, _10;
        long OffsetType;                        // +0x14
    };
};
struct CActionTalkBase {
    void* __vftable;                            // +0x00
    CCreaturePointer* PControl;                 // +0x04
    unsigned char _pad08[4];
    CScriptThing PTalkTarget;                   // +0x0C
    unsigned char _pad18[0x8d - 0x18];
    bool ParentIsDemonDoor;                     // +0x8d
    void GetCameraSetupPositions(const CSubtitledMessage::CCameraTag& tag, const CThing*& fromThing, C3DVector& fromPos,
                                 const CThing*& atThing, C3DVector& atPos) const;
};
}

void NTCScriptedControl::CActionTalkBase::GetCameraSetupPositions(const CSubtitledMessage::CCameraTag& tag, const CThing*& fromThing,
    C3DVector& fromPos, const CThing*& atThing, C3DVector& atPos) const
{
    CThing* me = PControl->PThing;
    CThing* other = PTalkTarget.GetThing();
    bool talkingToSelf = me == other;
    const CThing* first = 0;
    const CThing* second = 0;
    if (tag.Who == 0xb) {
        first = me;
        second = other;
    } else if (tag.Who == 0xc) {
        first = other;
        second = me;
    }
    fromThing = 0;
    if (talkingToSelf) {
        fromThing = PControl->PThing;
        C3DVector forward = fromThing->PPhysics->GetForward();
        CTCGraphicAppearance* app = fromThing->PAppearance;
        C3DVector head = app->GetHelperPoint(1.0f);
        fromPos = forward * 2.0f + head;
        fromPos.Z += 0.2f;
    } else if (tag.FromType == 1) {
        fromThing = PControl->PThing;
        {
            CTCGraphicAppearance* app = fromThing->PAppearance;
            fromPos = app->GetHelperPoint(1.0f);
        }
    } else if (tag.FromType == 2) {
        fromThing = PTalkTarget.GetThing();
        {
            CTCGraphicAppearance* app = fromThing->PAppearance;
            fromPos = app->GetHelperPoint(1.0f);
        }
    } else {
        C3DVector half = (second->GetPosition() - first->GetPosition()) * 0.5f;
        fromPos = half + first->GetPosition();
        switch (tag.OffsetType) {
        case 0: {
            C3DVector d = second->GetPosition() - first->GetPosition();
            fromPos += C3DVector(d.Y, -d.X, 0.0f);
            break;
        }
        case 1: {
            C3DVector d = second->GetPosition() - first->GetPosition();
            fromPos += C3DVector(d.Y, -d.X, 0.0f) * 0.75f;
            break;
        }
        case 2: {
            C3DVector d = second->GetPosition() - first->GetPosition();
            fromPos += C3DVector(d.Y, -d.X, 0.0f) * 1.75f;
            break;
        }
        }
        CTCGraphicAppearance* appA = first->PAppearance;
        float z = appA->GetHelperPoint(1.0f).Z;
        CTCGraphicAppearance* appB = second->PAppearance;
        fromPos.Z = (appB->GetHelperPoint(1.0f).Z - z) * 0.5f + z;
    }
    atThing = 0;
    if (tag.AtType == 1) {
        atThing = PControl->PThing;
        {
            CTCGraphicAppearance* app = atThing->PAppearance;
            atPos = g_CameraHeadOffset + app->GetHelperPoint(1.0f);
        }
    } else if (tag.AtType == 2) {
        atThing = PTalkTarget.GetThing();
        {
            CTCGraphicAppearance* app = atThing->PAppearance;
            atPos = g_CameraHeadOffset + app->GetHelperPoint(1.0f);
        }
    } else {
        C3DVector half = (second->GetPosition() - first->GetPosition()) * 0.5f;
        atPos = half + first->GetPosition();
        CTCGraphicAppearance* appA = first->PAppearance;
        float z = appA->GetHelperPoint(1.0f).Z;
        CTCGraphicAppearance* appB = second->PAppearance;
        atPos.Z = (appB->GetHelperPoint(1.0f).Z - z) * 0.5f + z;
    }
    if (ParentIsDemonDoor) {
        atPos.Z += 1.4f;
        C3DVector d = fromPos - atPos;
        float length = d.GetLength();
        C3DVector forward = me->PPhysics->GetForward();
        if (d.Y * forward.Y + d.Z * forward.Z + forward.X * d.X < 0.45f || length < 2.45f) {
            fromPos = atPos;
            fromPos += forward * 2.5f;
        }
    }
}
