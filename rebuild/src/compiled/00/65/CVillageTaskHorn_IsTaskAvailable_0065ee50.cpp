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
struct S { char pad[0x10]; char sub[4]; };
bool __fastcall IsTaskAvailable(S* task)
{
    return ((int)(((CBaseIntelligentPointer*)(reinterpret_cast<char*>(task) + 0x10))->GetPItem())) == 0;
}