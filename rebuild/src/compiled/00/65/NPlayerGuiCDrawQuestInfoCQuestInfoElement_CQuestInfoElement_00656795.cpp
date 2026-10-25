// NPlayerGui::CDrawQuestInfo::CQuestInfoElement::CQuestInfoElement(float) -- store the vtable, zero ID/ScriptID and the
// primitive-handle vector, keep the scale argument at +0x0c
// Leaf: ??0CQuestInfoElement@CDrawQuestInfo@NPlayerGui@@QAE@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00656795, length 35 bytes. No callees.
// Name: the body stores vtable 0x0125A2B4 = .?AVCQuestInfoElement@CDrawQuestInfo@NPlayerGui@@ (slot 1 Update 0x006567B8,
// slot 2 0x00644FE5 walks the +0x10/+0x14 vector); Ego_r.pdb/FableWin.pdb ctor ??0CQuestInfoElement@CDrawQuestInfo@NPlayerGui@@QAE@M@Z
// (one float). Layout from Ego_r.pdb: ID +4, ScriptID +8, float Scale +0x0c, vector<CEnginePrimitiveHandle> PrimitiveHandles
// +0x10 (PDB 16 bytes, retail STLport 12: retail object 0x1c). The nine callers are the derived element constructors
// (CQuestInfoElementBar 0x0064A450 / 0x00656B1B, ...). The catalog label "CVsyncCallbackTimerInfo" is propagated.
// Shape: argument loaded into edx first, `mov eax,ecx ; xor ecx,ecx`, vtable, then members in order: a plain initialiser list.
class CEnginePrimitiveHandle;

namespace NPlayerGui {

class CDrawQuestInfo {
public:
    class CQuestInfoElement {
    public:
        struct CPrimitiveHandleVector {                    // retail STLport vector<CEnginePrimitiveHandle> (12 bytes)
            CEnginePrimitiveHandle* _M_start;
            CEnginePrimitiveHandle* _M_finish;
            CEnginePrimitiveHandle* _M_end_of_storage;
            CPrimitiveHandleVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
        };
        virtual void v00();                                // slot 0 (0x00BFEA08, shared stub)
        virtual bool Update();                             // slot 1 (0x006567B8)
        virtual void Clear();                              // slot 2 (0x00644FE5)
        long                   ID;                         // +0x04
        long                   ScriptID;                   // +0x08
        float                  Scale;                      // +0x0c (the constructor argument)
        CPrimitiveHandleVector PrimitiveHandles;           // +0x10 (size 0x1c)
        CQuestInfoElement(float scale);
    };
};

CDrawQuestInfo::CQuestInfoElement::CQuestInfoElement(float scale) : ID(0), ScriptID(0), Scale(scale), PrimitiveHandles()
{
}

} // namespace NPlayerGui
