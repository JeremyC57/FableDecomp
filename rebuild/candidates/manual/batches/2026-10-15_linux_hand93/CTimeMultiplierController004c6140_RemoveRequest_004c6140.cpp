// CTimeMultiplierController_004c6140::RemoveRequest(CThing* requester) -- erases the requester's entry from the multiplier request map, flags the controller when no request is left and recomputes the minimum multiplier
// Leaf: RemoveRequest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C6140, length 35 bytes (+13 int3 padding); speed-optimised TU.
// The catalog label "CTCInventoryExperience::ProcessButtonBReleased" is propagated and wrong: the body reads +0x04 / writes +0x0D
// (CTCInventoryExperience's own members start at +0x14C), and its eight callers (0x0079D031, 0x007AA555, ... 0x00890B3F) fetch the
// object from `[GetX()+68h]` and pass a thing pointer. 0x004C5F10 (manifest "BulletTimeController_UpdateMinMultiplier") walks the
// same map from +0 for the smallest float value starting at 1.0, so this is the time-multiplier (bullet time) controller's
// "remove request"; class and method names are descriptive.
// Callees: the map's erase(const key&) @ 0x004C6B60 (equal_range + range erase, returns the count; result ignored),
// UpdateMinMultiplier @ 0x004C5F10.
// Shape: the key is passed by reference to the by-value parameter's slot (`lea eax,[esp+8] ; push eax`); the count is read through
// an inline size() (`mov eax,[esi+4] ; test eax,eax`).
struct CThing;
struct CMultiplierRequestMap {                                   // retail std::map<CThing*, float> header + count
    void*         m_header;                                      // +0x00
    unsigned long m_count;                                       // +0x04
    unsigned long size() const { return m_count; }
    unsigned long erase(CThing* const& key);                     // 0x004C6B60
};
struct CTimeMultiplierController_004c6140 {
    CMultiplierRequestMap m_requests;                            // +0x00
    unsigned char         _pad_0x08[0x5];
    bool                  m_noRequests;                          // +0x0D
    void UpdateMinMultiplier();                                  // 0x004C5F10
    void RemoveRequest(CThing* requester);
};
void CTimeMultiplierController_004c6140::RemoveRequest(CThing* requester)
{
    m_requests.erase(requester);
    if (m_requests.size() == 0)
        m_noRequests = true;
    UpdateMinMultiplier();
}
