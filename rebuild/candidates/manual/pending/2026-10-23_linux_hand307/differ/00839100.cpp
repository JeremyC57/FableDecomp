// Grade: DIFFER(844v795) -- the trail-def pointer: retail keeps the out-filled def in edi (its stack slot is reused for the reverse iterator) and releases it in place (dec [edi+4]) on most exits, reloading the slot only on the charge exits; our CDefPointer stays address-taken (reloads after the decrement, extra frame word), which shifts every frame slot and the loop's register use
// CThingGameObject::GetWeaponTrailGraphicIndex() const -- the trail graphic for this weapon: from its trail definition,
// picking the special-move variant when the wielder (the carrier, if carried) is doing a flourish/charge/special action;
// a charging wielder uses its charge component's graphic, a wielder with an active, non-suppressed hand component uses
// the def's plain graphic if set, otherwise the first augmentation (walking the def's augmentation map backwards) the
// weapon carries, else the map's default (0) entry; -1 when there is no trail def or no entry
// Retail: Fable.exe (TLC, VC7.1) @ 0x00839100, `ret`, long in eax.
// Layout (retail; Ego_r.pdb in brackets): CThing interface bitset +0x20, map +0x44, ThingTypeFlags +0x6c [+0x7c],
//   PBaseDef +0x70 [+0x80]; creature current action +0x100; trail def graphics +0x7c / +0x80, augmentation map +0x84
//   (STLport map<long, pair of graphic indices>: key at node +0x10, graphics at +0x14 / +0x18).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CDefBase {
    virtual void v00();
    virtual void DeleteThis();                       // slot 1
    long RefCount;                                   // +0x04
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object) {
            --Object->RefCount;
            if (Object->RefCount == 0)
                Object->DeleteThis();
        }
    }
    T* operator->() const { return Object; }
};
struct STrailGraphics { long Normal; long Special; };
struct SNodeBase {
    long       _M_color;
    SNodeBase* _M_parent;
    SNodeBase* _M_left;
    SNodeBase* _M_right;
};
struct SNode : SNodeBase {
    long           Key;                              // +0x10
    STrailGraphics Graphics;                         // +0x14
};
struct CTrailMapIterator {
    SNodeBase* _M_node;
    CTrailMapIterator& operator--();                 // 0x0083AEA0
    SNode& operator*() const { return *(SNode*)_M_node; }
};
struct CTrailMapReverseIterator {
    CTrailMapIterator current;
    SNode& operator*() const
    {
        CTrailMapIterator tmp = current;
        return *--tmp;
    }
    CTrailMapReverseIterator& operator++()
    {
        --current;
        return *this;
    }
};
struct CTrailMap {
    SNodeBase* _M_header;
    long       _M_node_count;
    CTrailMapIterator find(const long& key) const;   // 0x0083B170
};
#pragma auto_inline(off)
__declspec(noinline) CTrailMapIterator& CTrailMapIterator::operator--()
{
    if (_M_node->_M_color == 0 && _M_node->_M_parent->_M_parent == _M_node)
        _M_node = _M_node->_M_right;
    else if (_M_node->_M_left != 0) {
        SNodeBase* y = _M_node->_M_left;
        while (y->_M_right != 0)
            y = y->_M_right;
        _M_node = y;
    } else {
        SNodeBase* y = _M_node->_M_parent;
        while (_M_node == y->_M_left) {
            _M_node = y;
            y = y->_M_parent;
        }
        _M_node = y;
    }
    return *this;
}
__declspec(noinline) CTrailMapIterator CTrailMap::find(const long& key) const
{
    SNodeBase* y = _M_header;
    SNodeBase* x = _M_header->_M_parent;
    while (x != 0) {
        if (!(((SNode*)x)->Key < key)) {
            y = x;
            x = x->_M_left;
        } else
            x = x->_M_right;
    }
    CTrailMapIterator it;
    it._M_node = (y == _M_header || key < ((SNode*)y)->Key) ? _M_header : y;
    return it;
}
#pragma auto_inline(on)
struct CWeaponTrailDef : CDefBase {
    unsigned char _pad08[0x7c - 0x08];
    long      NormalGraphic;                         // +0x7c
    long      SpecialGraphic;                        // +0x80
    CTrailMap Augmentations;                         // +0x84
};
struct CThingBaseDef {
    bool GetWeaponTrailDef(CDefPointer<CWeaponTrailDef>* out) const;   // 0x0042B1A6
};
struct CThing;
struct CTCCarryable {                                // interface 0x47
    unsigned char _pad00[0x10];
    CThing* Carrier;                                 // +0x10
    CThing* GetCarrier() const;                      // 0x00705170
};
struct CTCSpecialMove {                              // interface 0x7d (descriptive)
    bool IsSpecialMoveActive() const;                // 0x00787C18
};
struct CTCCharge {                                   // interface 0x84 (descriptive)
    long GetTrailGraphic() const;                    // 0x0078BED0
    long GetSpecialTrailGraphic() const;             // 0x0078BEE0
};
struct CTCHand {                                     // interface 0x30 (descriptive)
    unsigned char _pad00[0x94];
    bool Suppressed;                                 // +0x94
    bool IsActive() const;                           // 0x00733EE0
};
struct CTCAugmentations {                            // interface 0xbf (descriptive)
    bool HasAugmentation(long type) const;           // 0x00766050
};
struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool IsSpecialA();                       // slot 18 (+0x48)
    virtual bool IsKnockDown();                      // slot 19 (+0x4c)
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual bool IsDecapitate();                     // slot 54 (+0xd8)
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                  // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                    // +0x6c
    const CThingBaseDef* PBaseDef;                   // +0x70
    unsigned char _pad74[0x100 - 0x74];
    CCreatureActionBase* CurrentAction;              // +0x100 (creatures only)
    template<class T> __forceinline bool GetInterface(const T** out, long wanted) const
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingGameObject : CThing {
    long GetWeaponTrailGraphicIndex() const;
};
long CThingGameObject::GetWeaponTrailGraphicIndex() const
{
    CDefPointer<CWeaponTrailDef> def;
    if (!PBaseDef->GetWeaponTrailDef(&def))
        return -1;
    const CThing* wielder = this;
    const CTCCarryable* carryable;
    if (GetInterface(&carryable, 0x47) && carryable->Carrier)
        wielder = carryable->GetCarrier();
    bool special = false;
    if (wielder->ThingTypeFlags & 8) {
        CCreatureActionBase* action = wielder->CurrentAction;
        if (action && (action->IsKnockDown() || action->IsSpecialA() || action->IsDecapitate()))
            special = true;
    } else {
        const CTCSpecialMove* move;
        if (GetInterface(&move, 0x7d) && move->IsSpecialMoveActive())
            special = true;
    }
    const CTCCharge* charge;
    if (wielder->GetInterface(&charge, 0x84)) {
        if (special)
            return charge->GetSpecialTrailGraphic();
        return charge->GetTrailGraphic();
    }
    const CTCHand* hand;
    if (wielder->GetInterface(&hand, 0x30) && hand->IsActive() && !hand->Suppressed) {
        long graphic = special ? def->SpecialGraphic : def->NormalGraphic;
        if (graphic > 0)
            return graphic;
    }
    const CTCAugmentations* augmentations;
    if (GetInterface(&augmentations, 0xbf)) {
        CTrailMapReverseIterator it;
        it.current._M_node = def->Augmentations._M_header;
        CTrailMapReverseIterator end;
        end.current._M_node = def->Augmentations._M_header->_M_left;
        for (; it.current._M_node != end.current._M_node; ++it) {
            long key = (*it).Key;
            if (key && augmentations->HasAugmentation(key)) {
                if (special)
                    return (*it).Graphics.Special;
                return (*it).Graphics.Normal;
            }
        }
    }
    long none = 0;
    CTrailMapIterator entry = def->Augmentations.find(none);
    if (entry._M_node == def->Augmentations._M_header)
        return -1;
    if (special)
        return (*entry).Graphics.Special;
    return (*entry).Graphics.Normal;
}
