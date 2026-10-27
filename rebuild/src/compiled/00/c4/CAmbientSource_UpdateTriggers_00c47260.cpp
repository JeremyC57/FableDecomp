// CAmbientSource::UpdateTriggers() -- run the CVtable_012b7e58 visitor over the m_triggers list (+0x08)
// Leaf: ?UpdateTriggers@CAmbientSource@@QAEXXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c47260, 72 bytes, thiscall, plain `ret`, EH-framed (handler 0x01215654).
// Identity (probable): bound ?UpdateTriggers@CAmbientSource@@QAEXXZ (the landed Update 0x00C47300 calls it while the set is active); it walks the source's +0x08 trigger list. libpk: no PDB, no RTTI; descriptive class names as landed.
// Callees: the visitor constructor CVtable_012b7e58::CVtable_012b7e58(void*) 0x00C47010 (landed; vtable 0x012B7E58 over the base vtable 0x012B7E40),
//   the list's ForEach (virtual slot 10, +0x28) with the visitor's address.
// Shape (family L130): the visitor constructor is a same-TU noinline body compiled first, so VC7.1 keeps `this` in edx
//   across the call (retail's `mov edx,ecx ; push edx` ... `mov eax,[edx+0x08]`); the visitor's inline destructor is
//   dead (only the EH state 0 store remains).
class CPKVisitor {                         // the visitor base (vtable 0x012B7E40: { _purecall, scalar deleting destructor })
public:
    virtual void Visit(void* item) = 0;     // slot 0
    virtual ~CPKVisitor() { }
};

struct CVtable_012b7e58 : CPKVisitor {        // vtable 0x012B7E58
    void* m_first;                          // +0x4: the walking object
    CVtable_012b7e58(void* first);
    virtual void Visit(void* item);
};

__declspec(noinline) CVtable_012b7e58::CVtable_012b7e58(void* first)
    : m_first(first)
{
}

class CPKList {                             // libpk list base
public:
    virtual ~CPKList();                     // slot 0
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void Remove(void* item);        // slot 8
    virtual void Add(void* item);           // slot 9
    virtual void ForEach(CPKVisitor& visitor);   // slot 10 (+0x28)
    virtual void* GetHead();                // slot 11
    void* m_pImpl;                          // +0x04
};
template <class T>
class PKCTRList : public CPKList {          // PKCTRList.h
public:
    unsigned char m_impl[0x28];             // +0x08
};
class CAmbientTrigger;
class CAmbientChannel;
class CAmbientEvent;
class CAmbientSource {
public:
    void*                       m_pSet;     // +0x00
    void*                       m_pDef;     // +0x04
    PKCTRList<CAmbientTrigger>  m_triggers; // +0x08 time-ordered pending triggers
    PKCTRList<CAmbientChannel>  m_channels; // +0x38
    void UpdateTriggers();
};

void CAmbientSource::UpdateTriggers()
{
    CVtable_012b7e58 visitor(this);
    m_triggers.ForEach(visitor);
}