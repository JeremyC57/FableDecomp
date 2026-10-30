// Grade: DIFFER(789v780) -- the creature search respelled from the labels/Ego_r.pdb instantiation (0x007D4CE0 CThingSearchTools::GetAllThings<CThingCreatureBase, list<CThingCreatureBase*>, CThingFilter_IsHuman>(const CThingFilter_IsHuman&, list&) const via CTCBase::GetThingSearchTools 0x00686D90): filter pushed first as retail; appearance lookups in the GetInterface form; residue: frame 0x2c v 0x28 (fov/camera slots swapped, list header kept in a frame word instead of edi), this in ebx v ebp
// CTCTavernGameCoinGolf::FadeOutTableOccupants(bool restore) -- while the table is in view, fade every live creature and every live coin (except the held one) by its distance from the camera so they do not block the shot, or restore them all to opaque
// Leaf: ?FadeOutTableOccupants@CTCTavernGameCoinGolf
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D46D0, 778 bytes (+ int3 padding), `ret 4`.
#include <math.h>
#include <stdlib.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCGraphicAppearance {                   // interface 0x5B
    void SetAlpha(unsigned char alpha);         // 0x004BFAC0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 dead
    bool HasTC(long t) const { return (m_presentBits[t >> 5] & (1UL << (t & 31))) != 0; }
    template <class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CThingCreatureBase : public CThing { };
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
#pragma pack(push, 1)
struct Coin {                                   // 0x3c bytes
    unsigned char _pad00[0x25];
    CBaseIntelligentPointer Thing;              // +0x25
    unsigned char _pad_0x2d[0x3c - 0x2d];
};
#pragma pack(pop)
template <class T> struct SNode {
    SNode* _M_next;
    SNode* _M_prev;
    T      _M_data;
};
template <class T> struct CList {               // STLport list
    SNode<T>* _M_node;
    CList()
    {
        SNode<T>* node = (SNode<T>*)malloc(sizeof(SNode<T>));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CList()
    {
        clear();
        free(_M_node);
    }
    void clear()
    {
        SNode<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SNode<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
struct CThingFilter_IsHuman { };
struct CThingSearchTools {
    template <class T, class TList, class TFilter> void GetAllThings(const TFilter& filter, TList& things) const;   // 0x007D4CE0 <CThingCreatureBase, list<CThingCreatureBase*>, CThingFilter_IsHuman>
};
class CThingTable : public CThing {
public:
};
struct CThingFlags {
    unsigned char _pad[0x34];
    unsigned long Flags34;                      // +0x34 bit 24: in view (descriptive)
};
inline const float& Min(const float& a, const float& b) { return b < a ? b : a; }
inline unsigned char FadeAlpha(float dist, float far, float near, float scale)
{
    if (!(dist < far))
        return 0xff;
    if (dist <= near)
        return 0;
    return (unsigned char)((dist - near) * scale);
}
struct CTCTavernGameCoinGolf {
    void* __vftable;
    CThingFlags* ParentThing;                   // +0x04
    unsigned char _pad08[0xfc - 0x08];
    Coin* CoinsStart;                           // +0xfc
    Coin* CoinsFinish;                          // +0x100
    unsigned char _pad104[4];
    Coin* Held;                                 // +0x108
    C3DVector GetCamera(float* fov);            // 0x007D3560 (descriptive)
    const CThingSearchTools* GetThingSearchTools() const;   // 0x00686D90 (CTCBase)
    void FadeOutTableOccupants(bool restore);
};

void CTCTavernGameCoinGolf::FadeOutTableOccupants(bool restore)
{
    float fov;
    C3DVector camera = GetCamera(&fov);
    if (!(ParentThing->Flags34 & 0x1000000))
        return;
    CList<CThingCreatureBase*> creatures;
    CThingFilter_IsHuman filter;
    GetThingSearchTools()->GetAllThings<CThingCreatureBase>(filter, creatures);
    for (SNode<CThingCreatureBase*>* node = creatures._M_node->_M_next; node != creatures._M_node; node = node->_M_next) {
        CThingCreatureBase* creature = node->_M_data;
        if (creature && !(creature->Flags91 & 1)) {
            const C3DVector& pos = creature->GetPosition();
            float dx = pos.X - camera.X;
            float dy = pos.Y - camera.Y;
            float dist = (float)sqrt(dx * dx + dy * dy);
            unsigned char alpha;
            if (restore || dist > 0.6f)
                alpha = 0xff;
            else if (dist < 0.4f)
                alpha = 0;
            else
                alpha = (unsigned char)((dist - 0.4f) * 1274.9998779296875f);
            CTCGraphicAppearance* appearance;
            if (creature->GetInterface(&appearance, 0x5b))
                appearance->SetAlpha(alpha);
        }
    }
    for (unsigned long i = 0; i < (unsigned long)(CoinsFinish - CoinsStart); ++i) {
        CThing* thing = CoinsStart[i].Thing.GetPItem();
        if (thing && !(thing->Flags91 & 1) && &CoinsStart[i] != Held) {
            C3DVector pos = thing->GetPosition();
            float top = pos.Z + 0.4f;
            float height = Min(top, camera.Z);
            float dx = pos.X - camera.X;
            float dy = pos.Y - camera.Y;
            float dz = height - camera.Z;
            float dist = (float)sqrt(dx * dx + dy * dy + dz * dz);
            unsigned char alpha;
            if (restore || dist > 0.3f)
                alpha = 0xff;
            else if (dist < 0.2f)
                alpha = 0;
            else
                alpha = (unsigned char)((dist - 0.2f) * 2549.999755859375f);
            CTCGraphicAppearance* appearance;
            if (thing->GetInterface(&appearance, 0x5b))
                appearance->SetAlpha(alpha);
        }
    }
}
