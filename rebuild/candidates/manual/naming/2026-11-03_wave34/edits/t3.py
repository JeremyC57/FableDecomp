D="rebuild/src/compiled/00/"
A=D+"6d/CTCHeroExperience_OnSerialise_006d8e30.cpp"
B=D+"65/CTCVillage_OnSerialise_0065ce9b.cpp"
C=D+"56/CTCHeroOnlineScoreboard_OnSerialise_00560720.cpp"
E=D+"77/CTCShop_OnSerialise_00775c60.cpp"
EDITS=[
(A,"""struct CLongVector {                                   // retail std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    void resize(unsigned long n, const long& value);   // 0x004428F0
};
""","""namespace std {                                       // STLport 4.6 vector<long> (12 bytes: start, finish, end of storage)
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    void resize(unsigned int __new_size, const _Tp& __x);   // 0x004428F0
};
}
"""),
(A,"    void TransferVector(const char* name, CLongVector* value);              // 0x005815D6",
   "    template <class T> void TransferVector(const char* name, std::vector<T, std::allocator<T> >& value);   // 0x005815D6 <long> (Ego_r / FableWin ??$TransferVector@J@)"),
(A,"    CLongVector   StatExperiencePoints;                 // +0x18","    std::vector<long> StatExperiencePoints;             // +0x18"),
(A,"    CLongVector   TrainableStatLevels;                  // +0x24 (PDB +0x28)","    std::vector<long> TrainableStatLevels;              // +0x24 (PDB +0x28)"),
(A,'context.TransferVector("StatExperiencePoints", &StatExperiencePoints);','context.TransferVector("StatExperiencePoints", StatExperiencePoints);'),
(A,'context.TransferVector("TrainableStatLevels", &TrainableStatLevels);','context.TransferVector("TrainableStatLevels", TrainableStatLevels);'),
(B,"    void TransferVector(const char* name, std::vector<long>* value);           // 0x005815D6",
   "    template <class T> void TransferVector(const char* name, std::vector<T, std::allocator<T> >& value);   // 0x005815D6 <long> (Ego_r / FableWin ??$TransferVector@J@)"),
(B,'context.TransferVector("PlayersCrimes", &crimes);','context.TransferVector("PlayersCrimes", crimes);',2),
(C,"    template <class T> void Transfer(const char* name, std::vector<T>& value);      // 0x00569F20 / 0x00569FE0",
   "    template <class T> void TransferVector(const char* name, std::vector<T>& value);   // 0x00569F20 / 0x00569FE0 (Ego_r / FableWin ??$TransferVector@M@)"),
(C,'context.Transfer("ActivitiesVec", activities);','context.TransferVector("ActivitiesVec", activities);',2),
(C,'context.Transfer("ScoresVec", scores);','context.TransferVector("ScoresVec", scores);',2),
(E,'context.Transfer("StockItemDefsQuantity",current_stock_item_defs_quantity);','context.TransferVector("StockItemDefsQuantity",current_stock_item_defs_quantity);',2),
(E,'context.Transfer("StockItemDefsMaxStock",current_stock_item_defs_max_stock);','context.TransferVector("StockItemDefsMaxStock",current_stock_item_defs_max_stock);',2),
]
