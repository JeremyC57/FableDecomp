// Behaviour test for 0x0058F9BC CTCInventory::OpenPCPhotojournal(): the old menu components are deleted, the menu state becomes 5,
// PC_PHOTOJOURNAL_REVIEW is created; the list-position dummy positions the frame when both exist; the screenshots found by the scan
// are keyed by modification time and listed NEWEST FIRST: one menu entry per photo titled with the file's base name (text width 410,
// the bottom-right shape of the button template at x 510, close-menu (274) on back), a photo sub-list whose entry holds a sprite
// (563 x 420, positioned by the frame), a thumbnail (160 x 120) and a description (the title, width 834), the load-photo action
// (3022: sprite, thumbnail, full file name) on select, scroll-down (3023) / scroll-up (3024) actions carrying the list; the scroll
// arrows and the back / close clickables observe mouse movement (25) and are registered with the manager; the review is initialised,
// the list goes to state 5 and the review is kept as a menu component.
#include <cstdio>
#include <string>
#include <vector>
#include <map>

struct Comp {
    std::string name; std::map<std::string, Comp*> children; std::vector<Comp*> positioned, kids, scrolling;
    std::vector<int> back, selected, downActs, upActs; std::vector<std::vector<void*> > selectedParams, downParams, upParams;
    unsigned long width, height; float forced, shapeX; std::wstring value; int observed, initialised, state;
    Comp(const char* n) : name(n), width(0), height(0), forced(0.0f), shapeX(0.0f), observed(0), initialised(0), state(0) {}
    Comp* FindChild(const char* n) { std::map<std::string, Comp*>::iterator it = children.find(n); return it == children.end() ? 0 : it->second; }
};
struct Manager {
    std::map<std::string, std::vector<Comp*> > make; std::vector<Comp*> observers; std::vector<std::wstring> titles;
    Comp* CreateComponent(const char* n) { Comp* c = make[n].front(); make[n].erase(make[n].begin()); return c; }
    Comp* CreateMenuEntry(const std::wstring& title) { titles.push_back(title); return CreateComponent("ENTRY"); }
};
struct Inventory {
    Manager* manager; int deleted; int menu; std::vector<Comp*> components;
    std::map<int, std::wstring> files;   // modification time -> file name (the scan result)
    void OpenPCPhotojournal()
    {
        ++deleted; menu = 5;
        Comp* review = manager->CreateComponent("PC_PHOTOJOURNAL_REVIEW");
        Comp* frame = review->FindChild("PC_UI_PHOTOJOURNAL_FRAME");
        Comp* dummy = review->FindChild("PC_LIST_POSITION_DUMMY");
        if (dummy && frame) dummy->positioned.push_back(frame);
        Comp* list = review->FindChild("PC_PHOTOJOURNAL_LIST");
        for (std::map<int, std::wstring>::reverse_iterator photo = files.rbegin(); photo != files.rend(); ++photo) {
            std::wstring name = photo->second;
            std::wstring title = name.substr(0, name.find(L'.'));
            Comp* entry = manager->CreateMenuEntry(title);
            Comp* text = entry->FindChild("MENU_ENTRY_TEXT");
            if (text) text->forced = 410.0f;
            Comp* button = entry->FindChild("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE");
            if (button) { Comp* shape = button->FindChild("PC_MENU_ENTRY_SHAPE_BOTTOM_RIGHT"); if (shape) shape->shapeX = 510.0f; }
            entry->back.push_back(274);
            Comp* subList = manager->CreateComponent("PC_INDIVIDUAL_PHOTO_SUB_LIST");
            Comp* photoEntry = manager->CreateComponent("PC_INDIVIDUAL_PHOTO_MENU_ENTRY");
            Comp* sprite = photoEntry->FindChild("PC_INDIVIDUAL_PHOTO_SPRITE");
            if (sprite) {
                sprite->width = 563; sprite->height = 420;
                if (frame) frame->positioned.push_back(sprite);
                Comp* thumbnail = manager->CreateComponent("PC_PHOTOJOURNAL_THUMBNAIL");
                if (frame) frame->positioned.push_back(thumbnail);
                thumbnail->width = 160; thumbnail->height = 120;
                review->positioned.push_back(thumbnail);
                entry->kids.push_back(thumbnail);
                std::vector<void*> params; params.push_back(sprite); params.push_back(thumbnail); params.push_back(&files[photo->first]);
                entry->selected.push_back(3022); entry->selectedParams.push_back(params);
                Comp* description = manager->CreateComponent("PC_PHOTOJOURNAL_DESCRIPTION");
                description->value = title; description->forced = 834.0f;
                entry->kids.push_back(description);
                if (frame) frame->positioned.push_back(description);
            }
            subList->scrolling.push_back(photoEntry);
            entry->kids.push_back(subList);
            subList->downActs.push_back(3023); subList->downParams.push_back(std::vector<void*>(1, list));
            subList->upActs.push_back(3024); subList->upParams.push_back(std::vector<void*>(1, list));
            Comp* up = review->FindChild("PC_PHOTOJOURNAL_SCROLL_LIST_UP");
            Comp* down = review->FindChild("PC_PHOTOJOURNAL_SCROLL_LIST_DOWN");
            if (up) { manager->observers.push_back(up); up->observed = 25; }
            if (down) { manager->observers.push_back(down); down->observed = 25; }
            list->scrolling.push_back(entry);
        }
        if (frame) { Comp* pageTitle = review->FindChild("PC_PHOTOJOURNAL_TITLE"); if (pageTitle) frame->positioned.push_back(pageTitle); }
        ++review->initialised;
        Comp* backClickable = list->FindChild("PC_BACK_CLICKABLE_PHOTOJOURNAL");
        if (backClickable) { manager->observers.push_back(backClickable); backClickable->observed = 25; }
        Comp* closeClickable = list->FindChild("PC_CLOSE_CLICKABLE_PHOTOJOURNAL");
        if (closeClickable) { manager->observers.push_back(closeClickable); closeClickable->observed = 25; }
        list->state = 5;
        components.push_back(review);
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); ++fails; } }
int main()
{
    Comp review("REVIEW"), frame("FRAME"), dummy("DUMMY"), list("LIST"), title("TITLE"), up("UP"), back("BACK"), close("CLOSE");
    review.children["PC_UI_PHOTOJOURNAL_FRAME"] = &frame; review.children["PC_LIST_POSITION_DUMMY"] = &dummy;
    review.children["PC_PHOTOJOURNAL_LIST"] = &list; review.children["PC_PHOTOJOURNAL_TITLE"] = &title;
    review.children["PC_PHOTOJOURNAL_SCROLL_LIST_UP"] = &up;
    list.children["PC_BACK_CLICKABLE_PHOTOJOURNAL"] = &back; list.children["PC_CLOSE_CLICKABLE_PHOTOJOURNAL"] = &close;
    Comp e1("ENTRY"), e2("ENTRY"), t1("TEXT"), btn("BUTTON"), shape("SHAPE"), s1("SUB"), s2("SUB"), p1("PE"), p2("PE"), sp1("SPRITE"),
         th1("THUMB"), d1("DESC");
    e1.children["MENU_ENTRY_TEXT"] = &t1; e1.children["PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE"] = &btn;
    btn.children["PC_MENU_ENTRY_SHAPE_BOTTOM_RIGHT"] = &shape; p1.children["PC_INDIVIDUAL_PHOTO_SPRITE"] = &sp1;
    Manager m; m.make["PC_PHOTOJOURNAL_REVIEW"].push_back(&review);
    m.make["ENTRY"].push_back(&e1); m.make["ENTRY"].push_back(&e2);
    m.make["PC_INDIVIDUAL_PHOTO_SUB_LIST"].push_back(&s1); m.make["PC_INDIVIDUAL_PHOTO_SUB_LIST"].push_back(&s2);
    m.make["PC_INDIVIDUAL_PHOTO_MENU_ENTRY"].push_back(&p1); m.make["PC_INDIVIDUAL_PHOTO_MENU_ENTRY"].push_back(&p2);
    m.make["PC_PHOTOJOURNAL_THUMBNAIL"].push_back(&th1); m.make["PC_PHOTOJOURNAL_DESCRIPTION"].push_back(&d1);
    Comp old("OLD"); Inventory inv; inv.manager = &m; inv.deleted = 0; inv.menu = 0; inv.components.push_back(&old);
    inv.files[100] = L"older.jpg"; inv.files[200] = L"newer.jpg";
    inv.OpenPCPhotojournal();
    check(inv.deleted == 1 && inv.menu == 5, "delete / menu");
    check(dummy.positioned.size() == 1 && dummy.positioned[0] == &frame, "dummy positions frame");
    check(m.titles.size() == 2 && m.titles[0] == L"newer" && m.titles[1] == L"older", "newest first, base names");
    check(t1.forced == 410.0f && shape.shapeX == 510.0f && e1.back.size() == 1 && e1.back[0] == 274, "entry text / shape / back");
    check(sp1.width == 563 && sp1.height == 420 && th1.width == 160 && th1.height == 120, "sprite / thumbnail sizes");
    check(frame.positioned.size() == 4 && frame.positioned[0] == &sp1 && frame.positioned[1] == &th1 && frame.positioned[2] == &d1 &&
          frame.positioned[3] == &title, "frame positions sprite, thumbnail, description, title");
    check(review.positioned.size() == 1 && review.positioned[0] == &th1, "review positions thumbnail");
    check(e1.selected.size() == 1 && e1.selected[0] == 3022 && e1.selectedParams[0][0] == &sp1 && e1.selectedParams[0][1] == &th1 &&
          *(std::wstring*)e1.selectedParams[0][2] == L"newer.jpg", "load action");
    check(d1.value == L"newer" && d1.forced == 834.0f, "description");
    check(e1.kids.size() == 3 && e1.kids[2] == &s1 && e2.kids.size() == 1 && e2.kids[0] == &s2 && e2.selected.empty(), "entry children");
    check(s1.scrolling.size() == 1 && s1.scrolling[0] == &p1 && s1.downActs[0] == 3023 && s1.upActs[0] == 3024 &&
          s1.downParams[0][0] == &list && s2.upParams[0][0] == &list, "sub-list scroll actions");
    check(up.observed == 25 && back.observed == 25 && close.observed == 25 && m.observers.size() == 4, "observers");
    check(list.scrolling.size() == 2 && list.scrolling[0] == &e1 && list.state == 5 && review.initialised == 1, "list / review");
    check(inv.components.size() == 2 && inv.components[1] == &review, "kept component");
    if (fails == 0) std::printf("0058f9bc_TEST PASS\n");
    return fails;
}
