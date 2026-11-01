// CTCActionUseScriptedHook::SetEntranceAsLocked  @ 0x00729c30
// __fastcall void(CTCActionUseScriptedHook*, bool)

class CBaseObject;
class CBaseIntelligentPointer { public:
    CBaseObject* GetPItem() const;   // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
};
struct Entrance
{
    // method at 0x00a01b50 : __fastcall, returns pointer/bool in eax
    char data[8];   // occupies +0x20..+0x27
};

class CTCActionUseScriptedHook
{ public:
    char pad0[0x20];
    Entrance m_entrance;   // +0x20
    char m_locked;         // +0x28

    void SetEntranceAsLocked(bool locked);
};

void CTCActionUseScriptedHook::SetEntranceAsLocked(bool locked)
{
    if (((void*)((CBaseIntelligentPointer*)&(this->m_entrance))->GetPItem()))
    {
        this->m_locked = locked;
    }
}