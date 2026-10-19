#include <cstdio>
// CShaderRenderManager::SetLightGlobal(unsigned long,const C4DVector&) -- grow light-global constants, replace one value and mark dirty
// Leaf: SetLightGlobal
// Retail0098B2C0,201 bytes ending ret8 at0098B386;7 trailing int3 bytes excluded.
// No CShaderRenderManager header. PDB LightGlobals+19c shifts8 bytes to retail194;
// UpdateFlags+3f8 shifts32 to3d8 after eight16->12-byte containers.
// Vector Erase0098CF70/FillInsert0098D360; resized default vector=(0,0,0,1).
struct C4DVector {
    float X,Y,Z,W;
    C4DVector():X(0),Y(0),Z(0),W(1) {}
};
struct CLightGlobalArray {
    C4DVector* Start; C4DVector* Finish; C4DVector* End;
    unsigned long size() const {return Finish-Start;}
    void Erase(C4DVector*,C4DVector*); //0098CF70
    void FillInsert(C4DVector*,unsigned long,const C4DVector&); //0098D360
    void Resize(unsigned long count,const C4DVector& value=C4DVector()) {
        if(count<size()) Erase(Start+count,Finish);
        else FillInsert(Finish,count-size(),value);
    }
};
struct CShaderRenderManager {
    void* VTable;
    unsigned char BeforeGlobals[0x194-4];
    CLightGlobalArray LightGlobals;
    unsigned char BeforeFlags[0x3d8-0x1a0];
    unsigned long UpdateFlags;
    void SetLightGlobal(unsigned long,const C4DVector&);
};
void CShaderRenderManager::SetLightGlobal(unsigned long index,const C4DVector& value) {
    unsigned long count=index+1;
    if(count<=LightGlobals.size()) count=LightGlobals.size();
    LightGlobals.Resize(count);
    LightGlobals.Start[index]=value;
    UpdateFlags|=0x400;
}

static CShaderRenderManager* active;
static C4DVector* replacement;
static int bad,inserts,erases;
static unsigned long oldCount,newCount,initialFlags;
void CLightGlobalArray::Erase(C4DVector*,C4DVector*) {++erases;bad=1;}
void CLightGlobalArray::FillInsert(C4DVector* position,unsigned long count,const C4DVector& value) {
    ++inserts;
    if(this!=&active->LightGlobals || position!=Finish || count!=newCount-oldCount ||
       active->UpdateFlags!=initialFlags || value.X!=0 || value.Y!=0 || value.Z!=0 || value.W!=1) bad=1;
    if(count) {
        for(unsigned long i=0;i<oldCount;++i) replacement[i]=Start[i];
        for(unsigned long j=oldCount;j<newCount;++j) replacement[j]=value;
        Start=replacement;Finish=replacement+newCount;End=replacement+8;
    }
}
static int run(unsigned long oldSize,unsigned long index,unsigned long flags) {
    CShaderRenderManager manager={};active=&manager;
    C4DVector old[8],next[8];replacement=next;
    for(unsigned long i=0;i<8;++i) {old[i].X=(float)i+1;old[i].Y=(float)i+2;old[i].Z=(float)i+3;old[i].W=(float)i+4;next[i].X=-99;}
    manager.LightGlobals.Start=old;manager.LightGlobals.Finish=old+oldSize;manager.LightGlobals.End=old+8;
    manager.UpdateFlags=flags;initialFlags=flags;oldCount=oldSize;newCount=index+1>oldSize?index+1:oldSize;
    manager.BeforeGlobals[0]=19;manager.BeforeGlobals[sizeof(manager.BeforeGlobals)-1]=23;
    manager.BeforeFlags[0]=29;manager.BeforeFlags[sizeof(manager.BeforeFlags)-1]=31;
    C4DVector value;value.X=12.5f;value.Y=-3.25f;value.Z=0.125f;value.W=-0.5f;
    bad=inserts=erases=0;manager.SetLightGlobal(index,value);
    C4DVector* data=newCount>oldSize?next:old;
    if(bad || inserts!=1 || erases || manager.LightGlobals.Start!=data || manager.LightGlobals.Finish!=data+newCount ||
       manager.LightGlobals.End!=data+8 || manager.UpdateFlags!=(flags|0x400)) return 0;
    for(unsigned long j=0;j<newCount;++j) {
        if(j==index) {if(data[j].X!=value.X || data[j].Y!=value.Y || data[j].Z!=value.Z || data[j].W!=value.W) return 0;}
        else if(j<oldSize) {if(data[j].X!=(float)j+1 || data[j].Y!=(float)j+2 || data[j].Z!=(float)j+3 || data[j].W!=(float)j+4) return 0;}
        else if(data[j].X!=0 || data[j].Y!=0 || data[j].Z!=0 || data[j].W!=1) return 0;
    }
    return manager.BeforeGlobals[0]==19 && manager.BeforeGlobals[sizeof(manager.BeforeGlobals)-1]==23 &&
      manager.BeforeFlags[0]==29 && manager.BeforeFlags[sizeof(manager.BeforeFlags)-1]==31;
}
int main() {
    if(!run(0,0,0) || !run(0,4,0x100) || !run(4,1,0x804) || !run(4,3,0x400) || !run(4,6,0x3ff)) {
        printf("0098b2c0_TEST FAIL\n");return 1;
    }
    printf("0098b2c0_TEST PASS\n");return 0;
}