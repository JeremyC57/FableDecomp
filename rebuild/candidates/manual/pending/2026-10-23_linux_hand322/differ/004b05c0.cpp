// CQuestManager::TransferBoasts(CPersistContext&) -- save: the accepted-boast count, then each accepted boast's script name, id and failed/completed flags under numbered names; load: clear the temporary boast list, read the count and rebuild each boast (name, id, flags) into it
// Grade: DIFFER(1116v1126) -- retail keeps the three per-boast default values (0 / false / false) in distinct frame slots
//   stored once in each loop preheader (after the empty-list guard); every source shape tried (literal temporaries,
//   outer-scope locals, in-loop locals, const in-loop locals) either re-stores them per call or before the guard.
// Leaf: TransferBoasts
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B05C0, 1124 bytes (int3 padded to 1136), `ret 4`.
// Callees: CPersistContext::Transfer overloads long 0x004B8D70 (the count), CCharString 0x004109A0, long 0x00410BE0,
//   bool 0x004045C0; GFIntToCharString 0x0099F830 / operator+(const char*, const CCharString&) 0x0099F690 (`__fastcall`);
//   CCharString() 0x0099E4B0 / copy 0x0099EC30 / operator= 0x0099EFB0 / operator const char* 0x0099E4C0 / ~ 0x0099EAE0;
//   CWideString() 0x0099AED0 / ~ 0x0099B510; list<CBoast>::clear 0x004B8C00; malloc 0x00BFEA0E; CBoast copy 0x004B73A0.
// Layout: retail STLport CQuestManager (list headers 4 bytes: Boasts +0x68, AcceptedBoasts +0x6c, TempBoasts +0x70; as
//   the landed AddBoast 0x004B1720); CBoast as Ego_r.pdb (32 bytes); CPersistContext Mode +0x18.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CCharString {
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    operator const char*() const;                       // 0x0099E4C0
};
CCharString __fastcall GFIntToCharString(long value);  // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
class CWideString {
public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
};
struct CPersistContext {
    unsigned char _pad00[0x18];
    long          Mode;                                 // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void TransferCount(const char* name, long& value, const long& defaultValue);             // 0x004B8D70
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);    // 0x004109A0
    void Transfer(const char* name, long& value, const long& defaultValue);                  // 0x00410BE0
    void Transfer(const char* name, bool& value, const bool& defaultValue);                  // 0x004045C0
};
inline const CCharString& AsConst(const CCharString& s) { return s; }
inline void TransferString(CPersistContext& context, const char* name, CCharString& value)
{
    CCharString defaultValue(AsConst(CCharString()));
    context.Transfer(name, value, defaultValue);
}
struct CBoast {                                         // CQuestManager::CBoast (PDB, 32 bytes)
    long        ID;                                     // +0x00
    long        Cost;                                   // +0x04
    long        Reward;                                 // +0x08
    CCharString ScriptName;                             // +0x0c
    CWideString Text;                                   // +0x10
    CWideString Title;                                  // +0x14
    bool        Evil;                                   // +0x18
    bool        Failed;                                 // +0x19
    bool        Completed;                              // +0x1a
    long        GroupIndex;                             // +0x1c
    CBoast() : Failed(false), Completed(false), GroupIndex(0) {}
    CBoast(const CBoast& other);                        // 0x004B73A0
};
struct CBoastNode {
    CBoastNode* next;
    CBoastNode* prev;
    CBoast      value;                                  // +0x08
};
struct CBoastList {                                     // STLport list<CBoast>
    struct iterator {
        CBoastNode* _M_node;
        iterator(CBoastNode* n) : _M_node(n) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
        CBoast& operator*() const { return _M_node->value; }
        CBoast* operator->() const { return &_M_node->value; }
        iterator& operator++() { _M_node = _M_node->next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    CBoastNode* m_node;
    iterator begin() { return iterator(m_node->next); }
    iterator end() { return iterator(m_node); }
    long size()
    {
        long n = 0;
        for (iterator it = begin(); it != end(); ++it)
            ++n;
        return n;
    }
    void clear();                                       // 0x004B8C00
    CBoastNode* _M_create_node(const CBoast& x)
    {
        CBoastNode* tmp = static_cast<CBoastNode*>(malloc(sizeof(CBoastNode)));
        new (&tmp->value) CBoast(x);
        return tmp;
    }
    void insert(iterator position, const CBoast& x)
    {
        CBoastNode* tmp = _M_create_node(x);
        CBoastNode* pos = position._M_node;
        CBoastNode* prev = pos->prev;
        tmp->next = pos;
        tmp->prev = prev;
        prev->next = tmp;
        pos->prev = tmp;
    }
    void push_back(const CBoast& x) { insert(end(), x); }
};
struct CQuestManager {
    unsigned char _pad00[0x68];
    CBoastList    Boasts;                               // +0x68
    CBoastList    AcceptedBoasts;                       // +0x6c
    CBoastList    TempBoasts;                           // +0x70
    void TransferBoasts(CPersistContext& context);
};

void CQuestManager::TransferBoasts(CPersistContext& context)
{
    if (context.IsSaving()) {
        long numAccepted = AcceptedBoasts.size();
        context.TransferCount("NumAcceptedBoasts", numAccepted, 0);
        long i = 0;
        for (CBoastList::iterator it = AcceptedBoasts.begin(); it != AcceptedBoasts.end(); ++it) {
            CCharString scriptName(it->ScriptName);
            long id = it->ID;
            bool failed = it->Failed;
            bool completed = it->Completed;
            TransferString(context, "BoastScriptName" + GFIntToCharString(i), scriptName);
            context.Transfer("BoastId" + GFIntToCharString(i), id, 0);
            context.Transfer("BoastFailed" + GFIntToCharString(i), failed, false);
            context.Transfer("BoastCompleted" + GFIntToCharString(i), completed, false);
            ++i;
        }
    } else {
        TempBoasts.clear();
        long numAccepted;
        context.TransferCount("NumAcceptedBoasts", numAccepted, 0);
        for (long i = 0; i != numAccepted; ++i) {
            CCharString scriptName;
            long id;
            bool failed;
            bool completed;
            TransferString(context, "BoastScriptName" + GFIntToCharString(i), scriptName);
            context.Transfer("BoastId" + GFIntToCharString(i), id, 0);
            context.Transfer("BoastFailed" + GFIntToCharString(i), failed, false);
            context.Transfer("BoastCompleted" + GFIntToCharString(i), completed, false);
            CBoast boast;
            boast.ScriptName = CCharString(scriptName);
            boast.ID = id;
            if (failed)
                boast.Failed = true;
            if (completed)
                boast.Completed = true;
            TempBoasts.push_back(boast);
        }
    }
}
