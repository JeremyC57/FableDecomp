#include <stdio.h>
struct C2DVector {
    float X, Y;
    C2DVector(float x=0, float y=0):X(x),Y(y){}
    C2DVector operator-(const C2DVector& p) const {return C2DVector(X-p.X,Y-p.Y);}
};
struct C2DExtentsI { long X,Y; };
class CDisplayManager {
public:
    unsigned calls;
    C2DExtentsI GetRenderTargetDimensions() {
        ++calls;
        C2DExtentsI dims={8,6};
        return dims;
    }
};
static CDisplayManager display;
struct CSystemManager { CDisplayManager* GetDisplayManager(){return &display;} };
static CSystemManager systemManager;
static CSystemManager* GFGetSystemManager(){return &systemManager;}
namespace NUISystem {
enum EEvent { UI_EVENT_MOUSE_MOVEMENT=25, OtherEvent=1 };
class CManager {
public:
    C2DVector movement;
    unsigned sets;
    static CManager* GetInstance(){static CManager m;return &m;}
    C2DVector& GetMouseMovement(){return movement;}
    void SetMouseMovement(C2DVector& delta){movement=delta;++sets;}
};
class CMouseCursor {
public:
    C2DVector Position;
    unsigned changes;
    bool invalidArgs;
    void ChangePositionDelta(const C2DVector& d,float time,bool immediate){
        ++changes;
        if(time!=0 || immediate)invalidArgs=true;
        Position.X+=d.X; Position.Y+=d.Y;
    }
    void ProcessEvent(EEvent event);
};

void CMouseCursor::ProcessEvent(EEvent event)
{
    switch (event) {
    case UI_EVENT_MOUSE_MOVEMENT: {
        C2DVector delta(Position);
        ChangePositionDelta(CManager::GetInstance()->GetMouseMovement(), 0.0f, false);
        if (Position.X >= GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().X)
            Position.X = (float)GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().X;
        if (Position.X < 0.0f)
            Position.X = 0.0f;
        if (Position.Y >= GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().Y)
            Position.Y = (float)GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().Y;
        if (Position.Y < 0.0f)
            Position.Y = 0.0f;
        delta = Position - delta;
        CManager::GetInstance()->SetMouseMovement(delta);
        break;
    }
    }
}
}

int main() {
    using namespace NUISystem;
    struct Case {EEvent event;float dx,dy,x,y;unsigned dimensions;};
    const Case cases[]={
        {OtherEvent,20,20,4,3,0},
        {UI_EVENT_MOUSE_MOVEMENT,1,-1,5,2,2},
        {UI_EVENT_MOUSE_MOVEMENT,-9,-6,0,0,2},
        {UI_EVENT_MOUSE_MOVEMENT,9,7,8,6,4},
        {UI_EVENT_MOUSE_MOVEMENT,4,-3,8,0,3},
        {UI_EVENT_MOUSE_MOVEMENT,-4,3,0,6,3}
    };
    for(unsigned i=0;i<sizeof(cases)/sizeof(cases[0]);++i){
        const Case& c=cases[i];
        CMouseCursor cursor;
        cursor.Position=C2DVector(4,3);
        cursor.changes=0;cursor.invalidArgs=false;
        CManager* manager=CManager::GetInstance();
        manager->sets=0;manager->movement=C2DVector(c.dx,c.dy);
        display.calls=0;
        cursor.ProcessEvent(c.event);
        const bool moving=c.event==UI_EVENT_MOUSE_MOVEMENT;
        if(cursor.Position.X!=c.x || cursor.Position.Y!=c.y ||
           manager->movement.X!=(moving?c.x-4:c.dx) || manager->movement.Y!=(moving?c.y-3:c.dy) ||
           manager->sets!=unsigned(moving)||cursor.changes!=unsigned(moving)||
           cursor.invalidArgs||display.calls!=c.dimensions)return i+1;
    }
    puts("0055c6f0_TEST PASS");
}