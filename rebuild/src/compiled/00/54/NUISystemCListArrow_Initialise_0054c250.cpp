struct CListArrow; extern void __fastcall sub(CListArrow* arrow);
struct CListArrow { char pad[0x18c]; char b18c; };
void __fastcall ChangeCameraAngle(CListArrow* arrow){ sub(arrow); arrow->b18c = 0; }