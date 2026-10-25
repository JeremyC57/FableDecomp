// CCombatActionBase::SetTarget(CThing& target) -- point the action's PTarget intelligent pointer (+0xA8) at the target, through a temporary CIntelligentPointer<CThing>
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085AA90, oracle length 74 bytes, `ret 4`.
// Leaf: ?SetTarget@CCombatActionBase@@QAEXAAVCThing@@@Z
// Naming: Ego_r.pdb ?SetTarget@CCombatActionBase@@QAEXAAVCThing@@@Z. The one caller is
//   CCombatActionBase::GetPCombatActionFromEnum 0x00857430 (Ego_r.pdb ?GetPCombatActionFromEnum@CCombatActionBase@@SI
//   ?AV?$CCountedPointer@VCCombatActionBase@@@@W4EActionRegisteredType@@AAVCThingCreatureBase@@1@Z): it creates the
//   action through the registered factory, initialises it with the creature (0x006928C0) and then calls this with the
//   second creature argument, the target. Ego_r.pdb CCombatActionBase::PTarget is the CIntelligentPointer<CThing> right
//   after the CCreatureActionBase base (PDB +0xB4; retail +0xA8, the base's debug STL members being larger).
//   FINDINGS chunk 402. Was landed as the stand-in "CClass_0085aa90::AssignTarget_0085aa90" (manifest
//   "CVillageTaskCarryCrate::AssignCreatureToTask" propagated).
// Retail bytes:  sub esp,8 ; push esi ; mov esi,ecx ; lea ecx,[esp+4] ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; mov eax,[esp+10h] ; push eax ; lea ecx,[esp+8] ; mov dword ptr [esp+8],offset CIntelligentPointer::`vftable' ; call SetPItem ; lea ecx,[esp+4] ; call GetPItem ; push eax ; lea ecx,[esi+0xa8] ; call SetPItem ; lea ecx,[esp+4] ; call ~CBaseIntelligentPointer ; pop esi ; add esp,8 ; ret 4
// A stack CIntelligentPointer<CThing> (8 bytes: vtable 0x01238C6C + item; constructed by the inlined default ctor over
// CBaseIntelligentPointer() @ 0x00A01B10) is pointed at the target with SetPItem @ 0x00A01B90, read back with
// GetPItem @ 0x00A01B50 into PTarget, and destroyed @ 0x00A01C10 (the intelligent-pointer assignment).
class CBaseObject { };
class CThing : public CBaseObject { };

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* m_pItem;                  // +4 (an 8-byte temporary: `sub esp,8`)
    CBaseIntelligentPointer();             // 0x00A01B10
    ~CBaseIntelligentPointer();            // 0x00A01C10
    CBaseObject* GetPItem() const;         // 0x00A01B50
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer& operator=(T* item)
    {
        CIntelligentPointer temp;
        temp.SetPItem(item);
        SetPItem(temp.GetPItem());
        return *this;
    }
};

class CCombatActionBase {
public:
    void SetTarget(CThing& target);
    unsigned char             _pad[0xa8];  // CCreatureActionBase base and members
    CIntelligentPointer<CThing> PTarget;   // +0xa8
};

void CCombatActionBase::SetTarget(CThing& target)
{
    PTarget = &target;
}
