#pragma optimize("s",on)
// Leaf: `vector_deleting_destructor'
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
typedef unsigned int uint;

struct Sub;
class CLandscapeBackgroundPatch { public:
    char pad[8];
    Sub* sub;
};


void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx*/, uint flags)
{
    ((CBaseIntelligentPointer*)((char*)self + 8))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    if (flags & 1)
        operator delete(self);
    return self;
}