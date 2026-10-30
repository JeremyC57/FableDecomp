// PKSerialiseLoad<CBankData>(IRemoteSource* source) -- libpk PKSerialiseLoad.h: read the whole source into a memory stream, allocate a CBankData under an auto-delete owner (trace "EMEM", PKSerialiseLoad.h line 34, when the allocation fails), transfer it from the stream through a reading parser and hand it out, or delete it and return null when the transfer fails
// Leaf: ??$PKSerialiseLoad@UCBankData@@@@YIPAUCBankData@@PAUIRemoteSource@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C17940, 393 bytes, __fastcall (source in ecx, passed straight through by the only
//   caller 0x00C17400), `ret`, EH frame (state 0 the stream, 1 the new-expression, 2 the owner, 3 the trace string).
// Identity (descriptive; libpk has no PDB or RTTI): the trace names "d:\dev\audio\ver_fable_1.5_b\libpk\PKSerialiseLoad.h"
//   (0x012B2D0C) line 34 "EMEM" (0x012B1B58): a header template, so the name is the header's and T is the 0x30-byte object
//   built by 0x00C2EED0 (`push 30h ; call operator new`), the landed CBankData constructor (destructor 0x00C2EF30). The
//   manifest label "DeleteSelectedTrackNode_Implementation" (CEditControlCentre) is propagated and wrong.
// Callees: CMemoryStream::CMemoryStream(IRemoteSource*) 0x00C31F60 / ~CMemoryStream 0x00C31F00 (landed), operator new
//   0x00BFEA1A, CBankData::CBankData 0x00C2EED0 / ~CBankData 0x00C2EF30 (landed), operator delete 0x00BFE9BC, the parser
//   constructor 0x00C31970 ({reader, 0}), CBankData::Transfer(PKSerialiseCParser&) 0x00C2EE80 (bool: retail tests al),
//   PKCAutoDelete<CBankData>::GetChecked 0x00C17740 (".h" line 32) and ::Release 0x00C17850 (line 42); the trace macro
//   (0x00C0DEC0 / 0x00C0DF00 / 0x00C0BA40 / 0x00C16750, globals 0x0139C900 / 0x0143BB2C) with the hand592/596 spellings.
//   Several of these addresses carry other (placeholder) spellings in landed rows: see NAME_FIXES.tsv.
// Shape: `object->Transfer(parser)` goes through the checked operator-> (GetChecked out of line); the failure return
//   destroys the owner inline (`if (m_pObject) delete m_pObject;`, the state store reusing al = 0) and the stream. The
//   bodies of GetChecked / Release must be in the TU (`#pragma auto_inline(off)`): with declarations only VC7.1 assumes
//   GetChecked may change m_pObject, tests the owner through eax and reloads it (395v393, no esi). The CDebugBreak
//   temporary lands in a byte of a dead slot ([esp+7]) by itself.
struct CDebugStream {
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text); // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value);       // 0x00C0DF00
    ~CDebugStream();                                       // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); };                     // 0x00C16750
extern char* g_pDebugBuffer;                               // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*);   // 0x0143BB2C

struct IRemoteSource {                                     // the data source (landed CMemoryStream model)
    virtual bool ReadSize(unsigned long* size);            // slot 0
};
struct IDataStream {                                       // vtable 0x012B4E00
    virtual bool Read(void* dest, unsigned long size) = 0; // slot 0
    virtual ~IDataStream() { }                             // slot 1
};
struct CMemoryStream : IDataStream {                       // vtable 0x012B4E08
    char* m_pBuffer;                                       // +0x04
    char* m_pCursor;                                       // +0x08
    unsigned long m_remaining;                             // +0x0C
    CMemoryStream(IRemoteSource* source);                  // 0x00C31F60
    virtual bool Read(void* dest, unsigned long size);     // 0x00C31EB0
    virtual ~CMemoryStream();                              // 0x00C31F00
};

struct PKSerialiseCParser {                                // the libpk serialise parser (landed CBankData::Transfer model)
    void* m_pReader;                                       // +0x00
    void* m_pStream;                                       // +0x04
    PKSerialiseCParser(IDataStream* reader);               // 0x00C31970
};

struct CBankData {                                         // 0x30 bytes: three record lists (samples / drivers / events)
    long _words[12];
    CBankData();                                           // 0x00C2EED0
    ~CBankData();                                          // 0x00C2EF30
    bool Transfer(PKSerialiseCParser& parser);             // 0x00C2EE80
};

template <class T> class PKCAutoDelete {                  // libpk PKCAutoDelete.h: owns one heap object
public:
    T* m_pObject;
    PKCAutoDelete(T* object) : m_pObject(object) { }
    ~PKCAutoDelete() { if (m_pObject) delete m_pObject; }
    T* GetChecked();                                       // 0x00C17740
    T* Release();                                          // 0x00C17850
    T* operator->() { return GetChecked(); }
};

// Same-TU bodies of 0x00C17740 / 0x00C17850 (landed separately), context only: knowing that GetChecked does not write
// m_pObject is what lets VC7.1 keep the object in esi for the failure-path delete, as retail.
#pragma auto_inline(off)
template <class T> T* PKCAutoDelete<T>::GetChecked()
{
    if (m_pObject == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << 32 << ") : " << "Should have checked first..." << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    return m_pObject;
}
template <class T> T* PKCAutoDelete<T>::Release()
{
    if (m_pObject == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << 42 << ") : " << "Shouls have checked first..." << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    T* object = m_pObject;
    m_pObject = 0;
    return object;
}
#pragma auto_inline(on)

template <class T> T* __fastcall PKSerialiseLoad(IRemoteSource* source)
{
    CMemoryStream stream(source);
    PKCAutoDelete<T> object(new T);
    if (object.m_pObject == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKSerialiseLoad.h" << "(" << 34 << ") : " << "EMEM" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    PKSerialiseCParser parser(&stream);
    if (!object->Transfer(parser))
        return 0;
    return object.Release();
}

template CBankData* __fastcall PKSerialiseLoad<CBankData>(IRemoteSource* source);