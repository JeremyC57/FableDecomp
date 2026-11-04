// CThingManager::RuntimeSaveEntities(long section, const vector<CThing*>& things, bool gamePersistent) -- save a section's things into the runtime persistence: game-persistent ones as a binary stream (clearing the script form) or as script text (clearing the binary form), level-persistent ones as script text
// Leaf: ?RuntimeSaveEntities@CThingManager@@QAEXJABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005239C0, 379 bytes (+5 int3 to 0x005239C0+0x180), thiscall `ret 0Ch`, no EH frame.
// Identity (two sources): Ego_r.pdb ?RuntimeSaveEntities@CThingManager@@QAEXJABV?$vector@PAVCThing@@...@std@@_N@Z 0x0052E3A0, the body after
//   CEntityRuntimePersistence::SetLevelPersistentScript / SetGamePersistentScript / SetGamePersistentSaveData(Compressed)
//   (Ego_r 0x0052E1C0..0x0052E320), as retail 0x005239C0 follows 0x00523790 / 0x005237F0 / 0x00523860 (SetGamePersistentSaveData
//   landed); its three arguments (long, const vector<CThing*>&, bool) are exactly what the body passes to SaveEntitiesBinary /
//   SaveEntitiesToScript and the persistence setters. The catalog's "CThingManager::Member_5239c0" is a placeholder.
// Callees: CMemoryDataOutputStream() 0x00996AF0 / ReserveSize 0x00996BC0 / GetLength 0x00994B40 / PeekData 0x00995E70 / ~ 0x00996B30,
//   CThingManager::SaveEntitiesBinary 0x00522160 / SaveEntitiesToScript 0x00521EB0, CEntityRuntimePersistence::SetGamePersistentSaveData
//   0x00523860 / SetGamePersistentScript 0x005237F0 / SetLevelPersistentScript 0x00523790 (Ego_r.pdb spelling), CCharString() 0x0099E4B0,
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, free 0x00BFEA14; "" 0x0122D70E; the global bool 0x0137548C
//   (initialised true in .data, read only here; no PDB data symbols: descriptive name g_bRuntimeSaveBinary).
// TU: the same-TU model of the landed sibling RuntimeSaveGameHero 0x00523B40 (rebuild/src/compiled/00/52/
//   CThingManager_RuntimeSaveGameHero_00523b40.cpp), copied verbatim: same local classes, the same-TU noinline stand-in bodies of
//   the two savers, the same STLport vector<CCharString> name list.
// Shape: the name vector is the first local; SerialisingGameState (+0x9a) is set before the branch and cleared after it; the script
//   strings and the empty "" script live in the dead `gamePersistent` argument slot.
#define CMemoryDataOutputStream CMemoryDataOutputStreamDonor
#include "engine/CMemoryDataOutputStream.h"
#undef CMemoryDataOutputStream
extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

void* GrowThingVectorStorage(unsigned int count);                                     // stand-in allocator

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);                                   // 0x0099EBF0
    CCharString(const CCharString& other);                                             // 0x0099EC30
    ~CCharString();                                                                    // 0x0099EAE0
    void Reserve(long size);                                                           // 0x0099EF90
    void SetUseFastExtend(bool fast);                                                  // 0x0099EF70
};

class CThing {
public:
    unsigned char _pad_0x00[0x91];
    unsigned char Dead : 1;                                                            // +0x91 bit 0
};
class CThingPlayerCreature : public CThing { };

class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                                                            // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;                                                     // +0x1c
};

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_assignment_operator; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_assignment_operator; };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef typename _Vector_base<_Tp, _Alloc>::allocator_type allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
    __forceinline ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    const _Tp* begin() const { return this->_M_start; }
    const _Tp* end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);
    public:
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
    public:
};

template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x,
                                                                  const __false_type&, size_type __fill_len,
                                                                  bool __atend)
{                                                   // vector<CCharString> growth (same-TU stand-in)
    const size_type __old_size = size();
    const size_type __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    for (pointer __p = this->_M_start; __p != __position; ++__p, ++__new_finish)
        _Construct(__new_finish, *__p);
    for (size_type __i = 0; __i < __fill_len; ++__i, ++__new_finish)
        _Construct(__new_finish, __x);
    if (!__atend)
        for (pointer __q = __position; __q != this->_M_finish; ++__q, ++__new_finish)
            _Construct(__new_finish, *__q);
    _Destroy(this->_M_start, this->_M_finish);
    this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}

template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x,
                                                                  const __true_type&, size_type __fill_len, bool)
{                                                   // 0x00436AC0 (same-TU stand-in: reads x, never the tag)
    pointer __new_start = (pointer)GrowThingVectorStorage(__fill_len);
    if (__new_start != 0) {
        *__new_start = __x;
        this->_M_start = __new_start;
        this->_M_finish = __new_start + 1;
        this->_M_end_of_storage._M_data = __new_start + __fill_len;
    }
}

}

class CMemoryDataOutputStream : public CMemoryDataOutputStreamDonor {
public:
    unsigned long GetLength() const;   // 0x00994B40 (Ego_r.pdb ?GetLength@CMemoryDataOutputStream@@QBEKXZ)
private:
public:
    CMemoryDataOutputStream();                                                         // 0x00996AF0
    ~CMemoryDataOutputStream();                                                        // 0x00996B30
    void ReserveSize(unsigned long size);                                              // 0x00996BC0
    void const* PeekData() const;                                                                  // 0x00995E70
};

namespace NThingManager {
class CEntityRuntimePersistence {
public:
    void SetGamePersistentSaveData(long index, const void* data, unsigned long length);   // 0x00523860
    void SetGamePersistentScript(long index, const CCharString& script);              // 0x005237F0
    void SetLevelPersistentScript(long index, const CCharString& script);             // 0x00523790
};
}

class CThingManager {
public:
    unsigned char _pad_0x00[0x9a];
    bool          SerialisingGameState;                                                // +0x9a
    unsigned char _pad_0x9b[0xa0 - 0x9b];
    NThingManager::CEntityRuntimePersistence EntityRuntimePersistence;                 // +0xa0
    void SaveEntitiesToScript(CCharString& script, long mode, const std::vector<CThing*>& things,
                              std::vector<CCharString>& names);                       // 0x00521EB0
    void SaveEntitiesBinary(CMemoryDataOutputStream& stream, long mode, const std::vector<CThing*>& things,
                            std::vector<CCharString>& names);                         // 0x00522160
    void RuntimeSaveEntities(long section, const std::vector<CThing*>& things, bool gamePersistent);
};

// Same-TU stand-ins for the two savers (0x00521EB0 / 0x00522160 are this TU's members): they read the thing vector,
// write the script or stream and append to the name vector, but keep neither vector's address. Retail VC7.1 saw
// their bodies, so it keeps the name vector's start in ebp across its element destructors.
void SaveThingScript(CCharString& script, CThing* thing, long mode);                  // stand-in helper
void SaveThingBinary(CMemoryDataOutputStream& stream, CThing* thing, long mode);      // stand-in helper
const CCharString& GetThingSectionName(CThing* thing);                                // stand-in helper

__declspec(noinline) void CThingManager::SaveEntitiesToScript(CCharString& script, long mode,
                                                              const std::vector<CThing*>& things,
                                                              std::vector<CCharString>& names)
{                                                   // 0x00521EB0 (stand-in)
    for (CThing* const* it = things.begin(); it != things.end(); ++it) {
        SaveThingScript(script, *it, mode);
        names.push_back(GetThingSectionName(*it));
    }
}

__declspec(noinline) void CThingManager::SaveEntitiesBinary(CMemoryDataOutputStream& stream, long mode,
                                                            const std::vector<CThing*>& things,
                                                            std::vector<CCharString>& names)
{                                                   // 0x00522160 (stand-in)
    for (CThing* const* it = things.begin(); it != things.end(); ++it) {
        SaveThingBinary(stream, *it, mode);
        names.push_back(GetThingSectionName(*it));
    }
}


extern bool g_bRuntimeSaveBinary;                                                     // 0x0137548C (descriptive)

void CThingManager::RuntimeSaveEntities(long section, const std::vector<CThing*>& things, bool gamePersistent)
{
    std::vector<CCharString> names;
    SerialisingGameState = true;
    if (gamePersistent) {
        if (g_bRuntimeSaveBinary) {
            CMemoryDataOutputStream stream;
            stream.ReserveSize(0x2000);
            SaveEntitiesBinary(stream, 2, things, names);
            EntityRuntimePersistence.SetGamePersistentSaveData(section, ((void*)stream.PeekData()), ((CMemoryDataOutputStream*)&stream)->GetLength());
            CCharString noScript("");
            EntityRuntimePersistence.SetGamePersistentScript(section, noScript);
        }
        else {
            CCharString script;
            SaveEntitiesToScript(script, 2, things, names);
            EntityRuntimePersistence.SetGamePersistentScript(section, script);
            EntityRuntimePersistence.SetGamePersistentSaveData(section, 0, 0);
        }
    }
    else {
        CCharString script;
        SaveEntitiesToScript(script, 2, things, names);
        EntityRuntimePersistence.SetLevelPersistentScript(section, script);
    }
    SerialisingGameState = false;
}