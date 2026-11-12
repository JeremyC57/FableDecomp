// CConsole::GetRegisteredConsoleInput(const CCharString&) -- look up a registered console input by its lower-cased name (null when absent)
// Naming (agentNF31, 2026-11-12): CCharString::ToLower (0x0099ED10) respelled GetAsLowerCase: Ego_r.pdb 0x00AFA690 + FableWin.pdb
//   0x02F61580 ?GetAsLowerCase@CCharString@@QBE?AV1@XZ (fable_string.h declares it at 0x0099ED10).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EB220, 59 bytes (64 with padding). PDB: ?GetRegisteredConsoleInput@CConsole@@AAEPAVCConsoleInputBase@@ABVCCharString@@@Z.
//   Catalog label "StringMap_FindLowerBoundAndToLower" is wrong.
// Callees: CCharString::ToLower 0x0099ED10 (its temporary lives in the dead argument slot), the input map's find
//   0x009EED20 (iterator in eax), ~CCharString 0x0099EAE0.
// Shape: the same inline FindInput wrapper as the landed CConsole::RunCommandLine 0x009EB430 (a direct
//   RegisteredConsoleInputs.find(...) hoists `lea esi,[ecx+40h]` above the ToLower call); end() is the header pointer.
// Layout: retail CConsole RegisteredConsoleInputs +0x40 (STLport map, 12 bytes); node value pair at +0x10.
// Leaf: ?GetRegisteredConsoleInput@CConsole_Retail@@QAEPAVCConsoleInputBase@@ABVCCharString@@@Z
#include "fable_compiler.h"
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString GetAsLowerCase() const;            // 0x0099ED10
    ~CCharString();                         // 0x0099EAE0
};

class CConsoleInputBase { public:
    virtual void v00();
};
struct CInputPointer {                      // CCountedPointer<CConsoleInputBase>
    CConsoleInputBase* Object;
    void*              Info;
};
struct SInputNode {                         // STLport _Rb_tree_node<pair<const CCharString, CCountedPointer<..>>>
    unsigned char  _M_color;
    SInputNode*    _M_parent;
    SInputNode*    _M_left;
    SInputNode*    _M_right;
    CCharString    first;                   // +0x10
    CInputPointer  second;                  // +0x14
};
struct CInputMap {                          // map<CCharString, CCountedPointer<CConsoleInputBase>> (12 bytes)
    SInputNode*   _M_header;
    unsigned long _M_node_count;
    unsigned char _M_compare;
    SInputNode* find(const CCharString& key);       // 0x009EED20 (the iterator in eax)
};
struct CConsole_Retail {
    virtual void v00();
    unsigned char  _pad04[0x3c];            // +0x04 maps, command lines, HelpBarString
    CInputMap      RegisteredConsoleInputs; // +0x40
    CConsoleInputBase* GetRegisteredConsoleInput(const CCharString& name);
    FABLE_FORCEINLINE SInputNode* FindInput(const CCharString& key) { return RegisteredConsoleInputs.find(key); }
};

CConsoleInputBase* CConsole_Retail::GetRegisteredConsoleInput(const CCharString& name)
{
    SInputNode* it = FindInput(name.GetAsLowerCase());
    if (it != RegisteredConsoleInputs._M_header)
        return it->second.Object;
    return 0;
}