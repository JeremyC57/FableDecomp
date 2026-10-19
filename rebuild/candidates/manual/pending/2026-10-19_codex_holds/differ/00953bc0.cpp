// Grade: HELD(bytes match retail) -- test uses a copy_model stand-in | Codex source 00/95/CCreatureActionDragonTakeOffIntoFlying_CCreatureActionDragonTakeOffIntoFlying_00953bc0.cpp, flags , pass pattern 00953bc0_TEST PASS
// CCreatureAction_DragonTakeOffIntoFlying::CCreatureAction_DragonTakeOffIntoFlying(const CCreatureAction_DragonTakeOffIntoFlying&) -- copy action state and retain counted end action
// Retail 00953bc0, length 0xC1; base copy 0x62f3b0 and vector copy 0x6f0810.
struct CCombatActionBase { virtual void v00(); unsigned char _body[0x110]; CCombatActionBase(const CCombatActionBase&); // 0x0062F3B0
};
struct CCPPointerInfo {};
struct C3DVector { float x,y,z; };
template<class T> struct CRetailVector { T* first; T* last; T* end; CRetailVector(const CRetailVector&); // 0x006F0810
};
template<class T> struct CCountedPointer { T* Data; CCPPointerInfo* Info; CCountedPointer() : Data(0), Info(0) {} CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Data) ++*(long*)Data; } };
struct CCreatureAction_DragonTakeOffIntoFlying : CCombatActionBase {
    float Dest_X,Dest_Y,Dest_Z,Start_X,Start_Y,Start_Z,Previous_X,Previous_Y,Previous_Z;
    CRetailVector<C3DVector> MovementPoints;
    unsigned char UsePreviousPoint; unsigned char _pad[3];
    CCombatActionBase* PEndAction_Data; CCPPointerInfo* PEndAction_Info;
    CCreatureAction_DragonTakeOffIntoFlying(const CCreatureAction_DragonTakeOffIntoFlying&);
};
CCreatureAction_DragonTakeOffIntoFlying::CCreatureAction_DragonTakeOffIntoFlying(const CCreatureAction_DragonTakeOffIntoFlying& o)
 : CCombatActionBase(o), Dest_X(o.Dest_X), Dest_Y(o.Dest_Y), Dest_Z(o.Dest_Z), Start_X(o.Start_X), Start_Y(o.Start_Y), Start_Z(o.Start_Z), Previous_X(o.Previous_X), Previous_Y(o.Previous_Y), Previous_Z(o.Previous_Z), MovementPoints(o.MovementPoints), UsePreviousPoint(o.UsePreviousPoint), PEndAction_Data(o.PEndAction_Data), PEndAction_Info(o.PEndAction_Info) {
    if (PEndAction_Info) ++*(long*)PEndAction_Info;
}