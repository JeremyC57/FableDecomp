// CBankData::CBankData() -- EH-framed constructor of the three 0x10-byte record lists built out of line (samples @ 0x00C304F0 at +0, drivers @ 0x00C305E0 at +0x10, events @ 0x00C306D0 at +0x20; EH states 0 and 1)
// Leaf: ??0CBankData@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2EED0, oracle length 81 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CFirst::CFirst ; lea ecx,[esi+10h] ; mov dword ptr [esp+10h],0 ; call CSecond::CSecond ; lea ecx,[esi+20h] ; mov byte ptr [esp+10h],1 ; call CThird::CThird ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 433 lists this pair as suspected libpk): the 0x30-byte libpk bank data
//   whose three polymorphic record lists the sweep's CBankData rows read -- CBankData::Prepare 0x00C2ED50 (slot 5 of
//   +0 / +0x10 / +0x20), CBankProcessor::AddSamples 0x00C2E270 (samples +0, GetItem/GetCount), AddDrivers / AddEvents
//   (+0x10 / +0x20). The constructor's callers are the libpk bank loaders 0x00C16DAF / 0x00C1727B / 0x00C17984 (a frame
//   CBankData built before the bank file is read), the destructor runs at their exits and in their unwind funclets.
//   The member classes are modelled as one template (three same-size constructors 0x00C304F0 / 0x00C305E0 /
//   0x00C306D0 and destructors 0x00C30550 / 0x00C30640 / 0x00C30730). Descriptive names (no PDB covers libpk).
// Label correction: the landed row was WaypointData_00c2eed0::WaypointData_00c2eed0() over address-named members (manifest "WaypointData::WaypointData", propagated).
struct SSampleHeader;
struct SDriverRecord;
struct CEventRecord;

template <class T>
struct CRecordList {                            // the bank data's polymorphic record lists (0x10 bytes)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool Prepare();                     // slot 5 (+0x14)
    unsigned char _body[0x0c];
    CRecordList();
    ~CRecordList();
};

struct CBankData {
    CRecordList<SSampleHeader> m_samples;       // +0x00 (constructor 0x00C304F0, destructor 0x00C30550)
    CRecordList<SDriverRecord> m_drivers;       // +0x10 (constructor 0x00C305E0, destructor 0x00C30640)
    CRecordList<CEventRecord>  m_events;        // +0x20 (constructor 0x00C306D0, destructor 0x00C30730)
    CBankData();
    ~CBankData();
};

CBankData::CBankData()
    : m_samples(), m_drivers(), m_events()
{
}
