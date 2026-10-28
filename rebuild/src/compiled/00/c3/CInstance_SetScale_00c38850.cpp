// CInstance::SetScale(float scale) -- store the instance's scale (+0x17C) and pass it on to the attached frame streamer (its slot-5 virtual), if any
// Leaf: ?SetScale@CInstance@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C38850, 28 bytes, thiscall, `ret 4` (the notification is a tail `jmp [edx+14h]`).
// Identity: a method of libpk's CInstance (the 0x180-byte record of .\CInstance.cpp; frame streamer pointer +0x16C as in
//   the landed CheckFrameStreamer 0x00C388A0). Its two callers walk CInstanceManager's two instances at +0x18: the
//   manager constructor 0x00C2D2F0 (passing its sixth argument) and 0x00C2D1E0 (passing its argument). No PDB covers
//   libpk: the name is descriptive (the value is a float: 0x00C2D1E0's caller 0x00C15B90 passes the result of 0x00C15A70,
//   which the landed C3DMeshObject ctor 0x00C16050 calls GetScale; the manager ctor passes that same scale). Label corrections: "CBankFile::SetBankUpdateStatus" (manifest) and the caller-side
//   spellings CBankFile_00c2d1e0::SetBankUpdateStatus / CMeshPart_00c2d2f0::Bind are placeholders for this one method.
// Shape: store, reload the streamer pointer, null test, then the streamer call takes the same argument, so VC7.1 tail-jumps
//   through slot 5 (+0x14) reusing the caller's argument slot (`jmp [edx+14h]`). The value must be a float: with a
//   pointer or integer argument VC7.1 re-stores the copy in eax into the slot before the jump (`mov [esp+4],eax`).
class CFrameStreamer {                          // the instance's frame streamer (+0x16C)
public:
    virtual void v0();
    virtual long IsReady();                     // slot 1 (+0x04)
    virtual void v2();
    virtual long Stream();                      // slot 3 (+0x0c)
    virtual void v4();
    virtual void SetScale(float scale);     // slot 5 (+0x14)
};
class CInstance {                               // 0x180 bytes (.\CInstance.cpp)
public:
    void SetScale(float scale);
    unsigned char   _pad000[0xf8];
    long            m_state;                    // +0x0F8
    unsigned char   _pad0fc[0x16c - 0xfc];
    CFrameStreamer* m_pFrameStreamer;           // +0x16C
    unsigned char   _pad170[0xc];
    float           m_scale;                 // +0x17C
};

void CInstance::SetScale(float scale)
{
    m_scale = scale;
    if (m_pFrameStreamer)
        m_pFrameStreamer->SetScale(scale);
}