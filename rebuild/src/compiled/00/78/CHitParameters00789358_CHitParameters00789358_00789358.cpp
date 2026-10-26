// CHitParameters_00789358::~CHitParameters_00789358() -- destroy the intelligent pointers at +0x58 and +0x50 (0x00A01C10), release the counted pointer at +0x4c, free the buffer at +0x3c when set (free @ 0x00BFEA14), then the 12-byte member at +0x30 (0x00451AC6, a tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789358, length 63 bytes.
// The chunk label "CHitParameters::~CHitParameters" is propagated (a different layout); the member label "CActiveFile::OnReadFinished" too. Shape: the size-optimised module (`and dword ptr [esi+4Ch],0`, `pop ecx` after free).
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    __forceinline void Release()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};
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
struct CMember12_00451ac6 {
    unsigned char _body[0xc];
    ~CMember12_00451ac6();                         // 0x00451AC6
};
struct CBuffer_00789358 {                          // a malloc'd block freed by its destructor
    void* m_pBlock;
    ~CBuffer_00789358() { if (m_pBlock) free(m_pBlock); }
};
struct CHitParameters_00789358 {
    unsigned char           _pad[0x30];
    CMember12_00451ac6      m_member30;            // +0x30
    CBuffer_00789358        m_buffer3c;            // +0x3c
    unsigned char           _pad40[0xc];
    CIVCountedPointer       m_counted4c;           // +0x4c
    CBaseIntelligentPointer m_pointer50;           // +0x50
    CBaseIntelligentPointer m_pointer58;           // +0x58
    ~CHitParameters_00789358();
};

CHitParameters_00789358::~CHitParameters_00789358()
{
}