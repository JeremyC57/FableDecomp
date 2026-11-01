// CCreatureAction_Die::FinishAction  (retail 0x008ed830, __fastcall, void)

class CThing { public:
    void Kill(bool a0);   // 0x004C9B80 (Ego_r.pdb ?Kill@CThing@@QAEX_N@Z)
};
struct RetObj
{
};

class CBaseObject;
class CBaseIntelligentPointer { public:
    CBaseObject* GetPItem() const;   // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
};
struct SubObj
{
};

struct Base
{
    virtual void FinishAction_00859a20();  // call 0x694430 (base FinishAction on this)
};

class CCreatureAction_Die : public Base
{ public:
    int    filler;   // +4
    SubObj sub;      // +8

    virtual void FinishAction();
};

void CCreatureAction_Die::FinishAction()
{
    Base::FinishAction_00859a20();
    ((CThing*)(((RetObj*)((CBaseIntelligentPointer*)&(this->sub))->GetPItem())))->Kill(true);
}