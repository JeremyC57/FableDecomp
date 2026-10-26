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
struct Sub;
// Leaf: CRegisteredMusicEntry>_>
struct Obj;

struct Sub { int x; };
struct Obj {
    char pad[0x114];
    Sub sub;
};

void __fastcall DestVal_Music_B(Obj* self, void* /*edx*/)
{
    ((CBaseIntelligentPointer*)(&self->sub))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    ((CCombatActionBase*)(self))->CCombatActionBase::~CCombatActionBase();
}