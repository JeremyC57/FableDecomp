// CLoadLog::AddMessage() -- allocate a 12-byte log message {unread flag, empty CString, serial number}, register it through the log's virtual slot 9 and return its text for the caller to stream into
// Leaf: ?AddMessage@CLoadLog@@QAEAAVCString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C307E0, length 96 bytes.
// Naming (sweep1, 2026-10-25): was landed as CScriptHandleRegistry::CreateNewInstance (the manifest's
//   "C3DAnimFileXSequenceChunk::CreateNewInstance" label is propagated); descriptive (no PDB covers libpk). FINDINGS
//   chunk 436: every one of its 11 callers streams an error text into the returned object with the CString
//   `operator<<` family (e.g. CBankProcessor::AddSample 0x00C2E1D4 "Unknown sample format for index ", AddDriver
//   0x00C2E2D1 "Could not find resource number ", CIndexMap Add 0x00C3AB67 / 0x00C3B0F7 "Index <n> already exists!"),
//   and the log's +0x30 byte is the IsOk 0x00C307C0 the bank processor tests after each step. The global the landed
//   source called g_pCurrentOwner is MFC's _afxPchNil (the nil string's text, 0x0139C900, next to the _afxDataNil
//   0x0139C8FC that ~CString 0x00C0BA40 compares against), so the message's +4 word is an empty CString.
// Callees: operator new @ 0x00BFEA1A, virtual slot 9 (+0x24) on `this` with the new message.
// Globals: _afxPchNil @ 0x0139C900, the message serial counter @ 0x0143BB44 (post-incremented, re-read after the store
//   because the message could alias it).
// Shape: the virtual's result is discarded and the address of the embedded text (`lea eax,[esi+4]`, no null test for
//   a member address) is returned on both sides of the allocation test.
extern char* _afxPchNil;                        // 0x0139C900
extern long g_nextMessageSerial;                // 0x0143BB44

class CString {
public:
    char* m_pchData;                            // +0x00
    CString() { m_pchData = _afxPchNil; }       // MFC Init(): the shared empty string
};

struct SLogMessage {                            // 12 bytes
    bool    m_bRead;                            // +0x00
    CString m_text;                             // +0x04
    long    m_serial;                           // +0x08
    SLogMessage() : m_bRead(false) { m_serial = g_nextMessageSerial++; }
};

class CLoadLog {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Add(SLogMessage* message);     // slot 9 (+0x24)
    CString& AddMessage();
};

CString& CLoadLog::AddMessage()
{
    SLogMessage* message = new SLogMessage;
    Add(message);
    return message->m_text;
}
