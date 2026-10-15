// CStackOwner_00c287e0::CStackOwner_00c287e0() -- ctor of the vtable-0x012B4098 class (the landed dtor is 0x00C287E0): zero words +4..+0x18, the +0x34 listener member, the 0x400-byte table cleared by Clear(), an empty vector at +0x440, +0x44c = -1, the +0x450 member from the global 0x0139C900, then the listener self-pointer and four size/flag words
// Grade: DIFFER(171v158) -- retail opens the /EHsc frame but stores NO EH state and does not home `this`; with a throwing Clear() ours stores states 0/2 and `this` ([esp+8]), with Clear() throw()/nothrow (or trivial member dtors) ours drops the whole frame (123 bytes)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C28AE0, length 158 bytes. Callees: Clear @ 0x00C28200 (rep stosd of 0x100 dwords at +0x3c).
// Label: the catalog's "CSoundDef::CSoundDef" is propagated; the vtable store identifies the class of the landed dtor 0x00C287E0.
// Flags: /EHsc. Tried: Clear() throwing (this file), throw(), __declspec(nothrow), trivial member dtors.
// Leaf: ??0CStackOwner
// Flags: /EHsc
void __cdecl operator delete(void* block) throw();       // 0x00BFE9BC

struct CListener_00c287e0 {                // +0x34, vtable 0x012B37FC
    virtual void v00();
    ~CListener_00c287e0() { }
};

struct CVector_00c287e0 {                  // +0x440: {begin, end, capacity}
    void* m_pBegin;
    void* m_pEnd;
    void* m_pCapacity;
    CVector_00c287e0() : m_pBegin(0), m_pEnd(0), m_pCapacity(0) {}
    ~CVector_00c287e0()
    {
        if (m_pBegin)
            operator delete(m_pBegin);
        m_pBegin = 0;
        m_pEnd = 0;
        m_pCapacity = 0;
    }
};

extern long g_defaultMemberValue;          // 0x0139C900

struct CMember_00c287e0 {                  // +0x450
    long m_value;
    CMember_00c287e0() : m_value(g_defaultMemberValue) {}
    ~CMember_00c287e0();                   // 0x00C0BA40
};

struct CStackOwner_00c287e0 {              // vtable 0x012B4098
    virtual void v00();
    long m_04, m_08, m_0c, m_10, m_14, m_18; // +0x04 .. +0x18
    long m_1c;
    long m_20, m_24, m_28, m_2c;           // +0x20 .. +0x2c
    long m_30;
    CListener_00c287e0  m_listener;        // +0x34
    CListener_00c287e0* m_pListener;       // +0x38
    unsigned long       m_table[0x100];    // +0x3c
    long                m_43c;             // +0x43c
    CVector_00c287e0    m_vector;          // +0x440
    long                m_44c;             // +0x44c
    CMember_00c287e0    m_member;          // +0x450
    void Clear();                          // 0x00C28200 (zeroes m_table)
    CStackOwner_00c287e0();
    ~CStackOwner_00c287e0();
};

CStackOwner_00c287e0::CStackOwner_00c287e0()
    : m_04(0), m_08(0), m_0c(0), m_10(0), m_14(0), m_18(0),
      m_pListener(0),
      m_44c(-1)
{
    Clear();
    m_pListener = &m_listener;
    m_20 = 0x80000;
    m_24 = 0x100000;
    m_28 = 1;
    m_2c = 0x10;
}
