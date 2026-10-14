// CWaypointDataOwner_00c17910::ReleaseWaypointData() -- deletes the owned WaypointData (non-virtual destructor, then operator delete) and nulls the pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17910, length 35 bytes.
// Callees: WaypointData::~WaypointData @ 0x00C2EF30 (non-virtual: called directly before the
// deallocation), operator delete @ 0x00BFE9BC.
// Shape: `if (p) delete p;` keeps retail's explicit null test around the destructor + operator delete
// pair; the null store follows inside the same branch (the manifest label CActiveFile::OnReadFinished
// is propagated; the owner class is modelled over its one pointer at +0).
struct WaypointData {
    ~WaypointData();                                         // 0x00C2EF30
};

struct CWaypointDataOwner_00c17910 {
    WaypointData* m_pWaypointData;                           // +0x00
    void ReleaseWaypointData();
};

void CWaypointDataOwner_00c17910::ReleaseWaypointData()
{
    if (m_pWaypointData) {
        delete m_pWaypointData;
        m_pWaypointData = 0;
    }
}
