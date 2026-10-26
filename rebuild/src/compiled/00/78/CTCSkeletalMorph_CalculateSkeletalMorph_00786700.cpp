// CTCSkeletalMorph::CalculateSkeletalMorph() -- rebuild the skeletal morph: clear its parameters, then for every morph entry fetch the named morph target from the (lazily created) resource manager, wrap it in a new counted morph parameter, add it and set its value; finally drop the cached combined morph
// Leaf: CalculateSkeletalMorph
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00786700, 368 bytes (`ret`).
// Callees: C3DSkeletalMorph parameter clear 0x00A45410 / add 0x00A45470 / combined-morph reset 0x00A45330 (catalog labels are
//          propagated twins; names descriptive); CCountedPointer<CSkeletalMorphResourceManager>::Reset 0x004A9A80 on the global
//          0x013BAB10; CSkeletalMorphResourceManager::GetMorphTarget 0x006C3620; C3DSkeletalMorphParameter ctor 0x00A455A0 and
//          SetCurrentValue 0x00A45370; DeleteData<C3DSkeletalMorphParameter> 0x00786A30 (__fastcall deleter in the count block);
//          operator new 0x00BFEA1A, malloc 0x00BFEA0E (the map header), operator delete 0x00BFE9BC.
// Layout: retail CTCSkeletalMorph Morphs +0x10 (12-byte entries: name +4, value +8), PSkeletalMorph +0x1c (PDB +0x20, CArray
//          16 bytes in the PDB v 12 in release); CSkeletalMorphResourceManager vtable 0x01238F84, CachedMorphs header +4, count +8.
// Shape: the manager's map is its own member object (inline ctor), so the vtable store precedes the header malloc; the
//          parameter's count block is a `new SCountBlock(1, DeleteData, object)` expression; the counted-pointer dtor nulls both words.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
class CCharString { public: void* m_pData; };
struct SCountBlock {
    long  references;
    void (__fastcall* destroy)(void*);
    void* object;
    SCountBlock(long refs, void (__fastcall* fn)(void*), void* data) : references(refs), destroy(fn), object(data) {}
};
template <class T> class CCountedPointer { public:
    T*           object;
    SCountBlock* count;
    CCountedPointer(const CCountedPointer& other) : object(other.object), count(other.count)
    {
        if (count)
            ++count->references;
    }
    explicit CCountedPointer(T* value);
    ~CCountedPointer()
    {
        if (count) {
            if (--count->references == 0) {
                count->destroy(count->object);
                delete count;
            }
        }
        object = 0;
        count = 0;
    }
    T* operator->() const { return object; }
    void Reset(T* value);                           // 0x004A9A80 (twin instantiation)
};
class C3DSkeletalMorphTarget;
class C3DSkeletalMorphParameter { public:
    unsigned char _d[0x14];
    C3DSkeletalMorphParameter(C3DSkeletalMorphTarget* target);   // 0x00A455A0
    void SetCurrentValue(float value);              // 0x00A45370
};
void __fastcall DeleteData(void* data);             // 0x00786A30 (DeleteData<C3DSkeletalMorphParameter>)
template <> inline CCountedPointer<C3DSkeletalMorphParameter>::CCountedPointer(C3DSkeletalMorphParameter* value)
    : object(value)
{
    if (value)
        count = new SCountBlock(1, DeleteData, object);
    else
        count = 0;
}
struct CMorphMapNode {                              // STLport _Rb_tree_node_base header (0x1c-byte node)
    unsigned char  _M_color;
    CMorphMapNode* _M_parent;
    CMorphMapNode* _M_left;
    CMorphMapNode* _M_right;
};
struct CMorphMap {                                  // STLport map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> >
    CMorphMapNode* _M_header;
    long           _M_node_count;
    CMorphMap() : _M_header(0)
    {
        _M_header = (CMorphMapNode*)malloc(0x1c);
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};
class CSkeletalMorphResourceManager {
public:
    virtual void v00();
    CMorphMap CachedMorphs;                         // +0x04
    long      _0c;                                  // +0x0c
    const CCountedPointer<C3DSkeletalMorphTarget>& GetMorphTarget(const CCharString& name);   // 0x006C3620
};
extern CCountedPointer<CSkeletalMorphResourceManager> g_SkeletalMorphResourceManager;   // 0x013BAB10
class C3DSkeletalMorph { public:
    void ClearMorphParameters();                    // 0x00A45410
    void AddMorphParameter(const CCountedPointer<C3DSkeletalMorphParameter>& parameter);   // 0x00A45470
    void ResetCombinedMorph();                      // 0x00A45330
};
struct CSkeletalMorphEntry {                        // CTCSkeletalMorph::CSkeletalMorph, 12 bytes
    long        _00;
    CCharString Name;                               // +0x04
    float       Value;                              // +0x08
};
class CTCSkeletalMorph {
public:
    virtual void v00();
    unsigned char        _pad04[0x10 - 0x04];
    CSkeletalMorphEntry* m_morphsStart;             // +0x10 Morphs
    CSkeletalMorphEntry* m_morphsFinish;            // +0x14
    CSkeletalMorphEntry* m_morphsEnd;               // +0x18
    C3DSkeletalMorph*    PSkeletalMorph;            // +0x1c (CCountedPointer object)
    void CalculateSkeletalMorph();
};

void CTCSkeletalMorph::CalculateSkeletalMorph()
{
    PSkeletalMorph->ClearMorphParameters();
    for (CSkeletalMorphEntry* morph = m_morphsStart; morph != m_morphsFinish; ++morph) {
        if (!g_SkeletalMorphResourceManager.object)
            g_SkeletalMorphResourceManager.Reset(new CSkeletalMorphResourceManager);
        CCountedPointer<C3DSkeletalMorphTarget> target = g_SkeletalMorphResourceManager->GetMorphTarget(morph->Name);
        CCountedPointer<C3DSkeletalMorphParameter> parameter(new C3DSkeletalMorphParameter(target.object));
        PSkeletalMorph->AddMorphParameter(parameter);
        parameter->SetCurrentValue(morph->Value);
    }
    PSkeletalMorph->ResetCombinedMorph();
}