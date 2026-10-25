// CBankProcessor::AddEvent(const CEventRecord& record) -- register the record's criteria-string trigger (not as a sub-event); when sub-events are enabled (+0x76) a record whose name ends in "_SUBn" is skipped here (AddSubEvent handles it)
// Leaf: ?AddEvent@CBankProcessor
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E8D0, length 144 bytes (129 + int3 padding), __fastcall, `ret 4`.
// Owner: the libpk bank processor (constructed at 0x00C2E0C0, run step by step by 0x00C2EB00, which tests the log's
//   IsOk @ 0x00C307C0 after each step): data +0x00, sound manager +0x04, log +0x08, driver map +0x0C, sample map +0x3C,
//   criteria-trigger registry +0x6C, number-trigger registry +0x70, option bytes +0x74..+0x7A. No PDB covers libpk: names are descriptive.
// Caller: 0x00C2EA53 (AddEvents). Callees: CString's copy constructor @ 0x00C0DAB0 and destructor @ 0x00C0BA40,
//   IsSubEventName @ 0x00C2ECD0 (same TU: strlen > 5 and strncmp(name + len - 5, "_SUB", 4) == 0), AddEventTrigger(record,
//   subEvent) @ 0x00C2E7A0 (registers the criteria-string trigger; "Duplicate event trigger for criteria string '").
// Name: the catalog's `AnimationEvent_Process` is a guess; descriptive.
// Shape: built under /EHsc. The name copy lives in the dead argument slot. Retail stores NO EH state between the copy's
//   construction and IsSubEventName, and builds the frame `mov eax,fs:[0]` first: the helper's body is in the TU and calls
//   only C runtime functions, so VC7.1 knows it cannot throw -- hence the same-TU body. It must have external linkage: a
//   `static` helper gets a custom convention (the name's address in eax, 1 grade). The copy is a block-scoped local destroyed
//   on both exits of the test.
extern "C" unsigned int __cdecl strlen(const char* text);
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);   // MSVCR71 (IAT 0x01440138)
#pragma intrinsic(strlen)

class CString {                              // libpk's MFC-style string (one pointer, 12-byte header in front)
public:
    char* m_pchData;
    CString(const CString& other);           // 0x00C0DAB0
    ~CString();                              // 0x00C0BA40
};

struct CEventRecord {                        // 12 bytes
    CString        m_name;                   // +0x00 criteria string
    unsigned long* m_pDrivers;               // +0x04 driver numbers
    unsigned long  m_driverCount;            // +0x08
};

struct CBankProcessor {
    unsigned char _pad_0x00[0x76];
    bool          m_bSubEvents;              // +0x76
    void AddEventTrigger(const CEventRecord& record, bool subEvent);   // 0x00C2E7A0
    void AddEvent(const CEventRecord& record);
};

// Same-TU body of 0x00C2ECD0 (retail's TU, just past this row's siblings): it calls only C runtime functions, so under
// /EHsc VC7.1 knows it cannot throw and stores no EH state for the name copy around it. Not graded from this file.
#pragma auto_inline(off)
__declspec(noinline) bool __fastcall IsSubEventName(const CString& name)
{
    if (strlen(name.m_pchData) > 5 && strncmp(name.m_pchData + strlen(name.m_pchData) - 5, "_SUB", 4) == 0)
        return true;
    return false;
}
#pragma auto_inline(on)

void CBankProcessor::AddEvent(const CEventRecord& record)
{
    if (m_bSubEvents) {
        CString name(record.m_name);
        if (IsSubEventName(name))
            return;
    }
    AddEventTrigger(record, false);
}