// CBankProcessor::AddSubEvent(const CEventRecord& record) -- when sub-events are enabled (+0x76) and the record's name ends in "_SUBn", register a copy of the record under the name with its five-character suffix cut off, as a sub-event trigger
// Leaf: ?AddSubEvent@CBankProcessor
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E960, length 208 bytes (207 + int3 padding), __fastcall, `ret 4`.
// Owner: the libpk bank processor (constructed at 0x00C2E0C0, run step by step by 0x00C2EB00, which tests the log's
//   IsOk @ 0x00C307C0 after each step): data +0x00, sound manager +0x04, log +0x08, driver map +0x0C, sample map +0x3C,
//   criteria-trigger registry +0x6C, number-trigger registry +0x70, option bytes +0x74..+0x7A. No PDB covers libpk: names are descriptive.
// Caller: 0x00C2EA93 (AddSubEvents). Callees: CString's copy constructor @ 0x00C0DAB0, operator= @ 0x00C0C0C0, SetAt @ 0x00C0C970
//   (landed), SafeStrlen @ 0x00C0BCC0 (landed, `__stdcall`), destructor @ 0x00C0BA40; IsSubEventName @ 0x00C2ECD0 (same TU);
//   CEventRecord's copy constructor @ 0x00C3B710 (name, then the driver-number array copied) and destructor @ 0x00C3B6B0 (frees
//   the array, then the name); AddEventTrigger(record, subEvent) @ 0x00C2E7A0.
// Name: the catalog's `AnimationEvent_ProcessMapping` is a guess; descriptive.
// Shape: built under /EHsc. The flag test returns before edi is saved; the name copy lives in the dead argument slot (EH
//   state 0), the record copy in the frame (state 1, a byte store placed between the operator= push and call). The cut is
//   `SetAt(SafeStrlen(name) - 5, 0)`: the 0 is pushed before the length is computed.
extern "C" unsigned int __cdecl strlen(const char* text);
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);   // MSVCR71 (IAT 0x01440138)
#pragma intrinsic(strlen)

class CString {                              // libpk's MFC-style string (one pointer, 12-byte header in front)
public:
    char* m_pchData;
    CString(const CString& other);           // 0x00C0DAB0
    ~CString();                              // 0x00C0BA40
    const CString& operator=(const CString& other);   // 0x00C0C0C0
    void SetAt(int index, char ch);          // 0x00C0C970
    static int __stdcall SafeStrlen(const char* text);   // 0x00C0BCC0
};

struct CEventRecord {                        // 12 bytes
    CString        m_name;                   // +0x00 criteria string
    unsigned long* m_pDrivers;               // +0x04 driver numbers
    unsigned long  m_driverCount;            // +0x08
    CEventRecord(const CEventRecord& other); // 0x00C3B710
    ~CEventRecord();                         // 0x00C3B6B0
};

struct CBankProcessor {
    unsigned char _pad_0x00[0x76];
    bool          m_bSubEvents;              // +0x76
    void AddEventTrigger(const CEventRecord& record, bool subEvent);   // 0x00C2E7A0
    void AddSubEvent(const CEventRecord& record);
};

// Same-TU body of 0x00C2ECD0 (see 0x00C2E8D0). Not graded from this file.
#pragma auto_inline(off)
__declspec(noinline) bool __fastcall IsSubEventName(const CString& name)
{
    if (strlen(name.m_pchData) > 5 && strncmp(name.m_pchData + strlen(name.m_pchData) - 5, "_SUB", 4) == 0)
        return true;
    return false;
}
#pragma auto_inline(on)

void CBankProcessor::AddSubEvent(const CEventRecord& record)
{
    if (!m_bSubEvents)
        return;
    CString name(record.m_name);
    if (IsSubEventName(name)) {
        if (CString::SafeStrlen(name.m_pchData) > 5)
            name.SetAt(CString::SafeStrlen(name.m_pchData) - 5, 0);
        CEventRecord subRecord(record);
        subRecord.m_name = name;
        AddEventTrigger(subRecord, true);
    }
}