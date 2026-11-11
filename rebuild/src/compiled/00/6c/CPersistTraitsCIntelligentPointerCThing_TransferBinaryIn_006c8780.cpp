// 006c8780 CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryIn
// Flags: /O2 /Oy /W3
// Ego_r.pdb 0x006FF6C0 / FableWin.pdb 0x01F373C0
//   ?TransferBinaryIn@?$CPersistTraits@V?$CIntelligentPointer@VCThing@@@@@@QAEXAAVCMemoryDataInputStream@@AAV?$CIntelligentPointer@VCThing@@@@@Z
// (thiscall, `ret 8`; `this` unused). Retail: Fable.exe (TLC, VC7.1) @ 0x006C8780, 324 bytes.
// Rebuilt (agentNF14, w22 item 2) on the shared headers in place of the X_006c8780 namespace and its stand-in bodies (CCharString,
// CTCDNone::Create, the intelligent-pointer bodies, the component LowerBound): the binary twin of the landed text reader
// CPersistTraits<CIntelligentPointer<CThing> >::TransferIn 0x006C9900.
//   ReadType<unsigned __int64>          CDataInputStream inline bounds-checked chunk read (8 bytes), overflow 0x00993CA0
//   CCharString("THING_DUMMY_FOR_SERIALISATION")   0x0099EBF0 / ~ 0x0099EAE0 (string 0x0125E9EC)
//   CThing::PeekPCurrentThingManager()  the static 0x013B8A1C, read in place; CThingManager::PeekWorld (World +0x24, FableWin
//                                       0x01CF6420); CWorld::PeekDefinitionManager (+0x10, FableWin 0x018D4A60)
//   CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410; CTCDNone::Create 0x00835A20 (Ego_r ?Create@CTCDNone@@SIPAVCThing@@JABVC3DVector@@_N@Z)
//   CThing::GetTC(TCI_THING_UID_DUMMY_FOR_SERIALISATION 235, ..)  UsingComponents bit (`test ch,8` on +0x3C), LowerBound 0x0040F020
//   CTCThingUIDDummyForSerialisation::ThingUID +0x10 (Ego_r.pdb, 0x18 bytes over CTCBase)
//   CIntelligentPointer<CThing>(thing) then operator= (vtable 0x01238C6C; 0x00A01B10 / 0x00A01B90 / 0x00A01B50 / 0x00A01C10)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER_PEEKWORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/C3DVector.h"
#include "engine/CPersistTraits.h"

class CTCDNone {
public:
    static CThing* __fastcall Create(long def_index, const C3DVector& position, bool ignore_map);   // 0x00835A20
};

class CTCThingUIDDummyForSerialisation : public CTCBase {                // Ego_r.pdb, 0x18 bytes
public:
    unsigned __int64 ThingUID;                                           // +0x10
};

template <>
void CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryIn(CMemoryDataInputStream& stream, CIntelligentPointer<CThing>& value)
{
    unsigned __int64 uid = stream.ReadType<unsigned __int64>();
    if (uid > 0) {
        CThing* thing;
        {
            CCharString name("THING_DUMMY_FOR_SERIALISATION");
            C3DVector position(0.0f, 0.0f, 0.0f);
            thing = CTCDNone::Create(CThing::PeekPCurrentThingManager()->PeekWorld().PeekDefinitionManager().GetDefGlobalIndexFromName(name),
                                     position, true);
        }
        CTCThingUIDDummyForSerialisation* dummy = 0;
        thing->GetTC(TCI_THING_UID_DUMMY_FOR_SERIALISATION, &dummy);
        dummy->ThingUID = uid;
        value = CIntelligentPointer<CThing>(thing);
    }
}
