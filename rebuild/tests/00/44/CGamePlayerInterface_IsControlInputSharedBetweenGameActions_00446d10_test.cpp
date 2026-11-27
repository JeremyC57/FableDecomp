// 00446d10 CGamePlayerInterface::IsControlInputSharedBetweenGameActions
// Leaf: ?IsControlInputSharedBetweenGameActions@CGamePlayerInterface@@QBE_NABV?$vector@W4EGameAction@@V?$allocator@W4EGameAction@@@std@@@std@@@Z
// Flags: /Gr
#define FABLE_RETAIL_SHARED_CONTROLS_PASS29
#define FABLE_RETAIL_COMBAT_INPUT_PASS26
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND_BODY
#include "engine/EGameAction.h"
#include "engine/CInputTypeBase.h"
#include "engine/CCountedPointer.h"
#include "engine/CVectorMap.h"
#include "engine/CGamePlayerInterface.h"

bool CGamePlayerInterface::IsControlInputSharedBetweenGameActions(const std::vector<EGameAction>& game_actions) const
{
    std::vector<CCountedPointer<const CInputTypeBase> > inputs;
    for (std::vector<EGameAction>::const_iterator it=game_actions.begin();it!=game_actions.end();it++) {
        EGameAction game_action=*it;
        CVectorMap<EGameAction,CCountedPointer<const CInputTypeBase> >::const_iterator it_upper=Controls.UpperBound(game_action);
        for (CVectorMap<EGameAction,CCountedPointer<const CInputTypeBase> >::const_iterator it2=Controls.LowerBound(game_action);it2!=it_upper;it2++) {
            const CCountedPointer<const CInputTypeBase>& pinput=it2->second;
            for (std::vector<CCountedPointer<const CInputTypeBase> >::iterator it3=inputs.begin();it3!=inputs.end();it3++) {
                if (**it3==*pinput) return true;
            }
            inputs.push_back(pinput);
        }
    }
    return false;
}

#include <stdio.h>
CInputTypeBase::~CInputTypeBase() {}
static int calls;
static int destroyed;
class TestInput : public CInputTypeBase {
public:
    int identity;
    TestInput(int value) : identity(value) {}
    virtual ~TestInput() { ++destroyed; }
    virtual NControlSystem::EInputType GetType() const { return NControlSystem::INPUT_TYPE_KEYBOARD_KEY; }
    virtual bool operator==(const CInputTypeBase& rhs) const {
        ++calls;
        return identity == static_cast<const TestInput&>(rhs).identity;
    }
};
int main() {
    unsigned cases=0;
    typedef CCountedPointer<const CInputTypeBase> Input;
    typedef std::pair<EGameAction,Input> Binding;
    for (unsigned pattern=0;pattern<27;++pattern) {
        for (unsigned seq=0;seq<85;++seq) {
            unsigned created=0;
            destroyed=0;
            {
                CGamePlayerInterface player;
                int keys[6],values[6]; unsigned n=0, digits=pattern;
                for(unsigned key=0;key<3;++key) {
                    unsigned count=digits%3;digits/=3;
                    for(unsigned j=0;j<count;++j) {
                        int id=(key+2*j)%3;
                        Input input(new TestInput(id));
                        player.Controls.push_back(Binding((EGameAction)key,input));
                        keys[n]=key;values[n++]=id;++created;
                    }
                }
                std::vector<EGameAction> actions;
                unsigned len=(seq==0?0:seq<=4?1:seq<=20?2:3);
                unsigned code=seq-(len==0?0:len==1?1:len==2?5:21);
                bool seen[3]={false,false,false};
                bool expected=false;
                int expectedCalls=0,prior[18];unsigned entries=0;
                for(unsigned k=0;k<len;++k) {
                    int key=code%4;code/=4;actions.push_back((EGameAction)key);
                    for(unsigned e=0;e<n && !expected;++e) if(keys[e]==key) {
                        int val=values[e];
                        for(unsigned q=0;q<entries;++q) {++expectedCalls;if(prior[q]==val) break;}
                        expected=seen[val];seen[val]=true;prior[entries++]=val;
                    }
                }
                calls=0;
                if(player.IsControlInputSharedBetweenGameActions(actions)!=expected || calls!=expectedCalls || destroyed) return 1;
                for(unsigned e=0;e<n;++e)
                    if(player.Controls.begin()[e].second.Info->RefCount!=1) return 2;
                ++cases;
            }
            if(destroyed!=(int)created) return 3;
        }
    }
    if(cases!=2295) return 4;
    puts("00446d10_TEST PASS");return 0;
}