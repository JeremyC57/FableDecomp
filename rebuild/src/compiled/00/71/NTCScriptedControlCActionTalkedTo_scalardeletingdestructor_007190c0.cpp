#pragma optimize("t", on)
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
class CScriptThing {
public:
    virtual ~CScriptThing();   // 0x004AA840
};
extern void __fastcall CActionTalkedTo_Destruct(void* action);
void* __fastcall CActionTalkedTo_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    ((CScriptThing*)(static_cast<unsigned char*>(action) + 20))->CScriptThing::~CScriptThing();
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(action) + 12))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    CActionTalkedTo_Destruct(action);
    if (flags & 1)
        operator delete(action);
    return action;
}