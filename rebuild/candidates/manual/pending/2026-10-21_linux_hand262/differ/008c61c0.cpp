// Grade: DIFFER(456v457) -- frame + register allocation: retail's frame is 0x4c (the difference vector, the normalised return temporary and the named direction each get their own 12-byte slot; the direction is copied from the temporary through eax), this in edi and &PCreature in esi; ours packs the vectors into a 0x40 frame with this in ebx, so every slot offset and most registers differ.
// CCreatureAction_TrollWhackGroundBase::WhackGround() -- when the troll's def and the action's whack helper name resolve to a whack effect
// that the action wants: spawn the effect at the named helper point of the troll's graphic, facing along the troll's (target - position)
// direction; in every case start a quake (strength 1) at the troll's position.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C61C0, length 457 bytes.
// Callees: CCharString 0x0099E4B0 / 0x0099EAE0; CBaseIntelligentPointer::GetPItem 0x00A01B50; CDefClassBase::GetSubDef 0x00669580; action
// slots 82 (+0x148, helper name out), 22 (+0x58, bool) and 23 (+0x5c, CCharString by value); troll def whack-effect lookup 0x00669D30
// (manifest "CCRC::CalcAndLookup"); CTCGraphicAppearance::GetHelperPoint 0x004C0560; CreateEffect 0x008F7F50 (__fastcall); vector dtor
// 0x00414EA0 (manifest "CCombatWheel::ResetRings" is propagated); CThing position 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is
// propagated); CQuake::Create 0x007F1B70 (__fastcall). 0.0001f 0x0129BA3C, 1.0f 0x0122DED8.
// Layout (descriptive): PCreature +8; thing physics +0x60 (target +0xc, position +0x18), graphic appearance +0x64, def +0x70.
// Tried: plain inline vector helpers (out-of-line call, 357), __forceinline helpers (456).
// Leaf: WhackGround
extern "C" float __cdecl sqrtf(float);
#pragma intrinsic(sqrtf)
struct CCharString {
    char* m_pData;
    CCharString();                                     // 0x0099E4B0
    ~CCharString();                                    // 0x0099EAE0
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector Normalised() const
    {
        float length = sqrtf(X * X + Y * Y + Z * Z);
        if (length > 0.0001f) {
            float inv = 1.0f / length;
            return C3DVector(X * inv, Y * inv, Z * inv);
        }
        return C3DVector(0.0f, 0.0f, 0.0f);
    }
};
struct CEffectList {                                   // 12-byte STLport vector, out-of-line dtor
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    CEffectList() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CEffectList();                                    // 0x00414EA0 (manifest "CCombatWheel::ResetRings" is propagated)
};
struct CDefRefCounted {
    virtual void v00();
    virtual void Release();                            // slot 1
    long RefCount;                                     // +4
};
struct CTrollDef : CDefRefCounted {
    bool GetWhackEffect(const CCharString& helperName, long* effect) const;   // 0x00669D30
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CTrollDef>* out);       // 0x00669580
};
struct CPhysicsState {
    unsigned char _pad00[0xc];
    C3DVector Target;                                  // +0x0c
    C3DVector Position;                                // +0x18
};
struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long a, unsigned long b, C3DVector* out, float scale);   // 0x004C0560
};
struct CThing {
    unsigned char _pad00[0x60];
    CPhysicsState* PPhysics;                           // +0x60
    CTCGraphicAppearance* PGraphic;                    // +0x64
    unsigned char _pad68[0x70 - 0x68];
    CDefClassBase* PDef;                               // +0x70
    const C3DVector& GetPosition() const;              // 0x004C73D0
};
void __fastcall CreateEffect(CThing* creature, long effect, const C3DVector* direction, const C3DVector* position);   // 0x008F7F50
struct CQuake { static CThing* __fastcall Create(const C3DVector& pos, long strength, long length, bool b); };   // 0x007F1B70
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                   // +4
    CThing* GetPItem() const;                          // 0x00A01B50
};
struct CCreatureAction_TrollWhackGroundBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual bool IsEffectWanted();                     // slot 22 (+0x58)
    virtual CCharString GetEffectHelperName();         // slot 23 (+0x5c)
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81();
    virtual bool GetWhackHelperName(CCharString* out); // slot 82 (+0x148)
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    void WhackGround();
};

void CCreatureAction_TrollWhackGroundBase::WhackGround()
{
    CDefPointer<CTrollDef> def;
    CCharString helperName;
    if (PCreature.GetPItem()->PDef->GetSubDef(&def) && GetWhackHelperName(&helperName)) {
        long effect;
        if (def->GetWhackEffect(helperName, &effect) && IsEffectWanted()) {
            CEffectList effects;
            C3DVector direction = (PCreature.GetPItem()->PPhysics->Target - PCreature.GetPItem()->PPhysics->Position).Normalised();
            C3DVector position;
            PCreature.GetPItem()->PGraphic->GetHelperPoint(GetEffectHelperName(), 0, 0, &position, 1.0f);
            CreateEffect(PCreature.GetPItem(), effect, &direction, &position);
        }
    }
    CQuake::Create(PCreature.GetPItem()->GetPosition(), 1, 0, false);
}
