// _global::CopyBackward_PairCharStringScriptInfo_00cb62f0(pair<CCharString,CScriptInfo>*, pair<..>*, pair<..>*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over the script registry's (name, script info) pairs
// Leaf: CopyBackward_PairCharStringScriptInfo_00cb62f0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB62F0, length 123 bytes.
// SGI-STL random-access `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first
// in ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / sizeof(T) by reciprocal multiply, the loop counts it down and the moved-back result is returned.
// Element: std::pair<CCharString, CScriptInfo> (the manifest label; 0x18 bytes). Ego_r.pdb CScriptInfo (0x14):
// AllocFunc +0, MasterScriptDataAlloc +4, CCharString Name +8, long ID +0xC, bool MasterScript +0x10. The implicit
// operator= calls CCharString::operator= @ 0x0099EFB0 for the key, copies the two function pointers, calls it again for
// Name, then ID and MasterScript.
class CCharString { public:                         // one-word handle
    void* m_pData;                           // +0x00
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;

class CScriptInfo { public:                         // Ego_r.pdb, 0x14 bytes
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                   // +0x08
    long             ID;                     // +0x0C
    bool             MasterScript;           // +0x10
};

struct PairCharStringScriptInfo {            // std::pair<CCharString, CScriptInfo>
    CCharString first;                       // +0x00
    CScriptInfo second;                      // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairCharStringScriptInfo* __fastcall CopyBackward_PairCharStringScriptInfo_00cb62f0(PairCharStringScriptInfo* first, PairCharStringScriptInfo* last, PairCharStringScriptInfo* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}