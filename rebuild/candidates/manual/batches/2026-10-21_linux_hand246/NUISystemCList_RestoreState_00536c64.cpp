// NUISystem::CList::RestoreState(const std::list<long>& path) -- replay a saved menu path with the front end in "restoring" mode: for each index select that child of the current list, change it to state 5 and pump updates until the state change finishes plus ten more frames, run a type-7 child's select actions, then descend into the first type-8 (list) child of the selected scrolling child, stopping when there is none
// Leaf: RestoreState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00536C64, length 371 bytes, `ret 4`; size-optimised frame-pointer TU
//   (`#pragma optimize("s", on)`).
// Callees: NUISystem::CFrontEndManager::GetInstance @ 0x0041E5F2 (restoring flag at +0xA4); component virtual slots 1
//   Update(float), 48 ChangeState(long), 49 (state change finished), 53 (children: 8-byte counted pointers), 65 GetType,
//   131 ExecuteActions, 154 (select actions), 158 SetSelectedChild(long), 163 (scrolling children). Float pool: 5.0f
//   @ 0x012462A8.
// Layout: STLport list<long> {node}; CList ScrollingChildren (vector<CComponent*>) at retail +0x164 as in the parked
//   GetDataFromDef 0x0053822B; components are modelled through one polymorphic view (slot numbers from vtable 0x012462E4).
// Shape: `current` starts as this and is replaced by the found child; the update pumps always run on the original list;
//   the ten-frame pump is a counted `for`; the child search leaves its index for the "not found" test after the loop.
#pragma optimize("s", on)
namespace NUISystem {
struct CComponent;
struct ChildPointer { CComponent* Data; void* CountInfo; };
struct ChildVector { ChildPointer* Start; ChildPointer* Finish; ChildPointer* Limit; unsigned long size() const { return Finish - Start; } };
struct ComponentVector { CComponent** Start; CComponent** Finish; CComponent** Limit; };
struct CComponent {
    virtual void v000();
    virtual void Update(float dt);                                       // slot 1 (+0x4)
    virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void ChangeState(long state);                                // slot 48 (+0xc0)
    virtual bool IsStateChangeFinished();                                // slot 49 (+0xc4)
    virtual void v050(); virtual void v051(); virtual void v052();
    virtual const ChildVector& GetChildren();                            // slot 53 (+0xd4)
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061();
    virtual void v062(); virtual void v063(); virtual void v064();
    virtual long GetType();                                              // slot 65 (+0x104)
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073();
    virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081();
    virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121();
    virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130();
    virtual void ExecuteActions(void* actions);                          // slot 131 (+0x20c)
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147();
    virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153();
    virtual void* GetActionsOnSelect();                                  // slot 154 (+0x268)
    virtual void v155(); virtual void v156(); virtual void v157();
    virtual void SetSelectedChild(long index);                           // slot 158 (+0x278)
    virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162();
    virtual const ComponentVector& GetScrollingChildren();               // slot 163 (+0x28c)
    unsigned char   _pad004[0x164 - 0x04];
    ComponentVector ScrollingChildren;             // +0x164 (CList)
};
class CFrontEndManager {
public:
    unsigned char _pad[0xa4];
    bool          Restoring;                       // +0xa4
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
struct ListNode {
    ListNode* _M_next;
    ListNode* _M_prev;
    long      _M_data;
};
struct LongList {
    ListNode* _M_node;
};
struct CList : CComponent {
    void RestoreState(const LongList& path);
};

void CList::RestoreState(const LongList& path)
{
    CFrontEndManager* frontEnd = CFrontEndManager::GetInstance();
    frontEnd->Restoring = true;
    CComponent* current = this;
    for (ListNode* node = path._M_node->_M_next; node != path._M_node; node = node->_M_next) {
        long index = node->_M_data;
        current->SetSelectedChild(index);
        current->ChangeState(5);
        while (!current->IsStateChangeFinished())
            Update(5.0f);
        for (long frame = 10; frame; --frame)
            Update(1.0f);
        if (current->GetScrollingChildren().Start[index]->GetType() == 7) {
            CComponent* child = current->GetScrollingChildren().Start[index];
            child->ExecuteActions(child->GetActionsOnSelect());
        }
        CComponent* selected = current->ScrollingChildren.Start[index];
        unsigned long i = 0;
        for (; i != selected->GetChildren().size(); ++i) {
            CComponent* candidate = selected->GetChildren().Start[i].Data;
            if (candidate->GetType() == 8) {
                current = candidate;
                break;
            }
        }
        if (i == selected->GetChildren().size())
            break;
    }
    frontEnd->Restoring = false;
}
}
