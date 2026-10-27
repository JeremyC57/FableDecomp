// CVillageTaskHorn::CVillageTaskHorn(CThing& thing) -- a village task for the thing: remember it (PHorn) with no creature assigned yet
// Leaf: ??0CVillageTaskHorn@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065EDBA, 50 bytes, thiscall, `ret 4`. Vtable 0x0125A8D8.
// Identity (proven): it stores vtable 0x0125A8D8 whose RTTI is .?AVCVillageTaskHorn@@; Ego_r.pdb ??0CVillageTaskHorn@@QAE@AAVCThing@@@Z and members
//   PHorn (CIntelligentPointer<CThing>, +0x08) / CreatureAssignedToTask (CIntelligentPointer<const CThing>, +0x10); its caller is CTCVillage::UpdateHornTasks 0x0065BE6C.
//   The manifest's "CVillageTaskHorn::CVillageTaskHorn" is propagated across byte-identical bodies.
// Callees: CBaseObject::CBaseObject() 0x0099A380, CIntelligentPointer<CThing>(CThing*) 0x004A96F0, CBaseIntelligentPointer::
//   CBaseIntelligentPointer() 0x00A01B10 (then the CIntelligentPointer<const CThing> vtable 0x01237F1C, inline).
// Shape (family M181, representative 0x0065ED08): base, vptr, then the members in declaration order. Size TU (`push [esp+0Ch]`).
#pragma optimize("s", on)
class CBaseObjectPointer;
class CBaseObject {
protected:
    CBaseObject();                                        // 0x0099A380
    virtual ~CBaseObject();
public:
    CBaseObjectPointer* IntelligentPointer;               // +0x04
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                            // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item);                         // out of line
    virtual ~CIntelligentPointer();
};
class CThing;
class CVillageTask : public CBaseObject {
public:
    virtual ~CVillageTask();
};
class CVillageTaskHorn : public CVillageTask {
public:
    CVillageTaskHorn(CThing& thing);
    virtual ~CVillageTaskHorn();
    CIntelligentPointer<CThing> PHorn;                     // +0x08
    CIntelligentPointer<const CThing> CreatureAssignedToTask;   // +0x10
};
CVillageTaskHorn::CVillageTaskHorn(CThing& thing)
    : PHorn(&thing)
{
}