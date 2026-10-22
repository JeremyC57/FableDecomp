// C3DAnimationBuilder::AssignAnimation(CBoneInfo*, const CCountedPointer<CBoneInfo>&, bool) -- point a bone (and recursively its children) at its animation source bone, returning how many bones were assigned
// Grade: DIFFER(605v592) -- register allocation: retail frees edi for the child and reloads source into esi after the prefix loop (index in esi); ours keeps source in edi and spills the index (advisor c11: accessor ternaries + loop-local source pointer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB1740, length 592 bytes (`ret 0Ch`, recursive).
// Callees: CCharString::ToLower @ 0x0099ED10 (by value), strncmp @ 0x00BFEAF8, ~CCharString @ 0x0099EAE0,
//          CBoneInfo::FindChildIndex(const CCharString&) @ 0x00AB15F0, operator delete @ 0x00BFE9BC, itself.
// Shape: AnimSource = source (inline counted-pointer assignment: the source Object is read before its Count, the old
// count block released when different). With `extend` set and both bones originally single-child, the bone's only
// child is matched against the added-bone prefixes (strncmp over each prefix's length on the lower-cased name): an
// added bone that maps better onto the source itself than onto the source's first child is flagged as an extension
// (parent/child flags set, its AnimSource released) and counted; otherwise the child is mapped onto the source's first
// child. Without `extend`, every child found by name under the source is assigned recursively and counted.
// Layout (Ego_r.pdb CBoneInfo 0x28, retail 0x24 with the 12-byte vector): Name +0, Index +4, OriginalNoChildren +8,
// Children +0xC (CCountedPointer<CBoneInfo> elements), AnimSource +0x18, ExtensionChild +0x20, ExtensionParent +0x21;
// C3DAnimationBuilder AddedBonePrefixes +4 (vector<CCharString>).
// Leaf: AssignAnimation
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);

struct CCharStringData { char* Text; long Length; };
struct CCharString {
    CCharStringData* Data;
    ~CCharString();                                // 0x0099EAE0
    CCharString ToLower() const;                   // 0x0099ED10
    const char* c_str() const { return Data == 0 ? "" : Data->Text; }
    long Length() const { return Data == 0 ? 0 : Data->Length; }
};
struct SCountBlock { long references; void (__fastcall* destroy)(void*); void* object; };
template<class T> struct CCountedPointer {
    T* Object; SCountBlock* Count;
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        T* p = o.Object; SCountBlock* c = o.Count;
        if (Count != c) {
            if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } }
            Object = p; Count = c; if (c) ++c->references;
        }
        return *this;
    }
    __forceinline void Release()
    {
        if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } }
        Count = 0;
        Object = 0;
    }
    T* operator->() const { return Object; }
};
template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    bool empty() const { return _M_start == _M_finish; }
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};

struct C3DAnimationBuilder {
    struct CBoneInfo {
        CCharString Name;                                   // +0x00
        unsigned long Index;                                // +0x04
        unsigned long OriginalNoChildren;                   // +0x08
        CArray<CCountedPointer<CBoneInfo> > Children;       // +0x0c
        CCountedPointer<CBoneInfo> AnimSource;              // +0x18
        bool ExtensionChild;                                // +0x20
        bool ExtensionParent;                               // +0x21
        long FindChildIndex(const CCharString& name);       // 0x00AB15F0
    };
    long _padding;                                          // +0x00
    CArray<CCharString> AddedBonePrefixes;                  // +0x04
    unsigned long AssignAnimation(CBoneInfo* bone, const CCountedPointer<CBoneInfo>& source, bool extend);
};

unsigned long C3DAnimationBuilder::AssignAnimation(CBoneInfo* bone, const CCountedPointer<CBoneInfo>& source, bool extend)
{
    unsigned long assigned = 0;
    bone->AnimSource = source;
    if (extend && bone->OriginalNoChildren == 1 && source->OriginalNoChildren == 1) {
        if (bone->Children.empty() || source->Children.empty())
            return 1;
        CBoneInfo* child = bone->Children[0].Object;
        bool added = false;
        CCharString name = child->Name.ToLower();
        for (unsigned long i = 0; i < AddedBonePrefixes.size(); ++i) {
            const CCharString& prefix = AddedBonePrefixes[i];
            if (strncmp(name.c_str(), prefix.c_str(), prefix.Length()) == 0)
                added = true;
        }
        if (added) {
            unsigned long viaChild = AssignAnimation(child, source->Children[0], extend);
            unsigned long viaSelf = AssignAnimation(child, source, extend);
            if (viaSelf > viaChild) {
                unsigned long count = AssignAnimation(child, source, extend);
                bone->ExtensionParent = true;
                child->ExtensionChild = true;
                child->AnimSource.Release();
                return count + 1;
            }
        }
        return AssignAnimation(child, source->Children[0], extend) + 1;
    }
    for (unsigned long i = 0; i < bone->Children.size(); ++i) {
        CBoneInfo* child = bone->Children[i].Object;
        CBoneInfo* src = source.Object;
        long index = src->FindChildIndex(child->Name);
        if (index != -1)
            assigned += AssignAnimation(child, src->Children[index], extend) + 1;
    }
    return assigned;
}
