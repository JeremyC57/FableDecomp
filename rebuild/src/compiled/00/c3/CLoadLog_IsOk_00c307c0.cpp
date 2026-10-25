// CLoadLog::IsOk() const -- true while no error has been logged: returns the log's status byte at +0x30
// Leaf: ?IsOk@CLoadLog@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C307C0, 4 bytes: mov al,[ecx+30h] ; ret
// Naming (sweep1, 2026-10-25): was landed as CTCInventoryBase::GetCanItemBeQuickAssigned (a propagated manifest label;
//   the catalog has the placeholder `GetMember_byte0x30_00c307c0`); descriptive (no PDB covers libpk). FINDINGS chunk
//   436: the bank processor tests it on its load log (+8) before every step of CBankProcessor::Load 0x00C2EB00
//   (0x00C2EB06..) and after each item of AddSamples 0x00C2E270 (0x00C2E294) / AddDrivers 0x00C2E480 (0x00C2E4AB); the
//   bank loader 0x00C16CE0's callers test the same log (0x00C16DD0, 0x00C1729D, 0x00C1738B, 0x00C176BE). The log
//   object is the one CLoadLog::AddMessage 0x00C307E0 registers messages in.
class CLoadLog {
public:
    virtual void v00();
    unsigned char _pad04[0x2c];
    bool m_ok;                                  // +0x30
    bool IsOk() const;
};

bool CLoadLog::IsOk() const
{
    return m_ok;
}
