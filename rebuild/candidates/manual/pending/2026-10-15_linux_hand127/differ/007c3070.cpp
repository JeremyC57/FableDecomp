// CCreatureActionBase::GetActionSpeedParameter() -- the creature's "SOUL_EFFECT" sub-definition parameter: looks the sub-def up through the creature, reads the named value out of it (0 when there is no such sub-def) and releases the def
// Grade: DIFFER(ours v retail) -- register choice only (112 v 112 bytes): retail loads the def pointer straight into esi (`mov esi,[esp+8]` then `mov ecx,esi` for the call), ours via ecx (`mov ecx,[esp+8] ; mov esi,ecx`); the two argument `lea`s use ecx/edx in retail, edx/eax here; the release's vtable load uses eax there, edx here
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C3070, length 112 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CDefClassBase::GetSubDef<T>(CDefPointer<T>&) @ 0x00669580 (on the
// creature read straight from the intelligent pointer's item word at +0xc: the inline operator->), the sub-def's value lookup
// (const CCharString&, long*) @ 0x00669D30 (manifest "CCRC::CalcAndLookup" is propagated), CCharString::~CCharString @ 0x0099EAE0,
// then the def's release (`dec [p+4] ; jne ; call [vt+4]`) written by hand on a pointer local (GOTCHAS wave 25).
// Shape: the string is a named local built first (`push -1 ; push offset` before the frame stores); the CDefPointer word and the
// result are zeroed in the frame (the result through edi, which also carries the final return); the def pointer is copied into
// esi right after GetSubDef, before the test, and released last; the module is a size TU (the counted-def release stays out of line).
// Layout: engine/CCreatureActionBase.h (PDB) agrees on PCreature at +8 (item at +0xc); `this` is modelled locally.
struct CCharString {                        // one pointer (PStringData)
    void* m_pData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

struct CSubDef {                            // CDefBase: vptr, then the reference count at +4
    virtual void v00();
    virtual void Destroy();                 // slot 1 (+4)
    long m_count;                           // +4
    void GetParameter(const CCharString& name, long* pValue);   // 0x00669D30
};

struct CDefPointer {                        // CDefPointer<T>: one word, zero-initialised; released by hand below
    CSubDef* m_p;
    CDefPointer() : m_p(0) { }
};

struct CThing {                             // the creature (its def-class base carries GetSubDef)
    bool GetSubDef(CDefPointer& def);       // 0x00669580
};

struct CIntelligentPointer {                // CIntelligentPointer<CThingCreatureBase>: vptr + item
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* operator->() const { return m_pItem; }   // inline: the item word read directly (no GetPItem call in this TU)
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    long GetActionSpeedParameter();
};

long CCreatureActionBase::GetActionSpeedParameter()
{
    CDefPointer def;
    long result = 0;                        // the register result (edi), whose zero also initialises the out word
    long value = 0;                         // the addressed out word (frame slot)
    CSubDef* p;
    {
        CCharString name("SOUL_EFFECT", -1);
        bool found = PCreature->GetSubDef(def);
        p = def.m_p;                        // the pointer local, loaded right after the lookup
        if (found) {
            p->GetParameter(name, &value);
            result = value;                 // read into edi before the string destructor and the release
        }
    }
    if (p) {
        if (--p->m_count == 0)
            p->Destroy();
    }
    return result;
}
