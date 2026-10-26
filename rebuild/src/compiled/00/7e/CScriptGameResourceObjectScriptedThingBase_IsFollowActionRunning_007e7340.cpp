struct P {
  virtual char d0(){return 0;}
  virtual char d1(){return 0;}
  virtual char d2(){return 0;}
  virtual char d3(){return 0;}
  virtual char d4(){return 0;}
  virtual char d5(){return 0;}
  virtual char d6(){return 0;}
  virtual char d7(){return 0;}
  virtual char d8(){return 0;}
  virtual char tgt(int scriptThing);
};
class CScriptGameResourceObjectScriptedThingBase { public: char pad[8]; P* fp; char IsFollowActionRunning(int scriptThing); };
char CScriptGameResourceObjectScriptedThingBase::IsFollowActionRunning(int scriptThing){ P* p=this->fp; if(!p) return 0; return p->tgt(scriptThing); }