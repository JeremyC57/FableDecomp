// CTCBed::FrameUpdate() -- each frame, run the bed's animation update when its def says the bed animates
// Leaf: ?FrameUpdate@CTCBed@@UAEXXZ
// Retail: Fable.exe @ 0x00746B20, 16 bytes: `mov eax,[ecx+14h] ; mov dl,[eax+38h] ; test dl,dl ; je ; jmp 0x007468C0 ; ret`
// (CTCBed vtable 0x01268964 slot 9).
// Identity (proven): CTCBase vtable slot 9 is FrameUpdate (landed CTCHeroStats::FrameUpdate 0x0057D5A0 and
// CTCInventoryStats::FrameUpdate 0x005CDCCB sit in slot 9) and Ego_r.pdb names ?FrameUpdate@CTCBed@@UAEXXZ. The callee is
// the landed private CTCBed::UpdateAnimation 0x007468C0, entered with a tail jump.
// Layout: Ego_r.pdb CTCBed (PDef +0x14, the CDefPointer<const CBedDef> object word) and CBedDef::Animating +0x38, the
// offsets the landed StartGetInBedAnimation 0x007469C0 uses.
class CBedDef { public:
    unsigned char _pad00[0x28];
    long GetInBedAnimName;                          // +0x28 CDefString
    long SleepInBedAnimName;                        // +0x2c CDefString
    long GetUpFromBedAnimName;                      // +0x30 CDefString
    long GetUpFromBedFastAnimName;                  // +0x34 CDefString
    bool Animating;                                 // +0x38
};
template<class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class CThing;
class CTCBase { public:                             // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void FrameUpdate();                     // slot 9
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};
class CTCBed : public CTCBase { public:
    bool  Empty;                                    // +0x0c
    bool  GettingIn;                                // +0x0d
    bool  GettingOut;                               // +0x0e
    bool  Animating;                                // +0x0f
    long  BedSleepAnim;                             // +0x10
    CDefPointer<const CBedDef> PDef;                // +0x14
    void FrameUpdate();
private:
    void UpdateAnimation();                         // 0x007468C0
};

void CTCBed::FrameUpdate()
{
    if (PDef->Animating)
        UpdateAnimation();
}