#pragma optimize("s",on)
// Leaf: _Destroy
struct Element450a14 { virtual void Destroy(int flags); unsigned char unused04[0x3c]; };
void __fastcall _Destroy(Element450a14* first,Element450a14* last,void*){while(first!=last){first->Destroy(0);++first;}}