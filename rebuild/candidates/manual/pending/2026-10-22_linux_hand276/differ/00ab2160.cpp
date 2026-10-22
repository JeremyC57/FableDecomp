// C3DAnimationBuilder::GetIntersection(CArray<CCountedPointer<C3DAnimFileSequenceChunk>>& a, ... b) -- builds both bone trees, matches them, then keeps only the matched/forced bones of a (with b's counterparts, re-parented by the index map) and assigns the results back
// Grade: DIFFER(1097v960) -- first draft: structure and destruction order match (outA, outB, infosA, infosB, roots, index map), but the AssignAnimation result is kept in edi not the frame, the loop-body counted-pointer copies schedule differently and the inlined array destructors are larger than retail's
// Leaf: GetIntersection
extern "C" void __cdecl free(void*);
void __cdecl operator delete(void*);

struct __true_type {};

struct CRefInfo {
    long RefCount;
    void (__fastcall* Deleter)(void*);
    void* Data;
};

template <class T>
class CCountedPointer {
public:
    T* Object;
    CRefInfo* Info;
    CCountedPointer() : Object(0), Info(0) {}
    CCountedPointer(const CCountedPointer& o) : Object(o.Object), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->Deleter(Info->Data);
            operator delete(Info);
        }
        Object = 0;
        Info = 0;
    }
    T* operator->() const { return Object; }
    T& operator*() const { return *Object; }
};

class C3DAnimFileSequenceChunk;
void __fastcall DestroyRange(CCountedPointer<C3DAnimFileSequenceChunk>* first, CCountedPointer<C3DAnimFileSequenceChunk>* last, const char& alloc);
class C3DAnimFileSequenceChunk {
public:
    char pad[0x30];
    long ParentIndex;
    void SetParentIndex(long index);
};

template <class T> class CCountedPointer;
class C3DAnimFileSequenceChunk;
template <class T>
class CArray {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CArray()
    {
        char alloc;
        DestroyRange(_M_start, _M_finish, alloc);
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long i) { return _M_start[i]; }
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned long n, bool atend);
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else {
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
        }
    }
    CArray& operator=(const CArray& o);
};

inline void* __cdecl operator new(unsigned int, void* p) { return p; }

typedef CArray<CCountedPointer<C3DAnimFileSequenceChunk> > CSequenceChunkArray;

class CULongArray {
public:
    unsigned long* _M_start;
    unsigned long* _M_finish;
    unsigned long* _M_end_of_storage;
    CULongArray(unsigned long n, const unsigned long& value, const char& alloc);
    ~CULongArray() { if (_M_start) free(_M_start); }
};

class C3DAnimationBuilder {
public:
    class CBoneInfo {
    public:
        char pad[0x18];
        CBoneInfo* Matched;
        long Index;
        bool ForceA;
        bool ForceB;
    };
    typedef CArray<CCountedPointer<CBoneInfo> > CBoneInfoArray;

    CCountedPointer<CBoneInfo> BuildBoneInfoTree(const CSequenceChunkArray& chunks, CBoneInfoArray& infos);
    unsigned long AssignAnimation(CBoneInfo& info, const CCountedPointer<CBoneInfo>& other, bool top);
    unsigned long GetIntersection(CSequenceChunkArray& a, CSequenceChunkArray& b);
};


void __fastcall DestroyRange(CCountedPointer<C3DAnimationBuilder::CBoneInfo>* first, CCountedPointer<C3DAnimationBuilder::CBoneInfo>* last, const char& alloc);

unsigned long C3DAnimationBuilder::GetIntersection(CSequenceChunkArray& a, CSequenceChunkArray& b)
{
    CSequenceChunkArray outA;
    CSequenceChunkArray outB;
    CBoneInfoArray infosA;
    CBoneInfoArray infosB;
    CCountedPointer<CBoneInfo> rootA = BuildBoneInfoTree(a, infosA);
    CCountedPointer<CBoneInfo> rootB = BuildBoneInfoTree(b, infosB);
    unsigned long result = AssignAnimation(*rootA, rootB, true);
    char alloc;
    unsigned long none = 0xffffffff;
    CULongArray indexMap(a.size(), none, alloc);
    for (unsigned long i = 0; i < a.size(); ++i) {
        CBoneInfo* info = infosA[i].Object;
        bool forced = info->ForceA || info->ForceB;
        if (info->Matched == 0 && !forced)
            continue;
        CCountedPointer<C3DAnimFileSequenceChunk> chunk = a[i];
        long parent = chunk->ParentIndex;
        if (parent != -1) {
            parent = indexMap._M_start[parent];
            chunk->SetParentIndex(parent);
        }
        chunk->SetParentIndex(parent);
        indexMap._M_start[i] = outA.size();
        outA.push_back(chunk);
        if (forced) {
            outB.push_back(CCountedPointer<C3DAnimFileSequenceChunk>());
        } else {
            CCountedPointer<C3DAnimFileSequenceChunk> other = b[info->Matched->Index];
            outB.push_back(other);
        }
    }
    a = outA;
    b = outB;
    return result;
}
