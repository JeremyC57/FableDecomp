// CThing::SetInLimbo(bool inLimbo) -- when the limbo state changes: store it, refresh the thing-scan inclusion, tell every live component (with adding forbidden meanwhile), run the virtual limbo hook and broadcast a clear-primitives message to the components
// Leaf: SetInLimbo
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C8CF0, 173 bytes (+3 int3). `ret 4`.
// Callees: CThing::UpdateThingScanInclusion 0x004C8C00, CTCBase virtual slot 16 (+0x40, bool) per live component, own
//          virtual slot 14 (+0x38, bool), CBase::CBase 0x0099A2D0 / ~CBase 0x0099A2E0 around the message (vtable
//          0x01239A8C NTCMessages::CMessageClearPrimitives), CThing::SendMessageW(CMessage*) 0x004C7820.
// Shape: InLimbo is bit 5 of the +0x91 flag byte: the compare extracts it (`shr cl,5 ; and cl,1 ; cmp cl,bl`) and the
//        store merges the new bit (`shl dl,5 ; xor ; and 20h ; xor`); AddingTCsPermitted (bit 6 of +0x93) is cleared
//        before the loop and restored (`or ..,40h`) just before the slot-14 call; the message lives in the frame.
// Layout: Components pairs +0x44/+0x48; CTCBase ToBeRemoved +0x08.
struct CTCBase;
struct SComponentPair {                              // pair<ETCInterfaceType, CTCBase*>
    long     first;
    CTCBase* second;
};
struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void OnParentInLimbo_slot16(bool inLimbo);   // slot 16 (+0x40)
    void* ParentThing;                               // +0x04
    bool  ToBeRemoved;                               // +0x08
};
namespace N_0099a2d0 { struct CBase {
    virtual ~CBase();                                // 0x0099A2E0
    CBase();                                         // 0x0099A2D0
}; } using N_0099a2d0::CBase;
namespace NTCMessages {
struct CMessageClearPrimitives : CBase {             // vtable 0x01239A8C
    CMessageClearPrimitives() { }
};
}
struct CMessage;
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual void OnInLimboChanged_slot14(bool inLimbo);   // slot 14 (+0x38) 0x004CD1C0
    unsigned char   _pad04[0x44 - 0x04];
    SComponentPair* m_componentsFirst;               // +0x44 Components
    SComponentPair* m_componentsLast;                // +0x48
    unsigned char   _pad4c[0x91 - 0x4c];
    unsigned char   Dead : 1;                        // +0x91
    unsigned char   CreatedByOwnerThingCreation : 1;
    unsigned char   IncludeInThingScans : 1;
    unsigned char   Activated : 1;
    unsigned char   GloballyLoaded : 1;
    unsigned char   InLimbo : 1;                     // bit 5
    unsigned char   KillLocked : 1;
    unsigned char   CreatedBySerialisation : 1;
    unsigned char   _pad92;
    unsigned char   NotPausedDuringCutscenes : 1;    // +0x93
    unsigned char   DeathPersistent : 1;
    unsigned char   KillOnLevelUnload : 1;
    unsigned char   ActivatedScript : 1;
    unsigned char   LevelIsUnloading : 1;
    unsigned char   IsAffectedByBulletTimeOverride : 1;
    unsigned char   AddingTCsPermitted : 1;          // bit 6
    void UpdateThingScanInclusion();                 // 0x004C8C00
    void SendMessageW(CMessage* message);            // 0x004C7820
    void SetInLimbo(bool inLimbo);
};
void CThing::SetInLimbo(bool inLimbo)
{
    if (InLimbo != inLimbo) {
        InLimbo = inLimbo;
        UpdateThingScanInclusion();
        AddingTCsPermitted = false;
        for (SComponentPair* it = m_componentsFirst; it != m_componentsLast; ++it) {
            CTCBase* tc = it->second;
            if (!tc->ToBeRemoved)
                tc->OnParentInLimbo_slot16(inLimbo);
        }
        AddingTCsPermitted = true;
        OnInLimboChanged_slot14(inLimbo);
        NTCMessages::CMessageClearPrimitives message;
        SendMessageW((CMessage*)&message);
    }
}