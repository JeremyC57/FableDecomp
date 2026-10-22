// Grade: DIFFER(701v692) -- the inline font-frame texture fetch: retail cross-jumps the two `return CGraphicFrame(Data->Frame).GetTexture()` tails (one copy of the ref-count/delete tail, laid out after the forced-load test); this single-return form puts the tail after the touch block instead. The early-return form (one tail, matches the layout) shifts the zero constant into ebx and the primitive into edi. Also the layer byte index is [layer+global] in retail.
// CEnginePrimitiveManagerText::Render2DPrimitive(CEngineInternalPrimitiveBase&, CEngineSubPrimitiveChain&, long) -- queue a 2D text primitive: when visible and its layer is enabled, build a pool-allocated CEngineSubPrimitiveText (position, colour, string, font texture from the font's graphic frame) and add it at the layer's depth
// Leaf: Render2DPrimitive
// Layout: local models (landed 2DStaticMeshes Render2DPrimitive 0x00B9E950, SubPrimitiveText ctor 0x00BC7A50, GetVolumeTexture 0x009FF710, CGraphicFrame operator= 0x0041C0E0).
void __cdecl operator delete(void* block);         // 0x00BFE9BC
void* operator new(unsigned int size);             // 0x00BFEA1A
struct CEngineInternalPrimitiveBase;
struct CEngineSubPrimitiveChain;
struct CCharString {
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099B7D0
};
struct CRGBColour { unsigned char B, G, R, A; };

struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void* object);
    void* m_pObject;
};
struct CTexture;
struct CCountedPointer_CTexture {
    CTexture*    m_pObject;
    SCountBlock* m_pCount;
    __forceinline ~CCountedPointer_CTexture()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct CGraphicFrameData {
    unsigned long            ReferenceCount;       // +0x00
    void*                    Data;                 // +0x04
    CCountedPointer_CTexture PTexture;             // +0x08
    unsigned char            _pad10[0xa];
    unsigned char            m_flags;              // +0x1a
    void Uninitialise();                           // 0x009FCBA0
    __forceinline ~CGraphicFrameData()
    {
        if (m_flags & 0x10)
            Uninitialise();
    }
};
struct CGraphicFrame {
    CGraphicFrameData* FrameData;
    CGraphicFrame(const CGraphicFrame& other) : FrameData(other.FrameData) { if (FrameData) ++FrameData->ReferenceCount; }
    __forceinline ~CGraphicFrame()
    {
        if (--FrameData->ReferenceCount == 0)
            delete FrameData;
    }
    CTexture* GetTexture() const { return FrameData->PTexture.m_pObject; }
};

struct CResource;
struct CResourceList {
    unsigned char _pad00[4];
    CResource*    Tail;                            // +0x04
    unsigned char _pad08[0x10 - 0x08];
    CResource*    Head;                            // +0x10
    unsigned char _pad14[0x2c - 0x14];
    unsigned long Stamp;                           // +0x2c
};
struct CResource {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+4)
    long           m_references;                   // +0x04
    CResourceList* List;                           // +0x08
    unsigned char  _pad0C[0x18 - 0x0c];
    unsigned long  Stamp;                          // +0x18
    void ListRemove();                             // 0x009FBFB0
    void ListInsert(CResource* before, CResource* after);   // 0x009FBF90
    __forceinline void Touch()
    {
        if (List && Stamp != List->Stamp) {
            ListRemove();
            ListInsert(List->Head, (CResource*)&List->Tail);
            Stamp = List->Stamp;
        }
    }
};
struct CVolumeHolder {
    void*      _word00;                            // +0x00
    CResource* Volume;                             // +0x04
};
struct CGraphicInfoFrame;
struct CFrameData : CResource {
    CGraphicFrame Frame;                           // +0x1c
    CVolumeHolder VolumeHolder;                    // +0x20
    unsigned char _pad28[0x38 - 0x28];
    signed char   SmallestResidentMip;             // +0x38
    unsigned char _pad39;
    signed char   LowestRequestedMip;              // +0x3a
    unsigned char _pad3B;
    unsigned long LastFrameStamp;                  // +0x3c
    unsigned char _pad40[4];
    CFrameData(CGraphicInfoFrame* frame);          // 0x009FE380
    void BeginLoading(unsigned long a, unsigned long b);   // 0x009FDE80
};
struct CFrameCounter {
    unsigned char _pad[0x194];
    unsigned long FrameStamp;                      // +0x194
};
struct CGraphicInfo {
    unsigned char  _pad[0x20];
    CFrameCounter* Counter;                        // +0x20
};
struct CGraphicInfoFrame {
    void*          _word00;
    unsigned short FrameNum;
    signed char    SmallMipmapLevel;
    CFrameData*    Data;                           // +0x08
    CGraphicInfo*  GraphicInfo;                    // +0x0c
    bool InternalGetFrameForcedLoading(long flags);   // 0x009FF6B0
};
template <class T> struct CIVCountedPointer {
    T* Data;
    void Reset()
    {
        T* p = Data;
        if (p) {
            if (--p->m_references == 0)
                p->Destroy();
            Data = 0;
        }
    }
    CIVCountedPointer& operator=(T* p) { Reset(); Data = p; return *this; }
    T* operator->() const { return Data; }
};
struct CGraphicInfoFrameT {
    void*                         _word00;
    unsigned short                FrameNum;
    signed char                   SmallMipmapLevel;
    CIVCountedPointer<CFrameData> Data;            // +0x08
    CGraphicInfo*                 GraphicInfo;     // +0x0c
    bool InternalGetFrameForcedLoading(long flags);   // 0x009FF6B0
    __forceinline CTexture* GetFrameTexture(unsigned long mip, unsigned long flags)
    {
        if (Data.Data && Data->Frame.FrameData) {
            Data->LastFrameStamp = GraphicInfo->Counter->FrameStamp;
            CVolumeHolder& holder = Data->VolumeHolder;
            if (holder.Volume)
                holder.Volume->Touch();
            Data->Touch();
            if ((unsigned long)Data->LowestRequestedMip > mip)
                Data->LowestRequestedMip = (signed char)mip;
            if ((unsigned long)Data->SmallestResidentMip > mip)
                Data->BeginLoading(mip, flags);
        } else {
            if (!Data.Data)
                Data = new CFrameData((CGraphicInfoFrame*)this);
            Data->BeginLoading(mip, flags);
            if (!InternalGetFrameForcedLoading(1))
                return 0;
        }
        CGraphicFrame frame(Data->Frame);
        return frame.GetTexture();
    }
};
struct SBankGraphic {
    unsigned char       _pad[0x28];
    CGraphicInfoFrameT* m_pFrame;                  // +0x28
};
template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    T* end() { return m_pLast; }
    unsigned long size() { return end() - begin(); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct CGraphicBank {
    unsigned char               _pad[0x1e0];
    CRetailVector<SBankGraphic> m_records;         // +0x1e0
    __forceinline SBankGraphic* Get(unsigned long index) { return index < m_records.size() ? &m_records[index] : 0; }
    __forceinline CGraphicInfoFrameT* GetFrame(unsigned long index) { return Get(index) ? Get(index)->m_pFrame : 0; }
};
struct CEngineGlobal_01436e98 {
    unsigned char _pad[0x954];
    CGraphicBank* m_pGraphicBank;                  // +0x954
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CFontGraphic {
    void*         Graphic;                         // +0x00
    unsigned long BankIndex;                       // +0x04
};
struct CFont {
    unsigned char _pad[0xc];
    CFontGraphic  Info;                            // +0x0c
};
struct CTextSettings {
    unsigned char _pad[0x94];
    CFont*        Font;                            // +0x94
};
extern CTextSettings* g_pEnginePrimitiveTypeInfo;  // 0x01436E84

struct CEnginePrimitiveRenderer2D {
    unsigned char _pad[0x320];
    bool          LayerEnabled[1];                 // +0x320
    float Layer2DToDistance(long layer);           // 0x00B4A6A0
};
extern CEnginePrimitiveRenderer2D* g_pPrimitiveRenderer2D;   // 0x01436E7C

struct CBase {
    virtual void v00();
    CBase();
};
struct __declspec(novtable) CEngineSubPrimitiveBase : CBase {
    unsigned long RenderFlags;                     // +0x04
    unsigned char Alpha;                           // +0x08
    static void* __fastcall operator new(unsigned int size);   // 0x00BEB540
};
struct CEngineInternalPrimitiveText {
    unsigned char _pad00[0x48];
    float         X;                               // +0x48
    float         Y;                               // +0x4c
    unsigned char _pad50[4];
    float         Scale;                           // +0x54
    CRGBColour    Colour;                          // +0x58
    unsigned char _pad5c[4];
    CCharString   Text;                            // +0x60
    long          Justification;                   // +0x64
    long          Layer;                           // +0x68
};
struct CEngineSubPrimitiveText : CEngineSubPrimitiveBase {   // 0x38 bytes
    unsigned char _pad0c[0x18 - 0x0c];
    float         X;                               // +0x18
    float         Y;                               // +0x1c
    float         Z;                               // +0x20
    float         Scale;                           // +0x24
    CRGBColour    Colour;                          // +0x28
    CCharString   Text;                            // +0x2c
    long          Justification;                   // +0x30
    CTexture*     Texture;                         // +0x34
    CEngineSubPrimitiveText(CEngineInternalPrimitiveText* primitive);   // 0x00BC7A50
};
struct CEnginePrimitiveManager;
struct CEngineSubPrimitiveRenderer {
    void AddSubPrimitive(long list, CEngineSubPrimitiveBase* subPrimitive, float distance, CEnginePrimitiveManager* manager);   // 0x00B84720
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74

struct CEnginePrimitiveManagerText {
    virtual void Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, long type);
};

void CEnginePrimitiveManagerText::Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, long type)
{
    CEngineInternalPrimitiveText& text = (CEngineInternalPrimitiveText&)primitive;
    if (!text.Colour.A)
        return;
    const long& layer = text.Layer;
    if (!g_pPrimitiveRenderer2D->LayerEnabled[layer])
        return;
    CEngineSubPrimitiveText* sub = new CEngineSubPrimitiveText(&text);
    sub->X = text.X;
    sub->Y = text.Y;
    sub->Z = 0;
    sub->Scale = text.Scale;
    sub->Colour = text.Colour;
    sub->Text = text.Text;
    sub->Justification = text.Justification;
    CFontGraphic& font = g_pEnginePrimitiveTypeInfo->Font->Info;
    if (font.BankIndex && font.Graphic)
        sub->Texture = g_pEngine_01436e98->m_pGraphicBank->GetFrame(font.BankIndex)->GetFrameTexture(0, 1);
    else
        sub->Texture = 0;
    sub->Alpha = sub->Colour.A;
    sub->RenderFlags = 1;
    float depth = g_pPrimitiveRenderer2D->Layer2DToDistance(text.Layer);
    g_pEngineSubPrimitiveRenderer->AddSubPrimitive(type, sub, depth, (CEnginePrimitiveManager*)this);
}
