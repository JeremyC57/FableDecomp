// CTCHeroReceiveItems::PrepareToShowItems(const C3DVector& position) -- with items queued and none on display, start the item-taking area at the item's screen position (or the centre of the visible TV area), reset the item orientation and clear the current item
// Leaf: PrepareToShowItems
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CA670, 397 bytes (+ int3 padding), `ret 4`.
//   Ego_r.pdb: `?PrepareToShowItems@CTCHeroReceiveItems@@AAEXABVC3DVector@@@Z`.
// Callees: the display engine's renderer (g_pMainGameComponent 0x013B86A0 -> +0x28 -> +0x2c) virtual slot 13 (+0x34,
//   world position -> screen position, bool), CDisplayManager::GetAVisibleTelevisionScreenArea @ 0x009BECB0 (C2DBoxF
//   by value, on the display manager 0x013B8390), CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10 /
//   0x00A01B50 / 0x00A01B90 / 0x00A01C10 (CIntelligentPointer<CThingObject> vtable 0x0125A858).
// Data: the file-static item "up" vector @ 0x01383A5C (three references, all in this TU); 0x0122F59C is 0.5f.
// Shape: the area box and the orientation are written in place through inline Set members (`lea eax,[esi+3Ch]` in
//   each arm, `mov ecx,eax ; lea ecx,[eax+0Ch]` for the right-handed set); an assignment from a constructed temporary
//   copies through the frame instead. The forward C3DVector(0,0,-1) temporary is stored and re-read per component
//   (bound to the Set's const reference). The current item is cleared by assigning a CIntelligentPointer temporary
//   (operator= as GetPItem + SetPItem; no user dtor on the template, so the base dtor is called directly).
// Layout: retail CTCHeroReceiveItems as the landed ctor 0x007CA830 (Items +0x0c, PCurrentItem +0x1c, flags +0x25/+0x26,
//   AmountThroughForAreaResize +0x34, three C2DBoxF +0x3c/+0x4c/+0x5c, two CRightHandedSet +0x6c/+0x84).
#include <stddef.h>
class CThing;
class CThingObject;
class CBaseObject;

struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    void Set(float tlx, float tly, float brx, float bry) { TLX = tlx; TLY = tly; BRX = brx; BRY = bry; }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet() { }
    void Set(const C3DVector& up, const C3DVector& forward) { Up = up; Forward = forward; }
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0125A858 (CThingObject)
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
#pragma pack(pop)
struct SItemNode { SItemNode* _M_next; SItemNode* _M_prev; };
struct CItemList {                                      // STLport list (retail: the node pointer only)
    SItemNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
struct CRetailVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; };

struct CRenderer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual bool GetScreenPosition(const C3DVector& position, C2DVector* out);   // slot 13 (+0x34)
};
struct CDisplayEngine { unsigned char _pad[0x2c]; CRenderer* Renderer; };       // +0x2c
struct CMainGameComponent { unsigned char _pad[0x28]; CDisplayEngine* DisplayEngine; };   // +0x28
struct CDisplayManager {
    C2DBoxF GetAVisibleTelevisionScreenArea();          // 0x009BECB0
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
extern CDisplayManager* g_pDisplayManager;              // 0x013B8390
extern C3DVector g_ItemUp;                              // 0x01383A5C

struct CTCHeroReceiveItems : CTCBase {
    CItemList Items;                                    // +0x0c
    CRetailVector PickedUpItems;                        // +0x10
    CIntelligentPointer<CThingObject> PCurrentItem;     // +0x1c
    bool PersistCurrentItem;                            // +0x24
    bool ReceivingItems;                                // +0x25
    bool DisplayingItems;                               // +0x26
    bool ForceClear;                                    // +0x27
    bool SendMessage;                                   // +0x28
    CIntelligentPointer<CThing> PContainer;             // +0x29
    float AmountThroughForAreaResize;                   // +0x34
    long FullSizeTimer;                                 // +0x38
    C2DBoxF InitialAreaItemTook;                        // +0x3c
    C2DBoxF OldAreaItemTaking;                          // +0x4c
    C2DBoxF CurrentAreaItemTaking;                      // +0x5c
    CRightHandedSet OldItemRHSet;                       // +0x6c
    CRightHandedSet CurrentItemRHSet;                   // +0x84
    void PrepareToShowItems(const C3DVector& position);
};

void CTCHeroReceiveItems::PrepareToShowItems(const C3DVector& position)
{
    if (Items.empty())
        return;
    if (DisplayingItems)
        return;
    AmountThroughForAreaResize = 0.0f;
    C2DVector screen;
    if (g_pMainGameComponent->DisplayEngine->Renderer->GetScreenPosition(position, &screen)) {
        InitialAreaItemTook.Set(screen.X, screen.Y, screen.X, screen.Y);
    } else {
        C2DBoxF area = g_pDisplayManager->GetAVisibleTelevisionScreenArea();
        float x = (area.BRX - area.TLX) * 0.5f + area.TLX;
        float y = (area.BRY - area.TLY) * 0.5f + area.TLY;
        InitialAreaItemTook.Set(x, y, x, y);
    }
    CurrentAreaItemTaking = InitialAreaItemTook;
    OldAreaItemTaking = InitialAreaItemTook;
    ReceivingItems = true;
    OldItemRHSet.Set(g_ItemUp, C3DVector(0.0f, 0.0f, -1.0f));
    CurrentItemRHSet = OldItemRHSet;
    PCurrentItem = CIntelligentPointer<CThingObject>(0);
}
