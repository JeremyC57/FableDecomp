// Leaf: CRegisteredMusicEntry>_>
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
extern void __fastcall dv_h2_004c7990(void*);
void __fastcall Dest_val(void* self){ dv_h2_004c7990(((void*)(((CBaseIntelligentPointer*)(reinterpret_cast<char*>(self)+8))->GetPItem()))); }