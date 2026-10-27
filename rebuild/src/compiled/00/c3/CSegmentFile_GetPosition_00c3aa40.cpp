// CSegmentFile::GetPosition() const -- absolute file offset of the segment cursor: range cursor (+0x78) plus range start (+0x70)
// Leaf: ?GetPosition@CSegmentFile@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3AA40, 7 bytes, thiscall, plain `ret`.
// Identity: the segment file of the landed SetRange 0x00C3AA50 (range first +0x70, count +0x74, cursor +0x78) and Read
//   0x00C3A9E0; reached through the libpk C-API forwarder 0x00C2DA10 (`jmp`), called from 0x00C170CC and 0x00C310F2. The
//   catalog label "CGameScriptThing::GetPThingUniqueID" is propagated. Descriptive (libpk, no PDB/RTTI).
// Shape: `m_cursor + m_first` (the cursor loaded first).
struct CSegmentFile {
    unsigned char _pad[0x70];
    unsigned long RangeFirst;                   // +0x70
    unsigned long RangeCount;                   // +0x74
    unsigned long RangeCursor;                  // +0x78
    unsigned long GetPosition() const;
};

unsigned long CSegmentFile::GetPosition() const
{
    return RangeCursor + RangeFirst;
}