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
struct P { char pad[0x18]; char* f18; };
struct S { char pad[4]; P* f4; void* M(); };
void* S::M(){ P* p=this->f4; char* q=p->f18; return ((void*)(((CBaseIntelligentPointer*)(q+368))->GetPItem())); }