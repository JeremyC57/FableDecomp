#include <cstdio>
#include <cstring>
#include <new>

class CBaseClassNonCopyable {
public:  virtual ~CBaseClassNonCopyable() {} CBaseClassNonCopyable() {} };
class CScriptThing { public:
    long m_id;
    CScriptThing(long id) : m_id(id) {}
    CScriptThing MakeScriptThing() const { return CScriptThing(m_id * 10); }   // stands in for the manager call
};
struct CActionBase : CBaseClassNonCopyable { long m_actionState; bool m_started; bool m_finished;
    CActionBase() : m_actionState(0), m_started(false), m_finished(false) {} };
struct CActionWait : CActionBase { CScriptThing m_thing;
    CActionWait(const CScriptThing& thing) : m_thing(thing.MakeScriptThing()) {} };
int main()
{
    CScriptThing source(7);
    CActionWait a(source);
    if (a.m_actionState != 0 || a.m_started || a.m_finished || a.m_thing.m_id != 70) { std::printf("00905130_TEST FAIL\n"); return 1; }
    std::printf("00905130_TEST PASS\n");
    return 0;
}