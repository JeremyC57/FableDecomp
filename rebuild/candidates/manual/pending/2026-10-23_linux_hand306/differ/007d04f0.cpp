// Grade: DIFFER(843v851) -- control flow/calls match; frame 0x38 v 0x40 (def/string slots swapped), right-vector components spilled with fstp+reload where retail keeps all six on the x87 stack (fst)
// CTCTavernGameShoveHaPenny::OnInitialActivate() -- after the base activation, find the live table thing whose def index matches the game def's board and read the board's DUMMY_OBJECT_MIN / DUMMY_OBJECT_MAX dummies: the board origin and orientation come from the min dummy, the board size is the min-to-max span in the board's right/forward/up frame, folding any negative axis back onto the origin
// Leaf: ?OnInitialActivate@CTCTavernGameShoveHaPenny
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D04F0, 851 bytes (+ int3 padding), plain `ret`.
// Callees: CTCTavernGame::OnInitialActivate 0x005EA650; CDefClassBase::GetSubDef<CTavernGameShoveHaPennyDef> 0x007D1260;
//   CVectorMap LowerBound (const) 0x004365B0 (interface 0x21); CBaseIntelligentPointer::GetPItem 0x00A01B50;
//   CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0; CTCGraphicAppearance::GetDummyObject 0x004C08C0. 0.0f 0x0122DEDC.
// Layout (retail): BoardPosition +0x10c, BoardOrientation +0x118, BoardSize +0x130 (CalcScores 0x007CF860);
//   game def board index +0x1b0; table component thing list +0x0c; CThing def index (ushort) +0x8c, appearance +0x64.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
inline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct CRightHandedSet { C3DVector Up, Forward; };

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CTavernGameShoveHaPennyDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
    unsigned char _pad08[0x1b0 - 0x08];
    long BoardDefIndex;                         // +0x1b0 (descriptive)
};
struct CShoveHaPennyDefPointer {
    CTavernGameShoveHaPennyDef* Object;
    CShoveHaPennyDefPointer() : Object(0) { }
    ~CShoveHaPennyDefPointer()
    {
        if (Object) {
            if (--Object->m_count == 0)
                Object->Destroy();
        }
    }
    CTavernGameShoveHaPennyDef* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CShoveHaPennyDefPointer* out);   // 0x007D1260
};
CTavernGameShoveHaPennyDef* FindRef_GetSubDef(CDefClassBase* defClass);
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CShoveHaPennyDefPointer* out)
{
    CTavernGameShoveHaPennyDef* def = FindRef_GetSubDef(this);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* GraphicAppearance;    // +0x64
    unsigned char _pad68[0x70 - 0x68];
    CDefClassBase* DefClass;                    // +0x70
    unsigned char _pad74[0x8c - 0x74];
    unsigned short DefIndex;                    // +0x8c
    unsigned char _pad8e[0x91 - 0x8e];
    unsigned char Flags91;                      // +0x91 bit 0: dead
    unsigned long HasTC(long t) const { return m_presentBits[t >> 5] & (1UL << (t & 31)); }
    template<class T> __forceinline T* GetTC(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    void* _vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
template<class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
};
struct CTCThingList {                           // interface 0x21 (descriptive)
    unsigned char _pad00[0x0c];
    CRetailList<CBaseIntelligentPointer> Things;   // +0x0c
};

struct CTCTavernGame {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    void OnInitialActivate();                   // 0x005EA650
};
struct CTCTavernGameShoveHaPenny : CTCTavernGame {
    unsigned char _pad08[0x10c - 0x08];
    C3DVector BoardPosition;                    // +0x10c
    CRightHandedSet BoardOrientation;           // +0x118
    C3DVector BoardSize;                        // +0x130
    void OnInitialActivate();
};

void CTCTavernGameShoveHaPenny::OnInitialActivate()
{
    CTCTavernGame::OnInitialActivate();
    CShoveHaPennyDefPointer def;
    ParentThing->DefClass->GetSubDef(&def);
    if (ParentThing->HasTC(0x21)) {
        CTCThingList* list = ParentThing->GetTC<CTCThingList>(0x21);
        for (CRetailList<CBaseIntelligentPointer>::node* n = list->Things.head->next; n != list->Things.head; n = n->next) {
            CThing* thing = n->value.GetPItem();
            if (thing && !(thing->Flags91 & 1) && thing->DefIndex == def->BoardDefIndex) {
                CTCGraphicAppearance* appearance = thing->GraphicAppearance;
                C3DVector minPos, maxPos;
                CRightHandedSet orientation;
                appearance->GetDummyObject("DUMMY_OBJECT_MIN", 0, 0, &minPos, &orientation, 1.0f);
                appearance->GetDummyObject("DUMMY_OBJECT_MAX", 0, 0, &maxPos, &orientation, 1.0f);
                C3DVector span = maxPos - minPos;
                BoardPosition = minPos;
                BoardOrientation = orientation;
                C3DVector right = CrossProduct(orientation.Forward, orientation.Up);
                BoardSize = C3DVector(DotProduct(span, right), DotProduct(span, orientation.Forward), DotProduct(span, orientation.Up));
                if (BoardSize.X < 0.0f) {
                    BoardPosition += right * BoardSize.X;
                    BoardSize.X = -BoardSize.X;
                }
                if (BoardSize.Y < 0.0f) {
                    BoardPosition += orientation.Forward * BoardSize.Y;
                    BoardSize.Y = -BoardSize.Y;
                }
                if (BoardSize.Z < 0.0f) {
                    BoardPosition += orientation.Up * BoardSize.Z;
                    BoardSize.Z = -BoardSize.Z;
                }
                BoardPosition.Z += BoardSize.Z;
                break;
            }
        }
    }
}
