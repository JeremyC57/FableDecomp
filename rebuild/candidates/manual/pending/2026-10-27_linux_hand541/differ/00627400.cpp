// CTCQuickAccessMenu::ConstructExpressionCategoryList(CWideString& name, NUISystem::CList* list, std::vector<std::pair<long, long> >& items, NUISystem::CComponent* viewport) -- add one expression category entry to the quick access menu list: the name (plus the X string and "(count)" when there are items) as a UI_QAM_CATEGORY_TEMPLATE menu entry; with no items give its ARROW child a deletion and make the entry unavailable, else attach a UI_QAM_SUBLIST child sized to the items, create the UI_QAM_ENTRIES component and build the item lists
// Grade: DIFFER(750v742) -- everything matches but the CDeletion temporary: ours stores the _List_base null head (mov [esp+0x2c],0) before the allocate, retail drops that dead store and builds the by-value CDeletion argument in place (push ecx; lea edx; mov ecx,esp) -- lane gap: local STL container dead store kept
// Leaf: ?ConstructExpressionCategoryList@CTCQuickAccessMenu@@AAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCComponent@4@@Z
// Flags: /Zc:wchar_t
// Retail: Fable.exe (TLC, VC7.1) @ 0x00627400, 742 bytes, `ret 10h`.
extern "C" void* __cdecl malloc(unsigned int size);                             // 0x00BFEA0E
void* __cdecl operator new(unsigned int size);                                  // 0x00BFEA1A
void __cdecl operator delete(void* block);                                      // 0x00BFE9BC

namespace std {
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    bool empty() const { return this->_M_start == this->_M_finish; }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _List_base(const _Alloc& __a) : _M_node(allocator<_Node>(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base();                                                              // 0x0042ABCA
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
};
}

class CCharString {
public:
    CCharString(const char* text, long length = -1);                           // 0x0099EBF0
    ~CCharString();                                                             // 0x0099EAE0
private:
    void* PStringData;
};
class CWideString {
public:
    CWideString();                                                              // 0x0099AED0
    CWideString(const CWideString& other);                                      // 0x0099B720
    ~CWideString();                                                             // 0x0099B510
    const CWideString& operator+=(const CWideString& other);                    // 0x0099B8D0
    void __cdecl Sprintf(const wchar_t* format, ...);                           // 0x0099BA70
private:
    void* PStringData;
};
CWideString __fastcall GetXString();                                            // 0x005CC772

// Descriptive: retail expands this by-value helper inline (default-construct, Sprintf "(%d)", copy out, destroy);
// no out-of-line body or PDB name exists for it.
inline CWideString FormatBracketedCount(long count)
{
    CWideString text;
    text.Sprintf(L"(%d)", count);
    return text;
}

class CCPPointerInfo {
public:
    unsigned long RefCount;                                                     // +0x00
    void (__fastcall* DeleteFunc)(void* data);                                  // +0x04
    void* Data;                                                                 // +0x08
    CCPPointerInfo(void* data, void (__fastcall* deleteFunc)(void*)) : RefCount(1), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> class CCountedPointer {
public:
    T* Data;                                                                    // +0x00
    CCPPointerInfo* Info;                                                       // +0x04
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(Data, DeleteData);
        else
            Info = 0;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    static void __fastcall DeleteData(void* data);                              // 0x00428B10
};

namespace NUISystem {
enum EDeletionMethod { };
class CComponent;
class CDeletion {
public:
    EDeletionMethod Method;                                                     // +0x00
    std::list<CComponent*> AssociatedParents;                                   // +0x04
    CDeletion() { }
    CDeletion(const CDeletion& other);                                          // 0x0042CD84
};
class CComponent {
public:
#define S(n) virtual void Slot##n();
    S(00) S(01) S(02)
    virtual CComponent* FindChild(CCharString name);                           // slot 3 (+0x0C)
    S(04) S(05) S(06) S(07) S(08) S(09) S(10) S(11) S(12) S(13) S(14) S(15) S(16) S(17) S(18) S(19)
    S(20) S(21) S(22) S(23) S(24) S(25) S(26) S(27) S(28) S(29) S(30) S(31) S(32) S(33) S(34) S(35) S(36) S(37) S(38) S(39)
    S(40) S(41) S(42) S(43) S(44) S(45) S(46) S(47) S(48) S(49) S(50) S(51) S(52) S(53) S(54) S(55) S(56) S(57) S(58)
    virtual void AddChild(CCountedPointer<CComponent>& child);                  // slot 59 (+0xEC)
    S(60) S(61)
    virtual void SetDeletion(CDeletion deletion);                               // slot 62 (+0xF8)
    S(63) S(64) S(65) S(66) S(67) S(68) S(69) S(70) S(71) S(72) S(73) S(74) S(75) S(76) S(77) S(78) S(79)
    S(80) S(81) S(82) S(83) S(84) S(85) S(86) S(87) S(88) S(89) S(90) S(91)
    virtual void MakeUnavailable();                                             // slot 92 (+0x170)
};
class CChangingStateComponent : public CComponent { };
class CList : public CChangingStateComponent {
public:
    S(93) S(94) S(95) S(96) S(97) S(98) S(99)
    S(100) S(101) S(102) S(103) S(104) S(105) S(106) S(107) S(108) S(109) S(110) S(111) S(112) S(113) S(114) S(115) S(116) S(117) S(118) S(119)
    S(120) S(121) S(122) S(123) S(124) S(125) S(126) S(127) S(128) S(129) S(130) S(131) S(132) S(133) S(134) S(135) S(136) S(137) S(138) S(139)
    S(140) S(141) S(142) S(143) S(144) S(145) S(146) S(147) S(148) S(149) S(150) S(151)
    virtual void AddScrollingChildAndSetOffsets(CCountedPointer<CComponent>& child);   // slot 152 (+0x260)
    S(153) S(154) S(155)
    virtual void InitialiseOffsets(unsigned long count);                        // slot 156 (+0x270)
    S(157)
    virtual void SetSelectedChild(long index);                                  // slot 158 (+0x278)
#undef S
};
class CManager {
public:
    CComponent* CreateComponent(const CCharString& name, bool b);               // 0x0041DB1D
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, const CCharString f);   // 0x00428494 (const by-value breaks the arg back-reference: PDB MV5@MV5@@Z)
    static CManager* __fastcall GetInstance();                                  // 0x0041E5F2
};
}

class CTCQuickAccessMenu {
private:
    void ContructItemLists(std::vector<std::pair<long, long> >& items, NUISystem::CList* list,
                           NUISystem::CComponent* viewport, CCountedPointer<NUISystem::CComponent>& entries);   // 0x00626750
    void ConstructExpressionCategoryList(CWideString& name, NUISystem::CList* list,
                                         std::vector<std::pair<long, long> >& items, NUISystem::CComponent* viewport);
};

void CTCQuickAccessMenu::ConstructExpressionCategoryList(CWideString& name, NUISystem::CList* list,
                                                         std::vector<std::pair<long, long> >& items,
                                                         NUISystem::CComponent* viewport)
{
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CWideString text(name);
    if (!items.empty()) {
        text += GetXString();
        text += FormatBracketedCount(items.size());
    }
    CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(text, 0.0f, 1.0f, "UI_QAM_CATEGORY_TEMPLATE",
                                                                                    0.0f, "", 0.0f, "PC_BUTTON"));
    list->AddScrollingChildAndSetOffsets(entry);
    if (items.empty()) {
        NUISystem::CComponent* data = entry.Data;
        NUISystem::CComponent* arrow = data->FindChild("ARROW");
        if (arrow) {
            NUISystem::CDeletion deletion;
            deletion.Method = (NUISystem::EDeletionMethod)1;
            arrow->SetDeletion(deletion);
        }
        entry.Data->MakeUnavailable();
    } else {
        CCountedPointer<NUISystem::CComponent> subList(manager->CreateComponent("UI_QAM_SUBLIST", false));
        NUISystem::CList* sub = (NUISystem::CList*)subList.Data;
        entry.Data->AddChild(subList);
        sub->InitialiseOffsets(items.size());
        CCountedPointer<NUISystem::CComponent> entries(NUISystem::CManager::GetInstance()->CreateComponent("UI_QAM_ENTRIES", false));
        ((NUISystem::CList*)entries.Data)->SetSelectedChild(0);
        ContructItemLists(items, sub, viewport, entries);
    }
}
