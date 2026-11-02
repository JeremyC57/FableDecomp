# h28_clk.py: naming wave 28, family clk (c696 queue): 0x0055ACB0 is ?Update@CClickable@NUISystem@@UAEXM@Z (RTTI COL of vtable 0x0124BD2C
#  slot 1; Ego_r.pdb; chunk 696). Its two binders call it qualified as the base CClickable::Update: CScrollBar::Update 0x00559600 (which
#  skips CDraggable's override) and CDraggable::Update 0x0055A3E0 (own Leaf the Ego_r / FableWin virtual ?Update@CDraggable@NUISystem@@UAEXM@Z).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
F = 'clk'
a = '00559600'
he.edit(a, 'src', '''struct CDraggable_Retail : CComponentInterface, CObserver_Retail, CEventSinkInterface {   // retail 0x1AC bytes
    unsigned char _pad_0x1c[0x12f - 0x1c];
    char          Layer;                         // +0x12F CComponent::Layer
    unsigned char _pad_0x130[0x170 - 0x130];
    bool          WasLeftClicked;                // +0x170 CClickable
    unsigned char _pad_0x171[0x194 - 0x171];
    C2DVector     PositionBeforeDrag;            // +0x194 CDraggable
    bool          WasDragged;                    // +0x19C
    unsigned char _pad_0x19d[0x1ac - 0x19d];
    virtual void Update(float deltaTime);        // CClickable::Update 0x0055ACB0
};

class CScrollBar : public CDraggable_Retail { public:''', '''class CClickable : public CComponentInterface, public CObserver_Retail, public CEventSinkInterface { public:   // retail 0x194 bytes
    unsigned char _pad_0x1c[0x12f - 0x1c];
    char          Layer;                         // +0x12F CComponent::Layer
    unsigned char _pad_0x130[0x170 - 0x130];
    bool          WasLeftClicked;                // +0x170 CClickable
    unsigned char _pad_0x171[0x194 - 0x171];
    virtual void Update(float deltaTime);        // 0x0055ACB0
};
class CDraggable : public CClickable { public:   // retail 0x1AC bytes
    C2DVector     PositionBeforeDrag;            // +0x194 CDraggable
    bool          WasDragged;                    // +0x19C
    unsigned char _pad_0x19d[0x1ac - 0x19d];
    virtual void Update(float deltaTime);        // 0x0055A3E0
};

class CScrollBar : public CDraggable { public:''')
he.edit(a, 'src', '''    CDraggable_Retail::Update(deltaTime);''', '''    CClickable::Update(deltaTime);''')
he.edit(a, 'src', '''// bool)`, called with PositionBeforeDrag, 0.0f, false), the base CDraggable/CClickable::Update(float)
// @ 0x0055ACB0 (CDraggable does not override it).''', '''// bool)`, called with PositionBeforeDrag, 0.0f, false), CClickable::Update(float) @ 0x0055ACB0, called qualified
// (CDraggable's own override 0x0055A3E0 is skipped).''')
note(a, F, 'the base call 0x0055ACB0 is ?Update@CClickable@NUISystem@@UAEXM@Z (chunk 696; RTTI COL of\n'
     '//   vtable 0x0124BD2C slot 1; Ego_r.pdb), qualified on CClickable. Was CDraggable_Retail::Update.')

a = '0055a3e0'
he.edit(a, 'src', '''class CDraggable { public:
 void* VTable;unsigned char BeforeTime[0x30-4];float Time;C2DVector Position,TargetPosition;
 unsigned char BeforeLayer[0x12f-0x44];unsigned char Layer;unsigned char BeforeClick[0x170-0x130];bool WasLeftClicked;
 unsigned char BeforeDrag[0x194-0x171];C2DVector PositionBeforeDrag;''', '''class CClickable { public:
 virtual void v000();virtual void Update(float delta); //slot 1: 0055ACB0
 unsigned char BeforeTime[0x30-4];float Time;C2DVector Position,TargetPosition;
 unsigned char BeforeLayer[0x12f-0x44];unsigned char Layer;unsigned char BeforeClick[0x170-0x130];bool WasLeftClicked;
 unsigned char BeforeDrag[0x194-0x171];
};
class CDraggable : public CClickable { public:
 C2DVector PositionBeforeDrag;''')
he.edit(a, 'src', ''' void ChangePosition(const C2DVector& p,float time,bool relative){(this->*static_cast<ChangePositionMethod*>(VTable)[31])(p,time,relative);}
 void UpdateMouseCursor(){(this->*static_cast<CursorMethod*>(VTable)[153])();}
 void UpdateClickable(float); //0055ACB0
 bool AtTarget()const{return Position==TargetPosition;}
 void Update(float delta);''', ''' void ChangePosition(const C2DVector& p,float time,bool relative){(this->*static_cast<ChangePositionMethod*>(*(void**)this)[31])(p,time,relative);}
 void UpdateMouseCursor(){(this->*static_cast<CursorMethod*>(*(void**)this)[153])();}
 bool AtTarget()const{return Position==TargetPosition;}
 virtual void Update(float delta);''')
he.edit(a, 'src', ''' UpdateClickable(delta);''', ''' CClickable::Update(delta);''')
note(a, F, 'the base update 0x0055ACB0 is ?Update@CClickable@NUISystem@@UAEXM@Z (chunk 696), called\n'
     '//   qualified on the polymorphic CClickable base; the own Leaf is the virtual (Ego_r / FableWin UAE). Was the non-virtual\n'
     '//   CDraggable::UpdateClickable (QAE) over a flattened VTable member.', leaf='?Update@CDraggable@NUISystem@@UAEXM@Z')
