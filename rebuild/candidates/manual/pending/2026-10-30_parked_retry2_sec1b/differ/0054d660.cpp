// Grade: DIFFER(986v1001) 125 of 341 lines (was 976v1001, 131): the counted component pointer is the shared engine/CCountedPointer.h (CCountedPointer<NUISystem::CComponent>; the deleter 0x00428B10 is its DeleteData, reloc-bound as ?DeleteData@?$CCountedPointer@VCComponent@NUISystem@@@@SIXPAX@Z -- the parked local DestroyComponent spelling is retired) -- residue: VC7.1 reuses the function-wide zero register (ebp) as the new count block's register, so both null paths need no xor (retail: count block in ebx with a shared xor ebx,ebx; ebp stays the zero), which rotates ebx/ebp through the back() index math and the second arrow block; frame 0x1C v 0x20
// NUISystem::CFrontEndList::Initialise() -- the changing-state initialisation, then collect every item's name, reset the selection, and for the up / down arrow definitions in the def create the arrow component, keep it as a child (counted pointer) and in the arrow list, give it its scroll range and initialise it with this list as parent
// Leaf: ?Initialise@CFrontEndList@NUISystem@@
// Layout: CChangingStateComponent model of the landed NUISystem::CScrollableList::Initialise 0x00555EB0 (count block, CDefPointer,
//          slots 43/51/108); children vector<CCountedPointer<CComponent>> +0xb0, list members +0x15c..+0x184 descriptive.
#include <stddef.h>
#include <stdlib.h>
#include "engine/CCountedPointer.h"
inline void* operator new(size_t, void* where) { return where; }
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* block);
namespace NUISystem {
struct CComponent;
typedef ::CCountedPointer<CComponent> CComponentPointer;   // engine/CCountedPointer.h (DeleteData = 0x00428B10)
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                // slot 1 (+4)
};
struct CFrontEndListDef : CDefVirtual, CRefCounted {
    unsigned char _def08[0x130 - 0x08];
    long          DownArrowDef;            // +0x130
    long          UpArrowDef;              // +0x134
    long          UpArrowMinimum;          // +0x138
    long          DownArrowOffset;         // +0x13c
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CFrontEndListDef* m_p;
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CFrontEndListDef* operator->() const { return m_p; }
};
struct CListEntryName {
    unsigned char _b[0xc];
    CListEntryName(const CListEntryName& other);       // 0x0053CB9B
};
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct CComponent {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void Initialise();                                  // slot 43 (+0xAC)
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void SetParent(CComponent* parent);                 // slot 51 (+0xCC)
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual const CListEntryName& GetName() const;              // slot 82 (+0x148)
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual CDefPointer GetDef();                               // slot 108 (+0x1B0)
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void v133();
    virtual void v134();
    virtual void v135();
    virtual void v136();
    virtual void v137();
    virtual void v138();
    virtual void v139();
    virtual void v140();
    virtual void v141();
    virtual void v142();
    virtual void v143();
    virtual void v144();
    virtual void v145();
    virtual void v146();
    virtual void v147();
    virtual void v148();
    virtual void SetScrollMinimum(long value);                  // slot 149 (+0x254)
    virtual void SetScrollMaximum(long value);                  // slot 150 (+0x258)
};
struct CNameVector {
    CListEntryName* _M_start; CListEntryName* _M_finish; CListEntryName* _M_end_of_storage;
    void _M_insert_overflow(CListEntryName* pos, const CListEntryName& x, const __false_type&, unsigned int n, bool atend);   // 0x0053CEF6
    void push_back(const CListEntryName& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CListEntryName(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CChildVector {
    CComponentPointer* _M_start; CComponentPointer* _M_finish; CComponentPointer* _M_end_of_storage;
    void _M_insert_overflow(CComponentPointer* pos, const CComponentPointer& x, const __false_type&, unsigned int n, bool atend);   // 0x005359D0
    void push_back(const CComponentPointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CComponentPointer(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    unsigned int size() const { return _M_finish - _M_start; }
    CComponentPointer& back() { return _M_start[size() - 1]; }
};
struct CArrowVector {
    CComponent** _M_start; CComponent** _M_finish; CComponent** _M_end_of_storage;
    void _M_insert_overflow(CComponent** pos, CComponent* const& x, const __true_type&, unsigned int n, bool atend);   // 0x00485DBC
    void push_back(CComponent* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CComponent*(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
#pragma auto_inline(off)
#define GROW_BODY(T)                                                                 \
{                                                                                    \
    T* start = _M_start;                                                             \
    T* finish = _M_finish;                                                           \
    unsigned int size = finish - start;                                              \
    unsigned int len = size + (size > n ? size : n);                                 \
    T* buffer = (T*)malloc(len * sizeof(T));                                         \
    T* cur = buffer;                                                                 \
    for (T* p = start; p != pos; ++p, ++cur)                                         \
        new (cur) T(*p);                                                             \
    for (unsigned int i = 0; i < n; ++i, ++cur)                                      \
        new (cur) T(x);                                                              \
    if (!atend)                                                                      \
        for (T* p = pos; p != finish; ++p, ++cur)                                    \
            new (cur) T(*p);                                                         \
    if (start)                                                                       \
        free(start);                                                                 \
    _M_start = buffer;                                                               \
    _M_finish = cur;                                                                 \
    _M_end_of_storage = buffer + len;                                                \
}
__declspec(noinline) void CNameVector::_M_insert_overflow(CListEntryName* pos, const CListEntryName& x, const __false_type&, unsigned int n, bool atend)
GROW_BODY(CListEntryName)
__declspec(noinline) void CChildVector::_M_insert_overflow(CComponentPointer* pos, const CComponentPointer& x, const __false_type&, unsigned int n, bool atend)
GROW_BODY(CComponentPointer)
typedef CComponent* CComponentRaw;
__declspec(noinline) void CArrowVector::_M_insert_overflow(CComponent** pos, CComponent* const& x, const __true_type&, unsigned int n, bool atend)
GROW_BODY(CComponentRaw)
#pragma auto_inline(on)
struct CManager {
    CComponent* CreateComponent(long definition);       // 0x0041D21B
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
struct CChangingStateComponent : CComponent {
    unsigned char _pad004[0x2c];
    long          DefaultSelection;                     // +0x30
    unsigned char _pad034[0x7c];
    CChildVector  Children;                             // +0xb0
    unsigned char _pad0bc[0x15c - 0xbc];
    void Initialise();                                  // 0x0052C730
};
struct CFrontEndItem { virtual void v00(); };
struct CFrontEndList : CChangingStateComponent {
    long          Scroll;                               // +0x15c
    long          Selection;                            // +0x160
    CComponent**  Items;                                // +0x164
    CComponent**  ItemsEnd;                             // +0x168
    CComponent**  ItemsCapacity;
    CArrowVector  Arrows;                               // +0x170
    CNameVector   Names;                                // +0x17c
    unsigned int ItemCount() const { return ItemsEnd - Items; }
    void Initialise();
};

void CFrontEndList::Initialise()
{
    CChangingStateComponent::Initialise();
    for (unsigned int i = 0; i < ItemCount(); ++i)
        Names.push_back(Items[i]->GetName());
    Selection = DefaultSelection;
    Scroll = 0;
    CDefPointer def = GetDef();
    CComponentPointer up;
    if (def->UpArrowDef) {
        Children.push_back(CComponentPointer(CFrontEndManager::GetInstance()->CreateComponent(def->UpArrowDef)));
        Arrows.push_back(Children.back().operator->());
        up = Children.back();
        up->SetScrollMinimum(def->UpArrowMinimum);
        up->SetScrollMaximum(ItemCount());
        up->Initialise();
        Children.back()->SetParent(this);
    }
    if (def->DownArrowDef) {
        Children.push_back(CComponentPointer(CFrontEndManager::GetInstance()->CreateComponent(def->DownArrowDef)));
        Arrows.push_back(Children.back().operator->());
        CComponentPointer down;
        down = Children.back();
        down->SetScrollMinimum(0);
        down->SetScrollMaximum(ItemCount() - def->DownArrowOffset - 1);
        down->Initialise();
        Children.back()->SetParent(this);
    }
}

}
