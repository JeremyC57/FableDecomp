// CInputProcessCombat::GetCurrentRelativeMovementAngleXY() -- in the current player's first-person-style mode (CPlayer byte +0x220) the angle comes from the world's camera record (CWorld slot 6, a 56-byte record returned by value) through its GetAngleXY; otherwise from the current game camera (CWorld +0x34, slot 7)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068EA10, length 65 bytes (+ int3 padding).
// Callees: CWorld::GetCurrentPlayer @ 0x0049D9D0 (PlayerManager +0xc -> CPlayerManager::GetCurrentPlayer @ 0x00449970),
// SCameraRecord::GetAngleXY @ 0x00A0C380 (`fpatan` of the record's +0x1c/+0x18, labelled "Math_ComputeAngleXY"), CWorld vtable slot 6 and
// the camera's slot 7. World is CInputProcessGameBase::World (+0x14, engine/CInputProcessGameBase.h; the base is modelled locally for
// the virtual chain). Shape: a 0x38-byte frame for the hidden-return record, whose address comes back in eax and is used as `this`
// straight away (`mov ecx,eax`); both arms return a float on the x87 stack through `add esp,38h ; ret`.
struct CPlayer {
    unsigned char _pad[0x220];
    unsigned char m_firstPersonControl;             // +0x220 (the flag tested by this function)
};
struct CPlayerManager {
    CPlayer* GetCurrentPlayer();                    // 0x00449970
};
struct SCameraRecord_0068ea10 {                     // 56 bytes: the world's camera record returned by value
    unsigned char _body[0x38];
    float GetAngleXY() const;                       // 0x00A0C380
};
struct CAIGameCameraBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual float GetRelativeMovementAngleXY();     // slot 7
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual SCameraRecord_0068ea10 GetCameraRecord();   // slot 6
    unsigned char       _pad04[0x08];               // +0x04
    CPlayerManager*     PlayerManager;              // +0x0c
    unsigned char       _pad10[0x24];               // +0x10
    CAIGameCameraBase*  PCurrentGameCamera;         // +0x34
    CPlayer* GetCurrentPlayer();                    // 0x0049D9D0
};
struct CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout (CAInputProcess 0x14 bytes, then the init pointers)
    virtual void v00();
    unsigned char _pad04[0x10];                     // +0x04 CAInputProcess: PPlayerInterface, InputProcessListPos
    CWorld*       World;                            // +0x14
    void*         MyPlayer;                         // +0x18
    void*         DefinitionManager;                // +0x1c
    void*         Component;                        // +0x20
    void*         PGamePlayerInterface;             // +0x24
};
struct CInputProcessCombat : CInputProcessGameBase {
    float GetCurrentRelativeMovementAngleXY();
};

float CInputProcessCombat::GetCurrentRelativeMovementAngleXY()
{
    if (World->GetCurrentPlayer()->m_firstPersonControl)
        return World->GetCameraRecord().GetAngleXY();
    return World->PCurrentGameCamera->GetRelativeMovementAngleXY();
}
