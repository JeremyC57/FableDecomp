# sub-wave mig8 (hand): NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer ctor 0x0064A4E9 nested in CDrawQuestInfo, timer by
# reference (Ego_r.pdb 0x00690EEF + FableWin.pdb 0x020EEE7D ??0CQuestInfoElementTimer@CDrawQuestInfo@NPlayerGui@@QAE@ABVCTimer@@JM@Z),
# and its caller AddTimer 0x0064F93F onto the PDB spellings (AddTimer / GetScriptSpriteBySymbol QBE / AddElement private, the
# global CCountedPointer<T> instantiations of FableWin.pdb).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
edit('0064a4e9', 'src', '''struct CQuestInfoElement {                         // NPlayerGui::CDrawQuestInfo::CQuestInfoElement (retail 0x1c bytes)
    virtual void v00();
    long  ID;                                      // +0x04
    long  ScriptID;                                // +0x08
    float Scale;                                   // +0x0c
    CEnginePrimitiveHandleVector PrimitiveHandles; // +0x10
    CQuestInfoElement(float scale);                // 0x00656795
};

struct CQuestInfoElementTimer : CQuestInfoElement {   // retail 0x24 bytes, vtable 0x0125A458
    virtual void v00();
    long TimerIndex;                               // +0x1c
    long Graphic;                                  // +0x20
    CQuestInfoElementTimer(const CTimer* timer, long graphic, float scale);
};

CQuestInfoElementTimer::CQuestInfoElementTimer(const CTimer* timer, long graphic, float scale)
    : CQuestInfoElement(scale), Graphic(graphic)
{
    TimerIndex = timer->TimerIndex;
}
''', '''class CDrawQuestInfo {
public:
    class CQuestInfoElement {                      // retail 0x1c bytes
    public:
        virtual void v00();
        long  ID;                                  // +0x04
        long  ScriptID;                            // +0x08
        float Scale;                               // +0x0c
        CEnginePrimitiveHandleVector PrimitiveHandles; // +0x10
        CQuestInfoElement(float scale);            // 0x00656795 ??0CQuestInfoElement@CDrawQuestInfo@NPlayerGui@@QAE@M@Z (FableWin.pdb)
    };
    class CQuestInfoElementTimer : public CQuestInfoElement {   // retail 0x24 bytes, vtable 0x0125A458
    public:
        virtual void v00();
        long TimerIndex;                           // +0x1c
        long Graphic;                              // +0x20
        CQuestInfoElementTimer(const CTimer& timer, long graphic, float scale);
    };
};

CDrawQuestInfo::CQuestInfoElementTimer::CQuestInfoElementTimer(const CTimer& timer, long graphic, float scale)
    : CQuestInfoElement(scale), Graphic(graphic)
{
    TimerIndex = timer.TimerIndex;
}
''')
edit('0064f93f', 'src', '''template <class T> struct CCountedPointer {        // object + count block (8 bytes); bodies out of line in other TUs
    T*    m_pObject;
    void* m_pBlock;
    explicit CCountedPointer(T* object);
    template <class U> CCountedPointer(const CCountedPointer<U>& other);
    ~CCountedPointer();
    T* operator->() const { return m_pObject; }
};

struct CQuestInfoElement {                         // NPlayerGui::CDrawQuestInfo::CQuestInfoElement (retail 0x1c bytes)
    virtual void v00();
    long  ID;                                      // +0x04
    long  ScriptID;                                // +0x08
    float Scale;                                   // +0x0c
    void* PrimitiveHandles[3];                     // +0x10 vector<CEnginePrimitiveHandle>
};

struct CTimer;

struct CQuestInfoElementTimer : CQuestInfoElement {   // retail 0x24 bytes, vtable 0x0125A458
    long TimerIndex;                               // +0x1c
    long Graphic;                                  // +0x20
    CQuestInfoElementTimer(const CTimer* timer, long graphic, float scale);   // 0x0064A4E9
};

class CDrawQuestInfo { public:
    long GetScriptSpriteBySymbol(const CCharString& symbol);           // 0x0064A37B
    void AddElement(CCountedPointer<CQuestInfoElement> element);       // 0x0064A3ED
    long AddTimer(const CTimer* timer, const CCharString& symbol, float scale);
};

long CDrawQuestInfo::AddTimer(const CTimer* timer, const CCharString& symbol, float scale)
{''', '''}
// The global counted pointer (Ego_r.pdb / FableWin.pdb ?$CCountedPointer@V..@@ instantiations); bodies out of line in other TUs.
template <class T> class CCountedPointer {         // object + count block (8 bytes)
public:
    T*    m_pObject;
    void* m_pBlock;
    explicit CCountedPointer(T* object);           // 0x006570BC (Timer)
    template <class U> CCountedPointer(const CCountedPointer<U>& other);   // 0x00654B7B (Timer -> element)
    ~CCountedPointer();                            // 0x00654B4F (Timer)
    T* operator->() const { return m_pObject; }
};
class CTimer;
namespace NPlayerGui {

class CDrawQuestInfo {
public:
    class CQuestInfoElement {                      // retail 0x1c bytes
    public:
        virtual void v00();
        long  ID;                                  // +0x04
        long  ScriptID;                            // +0x08
        float Scale;                               // +0x0c
        void* PrimitiveHandles[3];                 // +0x10 vector<CEnginePrimitiveHandle>
    };
    class CQuestInfoElementTimer : public CQuestInfoElement {   // retail 0x24 bytes, vtable 0x0125A458
    public:
        long TimerIndex;                           // +0x1c
        long Graphic;                              // +0x20
        CQuestInfoElementTimer(const CTimer& timer, long graphic, float scale);   // 0x0064A4E9
    };
    long GetScriptSpriteBySymbol(const CCharString& symbol) const;     // 0x0064A37B
    long AddTimer(const CTimer& timer, const CCharString& symbol, float scale);
private:
    void AddElement(CCountedPointer<CQuestInfoElement> element);       // 0x0064A3ED
};

long CDrawQuestInfo::AddTimer(const CTimer& timer, const CCharString& symbol, float scale)
{''')
edit('0064a4e9', 'src', '''CQuestInfoElementTimer(const CTimer* timer, long graphic, float scale) -- a quest-info timer element''',
     '''CQuestInfoElementTimer(const CTimer& timer, long graphic, float scale) -- a quest-info timer element''')
edit('0064a4e9', 'src', '''// Leaf: ??0CQuestInfoElementTimer
''', '''// Leaf: ??0CQuestInfoElementTimer
// Identity (naming wave 11): ??0CQuestInfoElementTimer@CDrawQuestInfo@NPlayerGui@@QAE@ABVCTimer@@JM@Z (Ego_r.pdb 0x00690EEF,
//   FableWin.pdb 0x020EEE7D): nested in CDrawQuestInfo, the timer by const reference (same code as the pointer).
''')
edit('0064f93f', 'src', '''// NPlayerGui::CDrawQuestInfo::AddTimer(const CTimer*, const CCharString&, float)''', '''// NPlayerGui::CDrawQuestInfo::AddTimer(const CTimer&, const CCharString&, float)''')
edit('0064f93f', 'src', '''// Leaf: AddTimer
''', '''// Leaf: AddTimer
// Identity (naming wave 11): ?AddTimer@CDrawQuestInfo@NPlayerGui@@QAEJABVCTimer@@ABVCCharString@@M@Z (Ego_r.pdb 0x006977CC,
//   FableWin.pdb 0x020EA61D); callees as both PDBs spell them: GetScriptSpriteBySymbol QBE, AddElement private (AAE) taking
//   the global CCountedPointer<CDrawQuestInfo::CQuestInfoElement>, the nested element classes and FableWin's
//   CCountedPointer<CQuestInfoElementTimer> ctor / converting ctor / dtor.
''')
