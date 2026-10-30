// NUISystem::CTable::GetDataFromDef() -- read the table def: size, the two separation lists and one sprite component per def entry
// Grade: DIFFER(431v431) -- frame only: retail `sub esp,10h` (frame word F3 unused), VC7.1 here `sub esp,0Ch`; every instruction else identical. 2 grades.
// Leaf: ?GetDataFromDef@CTable@NUISystem@@
// Retail: Fable.exe @ 0x00551340, 431 bytes (`ret`), /O2. Catalog row: only the first 428 bytes (it stops inside the
// final `add esp,10h`), so no compiled body can equal the catalog oracle; graded against the full body.
// Body: the table def by value (own slot 108, +0x1B0; CDefPointer released at the end through the ref-count base);
// Size = (def +0x5C, def +0x58); PHorizontalSeparations / PVerticalSeparations = new vector<unsigned long> copies of the
// def's lists (+0x7C / +0x88; copy ctor 0x00535470, operator new 0x00BFEA1A); then for every (sprite, def index) entry of
// the def's map (+0x64; inline STLport iterator increment) the CManager singleton (0x0041E5F2) creates the component
// (CreateComponent(long) 0x0041D21B), Sprites[sprite] (map operator[] 0x00554030 at +0x15C) takes a fresh counted
// pointer to it (count block {1, DeleteData 0x00428B10, object}; old block released, operator delete 0x00BFE9BC) and
// the sprite is initialised (component slot 43, +0xAC).
// Layout: Ego_r.pdb CTable (Sprites +0x15C, PHorizontalSeparations +0x168, PVerticalSeparations +0x16C at the retail
// offsets, 0x30..0x34 below the PDB's) and CComponent::Size (+0xCC).
void* __cdecl operator new(unsigned int size);                 // 0x00BFEA1A
void __cdecl operator delete(void* p);                         // 0x00BFE9BC
void __fastcall DeleteData(void* object);                      // 0x00428B10
struct CCPPointerInfo {
    long  RefCount;                                            // +0
    void (__fastcall* DeleteFunc)(void*);                      // +4
    void* Data;                                                // +8
    CCPPointerInfo(void* data) : RefCount(1), DeleteFunc(DeleteData), Data(data) { }
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data) : Data(data) { Info = data ? new CCPPointerInfo(data) : 0; }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (Info != other.Info) {
            if (Info) {
                if (--Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = other.Data;
            Info = other.Info;
            if (Info)
                ++Info->RefCount;
        }
        return *this;
    }
    T* operator->() const { return Data; }
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                    // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
namespace std {
template <class _Dummy> class _Rb_global {
public:
    static struct _Rb_tree_node_base* _M_increment(struct _Rb_tree_node_base* __x);
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Dummy>
inline _Rb_tree_node_base* _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    } else {
        _Rb_tree_node_base* __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}
template <class K, class V> struct pair { K first; V second; };
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
    void _M_increment() { _M_node = _Rb_global<bool>::_M_increment(_M_node); }
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Base_ptr __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    const _Value* operator->() const { return &((_Link_type)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
};
template <class K, class V> struct map {
    typedef pair<const K, V> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    long                _M_key_compare;
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    V& operator[](const K& key);
};
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector(const vector& other);                               // 0x00535470 for unsigned long
};
}
namespace NUISystem {
enum ETableSprites { TABLE_SPRITE_NONE };
struct C2DVector { float X, Y; };
class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042();
    virtual void Initialise();                                 // slot 43 (+0xac)
};
struct CTableDef : CDefBase {
    unsigned char                      _pad08[0x50];
    float                              Height;                 // +0x58 (descriptive)
    float                              Width;                  // +0x5c (descriptive)
    unsigned char                      _pad60[0x4];
    std::map<ETableSprites, long>      SpriteDefs;             // +0x64 (sprite -> component def index, descriptive)
    unsigned char                      _pad70[0xc];
    std::vector<unsigned long>         HorizontalSeparations;  // +0x7c
    std::vector<unsigned long>         VerticalSeparations;    // +0x88
};
struct CTableDefPointer {
    CTableDef* m_p;
    ~CTableDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CTableDef* operator->() const { return m_p; }
};
class CManager {
public:
    static CManager* __fastcall GetInstance();                 // 0x0041E5F2
    CComponent* CreateComponent(long definition);              // 0x0041D21B
};
typedef std::map<ETableSprites, CCountedPointer<CComponent> > CSpriteMap;
// CSpriteMap::operator[] is the out-of-line STLport instance 0x00554030
class CTable {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual CTableDefPointer GetDef();                         // slot 108 (+0x1b0)
    void GetDataFromDef();
    unsigned char               _pad004[0xc8];
    C2DVector                   Size;                          // +0xcc (CComponent)
    unsigned char               _pad0d4[0x88];
    CSpriteMap                  Sprites;                       // +0x15c
    std::vector<unsigned long>* PHorizontalSeparations;        // +0x168
    std::vector<unsigned long>* PVerticalSeparations;          // +0x16c
};
void CTable::GetDataFromDef()
{
    CTableDefPointer def = GetDef();
    Size.X = def->Width;
    Size.Y = def->Height;
    PHorizontalSeparations = new std::vector<unsigned long>(def->HorizontalSeparations);
    PVerticalSeparations = new std::vector<unsigned long>(def->VerticalSeparations);
    for (std::map<ETableSprites, long>::const_iterator it = def->SpriteDefs.begin(); it != def->SpriteDefs.end(); ++it) {
        Sprites[it->first] = CCountedPointer<CComponent>(CManager::GetInstance()->CreateComponent(it->second));
        Sprites[it->first]->Initialise();
    }
}
}
