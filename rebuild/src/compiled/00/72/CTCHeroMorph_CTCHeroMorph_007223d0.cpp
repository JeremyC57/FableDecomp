// CTCHeroMorph::~CTCHeroMorph() -- destroy the custom-tattoo map, the tattoo def pointers, both particle-emitter lists and the mesh morph set in reverse order, then ~CTCBase
// Leaf: ??1CTCHeroMorph
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007223D0, length 204 bytes. Called only by the deleting destructor 0x007223B0
// (slot 0 of CTCHeroMorph's vtable 0x01265CF4); the catalog label "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip"
// is propagated.
// Callees: the map<CCharString, pair<long, CDateAndTime>> erase @ 0x0071F8F0 (landed CRetailTree_erase0071f8f0), the 4-byte
//          element range destructor @ 0x005B26E9 (__fastcall first/last + allocator; landed under the name
//          _Destroy_range_CFlash, kept here so the address has one symbol -- the Tattoos elements are CDefPointers), the list
//          clear @ 0x004B8050 (landed CRetailList_004b8050::clear_CRetailList_004b8050, x2), free @ 0x00BFEA14 (x4),
//          operator delete @ 0x00BFE9BC (the count block), CTCBase::~CTCBase @ 0x00686830.
// Layout: Ego_r.pdb CTCHeroMorph at retail offsets: PMeshMorphSet +0x0c (CCountedPointer), ParticleEmitters +0x14 and
// IdleParticleEmitters +0x18 (one-word STLport lists), Tattoos +0x1c (12-byte vector), CustomToBaseTattooMap +0x28.
// Shape: members die in reverse order with every container destructor inline (the zero kept in ebx); the counted pointer's
// destructor releases the count block (destroy member-function pointer, then delete) and nulls both words.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

class CThing;
class CTCBase { public:
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    virtual ~CTCBase();                                     // 0x00686830
};

struct CCountedItem { };
class CCPPointerInfo { public:                                     // the shared count block
    long m_count;                                           // +0x00
    void (CCountedItem::*m_pfnDestroy)();                   // +0x04 (called with the item as `this`)
    CCountedItem* m_pItem;                                  // +0x08
};
template<class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->m_count == 0) {
                (Info->m_pItem->*Info->m_pfnDestroy)();
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CNodeValue_004b8050 { void* m_data[2]; };            // CIntelligentPointer<CThing>
struct CRetailList_004b8050 {                               // STLport list<CIntelligentPointer<CThing>> (one word)
    struct SNode {
        SNode* m_next;
        SNode* m_prev;
        CNodeValue_004b8050 m_value;
    };
    SNode* m_head;
    void clear_CRetailList_004b8050();                      // 0x004B8050
    ~CRetailList_004b8050()
    {
        clear_CRetailList_004b8050();
        if (m_head)
            free(m_head);
    }
};

struct CFlash;                                              // the landed element name of 0x005B26E9
struct Alloc {};
void __fastcall _Destroy_range_CFlash(CFlash* first, CFlash* last, Alloc* al);   // 0x005B26E9

struct CThingBaseDefPointerVector {                         // vector<CDefPointer<const CThingBaseDef>>
    CFlash* _M_start;
    CFlash* _M_finish;
    CFlash* _M_end_of_storage;
    ~CThingBaseDefPointerVector()
    {
        Alloc allocator;
        _Destroy_range_CFlash(_M_start, _M_finish, &allocator);
        if (_M_start)
            free(_M_start);
    }
};

struct CRetailTree_erase0071f8f0 {                          // map<CCharString, pair<long, CDateAndTime>>
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node* head;
    unsigned long size;
    unsigned long _comp;
    void Erase(node* root);                                 // 0x0071F8F0
    ~CRetailTree_erase0071f8f0()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
};

struct CMeshMorphSet;

class CTCHeroMorph : public CTCBase { public:                             // vtable 0x01265CF4
    CCountedPointer<CMeshMorphSet> PMeshMorphSet;           // +0x0c
    CRetailList_004b8050       ParticleEmitters;            // +0x14
    CRetailList_004b8050       IdleParticleEmitters;        // +0x18
    CThingBaseDefPointerVector Tattoos;                     // +0x1c
    CRetailTree_erase0071f8f0  CustomToBaseTattooMap;       // +0x28
    long  FrameTattoosChanged;                              // +0x34
    long  FrameHeroBecameIdle;                              // +0x38
    bool  IdleParticlesAdded;                               // +0x3c
    bool  Changed;                                          // +0x3d
    float Strength, Berserk, Will, Skill, Age, Morality, Fatness, Tan;   // +0x40 .. +0x5c
    bool  Teenager;                                         // +0x60
    virtual ~CTCHeroMorph();
};

CTCHeroMorph::~CTCHeroMorph()
{
}