// CWorldMap::GetSoundThemesUsedInCurrentRegion(set<unsigned long>& themes) const -- collect the sound-theme switch IDs used by the current region: every game-map cell's SoundTheme across the region's maps goes into a local set, then each theme's SOUND_THEME definition (by class index) adds its switch ID to themes
// Leaf: ?GetSoundThemesUsedInCurrentRegion@CWorldMap@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FE7F0, 562 bytes (+ int3 padding), `ret 4`, an `and esp,-8` aligned frame
//   (Ego_r.pdb ?GetSoundThemesUsedInCurrentRegion@CWorldMap@@QBEXAAV?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@@Z).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the local set's header and nodes), CWorldMap::PeekMap (virtual slot 19,
//   +0x4C; 0x0051CE40), _Rb_tree<char>::insert_unique(const char&) 0x00514B50, _Rb_tree<char>::_M_erase 0x00510DA0
//   (the recursive call of the one inlined level), CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0,
//   CGameDefinitionManager::Get 0x0044C6B0 (`__fastcall`), CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390,
//   CDefinitionManager::GetDef<CSoundThemeDef> 0x00512E60, _Rb_tree<unsigned long>::insert_unique 0x00514C10, the
//   def's virtual slot 1 (destroy at zero references). "SOUND_THEME" 0x01235E60.
// Layout: CWorldMap Regions +0x2C (0x58-byte CRegion, map numbers vector<long> +0), CurrentRegion +0x9C; CMap GameMap
//   +0x1A78; CGameMap Cells +0x04 (7-byte cells, SoundTheme +5), SizeX +0x1C, SizeY +0x20 (landed 0x0063D8A0 model).
// Shape: the `and esp,-8` frame (ebp frame pointer, so sizeY lives in memory) appears only once STLport's pair has its
//   user constructors and set::insert converts the tree's pair (an 8-byte object built through a hidden pointer inside
//   the loop); the def object stays in edi across the insert and the release only with a same-TU noinline GetDef body;
//   the class-name string is a block-scoped named local (built before the classIndex push, destroyed before the test).
// Label correction: catalogued "CollectSoundThemeSwitchDefs"; the PDB name set holds GetSoundThemesUsedInCurrentRegion.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp*) { }
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> struct less { };
template <class _Tp> struct _Identity { };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                               // +0x00
    _Base_ptr _M_parent;                              // +0x04
    _Base_ptr _M_left;                                // +0x08
    _Base_ptr _M_right;                               // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                            // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _M_increment(_Base_ptr);
};
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    } else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value> _Self;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    _Rb_tree_base(const allocator<_Node>& __a) : _M_header(__a, (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;

    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(allocator<_Node>()), _M_node_count(0) { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
    private:
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    public:
    protected:
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    public:
    protected:
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    public:
    protected:
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    public:
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }

private:
    void _M_erase(_Link_type __x);
public:
    pair<iterator, bool> insert_unique(const _Value& __v);   // 0x00514B50 (char) / 0x00514C10 (unsigned long)
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{
    while (__x != 0) {
        _M_erase((_Link_type)__x->_M_right);
        _Link_type __y = (_Link_type)__x->_M_left;
        _Destroy(&__x->_M_value_field);
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    pair<iterator, bool> insert(const _Key& __x)
    {
        pair<typename _Rep_type::iterator, bool> __p = _M_t.insert_unique(__x);
        return pair<iterator, bool>(__p.first, __p.second);
    }
};
}

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CSoundThemeDef {
public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long          RefCount;                             // +0x04
    unsigned char _pad08[0x40 - 0x08];
    long          SwitchID;                             // +0x40
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    template <class T> bool GetDef(long index, CDefPointer<T>& def) const;                   // 0x00512E60
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
void* FindDef_GetDef_00512e60(long index);             // stand-in callee
#pragma auto_inline(off)
// Same-TU stand-in for CDefinitionManager::GetDef<CSoundThemeDef> 0x00512E60: with a visible body VC7.1 treats the def
// pointer as uncaptured and keeps its object in edi across the insert and the release.
template <> __declspec(noinline) bool CDefinitionManager::GetDef<CSoundThemeDef>(long index, CDefPointer<CSoundThemeDef>& def) const
{
    void* found = FindDef_GetDef_00512e60(index);
    if (!found)
        return false;
    def.Object = (CSoundThemeDef*)found;
    return true;
}
#pragma auto_inline(on)

class C2DCoordI { public: long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
class CGameMapCell {                                   // 7 bytes
public:
    unsigned char AtmosThemes[3];
    unsigned char AtmosBlends[2];
    signed char   SoundTheme;                           // +5
    unsigned char Music;
};
template <class T> struct CVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    const T& operator[](unsigned long n) const { return *(begin() + n); }
};
class CGameMap {
public:
    void*                 _vptr;                        // +0x00
    CVector<CGameMapCell> Cells;                        // +0x04
    void*                 Map;                          // +0x10
    void*                 World;                        // +0x14
    unsigned long*        WorldSeed;                    // +0x18
    long                  SizeX;                        // +0x1C
    long                  SizeY;                        // +0x20
    const CGameMapCell* PeekCell(const C2DCoordI& pos) const { return &Cells[(SizeY - pos.Y - 1) * SizeX + pos.X]; }
};
class CMap {
public:
    unsigned char _pad[0x1a78];
    CGameMap*     GameMap;                              // +0x1A78
};
class CRegion {                                        // 0x58 bytes
public:
    CVector<long> MapNumbers;                           // +0x00
    unsigned char _rest[0x58 - 0x0c];
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual const CMap& PeekMap(long index) const;      // slot 19 (+0x4C) 0x0051CE40
    unsigned char     _pad04[0x2c - 4];
    CVector<CRegion>  Regions;                          // +0x2C
    unsigned char     _pad38[0x9c - 0x38];
    long              CurrentRegion;                    // +0x9C
    void GetSoundThemesUsedInCurrentRegion(std::set<unsigned long>& themes) const;
};

void CWorldMap::GetSoundThemesUsedInCurrentRegion(std::set<unsigned long>& themes) const
{
    const CRegion* region = &Regions[CurrentRegion];
    std::set<char> soundThemes;
    for (unsigned long i = 0; i < region->MapNumbers.size(); ++i) {
        const CGameMap* gameMap = PeekMap(region->MapNumbers[i]).GameMap;
        long sizeX = gameMap->SizeX;
        long sizeY = gameMap->SizeY;
        for (long x = 0; x < sizeX; ++x)
            for (long y = 0; y < sizeY; ++y)
                soundThemes.insert(gameMap->PeekCell(C2DCoordI(x, y))->SoundTheme);
    }
    for (std::set<char>::iterator it = soundThemes.begin(); it != soundThemes.end(); ++it) {
        CDefPointer<CSoundThemeDef> def;
        long index;
        {
            CCharString className("SOUND_THEME");
            index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(className, *it);
        }
        if (index > 0) {
            if (CGameDefinitionManager::Get().GetDef(index, def))
                themes.insert(def->SwitchID);
        }
    }
}