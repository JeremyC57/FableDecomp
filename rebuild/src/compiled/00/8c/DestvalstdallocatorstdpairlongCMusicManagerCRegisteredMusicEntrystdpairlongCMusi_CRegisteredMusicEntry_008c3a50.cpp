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
struct Sub { int x; };
// Leaf: CRegisteredMusicEntry>_>
struct Self {
    char pad[0x114];
    Sub m114;
};
extern "C" void __fastcall DestVal_114_A(Self* self, void* /*edx*/)
{
    ((CBaseIntelligentPointer*)(&self->m114))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    ((CCombatActionBase*)(self))->CCombatActionBase::~CCombatActionBase();
}