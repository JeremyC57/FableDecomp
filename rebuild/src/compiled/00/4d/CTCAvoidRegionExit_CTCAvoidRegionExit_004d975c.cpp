// CTCAvoidRegionExit::CTCAvoidRegionExit(CThing* thing) -- CTCBase(thing), own vtable, the EntranceMarker and ExitMarker intelligent pointers constructed, NextFrameCheck = 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D975C, length 61 bytes; a size-optimised TU
// (`push dword ptr [esp+10h]` pass-through, `and dword ptr [esi+1Ch],0`, the CIntelligentPointer vtable
// held in ebx for both member stores): `#pragma optimize("s", on)`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10 (x2). Vtables: 0x0123EFAC = CTCAvoidRegionExit, 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCAvoidRegionExit (32 bytes): EntranceMarker +0xc, ExitMarker +0x14
// (CIntelligentPointer<CThing>), NextFrameCheck +0x1c. Shape: the vtable store precedes the member
// constructors (inline CIntelligentPointer() over the out-of-line base, vtable stored through the
// member's address register); the scalar initialiser is stored last.
#pragma optimize("s", on)
struct CThing;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CTCAvoidRegionExit : CTCBase_Calls {                 // vtable 0x0123EFAC
    CIntelligentPointer EntranceMarker;                     // +0x0c
    CIntelligentPointer ExitMarker;                         // +0x14
    unsigned long       NextFrameCheck;                     // +0x1c
    virtual void v00();
    CTCAvoidRegionExit(CThing* thing);
};

CTCAvoidRegionExit::CTCAvoidRegionExit(CThing* thing)
    : CTCBase_Calls(thing)
    , NextFrameCheck(0)
{
}