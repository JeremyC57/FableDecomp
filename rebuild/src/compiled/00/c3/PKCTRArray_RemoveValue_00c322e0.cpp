// PKCTRArray::RemoveValue(CUnprovenArrayComparator* comparator, const void* value) -- remove an object from the sorted pointer array: trace "Error: NULL object" (PKCTRArray.cpp line 154) for a null object, binary-search it with the comparator's value slots, trace "Error: Could not find this object..." (line 157) when it is missing, then close the gap and drop the count
// Leaf: ?RemoveValue@PKCTRArray@@UAEXPAVCUnprovenArrayComparator@@PBX@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C322E0, 385 bytes, thiscall `ret 8`, EH-framed (the two trace strings, states 0 / 1).
// Identity (class proven, method name descriptive; libpk has no PDB or RTTI): slot 6 of the PKCTRArray vtable 0x012B4E80
//   (its only reference, 0x012B4E98), next to FindValue (slot 7, 0x00C32470) and FindKey (slot 8, 0x00C324B0); the traces
//   name ".\PKCTRArray.cpp" (0x012B4E4C) like the landed GetFirst 0x00C325F0 (line 204) and GetLast. It searches with
//   FindValueIndex 0x00C32160 (comparator slots 0/1, as FindValue), so "RemoveValue". Manifest label "sub_00c322e0".
// Callees: PKCTRArray::FindValueIndex 0x00C32160 (bound `?FindValueIndex@PKCTRArray@@QAEHPAVCUnprovenArrayComparator@@PBXPAH@Z`
//   by the landed FindValue; the landed body itself is spelled CSortedPtrArray::FindIndex, see NOTES.md); the trace string's
//   `__fastcall` appenders 0x00C0DEC0 / 0x00C0DF00, its destructor 0x00C0BA40, the debugger break temporary 0x00C16750;
//   globals 0x0139C900 (empty buffer) and 0x0143BB2C (report hook). Spellings follow the landed GetFirst / FindValue.
// Shape: the libpk trace macro (stream over the shared buffer, report hook, debugger break) twice. Retail keeps no frame:
//   the first trace string and the `found` out-parameter live in the dead `value` argument slot, the second break
//   temporary in the dead `comparator` slot. That comes from FindValueIndex's body being in the TU (the landed FindValue
//   0x00C32470 form, `#pragma auto_inline(off)`): VC7.1 then sees that &found does not escape and homes it in the
//   argument slot; with a declaration only it reserves a `push ecx` slot (386v385) and the EH prologue order changes.
//   Land the leaf only: FindValueIndex is landed separately (0x00C32160).
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

class CUnprovenArrayComparator {          // the landed FindValue model (four pure comparison slots + virtual dtor)
public:
    virtual int EqualValue(const void*, const void*) = 0;
    virtual int LessValue(const void*, const void*) = 0;
    virtual int EqualKey(const void*, const void*) = 0;
    virtual int LessKey(const void*, const void*) = 0;
    virtual ~CUnprovenArrayComparator() {}
};

// libpk PKCTRArray.cpp (no PDB): vtable 0x012B4E80, entry block +0x04, capacity +0x08, count +0x0C (landed GetFirst /
// FindValue / ResizeAllocation 0x00C32950).
class PKCTRArray {
public:
    void**       Entries;                   // +0x04
    unsigned int Capacity;                  // +0x08
    unsigned int Count;                     // +0x0C
    virtual void RemoveValue(CUnprovenArrayComparator* comparator, const void* value);   // slot 6 (0x00C322E0)
    int FindValueIndex(CUnprovenArrayComparator* comparator, const void* key, int* found);   // 0x00C32160
};

// Same-TU body of 0x00C32160 (the landed FindValue model), context only.
#pragma auto_inline(off)
int PKCTRArray::FindValueIndex(CUnprovenArrayComparator* cmp, const void* key, int* found)
{
    if (Count == 0) { *found = 0; return 0; }
    int low = 0, high = (int)Count - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (cmp->EqualValue(key, Entries[mid])) { *found = 1; return mid; }
        if (cmp->LessValue(key, Entries[mid])) high = mid - 1; else low = mid + 1;
    }
    *found = 0;
    int mid = (low + high) / 2;
    return cmp->LessValue(key, Entries[mid]) ? mid : mid + 1;
}
#pragma auto_inline(on)

void PKCTRArray::RemoveValue(CUnprovenArrayComparator* comparator, const void* value)
{
    if (value == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << ".\\PKCTRArray.cpp" << "(" << 154 << ") : " << "Error: NULL object" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    int found;
    int index = FindValueIndex(comparator, value, &found);
    if (!found) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << ".\\PKCTRArray.cpp" << "(" << 157 << ") : " << "Error: Could not find this object..." << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    for (unsigned int i = index; i < Count - 1; ++i)
        Entries[i] = Entries[i + 1];
    --Count;
}