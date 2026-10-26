// CPlayer::Uninitialise() -- when initialised and a player character exists, kill it and drop the pointer (assign a null intelligent pointer), then the CInitBaseClass uninitialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487AF0, length 113 bytes (+ int3 padding).
// Leaf: ?Uninitialise@CPlayer@@QAEXXZ
// Callees: CInitBaseClass::IsInitialised @ 0x0099A330 (catalogued as the propagated "CTCInventoryBase::
// GetCanItemBeQuickAssigned"), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CThing::Kill(bool) @ 0x004C9B80,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::SetPItem @ 0x00A01B90 (x2), ::~CBaseIntelligentPointer
// @ 0x00A01C10, CInitBaseClass::Uninitialise @ 0x0099A360; CIntelligentPointer<CThingPlayerCreature> vtable 0x01231C4C.
// Layout: engine/CPlayer.h places PlayerCharacter (CIntelligentPointer<CThingPlayerCreature>, 8 bytes) at +0x34 but as
// opaque bytes; the member is modelled locally with the header's offset.
// Shape: `IsInitialised() == true` (`cmp al,1`); the pointer member kept in esi; `GetPItem()->Kill(false)` pushes the
// argument before the second GetPItem; `PlayerCharacter = CIntelligentPointer(0)` as the landed CThingGameObject::OnCreate
// 0x00838A40 (out-of-line base ctor, derived vtable store, SetPItem(0); inline operator= `SetPItem(other.GetPItem())`;
// base dtor); the base Uninitialise is a plain call (the 8-byte frame is popped after it); `ret`.
class CThingPlayerCreature { public:
    void Kill(bool removeImmediately);             // 0x004C9B80 (CThing::Kill)
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThingPlayerCreature>, vtable 0x01231C4C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CInitBaseClass { public:
    virtual void v00();
    bool IsInitialised() const;                    // 0x0099A330
    void Uninitialise();                           // 0x0099A360
};

class CPlayer : public CInitBaseClass { public:                  // engine/CPlayer.h layout
    unsigned char                     _pad04[0x34 - 0x04];
    CIntelligentPointer<CThingPlayerCreature> PlayerCharacter;   // +0x34
    void Uninitialise();
};

void CPlayer::Uninitialise()
{
    if (IsInitialised() == true) {
        if (PlayerCharacter.GetPItem()) {
            PlayerCharacter.GetPItem()->Kill(false);
            PlayerCharacter = CIntelligentPointer<CThingPlayerCreature>(0);
        }
    }
    CInitBaseClass::Uninitialise();
}