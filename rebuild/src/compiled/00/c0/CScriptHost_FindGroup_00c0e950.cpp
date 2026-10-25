// CScriptHost::FindGroup(unsigned long group) -- looks a driver group up by number in the group registry at +0x74 (virtual slot 17, the key passed by reference) and returns it (or NULL)
// Leaf: ?FindGroup@CScriptHost@@QAEPAUCDriverGroup@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E950, 17 bytes: mov eax,[ecx+74h] ; add ecx,74h ; lea edx,[esp+4] ;
//   push edx ; call [eax+44h] ; ret 4
// Naming (sweep1, 2026-10-25): was landed as `CIsThingAlive::operator()` (a propagated manifest label). FINDINGS
//   chunk 436: the landed CBankProcessor::AddDriverToGroup 0x00C2E5A0 calls it (0x00C2E5E9) before CreateGroup
//   0x00C0E8E0; the other caller is 0x00C2C844.
// Owner: `this` is the libpk object of vtable 0x012B25A8, landed under agent431's descriptive class CScriptHost (QueueEvent
//   0x00C0E9E0, ClearEvents 0x00C0EA10, DeleteAllScripts 0x00C0EAF0, ...; ctor 0x00C0EDF0, dtor 0x00C0ECE0): these factories
//   sit in its translation unit between its slot-5 TryActivate 0x00C0E600 and QueueEvent, and FindGroup reads the
//   registry its destructor tears down at +0x74. The bank processor holds it at +4 (agent436 called it the sound
//   manager / CSoundManager). No PDB covers libpk: descriptive names.
struct CDriverGroup;

struct CGroupRegistry {                         // the keyed registry at +0x74 (vtable 0x012B23F0 at destruction)
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40();
    virtual CDriverGroup* Find(const unsigned long& key);   // slot 17 (+0x44)
};

class CScriptHost {
public:
    unsigned char  _pad_0x00[0x74];
    CGroupRegistry m_groups;                    // +0x74
    CDriverGroup* FindGroup(unsigned long group);
};

CDriverGroup* CScriptHost::FindGroup(unsigned long group)
{
    return m_groups.Find(group);
}
