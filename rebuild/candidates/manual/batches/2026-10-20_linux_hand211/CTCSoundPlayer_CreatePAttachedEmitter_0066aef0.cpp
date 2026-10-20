// CTCSoundPlayer::CreatePAttachedEmitter(bool unattached) -- the attached sound emitter: the live one, else a new emitter at the owner's position when that position is inside the world map
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066AEF0, 165 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem const @ 0x00A01B50, CTCBase::GetWorld @ 0x00686A70, CWorld virtual slot 12
// GetWorldMap (+0x30), CWorldMap::IsPosInMap(const C3DVector&) const @ 0x004FC600, CThingSoundEmitter::Create(const C3DVector&,
// const CThing*, bool) @ 0x008369F0 (static, __fastcall), CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 /
// dtor @ 0x00A01C10 (the CIntelligentPointer<CThingSoundEmitter> temporary, vtable 0x0125B1B8).
// Layout: CTCSoundPlayer PAttachedEmitter (CIntelligentPointer<CThingSoundEmitter>) +0x0c; CThing PhysicsTC +0x60 (position at
// +0x0c, the inline body of CThing::GetPosition 0x004C73D0), dead flag bit 0 of +0x91.
// Shape: the position reference is taken first; the alive test falls through to the creation, whose IsPosInMap argument is
// pushed before the GetWorldMap virtual call; the new emitter is stored through a CIntelligentPointer temporary only when
// non-null and returned either way; both other exits return PAttachedEmitter.GetPItem().
class CThing;
class CBaseObject;
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
    C3DVector Position;                                 // +0x0c
};
class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
};
struct CThingSoundEmitter {
    static CThingSoundEmitter* __fastcall Create(const C3DVector& position, const CThing* parent, bool unattached);   // 0x008369F0
};
struct CWorldMap {
    bool IsPosInMap(const C3DVector& pos) const;        // 0x004FC600
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                   // slot 12 (+0x30)
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0125B1B8
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.CBaseIntelligentPointer::GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
    CWorld* GetWorld();                                 // 0x00686A70
};
struct CTCSoundPlayer : CTCBase {
    CIntelligentPointer<CThingSoundEmitter> PAttachedEmitter;   // +0x0c
    CThingSoundEmitter* CreatePAttachedEmitter(bool unattached);
};

CThingSoundEmitter* CTCSoundPlayer::CreatePAttachedEmitter(bool unattached)
{
    const C3DVector& position = ParentThing->PhysicsTC->Position;
    CThing* current = PAttachedEmitter.CBaseIntelligentPointer::GetPItem();
    if (!current || (current->m_flags91 & 1)) {
        if (GetWorld()->GetWorldMap()->IsPosInMap(position)) {
            CThingSoundEmitter* emitter = CThingSoundEmitter::Create(position, ParentThing, unattached);
            if (emitter)
                PAttachedEmitter = CIntelligentPointer<CThingSoundEmitter>(emitter);
            return emitter;
        }
    }
    return PAttachedEmitter.GetPItem();
}
