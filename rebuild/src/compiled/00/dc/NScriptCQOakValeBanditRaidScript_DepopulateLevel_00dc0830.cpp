// NScript::CQ_OakValeBanditRaidScript::DepopulateLevel() -- gather every "OakValeBanditRaidBandit" thing and remove them all (no fade)
// Leaf: ?DepopulateLevel@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC0830, 141 bytes, `__fastcall`, no arguments, plain `ret`.
//   FableWin symbol ?DepopulateLevel@CQ_OakValeBanditRaidScript@NScript@@QAEXXZ. Caller: PopulateLevel @ 0x00DC0300
//   (call at 0x00DC05D5).
// Label correction: the catalog "CQ_OakValeBanditRaid_CleanupBandits" is descriptive.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, the script's Game (+0x40)
//   virtual slot 75 (+0x12C) GetAllThingsWithScriptName(const CCharString&, vector<CScriptThing>&) const,
//   KillAllThingsInVector(CGameScriptInterfaceBase&, const vector<CScriptThing>&, bool) @ 0x00CBED82 (__fastcall; the
//   chunk's own row), each element's virtual destructor (slot 0 with 0), free @ 0x00BFEA14.
// String: "OakValeBanditRaidBandit" @ 0x012D9EC8.
// Shape: speed TU (no pragma). A local STLport vector<CScriptThing> (three zero stores interleaved with the name
//   temporary's argument pushes), the name temporary dies right after the query, then the vector's inline destructor:
//   `_Destroy(first, last)` (virtual `p->~CScriptThing()` = `push 0 ; call [eax]`, start reloaded after the loop) and the
//   null-tested free of the storage.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptThing {                                    // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();
    void* PImp_Data;                                    // +0x04
    void* PImp_Info;                                    // +0x08
};
namespace _STL {
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        _Destroy(first);
}
template <class T> class allocator {
public:
    allocator() { }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class T> class _STLP_alloc_proxy : public allocator<T> {
public:
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};
template <class T> class _Vector_base {
public:
    T*                    _M_start;                     // +0
    T*                    _M_finish;                    // +4
    _STLP_alloc_proxy<T>  _M_end_of_storage;            // +8
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, (unsigned int)(_M_end_of_storage._M_data - _M_start)); }
};
template <class T> class vector : public _Vector_base<T> {
public:
    vector() : _Vector_base<T>(allocator<T>()) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
class CGameScriptInterfaceBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual long GetAllThingsWithScriptName(const CCharString& name, _STL::vector<CScriptThing>& things) const;   // slot 75 (+0x12c)
};
void __fastcall KillAllThingsInVector(CGameScriptInterfaceBase& game, const _STL::vector<CScriptThing>& things, bool fade);   // 0x00CBED82

namespace NScript {
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
};
class CQ_OakValeBanditRaidScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    void*                     MasterScriptData;         // +0x44
    void DepopulateLevel();
};
void CQ_OakValeBanditRaidScript::DepopulateLevel()
{
    _STL::vector<CScriptThing> bandits;
    Game->GetAllThingsWithScriptName("OakValeBanditRaidBandit", bandits);
    KillAllThingsInVector(*Game, bandits, false);
}
}