// CTCSmokeGenerator::FrameUpdate() -- once the generation positions exist, give every position without a live smoke thing a new particle-emitter effect thing there; otherwise create the positions
// Grade: DIFFER(236v228) -- 8-aligned frame and call sequence match; retail uses ebp as a second induction register (&pos->PSmoke in ebx, pos in ebp) and reuses the def out slot for the effect thing, the lane spills the PSmoke address and gives the thing its own word (an inline fetch helper and a reference local made it worse)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00722740, 228 bytes (plain `ret`; 8-aligned frame).
// Callees: CTCBase::GetWorld @ 0x00686A70, the current game camera's virtual slot 11 GetPosition (+0x2c, C3DVector through the
// hidden pointer; the result is not used), CBaseIntelligentPointer::GetPItem const @ 0x00A01B50, CDefClassBase::
// GetSubDef_00722d30(CDefPointer&) @ 0x00722D30 (the 'CSmokeGeneratorDef' instantiation), CreateEffectThing(long, const
// C3DVector*, bool) @ 0x006E0880 (__fastcall), CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor
// @ 0x00A01C10 (the CIntelligentPointer<CThing> temporary, vtable 0x01238C6C), CTCSmokeGenerator::CreatePositions @ 0x00722590;
// the def's virtual slot 1.
// Layout: CTCSmokeGenerator GenerationDistance +0x0c, GenerationPositions (vector<CGenerationPos>, 0x14-byte {C3DVector Pos,
// CIntelligentPointer<CThing> PSmoke}) +0x10, PositionsCreated +0x1c (the landed OnKill 0x007224E0 model); CThing PDef +0x70;
// CSmokeGeneratorDef ParticleEmitter +0x2c; CWorld PCurrentGameCamera +0x34.
// Shape: the def is read once from its out slot into a holder (esi), whose slot then holds the new effect thing; the smoke
// pointer is assigned from a CIntelligentPointer temporary through GetPItem/SetPItem.
class CThing;
class CBaseObject;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
};
struct CAIGameCameraBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual C3DVector GetPosition() const;              // slot 11 (+0x2c)
};
struct CWorld {
    unsigned char _pad[0x34];
    CAIGameCameraBase* PCurrentGameCamera;              // +0x34
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefOut {                     // the out slot (released by the holder below)
    T* Object;
    CDefOut() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def read once from the out slot, released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CSmokeGeneratorDef : CDefBase {
    unsigned char _pad08[0x2c - 0x08];
    long ParticleEmitter;                               // +0x2c
};
struct CDefClassBase : CDefBase {
    bool GetSubDef_00722d30(CDefOut<CSmokeGeneratorDef>& out);   // 0x00722D30
};
class CThing {
public:
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
struct CGenerationPos {                                 // CTCSmokeGenerator::CGenerationPos, 0x14 bytes
    C3DVector Pos;                                      // +0x00
    CIntelligentPointer<CThing> PSmoke;                 // +0x0c
};
struct CTCSmokeGenerator {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
    float GenerationDistance;                           // +0x0c
    CGenerationPos* GenerationPositions_first;          // +0x10
    CGenerationPos* GenerationPositions_last;           // +0x14
    CGenerationPos* GenerationPositions_end;            // +0x18
    bool PositionsCreated;                              // +0x1c
    CWorld* GetWorld();                                 // 0x00686A70 (CTCBase)
    void CreatePositions();                             // 0x00722590
    __forceinline CSmokeGeneratorDef* FetchSmokeGeneratorDef()
    {
        CDefOut<CSmokeGeneratorDef> defOut;
        ParentThing->PDef->GetSubDef_00722d30(defOut);
        return defOut.Object;
    }
    void FrameUpdate();
};

void CTCSmokeGenerator::FrameUpdate()
{
    if (PositionsCreated) {
        for (CGenerationPos* pos = GenerationPositions_first; pos != GenerationPositions_last; ++pos) {
            C3DVector cameraPosition = GetWorld()->PCurrentGameCamera->GetPosition();
            if (!pos->PSmoke.GetPItem()) {
                CDefOut<CSmokeGeneratorDef> defOut;
                ParentThing->PDef->GetSubDef_00722d30(defOut);
                CDefHolder<CSmokeGeneratorDef> def(defOut.Object);
                CThing* smoke = CreateEffectThing(def->ParticleEmitter, &pos->Pos, false);
                pos->PSmoke = CIntelligentPointer<CThing>(smoke);
            }
        }
    } else
        CreatePositions();
}
