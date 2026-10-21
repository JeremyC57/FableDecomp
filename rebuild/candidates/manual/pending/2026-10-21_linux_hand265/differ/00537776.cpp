// NUISystem::CList::CanMove(const C2DVector& position) -- the position the list may move to: unchanged, or with y pulled back to +0x40 when the (converted) target would leave the owner's visible band
// Grade: DIFFER(480v480) -- same length and instruction stream except: frame 0x3c vs retail 0x48 (retail keeps top int, the sum temporary and the margin in separate slots), and the first ConvertX(...) + Offset loads Offset.x first where retail loads the converted x first (the scroller sum matches)
// Retail 0x00537776, 479 bytes, `ret 8` (hidden C2DVector return, position).
// Callees: slot 108 (+0x1B0) GetOwner (counted owner by hidden pointer), slot 111 (+0x1BC) a C2DVector, slot 117
//   (+0x1D4) bool, __fastcall CFrontEndManager::GetInstance 0x0041E5F2, CManager::GetUIScale 0x0041CF47 (C2DVector by
//   hidden pointer), __fastcall ConvertX(C2DVector) 0x0052E580 (hidden pointer in ecx), __stdcall
//   LengthScaleConversion(float) 0x0052E4D0, the scroller's slot 118 (+0x1D8) C2DVector, __ftol2 0x00BFEA70.
// Layout: this +0x34 Position (C2DVector), +0x40 float, +0xF8 C2DVector offset, +0x1D4 scroller pointer; owner
//   +4 reference count (slot 1 Delete), +0x150 top, +0x158 bottom (longs). The PDB CList is padding only.
// Shape: the frame-pointer size TU of the landed ChangePosition 0x00537956; the owner release is the GOTCHAS
//   DecRef()/GetRefCount() accessor pair (`dec [eax] ; cmp [eax],0`).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C2DVector {
    float x, y;
    C2DVector() { }
    C2DVector(float ax, float ay) : x(ax), y(ay) { }
    C2DVector(const C2DVector& other) : x(other.x), y(other.y) { }
    C2DVector operator-(const C2DVector& other) const { return C2DVector(x - other.x, y - other.y); }
};
inline C2DVector operator+(const C2DVector& a, const C2DVector& b) { return C2DVector(a.x + b.x, a.y + b.y); }
struct CListOwner {
    virtual void v000();
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +0x04
    unsigned char _pad08[0x148];
    long          Top;                             // +0x150
    long          _154;
    long          Bottom;                          // +0x158
    void DecRef() { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
struct CListOwnerPointer {
    CListOwner* m_pObject;
    CListOwnerPointer() { }
    CListOwnerPointer(const CListOwnerPointer& other);
    CListOwner* operator->() const { return m_pObject; }
    ~CListOwnerPointer()
    {
        if (m_pObject)
            m_pObject->Release();
    }
};
struct CManager {
    C2DVector GetUIScale();                        // 0x0041CF47
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
C2DVector __fastcall ConvertX(C2DVector v);        // 0x0052E580
float __stdcall LengthScaleConversion(float v);    // 0x0052E4D0
struct CScroller {
    virtual void s000(); virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007();
    virtual void s008(); virtual void s009(); virtual void s010(); virtual void s011(); virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015();
    virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019(); virtual void s020(); virtual void s021(); virtual void s022(); virtual void s023();
    virtual void s024(); virtual void s025(); virtual void s026(); virtual void s027(); virtual void s028(); virtual void s029(); virtual void s030(); virtual void s031();
    virtual void s032(); virtual void s033(); virtual void s034(); virtual void s035(); virtual void s036(); virtual void s037(); virtual void s038(); virtual void s039();
    virtual void s040(); virtual void s041(); virtual void s042(); virtual void s043(); virtual void s044(); virtual void s045(); virtual void s046(); virtual void s047();
    virtual void s048(); virtual void s049(); virtual void s050(); virtual void s051(); virtual void s052(); virtual void s053(); virtual void s054(); virtual void s055();
    virtual void s056(); virtual void s057(); virtual void s058(); virtual void s059(); virtual void s060(); virtual void s061(); virtual void s062(); virtual void s063();
    virtual void s064(); virtual void s065(); virtual void s066(); virtual void s067(); virtual void s068(); virtual void s069(); virtual void s070(); virtual void s071();
    virtual void s072(); virtual void s073(); virtual void s074(); virtual void s075(); virtual void s076(); virtual void s077(); virtual void s078(); virtual void s079();
    virtual void s080(); virtual void s081(); virtual void s082(); virtual void s083(); virtual void s084(); virtual void s085(); virtual void s086(); virtual void s087();
    virtual void s088(); virtual void s089(); virtual void s090(); virtual void s091(); virtual void s092(); virtual void s093(); virtual void s094(); virtual void s095();
    virtual void s096(); virtual void s097(); virtual void s098(); virtual void s099(); virtual void s100(); virtual void s101(); virtual void s102(); virtual void s103();
    virtual void s104(); virtual void s105(); virtual void s106(); virtual void s107(); virtual void s108(); virtual void s109(); virtual void s110(); virtual void s111();
    virtual void s112(); virtual void s113(); virtual void s114(); virtual void s115(); virtual void s116(); virtual void s117();
    virtual C2DVector GetOffset();                     // slot 118 (+0x1D8)
};
namespace NUISystem {
struct CList {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual CListOwnerPointer GetOwner();                  // slot 108 (+0x1B0)
    virtual void v109(); virtual void v110();
    virtual C2DVector GetSize();                           // slot 111 (+0x1BC)
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116();
    virtual bool IsScrolling();                            // slot 117 (+0x1D4)
    unsigned char _pad04[0x30];
    C2DVector     Position;                        // +0x34
    float         _3c;
    float         m_40;                            // +0x40
    unsigned char _pad44[0xb4];
    C2DVector     Offset;                          // +0xF8
    unsigned char _pad100[0xd4];
    CScroller*    Scroller;                        // +0x1D4
    C2DVector CanMove(const C2DVector& position);
};
}

C2DVector NUISystem::CList::CanMove(const C2DVector& position)
{
    C2DVector original(position);
    C2DVector target(position);
    CListOwnerPointer owner = GetOwner();
    bool up = position.y < Position.y;
    long top = owner->Top;
    long bottom = owner->Bottom;
    CFrontEndManager::GetInstance()->GetUIScale();
    float topEdge = (float)top;
    long bottomEdge = (long)(topEdge + (float)(bottom - top) * CFrontEndManager::GetInstance()->GetUIScale().y);
    if (IsScrolling())
        target = ConvertX(position - Position) + Offset;
    float margin = LengthScaleConversion(topEdge) - topEdge;
    margin *= CFrontEndManager::GetInstance()->GetUIScale().y;
    if (Scroller)
        target = ConvertX(position - Position) + Scroller->GetOffset();
    float scaleY;
    return C2DVector(original.x, (up ? (scaleY = CFrontEndManager::GetInstance()->GetUIScale().y, scaleY * GetSize().y + target.y < (float)bottomEdge + margin)
                                     : topEdge + margin < target.y) ? m_40 : original.y);
}
