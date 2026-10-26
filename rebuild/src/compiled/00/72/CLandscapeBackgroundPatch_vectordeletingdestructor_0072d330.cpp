// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector deleting destructor'
// Leaf: `vector_deleting_destructor'
// retail 0x0047b300, __fastcall(self, uint flags) -> void*
//
// Pattern:
//   push esi; mov esi,ecx
//   lea ecx,[esi+4]; call innerDtor   ; scalar dtor of member/base at +4
//   test byte[esp+8],1; je S
//   push esi; call operator delete; add esp,4
// S: mov eax,esi; pop esi; ret 4

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
struct Sub;                 // subobject at offset +4
class CLandscapeBackgroundPatch { public:
    int   m_vtbl;           // +0  (vtable / first dword)
    Sub*  dummy;            // placeholder so member sits at +4
};

// scalar destructor operating on the subobject located at self+4.
// __fastcall => arg1 in ecx. Single pointer param = ecx.

// operator delete (cdecl)

// __fastcall: this in ecx, flags on the stack. Model via __fastcall with an
// unused edx dummy so the real `flags` param lands on the stack (=> ret 4).
extern "C" void* __fastcall VecDelDtor(CLandscapeBackgroundPatch* self, int edxDummy, unsigned int flags)
{
    (void)edxDummy;
    // scalar dtor on the subobject at self+4
    ((CBaseIntelligentPointer*)((char*)self + 4))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    if (flags & 1)
        operator delete(self);
    return self;
}