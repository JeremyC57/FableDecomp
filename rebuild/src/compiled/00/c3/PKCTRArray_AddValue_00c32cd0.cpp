// PKCTRArray::AddValue(CUnprovenArrayComparator* comparator, void* value) -- insert an object into the sorted pointer array: trace "Error: NULL object" (PKCTRArray.cpp line 131) for a null object, find its slot with the comparator's value slots (tracing "Error: this object already exists..." , line 134, when it is already there), grow the allocation by one when the array is full (tracing "Error: Growing did not work?", line 139, when that fails), shift the tail up one slot, store the object and bump the count
// Leaf: ?AddValue@PKCTRArray@@UAEXPAVCUnprovenArrayComparator@@PAX@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C32CD0, 562 bytes, thiscall `ret 8`, EH-framed (the three trace strings, states 0 / 1 / 2).
// Identity (class proven, method name descriptive; libpk has no PDB or RTTI): slot 5 of the PKCTRArray vtable 0x012B4E80 (its
//   only reference, 0x012B4E94), the slot the landed PKCTRMorphable::MorphToTree / MorphToArray 0x00C17BE0 / 0x00C18870 call
//   "Add" when they move items between the containers, next to slot 6 RemoveValue 0x00C322E0 and slot 7 FindValue 0x00C32470;
//   slot 5 of the tree vtable 0x012B2F0C is 0x00C18E20 (this chunk, the same name). The traces name ".\PKCTRArray.cpp"
//   (0x012B4E4C) with "Error: NULL object" (0x012B4E38), "Error: this object already exists..." (0x012B4F10) and "Error:
//   Growing did not work?" (0x012B4EF0). Layout of the landed GetFirst / FindValue / ResizeAllocation 0x00C32950 (entry
//   block +0x04, capacity +0x08, count +0x0C). The catalog's gap-scan name is a placeholder.
// Callees: PKCTRArray::FindValueIndex 0x00C32160 (the landed FindValue / RemoveValue spelling; its body is in the TU so VC7.1
//   homes `found` in the dead trace-string slot, as the landed RemoveValue explains), PKCTRArray::ResizeAllocation(long)
//   0x00C32950 (landed), the libpk trace stream (CString operator<< 0x00C0DEC0 text / 0x00C0DF00 long, ~CString 0x00C0BA40,
//   g_pchNil 0x0139C900, g_pfnAssertHook 0x0143BB2C) and CDebugBreak 0x00C16750.
// Shape: the free-slot test is `Capacity - Count` twice (zero -> grow; then still below one -> trace); the tail shift runs from
//   the last entry down to the insertion index and is guarded by an explicit non-empty test.
extern char* g_pchNil;                          // 0x0139C900 (the shared empty string data)
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

class CString {                                 // the libpk MFC-style string: one word
public:
    char* m_pchData;
    CString() : m_pchData(g_pchNil) { }
    ~CString();                                         // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
};
struct CDebugBreak {                            // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                              // 0x00C16750
};

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
    virtual void AddValue(CUnprovenArrayComparator* comparator, void* value);   // slot 5 (0x00C32CD0)
    int FindValueIndex(CUnprovenArrayComparator* comparator, const void* key, int* found);   // 0x00C32160
    void ResizeAllocation(long extra);      // 0x00C32950
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

void PKCTRArray::AddValue(CUnprovenArrayComparator* comparator, void* value)
{
    if (value == 0) {
        {
            CString line;
            line << ".\\PKCTRArray.cpp" << "(" << 131L << ") : " << "Error: NULL object" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    int found;
    int index = FindValueIndex(comparator, value, &found);
    if (found) {
        {
            CString line;
            line << ".\\PKCTRArray.cpp" << "(" << 134L << ") : " << "Error: this object already exists..." << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    if (Capacity - Count == 0)
        ResizeAllocation(1);
    if (Capacity - Count < 1) {
        {
            CString line;
            line << ".\\PKCTRArray.cpp" << "(" << 139L << ") : " << "Error: Growing did not work?" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    if (Count != 0) {
        for (int i = (int)Count - 1; i >= index; --i)
            Entries[i + 1] = Entries[i];
    }
    Entries[index] = value;
    ++Count;
}