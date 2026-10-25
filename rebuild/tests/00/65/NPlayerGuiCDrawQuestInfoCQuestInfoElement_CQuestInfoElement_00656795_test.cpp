// Behaviour test for NPlayerGui::CDrawQuestInfo::CQuestInfoElement::CQuestInfoElement(float) @ 0x00656795: the constructor
// comes from the source object (declared, not defined here); checks the zeroed words, the scale and the layout.
#include <cstdio>
#include <cstring>
#include <new>
#include <stddef.h>
class CEnginePrimitiveHandle;
namespace NPlayerGui {
class CDrawQuestInfo {
public:
    class CQuestInfoElement {
    public:
        struct CPrimitiveHandleVector {
            CEnginePrimitiveHandle* _M_start;
            CEnginePrimitiveHandle* _M_finish;
            CEnginePrimitiveHandle* _M_end_of_storage;
            CPrimitiveHandleVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
        };
        virtual void v00();
        virtual bool Update();
        virtual void Clear();
        long ID; long ScriptID; float Scale; CPrimitiveHandleVector PrimitiveHandles;
        CQuestInfoElement(float scale);
    };
};
void CDrawQuestInfo::CQuestInfoElement::v00() {}
bool CDrawQuestInfo::CQuestInfoElement::Update() { return true; }
void CDrawQuestInfo::CQuestInfoElement::Clear() {}
}
int main()
{
    typedef NPlayerGui::CDrawQuestInfo::CQuestInfoElement E;
    unsigned char raw[sizeof(E)]; std::memset(raw, 0xcd, sizeof raw);
    E* e = new (raw) E(2.5f);
    int ok = e->ID == 0 && e->ScriptID == 0 && e->Scale == 2.5f && e->PrimitiveHandles._M_start == 0
          && e->PrimitiveHandles._M_finish == 0 && e->PrimitiveHandles._M_end_of_storage == 0 && e->Update()
          && sizeof(E) == 0x1c && offsetof(E, Scale) == 0x0c && offsetof(E, PrimitiveHandles) == 0x10;
    if (!ok) { std::printf("00656795_TEST FAIL\n"); return 1; }
    std::printf("00656795_TEST PASS\n");
    return 0;
}
