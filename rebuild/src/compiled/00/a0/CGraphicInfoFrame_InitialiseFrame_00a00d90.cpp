// CGraphicInfoFrame::InitialiseFrame() -- when the counted frame-data pointer at +8 is empty, create a 0x44-byte CFrameData for this frame (0x009FE380) and assign it (the old pointee released through the inlined counted assignment); returns the frame data
// Leaf: InitialiseFrame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A00D90, length 72 bytes.
// Callees: operator new @ 0x00BFEA1A (0x44), CFrameData::CFrameData @ 0x009FE380 (on the block, `this`; the null arm yields 0), the old pointee's virtual slot 1 (Delete)
// when its count reaches zero. Shape: the new object kept in edi, the inlined release (`dec [ecx+4] ; jne ; call [eax+4]`, the zero store) then the plain store.
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};
class CGraphicInfoFrame;
struct CFrameData_00a00d90 : CIVCountedPointeeBase {
    unsigned char _body[0x44 - 8];
    CFrameData_00a00d90(CGraphicInfoFrame* frame);   // 0x009FE380
};
struct CIVCountedPointer_00a00d90 {
    CIVCountedPointeeBase* Object;
    void Release()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
    void Assign(CIVCountedPointeeBase* object)     // the new object arrives already counted
    {
        Release();
        Object = object;
    }
};

class CGraphicInfoFrame { public:
    unsigned char             _pad[8];
    CIVCountedPointer_00a00d90 m_data;             // +0x08
    CFrameData_00a00d90* InitialiseFrame();
};

CFrameData_00a00d90* CGraphicInfoFrame::InitialiseFrame()
{
    if (!m_data.Object)
        m_data.Assign(new CFrameData_00a00d90(this));
    return (CFrameData_00a00d90*)m_data.Object;
}