// CTCLook::FrameUpdateHeadInterested() -- per-frame head tracking of whatever the parent's targeting finds interesting: the camera when the targeting says so, else the nearest interesting thing's head (its look component), focal point or position; with neither, centre the head
// Grade: DIFFER(298v297) -- block layout: retail keeps a dead xor esi,esi for the target and lays the camera block as the fall-through of the first FocusOnCamera test (the target path's head/focal/position block after it); VC7.1 here drops the dead zero and puts the target block first with the camera block cross-jumped to the end. Tried: if/else orders, negated inner test, declaration order, ternary target.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753380, length 297 bytes (+ int3 padding), plain `ret`. FableWin.pdb
// ?FrameUpdateHeadInterested@CTCLook@@AAEXXZ (private).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (the inline GetInterface for type 8 = CTCTargetingBase and type 0x43 =
// CTCLook; the bitset tests fold to byte tests), CTCTargetingBase::GetPNearestInterestingThing @ 0x007F5860, CTCBase::GetWorld
// @ 0x00686A70 and the world's +0x34 object's virtual slot 11 (+0x2C, the camera position by hidden pointer; names
// descriptive), CTCLook::GetHeadPos() const @ 0x00751B30 (hidden pointer), CTCGraphicAppearance::GetFocalPosIfLoaded(C3DVector*,
// float) @ 0x004C11D0 (1.0f), CTCLook::CentreHead @ 0x007528A0, CTCLook::FrameUpdateHeadLookAtPos(const C3DVector&, bool) @
// 0x007530D0.
// Shape: the landed TrackNearestInterestingThing (0x00753840) GetInterface (the targeting pointer left unset on a miss: retail
// reads the key's frame word), the target preloaded with 0 (esi); the look position is a frame C3DVector assigned from the
// by-value camera / head temporaries (one shared 3-dword copy) or the thing's position; the medium-speed default flag
// (0x10) is set when no speed bit (0x38) is.
// Layout (retail CTCLook, see 0x00751C70): ParentThing +0x04, LookFlags +0x64; CTCTargetingBase camera flag +0x34; CThing
// UsingComponents +0x20, components +0x44, PhysicsTC +0x60 (Position +0x0c), graphic appearance +0x64.
extern "C" int  __cdecl puts(const char* text);    // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CPresenceBits {                             // std::bitset<274>
    unsigned long words[9];
    bool test(unsigned long position) const
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (words[position >> 5] & (1UL << (position & 31))) != 0;
    }
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
    C3DVector Position;                              // +0x0c
};
struct CTCGraphicAppearance {
    bool GetFocalPosIfLoaded(C3DVector* pos, float scale);   // 0x004C11D0
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits UsingComponents;                   // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad_0x4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                       // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (UsingComponents.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
struct CTCTargetingBase {                            // interface type 8
    unsigned char _pad00[0x34];
    bool FocusOnCamera;                              // +0x34 (name descriptive)
    CThing* GetPNearestInterestingThing();           // 0x007F5860
};
struct CCameraOwner {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual C3DVector GetCameraPosition();           // slot 11 (+0x2C)
};
struct CWorld {
    unsigned char _pad00[0x34];
    CCameraOwner* Camera;                            // +0x34
};

struct CTCLook {
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04
    unsigned char _pad08[0x64 - 0x08];
    long          LookFlags;                         // +0x64
    CWorld* GetWorld();                              // 0x00686A70 (CTCBase)
    C3DVector GetHeadPos() const;                    // 0x00751B30
    void CentreHead();                               // 0x007528A0
    void FrameUpdateHeadLookAtPos(const C3DVector& pos, bool turnBody);   // 0x007530D0
    void FrameUpdateHeadInterested();
};

void CTCLook::FrameUpdateHeadInterested()
{
    CTCTargetingBase* targeting;
    CThing* target = 0;
    if (ParentThing->GetInterface(&targeting, 8))
        target = targeting->GetPNearestInterestingThing();
    C3DVector pos;
    if (target) {
        if (targeting->FocusOnCamera) {
            pos = GetWorld()->Camera->GetCameraPosition();
        } else {
            CTCLook* look;
            if (target->GetInterface(&look, 0x43))
                pos = look->GetHeadPos();
            else if (!(target->PTCGraphicAppearance && target->PTCGraphicAppearance->GetFocalPosIfLoaded(&pos, 1.0f)))
                pos = target->PhysicsTC->Position;
        }
    } else if (targeting->FocusOnCamera) {
        pos = GetWorld()->Camera->GetCameraPosition();
    } else {
        CentreHead();
        return;
    }
    if (!(LookFlags & 0x38))
        LookFlags |= 0x10;
    FrameUpdateHeadLookAtPos(pos, false);
}
