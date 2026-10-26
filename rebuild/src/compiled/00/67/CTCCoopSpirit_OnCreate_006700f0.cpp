// CTCCoopSpirit::OnCreate() -- reset the co-op spirit: drop its particle emitter, clear its timers, score, movement and master, and fetch its CCoopSpiritDef
// Leaf: ?OnCreate@CTCCoopSpirit@@QAEXXZ
// Retail: Fable.exe @ 0x006700F0, 162 bytes, plain `ret`; Ego_r.pdb ?OnCreate@CTCCoopSpirit@@EAEXXZ (modelled non-virtual public).
// Body: PParticleEmitter = CIntelligentPointer<CThing>(0) (base ctor 0x00A01B10, vptr 0x01238C6C, SetPItem 0x00A01B90,
// GetPItem 0x00A01B50, base dtor 0x00A01C10); CTCBase::OnCreate() (0x00686850, an out-of-line empty body); zero
// NoMovementTimer, Score, OffscreenCounter, MovementVector and Master; then the landed 0x008EC870 def fetch:
// GetDefinitionManager() (0x006869D0)->GetDef(ParentThing->DefIndex, &def) (0x0042AF3C), def->GetSubDef(&PCoopSpiritDef)
// (0x00670B90, the CCoopSpiritDef instance), release def (slot 1 at zero refs).
// Layout: Ego_r.pdb CTCCoopSpirit (0x34): NoMovementTimer +0xc, Score +0x10, OffscreenCounter +0x14, MovementVector
// +0x18, Master +0x24, PCoopSpiritDef +0x28, PParticleEmitter +0x2c (CIntelligentPointer<CThing>, 8 bytes); CTCBase
// ParentThing +4; CThing DefIndex word +0x8c. Intelligent pointer and def pointer models as the landed 0x00838A40 /
// 0x008EC870 rows.
// Shape: /O2 (16-aligned entry). The emitter reset sits in its own block so its 8-byte temporary's slot is reused by
// the def pointer (retail `sub esp,8`); GetDef / GetSubDef have stand-in bodies that write the out slot only inline
// (the landed 0x008EC870 idiom), which keeps the def object in esi across GetSubDef; MovementVector is assigned from a
// C3DVector(0, 0, 0) temporary through a user operator= that stores Z, Y, X (retail order 0x20, 0x1c, 0x18, with the
// ParentThing load scheduled after it; the X, Y, Z order or plain member stores hoist that load first).
class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                             // +0x8c
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CCoopSpiritDef;
class CDefClassBase { public:
    virtual void v00();
    virtual void DestroySelf();                          // slot 1 (+4)
    long RefCount;                                       // +0x04
    bool GetSubDef(CDefPointer<const CCoopSpiritDef>* out);   // 0x00670B90
};
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CDefClassBase>* out); // 0x0042AF3C
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in bodies (the real ones are out of line in retail): they write the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 / 0x008EC870 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const CCoopSpiritDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
void* FindDef_CDefinitionManager_GetDef_0();
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector& operator=(const C3DVector& o) { Z = o.Z; Y = o.Y; X = o.X; return *this; }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CPlayer;
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
    CDefinitionManager* GetDefinitionManager();          // 0x006869D0
    void OnCreate();                                     // 0x00686850
};
class CTCCoopSpirit : public CTCBase { public:
    long                              NoMovementTimer;   // +0x0c
    long                              Score;             // +0x10
    long                              OffscreenCounter;  // +0x14
    C3DVector                         MovementVector;    // +0x18
    CPlayer*                          Master;            // +0x24
    CDefPointer<const CCoopSpiritDef> PCoopSpiritDef;    // +0x28
    CIntelligentPointer<CThing>          PParticleEmitter;  // +0x2c
    void OnCreate();
};

void CTCCoopSpirit::OnCreate()
{
    {
        PParticleEmitter = CIntelligentPointer<CThing>(0);
    }
    CTCBase::OnCreate();
    MovementVector = C3DVector(0, 0, 0);
    NoMovementTimer = 0;
    Master = 0;
    OffscreenCounter = 0;
    Score = 0;
    {
        CDefPointer<CDefClassBase> def;
        GetDefinitionManager()->GetDef(ParentThing->DefIndex, &def);
        CDefClassBase* object = def.Object;
        object->GetSubDef(&PCoopSpiritDef);
        if (object != 0 && --object->RefCount == 0)
            object->DestroySelf();
    }
}