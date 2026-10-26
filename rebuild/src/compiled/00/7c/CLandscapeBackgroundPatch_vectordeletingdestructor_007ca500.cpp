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
class CLandscapeBackgroundPatch { public:
// Leaf: `vector_deleting_destructor'
    void* vftable;
};

void operator delete(void*);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(self))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    if (flags & 1)
        operator delete(self);
    return self;
}