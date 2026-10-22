// CAnimationEntry::TransferBinaryIn(CMemoryDataInputStream* stream) -- read the bank index and the two def names, then (when the component count is non-zero) build a fresh component map and fill it with a clone of each keyed prototype component, each read from the stream
// Leaf: TransferBinaryIn
// Layout: retail CAnimComponentMap is 0x18 bytes (vtable, IV ref count, CVectorMap {12-byte vector, comparator, Dirty +0x15}).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStream {
    void* m_pVtable;
    unsigned long m_position;
    long StreamSize;
    const unsigned char* m_pData;
    long SourceChunkStreamPos;
    long m_remaining;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
template <class T> __forceinline void PersistTransferBinaryIn(CMemoryDataInputStream* stream, T* out)
{
    T value;
    stream->Read(&value, sizeof(value));
    *out = value;
}
template <class T> __forceinline T PersistRead(CMemoryDataInputStream* stream)
{
    T value;
    stream->Read(&value, sizeof(value));
    return value;
}

inline void* operator new(unsigned int, void* where) { return where; }
struct CDefString {
    long Index;
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x0044FC00
};

struct CAnimComponentBase {
    virtual void v00();
    virtual void Delete();                                   // slot 1
    virtual void v02();
    virtual void v03();
    virtual CAnimComponentBase* Clone() const;               // slot 4
    virtual void v05();
    virtual void TransferBinaryIn(CMemoryDataInputStream* stream);   // slot 6
    long RefCount;                                           // +4
};
struct CComponentPtr {                                       // CIVCountedPointer<CAnimComponentBase>
    CAnimComponentBase* P;
    __forceinline CComponentPtr() { }
    __forceinline CComponentPtr(CAnimComponentBase* p) : P(p) { }
    __forceinline CComponentPtr(const CComponentPtr& o) { CAnimComponentBase* p = o.P; P = p; ++p->RefCount; }
    __forceinline ~CComponentPtr()
    {
        CAnimComponentBase* p = P;
        if (--p->RefCount == 0)
            p->Delete();
        P = 0;
    }
};
struct CComponentPair {
    long first;
    CComponentPtr second;
    __forceinline CComponentPair(long k, const CComponentPtr& v) : first(k), second(v) { }
    __forceinline CComponentPair(const CComponentPair& x)
    {
        CAnimComponentBase* p = x.second.P;
        first = x.first;
        second.P = p;
        ++p->RefCount;
    }
};
struct __true_type_ { __forceinline __true_type_() { } };
struct CComponentVector {
    __forceinline CComponentVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CComponentPair* _M_start;
    CComponentPair* _M_finish;
    CComponentPair* _M_end_of_storage;
    void reserve(unsigned int n);                            // 0x005E24D0
    void _M_insert_overflow(CComponentPair* pos, const CComponentPair& x, const __true_type_& t, unsigned int n, bool atend);   // 0x005E29D0
    __forceinline void push_back(const CComponentPair& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CComponentPair(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type_(), 1, true);
    }
};
struct CComponentMap {                                       // CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >
    CComponentVector Vec;
    bool Comparator;
    bool Dirty;
    __forceinline void Add(long key, const CComponentPtr& v)
    {
        Vec.push_back(CComponentPair(key, v));
        Dirty = true;
    }
};
struct CIVCountedPointeeBase {
    virtual void v00();
    virtual void Delete();
    long RefCount;
    __forceinline CIVCountedPointeeBase() : RefCount(1) { }
};
struct CAnimComponentMap : CIVCountedPointeeBase {
    CComponentMap Map;
    __forceinline CAnimComponentMap() { Map.Dirty = false; }
    virtual void v00();
};
struct CMapPtr {
    CAnimComponentMap* P;
    __forceinline CMapPtr& operator=(CAnimComponentMap* p)
    {
        CAnimComponentMap* old = P;
        if (old) {
            if (--old->RefCount == 0)
                old->Delete();
            P = 0;
        }
        P = p;
        return *this;
    }
    CAnimComponentMap* operator->() const { return P; }
};
enum EAnimComponent { ANIM_COMPONENT_NONE = 0 };
struct CProtoEntry { EAnimComponent first; CAnimComponentBase* second; };
struct CProtoMap {
    CProtoEntry* _M_start;
    CProtoEntry* _M_finish;
    CProtoEntry* _M_end_of_storage;
    CProtoEntry* LowerBound(const EAnimComponent* key);                // 0x005DFA50
};
extern CProtoMap g_AnimComponentPrototypes;                  // 0x013B8C28

#pragma auto_inline(off)
CProtoEntry* CProtoMap::LowerBound(const EAnimComponent* key)
{
    CProtoEntry* it = _M_start;
    while (it != _M_finish && it->first < *key) ++it;
    return it;
}
#pragma auto_inline(on)

struct CAnimationEntry {
    long BankIndex;
    CDefString AnimName;
    CDefString GroupName;
    CMapPtr Components;
    void TransferBinaryIn(CMemoryDataInputStream* stream);
};

void CAnimationEntry::TransferBinaryIn(CMemoryDataInputStream* stream)
{
    PersistTransferBinaryIn(stream, &BankIndex);
    AnimName.TransferBinaryIn(stream);
    GroupName.TransferBinaryIn(stream);
    unsigned long count = PersistRead<unsigned long>(stream);
    if (count == 0)
        return;
    Components = new CAnimComponentMap;
    Components->Map.Vec.reserve(count);
    for (unsigned long i = 0; i < count; ++i) {
        EAnimComponent key;
        PersistTransferBinaryIn(stream, &key);
        CProtoEntry* it = g_AnimComponentPrototypes.LowerBound(&key);
        CAnimComponentBase* c = (it == g_AnimComponentPrototypes._M_finish || key < it->first) ? 0 : it->second->Clone();
        c->TransferBinaryIn(stream);
        CComponentPtr comp(c);
        Components->Map.Add(key, comp);
    }
    Components->Map.Dirty = false;
}
