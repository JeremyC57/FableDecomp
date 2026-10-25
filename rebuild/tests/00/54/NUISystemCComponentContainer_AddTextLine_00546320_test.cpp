// Behaviour test for NUISystem::CComponentContainer::AddTextLine (0x00546320): the text component gets its value and
// colour (and a 0..indent x 0..100 text window only for a non-zero indent); without a sprite the text is added as the
// container's child; with a sprite index a sprite-container holding the sprite (graphic set) then the text is added,
// the text moved to (sprite width + 4, 4) -- through a copy of the body over recording stand-in components.
#include <cstdio>
#include <string>
#include <vector>

struct Box { float TLX, TLY, BRX, BRY; };

struct Component {
    std::string templateName;
    std::vector<Component*> children;
    // text
    std::string value;
    bool hasWindow;
    Box window;
    unsigned int colour;
    float x, y;
    // sprite
    unsigned int graphic;
    unsigned int width;
    Component() : hasWindow(false), colour(0), x(0.0f), y(0.0f), graphic(0), width(0) { }
    void AddChild(Component* child) { children.push_back(child); }
    void SetTextWindow(const Box& box) { hasWindow = true; window = box; }
    void SetValue(const std::string& text) { value = text; }
    void SetGraphicIndex(unsigned int index) { graphic = index; width = 10 * index; }
    unsigned int GetWidth() const { return width; }
};

struct Manager {
    std::vector<Component*> made;
    ~Manager()
    {
        for (unsigned int i = 0; i < made.size(); ++i)
            delete made[i];
    }
    Component* CreateComponent(const char* name)
    {
        Component* c = new Component;
        c->templateName = name;
        made.push_back(c);
        return c;
    }
};

static void AddTextLine(Manager& manager, Component& self, const std::string& text, unsigned int colour,
                        unsigned int spriteIndex, int indent)
{
    Component* textLine = manager.CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE");
    Box window = { 0.0f, 0.0f, (float)indent, 100.0f };
    if (indent != 0)
        textLine->SetTextWindow(window);
    textLine->SetValue(text);
    textLine->colour = colour;
    if (spriteIndex != 0) {
        Component* sprite = manager.CreateComponent("UI_TEXT_SPRITE_CONTAINER_SPRITE");
        sprite->SetGraphicIndex(spriteIndex);
        float x = (float)sprite->GetWidth() + 4.0f;
        textLine->x = x;
        textLine->y = 4.0f;
        Component* container = manager.CreateComponent("UI_TEXT_SPRITE_CONTAINER");
        container->AddChild(sprite);
        container->AddChild(textLine);
        self.AddChild(container);
    }
    else
        self.AddChild(textLine);
}

int main()
{
    Manager manager;
    Component self;
    AddTextLine(manager, self, "Plain", 0xff00ff00u, 0, 0);
    if (self.children.size() != 1 || self.children[0]->templateName != "UI_TEXT_INSIDE_CONTAINER_TEMPLATE" ||
        self.children[0]->value != "Plain" || self.children[0]->colour != 0xff00ff00u || self.children[0]->hasWindow) {
        std::printf("00546320_TEST FAIL\n");
        return 1;
    }
    AddTextLine(manager, self, "Indented", 0x12345678u, 0, 30);
    Component* indented = self.children[1];
    if (!indented->hasWindow || indented->window.BRX != 30.0f || indented->window.BRY != 100.0f ||
        indented->window.TLX != 0.0f || indented->window.TLY != 0.0f) {
        std::printf("00546320_TEST FAIL\n");
        return 1;
    }
    AddTextLine(manager, self, "Iconic", 0x0u, 3, 0);
    Component* container = self.children[2];
    if (self.children.size() != 3 || container->templateName != "UI_TEXT_SPRITE_CONTAINER" ||
        container->children.size() != 2 || container->children[0]->templateName != "UI_TEXT_SPRITE_CONTAINER_SPRITE" ||
        container->children[0]->graphic != 3 || container->children[1]->value != "Iconic" ||
        container->children[1]->x != 34.0f || container->children[1]->y != 4.0f) {
        std::printf("00546320_TEST FAIL\n");
        return 1;
    }
    std::printf("00546320_TEST PASS\n");
    return 0;
}