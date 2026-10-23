// NUISystem::CList::GetDataFromDef() -- copy the list definition into the list: three behaviour flags, the input delay, the position offset (stored, mirrored for use) and the 0..255 alpha offset, then sort the children into scrolling (with a visible flag) and non-scrolling lists by the definition's non-scrolling index table
// Leaf: GetDataFromDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053822B, length 377 bytes; size-optimised frame-pointer TU
//   (`#pragma optimize("s", on)`, callees use `push ebp ; mov ebp,esp` too).
// Callees: the component's virtual slot 108 LoadAndReturnDef() const (CDefPointer by hidden pointer), std::find over
//   the definition's unsigned-long index vector @ 0x0053C8F8 (`__fastcall`, value by reference, ret 4; manifest
//   "_Umove<EWeaponClass*>" is propagated), vector<CComponent*>::push_back @ 0x00486024 (manifest "Vector_PushBack"),
//   vector<bool>::push_back @ 0x0053CB4F (manifest "deque_push_back_or_allocate"), the definition's virtual slot 1
//   (delete at refcount zero). Float pool: 1/255 @ 0x01231724.
// Layout: Ego_r.pdb NUISystem::CList (576 bytes) less 0x30..0x3C bytes of debug-only container words: retail Children
//   (component +0xB0, 8-byte counted pointers), behaviour flags byte +0x12E, ScrollingChildren +0x164, NonScrollingChildren
//   +0x170, PositionOffset +0x188, PositionOffsetToUse +0x190, AlphaOffset +0x198, AlphaOffsetToUse +0x19C,
//   ScrollingChildrenVisibility (vector<bool>) +0x1C8, InputDelay +0x1EC, PDefPositionOffset +0x1F0, PDefAlphaOffset
//   +0x1F8 -- modelled locally (engine/CList.h is the PDB layout with a flat vptr word). The list def is packed
//   (PositionOffset at the unaligned +0x142), as in the landed CDynamicList::GetDataFromDef 0x00541390.
// Shape: same-TU auto_inline(off) stand-ins for find and both push_backs let VC7.1 enregister the unsigned index (homed in
//   [ebp-0Ch] only around the find, sharing the word with the child temporaries and the alpha fild temp);
//   `AlphaOffset = AlphaOffsetToUse = ...` stores the second member first.
#pragma optimize("s", on)
namespace NUISystem {
struct CComponent;
struct C2DVector {
    float X, Y;
};
#pragma pack(push, 1)
struct CListDef {                                  // CIVCountedPointeeBase {vptr, count} + list data
    virtual void v00();
    virtual void Delete() const;                   // slot 1
    long           IVRefCount;                     // +0x04
    unsigned char  _pad08[0x94 - 0x08];
    unsigned long* NonScrollingStart;              // +0x94
    unsigned long* NonScrollingFinish;             // +0x98
    unsigned long* NonScrollingLimit;              // +0x9c
    unsigned char  _padA0[0x140 - 0xa0];
    bool           Wrapping;                       // +0x140
    bool           ScrollWithSelection;            // +0x141
    C2DVector      PositionOffset;                 // +0x142
    unsigned char  AlphaOffset;                    // +0x14a
    bool           NoInputOnInactive;              // +0x14b
    unsigned char  _pad14C[0x184 - 0x14c];
    float          InputDelay;                     // +0x184
    void DecRef() const { --const_cast<CListDef*>(this)->IVRefCount; }
    long GetRefCount() const { return IVRefCount; }
    void Release() const { DecRef(); if (GetRefCount() == 0) Delete(); }
};
#pragma pack(pop)
struct CDefPointer {
    const CListDef* Data;
    ~CDefPointer() { if (Data) Data->Release(); }
    const CListDef* operator->() const { return Data; }
};
const unsigned long* __fastcall find(const unsigned long* first, const unsigned long* last, const unsigned long& value);   // 0x0053C8F8
struct ChildPointer {
    CComponent* Data;
    void*       CountInfo;
    operator CComponent*() const { return Data; }
};
struct ChildVector {
    ChildPointer* Start;
    ChildPointer* Finish;
    ChildPointer* Limit;
    unsigned long size() const { return Finish - Start; }
    const ChildPointer& operator[](unsigned long i) const { return Start[i]; }
};
struct ComponentVector {
    CComponent** Start;
    CComponent** Finish;
    CComponent** Limit;
    void push_back(CComponent* const& value);      // 0x00486024
};
struct BoolVector {
    unsigned char _words[0x14];
    void push_back(const bool& value);             // 0x0053CB4F
};
#pragma auto_inline(off)
const unsigned long* __fastcall find(const unsigned long* first, const unsigned long* last, const unsigned long& value)
{
    for (; first != last; ++first)
        if (*first == value)
            return first;
    return last;
}
void ComponentVector::push_back(CComponent* const& value)
{
    if (Finish != Limit) {
        *Finish = value;
        ++Finish;
    }
}
void BoolVector::push_back(const bool& value)
{
    _words[0] = value;
}
#pragma auto_inline(on)
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
    virtual CDefPointer LoadAndReturnDef() const;                        // slot 108 (+0x1b0)
    unsigned char   _pad004[0xb0 - 0x04];
    ChildVector     Children;                      // +0xb0
    unsigned char   _pad0BC[0x12e - 0xbc];
    unsigned char   BehaviourFlags;                // +0x12e
    unsigned char   _pad12F[0x164 - 0x12f];
    ComponentVector ScrollingChildren;             // +0x164
    ComponentVector NonScrollingChildren;          // +0x170
    unsigned char   _pad17C[0x188 - 0x17c];
    C2DVector       PositionOffset;                // +0x188
    C2DVector       PositionOffsetToUse;           // +0x190
    float           AlphaOffset;                   // +0x198
    float           AlphaOffsetToUse;              // +0x19c
    unsigned char   _pad1A0[0x1c8 - 0x1a0];
    BoolVector      ScrollingChildrenVisibility;   // +0x1c8
    unsigned char   _pad1DC[0x1ec - 0x1dc];
    float           InputDelay;                    // +0x1ec
    C2DVector       PDefPositionOffset;            // +0x1f0
    unsigned char   PDefAlphaOffset;               // +0x1f8
    void GetDataFromDef();
};

void CList::GetDataFromDef()
{
    CDefPointer def = LoadAndReturnDef();
    if (def->Wrapping)
        BehaviourFlags |= 8;
    if (def->NoInputOnInactive)
        BehaviourFlags |= 0x10;
    if (def->ScrollWithSelection)
        BehaviourFlags |= 0x20;
    InputDelay = def->InputDelay;
    PDefPositionOffset = def->PositionOffset;
    PDefAlphaOffset = def->AlphaOffset;
    PositionOffsetToUse = def->PositionOffset;
    PositionOffset = def->PositionOffset;
    PositionOffsetToUse.X = -PositionOffsetToUse.X;
    AlphaOffset = AlphaOffsetToUse = def->AlphaOffset * (1.0f / 255.0f);
    for (unsigned long i = 0; i < Children.size(); ++i) {
        const unsigned long* last = def->NonScrollingFinish;
        if (find(def->NonScrollingStart, last, i) == last) {
            ScrollingChildren.push_back(Children[i]);
            ScrollingChildrenVisibility.push_back(true);
        } else {
            NonScrollingChildren.push_back(Children[i]);
        }
    }
}
}