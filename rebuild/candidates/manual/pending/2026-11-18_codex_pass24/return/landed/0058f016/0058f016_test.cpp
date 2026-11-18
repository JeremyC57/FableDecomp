#include <cassert>
#include <cstdio>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct CategoryFixture {
    std::map<long,long> entries;
    std::string caption, action_template;
    bool viewport_desc, template_component, deleted_arrow, cleared_empty, deleted_icon, cleared_not_usable;
    CategoryFixture() : viewport_desc(false), template_component(false), deleted_arrow(false), cleared_empty(false),
                        deleted_icon(false), cleared_not_usable(false) { }
    void ConstructItemCategoryList(const std::string& title, const std::vector<std::pair<long,long> >& items,
                                   bool ring_pic, bool is_gifts, bool has_targeting, bool live_target, bool arrow_exists, bool icon_exists)
    {
        caption = title;
        if (!items.empty()) {
            long number_of_items = 0;
            for (unsigned c1=0; c1<items.size(); ++c1)
                number_of_items += items[c1].second;
            char count[32];
            std::sprintf(count, " x %ld", number_of_items);
            caption += count;
        }
        viewport_desc = true;
        template_component = ring_pic;
        if (items.empty()) {
            deleted_arrow = arrow_exists;
            cleared_empty = true;
        } else {
            for (unsigned i=0; i<items.size(); ++i) {
                long def = items[i].first;
                std::map<long,long>::iterator findit = entries.find(def);
                if (findit == entries.end())
                    entries.insert(items[i]);
                else {
                    long& item_count = findit->second;
                    item_count += items[i].second;
                }
            }
            bool can_use_gift = true;
            if (is_gifts && has_targeting && !live_target)
                can_use_gift = false;
            action_template = can_use_gift ? "UI_LIST_ITEMS_ACTIONS" : "UI_LIST_ITEMS_ACTIONS_CANT_USE";
            if (!can_use_gift) {
                cleared_not_usable = true;
                deleted_icon = icon_exists;
            }
        }
    }
};

int main()
{
    for (unsigned flags=0; flags<128; ++flags) {
        const bool populated = (flags & 1) != 0;
        std::vector<std::pair<long,long> > items;
        if (populated) {
            items.push_back(std::make_pair(4L,2L));
            items.push_back(std::make_pair(9L,3L));
            items.push_back(std::make_pair(4L,7L));
        }
        CategoryFixture ui;
        ui.ConstructItemCategoryList("Gifts",items,(flags&2)!=0,(flags&4)!=0,(flags&8)!=0,
                                     (flags&16)!=0,(flags&32)!=0,(flags&64)!=0);
        assert(ui.viewport_desc);
        assert(ui.template_component == ((flags&2)!=0));
        const bool cant_use = populated && (flags&28)==12;
        assert(ui.cleared_empty == !populated);
        assert(ui.deleted_arrow == (!populated && (flags&32)));
        assert(ui.deleted_icon == (cant_use && (flags&64)));
        assert(ui.cleared_not_usable == cant_use);
        assert(ui.caption == (populated ? "Gifts x 12" : "Gifts"));
        if (populated) {
            assert(ui.entries.size()==2 && ui.entries.find(4)->second==9 && ui.entries.find(9)->second==3);
            assert(ui.action_template == (cant_use ? "UI_LIST_ITEMS_ACTIONS_CANT_USE" : "UI_LIST_ITEMS_ACTIONS"));
        } else {
            assert(ui.entries.empty() && ui.action_template.empty());
        }
    }
    puts("0058f016_TEST PASS");
    return 0;
}
