#ifndef FABLE_ENGINE_C2DEXTENTSI_H
#define FABLE_ENGINE_C2DEXTENTSI_H
// Original public fields and user default constructor from Ego_r TPI.
class C2DExtentsI { public: long X,Y; C2DExtentsI() {}
 C2DExtentsI(long x,long y):X(x),Y(y){}
#ifdef FABLE_RETAIL_C2DEXTENTSI
 // Retail opt-in (chunk 733, CUserProfileManager::ApplyVideoValues 0x0040A7F0): FableWin.pdb ??0C2DExtentsI@@QAE@ABV0@@Z, the user
 // copy constructor (as C2DExtentsF's): a by-value C2DExtentsI argument is then built in its stack slot (`sub esp,8 ; mov eax,esp ;
 // mov [eax],x ; mov [eax+4],y` before CDisplayManager::EnumerateDisplayModes 0x009C02D0) instead of pushed member by member.
 C2DExtentsI(const C2DExtentsI& other):X(other.X),Y(other.Y){}
#endif
};
#endif
