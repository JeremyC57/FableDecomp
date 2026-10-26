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
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();   // 0x00693EF0
};
struct Sub { char pad[4]; };
// Leaf: CRegisteredMusicEntry>_>
struct Obj {
    char pad[0xa8];
    Sub sub;
};


extern "C" void __fastcall DestVal_A8_K(void* self, void* /*edx*/)
{
    Obj* o = (Obj*)self;
    ((CBaseIntelligentPointer*)(&o->sub))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    ((CCreatureActionBase*)(o))->CCreatureActionBase::~CCreatureActionBase();
}