// CTavernGameShoveHaPennyDef::Transfer(CPersistContext& context) -- persist the shove-ha'penny def: the tavern-game base, the board regions, then the board/arrow indices, coin, joystick and power tunables and the round count
// Leaf: ?Transfer@CTavernGameShoveHaPennyDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D0A40, 1157 bytes, `ret 4`, virtual Transfer of CTavernGameShoveHaPennyDef.
// Callees: CTavernGameDef::Transfer 0x004ED472 (qualified base call),
//   CPersistContext::TransferVectorOfSubComponents<CShoveHaPennyRegionDef> 0x007D2740 (Regions),
//   CPersistContext::CheckCRC 0x00404500 (pooled empty key 0x0122D70E), CPersistTraits<long>::TransferBinaryIn
//   0x0040FE60 / CDataOutputStream::WriteLong 0x00993EB0, CPersistTraits<float>::TransferBinaryIn 0x0040EFB0 /
//   CDataOutputStream::WriteFloat 0x00993EE0.
// Shape: the landed CShoveHaPennyRegionDef::Transfer (0x007D0070) template -- each scalar member goes through the
// inline CPersistContext::Transfer<T>(name, value): CheckCRC, then `switch (mode)` with case 2 (binary load through
// the traits, an empty temporary homed in the dead `context` argument slot, `lea ecx,[esp+18h]`) and case 3 (save
// through the output stream). The member address lives in ebx; the final Rounds arms end in their own epilogues.
// The persist order is not the layout order: ImpulseScale..AverageJoystickMagAbove, MaxPower, AverageDrift, then the
// five PositionCoin floats, RestSpeed and Rounds.
// Layout: Ego_r.pdb CTavernGameShoveHaPennyDef (0x208) over the retail CTavernGameDef (0x1A4, 12-byte vectors):
// Regions +0x1A4 (PDB +0x1A8), the scalars 8 bytes below their PDB offsets (BoardDefIndex +0x1B0 .. Rounds +0x1FC).
class CMemoryDataInputStream;

class CDataOutputStream { public:
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<long> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, long* value);    // 0x0040FE60
    static void TransferBinaryOut(CDataOutputStream* stream, const long& value) { stream->WriteLong(value); }
};
template<> struct CPersistTraits<float> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, float* value);   // 0x0040EFB0
    static void TransferBinaryOut(CDataOutputStream* stream, const float& value) { stream->WriteFloat(value); }
};

class CShoveHaPennyRegionDef;
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };   // retail 12-byte STLport vector

class CPersistContext {
public:
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    void CheckCRC(const char* name);                    // 0x00404500
    template <class T> void TransferVectorOfSubComponents(vector<T>& value);   // <CShoveHaPennyRegionDef> 0x007D2740
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value);
            break;
        }
    }
};

class CTavernGameDef {
public:
    virtual void Transfer(CPersistContext& context);    // 0x004ED472
    unsigned char _body04[0x1A0];                       // +0x04..+0x1A3 (retail CTavernGameDef is 0x1A4)
};

class CTavernGameShoveHaPennyDef : public CTavernGameDef {
public:
    vector<CShoveHaPennyRegionDef> Regions;             // +0x1A4
    long  BoardDefIndex;                                // +0x1B0
    long  ArrowDefIndex;                                // +0x1B4
    float PositionCoinSpeedXbox;                        // +0x1B8
    float PositionCoinSpeedPC;                          // +0x1BC
    float PositionCoinMinimum;                          // +0x1C0
    float PositionCoinMaximum;                          // +0x1C4
    float PositionCoinDepth;                            // +0x1C8
    float ImpulseScale;                                 // +0x1CC
    float TargetDistanceScale;                          // +0x1D0
    float JoystickScaleXboxX;                           // +0x1D4
    float JoystickScaleXboxY;                           // +0x1D8
    float JoystickScalePCX;                             // +0x1DC
    float JoystickScalePCY;                             // +0x1E0
    float JoystickTranslateY;                           // +0x1E4
    float JoystickMagBelow;                             // +0x1E8
    float AverageJoystickMagAbove;                      // +0x1EC
    float AverageDrift;                                 // +0x1F0
    float MaxPower;                                     // +0x1F4
    float RestSpeed;                                    // +0x1F8
    long  Rounds;                                       // +0x1FC
    virtual void Transfer(CPersistContext& context);
};

void CTavernGameShoveHaPennyDef::Transfer(CPersistContext& context)
{
    CTavernGameDef::Transfer(context);
    context.TransferVectorOfSubComponents(Regions);
    context.Transfer("", BoardDefIndex);
    context.Transfer("", ArrowDefIndex);
    context.Transfer("", ImpulseScale);
    context.Transfer("", TargetDistanceScale);
    context.Transfer("", JoystickScaleXboxX);
    context.Transfer("", JoystickScaleXboxY);
    context.Transfer("", JoystickScalePCX);
    context.Transfer("", JoystickScalePCY);
    context.Transfer("", JoystickTranslateY);
    context.Transfer("", JoystickMagBelow);
    context.Transfer("", AverageJoystickMagAbove);
    context.Transfer("", MaxPower);
    context.Transfer("", AverageDrift);
    context.Transfer("", PositionCoinSpeedXbox);
    context.Transfer("", PositionCoinSpeedPC);
    context.Transfer("", PositionCoinMinimum);
    context.Transfer("", PositionCoinMaximum);
    context.Transfer("", PositionCoinDepth);
    context.Transfer("", RestSpeed);
    context.Transfer("", Rounds);
}