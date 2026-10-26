class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
struct Sub { char b; };
// Leaf: CRegisteredMusicEntry>_>

// call target at 0xa01c10: __fastcall method (ecx = this+0x114)
// tail-call target at 0x62e900: __fastcall method (ecx = this)
struct Owner;

void __fastcall DestVal_Music_C(Owner* self)
{
    ((CBaseIntelligentPointer*)((Sub*)((char*)self + 0x114)))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    ((CCombatActionBase*)(self))->CCombatActionBase::~CCombatActionBase();
}