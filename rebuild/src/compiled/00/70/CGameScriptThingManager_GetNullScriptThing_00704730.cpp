// CGameScriptThingManager::GetNullScriptThing() -- return a copy of the manager's null CScriptThing handle (vtable, both PImp words, one more reference)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00704730, 31 bytes. Manifest label "CScriptThing::CScriptThing" is propagated
// from the copy-constructor shape; the PDB name is `?GetNullScriptThing@CGameScriptThingManager@@QAE?AVCScriptThing@@XZ`
// (egor_signatures.tsv) and its callers are CGameScriptThing::CreateNullThing (0x008CFF40, `[g_world+4]` = the
// manager) and the CGameScriptThing::MsgWho* bodies (0x008D06A3 ...). No callees: the CScriptThing copy
// constructor (0x004ABE90) is inlined.
//
// Shape: hidden-pointer return `mov eax,[esp+4] ; mov [eax],vftable ; copy [ecx+8] ; copy [ecx+0xc] ; test ; inc ; ret 4`.
// CGameScriptThingManager (PDB: 24 bytes, ScriptThings list then NullThing) keeps the 4-byte retail list
// head at +0 (the PDB list is 12 bytes, the retail VC6-era list one pointer -- see the landed destructor
// 0x004BB600), so NullThing sits at +4: vptr +4, PImp.Data +8, PImp.Info +0xc. CScriptThing = CBase (vptr,
// out-of-line virtual destructor 0x0099A2E0) + CCountedPointer<CScriptThing> {Data, Info}; the vtable store
// precedes the copied words because they belong to the member's inline copy constructor.
// Leaf: GetNullScriptThing

class CCPPointerInfo { public:
    long m_references;                       // +0 reference count
};

struct CBase {
    virtual ~CBase();                        // 0x0099A2E0
};

class CScriptThing;

struct CCountedPointer_CScriptThing {
    CScriptThing* Data;                      // +0
    CCPPointerInfo* Info;                    // +4
    CCountedPointer_CScriptThing(const CCountedPointer_CScriptThing& other)
        : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
};

class CScriptThing : public CBase { public:                // vtable 0x01238C8C
    CCountedPointer_CScriptThing PImp;       // +0x04
    CScriptThing(const CScriptThing& other) : PImp(other.PImp) {}   // 0x004ABE90 (inlined)
    virtual ~CScriptThing();
};

struct CScriptThingInfoList {                // std::list<CGameScriptThingManager::CScriptThingInfo> (retail: head pointer only)
    void* m_pHead;                           // +0x00
};

class CGameScriptThingManager { public:
    CScriptThingInfoList ScriptThings;       // +0x00
    CScriptThing NullThing;                  // +0x04
    CScriptThing GetNullScriptThing();
};

CScriptThing CGameScriptThingManager::GetNullScriptThing()
{
    return NullThing;
}