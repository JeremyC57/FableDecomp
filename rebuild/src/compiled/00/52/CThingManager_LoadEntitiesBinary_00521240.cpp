// CThingManager::LoadEntitiesBinary(CMemoryDataInputStream& stream, long serialiseVersionNumber, long levelBeingLoaded, const set<CCharString>& sectionNames, CASuspendableProcess* process, long loadMode, vector<CThing*>& loadedThings) -- read the section count; for each section read its name and size-prefixed block and, when the name (or the load-all section name) is wanted, load size-prefixed entity blocks through LoadSingleThing until a zero size, suspending the process every tick interval; finally suspend once more and activate the loaded things
// Leaf: ?LoadEntitiesBinary@CThingManager@@QAEXAAVCMemoryDataInputStream@@JJABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@PAVCASuspendableProcess@@JAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@4@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00521240, 937 bytes, `ret 0x1c`. FableWin.pdb spelling above.
// Callees: CCharString operator=(const char*) 0x0099EFE0 / operator=(const CCharString&) 0x0099EFB0 / copy 0x0099EC30 /
//   (const char*, long) 0x0099EBF0 / dtor 0x0099EAE0; CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 /
//   SetPositionAssumeWithinSourceChunk 0x00993C10; virtual GetPosition (slot 2) / ReadNullTerminatedString (slot 6);
//   CMemoryDataInputStream(const void*, unsigned long) 0x00994930 / ~CMemoryDataInputStream 0x00993B10;
//   set<CCharString>'s _Rb_tree::_M_find 0x004B9370 (same-TU body, as landed) -> CBasicString<char>::operator< 0x00429950
//   (same-TU body, as landed); __fastcall NProgressDisplay::DisplayProgress 0x009E9F40; CPersistContext(CMemoryDataInputStream&,
//   bool) 0x009BAE30; CThingManager::LoadSingleThing 0x0051FD80; vector<CThing*>::_M_insert_overflow 0x00436AC0 (same-TU
//   body); CASuspendableProcess slot 2 SuspendSuspendableProcess (pure; CSuspendableProcessMicrothread 0x00A44840);
//   CThingManager::ActivateAfterLoading 0x0051E5A0; malloc 0x00BFEA0E / free 0x00BFEA14 (in the _M_insert_overflow body).
// Inline: CDataInputStream::Read / ReadULONG / ReadSLONG, CMemoryDataInputStream::ReadConstData and the static
//   CLevelLoader::GetLoadAllSectionName (a by-value copy of the "XXX_ALL" CCharString 0x013BAB08; the copy is built in the
//   return slot, so the find argument is `lea`d, not taken from the copy ctor's eax).
// Layout: retail CThingManager SerialiseVersionNumber +0xd8, LevelBeingLoaded +0xdc, CurrentSectionName +0xe0 (the
//   last three Ego_r members, in PDB order); CDataInputStream/CMemoryDataInputStream as engine/*.h (StreamSource +0x18,
//   0x1c bytes); CPersistContext 0x2c bytes (Ego_r). Vtable order from retail 0x0123675C and 0x0129D428.
// Shape: the same-TU noinline bodies of _M_find and _M_insert_overflow let VC7.1 keep the set header in esi across both
//   lookups and pack the empty __true_type temporary with retail's frame (0x94); the statement order of the opening
//   stores is retail's schedule (interval, the two members, hasProcess, ticks).
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);
#include <new>

template <class T> class CBasicString {
public:
    T* m_pData;
    __forceinline long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicString& other) const;   // 0x00429950
};
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CBasicString<T>::operator<(const CBasicString& other) const
{
    return Compare(other) < 0;
}
#pragma auto_inline(on)
class CCharStringData {
public:
    CBasicString<char> Data;
};
class CCharString {
public:
    CCharStringData* PStringData;
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return PStringData->Data < other.PStringData->Data;
    }
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    const CCharString& operator=(const char* text);     // 0x0099EFE0
};
class CWideString;
class CThing;

namespace std {
struct __true_type { __true_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend = false);   // 0x00436AC0
};

// Same-TU body of 0x00436AC0 (STLport 4.6 vector<_Tp*>::_M_insert_overflow), as in the CWorldMap::PostRegionLoad row.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&,
                                                                  unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
    _Tp* __new_start = (_Tp*)malloc(__len * sizeof(_Tp));
    _Tp* __new_finish = (_Tp*)((char*)memmove(__new_start, this->_M_start, (char*)__position - (char*)this->_M_start)
                               + ((char*)__position - (char*)this->_M_start));
    for (unsigned int __n = __fill_len; __n > 0; --__n, ++__new_finish)
        *__new_finish = __x;
    if (!__atend)
        __new_finish = (_Tp*)((char*)memmove(__new_finish, __position, (char*)this->_M_finish - (char*)__position)
                              + ((char*)this->_M_finish - (char*)__position));
    if (this->_M_start)
        free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}
#pragma auto_inline(on)

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Tp> struct _Const_traits { };
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
struct _Rb_tree_base_iterator { _Rb_tree_node_base* _M_node; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> > const_iterator;
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;                         // +0x04
    _Compare _M_key_compare;                            // +0x08 (empty)
    static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(__x->_M_value_field); }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
protected:
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
private:
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;   // 0x004B9370
public:
    const_iterator find(const _Key& __k) const { return const_iterator(_M_find(__k)); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator iterator;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;
    const_iterator find(const _Key& __x) const { return _M_t.find(__x); }
    const_iterator end() const { return _M_t.end(); }
};
// Same-TU body of 0x004B9370 (STLport 4.6 _tree.c), as landed.
#pragma auto_inline(off)
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
__declspec(noinline) _Rb_tree_node<_Value>* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;
    _Link_type __x = _M_root();
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
        __y = this->_M_header._M_data;
    return __y;
}
#pragma auto_inline(on)
} // namespace std

class CDataInputStream {
public:
    virtual ~CDataInputStream();
    virtual void SetPosition(unsigned long position);
    virtual unsigned long GetPosition() const;          // slot 2 (+0x08)
    virtual void Close();
    virtual void ReadCompressed(void* buffer, long size);
    virtual void ReadZlibCompressed(void* buffer, long size);
    virtual CCharString ReadNullTerminatedString();     // slot 6 (+0x18)
    virtual CWideString ReadNullTerminatedWString();
    unsigned long        StreamPos;                     // +0x04
    long                 StreamSize;                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long        SourceChunkStreamPos;          // +0x10
    long                 SourceChunkBytesRemaining;     // +0x14
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPos = (__int64)(StreamPos + size);
        if (newPos < 0 || newPos > 0x7fffffff)
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline unsigned long ReadULONG() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline long ReadSLONG() { long value; Read(&value, sizeof(value)); return value; }
protected:
    void SetPositionAssumeWithinSourceChunk(unsigned long position);   // 0x00993C10
private:
    virtual void InternalReadNextSourceChunk(const unsigned char*& source, long& size);
    virtual bool NeedsBufferedRead(long size);
    virtual void InternalReadUnbuffered(unsigned char* buffer, long size);
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
};
class CMemoryDataInputStream : public CDataInputStream {
public:
    const unsigned char* StreamSource;                  // +0x18
    CMemoryDataInputStream(const void* data, unsigned long size);   // 0x00994930
    virtual ~CMemoryDataInputStream();                  // 0x00993B10
    virtual void ReadCompressed(void* buffer, long size);
    virtual CCharString ReadNullTerminatedString();
    virtual CWideString ReadNullTerminatedWString();
    __forceinline const unsigned char* ReadConstData(long size)
    {
        if (size > 0) {
            unsigned long position = GetPosition();
            __int64 newPos = (__int64)(position + size);
            if (!(newPos < 0 || newPos > 0x7fffffff)) {
                const unsigned char* data = StreamSource + position;
                SetPositionAssumeWithinSourceChunk(position + size);
                return data;
            }
        }
        return 0;
    }
};
class CASuspendableProcess {
public:
    virtual ~CASuspendableProcess();
    virtual void RunSuspendableProcess() = 0;
    virtual bool SuspendSuspendableProcess() = 0;       // slot 2 (+0x08)
    virtual void Restart() = 0;
    virtual void SuspendableProcess();
};
class CPersistContext {                                 // 0x2c bytes
public:
    unsigned char _body[0x2c];
    CPersistContext(CMemoryDataInputStream& stream, bool binary);   // 0x009BAE30
};
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}
extern unsigned long g_LoadTickInterval;                // 0x01375714
extern CCharString g_CCharString_013bab08;              // 0x013BAB08 ("XXX_ALL")
class CLevelLoader {
public:
    static CCharString __fastcall GetLoadAllSectionName() { return g_CCharString_013bab08; }
};

class CThingManager {
public:
    unsigned char _pad00[0xd8];
    long          SerialiseVersionNumber;               // +0xd8
    long          LevelBeingLoaded;                     // +0xdc
    CCharString   CurrentSectionName;                   // +0xe0
    void LoadEntitiesBinary(CMemoryDataInputStream& stream, long serialiseVersionNumber, long levelBeingLoaded,
                            const std::set<CCharString>& sectionNames, CASuspendableProcess* process, long loadMode,
                            std::vector<CThing*>& loadedThings);
private:
    CThing* LoadSingleThing(CPersistContext& context, long loadMode);   // 0x0051FD80
    void ActivateAfterLoading(long level);              // 0x0051E5A0
};

void CThingManager::LoadEntitiesBinary(CMemoryDataInputStream& stream, long serialiseVersionNumber, long levelBeingLoaded,
                                       const std::set<CCharString>& sectionNames, CASuspendableProcess* process,
                                       long loadMode, std::vector<CThing*>& loadedThings)
{
    unsigned long interval = g_LoadTickInterval;
    SerialiseVersionNumber = serialiseVersionNumber;
    LevelBeingLoaded = levelBeingLoaded;
    bool hasProcess = process != 0;
    unsigned long ticks = 0;
    CurrentSectionName = "";
    unsigned long sectionCount = stream.ReadULONG();
    long entitySize;
    for (unsigned long i = 0; i < sectionCount; ++i) {
        CCharString sectionName = stream.ReadNullTerminatedString();
        long sectionSize = stream.ReadSLONG();
        CMemoryDataInputStream section(stream.ReadConstData(sectionSize), sectionSize);
        if (sectionNames.find(sectionName) != sectionNames.end()
            || sectionNames.find(CLevelLoader::GetLoadAllSectionName()) != sectionNames.end()) {
            CurrentSectionName = sectionName;
            for (;;) {
                entitySize = section.ReadSLONG();
                if (!entitySize)
                    break;
                NProgressDisplay::DisplayProgress("Loading entities from script", -1.0f, false, false);
                CMemoryDataInputStream entity(section.ReadConstData(entitySize), entitySize);
                CPersistContext context(entity, true);
                CThing* thing = LoadSingleThing(context, loadMode);
                if (thing)
                    loadedThings.push_back(thing);
                if (++ticks >= interval) {
                    ticks = 0;
                    if (hasProcess)
                        process->SuspendSuspendableProcess();
                }
            }
        }
    }
    if (process)
        process->SuspendSuspendableProcess();
    ActivateAfterLoading(levelBeingLoaded);
}