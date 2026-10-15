#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDraw2DText::Draw(CEnginePrimitivePrepareInfo*) -- hand every queued 2D text primitive to the renderer (one primitive handle each), then empty the queue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006541D1, length 135 bytes.
// Callees: vector<CEnginePrimitiveHandle>::Resize(long) @ 0x00443F90, CEnginePrimitive2DText::CEnginePrimitive2DText(const
// CEnginePrimitive2DText&) @ 0x00655048, the renderer's AddPrimitive (slot 23, `call [eax+5Ch]`, on the prepare info's +0x34
// pointer), CWideString::~CWideString() @ 0x0099B510 (the copy's only non-trivial member), list<CEnginePrimitive2DText>::clear()
// @ 0x00440770 (manifest "CGuiControlTreePane::SortTreeRecursively" is propagated; the landed ClearPrimitivesIfNecessary 0x00646567
// calls it on the same list).
// Layout: CDrawBase Drawing +0x18, PrimitiveHandles +0x1c; PDB CDraw2DText TextPrimitives +0x2c -> retail +0x28 (VC6 list, one
// sentinel pointer); CEnginePrimitive2DText (PDB 44 bytes): Pos +0xc, Layer +0x18, Scale +0x1c, Colour +0x20, Text +0x24, PFont +0x28.
// Shape: frame-pointer size TU; the list is reached through a pointer local (edi); size() walks the nodes (VC6 list, no stored
// count; the count is a named local taken BEFORE the handle-vector pointer local, else the lea is hoisted above the walk);
// the handle vector is cached in a frame slot ([ebp-8]) and indexed as `*(begin() + i)` (the 8-byte stride kept as a
// running offset in [ebp-4], begin() re-read per pass); each node is copied into a frame primitive whose destructor runs per pass.
struct CWideString {
    unsigned short* m_pData;
    ~CWideString();                                // 0x0099B510
};

struct CEnginePrimitiveHandle {                    // 8 bytes
    void* _vtable;
    void* m_pData;
};

struct CEnginePrimitive2DText {                    // PDB 44 bytes
    unsigned char _base[0xc];                      // CEnginePrimitive base
    float         Pos[3];                          // +0x0c C3DVector
    long          Layer;                           // +0x18 EEngine2DLayer
    float         Scale;                           // +0x1c
    unsigned long Colour;                          // +0x20 CRGBColour
    CWideString   Text;                            // +0x24
    void*         PFont;                           // +0x28 CFontBank*
    CEnginePrimitive2DText(const CEnginePrimitive2DText& other);   // 0x00655048
};

struct CEnginePrimitiveRenderer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DText* primitive, long flags);   // slot 23
};

struct CEnginePrimitivePrepareInfo {
    unsigned char             _pad[0x34];
    CEnginePrimitiveRenderer* Renderer;            // +0x34
};

namespace NPlayerGui {

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    CEnginePrimitiveHandle* begin() { return m_pFirst; }
    CEnginePrimitiveHandle& operator[](unsigned long n) { return *(begin() + n); }
    void Resize(long count);                       // 0x00443F90
};

struct CRetailTextList {                           // VC6 Dinkumware list<CEnginePrimitive2DText>: one sentinel pointer
    struct node { node* next; node* prev; CEnginePrimitive2DText value; };
    node* head;
    bool empty() const { return head->next == head; }
    unsigned long size() const
    {
        unsigned long count = 0;
        for (node* n = head->next; n != head; n = n->next)
            ++count;
        return count;
    }
    void clear();                                  // 0x00440770
};

struct CDraw2DText {
    void*               _vtable;                   // +0x00
    unsigned char       _base04[0x18 - 4];         // GUI pointers
    bool                Drawing;                   // +0x18
    CRetailHandleVector PrimitiveHandles;          // +0x1c
    CRetailTextList     TextPrimitives;            // +0x28
    void Draw(CEnginePrimitivePrepareInfo* info);
};

void CDraw2DText::Draw(CEnginePrimitivePrepareInfo* info)
{
    CRetailTextList* texts = &TextPrimitives;
    if (!texts->empty()) {
        Drawing = true;
        unsigned long count = texts->size();
        CRetailHandleVector* handles = &PrimitiveHandles;
        handles->Resize(count);
        unsigned long i = 0;
        for (CRetailTextList::node* n = texts->head->next; n != texts->head; n = n->next, ++i) {
            CEnginePrimitive2DText primitive(n->value);
            info->Renderer->AddPrimitive(&(*handles)[i], &primitive, 0x40);
        }
        texts->clear();
    }
}

}