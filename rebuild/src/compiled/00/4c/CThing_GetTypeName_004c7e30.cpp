// Naming wave 18: the global stand-in g_typeTable_4c7e30 (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
#include "rebuild_abi.h"
// CThing::GetTypeName @ 0x004c7e30
// idx = byte[this+0x10]; table = (*g).[+4]; return byte[table + idx*20 + 9]
class CThingManager { public: char pad[4]; char *m_data; };
class CThing {
private:
    static CThingManager* PCurrentThingManager;      // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A (private static)
public:
    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }   // Ego_r / FableWin ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ (inline)
private: public: char pad[0x10]; unsigned char m_type; char GetTypeName(); };
char CThing::GetTypeName(){
    unsigned idx = m_type;
    char *t = CThing::GetPCurrentThingManager()->m_data;
    return t[idx*20 + 9];
}