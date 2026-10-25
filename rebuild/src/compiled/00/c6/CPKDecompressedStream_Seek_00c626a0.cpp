// CPKDecompressedStream::Seek(long mode, float amount, long userData) -- convert an amount in one of three units to a decoder position and seek
// Leaf: ?Seek@CPKDecompressedStream@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C626A0, 164 bytes incl. the 5-entry jump table at 0x00C62730 (thiscall, `ret 0Ch`).
// Callees: the decompressor's vtable slots 1 (+4) and 4 (+0x10) (int unit getters), slot 2 (+8) Seek(position, flag);
//   `_ftol2` 0x00BFEA70; constant 0.001f (0x01266300).
// Identity: the manifest's "CWeaponVisualState_SetProperty" is a propagated label. +4 is the stream's decompressor (the
//   destructor 0x00C62750 frees it through CDecompressorInterface's null-checked release 0x00C73D90, ".\CDecompressorInterface.cpp"
//   "Null decompressor"); modes 1/2 = raw units, 3/4 = times slot 1, 5/other = slot 4 times amount / 1000; the odd mode of
//   each pair also sets the flag. Caller 0x00C47FC2. Descriptive class, member and slot names.
// Jump table checked by hand: retail 0x29/0x2E/0x34/0x39/0x4C from the entry = ours ($L345/$L346/$L348/$L349/$L351).
// Shape: fall-through `case 1: flag = true; case 2: ...`; `case 5:` falls into `default:`.
class CDecompressorInterface {
public:
    virtual ~CDecompressorInterface();
    virtual long GetUnitSize();                        // slot 1 (+0x04) (name inferred)
    virtual long Seek(long position, bool relative);   // slot 2 (+0x08) (bool name inferred)
    virtual void v03();
    virtual long GetRate();                            // slot 4 (+0x10) (name inferred: scaled by 1/1000)
    void Release();                                    // 0x00C73D90
};
class CPKSourceObject {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Close();                              // slot 3 (+0x0c)
};
class CPKDecompressedStream {
public:
    CPKSourceObject*        m_pSource;                 // +0x00
    CDecompressorInterface* m_pDecompressor;           // +0x04
    long                    m_position;                // +0x08
    long                    m_userData;                // +0x0c
    bool                    m_bFlag;                   // +0x10
    long Seek(long mode, float amount, long userData);
    ~CPKDecompressedStream();
};

long CPKDecompressedStream::Seek(long mode, float amount, long userData)
{
    if (!m_pDecompressor)
        return -1;
    bool relative = false;
    long position;
    switch (mode)
    {
    case 1:
        relative = true;
    case 2:
        position = (long)amount;
        break;
    case 3:
        relative = true;
    case 4:
        position = (long)(m_pDecompressor->GetUnitSize() * amount);
        break;
    case 5:
        relative = true;
    default:
        position = (long)(m_pDecompressor->GetRate() * amount * 0.001f);
        break;
    }
    m_position = m_pDecompressor->Seek(position, relative);
    m_userData = userData;
    m_bFlag = false;
    return m_position;
}