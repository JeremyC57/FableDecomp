#pragma optimize("s",on)
// Leaf: `vector_deleting_destructor'
// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector_deleting_destructor'
// retail 0x004da6a9, __fastcall(self /*ecx*/, uint flags /*stack*/)
// Harness rewrites __fastcall->__fastcall, so model this in ecx, a dummy in edx,
// and the real flags on the stack.

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

struct Sub {
    void nonvirt_dtor();
};

class CLandscapeBackgroundPatch { public:
    int   field0;      // +0
    Sub   sub;         // +4
};

// scalar destructor of subobject at +4 (retail 0x00a01c10), __fastcall

// operator delete (retail 0x00bfe9bc), cdecl caller-cleanup

void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx dummy*/, uint flags)
{
    ((CBaseIntelligentPointer*)(&self->sub))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    if (flags & 1)
        operator delete(self);
    return self;
}