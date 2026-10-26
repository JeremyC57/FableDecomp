// CTCLook::FrameUpdateHeadLookAtPos(const C3DVector& pos, bool turnBody) -- per-frame head tracking of a world position: angles from the body to the position (eyes too when the eyes flag is set), point the eyes, then turn the head with the speed chosen by the look-speed flags
// Leaf: ?FrameUpdateHeadLookAtPos@CTCLook@@AAEXABVC3DVector@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007530D0, length 150 bytes (`ret 8`).
// Callees (Ego_r/FableWin PDB signatures, all private members of CTCLook): GetAnglesFromBodyToPos(const C3DVector&, bool,
// float&, float&) const @ 0x00751C70, SetEyesLooking(const C3DVector&) @ 0x00752930, SetHeadLooking(float, bool, bool, float,
// float) @ 0x00752B00 (the catalog's "CTCLook_UpdateHeadDirection" is an autoname; chunk 477 graded the body as SetHeadLooking).
// Layout (retail, local model): LookFlags +0x64 (bit 2 eyes, bits 3/4/5 slow/medium/fast head speed 0.33/0.66/1.0).
// Shape: the eyes bool is kept in the first angle's frame word and pushed as a dword (both calls); the speed local reuses the dead
// pos argument slot; angles declared YZ first so XY gets the lower frame word.
class C3DVector { public: float X, Y, Z; };
class CTCLook {
public:
    virtual void v00();
    unsigned char _pad04[0x64 - 0x04];
    long LookFlags;                             // +0x64
private:
    void GetAnglesFromBodyToPos(const C3DVector& pos, bool includeEyes, float& angleXY, float& angleYZ) const;   // 0x00751C70
    void SetEyesLooking(const C3DVector& pos);  // 0x00752930
    void SetHeadLooking(float speed, bool includeEyes, bool turnBody, float angleXY, float angleYZ);   // 0x00752B00
    void FrameUpdateHeadLookAtPos(const C3DVector& pos, bool turnBody);
};

void CTCLook::FrameUpdateHeadLookAtPos(const C3DVector& pos, bool turnBody)
{
    float angleYZ;
    float angleXY;
    bool includeEyes = (LookFlags & 4) != 0;
    GetAnglesFromBodyToPos(pos, includeEyes, angleXY, angleYZ);
    if (includeEyes)
        SetEyesLooking(pos);
    float speed = 0.0f;
    if (LookFlags & 8)
        speed = 0.33f;
    else if (LookFlags & 0x10)
        speed = 0.66f;
    else if (LookFlags & 0x20)
        speed = 1.0f;
    SetHeadLooking(speed, includeEyes, turnBody, angleXY, angleYZ);
}
