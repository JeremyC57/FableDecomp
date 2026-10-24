// CNamedSet::Find(const CString& name, long* pCollisions) -- look an entry up by the Jenkins hash of its name; when the entry found has a non-empty name that differs from the one asked for, count a hash collision; return the entry (or NULL)
// Flags: /EHsc
// Leaf: Find
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C101D0, 214 bytes (`ret 8`). Built under /EHsc. Descriptive names (no PDB
//   covers this library): the manifest's "CAnimationSet_FindAnimation" is propagated. Callers 0x00C196E9, 0x00C1EF0E,
//   0x00C2E6E1, 0x00C2E7C3.
// Callees: CString::GetHash @ 0x00C0DBB0 (the landed "JenkinsHashOfName": JenkinsHash(m_pchData, SafeStrlen, 0)), the
//   index's keyed Find (virtual slot 17, +0x44; the hash passed by reference from a frame local), CString(const char*)
//   @ 0x00C0BFE0 for the "" temporary, the entry's GetName (virtual slot 2, +8), CString::Compare(const char*)
//   @ 0x00C0BCE0, ~CString @ 0x00C0BA40.
// Shape: an explicit `!= CString("")` builds a conditional temporary (VC's constructed-flag word plus bl, EH state 0
//   around it); MFC's operator!=(const CString&, const CString&) converts the second operand through the inline
//   `operator LPCTSTR`, so the temporary's text is read through the constructor's returned pointer (`mov edi,eax ;
//   mov edi,[edi]`) and the name stays in ebp (reading s2.m_pchData directly keeps the name in edi and reads the
//   temporary from the frame); the condition's bool is homed in the dead name slot.
struct CString {
    char* m_pchData;                            // +0x00
    CString(const char* lpsz);                  // 0x00C0BFE0
    ~CString();                                 // 0x00C0BA40
    int Compare(const char* lpsz) const;        // 0x00C0BCE0
    unsigned long GetHash() const;              // 0x00C0DBB0
    operator const char*() const { return m_pchData; }
};

inline bool operator!=(const CString& s1, const CString& s2)
{
    return s1.Compare(s2) != 0;
}
inline bool operator!=(const CString& s1, const char* s2)
{
    return s1.Compare(s2) != 0;
}
inline bool operator!=(const char* s1, const CString& s2)
{
    return s2.Compare(s1) != 0;
}

struct CNamedEntry {
    virtual void Slot0(); virtual void Slot1();
    virtual const CString& GetName();           // slot 2 (+8)
};

struct CNamedIndex {                            // typed list over an implementation object
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Slot5(); virtual void Slot6(); virtual void Slot7(); virtual void Slot8(); virtual void Slot9();
    virtual void Slot10(); virtual void Slot11(); virtual void Slot12(); virtual void Slot13(); virtual void Slot14();
    virtual void Slot15(); virtual void Slot16();
    virtual CNamedEntry* Find(const unsigned long& key);   // slot 17 (+0x44)
    void* m_pImpl;                              // +4
    unsigned char m_impl[8];                    // +8
};

struct CNamedSet {
    unsigned char _pad00[0x10];
    CNamedIndex m_index;                        // +0x10
    CNamedEntry* Find(const CString& name, long* pCollisions);
};

CNamedEntry* CNamedSet::Find(const CString& name, long* pCollisions)
{
    unsigned long hash = name.GetHash();
    CNamedEntry* entry = m_index.Find(hash);
    if (entry && entry->GetName() != CString("")) {
        if (entry->GetName() != name && pCollisions)
            ++*pCollisions;
    }
    return entry;
}