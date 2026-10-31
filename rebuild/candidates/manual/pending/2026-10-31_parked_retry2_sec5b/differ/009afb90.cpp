// CDefinitionManager::LoadBinaryDefinitions(CDataInputStream& stream) -- load the compiled binary definition set: bail on an empty stream; read the safe-format flag, the dependency CRC (rejected when it differs from a non-zero DependencyCheckCRC) and the string-table id (rejected when it differs from g_DefStringTable.RandomID); clear the instantiated defs, read the def count and per def (with a progress tick) allocate a CInstantiatedDefInfo, read its class / instantiation CDefStrings and class index, append the def's index to its class's GlobalIndices and its (name CRC, index) to NameCRCToIndexMap; then read CompressedOffsets and CompressedDefData as blocks and re-sort the name map if dirty
// Grade: DIFFER(1354v1344) 82/486 (metric-best sibling with the map Add left out of line: DIFFER(1314v1344) 79/486) -- the control flow, every call and the inline Read template match; the residue is the frame: retail runs a 0x1c frame with the def count and both type tags in the dead stream-argument slot, this build a 0x24 frame (count and the __false_type tag get their own slots), which shifts every esp-relative operand; inside the loop retail keeps `this` spilled and the class CRC in esi across LowerBound
// Leaf: ?LoadBinaryDefinitions@CDefinitionManager@@QAE_NAAVCDataInputStream@@@Z
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AFB90, 1344 bytes, `ret 4`.
// Name: Ego_r.pdb ?LoadBinaryDefinitions@CDefinitionManager@@QAE_NAAVCDataInputStream@@@Z; members from the PDB layout
//   (NameCRCToIndexMap +0x68, DefClassInfo +0x78, CompressedDefData +0x88, CompressedOffsets +0x94, InstantiatedDefs +0xa4,
//   DependencyCheckCRC +0xbc, UseSafeBinaryFormat +0xc0 = engine/CDefinitionManager.h retail offsets); CInstantiatedDefInfo
//   {ClassName, InstantiationName, ClassIndex, PDef} and CDefClassInfo {Name, AllocFunc, GlobalIndices} from fable_types.h.
//   0x013CA854 = g_DefStringTable (0x013CA828) + 0x2c RandomID (landed CDefStringTable writers name the field).
// Callees: landed ReadWithSrcChunkOverflow 0x00993CA0, ClearInstantiatedDefs 0x009AD950, InstantiatedDefs resize(n, x)
//   0x009B70D0, CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0, NProgressDisplay::DisplayProgress 0x009E9F40,
//   operator new 0x00BFEA1A, CDefString::TransferBinaryIn 0x0044FC00, CDefStringTable::GetStringCRC 0x009D4A20,
//   CVectorMap LowerBound 0x009B2230, vector<unsigned long>::_M_insert_overflow 0x00464ACE, vector<pair<ulong,long>>
//   ::_M_insert_overflow 0x009B61F0, vector<pair<ulong,ulong>>::resize(n) 0x009B8350, vector<unsigned char>::resize(n, x)
//   0x00486189, sort (landed as Sort_SElement8_009b8520, `__fastcall` /Gr body) 0x009B8520.
// Levers: inline value-returning ReadUByte/ReadULong (235 -> 98: the read temps share the dead argument slot); implicit
//   const char* -> CCharString conversion at DisplayProgress (retail recomputes the temp address); GetData `size() > 0`;
//   `long` loop index (98 -> 79: i lands in edi); tag-type empty constructors (no zero store); force-inlined map Add /
//   push_back (restores retail's inline pair push_back, 1314 -> 1354 bytes); caller-side `it != end()` compare + pointer
//   (retail reloads `this` for end()); named tag. Inert: address-taken count, one reused count variable, block-scoped
//   resize default, `int` index, by-value Find key, named class CRC.
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
void* __cdecl operator new(unsigned int size);              // 0x00BFEA1A
inline void* operator new(unsigned int, void* p) { return p; }
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
namespace NProgressDisplay {
    void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}
class CDataInputStream { public:
    virtual void v0();
    unsigned long StreamPos;                                // +0x04
    unsigned long StreamSize;                               // +0x08
    const unsigned char* CurrentSourcePtr;                  // +0x0c
    unsigned long SourceChunkStreamPos;                     // +0x10
    long SourceChunkBytesRemaining;                         // +0x14
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size)
    {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(dst, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    __forceinline unsigned char ReadUByte() { unsigned char value; Read(&value, 1); return value; }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, 4); return value; }
    __forceinline bool IsEndOfStream() const { return StreamPos == StreamSize; }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};
class CDefString { public:
    long TablePos;
    CDefString() : TablePos(-1) { }
    void TransferBinaryIn(CDataInputStream& stream);       // 0x0044FC00
};
class CDefClassBase;
template <class T> class CDefPointer { public:
    T* Data;
    CDefPointer() : Data(0) { }
};
class CDefStringTable { public:
    unsigned char _pad[0x2c];
    unsigned long RandomID;                                 // +0x2c
    unsigned long GetStringCRC(long index);                 // 0x009D4A20
};
extern CDefStringTable g_DefStringTable;                    // 0x013CA828
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class T1, class T2> struct pair {
    T1 first; T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class _Tp> class vector { public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type n) { return _M_start[n]; }
    void resize(size_type n, const _Tp& x);
    void resize(size_type n);
    __forceinline void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) _Tp(__x);
            ++_M_finish;
        } else
            { __false_type __tag; _M_insert_overflow(_M_finish, __x, __tag, 1UL, true); }
    }
    void _M_insert_overflow(_Tp* pos, const _Tp& x, const __false_type&, size_type fill_len, bool atend);
    void _M_insert_overflow(_Tp* pos, const _Tp& x, const __true_type&, size_type fill_len, bool atend);
};
template <> inline void vector<unsigned long>::push_back(const unsigned long& __x)
{
    if (_M_finish != _M_end_of_storage) {
        if (_M_finish)
            *_M_finish = __x;
        ++_M_finish;
    } else
        _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
}
}
template <class T> class CArray : public std::vector<T> { public:
    void Resize(unsigned int n) { resize(n, T()); }
    T* GetData() { return size() > 0 ? _M_start : (T*)this; }
};
template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
void __fastcall Sort_SElement8_009b8520(std::pair<unsigned long, long>* first, std::pair<unsigned long, long>* last,
                                        CKeyPairCompareLess<unsigned long, long> comp);   // 0x009B8520
template <class K, class V, class C> class CVectorMap { public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    CArray<value_type> Vec;
    C Compare;
    bool Dirty;
    iterator end() { return Vec._M_finish; }
    value_type* LowerBound(const K& key);
    iterator Find(const K& key)
    {
        iterator it = LowerBound(key);
        if (it == end() || key < it->first)
            return end();
        return it;
    }
    V* FindValue(const K& key)
    {
        iterator it = Find(key);
        if (it == end())
            return 0;
        return &it->second;
    }
    __forceinline void Add(const K& key, const V& value)
    {
        Vec.push_back(value_type(key, value));
        Dirty = true;
    }
};
class CDefinitionManager { public:
    struct CDefClassInfo {
        CCharString Name;
        void* AllocFunc;
        CArray<unsigned long> GlobalIndices;
    };
    struct CInstantiatedDefInfo {
        CDefString ClassName;
        CDefString InstantiationName;
        unsigned long ClassIndex;
        CDefPointer<CDefClassBase> PDef;
    };
    unsigned char _pad0[0x68];
    CVectorMap<unsigned long, long, CKeyPairCompareLess<unsigned long, long> > NameCRCToIndexMap;   // +0x68
    CVectorMap<unsigned long, CDefClassInfo, CKeyPairCompareLess<unsigned long, CDefClassInfo> > DefClassInfo;   // +0x78
    CArray<unsigned char> CompressedDefData;                // +0x88
    CVectorMap<unsigned long, unsigned long, CKeyPairCompareLess<unsigned long, unsigned long> > CompressedOffsets;   // +0x94
    CArray<CInstantiatedDefInfo*> InstantiatedDefs;         // +0xa4
    void* ErrorCallback;                                    // +0xb0
    CCharString CurrentFilename;                            // +0xb4
    void* CompiledFileName;                                 // +0xb8
    unsigned long DependencyCheckCRC;                       // +0xbc
    bool UseSafeBinaryFormat;                               // +0xc0
    void ClearInstantiatedDefs();                           // 0x009AD950
    bool LoadBinaryDefinitions(CDataInputStream& stream);
};

bool CDefinitionManager::LoadBinaryDefinitions(CDataInputStream& stream)
{
    if (stream.IsEndOfStream())
        return false;
    bool useSafe = stream.ReadUByte() != 0;
    unsigned long crc = stream.ReadULong();
    if (DependencyCheckCRC != 0 && crc != DependencyCheckCRC)
        return false;
    if (stream.ReadULong() != g_DefStringTable.RandomID)
        return false;
    UseSafeBinaryFormat = useSafe;
    ClearInstantiatedDefs();
    unsigned long count = stream.ReadULong();
    InstantiatedDefs.Resize(count);
    for (long i = 0; i < count; ++i) {
        NProgressDisplay::DisplayProgress("Definition Manager: Load Binary Def", -1.0f, false, false);
        InstantiatedDefs[i] = new CInstantiatedDefInfo;
        CInstantiatedDefInfo* info = InstantiatedDefs[i];
        info->ClassName.TransferBinaryIn(stream);
        info->InstantiationName.TransferBinaryIn(stream);
        info->ClassIndex = stream.ReadULong();
        unsigned long classCRC = g_DefStringTable.GetStringCRC(info->ClassName.TablePos);
        CVectorMap<unsigned long, CDefClassInfo, CKeyPairCompareLess<unsigned long, CDefClassInfo> >::iterator it = DefClassInfo.Find(classCRC);
        CDefClassInfo* classInfo = it != DefClassInfo.end() ? &it->second : 0;
        if (classInfo)
            classInfo->GlobalIndices.push_back(i);
        NameCRCToIndexMap.Add(g_DefStringTable.GetStringCRC(info->InstantiationName.TablePos), i);
    }
    CompressedOffsets.Vec.resize(stream.ReadULong());
    std::pair<unsigned long, unsigned long>* offsets = CompressedOffsets.Vec.GetData();
    long offsetBytes = CompressedOffsets.Vec.size() * sizeof(std::pair<unsigned long, unsigned long>);
    if (offsetBytes > 0)
        stream.Read(offsets, offsetBytes);
    CompressedDefData.Resize(stream.ReadULong());
    unsigned char* data = CompressedDefData.GetData();
    long dataBytes = CompressedDefData.size();
    if (dataBytes > 0)
        stream.Read(data, dataBytes);
    if (NameCRCToIndexMap.Dirty) {
        Sort_SElement8_009b8520(NameCRCToIndexMap.Vec._M_start, NameCRCToIndexMap.Vec._M_finish, NameCRCToIndexMap.Compare);
        NameCRCToIndexMap.Dirty = false;
    }
    return true;
}
