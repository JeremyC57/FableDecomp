class CThingGameObject {
public:
    virtual ~CThingGameObject();   // 0x00838A00
};
struct C {};
extern int g_vtbl;
void __fastcall Dtor(C* village){ *(void**)village = &g_vtbl; ((CThingGameObject*)(village))->CThingGameObject::~CThingGameObject(); }