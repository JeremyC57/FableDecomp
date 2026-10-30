// 00af2780 NParticleEngine::CPSCBase::GetCurrentPositionLocal (catalog: CParticleUpdateParameters_TransformPosition)
// Identity (naming wave 11): Ego_r.pdb 0x00AB5A70 + FableWin.pdb ?GetCurrentPositionLocal@CPSCBase@NParticleEngine@@QBE?AVC3DVector@@ABV?$CParticleUpdateParameters@KK@2@ABKM_NK@Z;
// its sibling 0x00AF2640 (GetCurrentPosition) applies GetSystemTransform 0x00AF2250, this body GetInverseSystemTransform 0x00AF22C0.
// Flags: /O2 /Oy /W3
// Leaf: ?GetCurrentPositionLocal@CPSCBase@NParticleEngine@@QBE?AVC3DVector@@ABV?$CParticleUpdateParameters@KK@2@ABKM_NK@Z
class C3DVector { public:float x,y,z;C3DVector(){} __forceinline C3DVector(const C3DVector& v){z=v.z;x=v.x;y=v.y;}__forceinline C3DVector(float a,float b,float c){x=a;y=b;z=c;}};
class CMatrix3x4 { public:float m[12];};
// ABI: 0x00af3080 dereferences second stack argument as a key; 0x00af2c50 receives its address.
namespace NParticleEngine {
template<class A,class B> class CParticleUpdateParameters { public: bool GetUpdateParam(C3DVector&,const unsigned long&,float,bool,unsigned long)const;};
// Layout: retail owner +4, owner flags +0x50, fallback virtual slot30.
class CParticleSystem { public:char pad[80];unsigned char flags;void GetInverseSystemTransform(CMatrix3x4&)const;__forceinline C3DVector Transform(const C3DVector& p){C3DVector q=p;if(!(flags&64)){CMatrix3x4 m;GetInverseSystemTransform(m);const CMatrix3x4& mat=m;float x=q.x,y=q.y;q.x=mat.m[6]*q.z+mat.m[3]*q.y+mat.m[0]*q.x+mat.m[9];q.y=mat.m[7]*q.z+mat.m[4]*q.y+mat.m[1]*x+mat.m[10];q.z=mat.m[8]*q.z+mat.m[5]*y+mat.m[2]*x+mat.m[11];}return C3DVector(q.x,q.y,q.z);}};
class CPSCBase { public:
virtual void v0()=0;
virtual void v1()=0;
virtual void v2()=0;
virtual void v3()=0;
virtual void v4()=0;
virtual void v5()=0;
virtual void v6()=0;
virtual void v7()=0;
virtual void v8()=0;
virtual void v9()=0;
virtual void v10()=0;
virtual void v11()=0;
virtual void v12()=0;
virtual void v13()=0;
virtual void v14()=0;
virtual void v15()=0;
virtual void v16()=0;
virtual void v17()=0;
virtual void v18()=0;
virtual void v19()=0;
virtual void v20()=0;
virtual void v21()=0;
virtual void v22()=0;
virtual void v23()=0;
virtual void v24()=0;
virtual void v25()=0;
virtual void v26()=0;
virtual void v27()=0;
virtual void v28()=0;
virtual void v29()=0;
virtual bool Fallback(C3DVector&)const=0;CParticleSystem* owner;C3DVector GetCurrentPositionLocal(const CParticleUpdateParameters<unsigned long,unsigned long>&,const unsigned long&,float,bool,unsigned long)const;
};
C3DVector CPSCBase::GetCurrentPositionLocal(const CParticleUpdateParameters<unsigned long,unsigned long>& r,const unsigned long&a,float b,bool c,unsigned long d)const{C3DVector p;if(!r.GetUpdateParam(p,a,b,c,d)&&!Fallback(p)){p.z=0;p.y=0;p.x=0;}if(owner)p=owner->Transform(p);return C3DVector(p.x,p.y,p.z);}
}