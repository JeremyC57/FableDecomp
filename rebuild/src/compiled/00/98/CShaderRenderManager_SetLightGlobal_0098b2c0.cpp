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