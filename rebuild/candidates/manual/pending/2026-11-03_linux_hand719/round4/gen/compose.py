import re,sys,os
D=sys.argv[1]; G=D+'/gen/engine'; H=D+'/hdr/engine'
def gen_body(cls):
    t=open(f'{G}/{cls}.h').read().splitlines()
    # drop guard lines (first two, last line)
    return '\n'.join(t[2:-1]).rstrip()+'\n'
def write(cls,retail,guard=None):
    g=guard or 'FABLE_ENGINE_'+cls.upper()+'_H'
    mac='FABLE_RETAIL_'+cls.upper()
    s=f'#ifndef {g}\n#define {g}\n#ifdef {mac}\n'+retail.strip('\n')+'\n#else\n'+gen_body(cls)+'#endif\n#endif\n'
    open(f'{H}/{cls}.h','w').write(s)
OPT='// Retail opt-in (2026-11-03, chunk 719 row 0x00509A20, agent719c): '
write('C2DLineF',OPT+'''typed C2DVector members (Ego_r TPI: Point1, Point2 : C2DVector); size 0x10.
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
class C2DLineF {
public:
    C2DVector Point1;
    C2DVector Point2;
};
FABLE_STATIC_ASSERT(sizeof(C2DLineF)==0x10);''')
write('CNavigationTransitionPoint',OPT+'''typed C2DVector Position. The (position, layer1, layer2) ctor has no
// out-of-line copy in Ego_r/FableWin (always inlined); 0x00509A20 builds it as Position, Layer1, Layer2 stores before push_back.
// Shape follows the PDB-proven sibling ??0CNavigationPosition@@QAE@ABVC2DVector@@J@Z (FableWin 0x01C90330).
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
class CNavigationTransitionPoint {
public:
    C2DVector Position;
    long Layer1;
    long Layer2;
    CNavigationTransitionPoint(const C2DVector& position,long layer1,long layer2):Position(position),Layer1(layer1),Layer2(layer2){}
};
FABLE_STATIC_ASSERT(sizeof(CNavigationTransitionPoint)==0x10);''')
write('CNavigationPosition',OPT+'''typed C2DVector Position; ctor ??0CNavigationPosition@@QAE@ABVC2DVector@@J@Z (FableWin.pdb 0x01C90330; inlined at 0x00509A20).
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
class CNavigationPosition {
public:
    C2DVector Position;
    long Layer;
    CNavigationPosition(const C2DVector& position,long layer):Position(position),Layer(layer){}
};
FABLE_STATIC_ASSERT(sizeof(CNavigationPosition)==0xc);''')
write('CNavigationAreaUpdate',OPT+'''retail STLport members and the two nested classes
// (Ego_r TPI: CNavigationAreaUpdate::CSwitchableBlockage {UniqueID, LineList} 0x10, ::CNavigationLayerUpdate {LineList,
// SwitchableBlockages} 0x18). Implicit copy/dtor: retail binds the out-of-line copy ctor of CSwitchableBlockage at 0x005199E0.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/C2DLineF.h"
#include "engine/CNavigationTransitionPoint.h"
#include "engine/CNavigationPosition.h"
class CNavigationAreaUpdate {
public:
    class CSwitchableBlockage {
    public:
        unsigned __int64 UniqueID;
        std::list<C2DLineF> LineList;
    };
    class CNavigationLayerUpdate {
    public:
        std::vector<C2DLineF> LineList;
        std::vector<CSwitchableBlockage> SwitchableBlockages;
    };
    std::vector<CNavigationLayerUpdate> NavigationLayers;
    std::vector<CNavigationTransitionPoint> TransitionPoints;
    std::vector<CNavigationPosition> ActionPoints;
    std::vector<unsigned char> TopologyWeights;
};
FABLE_STATIC_ASSERT(sizeof(CNavigationAreaUpdate::CSwitchableBlockage)==0x10);
FABLE_STATIC_ASSERT(sizeof(CNavigationAreaUpdate::CNavigationLayerUpdate)==0x18);
FABLE_STATIC_ASSERT(sizeof(CNavigationAreaUpdate)==0x30);''')
# CMap: retail branch = generated layout + two const queries
gb=gen_body('CMap')
cm=gb.split('#pragma pack(push, 1)')[1].split('#pragma pack(pop)')[0]
cm=cm.replace('    bool                    InitialisePassabilityFromThemes; // +0x1d38',
 '    bool                    InitialisePassabilityFromThemes; // +0x1d38')
cm=cm.replace('};','''    bool IsPassableAt(const C2DCoordI& pos) const;   // 0x0081F3D0 ?IsPassableAt@CMap@@QBE_NABVC2DCoordI@@@Z
    bool IsPrefNavAt(const C2DCoordI& pos) const;    // 0x0081F4B0 ?IsPrefNavAt@CMap@@QBE_NABVC2DCoordI@@@Z
};''')
write('CMap',OPT+'''the generated layout plus the two const queries this row calls (Ego_r.pdb + FableWin.pdb
// spellings, Ego_r 0x00679FE0/0x0067A0B0; reloc bindings 0x0081F3D0 / 0x0081F4B0). No layout change.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/C2DCoordI.h"
class CCachedVolumes;
class CGameDefinitionManager;
class CGameMap;
class CHeightMap;
class CWorld;
#pragma pack(push, 1)'''+cm+'#pragma pack(pop)\nFABLE_STATIC_ASSERT(sizeof(CMap) == 0x1d40);')
# CTCGraphicAppearance: retail branch puts the layout in NGraphicAppearance (its real namespace) + two methods
gb=gen_body('CTCGraphicAppearance')
ca=gb.split('#pragma pack(push, 1)')[1].split('#pragma pack(pop)')[0].replace('struct CTCGraphicAppearance {','class CTCGraphicAppearance {\npublic:')
ca=ca.replace('};','''    void GetAllDummyObjectNames(std::vector<CCharString>& names) const;   // 0x004BDB40
    bool GetDummyObject(const CCharString& name,long index,unsigned long flags,C3DVector& pos,CRightHandedSet& rhs,float scale) const;   // 0x004C08C0
};''')
write('CTCGraphicAppearance',OPT+'''the generated layout inside its real namespace NGraphicAppearance
// (RTTI/PDB: NGraphicAppearance::CTCGraphicAppearance) plus the two const queries this row calls (Ego_r.pdb
// ?GetAllDummyObjectNames@CTCGraphicAppearance@NGraphicAppearance@@QBEXAAV?$vector@VCCharString@@...@std@@@Z and
// ?GetDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@AAVCRightHandedSet@@M@Z).
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
class CTCAnimationBase;
namespace NGraphicAppearance {
#pragma pack(push, 1)'''+ca+'''#pragma pack(pop)
}
FABLE_STATIC_ASSERT(sizeof(NGraphicAppearance::CTCGraphicAppearance) == 0x5c);''')
