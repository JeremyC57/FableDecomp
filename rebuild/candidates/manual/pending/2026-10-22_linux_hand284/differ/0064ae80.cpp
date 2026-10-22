// NPlayerGui::CDrawAbilityButtons::DrawAbilityButton(CEnginePrimitivePrepareInfo*, vector<CDefPointer<const CHeroAbilityDef> >*, long, long, long, float) -- draw one ability button: for a live hero with the abilities component, place the button from the definition's ability origin, the slot's button-type offsets and the row step; with an ability def use its graphic (the alternate graphic for ability 4 when component 0x93 is present) and, when the hero cannot use it (or the special-abilities component 0xa7 reports it unavailable), add a half-alpha locked overlay sprite into the button's handle; without a def use the default graphic at a third of the scale
// Grade: DIFFER(812v812) -- two local hunks: (1) retail sums pos.Y as ((pos.Y + typeY) + row*step) + offset with faddp, ours reassociates to row*step first and adds pos.Y last (every single-expression form canonicalises the same; split statements or a float temp give the right x87 order but swap the pos / presence-cache frame words and add an &def->ButtonTypes CSE; the double-sum lever `(float)(((double)pos.Y + typeY) + ...)` also gives the right x87 order with the same frame swap); (2) retail materialises &abilities[index] (`lea ebx,[eax+ebx*4]`) before the AddRef copy, ours loads the element directly
// Leaf: DrawAbilityButton
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064AE80, length 812 bytes (frame pointer /Os TU, `ret 0x18`).
// Callees: GetMainPlayer 0x00449970, GetControlledCreature 0x00487DD0, IS_THING_ALIVE 0x0043DBF0, virtual slot 6 (button types),
//   bitset::test 0x00410DE0, LowerBound 0x004365B0, PeekGuiDef 0x00438570, GFA position helper 0x00643D0C, CTCInventoryAbilities::
//   IsAbleToUseAbility 0x005EE49A, CTCSpecialAbilities 0x0072C430, faded alpha 0x00643D5D, CEnginePrimitive2DSprite 0x00493AC2,
//   renderer slot 23, DrawButton 0x00645099, CRT free 0x00BFEA14.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stdlib.h>
struct CTCBase;
struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    mutable CPresenceBits m_present;               // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long type) const
    {
        if (m_present.test(type)) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
bool IsThingAliveImpl(const void* thing);
#pragma auto_inline(off)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && IsThingAliveImpl(thing);
}
#pragma auto_inline(on)
struct CPlayer {
    CThing* GetControlledCreature();               // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
};
struct LongVector {
    long *Start, *Finish, *End;
    LongVector() : Start(0), Finish(0), End(0) {}
    ~LongVector() { if (Start) free(Start); }
    unsigned int size() const { return Finish - Start; }
};
struct CRefCounted {
    long m_count;
    void AddRef() { ++m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
struct CHeroAbilityDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x3c - 0x08];
    long          Ability;                         // +0x3c
    unsigned char _pad40[4];
    long          Graphic;                         // +0x44
    __forceinline void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    void Adopt(const CDefPointer& other)
    {
        T* object = other.Object;
        if (object) {
            object->AddRef();
            Object = object;
        }
    }
};
struct AbilityVector {                             // vector<CDefPointer<const CHeroAbilityDef> >
    CDefPointer<CHeroAbilityDef> *Start, *Finish, *End;
    unsigned int size() const { return Finish - Start; }
};
struct CTCInventoryAbilities {
    bool IsAbleToUseAbility(long ability, bool b);     // 0x005EE49A
};
struct CTCSpecialAbilities {
    bool GetAbilityAvailability(long ability);         // 0x0072C430
};
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DCoordF {
    float X, Y;
    C2DCoordF(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
void __fastcall GFAPositionToScreen(C2DVector* out, const C2DVector* in);   // 0x00643D0C
struct CEnginePrimitiveHandle { unsigned long Data[2]; };
class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DCoordF& pos, long layer, unsigned long graphic, unsigned long b, long blend,
                             long interpolation, float scaleX, float scaleY, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long e, const C2DVector& uv0, const C2DVector& uv1);   // 0x00493AC2
};
class CEnginePrimitiveRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 23
};
class CEnginePrimitivePrepareInfo {
public:
    unsigned char _pad00[0x34];
    CEnginePrimitiveRenderer* Renderer;             // +0x34
};
struct SButtonEntry { unsigned char _pad[0x54]; C2DVector Position; };   // 0x5c bytes
struct ButtonVector { SButtonEntry *Start, *Finish, *End; long size() const { return Finish - Start; } };
struct CPlayerGuiDef {
    unsigned char _pad00[0x214];
    C2DVector     AbilityOrigin;                // +0x214
    unsigned char _pad21c[0x7a4 - 0x21c];
    float         RowOffset;                    // +0x7a4
    float         _f7a8;
    float         RowStep;                      // +0x7ac
    long          DefaultGraphic;               // +0x7b0
    long          LockedOverlayGraphic;         // +0x7b4
    long          _l7b8;
    long          AlternateGraphic;             // +0x7bc
    unsigned char _pad7c0[0x7fc - 0x7c0];
    ButtonVector  AbilityButtons;               // +0x7fc
    ButtonVector  ButtonTypes;                  // +0x808
};
extern CPlayerGuiDef* __fastcall PeekGuiDef();  // 0x00438570
struct CHandleVector {
    CEnginePrimitiveHandle *Start, *Finish, *End;
    unsigned int size() const { return Finish - Start; }
};
enum EEngine2DLayer { LAYER_43 = 0x43, LAYER_44 = 0x44, LAYER_45 = 0x45 };

namespace NPlayerGui {
struct CDrawAbilityButtons {
    void*           PlayerGui;              // +0x04
    CPlayerManager* PlayerManager;          // +0x08
    unsigned char   _pad0c[0x1c - 0x0c];
    CHandleVector   Handles;                // +0x1c
    long            Columns;                // +0x28
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void GetButtonTypes(LongVector& types);   // slot 6
    unsigned char GetFadedAlpha(unsigned char alpha);   // 0x00643D5D
    void DrawButton(CEnginePrimitivePrepareInfo* info, C2DVector* pos, float scale, long button, long graphic, bool flag,
                    EEngine2DLayer layer);  // 0x00645099
    void DrawAbilityButton(CEnginePrimitivePrepareInfo* info, AbilityVector* abilities, long slot, long index, long row, float scale);
};

void CDrawAbilityButtons::DrawAbilityButton(CEnginePrimitivePrepareInfo* info, AbilityVector* abilities, long slot, long index,
                                            long row, float scale)
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetControlledCreature();
    if (!IS_THING_ALIVE(hero))
        return;
    LongVector types;
    GetButtonTypes(types);
    CTCInventoryAbilities* tc;
    if (hero->GetInterface(&tc, 0x6f) && tc) {
        long count = abilities->size();
        if (index < (count + 2) / Columns * Columns && (unsigned int)index < abilities->size()) {
            CPlayerGuiDef* def = PeekGuiDef();
            long button = def->AbilityButtons.size() + index + 7;
            C2DVector pos;
            GFAPositionToScreen(&pos, &def->AbilityOrigin);
            pos.X += def->ButtonTypes.Start[types.Start[slot]].Position.X;
            pos.Y = pos.Y + def->ButtonTypes.Start[types.Start[0]].Position.Y + row * def->RowStep + def->RowOffset;
            CDefPointer<CHeroAbilityDef> ability;
            if ((unsigned int)index < abilities->size())
                ability.Adopt(abilities->Start[index]);
            float buttonScale = scale;
            long graphic = def->DefaultGraphic;
            if (ability.Object) {
                graphic = ability.Object->Graphic;
                bool locked = false;
                if (!tc->IsAbleToUseAbility(ability.Object->Ability, true)) {
                    if (ability.Object->Ability != 4 || !hero->m_present.test(0x93))
                        locked = true;
                } else {
                    CTCSpecialAbilities* special;
                    if (hero->GetInterface(&special, 0xa7) && !special->GetAbilityAvailability(ability.Object->Ability))
                        locked = true;
                }
                if (locked) {
                    unsigned int handle = button + 0x14;
                    if (handle < Handles.size()) {
                        long layer = LAYER_43;
                        if (types.Start[slot] == 1)
                            layer = LAYER_44;
                        CEnginePrimitive2DSprite overlay(0, pos, layer, def->LockedOverlayGraphic, 0, 2, 0, 0.0f, 0.0f, GetFadedAlpha(0x80),
                                                         0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                        info->Renderer->AddPrimitive(&Handles.Start[handle], overlay, 0x8c0);
                    }
                }
                if (ability.Object->Ability == 4 && hero->m_present.test(0x93))
                    graphic = def->AlternateGraphic;
            } else
                buttonScale = scale * 0.33333334f;
            EEngine2DLayer layer = LAYER_44;
            if (types.Start[slot] == 1)
                layer = LAYER_45;
            DrawButton(info, &pos, buttonScale, button, graphic, false, layer);
        }
    }
}
}
