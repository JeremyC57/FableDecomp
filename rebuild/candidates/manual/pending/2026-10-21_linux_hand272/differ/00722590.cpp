// CTCSmokeGenerator::CreatePositions() -- once the owner's graphic appearance has its resources, collect the focus_chimney_01..09 helper points as smoke generation positions (stopping at the first missing one), then take the generation distance from the smoke def and mark the positions created
// Grade: DIFFER(434v425) -- frame 12 bytes larger (sub esp,0x3c v 0x30): retail lets the smoke def pointer reuse the loop's prefix string word and packs the helper position / CGenerationPos temporary tighter; the def release reloads the pointer instead of retail's single ecx (dec [ecx+4]). Tried if/else Format (fixed the per-arm pushes), Release member, HasTC+GetTC, declaration order.
// Retail: Fable.exe @ 0x00722590, 432 bytes (plain `ret`).
// Callees: LowerBound 0x004365B0 (const overload), CTCGraphicAppearance::IsResourcesLoaded 0x004BE3E0 / GetHelperPoint 0x004C0560,
//   CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, operator+ 0x0099F570 (__fastcall),
//   CCharString::Format 0x0099F1F0 (__cdecl varargs), CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 /
//   dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), vector<CGenerationPos>::push_back 0x00723270,
//   CDefClassBase::GetSubDef<CSmokeGeneratorDef> 0x00722D30; def release slot 1.
// Layout: Ego_r.pdb CTCSmokeGenerator with the retail 12-byte vector: GenerationDistance +0x0c, GenerationPositions +0x10,
//   PositionsCreated +0x1c; CGenerationPos {C3DVector, CIntelligentPointer<CThing>} 0x18 bytes; CThing TC bitset +0x20
//   (0x5b = +0x28 bit 27), interface map +0x44, def class +0x70; the smoke def's distance +0x28.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
};
struct CCharString {
    void* m_data;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    void Format(const char* format, ...);               // 0x0099F1F0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
extern CCharString g_CCharString_013bacc0;              // 0x013BACC0 "focus_chimney_"
class CThing;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    long    m_link;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
};
struct CGenerationPos {
    C3DVector               Position;
    CIntelligentPointerThing Thing;
    CGenerationPos(const C3DVector& position, CThing* thing) : Position(position), Thing(thing) {}
};
struct CGenerationPosVector {
    CGenerationPos* _M_start; CGenerationPos* _M_finish; CGenerationPos* _M_end_of_storage;
    void push_back(const CGenerationPos& pos);         // 0x00723270
};
struct CTCGraphicAppearance {
    bool IsResourcesLoaded(bool flag);                  // 0x004BE3E0
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position, float time);   // 0x004C0560
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last; SEntry* end;
    const SEntry* LowerBound(const long& key) const;   // 0x004365B0
};
struct CSmokeGeneratorDef {
    virtual void v00();
    virtual void Destroy();
    long RefCount;
    unsigned char _pad08[0x20];
    float GenerationDistance;                           // +0x28
};
struct CSmokeDefPtr {
    CSmokeGeneratorDef* Object;
    CSmokeDefPtr() : Object(0) {}
    ~CSmokeDefPtr() { if (--Object->RefCount == 0) Object->Destroy(); }
};
struct CDefClassBase { bool GetSubDef(CSmokeDefPtr* out); };   // 0x00722D30
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long TCBits[9];                            // +0x20
    CInterfaceMap Interfaces;                           // +0x44
    unsigned char _pad50[0x20];
    CDefClassBase* DefClass;                            // +0x70
    template <class T> __forceinline bool GetInterface(long type, T** out) const
    {
        if (TCBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &Interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)(it == map->last || type < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCSmokeGenerator {
    void*   __vftable;
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[4];
    float   GenerationDistance;                         // +0x0c
    CGenerationPosVector GenerationPositions;           // +0x10
    bool    PositionsCreated;                           // +0x1c
    void CreatePositions();
};

void CTCSmokeGenerator::CreatePositions()
{
    CTCGraphicAppearance* appearance;
    if (!ParentThing->GetInterface(0x5b, &appearance))
        return;
    if (!appearance->IsResourcesLoaded(true))
        return;
    for (long i = 1; i < 10; ++i)
    {
        CCharString prefix = CCharString("") + g_CCharString_013bacc0;
        CCharString number;
        if (i < 10)
            number.Format("0%d", i);
        else
            number.Format("%d", i);
        CCharString helperName = prefix + number;
        C3DVector position;
        if (!appearance->GetHelperPoint(helperName, 0, 0, &position, 1.0f))
            break;
        GenerationPositions.push_back(CGenerationPos(position, 0));
    }
    CSmokeDefPtr def;
    ParentThing->DefClass->GetSubDef(&def);
    GenerationDistance = def.Object->GenerationDistance;
    PositionsCreated = true;
}
