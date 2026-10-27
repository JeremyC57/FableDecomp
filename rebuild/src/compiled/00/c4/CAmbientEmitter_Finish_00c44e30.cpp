// CAmbientEmitter::Finish() -- run the CVtable_012b7bbc visitor over the m_events list (+0x20)
// Leaf: ?Finish@CAmbientEmitter@@QAEXXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c44e30, 72 bytes, thiscall, plain `ret`, EH-framed (handler 0x0121535F).
// Identity (descriptive): it walks the emitter's +0x20 event list (CAmbientEmitter layout of the landed AcquireSounds 0x00C44CE0) with the visitor 0x012B7BBC; its only caller 0x00C44E80 runs it when the definition's IsFinished 0x00C384E0 is true -- the same shape as CAmbientSource::Update calling CAmbientSource::Finish 0x00C472B0, so it is named Finish by analogy. libpk: no PDB, no RTTI; descriptive class names as landed.
// Callees: the visitor constructor CVtable_012b7bbc::CVtable_012b7bbc(void*) 0x00C44DA0 (landed; vtable 0x012B7BBC over the base vtable 0x012B7BAC),
//   the list's ForEach (virtual slot 10, +0x28) with the visitor's address.
// Shape (family L130): the visitor constructor is a same-TU noinline body compiled first, so VC7.1 keeps `this` in edx
//   across the call (retail's `mov edx,ecx ; push edx` ... `mov eax,[edx+0x20]`); the visitor's inline destructor is
//   dead (only the EH state 0 store remains).
class CPKVisitor {                         // the visitor base (vtable 0x012B7BAC: { _purecall, scalar deleting destructor })
public:
    virtual void Visit(void* item) = 0;     // slot 0
    virtual ~CPKVisitor() { }
};

struct CVtable_012b7bbc : CPKVisitor {        // vtable 0x012B7BBC
    void* m_first;                          // +0x4: the walking object
    CVtable_012b7bbc(void* first);
    virtual void Visit(void* item);
};

__declspec(noinline) CVtable_012b7bbc::CVtable_012b7bbc(void* first)
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
class CAmbientEmitter {
public:
    void*         m_pZone;                  // +0x00
    void*         m_pDef;                   // +0x04
    unsigned char m_pad08[0x20 - 8];
    PKCTRList<CAmbientEvent> m_events;      // +0x20 time-ordered pending events
    void Finish();
};

void CAmbientEmitter::Finish()
{
    CVtable_012b7bbc visitor(this);
    m_events.ForEach(visitor);
}