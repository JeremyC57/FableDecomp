// Behaviour test for CQ_SecretPassageScript::CQ_SecretPassageScript (0x00DEC7F0): argument pair, empty self-linked
// map headers and cleared entity handles, built over garbage-filled storage.
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __declspec(x)
#endif
class CGameScriptInterface;

class CScriptBase { public:                               // 0x40 bytes, the quest-script root (vtable 0x012C1648)
    virtual void v00() {}
    unsigned char _body[0x3c];
    CScriptBase() { _body[0] = 7; }
};

struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};

struct CCountedPointerWords {                      // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};

class CBaseObject { public:                               // CBaseIntelligentPointer root
    virtual void v00() {}
    void* m_pIntelligentPointer;                   // +0x04
    CBaseObject() : m_pIntelligentPointer(0) {}
};

class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                     // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual void v00() {}
};

struct _Rb_tree_node_base {
    char                _M_color;                  // +0x00
    _Rb_tree_node_base* _M_parent;                 // +0x04
    _Rb_tree_node_base* _M_left;                   // +0x08
    _Rb_tree_node_base* _M_right;                  // +0x0c
};

template <unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(NodeSize * sizeof(void*) / 4); }   // host-width node (32-bit: NodeSize)
};

template <unsigned int NodeSize> struct CRetailMap : _Rb_tree_base<NodeSize> {   // STLport map/_Rb_tree (12 bytes in retail)
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CRetailMap() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = 0;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};

struct CQ_SecretPassageScript : CQuestScriptObject {   // vtable 0x012DDC30
    bool MissionSucceeded;                         // +0x48
    bool MissionFailed;                            // +0x49
    bool MissionOver;                              // +0x4a
    bool MotherFound;                              // +0x4b
    bool DoorsBlocked;                             // +0x4c
    bool DoorsBeingDone;                           // +0x4d
    bool OkayToSpawn;                              // +0x4e
    bool QuestStartScreenShown;                    // +0x4f
    long GhostSightings;                           // +0x50
    CRetailMap<0x24> GlobalMymap;                  // +0x54 map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    CRetailMap<0x18> FlagMap;                      // +0x60 map<CCharString, bool>
    CScriptGameResourceObjectScriptedThingBase minions[11];   // +0x6c
    CScriptGameResourceObjectScriptedThingBase seh_Mother;   // +0x11c
    virtual void v00() {}
    CQ_SecretPassageScript(void* data, CGameScriptInterface* game);
};

CQ_SecretPassageScript::CQ_SecretPassageScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}

int main()
{
    void* mem = malloc(sizeof(CQ_SecretPassageScript)); memset(mem, 0xcd, sizeof(CQ_SecretPassageScript));
    int dataWord = 0;
    CGameScriptInterface* game = (CGameScriptInterface*)(&dataWord + 1);
    CQ_SecretPassageScript* p = new (mem) CQ_SecretPassageScript(&dataWord, game);
    int bad = 0;
    if (p->Data != &dataWord || p->Game != game || p->_body[0] != 7) bad = 1;
    _Rb_tree_node_base* h = p->GlobalMymap._M_header;
    if (!h || h->_M_color != 0 || h->_M_parent != 0 || h->_M_left != h || h->_M_right != h || p->GlobalMymap._M_node_count) bad = 2;
    h = p->FlagMap._M_header;
    if (!h || h->_M_color != 0 || h->_M_parent != 0 || h->_M_left != h || h->_M_right != h || p->FlagMap._M_node_count) bad = 3;
    for (int i = 0; i < 11; ++i)
        if (p->minions[i].PImp.Data || p->minions[i].PImp.Info || p->minions[i].m_pIntelligentPointer) bad = 4;
    if (p->seh_Mother.PImp.Data || p->seh_Mother.PImp.Info) bad = 5;
    if (bad) { std::printf("00dec7f0_TEST FAIL\n"); return 1; }
    std::printf("00dec7f0_TEST PASS\n");
    return 0;
}