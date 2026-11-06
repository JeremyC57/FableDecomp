// Behaviour test for CSubtitleRenderer::ProcessEmbeddedHighlightTags (0x00491AD2): a model of the tag pass and the highlight
// placement -- every <tag> becomes 'tag' in the text and joins the action-name list; each name is found after the previous
// match of the same name; after word wrap a highlight split by a line break gets a second (split) highlight on the next line;
// each highlight's index moves past the inserted line breaks before its line is measured.
#include <stdio.h>
#include <string>
#include <vector>
struct Highlight { std::string Text; size_t CharIndex; bool Split; int Line; size_t Column; };
static std::string Process(std::string text, std::vector<std::string>& names)
{
    for (size_t start = text.find('<'); start != std::string::npos; start = text.find('<')) {
        size_t end = text.find('>', start);
        if (end == std::string::npos) break;
        std::string name = text.substr(start + 1, end - start - 1);
        text = text.substr(0, start) + "'" + name + "'" + text.substr(end + 1);
        names.push_back("'" + name + "'");
    }
    return text;
}
static std::vector<Highlight> Place(const std::string& text, const std::vector<std::string>& names, size_t wrapAt)
{
    std::vector<Highlight> out;
    for (size_t i = 0; i < names.size(); ++i) {
        size_t from = 0;
        for (size_t j = 0; j < out.size(); ++j)
            if (out[j].Text == names[i]) from = out[j].CharIndex + 1;
        size_t at = text.find(names[i], from);
        if (at == std::string::npos) break;
        Highlight h = { names[i], at, false, 0, 0 };
        out.push_back(h);
    }
    std::string wrapped;
    for (size_t i = 0; i < text.size(); ++i) { if (i && i % wrapAt == 0) wrapped += '\n'; wrapped += text[i]; }
    size_t n = out.size();
    for (size_t k = 0; k < n; ++k) {
        size_t first = out[k].CharIndex, last = first + out[k].Text.size() - 1;
        if (first / wrapAt != last / wrapAt) {
            size_t cut = (last / wrapAt) * wrapAt;
            Highlight s = { out[k].Text.substr(cut - first), cut, true, 0, 0 };
            out[k].Text = out[k].Text.substr(0, cut - first);
            out.push_back(s);
        }
    }
    for (size_t k = 0; k < out.size(); ++k) {
        size_t breaks = out[k].CharIndex / wrapAt;
        out[k].CharIndex += breaks;
        out[k].Line = (int)breaks;
        out[k].Column = out[k].CharIndex - wrapped.rfind('\n', out[k].CharIndex) - 1;
        if (breaks == 0) out[k].Column = out[k].CharIndex;
    }
    return out;
}
int main()
{
    int ok = 1;
    std::vector<std::string> names;
    std::string t = Process("Press <A> then <B> and <A>", names);
    ok &= t == "Press 'A' then 'B' and 'A'" && names.size() == 3 && names[0] == "'A'" && names[2] == "'A'";
    std::vector<Highlight> h = Place(t, names, 1000);
    ok &= h.size() == 3 && h[0].CharIndex == 6 && h[1].CharIndex == 15 && h[2].CharIndex == 23;
    std::vector<std::string> n2;
    std::string t2 = Process("abcdefg<XYZ>", n2);
    std::vector<Highlight> h2 = Place(t2, n2, 8);
    ok &= h2.size() == 2 && h2[0].Text == "'" && !h2[0].Split && h2[1].Split && h2[1].Text == "XYZ'" && h2[1].Line == 1
          && h2[1].Column == 0 && h2[1].CharIndex == 9;
    printf(ok ? "00491ad2_TEST PASS\n" : "00491ad2_TEST FAIL\n");
    return ok ? 0 : 1;
}
