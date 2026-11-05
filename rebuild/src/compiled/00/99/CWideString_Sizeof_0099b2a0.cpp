// CWideString::Sizeof @ 0099b2a0
// Leaf: ?CWideString_Sizeof@@YIKPAVCWideString@@@Z
struct WVec { long begin; long end; };
class CWideString { public: WVec* vec; };

unsigned long __fastcall CWideString_Sizeof(CWideString* self)
{
    WVec* v = self->vec;
    unsigned long r = 4;
    if (v != 0)
        r = (unsigned long)((((v->end - v->begin) >> 1)) * 2 + 4);
    return r;
}